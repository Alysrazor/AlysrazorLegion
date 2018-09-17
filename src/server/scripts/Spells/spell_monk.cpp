/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
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

/*
 * Scripts for spells with SPELLFAMILY_MONK and SPELLFAMILY_GENERIC spells used by monk players.
 * Scriptnames of files in this file should be prefixed with "spell_monk_".
 */

#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Unit.h"
#include "Pet.h"
#include "WorldSession.h"
#include "Player.h"

enum MonkSpells
{
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL         = 117952,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHI_PROC        = 123333,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK       = 117962,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD    = 117953,
	SPELL_MONK_FISTS_OF_FURY                            = 113656,
	SPELL_MONK_FISTS_OF_FURY_DAMAGE                     = 117418,
    SPELL_MONK_PROVOKE_SINGLE_TARGET                    = 116189,
    SPELL_MONK_PROVOKE_AOE                              = 118635,
    SPELL_MONK_SOOTHING_MIST                            = 115175,
    SPELL_MONK_STANCE_OF_THE_SPIRITED_CRANE             = 154436,
    SPELL_MONK_SURGING_MIST_HEAL                        = 115175,
	SPELL_MONK_SOOTHING_MIST_PASIVE                     = 193884,
	SPELL_MONK_EFFUSE                                   = 116694,
	SPELL_MONK_ENVELOPING_MIST                          = 124682,
	SPELL_MONK_VIVIFY                                   = 116670,
	SPELL_MONK_LIFE_COCOON                              = 116849,
};
// 101643 - Transcendence
class spell_transcendence : public SpellScriptLoader
{
public:
    spell_transcendence() : SpellScriptLoader("spell_transcendence") { }

    class SwiftmendSpellScript : public SpellScript
    {
        PrepareSpellScript(SwiftmendSpellScript);

        void HandleAfterCast()
        {
            // upon on hit make all the chains spells
            if (!GetCaster())
                return;
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            // Remove Current Trans
            if (Creature* CurrentTrasendanceTrigger = player->FindNearestCreature(54569, 500.0f, true))
            {
                if (CurrentTrasendanceTrigger->IsSummon())
                    CurrentTrasendanceTrigger->DespawnOrUnsummon(500);
            }

            // Summons new one
            Pet* TriggerTransdence = player->SummonPet(54569, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), MAX_PET_TYPE, 900000);
      
            TriggerTransdence->Respawn();
            TriggerTransdence->SetFullHealth();
			TriggerTransdence->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			TriggerTransdence->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);


            //Stun Movement
            TriggerTransdence->CastSpell(TriggerTransdence, 52165);

            //Clone player's look
            player->CastSpell(TriggerTransdence, 45204);

            //Visual meditation effect
            TriggerTransdence->CastSpell(TriggerTransdence, 119053);

            // gliff of fighting pose
            if (player->HasAura(125872))
                TriggerTransdence->HandleEmoteCommand(27);

        }

        void Register()
        {
            AfterCast += SpellCastFn(SwiftmendSpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new SwiftmendSpellScript();
    }
};
/*SPELL_MONK_EFFUSE                                   = 116694,
	SPELL_MONK_ENVELOPING_MIST                          = 124682,
	SPELL_MONK_VIVIFY                                   = 116670,
	SPELL_MONK_LIFE_COCOON                              = 116849,*/
class spell_monk_soothing_mist : SpellScriptLoader
{
public:
	 spell_monk_soothing_mist() : SpellScriptLoader("spell_monk_soothing_mist") { }
	 
	 class spell_monk_effuse : public SpellScript
	 {
         PrepareSpellScript(spell_monk_effuse);

		 bool Validate(SpellInfo const* /*spellInfo*/) override
		 {
			 return ValidateSpellInfo({ SPELL_MONK_EFFUSE });
		 }
		 
		 void HandleAfterCast()
		 {
			 Unit* caster = GetCaster();
             Unit* target = GetExplTargetUnit();
			 
			 if (!caster)
				 return;
			 
			 if (!target)
				 caster->CastSpell(caster, SPELL_MONK_SOOTHING_MIST, true);
			 else if (target->IsValidAssistTarget(caster))
				 caster->CastSpell(target, SPELL_MONK_SOOTHING_MIST, true);
		 }
		 
		 void Register() override
		 {
             AfterCast += SpellCastFn(spell_monk_effuse::HandleAfterCast);
		 }
     };
	 
	 class spell_monk_enveloping_mist : public SpellScript
	 {
         PrepareSpellScript(spell_monk_enveloping_mist);

		 bool Validate(SpellInfo const* /*spellInfo*/) override
		 {
			 return ValidateSpellInfo({ SPELL_MONK_ENVELOPING_MIST });
		 }
		 
