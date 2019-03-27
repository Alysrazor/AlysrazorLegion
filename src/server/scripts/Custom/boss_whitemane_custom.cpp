// #include "ScriptMgr.h"
// #include "InstanceScript.h"
// #include "MotionMaster.h"
// #include "ObjectAccessor.h"
// #include "ScriptedCreature.h"
// #include "SpellInfo.h"
// #include "Spell.h"
// #include "SpellAuraEffects.h"
// #include "SpellMgr.h"
// #include "SpellScript.h"
// #include "Player.h"
// #include "C:\Users\Sergio99\Desktop\TrinityCore 7.3.5\YserasDream\src\server\scripts\EasternKingdoms\ScarletMonastery\scarlet_monastery.h"

// Position const SummonCrusaders[2] =
// {
	// { 1140.730713f, 1453.878296f, 30.304049f, 4.763378f  }, // Left
	// { 1140.666504f, 1344.424072f, 30.304459f, 1.668911f }, // Right
// };

// Position const CombatZone[1] = 
// {
	// { 1132.510254f, 1399.070801f, 30.309042f, 3.172173f },
// };


// enum Spells
// {
	// SPELL_SMITE           = 585,
	// SPELL_SHIELD          = 17,
	// SPELL_MIND_BLAST      = 222072,
	// SPELL_MIND_FLAY       = 183324,
	// SPELL_HEAL            = 222089,
	// SPELL_HOLY_FIRE       = 222087,
	// //SPELL_HALO            = 120517,
	// SPELL_MIND_SPIKE      = 201039,
	// SPELL_SHADOWFORM      = 22917,
	// //SPELL_DIVINE_STAR     = 110744,
	// SPELL_MIND_SEAR       = 85643,
	// SPELL_WORD_POWER_PAIN = 19776,
	// SPELL_HOLY_WRATH      = 227823,
// };

// enum NPCsSpells
// {
    // //Crusader
    // SPELL_CRUSADER_STRIKE = 176931,
    // SPELL_TEMPLAR_VERDICT = 167993,
    // SPELL_HAMMER_JUSTICE  = 853,
    // SPELL_SHIELD_VENGANCE = 184662,
    // //Priest
    // //Warrior
    // SPELL_COLOSSAL_SMASH  = 198804,
    // SPELL_SHOCKWAVE       = 57728,
    // SPELL_MORTAL_STRIKE   = 198375,
// };

// enum Says
// {
	// SAY_AGGRO             = 1,
	// SAY_KILL              = 2,
	// SAY_EVADE             = 3,
	// SAY_DIED              = 4,
	// SAY_INTERPHASE        = 5,
	// SAY_PHASE_2           = 6,
	// SAY_PHASE_3           = 7,
	// SAY_KILL_ALL          = 8,
// };

// enum Events
// {
	// EVENT_SMITE           = 1,
	// EVENT_SHIELD          = 2,
	// EVENT_MIND_BAST       = 3,
	// EVENT_MIND_FLAY       = 4,
	// EVENT_HEAL            = 5,
	// EVENT_HOLY_FIRE       = 6,
	// EVENT_HALO            = 7,
	// EVENT_DIVINE_STAR     = 8,
	// EVENT_MIND_SEAR       = 9,
	// EVENT_WORD_POWER_PAIN = 10,
	// EVENT_HOLY_WRATH      = 11,
	// EVENT_SUMM_CRUSADER1  = 12, //Crusader Paladin
	// EVENT_SUMM_CRUSADER2  = 13, //Priest
	// EVENT_SUMM_CRUSADER3  = 14, //Crusader Warrior
	// EVENT_MIND_SPIKE      = 15,
// };

// enum Phases
// {
	// PHASE_1               = 1,
	// INTERPHASE            = 4,
	// PHASE_2               = 2,
	// PHASE_3               = 3,
// };

