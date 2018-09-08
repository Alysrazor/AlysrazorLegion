/*
* Created by Alysrazor -- ES Community Manager
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



#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "InstanceScript.h"
#include "ObjectAccessor.h"
#include "SpellScript.h"
#include "tomb_of_sargeras.h"

//constant positions about teleports and Kasparian Jumps
Position const LunaspyreTeleports[5] =
{
	{},
    {},
    {},
	{},
	{},
};

Position const CenterCombat[1] =
{
	{},
};

Position const YathaeTeleports[5] =
{
	{},
    {},
    {},
	{},
	{},
};

Position const KasparianJumps[5] =
{
	{},
    {},
    {},
	{},
	{},
};


enum Says
{
    //Sisters
    SAY_SISTERS_AGGRO     = 1,
    SAY_SISTERS_DEATH     = 2,
    SAY_SISTERS_EVADE     = 3,
	SAY_SISTER_KILLS      = 4,
    //Kasparian 
    SAY_SPELL_GLAIVE      = 5,
	SAY_KASPARIAN_KILLS   = 6,
    //Yathae
    SAY_PHASE_COMBAT      = 7,
    SAY_SPELLS_ARROW      = 8,
    SAY_SPELL_AOE         = 9,
	SAY_YATHAE_KILLS      = 10,
    //Lunaspyre
    SAY_SPELL             = 11,
    SAY_FOUNTAIN_OF_ELUNE = 12,
	SAY_LUNASPYRE_KILLS   = 13
};

enum SistersSpells
{
    //Generic
    SPELL_QUIET_SUICIDE = 3617, // Serverside spell
    SPELL_INCORPOREAL_TELEPORT = 236115, //Cast
    SPELL_INCORPOREAL_TELEPORT_2 = 236224, //Not sure if this is the teleport or need script. Instant cast. These two teleport spells are the same for Lunaspyre
    SPELL_COOLDOWN_CREATURE_SPECIAL = 61207, //To block other spells from interfering with their special
	SPELL_GHOST_AURA = 235268, //Cosmetic for Sisters
    //Kasparian
    SPELL_TWILIGHT_GLAIVE_AT = 236529,// AT 9785 and add aura to bunny + make bunny got target,then return
    SPELL_TWILIGHT_GLAIVE_DAMAGE = 236541,
    SPELL_TWILIGHT_GLAIVE_DUMMY = 237561,
    SPELL_MOON_GLAIVE = 236547,
    SPELL_DISCORPORATE = 236550, //Astral Purge will remove it.
    SPELL_GLAIVE_STORM = 239379, //Only used in Elune's Fountain Phase 3 (normal), phase 2 and 3 heroic and mythic
    //Yathae
    SPELL_SHADOW_SHOT = 237630,
    SPELL_INCORPOREAL_SHOT_CAST = 236305,//Only used in Elune's Fountain Phase 2 (normal), phase 1 and 3 heroic and mythic
    SPELL_INCORPOREAL_SHOT_DAMAGE = 236306,
    SPELL_CALL_MOONTALON = 236694,
    SPELL_TWILIGHT_VOLLEY_AT = 236442,// AT 9777
    SPELL_TWILIGHT_VOLLEY_DAMAGE = 236516, //Apply this aura OnUnitEnter();
    SPELL_RAPID_SHOT = 236598,
    SPELL_RAPID_SHOT_AURA = 236596,
    //Lunaspyre
    SPELL_EMBRACE_OF_THE_ECLIPSE_BOSS = 233263,//Shields Heal
    SPELL_EMBRACE_OF_THE_ECLIPSE_BOSS_2 = 233264,//Shield Boss
    SPELL_LUNAR_BEACON = 236712,//OnRemove(); actives Lunar barrage.
    SPELL_LUNAR_FIRE = 239264,//Only on current target,active.
    SPELL_LUNAR_STRIKE = 237632,
    SPELL_MOON_BURN = 236518,//Astral Purge will remove this.
    SPELL_LUNAR_BARRAGE_AT = 236726,//AT 9807
    SPELL_LUNAR_BARRAGE_DAMAGE = 237351,
};

enum SistersEvents
{
    //General
    EVENT_INCORPOREAL_TELEPORT = 1,
    EVENT_TELEPORT_CENTER_COMBAT,
    EVENT_BACK_TO_GHOST,
    EVENT_BERSERKER,
    //Kasparian
    EVENT_TWILIGHT_GLAIVE,
    EVENT_MOON_GLAIVE,
    EVENT_GLAIVE_STORM,
    EVENT_JUMP_TIGER,
    //Yathae
    EVENT_SHADOW_SHOT,
    EVENT_INCORPOREAL_SHOT,
    EVENT_CALL_MOONTALON,
    EVENT_TWILIGHT_VOLLEY,
    EVENT_RAPID_SHOT,
    //Lunaspyre
    EVENT_EMBRACE_OF_THE_ECLIPSE,
    EVENT_LUNAR_BEACON,
    EVENT_LUNAR_FIRE,
    EVENT_MOON_BURN
};

enum Phases
{
    THE_HUNTRESS   = 1,
    THE_CAPTAIN    = 2,
    THE_PRIESTESS  = 3,
	SISTER_GHOST   = 4,
};

uint32 const SistersData[3]
{
    DATA_HUNTRESS_KASPARIAN,
    DATA_PRIESTESS_LUNASPYRE,
	DATA_CAPTAIN_YATHAE_MOONSTRIKE,
};

enum SistersMisc
{
    SUMMON_SISTERS_GROUP = 1,
};

static uint32 GetRandomBossExcept(uint32 exception)
{
	std::vector<uint32> bossData;
	for (uint32 data : SistersData)
		if (data != exception)
			bossData.emplace_back(data);
	
	return bossData[urand(0,2)];
}

class boss_sisters_of_the_moon : public CreatureScript
{
public:
   boss_sisters_of_the_moon() : CreatureScript("boss_sisters_of_the_moon") { }
   
   struct boss_sisters_of_the_moonAI : public BossAI
   {
	   boss_sisters_of_the_moonAI(Creature* creature) : BossAI(creature, DATA_SISTERS_OF_THE_MOON), _inCombat(false) { }
	   
	   void Reset() override
	   {
		   _Reset();
		   _inCombat = false;
		   me->SummonCreatureGroup(SUMMON_SISTERS_GROUP);
		   instance->SetBossState(DATA_SISTERS_OF_THE_MOON, NOT_STARTED);
	   }
	   
	   void EnterCombat(Unit* /*who*/) override
	   {
		   if (!_inCombat)
		   {
			   _inCombat = true;
			   _EnterCombat();
			   for (uint32 bossData : SistersData)
			   {
				   if (Creature* sisters = instance->GetCreature(bossData))
				   {
                       instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, sisters);
					   DoZoneInCombat(sisters);
			       }
			   }
			   events.ScheduleEvent(EVENT_BERSERKER, Minutes(8));
			   if (Creature* sisters = instance->GetCreature(SistersData[urand(0,2)]))
				   sisters->AI()->Talk(SAY_SISTERS_AGGRO);
		   }
	   }
	   
	   void EnterEvadeMode(EvadeReason /*why*/) override
	   {
		   if (!me->IsInEvadeMode())
	       {
			   _inCombat = false;
			   for (uint32 bossData : SistersData)
				   if (Creature* sisters = instance->GetCreature(bossData))
					   instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, sisters);
			   
			   summons.DespawnAll();
			   //DespawnAllAT
			   _DespawnAtEvade();
			   instance->SetBossState(DATA_SISTERS_OF_THE_MOON, FAIL);
		   }
	   }
	   
	   void JustDied(Unit* /*killer*/) override
	   {
		   _inCombat = false;
		   events.Reset();
		   instance->SetBossState(DATA_SISTERS_OF_THE_MOON,	DONE);
		   
		   for (uint32 bossData : SistersData)
		   {
			   if (Creature* sisters = instance->GetCreature(bossData))
			   {
                   instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, sisters);
				   sisters->LowerPlayerDamageReq(sisters->GetMaxHealth());
				   sisters->CastSpell(sisters, SPELL_QUIET_SUICIDE, true);
			   }
			   if (Creature* sistersDeath = instance->GetCreature(SistersData[urand(0,2)]))
				   sistersDeath->AI()->Talk(SAY_SISTERS_DEATH);
		   }
	   }
	   
	   private:
	      bool _inCombat;
   };
   
   CreatureAI* GetAI(Creature* creature) const override
   {
	   return GetTombOfSargerasAI<boss_sisters_of_the_moonAI>(creature);
   }
};


