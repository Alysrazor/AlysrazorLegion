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
  //Kasparian 
  SAY_SPELL_GLAIVE      = 3,
  SAY_SISTER_KILLS      = 4,
  //Yathae
  SAY_PHASE_COMBAT      = 5,
  SAY_SPELLS_ARROW      = 6,
  SAY_SPELL_AOE         = 7,
  //Lunaspyre
  SAY_SPELL             = 8,
  SAY_FOUNTAIN_OF_ELUNE = 9,
}

enum SistersSpells
{
	//Generic
	SPELL_QUIET_SUICIDE                   = 3617, // Serverside spell
	SPELL_INCORPOREAL_TELEPORT            = 236115, //Cast
	SPELL_INCORPOREAL_TELEPORT_2          = 236224, //Not sure if this is the teleport or need script. Instant cast. These two teleport spells are the same for Lunaspyre
	SPELL_COOLDOWN_CREATURE_SPECIAL       = 61207, //To block other spells from interfering with their special
    //Kasparian
	SPELL_TWILIGHT_GLAIVE_AT              = 236529,// AT 9785 and add aura to bunny + make bunny got target,then return
	SPELL_TWILIGHT_GLAIVE_DAMAGE          = 236541,
	SPELL_TWILIGHT_GLAIVE_DUMMY           = 237561,
	SPELL_MOON_GLAIVE                     = 236547,
	SPELL_DISCORPORATE                    = 236550, //Astral Purge will remove it.
	SPELL_GLAIVE_STORM                    = 239379, //Only used in Elune's Fountain Phase 3 (normal), phase 2 and 3 heroic and mythic
	//Yathae
	SPELL_SHADOW_SHOT                     = 237630,
	SPELL_INCORPOREAL_SHOT_CAST           = 236305,//Only used in Elune's Fountain Phase 2 (normal), phase 1 and 3 heroic and mythic
	SPELL_INCORPOREAL_SHOT_DAMAGE         = 236306,
	SPELL_CALL_MOONTALON                  = 236694,
	SPELL_TWILIGHT_VOLLEY_AT              = 236442,// AT 9777
	SPELL_TWILIGHT_VOLLEY_DAMAGE          = 236516, //Apply this aura OnUnitEnter();
	SPELL_RAPID_SHOT                      = 236598,
	SPELL_RAPID_SHOT_AURA                 = 236596,
	//Lunaspyre
	SPELL_EMBRACE_OF_THE_ECLIPSE_BOSS     = 233263,//Shields Heal
	SPELL_EMBRACE_OF_THE_ECLIPSE_BOSS_2   = 233264,//Shield Boss
	SPELL_LUNAR_BEACON                    = 236712,//OnRemove(); actives Lunar barrage.
	SPELL_LUNAR_FIRE                      = 239264,//Only on current target,active.
	SPELL_LUNAR_STRIKE                    = 237632,
	SPELL_MOON_BURN                       = 236518,//Astral Purge will remove this.
	SPELL_LUNAR_BARRAGE_AT                = 236726,//AT 9807
	SPELL_LUNAR_BARRAGE_DAMAGE            = 237351,
}

enum SistersEvents
{
	//General
	EVENT_INCORPOREAL_TELEPORT = 1,
	EVENT_TELEPORT_CENTER_COMBAT,
	EVENT_BACK_TO_GHOST,
	//Kasparian
	EVENT_TWILIGHT_GLAIVE,
	EVENT_MOON_GLAIVE,
	EVENT_GLAIVE_STORM,
	EVENT_JUMP,
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
}

enum Phases
{
	PHASE_COMBAT = 1,
	PHASE_GHOST  = 2,
}

uint32 const SistersData[3]
{
	DATA_HUNTRESS_KASPARIAN,
	DATA_CAPTAIN_YATHAE_MOONSTRIKE,
	DATA_PRIESTESS_LUNASPYRE
}

