#include "angband.h"

static power_info _get_powers[] =
{
    { A_INT, { 25, 1, 90, eat_magic_spell}},
    { -1, {-1, -1, -1, NULL}}
};

static caster_info* _caster_info(void)
{
    static caster_info me = { 0 };
    static bool init = FALSE;
    if (!init)
    {
        me.magic_desc = "spell";
        me.which_stat = A_INT;
        me.encumbrance.max_wgt = 430;
        me.encumbrance.weapon_pct = 100;
        me.encumbrance.enc_wgt = 600;
        me.options = CASTER_ALLOW_DEC_MANA | CASTER_GLOVE_ENCUMBRANCE;
        init = TRUE;
    }
    return &me;
}

static void _birth(void)
{
    mut_gain(MUT_CHAOS_GIFT);
    mut_lock(MUT_CHAOS_GIFT);

    py_birth_obj_aux(TV_SWORD, SV_DAGGER, 1);
    py_birth_obj_aux(TV_SOFT_ARMOR, SV_ROBE, 1);
    py_birth_spellbooks();
}

class_t* logrus_master_get_class(void)
{
    static class_t me = { 0 };
    static bool init = FALSE;

    if (!init)
    {           /* dis, dev, sav, stl, srh, fos, thn, thb */
        skills_t bs = { 28,  40,  38,   3,  15,  20,  35,  20 };
        skills_t xs = { 7,  15,  11,   0,   0,   0,   6,   7 };

        me.name = "Logrus-Master";
        me.desc = "Logrus Masters are the arcane students of the terrible Demon Lords "
			"of Chaos. Every Logrus Master has a Patron Demon and, when "
			"gaining a level, may receive a reward from his Patron. He might "
			"be healed or polymorphed, his stats could be increased, or he "
			"might be rewarded with an awesome weapon. On the other hand, the "
			"Patrons might surround him with monsters, drain his stats or wreck "
			"his equipment or they might simply ignore him. The Demon Lords of "
			"Chaos are chaotic and unpredictable indeed. The exact type of "
			"reward depends on both the Patron Demon (different Demons give "
			"different rewards) and chance.\n \n"
			"Logrus Masters can select a realm from Chaos and Daemon, and one "
			"other realm excluding Life and Crusade. A  Logrus Master's prime statistic is "
			"Intelligence as this determines his spell casting ability.\n \n"
			"Logrus Masters are a little hardier than ordinary mages but less perceptive. "
			"They have a class powers - 'Eat Magic' - which absorbs mana "
			"from wands, staves or rods.";

        me.stats[A_STR] = -3;
        me.stats[A_INT] = 3;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] = 0;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] = -2;
        me.base_skills = bs;
        me.extra_skills = xs;
        me.life = 95;
        me.base_hp = 0;
        me.exp = 135;
        me.pets = 30;
        me.flags = CLASS_SENSE1_MED | CLASS_SENSE1_WEAK |
            CLASS_SENSE2_FAST | CLASS_SENSE2_STRONG |
            CLASS_REGEN_MANA;

        me.birth = _birth;
        me.caster_info = _caster_info;
        /* TODO: This class uses spell books, so we are SOL
        me.get_spells = _get_spells;*/
        me.character_dump = spellbook_character_dump;
        me.get_powers = _get_powers;
        init = TRUE;
    }

    return &me;
}