class boss_huntress_kasparian : public CreatureScript
{
public:
   boss_huntress_kasparian() : CreatureScript("boss_huntress_kasparian") { }
   
   struct boss_huntress_kasparianAI : public BossAI
   {
	   boss_huntress_kasparianAI(Creature* creature) : BossAI(creature, DATA_HUNTRESS_KASPARIAN) 
	   {
		   StartEncounter();
		   HealthBound(1000, 1000);
	   }

       void StartEncounter()
	   {
		   _Reset();
		   me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		   me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		   events.SetPhase(THE_HUNTRESS);
	   }
	   
	   void EnterCombat(Unit* /*who*/) override
	   {
		   me->setActive(true);
		   DoZoneInCombat();
		   HealthBound(1000, 1000);
           Creature* sistersController = instance->GetCreature(NPC_SISTERS_OF_THE_MOON);
           sistersController->SetInCombatWith(me);
		   
		   events.SetPhase(THE_HUNTRESS);
		   events.ScheduleEvent(EVENT_MOON_GLAIVE, Seconds(17), THE_HUNTRESS);
		   events.ScheduleEvent(EVENT_TWILIGHT_GLAIVE, Seconds(22));
	   }
	   
	   void EnterEvadeMode(EvadeReason /*why*/) override
	   {
           if (events.IsInPhase(THE_HUNTRESS))
               Talk(SAY_SISTERS_EVADE);
		   summons.DespawnAll();
		   _DespawnAtEvade();
	   }
	   