// class boss_whitemane_custom : public CreatureScript
// {
// public:
    // boss_whitemane_custom() : CreatureScript ("boss_whitemane_custom") { }
	
	// CreatureAI* GetAI(Creature* creature) const override
	// {
		// return GetScarletMonasteryAI<boss_whitemane_customAI>(creature);
	// }
	
	// struct boss_whitemane_customAI : public BossAI
	// {
		// boss_whitemane_customAI(Creature* creature) : BossAI(creature, DATA_WHITEMANE_CUSTOM) { }
		
		// void Reset() override
		// {
			// _Reset();
		// }
		
		// void EnterCombat(Unit* /*who*/) override
		// {
			// _EnterCombat();
			// events.SetPhase(PHASE_1);
			// instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
			// DoZoneInCombat();
			// events.ScheduleEvent(EVENT_SMITE, Seconds(6), 0, PHASE_1);
			// events.ScheduleEvent(EVENT_SHIELD, Seconds(30), 0, PHASE_1);
			// events.ScheduleEvent(EVENT_HOLY_FIRE, Seconds(14), PHASE_1);
			// events.ScheduleEvent(EVENT_HEAL, Seconds(65), PHASE_1);
			// events.ScheduleEvent(EVENT_HALO, Seconds(55), PHASE_1);
			// events.ScheduleEvent(EVENT_DIVINE_STAR, Seconds(20), PHASE_1);
			// events.ScheduleEvent(EVENT_HOLY_WRATH, Minutes(8));
		// }
		
		// void JustSummoned(Creature* summon) override
		// {
			// summons.Summon(summon);
			
			// if (summon->GetEntry() == 735901)
			// {
                // summon->GetMotionMaster()->MovePoint(1, CombatZone[0]);
			    // summon->SetWalk(false);
			// }
			
			// if (summon->GetEntry() == 735902)
			// {
                // summon->GetMotionMaster()->MovePoint(1, CombatZone[0]);
			    // summon->SetWalk(false);
			// }
			
			// if (summon->GetEntry() == 735903)
			// {
                // summon->GetMotionMaster()->MovePoint(1, CombatZone[0]);
			    // summon->SetWalk(false);
			// }
		// }
		
		// void EnterEvadeMode(EvadeReason /*why*/) override
		// {
			// Talk(SAY_EVADE);
			// _DespawnAtEvade();
			// instance->SetBossState(DATA_WHITEMANE_CUSTOM, FAIL);
			// summons.DespawnAll();
			

            // std::list<Player*> playerList;
			// GetPlayerListInGrid(playerList,me, 300.0f);
			
            // for (auto itr : playerList)
            // {
                // itr->RemoveAura(SPELL_HOLY_WRATH);
                // itr->RemoveAura(SPELL_CRUSADER_STRIKE);
            // }
                
		// }
		
		// void KilledUnit(Unit* victim) override
		// {
			// if (victim->GetTypeId() == TYPEID_PLAYER)
				// Talk(SAY_KILL);
		// }
		
		// void JustDied(Unit* /*killer*/) override
		// {
			// events.Reset();
			// Talk(SAY_DIED);
			// summons.DespawnAll();
			// instance->SetBossState(DATA_WHITEMANE_CUSTOM, DONE);
			
			// std::list<Player*> playerList;
			// GetPlayerListInGrid(playerList,me, 300.0f);
			
            // for (auto itr : playerList)
            // {
                // itr->RemoveAura(SPELL_HOLY_WRATH);
                // itr->RemoveAura(SPELL_CRUSADER_STRIKE);
            // }
		// }
		
        // void UpdateAI(uint32 diff) override
        // {
            // if (!UpdateVictim())
                // return;

            // events.Update(diff);

            // if (me->HasUnitState(UNIT_STATE_CASTING))
                // return;

            // if (me->HealthBelowPct(70) && events.IsInPhase(PHASE_1))
            // {
                // events.SetPhase(PHASE_2);
                // events.ScheduleEvent(EVENT_SMITE, Seconds(6), 0, PHASE_2);
                // events.ScheduleEvent(EVENT_SHIELD, Seconds(30), 0, PHASE_2);
                // events.ScheduleEvent(EVENT_HOLY_FIRE, Seconds(14), PHASE_2);
                // events.ScheduleEvent(EVENT_HEAL, Seconds(65), PHASE_2);
                // events.ScheduleEvent(EVENT_HALO, Seconds(55), PHASE_2);
                // events.ScheduleEvent(EVENT_DIVINE_STAR, Seconds(20), PHASE_2);
            // }

            // if (me->HealthBelowPct(50) && events.IsInPhase(PHASE_2))
            // {
                // events.SetPhase(INTERPHASE);
                // Talk(SAY_INTERPHASE);
                // me->GetMotionMaster()->MovePoint(1, CombatZone[0]);
                // if (me->GetPosition() == CombatZone[0])
                // {
                    // me->SetReactState(REACT_PASSIVE);
                    // me->HandleEmoteCommand(EMOTE_ONESHOT_SPELL_CAST_OMNI);
                    // events.ScheduleEvent(EVENT_SUMM_CRUSADER1, Seconds(10), 0, INTERPHASE);
                    // events.ScheduleEvent(EVENT_SUMM_CRUSADER2, Seconds(20), 0, INTERPHASE);
                    // events.ScheduleEvent(EVENT_SUMM_CRUSADER3, Seconds(30), 0, INTERPHASE);
                // }
				
            // }

                // if (me->HealthBelowPct(45) && events.IsInPhase(INTERPHASE))
                // {
                    // events.SetPhase(PHASE_2);
                    // me->SetReactState(REACT_AGGRESSIVE);
                // }

                // if (me->HealthBelowPct(20) && events.IsInPhase(PHASE_2))
                // {
                    // events.SetPhase(PHASE_3);
                    // me->AddAura(SPELL_SHADOWFORM, me);
                    // events.ScheduleEvent(EVENT_MIND_BAST, Seconds(9), 0, PHASE_3);
                    // events.ScheduleEvent(EVENT_SHIELD, Seconds(30), 0, PHASE_3);
                    // events.ScheduleEvent(EVENT_MIND_FLAY, Seconds(12), PHASE_3);
                    // events.ScheduleEvent(EVENT_MIND_SEAR, Seconds(22), PHASE_3);
                    // events.ScheduleEvent(EVENT_HALO, Seconds(55), PHASE_3);
                    // events.ScheduleEvent(EVENT_DIVINE_STAR, Seconds(20), PHASE_3);
					// events.ScheduleEvent(EVENT_MIND_SPIKE, Seconds(3), PHASE_3);
                // }

                // while (uint32 eventId = events.ExecuteEvent())
                // {
                    // switch (eventId)
                    // {
                       // case EVENT_SMITE:
                       // {
                          // int32 damage = 2500000;
                          // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                          // {
                             // me->CastCustomSpell(target, SPELL_SMITE, &damage, NULL, NULL, false);
                          // }
                          // events.ScheduleEvent(EVENT_SMITE, Seconds(6), 0, PHASE_1);
						  // events.ScheduleEvent(EVENT_SMITE, Seconds(6), 0, PHASE_2);
                       // }
                       // break;
                       // case EVENT_SHIELD:
                       // {
                          // int32 absorb = 30000000;
                          // me->CastCustomSpell(me, SPELL_SHIELD, &absorb, NULL, NULL, false);
                          // events.Repeat(Seconds(30));
                       // }
                       // break;
					   // case EVENT_MIND_SPIKE:
					   // {
						   // int32 damage = 1500000;
						   // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
							   // me->CastCustomSpell(target, SPELL_MIND_SPIKE, &damage, NULL, NULL, false);
						   // events.Repeat(Seconds(3));
					   // }
					   // break;
                       // // case EVENT_DIVINE_STAR:
                       // // {
                          // // int32 damage = 1500000;
                          // // me->CastCustomSpell(me, SPELL_DIVINE_STAR, &damage, NULL, NULL, false);
                          // // events.Repeat(Seconds(20));
                       // // }
                       // // break;
                       // // case EVENT_HALO:
                       // // {
                          // // int32 damage = 4000000;
                          // // me->CastCustomSpell(me, SPELL_HALO, &damage, NULL, NULL, true);
                          // // events.Repeat(Seconds(55));
                       // // }
                       // // break;
                       // case EVENT_HOLY_FIRE:
                       // {
                          // int32 damage = 6000000;
                          // me->CastCustomSpell(me->GetVictim(), SPELL_HOLY_FIRE, &damage, NULL, NULL, false);
                          // events.ScheduleEvent(EVENT_SMITE, Seconds(14), 0, PHASE_1);
						  // events.ScheduleEvent(EVENT_SMITE, Seconds(14), 0, PHASE_2);
                       // }
                       // break;
                       // case EVENT_HEAL:
                       // {
                          // int32 heal = 60000000;
                          // me->CastCustomSpell(me, SPELL_HEAL, &heal, NULL, NULL, false);
                          // events.Repeat(Seconds(60));
                       // }
                       // break;
                       // case EVENT_HOLY_WRATH:
                       // {
                          // int32 damage = 30000000;
                          // me->CastCustomSpell(me, SPELL_HOLY_WRATH, &damage, NULL, NULL, false);
                       // }
                       // break;
                       // case EVENT_MIND_BAST:
                       // {
                           // int32 damage = 3000000;
                           // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           // {
                              // me->CastCustomSpell(target, SPELL_MIND_BLAST, &damage, NULL, NULL, false);
                           // }
                           // events.Repeat(Seconds(9));
                       // }
                       // break;
                       // case EVENT_MIND_FLAY:
                       // {
                           // int32 damage = 650000;
                           // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           // {
                              // me->CastCustomSpell(target, SPELL_MIND_FLAY, &damage, NULL, NULL, false);
                           // }
                           // events.Repeat(Seconds(12));
                       // }
                       // break;
                       // case EVENT_MIND_SEAR:
                       // {
                           // int32 damage = 500000;
 
                           // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           // {
                               // me->CastCustomSpell(target, SPELL_MIND_SEAR, &damage, NULL, NULL, false);
                           // }
                           // events.Repeat(Seconds(25));
                       // }
                       // break;
                       // case EVENT_WORD_POWER_PAIN:
                       // {
                           // int32 damage = 700000;
                           // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           // {
                              // me->CastCustomSpell(target, SPELL_WORD_POWER_PAIN, &damage, NULL, NULL, false);
                           // }
                           // events.Repeat(Seconds(14));
                       // }
                       // case EVENT_SUMM_CRUSADER1:
                       // {
                          // me->SummonCreature(735901, SummonCrusaders[0], TEMPSUMMON_MANUAL_DESPAWN);
                          // me->SummonCreature(735901, SummonCrusaders[1], TEMPSUMMON_MANUAL_DESPAWN);
						  // events.Repeat(Seconds(30));
                       // }
                       // break;
                       // case EVENT_SUMM_CRUSADER2:
                       // {
                          // me->SummonCreature(735902, SummonCrusaders[0], TEMPSUMMON_MANUAL_DESPAWN);
                          // me->SummonCreature(735902, SummonCrusaders[1], TEMPSUMMON_MANUAL_DESPAWN);
						  // events.Repeat(Seconds(30));
                       // }
                       // case EVENT_SUMM_CRUSADER3:
                       // {
                           // me->SummonCreature(735903, SummonCrusaders[0], TEMPSUMMON_MANUAL_DESPAWN);
                           // me->SummonCreature(735903, SummonCrusaders[1], TEMPSUMMON_MANUAL_DESPAWN);
						   // events.Repeat(Seconds(30));
                       // }
                       // break;
                     // default:
                       // break;
                    
                    // }
                // }
            
            // DoMeleeAttackIfReady();
        // }
            
	// };
