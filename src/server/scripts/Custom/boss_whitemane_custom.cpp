#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "Player.h"
#include "C:\Users\Sergio99\Desktop\TrinityCore 7.3.5\YserasDream\src\server\scripts\EasternKingdoms\ScarletMonastery\scarlet_monastery.h"

Position const SummonCrusaders[2] =
{
	{ 1140.730713f, 1453.878296f, 30.304049f, 4.763378f  }, // Left
	{ 1140.666504f, 1344.424072f, 30.304459f, 1.668911f }, // Right
};

Position const CombatZone[1] = 
{
	{ 1132.510254f, 1399.070801f, 30.309042f, 3.172173f },
};


enum Spells
{
	SPELL_SMITE           = 585,
	SPELL_SHIELD          = 17,
	SPELL_MIND_BLAST      = 8092,
	SPELL_MIND_FLAY       = 15407,
	SPELL_HEAL            = 222089,
	SPELL_HOLY_FIRE       = 17142,
	SPELL_HALO            = 120517,
	SPELL_SHADOWFORM      = 22917,
	SPELL_DIVINE_STAR     = 110744,
	SPELL_MIND_SEAR       = 48045,
	SPELL_WORD_POWER_PAIN = 589,
	SPELL_HOLY_WRATH      = 227823,
};

enum Says
{
	SAY_AGGRO             = 1,
	SAY_KILL              = 2,
	SAY_EVADE             = 3,
	SAY_DIED              = 4,
	SAY_INTERPHASE        = 5,
	SAY_PHASE_2           = 6,
	SAY_PHASE_3           = 7,
	SAY_KILL_ALL          = 8,
};

enum Events
{
	EVENT_SMITE           = 1,
	EVENT_SHIELD          = 2,
	EVENT_MIND_BAST       = 3,
	EVENT_MIND_FLAY       = 4,
	EVENT_HEAL            = 5,
	EVENT_HOLY_FIRE       = 6,
	EVENT_HALO            = 7,
	EVENT_DIVINE_STAR     = 8,
	EVENT_MIND_SEAR       = 9,
	EVENT_WORD_POWER_PAIN = 10,
	EVENT_HOLY_WRATH      = 11,
	EVENT_SUMM_CRUSADER1  = 12, //Crusader Paladin
	EVENT_SUMM_CRUSADER2  = 13, //Priest
	EVENT_SUMM_CRUSADER3  = 14, //Crusader Warrior
};

enum Phases
{
	PHASE_1               = 1,
	INTERPHASE            = 4,
	PHASE_2               = 2,
	PHASE_3               = 3,
};

class boss_whitemane_custom : public CreatureScript
{
public:
    boss_whitemane_custom() : CreatureScript ("boss_whitemane_custom") { }
	
	CreatureAI* GetAI(Creature* creature) const override
	{
		return GetScarletMonasteryAI<boss_whitemane_customAI>(creature);
	}
	
	struct boss_whitemane_customAI : public BossAI
	{
		boss_whitemane_customAI(Creature* creature) : BossAI(creature, DATA_WHITEMANE_CUSTOM) { }
		
		void Reset() override
		{
			_Reset();
			events.SetPhase(PHASE_1);
		}
		
		void EnterCombat(Unit* who) override
		{
			_EnterCombat();
			instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
			DoZoneInCombat();
			events.ScheduleEvent(EVENT_SMITE, Seconds(6), 0, PHASE_1);
			events.ScheduleEvent(EVENT_SHIELD, Seconds(30), 0, PHASE_1);
			events.ScheduleEvent(EVENT_HOLY_FIRE, Seconds(14), PHASE_1);
			events.ScheduleEvent(EVENT_HEAL, Seconds(65), PHASE_1);
			events.ScheduleEvent(EVENT_HALO, Seconds(55), PHASE_1);
			events.ScheduleEvent(EVENT_DIVINE_STAR, Seconds(20), PHASE_1);
			events.ScheduleEvent(EVENT_HOLY_WRATH, Minutes(8));
		}
		