	   void KilledUnit(Unit* victim) override
	   {
		   if (victim->GetTypeId() == TYPEID_PLAYER && !me->IsInEvadeMode() && events.IsInPhase(THE_HUNTRESS))
		       Talk(SAY_KASPARIAN_KILLS);
		   
		   else if (events.IsInPhase(THE_CAPTAIN))
		   {
			   Creature* yathae = instance->GetCreature(DATA_CAPTAIN_YATHAE_MOONSTRIKE);
			   yathae->AI()->Talk(SAY_SISTER_KILLS);
		   }
		   
		   else if (events.IsInPhase(THE_PRIESTESS))
		   {
			   Creature* lunaspyre = instance->GetCreature(DATA_PRIESTESS_LUNASPYRE);
			   lunaspyre->AI()->Talk(SAY_SISTER_KILLS);
		   }
	   }
	   
	   void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
	   {
		   if (events.IsInPhase(THE_HUNTRESS) && !HealthAbovePct(70))
		   {
			   events.SetPhase(SISTER_GHOST);
			   //TODO: Apply not attackable and selectable, also add ghost aura and react passive?
			   me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			   me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			   me->AddAura(SPELL_GHOST_AURA, me);
			   return;
		   }		 
	   }
	   
	   void JustSummoned(Creature* summon) override
	   {
		   switch (summon->GetEntry())
		   {
			   case NPC_TWILIGHT_GLAIVE_BUNNY:
                   summon->CastSpell(summon, SPELL_TWILIGHT_GLAIVE_DUMMY, true);
                   summon->SetReactState(REACT_PASSIVE);
			   // case NPC_GLAIVE_STORM_DUMMY:
			    break;
			 default:
			    break;
		   }
         BossAI::JustSummoned(summon);
       }

	   
	   void HealthBound(uint32 diff, uint32 HPUpdate) 
	   {
		   if (events.IsInPhase(THE_HUNTRESS))
		   {
			   Creature* yathae = instance->GetCreature(DATA_CAPTAIN_YATHAE_MOONSTRIKE);
			   Creature* lunaspyre = instance->GetCreature(DATA_PRIESTESS_LUNASPYRE);
			   HPUpdate = 1000;
			   
			   if (HPUpdate <= diff)
			   {
                   uint32 kasparianHP = me->GetHealth();
				   yathae->SetHealth(kasparianHP);
				   lunaspyre->SetHealth(kasparianHP);
			   }
			   else HPUpdate -= diff;
		   }
	   }
				   
	   void UpdateAI(uint32 diff) override
	   {
		   if (!UpdateVictim())
			   return;
		   
		   events.Update(diff);
		   
		   if (me->HasUnitState(UNIT_STATE_CASTING))
			   return;
		   
		   while (uint32 eventId = events.ExecuteEvent())
		   {
			   
		      switch (eventId)
		      {
			      case EVENT_MOON_GLAIVE:
			      {
					 DoCastVictim(SPELL_MOON_GLAIVE);
				     events.Repeat(Seconds(17));
				  }
				  break;
			   
			      case EVENT_TWILIGHT_GLAIVE:
			           if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
						   DoCast(target, SPELL_TWILIGHT_GLAIVE_AT);
					   events.Repeat(Seconds(22));
				  break;
			      case EVENT_GLAIVE_STORM:
			           //TODO
			      break;
			   default:
			      break;
			  }
		   }
           if (events.IsInPhase(THE_HUNTRESS))
               DoMeleeAttackIfReady();
           else DoSpellAttackIfReady(SPELL_MOON_GLAIVE);
           
	   }
	   
   };
   
