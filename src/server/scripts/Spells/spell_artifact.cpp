/*
* Scripted By Alysrazor
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
#include "Containers.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "TemporarySummon.h"

enum DamageDruidSpells
{
	SPELL_DRUID_SOLAR_WRATH            = 190984,
	SPELL_DRUID_LUNAR_STRIKE           = 194153,
	SPELL_DRUID_STARSURGE              = 78674,
	SPELL_DRUID_MOONFIRE               = 8921,
	SPELL_DRUID_SUNFIRE                = 93402,
};
enum ScytheOfEluneSpells
{
	///Scythe Basic Attack Spells
	SPELL_DRUID_NEW_MOON_ADD           = 214842,
	SPELL_DRUID_NEW_MOON               = 202767,
	SPELL_DRUID_NEW_MOON_OVERRIDE      = 202787,
	SPELL_DRUID_HALF_MOON              = 202768,
	SPELL_DRUID_HALF_MOON_OVERRIDE     = 202788,
	SPELL_DRUID_FULL_MOON              = 202771,
	SPELL_DRUID_FULL_MOON_OVERRIDE     = 202789,
	///Artefact Trades
	SPELL_DRUID_CELESTIA_ALIGNMENT     = 194223,
	SPELL_DRUID_MOON_AND_STARS         = 202940,
	SPELL_DRUID_MOON_AND_STARS_BUFF    = 202941,
};

// New Moon Add - 214842
class spell_arti_dru_new_moon_learn : public SpellScriptLoader
{
public:
	spell_arti_dru_new_moon_learn() : SpellScriptLoader("spell_arti_dru_new_moon_learn") { }

	class spell_arti_dru_new_moon_add : public AuraScript
	{
		PrepareAuraScript(spell_arti_dru_new_moon_add);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			return ValidateSpellInfo({ SPELL_DRUID_NEW_MOON_ADD });
		}

		void HandleApply(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
		{
			Player* player = GetCaster()->ToPlayer();
			if (!player)
				return;

			player->LearnSpell(SPELL_DRUID_NEW_MOON, true);

		}

		void HandleRemove(AuraEffect const* /*aurEffect*/, AuraEffectHandleModes /*mode*/)
		{
			Player* player = GetCaster()->ToPlayer();
			if (!player)
				return;

			player->RemoveSpell(SPELL_DRUID_NEW_MOON);
		}

		void Register() override
		{
			OnEffectApply += AuraEffectApplyFn(spell_arti_dru_new_moon_add::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
			OnEffectRemove += AuraEffectRemoveFn(spell_arti_dru_new_moon_add::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
		}
	};
};

class spell_arti_dru_new_moon : public SpellScript
{
	PrepareSpellScript(spell_arti_dru_new_moon);

	bool Validate(SpellInfo const* /*spellInfo*/) override
	{
		return ValidateSpellInfo({ SPELL_DRUID_NEW_MOON, SPELL_DRUID_NEW_MOON_OVERRIDE });
	}

	void RemoveOverride()
	{
		Player* player = GetCaster()->ToPlayer();

		player->AddAura(SPELL_DRUID_NEW_MOON_OVERRIDE, player);
	}

	void Register() override
	{
		AfterCast += SpellCastFn(spell_arti_dru_new_moon::RemoveOverride);
	}
};

// Half Moon - 202768
class spell_arti_dru_half_moon : public SpellScript
{
	PrepareSpellScript(spell_arti_dru_half_moon);

	bool Validate(SpellInfo const* /*spellInfo*/) override
	{
		return ValidateSpellInfo({ SPELL_DRUID_HALF_MOON, SPELL_DRUID_HALF_MOON_OVERRIDE });
	}

	void RemoveOverride()
	{
		Player* player = GetCaster()->ToPlayer();

		player->AddAura(SPELL_DRUID_HALF_MOON_OVERRIDE, player);
	}

	void Register() override
	{
		AfterCast += SpellCastFn(spell_arti_dru_half_moon::RemoveOverride);
	}
};

//Full Moon - 202771
class spell_arti_dru_full_moon : public SpellScript
{
	PrepareSpellScript(spell_arti_dru_full_moon);

	bool Validate(SpellInfo const* /*spellInfo*/) override
	{
		return ValidateSpellInfo({ SPELL_DRUID_FULL_MOON, SPELL_DRUID_FULL_MOON_OVERRIDE });
	}

	void RemoveOverride()
	{
		Player* player = GetCaster()->ToPlayer();
		
		player->RemoveAurasDueToSpell(SPELL_DRUID_HALF_MOON_OVERRIDE);
		player->RemoveAurasDueToSpell(SPELL_DRUID_NEW_MOON_OVERRIDE);
	}

	void Register() override
	{
		AfterCast += SpellCastFn(spell_arti_dru_full_moon::RemoveOverride);
	}
};

class spell_arti_dru_moon_and_stars : public SpellScriptLoader
{
public:
	spell_arti_dru_moon_and_stars() : SpellScriptLoader("spell_arti_dru_moon_and_stars") { }

    class spell_arti_dru_solar_wrath : public SpellScript
    {
        PrepareSpellScript(spell_arti_dru_solar_wrath);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_SOLAR_WRATH, SPELL_DRUID_MOON_AND_STARS_BUFF });
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->AddAura(SPELL_DRUID_MOON_AND_STARS_BUFF,caster);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_arti_dru_solar_wrath::HandleAfterCast);
        }
    };

    class spell_arti_dru_lunar_strike : public SpellScript
    {
        PrepareSpellScript(spell_arti_dru_lunar_strike);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_DRUID_LUNAR_STRIKE, SPELL_DRUID_MOON_AND_STARS_BUFF });
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->AddAura(SPELL_DRUID_MOON_AND_STARS_BUFF, caster);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_arti_dru_lunar_strike::HandleAfterCast);
        }
    };

	SpellScript* GetSpellScript() const override
	{
		return new spell_arti_dru_solar_wrath();
        return new spell_arti_dru_lunar_strike();
	}
};
void AddSC_spell_artifact()
{
	new spell_arti_dru_moon_and_stars();
	new spell_arti_dru_new_moon_learn();
	new spell_arti_dru_new_moon();
	new spell_arti_dru_half_moon();
	new spell_arti_dru_full_moon();
}