// };

// enum NpcsEvents
// {
    // //Crusader
    // EVENT_CRUSADER_STRIKE    = 1,
    // EVENT_TEMPLAR_VERDICT    = 2,
    // EVENT_HAMMER_JUSTICE     = 3,
    // EVENT_SHIELD_VENGANCE    = 4,
    // //Priest
    // EVENT_SMITE_PRIEST       = 5,
    // EVENT_SHIELD_PRIEST      = 6,
    // EVENT_HEAL_PRIEST        = 7,
    // //Warrior
    // EVENT_SHOCKWAVE          = 8,
    // EVENT_COLOSSAL_SMASH     = 9,
    // EVENT_MORTAL_STRIKE      = 10,

// };


// class npc_scarlet_crusader : public CreatureScript
// {
// public:
   // npc_scarlet_crusader() : CreatureScript("npc_scarlet_crusader") { }
   
   // CreatureAI* GetAI(Creature* creature) const override
   // {
	   // return new npc_scarlet_crusaderAI(creature);
   // }
   
   // struct npc_scarlet_crusaderAI : public BossAI
   // {
	   // npc_scarlet_crusaderAI(Creature* creature) : BossAI(creature, NPC_SCARLET_CRUSADER)
	   // {
		   // instance = creature->GetInstanceScript();
	   // }
	   
	   // InstanceScript* instance;
	   
	  
	   // void Reset() override
	   // {
           // _Reset();
	   // }

       // void EnterCombat(Unit* who) override
       // {
           // _EnterCombat();
           // instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
           // DoZoneInCombat();
           // events.ScheduleEvent(EVENT_CRUSADER_STRIKE, Seconds(6));
           // events.ScheduleEvent(EVENT_TEMPLAR_VERDICT, Seconds(14));
           // events.ScheduleEvent(EVENT_SHIELD_VENGANCE, Seconds(30));
           // events.ScheduleEvent(EVENT_HAMMER_JUSTICE, Minutes(1));
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
               // switch (eventId)
               // {
                    // case EVENT_CRUSADER_STRIKE:
                    // {
                        // int32 damage = 1300000;
                        // Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO);
                        // me->CastCustomSpell(target, SPELL_CRUSADER_STRIKE, &damage, NULL, NULL, false);
                        // events.Repeat(Seconds(6));
                    // }
                    // break;
                    // case EVENT_SHIELD_VENGANCE:
                    // {
                        // int32 absorb = 20000000;
                        // me->CastCustomSpell(me, SPELL_SHIELD_VENGANCE, &absorb, NULL, NULL, false);
                        // events.Repeat(Seconds(30));
                    // }
                    // break;
                    // case EVENT_HAMMER_JUSTICE:
                    // {
                        // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            // DoCast(target, SPELL_HAMMER_JUSTICE);
                        // events.Repeat(Minutes(1));
                    // }
                    // break;
                    // case EVENT_TEMPLAR_VERDICT:
                    // {
                        // int32 damage = 2000000;
                        // me->CastCustomSpell(me->GetVictim(), SPELL_TEMPLAR_VERDICT, &damage, NULL, NULL, true);
                        // events.Repeat(Seconds(14));
                    // }
                    // break;
                  // default:
                    // break;

               // }
           // }
           // DoMeleeAttackIfReady();
	   // }
   // };