         void HandleAfterCast()
         {
             Unit* caster = GetCaster();
             Unit* target = GetExplTargetUnit();

             if (!caster)
                 return;

             if (!target)
                 caster->CastSpell(caster, SPELL_MONK_SOOTHING_MIST, true);
             else if (target->IsValidAssistTarget(caster))
                 caster->CastSpell(target, SPELL_MONK_SOOTHING_MIST, true);
         }
		 
		 void Register() override
		 {
			 AfterCast += SpellCastFn(spell_monk_enveloping_mist::HandleAfterCast);
		 }
     };
	 
	 class spell_monk_vivify : public SpellScript
	 {
         PrepareSpellScript(spell_monk_vivify);

		 bool Validate(SpellInfo const* /*spellInfo*/) override
		 {
			 return ValidateSpellInfo({ SPELL_MONK_VIVIFY });
		 }
		 
         void HandleAfterCast()
         {
             Unit* caster = GetCaster();
             Unit* target = GetExplTargetUnit();

             if (!caster)
                 return;

             if (!target)
                 caster->CastSpell(caster, SPELL_MONK_SOOTHING_MIST, true);
             else if (target->IsValidAssistTarget(caster))
                 caster->CastSpell(target, SPELL_MONK_SOOTHING_MIST, true);
         }
		 
		 void Register() override
		 {
			 AfterCast += SpellCastFn(spell_monk_vivify::HandleAfterCast);
		 }
     };
	 
	 class spell_monk_life_cocoon : public SpellScript
	 {
         PrepareSpellScript(spell_monk_life_cocoon);

		 bool Validate(SpellInfo const* /*spellInfo*/) override
		 {
			 return ValidateSpellInfo({ SPELL_MONK_LIFE_COCOON });
		 }
		 
         void HandleAfterCast()
         {
             Unit* caster = GetCaster();
             Unit* target = GetExplTargetUnit();

             if (!caster)
                 return;

             if (!target)
                 caster->CastSpell(caster, SPELL_MONK_SOOTHING_MIST, true);
             else if (target->IsValidAssistTarget(caster))
                 caster->CastSpell(target, SPELL_MONK_SOOTHING_MIST, true);
         }
		 
		 void Register() override
		 {
			 AfterCast += SpellCastFn(spell_monk_life_cocoon::HandleAfterCast);
		 }
     };
	 
	 SpellScript* GetSpellScript() const override
	 {
		 return new spell_monk_effuse();
		 return new spell_monk_enveloping_mist();
		 return new spell_monk_vivify();
		 return new spell_monk_life_cocoon();
	 }
};
//119996
class spell_monk_transcendence_transfer : public SpellScriptLoader
{
public:
    spell_monk_transcendence_transfer() : SpellScriptLoader("spell_monk_transcendence_transfer") { }

    class spell_monk_transcendence_transfer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_transcendence_transfer_SpellScript);

        SpellCastResult CheckRange()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Pet* CurrentTrasendanceTrigger = _player->GetPet())
                {
                    if (CurrentTrasendanceTrigger->GetEntry() == 54569)
                    {
                        if (_player->IsWithinDistInMap(CurrentTrasendanceTrigger, GetSpellInfo()->GetMaxRange(true, GetCaster(), GetSpell()), true))
                            return SPELL_CAST_OK;
                    }
                }
            }
            return SPELL_FAILED_OUT_OF_RANGE;
        }

        void HandleAfterCast()
        {
            Player* player = GetCaster()->ToPlayer();

            if (Pet* CurrentTrasendanceTrigger = player->GetPet())
            {
                if (CurrentTrasendanceTrigger->GetEntry() == 54569)
                {
                    float X_Trigger = CurrentTrasendanceTrigger->GetPositionX();
                    float Y_Trigger = CurrentTrasendanceTrigger->GetPositionY();
                    float Z_Trigger = CurrentTrasendanceTrigger->GetPositionZ();
                    float O_Trigger = CurrentTrasendanceTrigger->GetOrientation();

                    CurrentTrasendanceTrigger->Relocate(player->GetPositionX(), player->GetPositionY());

                    if (MotionMaster* motion = CurrentTrasendanceTrigger->GetMotionMaster())
                    {
                        CurrentTrasendanceTrigger->ClearUnitState(UNIT_STATE_ROOT);
                        CurrentTrasendanceTrigger->SetSpeed(MOVE_RUN, 100.0f);
                        motion->MovePoint(1, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());
                        CurrentTrasendanceTrigger->SetSpeed(MOVE_RUN, 0.0f);
                        CurrentTrasendanceTrigger->AddUnitState(UNIT_STATE_ROOT);

                    }
                    // player
                    player->TeleportTo(CurrentTrasendanceTrigger->GetMapId(), X_Trigger, Y_Trigger, Z_Trigger, O_Trigger);
                }
            }
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_monk_transcendence_transfer_SpellScript::CheckRange);
            AfterCast += SpellCastFn(spell_monk_transcendence_transfer_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_transcendence_transfer_SpellScript();
    }
};
// 178345 - Fists of Fury
class spell_monk_fists_of_fury : public SpellScript
{
	PrepareSpellScript(spell_monk_fists_of_fury);
	