		void JustSummoned(Creature* summon) override
		{
			summons.Summon(summon);
			
			if (summon->GetEntry() == 735901)
			{
                summon->GetMotionMaster()->MovePoint(1, CombatZone[0]);
			    summon->SetWalk(false);
			}
			
			if (summon->GetEntry() == 735902)
			{
                summon->GetMotionMaster()->MovePoint(1, CombatZone[0]);
			    summon->SetWalk(false);
			}
			
			if (summon->GetEntry() == 735903)
			{
                summon->GetMotionMaster()->MovePoint(1, CombatZone[0]);
			    summon->SetWalk(false);
			}
		}
		
		void EnterEvadeMode(EvadeReason /*why*/) override
		{
			Talk(SAY_EVADE);
			_DespawnAtEvade();
			instance->SetBossState(DATA_WHITEMANE_CUSTOM, FAIL);
			

            std::list<Player*> playerList;
			GetPlayerListInGrid(playerList,me, 300.0f);
			
            for (auto itr : playerList)
                itr->RemoveAura(SPELL_HOLY_WRATH);
		}
		
		void KilledUnit(Unit* victim) override
		{
			if (victim->GetTypeId() == TYPEID_PLAYER)
				Talk(SAY_KILL);
		}
		
		void JustDied(Unit* /*killer*/) override
		{
			events.Reset();
			Talk(SAY_DIED);
			summons.DespawnAll();
			instance->SetBossState(DATA_WHITEMANE_CUSTOM, DONE);
		}
		
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->HealthBelowPct(70) && events.IsInPhase(PHASE_1))
            {
                events.SetPhase(PHASE_2);
                events.ScheduleEvent(EVENT_SMITE, Seconds(6), 0, PHASE_2);
                events.ScheduleEvent(EVENT_SHIELD, Seconds(30), 0, PHASE_2);
                events.ScheduleEvent(EVENT_HOLY_FIRE, Seconds(14), PHASE_2);
                events.ScheduleEvent(EVENT_HEAL, Seconds(65), PHASE_2);
                events.ScheduleEvent(EVENT_HALO, Seconds(55), PHASE_2);
                events.ScheduleEvent(EVENT_DIVINE_STAR, Seconds(20), PHASE_2);
            }