// };

// class npc_scarlet_priest : public CreatureScript
// {
// public:
    // npc_scarlet_priest() : CreatureScript("npc_scarlet_priest") { }
	
	// CreatureAI* GetAI(Creature* creature) const override
	// {
		// return new npc_scarlet_priestAI(creature);
	// }
	
	// struct npc_scarlet_priestAI : public BossAI
	// {
		// npc_scarlet_priestAI(Creature* creature) : BossAI(creature, NPC_SCARLET_PRIEST)
		// {
			// instance = creature->GetInstanceScript();
		// }
		
		// InstanceScript* instance;
		
		// void Reset() override
		// {
            // _Reset();
		// }

        // void EnterCombat(Unit* /*who*/) override
        // {
            // _EnterCombat();
            // instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
            // DoZoneInCombat();
            // events.ScheduleEvent(EVENT_SMITE_PRIEST, Seconds(5));
            // events.ScheduleEvent(EVENT_SHIELD_PRIEST, Seconds(30));
            // events.ScheduleEvent(EVENT_HEAL_PRIEST, Seconds(40));
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
                // switch (eventId)
                // {
                    // case EVENT_SMITE_PRIEST:
                    // {
                       // int32 damage = 900000;
                       // if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           // me->CastCustomSpell(target, SPELL_SMITE, &damage, NULL, NULL, false);
                       // events.Repeat(Seconds(5));
                    // }
                    // break;
                    // case EVENT_SHIELD_PRIEST:
                    // {
                       // int32 absorb = 7000000;
                       // me->CastCustomSpell(me, SPELL_SHIELD, &absorb, NULL, NULL, false);
                       // events.Repeat(Seconds(30));
                    // }
                    // break;
                    // case EVENT_HEAL_PRIEST:
                    // {
                        // int32 heal = 7000000;
                        // if (me->HealthBelowPct(40))
                            // me->CastCustomSpell(me, SPELL_HEAL, &heal, NULL, NULL, false);
                         // events.Repeat(Seconds(40));
                    // }
                    // break;
                  // default:
                    // break;
                // } 
            // }
            // DoMeleeAttackIfReady();
		// }
	// };
