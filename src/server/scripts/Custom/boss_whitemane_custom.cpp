#include "ScriptPCH.h"
#include "InstanceScript.h"
#include "ObjectAccessor.h"
#include "SpellScript.h"
#include "scarlet_monastery.h"

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
			instance->SetBossState(DATA_WHITEMANE_CUSTOM, STARTED);
		}
		
		void EnterCombat(Unit* who) override
		{
			_EnterCombat();
			instance->SetEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
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
				summon->GetMotionMaster()->MovePoint(CombatZone[0]);
			    summon->SetWalk(false);
			}
			
			if (summon->GetEntry() == 735902)
			{
				summon->GetMotionMaster()->MovePoint(CombatZone[0]);
			    summon->SetWalk(false);
			}
			
			if (summon->GetEntry() == 735903)
			{
				summon->GetMotionMaster()->MovePoint(CombatZone[0]);
			    summon->SetWalk(false);
			}
		}
		
		void EnterEvadeMode(EvadeReason /*why*/) override
		{
			Talk(SAY_EVADE);
			_DespawnAtEvade();
			instance->SetBossDataState(DATA_WHITEMANE_CUSTOM, FAIL);
			

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
				
				while (me->HealthAbove(40) && events.IsInPhase(INTERPHASE))
				{
					RAND(
				    events.ScheduleEvent(EVENT_SUMM_CRUSADER1, Seconds(10),0, INTERPHASE);
					events.ScheduleEvent(EVENT_SUMM_CRUSADER2, Seconds(10), 0, INTERPHASE);
					events.ScheduleEvent(EVENT_SUMM_CRUSADER3, Seconds(10), 0, INTERPHASE));
				}
			}
			
			if (me->HealthBelowPct(45) && events.IsInPhase(INTERPHASE))
			{
				events.SetPhase(PHASE_2);
			}
			
			if (me->HealthBelowPct(20) && events.IsInPhase(PHASE_2))
			{
				events.SetPhase(PHASE_3);
				me->AddAura(me, SPELL_SHADOWFORM);
				events.ScheduleEvent(EVENT_MIND_BAST, Seconds(9), 0, PHASE_3);
			    events.ScheduleEvent(EVENT_SHIELD, Seconds(30), 0, PHASE_3);
			    events.ScheduleEvent(EVENT_MIND_FLAY, Seconds(12), PHASE_3);
			    events.ScheduleEvent(EVENT_MIND_SEAR, Seconds(22), PHASE_3);
			    events.ScheduleEvent(EVENT_HALO, Seconds(55), PHASE_3);
			    events.ScheduleEvent(EVENT_DIVINE_STAR, Seconds(20), PHASE_3);
			
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
						Unit* victim = GetTarget();
						me->CastCustomSpell(victim, SPELL_HOLY_FIRE, &damage, NULL, NULL, false);
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
					    events.Repeat(Seconds(Seconds(14));
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
					break;
				  default:
				    break;
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
    SPELL_SMITE_PRIEST       = 585,
	SPELL_SHIELD             = 17,
    SPELL_HEAL               = 222089,
	//Warrior
	SPELL_BLADESTORM         = 46924,
	SPELL_COLOSSAL_SMASH     = 167105,
	SPELL_MORTAL_STRIKE      = 12294,
};


class npc_scarlet_crusader : public CreatureScript
{
public:
   npc_scarlet_crusader() : CreatureScript("npc_scarlet_crusader") { }
   
   CreatureAI* GetAI(Creature* creature) const override
   {
	   return new GetScarletMonasteryAI<npc_scarlet_crusaderAI>(creature);
   }
   
   struct npc_scarlet_crusaderAI : public ScriptedAI
   {
	   npc_scarlet_crusaderAI(Creature* creature) : ScriptedAI(creature)
	   {
		   Initialize();
		   instance = creature->GetInstanceScript();
	   }
	   
	   void Initialize()
	   {
		   CrusaderStrike_Timer = 5500;
		   HammerOfJustice_Timer = 60000;
		   TemplarVerdict_Timer = 14000;
		   ShieldOfVengance_Timer = 30000;
	   }
	   
	   InstanceScript* instance;
	   
	   uint32 CrusaderStrike_Timer;
	   uint32 HammerOfJustice_Timer;
	   uint32 ShieldOfVengance_Timer;
	   uint32 TemplarVerdict_Timer;
	   
	   void Reset() override
	   {
		   Initialize();
	   }
	   
	   void UpdateAI(uint32 diff) override
	   {
		   if (!UpdateVictim())
			   return;
		   
		   if (CrusaderStrike_Timer <= diff)
		   {
			   Unit* target = GetTarget();
			   uint32 damage = 600000;
			   me->CastCustomSpell(target, SPELL_CRUSADER_STRIKE, &damage, NULL, NULL, false);
			   CrusaderStrike_Timer = 5500;
		   }
		   else CrusaderStrike_Timer -= diff;
		   
		   if (TemplarVerdict_Timer <= diff)
		   {
			   Unit* target = GetTarget();
			   uint32 damage = 1500000;
			   me->CastCustomSpell(target, SPELL_TEMPLAR_VERDICT, &damage, NULL, NULL, true);
	           TemplarVerdict_Timer = 14000;
		   }
		   else TemplarVerdict_Timer -= diff;
		   
		   if (ShieldOfVengance_Timer <= diff)
		   {
			   Unit* crusader = GetCaster();
			   uint32 absorb = 30000000;
			   me->CastCustomSpell(caster, SPELL_SHIELD_VENGANCE, &absorb, NULL, NULL, false);
			   ShieldOfVengance_Timer = 30000;
		   }
		   else ShieldOfVengance_Timer -= diff;
		   
		   if (HammerOfJustice_Timer <= diff)
		   {
			   if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
				   DoCast(target, SPELL_HAMMER_JUSTICE);
			   HammerOfJustice_Timer = 60000;
		   }
		   else HammerOfJustice_Timer -= diff;
	   }
   };
};

class npc_scarlet_priest : public CreatureScript
{
public:
    npc_scarlet_priest() : CreatureScript("npc_scarlet_priest") { }
	
	CreatureAI* GetAI(Creature* creature) const override
	{
		return new GetScarletMonasteryAI<npc_scarlet_priestAI>(creature);
	}
	
	struct npc_scarlet_priestAI : public ScriptedAI
	{
		npc_scarlet_priestAI(Creature* creature) : ScriptedAI(creature)
		{
			Initialize();
			instance = creature->GetInstanceScript();
		}
		
		void Initialize()
		{
			Smite_Timer = 4000;
			Shield_Timer = 15000;
			Heal_Timer = 25000;
		}
		
		InstanceScript* instance;
		
		uint32 Smite_Timer;
		uint32 Shield_Timer;
		uint32 Heal_Timer;
		
		void Reset() override
		{
			Initialize();
		}
		
		void UpdateAI(uint32 diff) override
		{
			if (!UpdateVictim())
				return;
			
			if (Smite_Timer <= diff)
			{
				uint32 damage = 1500000;
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
					me->CastCustomSpell(target, SPELL_SMITE_PRIEST, &damage, NULL, NULL, false);
				Smite_Timer = 4000;
			}
			else Smite_Timer -= diff;
			
			if (Shield_Timer <= diff)
			{
				uint32 absorb = 7500000;
				me->CastSpellScript(me, SPELL_SHIELD_PRIEST, &absorb, NULL, NULL, false);
			    Shield_Timer = 25000;
			}
			else Shield_Timer -= diff;
			
			if (Heal_Timer <= diff)
			{
				uint32 heal = 8000000;
				me->CastCustomSpell(me, SPELL_HEAL, &heal, NULL, NULL, false);
				Heal_Timer = 15000;
			}
			else Heal_Timer -= diff;
		}
	};
};

class npc_scarlet_warrior : public CreatureScript
{
public:
   npc_scarlet_warrior() : CreatureScript("npc_scarlet_warrior") { }
   
   CreatureAI* GetAI(Creature* creature) const override
   {
	   return new GetScarletMonasteryAI<npc_scarlet_warriorAI>(creature);
   }
   
   struct npc_scarlet_warriorAI : public ScriptedAI
   {
	   npc_scarlet_warriorAI(Creature* creature) : ScriptedAI(creature)
	   {
		   Initialize();
		   instance = creature->GetInstanceScript();
	   }
	   
	   void Initialize()
	   {
		   Bladestorm_Timer = 30000;
		   ColossusSmash_Timer = 13000;
		   MortalStrike_Timer = 6500;
	   }
	   
	   InstanceScript* instance;
	   
	   uint32 Bladestorm_Timer;
	   uint32 ColossusSmash_Timer;
	   uint32 MortalStrike_Timer;
	   
	   void Reset() override
	   {
		   Initialize();
	   }
	   
	   void UpdateAI(uint32 diff) override
	   {
		   if (!UpdateVictim())
			   return;
		   
		   if (Bladestorm_Timer <= diff)
		   {
			   uint32 damage = 750000;
			   me->CastCustomSpell(me, SPELL_BLADESTORM, &damage, NULL, NULL, false);
			   Bladestorm_Timer = 30000;
		   }
		   else Bladestorm_Timer -= diff;
		   
		   if (ColossusSmash_Timer <= diff)
		   {
			   Unit* target = GetTarget();
			   uint32 damage = 1500000;
			   me->CastCustomSpell(target, SPELL_COLOSSAL_SMASH, &damage, NULL, NULL, false);
			   ColossusSmash_Timer = 13000;
		   }
		   else ColossusSmash_Timer -= diff;
		   
		   if (MortalStrike_Timer <= diff)
		   {
			   Unit* target = GetTarget();
			   uint32 damage = 2000000;
			   me->CastCustomSpell(target, SPELL_MORTAL_STRIKE, &damage, NULL, NULL, false);
			   MortalStrike_Timer = 6500;
		   }
		   else MortalStrike_Timer -= diff;
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