	bool Validate(SpellInfo const* /*spellInfo*/) override
	{
		return ValidateSpellInfo({ SPELL_MONK_FISTS_OF_FURY });
    }
	
	void HandleOnCast()
	{
		Unit* caster = GetCaster();
        Unit* target = GetExplTargetUnit();
		
		if (!caster)
			return;

        if (!target || target)
            caster->CastSpell(caster, SPELL_MONK_FISTS_OF_FURY);
	}

    void HandleOnHit()
    {
        Unit* caster = GetCaster();

        if (!caster->GetTarget())
            return;
    }

	
	void Register() override
	{
		OnCast += SpellCastFn(spell_monk_fists_of_fury::HandleOnCast);
        OnHit += SpellHitFn(spell_monk_fists_of_fury::HandleOnHit);
	}
};
	
	
// 117952 - Crackling Jade Lightning
class spell_monk_crackling_jade_lightning : public AuraScript
{
    PrepareAuraScript(spell_monk_crackling_jade_lightning);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_MONK_STANCE_OF_THE_SPIRITED_CRANE,
            SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHI_PROC
        });
    }

    void OnTick(AuraEffect const* /*aurEff*/)
    {
        if (Unit* caster = GetCaster())
            if (caster->HasAura(SPELL_MONK_STANCE_OF_THE_SPIRITED_CRANE))
                caster->CastSpell(caster, SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHI_PROC, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_crackling_jade_lightning::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// 117959 - Crackling Jade Lightning
class spell_monk_crackling_jade_lightning_knockback_proc_aura : public AuraScript
{
    PrepareAuraScript(spell_monk_crackling_jade_lightning_knockback_proc_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK,
            SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD
        });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (GetTarget()->HasAura(SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD))
            return false;

        if (eventInfo.GetActor()->HasAura(SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL, GetTarget()->GetGUID()))
            return false;

        Spell* currentChanneledSpell = GetTarget()->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
        if (!currentChanneledSpell || currentChanneledSpell->GetSpellInfo()->Id != SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL)
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        GetTarget()->CastSpell(eventInfo.GetActor(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK, TRIGGERED_FULL_MASK);
        GetTarget()->CastSpell(GetTarget(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK_CD, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_crackling_jade_lightning_knockback_proc_aura::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_monk_crackling_jade_lightning_knockback_proc_aura::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 115546 - Provoke
class spell_monk_provoke : public SpellScript
{
    PrepareSpellScript(spell_monk_provoke);

    static uint32 const BlackOxStatusEntry = 61146;

    bool Validate(SpellInfo const* spellInfo) override
    {
        if (!(spellInfo->GetExplicitTargetMask() & TARGET_FLAG_UNIT_MASK)) // ensure GetExplTargetUnit() will return something meaningful during CheckCast
            return false;
        return ValidateSpellInfo(
        {
            SPELL_MONK_PROVOKE_SINGLE_TARGET,
            SPELL_MONK_PROVOKE_AOE
        });
    }

    SpellCastResult CheckExplicitTarget()
    {
        if (GetExplTargetUnit()->GetEntry() != BlackOxStatusEntry)
        {
            SpellInfo const* singleTarget = sSpellMgr->AssertSpellInfo(SPELL_MONK_PROVOKE_SINGLE_TARGET);
            SpellCastResult singleTargetExplicitResult = singleTarget->CheckExplicitTarget(GetCaster(), GetExplTargetUnit());
            if (singleTargetExplicitResult != SPELL_CAST_OK)
                return singleTargetExplicitResult;
        }
        else if (GetExplTargetUnit()->GetOwnerGUID() != GetCaster()->GetGUID())
            return SPELL_FAILED_BAD_TARGETS;

        return SPELL_CAST_OK;
    }

    void HandleDummy(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        if (GetHitUnit()->GetEntry() != BlackOxStatusEntry)
            GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_PROVOKE_SINGLE_TARGET, true);
        else
            GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_PROVOKE_AOE, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_monk_provoke::CheckExplicitTarget);
        OnEffectHitTarget += SpellEffectFn(spell_monk_provoke::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

void AddSC_monk_spell_scripts()
{
    RegisterAuraScript(spell_monk_crackling_jade_lightning);
    RegisterAuraScript(spell_monk_crackling_jade_lightning_knockback_proc_aura);
    RegisterSpellScript(spell_monk_provoke);
    new spell_monk_fists_of_fury();
    new spell_transcendence();
    new spell_monk_transcendence_transfer();
	new spell_monk_soothing_mist();
}