// };

// class npc_scarlet_warrior : public CreatureScript
// {
// public:
   // npc_scarlet_warrior() : CreatureScript("npc_scarlet_warrior") { }
   
   // CreatureAI* GetAI(Creature* creature) const override
   // {
	   // return new npc_scarlet_warriorAI(creature);
   // }
   
   // struct npc_scarlet_warriorAI : public BossAI
   // {
	   // npc_scarlet_warriorAI(Creature* creature) : BossAI(creature, NPC_SCARLET_WARRIOR)
	   // {
		   // instance = creature->GetInstanceScript();
	   // }
	   
	   // InstanceScript* instance;
	   
	   
       // void Reset() override
       // {
           // _Reset();
       // }

       // void EnterCombat(Unit* /*who*/) override
       // {
           // _EnterCombat();
           // instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
           // DoZoneInCombat();
           // events.ScheduleEvent(EVENT_COLOSSAL_SMASH, Seconds(14));
           // events.ScheduleEvent(EVENT_MORTAL_STRIKE, Seconds(6));
           // events.ScheduleEvent(EVENT_SHOCKWAVE, Seconds(30));
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
               // switch (eventId)
               // {
                  // case EVENT_SHOCKWAVE:
                  // {
                      // int32 damage = 650000;
                      // me->CastCustomSpell(me, SPELL_SHOCKWAVE, &damage, NULL, NULL, false);
                      // events.Repeat(Seconds(20));
                  // }
                  // break;
                  // case EVENT_COLOSSAL_SMASH:
                  // {
                      // int32 damage = 1500000;
                      // me->CastCustomSpell(me->GetVictim(), SPELL_COLOSSAL_SMASH, &damage, NULL, NULL, false);
                      // events.Repeat(Seconds(16));
                  // }
                  // break;
                  // case EVENT_MORTAL_STRIKE:
                  // {
                      // int32 damage = 2000000;
                      // me->CastCustomSpell(me->GetVictim(), SPELL_MORTAL_STRIKE, &damage, NULL, NULL, true);
                      // events.Repeat(Seconds(6));
                  // }
               // }
           // }
           // DoMeleeAttackIfReady();
	   // }
   // };