enum SistersMisc
{
	SUMMON_SISTERS_GROUP = 1,
}

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
					   instance->SendEncounterUnit(ENCOUNTER_FLAME_ENRAGE, sisters);
					   DoZoneInCombat(sisters);
			       }
			   }
			   events.ScheduleEvent(EVENT_BERSERKER, Minutes(8));
			   if (Creature* sisters = instance->GetCreature(SistersData[urand(0,2)])
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
					   instance->SendEncounterUnit(ENCOUNTER_FLAME_ENRAGE, sisters);
			   
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
				   instance->SendEncounterUnit(ENCOUNTER_FLAME_ENRAGE, sisters);
				   sisters->LowerPlayerDamageReq(sisters->GetMaxHealth();
				   sisters->CastSpell(sisters, SPELL_QUIET_SUICIDE, true);
			   }
			   if (Creature* sistersDeath = instance->GetCreature(SistersData[urand(0,2)])
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

struct SistersoftheMoonBossAI : public BossAI
{
	SistersoftheMoonBossAI(Creature* creature, uint32 bossId) : BossAI(creature, bossId) _bossId(bossId)
	{
		SetBoundary(instance->GetBossBoundary(DATA_SISTERS_OF_THE_MOON));
	}	
		void Reset() override
		{
			me->SetCombatPulseDelay(0);
			events.Reset();
	    }
		
		void EnterCombat(Unit* /*who*/) override
		{
			me->SetCombatPulseDelay(5);
			me->SetActive(true);
			if (Creature* sisters = instance->GetCreature(DATA_SISTERS_OF_THE_MOON))
				DoZoneInCombat(sisters);
			if (Creature* kasparian = instance->GetCreature(DATA_HUNTRESS_KASPARIAN))
				kasparian->SetPhase(PHASE_COMBAT);
			if (Creature* yathae = instance->GetCreature(DATA_CAPTAIN_YATHAE_MOONSTRIKE))
				yathae->SetPhase(PHASE_GHOST);
			if (Creature* lunaspyre = instance->GetCreature(DATA_PRIESTESS_LUNASPYRE))
				lunaspyre->SetPhase(PHASE_GHOST);
			PhaseCombat();
			PhaseGhost();
	    }
		
		virtual void PhaseCombat() = 0;
		virtual void PhaseGhost() = 0;
		
		void EnterEvadeMode(EvadeReason why) override
		{
			if (Creature* sisters = instance->GetCreature(DATA_SISTERS_OF_THE_MOON))
				sisters->AI()->EnterEvadeMode(why);
		}
		
		void KilledUnit(Unit* victim, Unit* killer) override
		{
			Creature* kasparian = instance->GetCreature(DATA_HUNTRESS_KASPARIAN)
			Creature* yathae = instance->GetCreature(DATA_CAPTAIN_YATHAE_MOONSTRIKE)
		    Creature* lunaspyre = instance->GetCreature(DATA_PRIESTESS_LUNASPYRE)
			
			if (victim->GetTypeId() == TYPEID_PLAYER)
				Talk(SAY_SISTER_KILLS);
			
			else if ((victim->GetTypeId() == TYPEID_PLAYER) && ((killer = yathae || killer = lunaspyre) && instance->GetBossState(DATA_HUNTRESS_KASPARIAN, IN_PROGRESS)))
				//Kasparian Special Say KilledUnit
			
			else if ((victim->GetTypeId() == TYPEID_PLAYER) && ((killer = kasparian || killer = lunaspyre) && instance->GetBossState(DATA_CAPTAIN_YATHAE_MOONSTRIKE, IN_PROGRESS)))
				//Yathae Special Say Killed Unit
			
			else if ((victim->GetTypeId() == TYPEID_PLAYER) && ((killer = kasparian || killer = yathae) && instance->GetBossState(DATA_PRIESTESS_LUNASPYRE, IN_PROGRESS)))
		      //Lunaspyre Special Say Killed Unit 
		}
	
		void DamageTaken() override
		{
			Creature* kasparian = instance->GetCreature(DATA_HUNTRESS_KASPARIAN)
			Creature* yathae = instance->GetCreature(DATA_CAPTAIN_YATHAE_MOONSTRIKE)
		    Creature* lunaspyre = instance->GetCreature(DATA_PRIESTESS_LUNASPYRE)
				if (kasparian->HealthBelowPct(70))
				{
					kasparian->SetPhase(PHASE_GHOST);
					yathae->SetPhase(PHASE_COMBAT);
					instance->SetBossState(DATA_HUNTRESS_KASPARIAN, DONE);
				}
			    if (yathae->HealthBelowPct(40))
				{
					yathae->SetPhase(PHASE_GHOST);
					lunaspyre->SetPhase(PHASE_COMBAT);
					instance->SetBossState(DATA_CAPTAIN_YATHAE_MOONSTRIKE, DONE);
				}
		}
		
		private:
		    uint32 _bossId;
};
		
		
class boss_huntress_kasparian : public CreatureScript
{
public:
   boss_huntress_kasparian() : CreatureScript("boss_huntress_kasparian") { }
   
   struct boss_huntress_kasparianAI : public SistersoftheMoonBossAI
   {
	   boss_huntress_kasparianAI(Creature* creature) SistersoftheMoonBossAI(creature, DATA_HUNTRESS_KASPARIAN) { }

       void PhaseCombat() override
	   {
		   if (me->GetPhase(PHASE_COMBAT))
		   {
		      events.ScheduleEvent(EVENT_MOON_GLAIVE, Seconds(17));
		      events.ScheduleEvent(EVENT_TWILIGHT_GLAIVE_AT, Seconds(22));
		   }
		   else
			   PhaseGhost();
	   }
	   
	   void PhaseGhost() override
	   {
		   if (me->GetPhase(PHASE_GHOST))
		   {
			   me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
               me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			   //AddAura Ghost
			   events.ScheduleEvent(EVENT_BACK_TO_GHOST, Seconds(0);
			   events.ScheduleEvent(EVENT_TWILIGHT_GLAIVE_AT, Seconds(22));
			   events.ScheduleEvent(EVENT_MOON_GLAIVE, Seconds(3));
			   events.ScheduleEvent(EVENT_JUMP, Seconds(30));
		   }
		   else
			   PhaseCombat();
	   }
	   
	   void ExecuteEvent(uint32 eventId) override
	   {
		   switch (eventId)
		   {
			   case EVENT_MOON_GLAIVE:
			   {
				   if (me->GetPhase(PHASE_COMBAT))
					   DoCastVictim(SPELL_MOON_GLAIVE);
				   events.Repeat(Seconds(17));
				   else
				   {
					   if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
						   DoCast(target, SPELL_MOON_GLAIVE,true);//SPELL_MOON_GLAIVE Instant Cast no debuff
					   events.Repeat(Seconds(3));
				   }
				   break;
			   }
			   case EVENT_TWILIGHT_GLAIVE_AT:
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
	   
	   void UpdateAI(uint32 diff) override
	   {
		   if (!UpdateVictim())
			   return;
		   
		   events.Update(diff);
		   
		   if (me->HasUnitState(UNIT_STATE_CASTING))
			   return;
		   
		   while (uint32 eventId = events.ExecuteEvent())
		   {
			   ExecuteEvent(eventId);
			   if (me->HasUnitState(UNIT_STATE_CASTING))
				   return;
		   }
		   
		   DoMeleeAttackIfReady();
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
	
	struct boss_captain_yathae_moonstrikeAI : public SistersoftheMoonBossAI
	{
		boss_captain_yathae_moonstrikeAI(Creature* creature) : SistersoftheMoonBossAI(creature, DATA_CAPTAIN_YATHAE_MOONSTRIKE) { }
		
		void PhaseCombat() override
		{
			if (me->GetPhase(PHASE_COMBAT))
			{
				 me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                 me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				 //Remove Ghost Aura
				 events.ScheduleEvent(EVENT_TELEPORT_CENTER_COMBAT, Seconds(0));
			}
			else
				PhaseGhost();
		}
		
		void PhaseGhost() override
		{
			if (me->GetPhase(PHASE_GHOST))
			{
				events.ScheduleEvent(EVENT_INCORPOREAL_TELEPORT, Seconds(30));
			}
			else
				PhaseCombat();
	    }
		
		void ExecuteEvent(uint32 eventId) override
	   {
		   switch (eventId)
		   {
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
			   ExecuteEvent(eventId);
			   if (me->HasUnitState(UNIT_STATE_CASTING))
				   return;
		   }
		   
		   DoMeleeAttackIfReady();
	   }
	};
	
	CreatureAI* GetAI(Creature* creature) const override
	{
		return GetTombOfSargerasAI<boss_captain_yathae_moonstrikeAI>(creature);
	}
};

class boss_priestess_lunaspyre : public CreatureScript
{
public: 
   boss_priestess_lunaspyre() : CreatureScript("boss_priestess_lunaspyre") { }
   
   struct boss_priestess_lunaspyreAI : public SistersoftheMoonBossAI
   {
	   boss_priestess_lunaspyreAI(Creature* creature) : SistersoftheMoonBossAI(creature, DATA_PRIESTESS_LUNASPYRE) { }
	   
	   void PhaseCombat() override
		{
			if (me->GetPhase(PHASE_COMBAT))
			{
				 me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                 me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				 //Remove Ghost Aura
				 events.ScheduleEvent(EVENT_TELEPORT_CENTER_COMBAT, Seconds(0));
				 events.ScheduleEvent(EVENT_LUNAR_BEACON, Seconds(12));
				 events.ScheduleEvent(EVENT_LUNAR_FIRE, Seconds((17), (24));
				 events.ScheduleEvent(EVENT_MOON_BURN, Seconds(25));
			}
			else
				PhaseGhost();
		}
		
		void PhaseGhost() override
		{
			if (me->GetPhase(PHASE_GHOST))
			{
				events.ScheduleEvent(EVENT_INCORPOREAL_TELEPORT, Seconds(30));
				events.ScheduleEvent(EVENT_LUNAR_FIRE, Seconds(4));
			}
			else
				PhaseCombat();
	    }
		
		void ExecuteEvent(uint32 eventId) override
	   {
		   switch (eventId)
		   {
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
			   ExecuteEvent(eventId);
			   if (me->HasUnitState(UNIT_STATE_CASTING))
				   return;
		   }
		   
		   DoMeleeAttackIfReady();
	   }
	};
	
	CreatureAI* GetAI(Creature* creature) const override
	{
		return GetTombOfSargerasAI<boss_priestess_lunaspyreAI>(creature);
	}
};


void AddSC_boss_sisters_of_the_moon()
{
	new boss_captain_yathae_moonstrike();
	new boss_huntress_kasparian();
	new boss_priestess_lunaspyre();
	new boss_sisters_of_the_moon();
}