   CreatureAI* GetAI(Creature* creature) const override
   {
	   return GetTombOfSargerasAI<boss_huntress_kasparianAI>(creature);
   }
};
		   
class boss_captain_yathae_moonstrike : public CreatureScript
{
public:
    boss_captain_yathae_moonstrike() : CreatureScript("boss_captain_yathae_moonstrike") { }

    struct boss_captain_yathae_moonstrikeAI : public BossAI
    {
        boss_captain_yathae_moonstrikeAI(Creature* creature) : BossAI(creature, DATA_CAPTAIN_YATHAE_MOONSTRIKE)
        {
            StartEncounter();
            HealthBound(1000, 1000);
        }

        void StartEncounter()
        {
            _Reset();
            me->SetReactState(REACT_PASSIVE);
            events.SetPhase(SISTER_GHOST);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            me->setActive(true);
            DoZoneInCombat();
            HealthBound(1000, 1000);

            events.SetPhase(SISTER_GHOST);
            events.ScheduleEvent(EVENT_TWILIGHT_VOLLEY, Seconds(15));
            events.ScheduleEvent(EVENT_CALL_MOONTALON, Seconds(40), 0, THE_CAPTAIN);
            //events.ScheduleEvent(EVENT_INCORPOREAL_TELEPORT, Seconds(urand(30, 38)), 0, SISTER_GHOST);
            events.ScheduleEvent(EVENT_RAPID_SHOT, Seconds(25), 0, THE_CAPTAIN);
            events.ScheduleEvent(EVENT_SHADOW_SHOT, Seconds(10), 0, THE_CAPTAIN);
        }

        void EnterEvadeMode(EvadeReason /*why*/) override
        {
            if (events.IsInPhase(THE_CAPTAIN))
                Talk(SAY_SISTERS_EVADE);
            summons.DespawnAll();
            _DespawnAtEvade();
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER && !me->IsInEvadeMode() && events.IsInPhase(THE_CAPTAIN))
                Talk(SAY_YATHAE_KILLS);

            else if (events.IsInPhase(THE_HUNTRESS))
            {
                Creature* kasparian = instance->GetCreature(DATA_HUNTRESS_KASPARIAN);
                kasparian->AI()->Talk(SAY_SISTER_KILLS);
            }