// };


// //class spell_shield_of_vengance : public SpellScriptLoader
// //{
// //public:
// //    spell_shield_of_vengance() : SpellScriptLoader("spell_shield_of_vengance") { }
// //	
// //	class spell_shield_of_vengance_AuraScript : public AuraScript
// //	{
// //		PrepareAuraScript(spell_shield_of_vengance_AuraScript);
// //		
// //		bool Validate(SpellInfo const* /*spellInfo*/) override
// //		{
// //			return ValidateSpellInfo({ SPELL_SHIELD_VENGANCE });
// //		}
// //		
// //        void HandleOnRemove(AuraEffect /*aurEff*/, DamageInfo &damageInfo, uint32 &absorbAmount, uint32 finaldamage, AuraEffectHandleModes /*modes*/)
// //		{
// //            absorbAmount = damageInfo.GetDamage();
// //            
// //            
// //
// //              if (Unit* caster = GetCaster())
// //                if (Unit* playercaster = caster->ToPlayer())
// //                    if (Unit* target = ObjectAccessor::GetUnit(*playercaster, playercaster->GetGUID))
// //                        if (target->IsWithinDist(caster, 40.f))
// //                        {
// //                            finaldamage = (absorbAmount / target);
// //                           /* int32 damage = absorbAmount;
// //                           int _targetCount;
// //
// //                           std::list<ObjectGuid> targetShieldDamageplayerGUIDs;
// //                           _targetCount = 0;
// //
// //
// //                           int32 finaldamage = damage / ;*/
// //                           caster->CastCustomSpell(target, SPELL_SHIELD_VENGANCE, &finaldamage, NULL, NULL, true);
// //                        }*/
// //                        
// //
// //            
// //		}
// //		
// //		void Register() override
// //		{
// //		}
// //	};
// //	
// //	AuraScript* GetAuraScript() const override
// //	{
// //		return new spell_shield_of_vengance_AuraScript();
// //	}
// //};

// void AddSC_boss_whitemane_custom()
// {
	// new boss_whitemane_custom();
	// new npc_scarlet_crusader();
	// new npc_scarlet_priest();
	// new npc_scarlet_warrior();
	// //Spells
	// //new spell_shield_of_vengance();
// }