            if (me->HealthBelowPct(50) && events.IsInPhase(PHASE_2))
            {
                events.SetPhase(INTERPHASE);
                Talk(SAY_INTERPHASE);
                me->SetPosition(CombatZone[0]);
                

                while (me->HealthAbovePct(40) && events.IsInPhase(INTERPHASE))
                {
                    int c = RAND(1, 2, 3);
                    uint32 wait = 10000;

                    switch (c)
                    {
                        if (wait <= diff)
                        {
                           case 1:
                             events.ScheduleEvent(EVENT_SUMM_CRUSADER1, Seconds(10), 0, INTERPHASE);
                           wait = 10000;
                           break;
                           case 2:
                             events.ScheduleEvent(EVENT_SUMM_CRUSADER2, Seconds(10), 0, INTERPHASE);
                           wait = 10000;
                           break;
                           case 3:
                             events.ScheduleEvent(EVENT_SUMM_CRUSADER3, Seconds(10), 0, INTERPHASE);
                           wait = 10000;
                           break;
                         default:
                           wait = 10000;
                           break;

                        }
                        else wait -= diff;
                    }
                }

                if (me->HealthBelowPct(45) && events.IsInPhase(INTERPHASE))
                {
                    events.SetPhase(PHASE_2);
                }

                if (me->HealthBelowPct(20) && events.IsInPhase(PHASE_2))
                {
                    events.SetPhase(PHASE_3);
                    me->AddAura(SPELL_SHADOWFORM, me);
                    events.ScheduleEvent(EVENT_MIND_BAST, Seconds(9), 0, PHASE_3);
                    events.ScheduleEvent(EVENT_SHIELD, Seconds(30), 0, PHASE_3);
                    events.ScheduleEvent(EVENT_MIND_FLAY, Seconds(12), PHASE_3);
                    events.ScheduleEvent(EVENT_MIND_SEAR, Seconds(22), PHASE_3);
                    events.ScheduleEvent(EVENT_HALO, Seconds(55), PHASE_3);
                    events.ScheduleEvent(EVENT_DIVINE_STAR, Seconds(20), PHASE_3);
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                       case EVENT_SMITE:
                       {
                          int32 damage = 2500000;
                          if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                          {
                             me->CastCustomSpell(target, SPELL_SMITE, &damage, NULL, NULL, false);
                          }
                          events.Repeat(Seconds(6));
                       }
                       break;
                       case EVENT_SHIELD:
                       {
                          int32 absorb = 30000000;
                          me->CastCustomSpell(me, SPELL_SHIELD, &absorb, NULL, NULL, false);
                          events.Repeat(Seconds(30));
                       }
                       break;
                       case EVENT_DIVINE_STAR:
                       {
                          int32 damage = 1500000;
                          me->CastCustomSpell(me, SPELL_DIVINE_STAR, &damage, NULL, NULL, false);
                          events.Repeat(Seconds(20));
                       }
                       break;
                       case EVENT_HALO:
                       {
                          int32 damage = 4000000;
                          me->CastCustomSpell(me, SPELL_HALO, &damage, NULL, NULL, true);
                          events.Repeat(Seconds(55));
                       }
                       break;
                       case EVENT_HOLY_FIRE:
                       {
                          int32 damage = 6000000;
                          me->CastCustomSpell(me->GetVictim(), SPELL_HOLY_FIRE, &damage, NULL, NULL, false);
                          events.Repeat(Seconds(14));
                       }
                       break;
                       case EVENT_HEAL:
                       {
                          int32 heal = 60000000;
                          me->CastCustomSpell(me, SPELL_HEAL, &heal, NULL, NULL, false);
                          events.Repeat(Seconds(60));
                       }
                       break;
                       case EVENT_HOLY_WRATH:
                       {
                          int32 damage = 30000000;
                          me->CastCustomSpell(me, SPELL_HOLY_WRATH, &damage, NULL, NULL, false);
                       }
                       break;
                       case EVENT_MIND_BAST:
                       {
                           int32 damage = 3000000;
                           if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           {
                              me->CastCustomSpell(target, SPELL_MIND_BLAST, &damage, NULL, NULL, false);
                           }
                           events.Repeat(Seconds(9));
                       }
                       break;
                       case EVENT_MIND_FLAY:
                       {
                           int32 damage = 2500000;
                           if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           {
                              me->CastCustomSpell(target, SPELL_MIND_FLAY, &damage, NULL, NULL, false);
                           }
                           events.Repeat(Seconds(12));
                       }
                       break;
                       case EVENT_MIND_SEAR:
                       {
                           int32 damage = 2500000;
 
                           if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           {
                               me->CastCustomSpell(target, SPELL_MIND_SEAR, &damage, NULL, NULL, false);
                           }
                           events.Repeat(Seconds(25));
                       }
                       break;
                       case EVENT_WORD_POWER_PAIN:
                       {
                           int32 damage = 4500000;
                           if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           {
                              me->CastCustomSpell(target, SPELL_WORD_POWER_PAIN, &damage, NULL, NULL, false);
                           }
                           events.Repeat(Seconds(14));
                       }
                       case EVENT_SUMM_CRUSADER1:
                       {
                          me->SummonCreature(735901, SummonCrusaders[0], TEMPSUMMON_MANUAL_DESPAWN);
                          me->SummonCreature(735901, SummonCrusaders[1], TEMPSUMMON_MANUAL_DESPAWN);
                       }
                       break;
                       case EVENT_SUMM_CRUSADER2:
                       {
                          me->SummonCreature(735902, SummonCrusaders[0], TEMPSUMMON_MANUAL_DESPAWN);
                          me->SummonCreature(735902, SummonCrusaders[1], TEMPSUMMON_MANUAL_DESPAWN);
                       }
                       case EVENT_SUMM_CRUSADER3:
                       {
                           me->SummonCreature(735903, SummonCrusaders[0], TEMPSUMMON_MANUAL_DESPAWN);
                           me->SummonCreature(735903, SummonCrusaders[1], TEMPSUMMON_MANUAL_DESPAWN);
                       }
                       break;
                     default:
                       break;
                    
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
            
	};
};

enum NPCsSpells
{
	//Crusader
	SPELL_CRUSADER_STRIKE    = 35395,
	SPELL_TEMPLAR_VERDICT    = 85256,
	SPELL_HAMMER_JUSTICE     = 853,
	SPELL_SHIELD_VENGANCE    = 184662,
	//Priest
    //SPELL_SMITE_PRIEST       = 585,
	//SPELL_SHIELD             = 17,
    //SPELL_HEAL               = 222089,
	//Warrior
	SPELL_BLADESTORM         = 46924,
	SPELL_COLOSSAL_SMASH     = 167105,
	SPELL_MORTAL_STRIKE      = 12294,
};

enum NpcsEvents
{
    //Crusader
    EVENT_CRUSADER_STRIKE    = 1,
    EVENT_TEMPLAR_VERDICT    = 2,
    EVENT_HAMMER_JUSTICE     = 3,
    EVENT_SHIELD_VENGANCE    = 4,
    //Priest
    EVENT_SMITE_PRIEST       = 5,
    EVENT_SHIELD_PRIEST      = 6,
    EVENT_HEAL_PRIEST        = 7,
    //Warrior
    EVENT_BLADESTORM         = 8,
    EVENT_COLOSSAL_SMASH     = 9,
    EVENT_MORTAL_STRIKE      = 10,

};


class npc_scarlet_crusader : public CreatureScript
{
public:
   npc_scarlet_crusader() : CreatureScript("npc_scarlet_crusader") { }
   
