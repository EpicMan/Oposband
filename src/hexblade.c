#include "angband.h"

static power_info _get_powers[] =
{
    { A_NONE, { 1, 0,  0, hex_stop_spelling_spell}},
    { -1, {-1, -1, -1, NULL}}
};

static caster_info * _caster_info(void)
{
	static caster_info me = { 0 };
	static bool init = FALSE;
	if (!init)
	{
		me.magic_desc = "spell";
		me.which_stat = A_CHR;
		me.encumbrance.max_wgt = 430;
		me.encumbrance.weapon_pct = 33;
		me.encumbrance.enc_wgt = 1200;
		init = TRUE;
	}
	return &me;
}

static void _birth(void)
{
    py_birth_obj_aux(TV_SWORD, SV_SHORT_SWORD, 1);
    py_birth_obj_aux(TV_SOFT_ARMOR, SV_SOFT_LEATHER_ARMOR, 1);
	py_birth_spellbooks();
}

static void _calc_bonuses(void)
{
	p_ptr->align -= 200;
}

class_t *hexblade_get_class(void)
{
	static class_t me = { 0 };
	static bool init = FALSE;

	if (!init)
	{           /* dis, dev, sav, stl, srh, fos, thn, thb */
		skills_t bs = { 30,  35,  36,   2,  18,  16,  50,  50 };
		skills_t xs = { 7,  10,  10,   0,   0,   0,  15,  15 };

		me.name = "Hexblade";
		me.desc = "A Hexblade is an evil fighter who employs hex magic.";

		me.stats[A_STR] = 2;
		me.stats[A_INT] = 1;
		me.stats[A_WIS] = 0;
		me.stats[A_DEX] = 1;
		me.stats[A_CON] = 0;
		me.stats[A_CHR] = 2;
		me.base_skills = bs;
		me.extra_skills = xs;
		me.life = 105;
		me.base_hp = 8;
		me.exp = 140;
		me.pets = 35;
		me.flags = CLASS_SENSE1_MED | CLASS_SENSE1_WEAK |
			CLASS_SENSE2_MED | CLASS_SENSE2_STRONG;
		me.calc_bonuses = _calc_bonuses;
		me.birth = _birth;
		me.caster_info = _caster_info;
		/* TODO: This class uses spell books, so we are SOL
		me.get_spells = _get_spells;*/
		me.get_powers = _get_powers;
		me.character_dump = spellbook_character_dump;
		init = TRUE;
	}

	return &me;
}