            else if (events.IsInPhase(THE_PRIESTESS))
            {
                Creature* lunaspyre = instance->GetCreature(DATA_PRIESTESS_LUNASPYRE);
                lunaspyre->AI()->Talk(SAY_SISTER_KILLS);
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
        {
            if (events.IsInPhase(SISTER_GHOST) && !HealthAbovePct(70))
            {
                events.SetPhase(THE_CAPTAIN);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveAura(SPELL_GHOST_AURA);
                return;
            }
        }

        void HealthBound(uint32 diff, uint32 HPUpdate)
        {
            if (events.IsInPhase(THE_CAPTAIN))
            {
                Creature* kasparian = instance->GetCreature(DATA_HUNTRESS_KASPARIAN);
                Creature* lunaspyre = instance->GetCreature(DATA_PRIESTESS_LUNASPYRE);
                HPUpdate = 1000;

                if (HPUpdate <= diff)
                {
                    uint32 yathaeHP = me->GetHealth();
                    kasparian->SetHealth(yathaeHP);
                    lunaspyre->SetHealth(yathaeHP);
                }
                else HPUpdate -= diff;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CALL_MOONTALON:
                         DoCast(me, SPELL_CALL_MOONTALON);
                    events.Repeat(Seconds(40));
                    break;
                    case EVENT_TWILIGHT_VOLLEY:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        {
                            DoCast(target, SPELL_TWILIGHT_VOLLEY_AT);
                        }
                        events.Repeat(Seconds(15));
                    }
                    break;
                    case EVENT_RAPID_SHOT:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        {
                            DoCast(target, SPELL_RAPID_SHOT);
                            DoCast(me, SPELL_COOLDOWN_CREATURE_SPECIAL, true);
                        }
                        events.Repeat(Seconds(17));
                    }
                    break;
                    case EVENT_SHADOW_SHOT:
                    {
                       if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                       {
                           DoCast(target, SPELL_SHADOW_SHOT);
                       }
                       events.Repeat(Seconds(20));
                    }
                    break;
                 default:
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

// class boss_priestess_lunaspyre : public CreatureScript
// {
// public: 
   // boss_priestess_lunaspyre() : CreatureScript("boss_priestess_lunaspyre") { }
   
   // struct boss_priestess_lunaspyreAI : public SistersoftheMoonBossAI
   // {
	   // boss_priestess_lunaspyreAI(Creature* creature) : SistersoftheMoonBossAI(creature, DATA_PRIESTESS_LUNASPYRE) { }
	   
	   // void PhaseCombat() override
		// {
			// if (events.IsInPhase(THE_PRIESTESS))
			// {
				 // me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                 // me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				 // //Remove Ghost Aura
				 // events.ScheduleEvent(EVENT_TELEPORT_CENTER_COMBAT, Seconds(0));
				 // events.ScheduleEvent(EVENT_LUNAR_BEACON, Seconds(12));
				 // events.ScheduleEvent(EVENT_LUNAR_FIRE, Seconds(22));
				 // events.ScheduleEvent(EVENT_MOON_BURN, Seconds(25));
			// }
			// else
				// PhaseGhost();
		// }
		
		// void PhaseGhost() override
		// {
			// if (events.IsInPhase(THE_HUNTRESS) || events.IsInPhase(THE_CAPTAIN))
			// {
				// events.ScheduleEvent(EVENT_INCORPOREAL_TELEPORT, Seconds(30));
				// events.ScheduleEvent(EVENT_LUNAR_FIRE, Seconds(4));
			// }
			// else
				// PhaseCombat();
	    // }
		
		// void ExecuteEvent(uint32 eventId) override
	    // {
		   // switch (eventId)
		   // {
		   // }
	    // }
	   
	   // void UpdateAI(uint32 diff) override
	   // {
		   // if (!UpdateVictim())
			   // return;
		   
		   // events.Update(diff);
		   
		   // if (me->HasUnitState(UNIT_STATE_CASTING))
			   // return;
		   
		   // while (uint32 eventId = events.ExecuteEvent())
		   // {
			   // ExecuteEvent(eventId);
			   // if (me->HasUnitState(UNIT_STATE_CASTING))
				   // return;
		   // }
		   
		   // DoMeleeAttackIfReady();
	   // }
	// };
	
	// CreatureAI* GetAI(Creature* creature) const override
	// {
		// return GetTombOfSargerasAI<boss_priestess_lunaspyreAI>(creature);
	// }
// };
/*struct at_twilight_volley : AreaTriggerAI
{
    at_twilight_volley(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    void OnInitialize() override
    {
       
    }
};*/
struct at_twilight_glaive : AreaTriggerAI
{
	at_twilight_glaive(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }
	
	void OnInitialize() override
	{
        
		if (Unit* caster = at->GetCaster())
			if (Creature* creCaster = caster->ToCreature())
				if (creCaster->IsAIEnabled)
                    if (Unit* target = ObjectAccessor::GetUnit(*creCaster, creCaster->AI()->GetGUID()))
                    {
                        Position targetPos = target->GetPosition();
                        float targetDist = caster->GetDistance(target);
                        float targetAngle = caster->GetAngle(target);
                        at->MovePosition(targetPos, targetDist, targetAngle);
                    }				
	}
	
	void OnUnitEnter(Unit* /*unit*/) override
	{
		if (Unit* caster = at->GetCaster())
			if (Unit* target = at->GetTarget())
			    if (caster->IsValidAttackTarget(target))
				    caster->CastSpell(target, SPELL_TWILIGHT_GLAIVE_DAMAGE, true);
	}

    void OnUpdate(uint32 diff) override
    {
        if (Unit* caster = at->GetCaster())
            if (!caster->IsInCombat())
                at->Remove();
    }
	
	void OnDestinationReached() override
	{
        if (Unit* caster = at->GetCaster())
        {
            Position casterPos = caster->GetPosition();
            float casterDist = at->GetDistance(caster);
            float casterAngle = at->GetAngle(caster);
            at->MovePosition(casterPos, casterDist, casterAngle);
        }
		at->Remove();
	}
};
			
void AddSC_boss_sisters_of_the_moon()
{
	//Kasparian
	new boss_huntress_kasparian();
    RegisterAreaTriggerAI(at_twilight_glaive);
	//new spell_twilight_glaive();
	//Yathae
	new boss_captain_yathae_moonstrike();
	//Lunaspyre
	//new boss_priestess_lunaspyre();
	
	//General
	new boss_sisters_of_the_moon();
}