   CreatureAI* GetAI(Creature* creature) const override
   {
	   return new npc_scarlet_crusaderAI(creature);
   }
   
   struct npc_scarlet_crusaderAI : public BossAI
   {
	   npc_scarlet_crusaderAI(Creature* creature) : BossAI(creature, NPC_SCARLET_CRUSADER)
	   {
		   instance = creature->GetInstanceScript();
	   }
	   
	   InstanceScript* instance;
	   
	  
	   void Reset() override
	   {
           _Reset();
	   }

       void EnterCombat(Unit* who) override
       {
           _EnterCombat();
           instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
           DoZoneInCombat();
           events.ScheduleEvent(EVENT_CRUSADER_STRIKE, Seconds(6));
           events.ScheduleEvent(EVENT_TEMPLAR_VERDICT, Seconds(14));
           events.ScheduleEvent(EVENT_SHIELD_VENGANCE, Seconds(30));
           events.ScheduleEvent(EVENT_HAMMER_JUSTICE, Minutes(1));
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
                    case EVENT_CRUSADER_STRIKE:
                    {
                        int32 damage = 1500000;
                        Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO);
                        me->CastCustomSpell(target, SPELL_CRUSADER_STRIKE, &damage, NULL, NULL, false);
                        events.Repeat(Seconds(6));
                    }
                    break;
                    case EVENT_SHIELD_VENGANCE:
                    {
                        int32 absorb = 20000000;
                        me->CastCustomSpell(me, SPELL_SHIELD_VENGANCE, &absorb, NULL, NULL, false);
                        events.Repeat(Seconds(30));
                    }
                    break;
                    case EVENT_HAMMER_JUSTICE:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_HAMMER_JUSTICE);
                        events.Repeat(Minutes(1));
                    }
                    break;
                    case EVENT_TEMPLAR_VERDICT:
                    {
                        int32 damage = 2000000;
                        me->CastCustomSpell(me->GetVictim(), SPELL_TEMPLAR_VERDICT, &damage, NULL, NULL, true);
                        events.Repeat(Seconds(14));
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

class npc_scarlet_priest : public CreatureScript
{
public:
    npc_scarlet_priest() : CreatureScript("npc_scarlet_priest") { }
	
	CreatureAI* GetAI(Creature* creature) const override
	{
		return new npc_scarlet_priestAI(creature);
	}
	
	struct npc_scarlet_priestAI : public BossAI
	{
		npc_scarlet_priestAI(Creature* creature) : BossAI(creature, NPC_SCARLET_PRIEST)
		{
			instance = creature->GetInstanceScript();
		}
		
		InstanceScript* instance;
		
		void Reset() override
		{
            _Reset();
		}

        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
            DoZoneInCombat();
            events.ScheduleEvent(EVENT_SMITE_PRIEST, Seconds(5));
            events.ScheduleEvent(EVENT_SHIELD_PRIEST, Seconds(30));
            events.ScheduleEvent(EVENT_HEAL_PRIEST, Seconds(40));
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
                    case EVENT_SMITE_PRIEST:
                    {
                       int32 damage = 900000;
                       if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           me->CastCustomSpell(target, SPELL_SMITE, &damage, NULL, NULL, false);
                       events.Repeat(Seconds(5));
                    }
                    break;
                    case EVENT_SHIELD_PRIEST:
                    {
                      int32 absorb = 7000000;
                      if (me->HealthBelowPct(60))
                          me->CastCustomSpell(me, SPELL_SHIELD, &absorb, NULL, NULL, false);
                       events.Repeat(Seconds(30));

                    }
                    break;
                    case EVENT_HEAL_PRIEST:
                    {
                        int32 heal = 7000000;
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                        if (me->HealthBelowPct(40))
                            me->CastCustomSpell(me, SPELL_HEAL, &heal, NULL, NULL, false);
                        else if (target->HealthBelowPct(40))
                            me->CastCustomSpell(target, SPELL_HEAL, &heal, NULL, NULL, false);
                         events.Repeat(Seconds(40));
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

class npc_scarlet_warrior : public CreatureScript
{
public:
   npc_scarlet_warrior() : CreatureScript("npc_scarlet_warrior") { }
   
   CreatureAI* GetAI(Creature* creature) const override
   {
	   return new npc_scarlet_warriorAI(creature);
   }
   
   struct npc_scarlet_warriorAI : public BossAI
   {
	   npc_scarlet_warriorAI(Creature* creature) : BossAI(creature, NPC_SCARLET_WARRIOR)
	   {
		   instance = creature->GetInstanceScript();
	   }
	   
	   InstanceScript* instance;
	   
	   
       void Reset() override
       {
           _Reset();
       }

       void EnterCombat(Unit* /*who*/) override
       {
           _EnterCombat();
           instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
           DoZoneInCombat();
           events.ScheduleEvent(EVENT_COLOSSAL_SMASH, Seconds(14));
           events.ScheduleEvent(EVENT_MORTAL_STRIKE, Seconds(6));
           events.ScheduleEvent(EVENT_BLADESTORM, Seconds(30));
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
                  case EVENT_BLADESTORM:
                  {
                      int32 damage = 650000;
                      me->CastCustomSpell(me, SPELL_BLADESTORM, &damage, NULL, NULL, false);
                      events.Repeat(Seconds(20));
                  }
                  break;
                  case EVENT_COLOSSAL_SMASH:
                  {
                      int32 damage = 1500000;
                      me->CastCustomSpell(me->GetVictim(), SPELL_COLOSSAL_SMASH, &damage, NULL, NULL, false);
                      events.Repeat(Seconds(16));
                  }
                  break;
                  case EVENT_MORTAL_STRIKE:
                  {
                      int32 damage = 2000000;
                      me->CastCustomSpell(me->GetVictim(), SPELL_MORTAL_STRIKE, &damage, NULL, NULL, false);
                      events.Repeat(Seconds(6));
                  }
               }
           }
           DoMeleeAttackIfReady();
	   }
   };
};

void AddSC_boss_whitemane_custom()
{
	new boss_whitemane_custom();
	new npc_scarlet_crusader();
	new npc_scarlet_priest();
	new npc_scarlet_warrior();
}
