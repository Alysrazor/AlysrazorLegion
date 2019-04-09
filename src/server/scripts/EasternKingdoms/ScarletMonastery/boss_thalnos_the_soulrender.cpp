/*
 * Copyright (C) 2019 DrustWoW Servers
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "MotionMaster.h"
#include "scarlet_monastery.h"

enum Says
{
    SAY_INTRO       = 0, // My endless agony shall be yours, as well! (27832)
    SAY_AGGRO       = 1, // We hunger for vengeance! (27829)
    SAY_RISE_FALLEN = 2, // No rest...for the angry dead! (27831)
    SAY_SUM_SPIRIT  = 3, // Claim a body, and wreak terrible vengeance! (27835)
    SAY_EVICT_SOUL  = 4, // Seek out a vessel...and return! (27834)
    SAY_KILL        = 5, // More, more souls! (27833)
    SAY_DIED        = 6  // Can this be...the end, at last...? (27830)
};

enum Spells
{
	//Thalnos
	SPELL_EVICT_SOUL            = 115297,
    SPELL_RAISE_FALLEN          = 115139,
    SPELL_SPIRIT_GALE           = 115289,
    SPELL_SUMMON_SPIRITS        = 115147,
    SPELL_SUMMON_SPIRITS_VISUAL = 115149, 
	
	//Empowering Spirit
	SPELL_EMPOWERING_SPIRIT = 115157,
	
	//Fallen Crusader
	SPELL_MIND_ROOT 		= 115144,

    //Evicted Soul
    SPELL_EVICTED_SOUL      = 115309
};

enum Events
{
    //Thalnos
	EVENT_EVICT_SOUL = 1,
	EVENT_RAISE_FALLEN,
	EVENT_SPIRIT_GALE,
	EVENT_SUMMON_SPIRITS,
    EVENT_SUMMON_SPIRITS_VISUAL,

    //Empowering Spirit
    EVENT_EMPOWERING_SPIRIT,

    //Fallen Crusader

    //Evicted Soul
    EVENT_EVICTED_SOUL
};

enum NPCs
{
    NPC_EVICTED_SOUL      = 59974,
	NPC_FALLEN_CRUSADER   = 59884,
	NPC_EMPOWERING_SPIRIT = 59893,
    NPC_EMPOWERED_ZOMBIE  = 59930
};

class boss_thalnos_the_soulrender : public CreatureScript
{
public:
    boss_thalnos_the_soulrender() : CreatureScript("boss_thalnos_the_soulrender") { }

    struct boss_thalnos_the_soulrenderAI : public BossAI
    {
        boss_thalnos_the_soulrenderAI(Creature* creature) : BossAI(creature, DATA_THALNOS) { }

        bool _intro;

        void Reset() override
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.ScheduleEvent(EVENT_EVICT_SOUL, Milliseconds(15500));
            events.ScheduleEvent(EVENT_RAISE_FALLEN, Seconds(6));
            events.ScheduleEvent(EVENT_SUMMON_SPIRITS, Minutes(1));
            events.ScheduleEvent(EVENT_SPIRIT_GALE, Seconds(10)); // Not right timer.
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_EMPOWERING_SPIRIT)
            {
                Unit* fallen = summon->FindNearestCreature(NPC_FALLEN_CRUSADER, 100.0f);

                if (!fallen || fallen->IsAlive())
                    return;

                summon->GetMotionMaster()->MoveCloserAndStop(1, fallen, 3.0f);
                summon->CastSpell(summon, SPELL_EMPOWERING_SPIRIT);
            }
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who && who->GetTypeId() == TYPEID_PLAYER && me->IsValidAttackTarget(who))
                if (!_intro && me->IsWithinDistInMap(who, 30.0f))
                {
                    _intro = true;
                    Talk(SAY_INTRO);
                    BossAI::MoveInLineOfSight(who);
                }
        }

        void KilledUnit(Unit* victim) override 
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_KILL);
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
            {
                Talk(SAY_DIED);
                summons.DespawnAll();
            }
        }

        void EnterEvadeMode(EvadeReason /*why*/) override
        {
            summons.DespawnAll();
            _DespawnAtEvade();
        }

        void JustReachedHome() override
        {
            //Add Custom Visual Spell
            events.ScheduleEvent(EVENT_SUMMON_SPIRITS_VISUAL, Seconds(1));
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_EVICT_SOUL:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_BOTTOMAGGRO))
                            DoCast(target, SPELL_EVICT_SOUL);
                        Talk(SAY_EVICT_SOUL);
                        events.Repeat(Seconds(41));
                        break;
                    }
                    case EVENT_RAISE_FALLEN:
                    {
                        DoCast(me, SPELL_RAISE_FALLEN);
                        Talk(SAY_RISE_FALLEN);
                        events.Repeat(Minutes(1));
                        break;
                    }
                    case EVENT_SUMMON_SPIRITS:
                    {
                        DoCast(me, SPELL_SUMMON_SPIRITS);
                        Talk(SAY_SUM_SPIRIT);
                        events.Repeat((Seconds(55), Minutes(1)));
                        break;
                    }
                    case EVENT_SPIRIT_GALE:
                    {
                        DoCastVictim(SPELL_SPIRIT_GALE);
                        Talk(SAY_SUM_SPIRIT);
                        events.Repeat((Seconds(15), Seconds(30)));
                        break;
                    }
                    case EVENT_SUMMON_SPIRITS_VISUAL:
                    {
                        if (!me->IsInCombat())
                        {
                            DoCast(me, SPELL_SUMMON_SPIRITS_VISUAL);
                            me->PlayOneShotAnimKitId(ANIM_CHANNEL_CAST_OMNI);
                            events.Repeat(Seconds(5));
                        }

                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetScarletMonasteryAI<boss_thalnos_the_soulrenderAI>(creature);
    }
};

class npc_empowering_spirit : public CreatureScript
{
public:
	npc_empowering_spirit() : CreatureScript("npc_empowering_spirit") { }
};

class npc_fallen_crusader : public CreatureScript
{
public:
    npc_fallen_crusader() : CreatureScript("npc_fallen_crusader") { }

    struct npc_fallen_crusaderAI : public ScriptedAI
    {
        npc_fallen_crusaderAI(Creature* creature) : ScriptedAI(creature) { }

        bool _died = false;
        Unit* spirit = me->FindNearestCreature(NPC_EMPOWERING_SPIRIT, 3.0f);

        void JustDied(Unit* killer) override
        {
            _died = true;
        }

        void DoFallenCrusaderAttackIfReady()
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            Unit* victim = me->GetVictim();
            if (me->isAttackReady(BASE_ATTACK) && me->IsWithinMeleeRange(victim))
            {
                me->AttackerStateUpdate(victim);
                me->resetAttackTimer();
                if (GetDifficulty() == DIFFICULTY_HEROIC || DIFFICULTY_FLAG_CHALLENGE_MODE)
                    me->CastSpell(victim, SPELL_MIND_ROOT, true);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;


            if (_died && spirit && spirit->HasAura(SPELL_EMPOWERING_SPIRIT))
            {
                spirit->SetVisible(false);
                spirit->KillSelf();
                spirit->SummonCreature(NPC_EMPOWERED_ZOMBIE, me->GetPosition(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT);
                me->SetVisible(false);
            }
            DoFallenCrusaderAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetScarletMonasteryAI<npc_fallen_crusaderAI>(creature);
    }
};

void AddSC_boss_thalnos_the_soulrender()
{
    new boss_thalnos_the_soulrender();

    new npc_fallen_crusader(); 
}
