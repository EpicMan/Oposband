#include "angband.h"

#include <assert.h>

static bool _split_recalc_bonuses = TRUE;
static byte _split_status[MAX_PERSONALITIES];
static byte _split_dominant = 0;
static bool _split_status_initialized = FALSE;

#define SPLIT_NONE 0
#define SPLIT_SUPPRESSED 1
#define SPLIT_NEGATIVE 2
#define SPLIT_DOMINANT 3

bool personality_is_(int idx)
{
    if (p_ptr->personality == PERS_SPLIT) 
        return ((idx == PERS_SPLIT) || ((idx >= 0) && (idx < MAX_PERSONALITIES) && (_split_status[idx] == SPLIT_DOMINANT)));
    return (p_ptr->personality == idx);
}

bool personality_includes_(int idx)
{
    if (p_ptr->personality == PERS_SPLIT)    
        return ((idx == PERS_SPLIT) || ((idx >= 0) && (idx < MAX_PERSONALITIES) && (_split_status[idx] != SPLIT_NONE)));
    return (p_ptr->personality == idx);
}


/****************************************************************
 * Combat
 ****************************************************************/
static personality_ptr _get_combat_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Combat";
        me.desc = "Combative adventurers favor melee and shooting at the expense "
                    "of other skills such as disarming, device-skill, or saving throw; "
                    "yet this personality has more subtlety to it than the raw, untempered "
                    "muscle of Mighty.";

        me.stats[A_STR] =  1;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] =  0;
        me.stats[A_CHR] =  0;

        me.skills.dis = -2;
        me.skills.dev = -2;
        me.skills.sav = -3;
        me.skills.stl =  0;
        me.skills.srh = -1;
        me.skills.fos =  2;
        me.skills.thn =  5;
        me.skills.thb =  3;

        me.life = 100;
        me.exp = 100;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Craven
 ****************************************************************/
static void _craven_calc_bonuses(void)
{
    res_add_vuln(RES_FEAR);
}
static void _craven_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_VULN_FEAR);
}

static personality_ptr _get_craven_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Craven";
        me.desc = "A Craven person is a coward, preferring to avoid a fight at any "
                    "cost. Craven adventurers shoot and use devices well, their "
                    "stealth is impressive, and they can move fast when not fighting; "
                    "but their stats and other skills are somewhat wanting.";

        me.stats[A_STR] = -1;
        me.stats[A_INT] =  1;
        me.stats[A_WIS] =  1;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] = -2;

        me.skills.dis =  5;
        me.skills.dev =  5;
        me.skills.sav = -1;
        me.skills.stl =  3;
        me.skills.srh =  0;
        me.skills.fos =  0;
        me.skills.thn =-10;
        me.skills.thb =  7;

        me.life = 99;
        me.exp = 100;

        me.calc_bonuses = _craven_calc_bonuses;
        me.get_flags = _craven_get_flags;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Fearless
 ****************************************************************/
static void _fearless_calc_bonuses(void)
{
    res_add(RES_FEAR);
}
static void _fearless_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_FEAR);
}
static personality_ptr _get_fearless_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Fearless";
        me.desc = "Being Fearless raises your melee skills and force of personality, besides helping you "
                    "resist fear; but your constitution and hit points are negatively affected.";

        me.stats[A_STR] =  1;
        me.stats[A_INT] =  0;
        me.stats[A_WIS] =  0;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] =  1;

        me.skills.dis = -5;
        me.skills.dev =  1;
        me.skills.sav = -2;
        me.skills.stl =  0;
        me.skills.srh =  2;
        me.skills.fos = -2;
        me.skills.thn =  5;
        me.skills.thb =  0;

        me.life = 98;
        me.exp = 100;

        me.calc_bonuses = _fearless_calc_bonuses;
        me.get_flags = _fearless_get_flags;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Fragile
 ****************************************************************/
static void _fragile_birth(void)
{
    if ((p_ptr->personality == PERS_SPLIT) && (_split_status[PERS_FRAGILE] < SPLIT_DOMINANT)) return;
    mut_gain(MUT_EASY_TIRING);
    mut_lock(MUT_EASY_TIRING);
    mut_gain(MUT_EASY_TIRING2);
    mut_lock(MUT_EASY_TIRING2);
}

static void _fragile_calc_bonuses(void)
{
    res_add_vuln(RES_FEAR);
}
static void _fragile_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_VULN_FEAR);
}

static personality_ptr _get_fragile_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Fragile";
        me.desc = "As a Fragile adventurer, you lack both self-confidence and physical toughness. "
                    "Due to your incredibly low stamina, you are quickly exhausted by melee, "
                    "or ranged combat, or magical combat, or any other strenuous activity. (Maybe you "
                    "should just stay at home, quietly reading some book that isn't too heavy.)";

        me.stats[A_STR] = -3;
        me.stats[A_INT] =  0;
        me.stats[A_WIS] =  1;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] = -3;
        me.stats[A_CHR] = -2;

        me.skills.dis =  0;
        me.skills.dev =  0;
        me.skills.sav = -4;
        me.skills.stl = -1;
        me.skills.srh =  2;
        me.skills.fos =  2;
        me.skills.thn = -3;
        me.skills.thb = -3;

        me.life = 91;
        me.exp = 100;

        me.calc_bonuses = _fragile_calc_bonuses;
        me.get_flags = _fragile_get_flags;
        me.birth = _fragile_birth;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Hasty
 ****************************************************************/
static void _hasty_calc_bonuses(void)
{
    p_ptr->pspeed += 2 + p_ptr->lev/37;
    p_ptr->to_m_chance += 1;
}
static void _hasty_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_SPEED);
}

static personality_ptr _get_hasty_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Hasty";
        me.desc = "A Hasty person endeavors to do all things quickly. Speed, "
                    "rather than skill or care, is paramount, and the Hasty "
                    "adventurer moves quickly through the dungeon, bungling much.";

        me.stats[A_STR] =  0;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] =  2;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] =  0;

        me.skills.dis = -5;
        me.skills.dev = -4;
        me.skills.sav = -3;
        me.skills.stl = -1;
        me.skills.srh = -4;
        me.skills.fos = -2;
        me.skills.thn = -5;
        me.skills.thb = -3;

        me.life = 100;
        me.exp = 100;

        me.calc_bonuses = _hasty_calc_bonuses;
        me.get_flags = _hasty_get_flags;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Lazy
 ****************************************************************/
static void _lazy_birth(void)
{
    p_ptr->au /= 2;
}
static void _lazy_calc_bonuses(void)
{
    p_ptr->to_m_chance += 10;
}
static personality_ptr _get_lazy_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Lazy";
        me.desc = "A Lazy person has no good stats and can do no action well. "
                    "All fail rates are negatively impacted. Yet some whisper "
                    "that the less hard you work, the greater the reward...";

        me.stats[A_STR] = -2;
        me.stats[A_INT] = -2;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] = -2;
        me.stats[A_CON] = -2;
        me.stats[A_CHR] = -2;

        me.skills.dis = -5;
        me.skills.dev = -5;
        me.skills.sav = -3;
        me.skills.stl = -1;
        me.skills.srh = -4;
        me.skills.fos = -2;
        me.skills.thn = -8;
        me.skills.thb = -5;

        me.life = 95;
        me.exp = 100;

        me.birth = _lazy_birth;
        me.calc_bonuses = _lazy_calc_bonuses;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Lucky
 ****************************************************************/
static void _lucky_birth(void)
{
    if ((p_ptr->personality == PERS_SPLIT) && (_split_status[PERS_LUCKY] < SPLIT_NEGATIVE)) return;
    mut_gain(MUT_GOOD_LUCK);
    mut_lock(MUT_GOOD_LUCK);
}
static personality_ptr _get_lucky_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Lucky";
        me.desc = "A Lucky adventurer has poor stats but, surprisingly, can do all "
                    "things well. For some reason, good things seem to happen "
                    "more often to lucky players.";

        me.stats[A_STR] = -2;
        me.stats[A_INT] = -2;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] = -2;
        me.stats[A_CON] = -2;
        me.stats[A_CHR] = -2;

        me.skills.dis = 10;
        me.skills.dev =  6;
        me.skills.sav =  3;
        me.skills.stl =  2;
        me.skills.srh = 10;
        me.skills.fos =  8;
        me.skills.thn = 15;
        me.skills.thb =  9;

        me.life = 98;
        me.exp = 100;

        me.birth = _lucky_birth;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Mighty
 ****************************************************************/
static void _mighty_calc_bonuses(void)
{
    p_ptr->to_m_chance += 1;
}
static personality_ptr _get_mighty_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Mighty";
        me.desc = "Mighty raises your physical stats, skills and stamina, but reduces "
                    "your magical abilities and gives a slight penalty to your spell "
                    "fail rates. It is the most warrior-like personality.";

        me.stats[A_STR] =  2;
        me.stats[A_INT] = -2;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] =  0;
        me.stats[A_CON] =  1;
        me.stats[A_CHR] =  1;

        me.skills.dis = -5;
        me.skills.dev = -4;
        me.skills.sav = -3;
        me.skills.stl = -1;
        me.skills.srh = -2;
        me.skills.fos = -2;
        me.skills.thn = 15;
        me.skills.thb =  0;

        me.life = 102;
        me.exp = 100;

        me.calc_bonuses = _mighty_calc_bonuses;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Munchkin
 ****************************************************************/
static void _munchkin_birth(void)
{
    p_ptr->au = 10 * 1000 * 1000;
}

static void _munchkin_calc_bonuses(void)
{
    p_ptr->auto_id = TRUE;
    res_add(RES_BLIND);
    res_add(RES_CONF);
    p_ptr->hold_life = TRUE;
    p_ptr->munchkin_pseudo_id = TRUE;
    if (!player_is_ninja)
        p_ptr->lite = TRUE;

    p_ptr->pspeed += p_ptr->lev/10 + 5;
}
static void _munchkin_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_BLIND);
    add_flag(flgs, OF_RES_CONF);
    add_flag(flgs, OF_HOLD_LIFE);
    if (!player_is_ninja)
        add_flag(flgs, OF_LITE);
    add_flag(flgs, OF_SPEED);
}
static personality_ptr _get_munchkin_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Munchkin";
        me.desc = "The Munchkin personality is for players who love being overpowered. "
                    "It raises all your stats and skills; with this personality, you can "
                    "win the game easily, but gain no honor in doing so.";

        me.stats[A_STR] = 3;
        me.stats[A_INT] = 3;
        me.stats[A_WIS] = 3;
        me.stats[A_DEX] = 3;
        me.stats[A_CON] = 3;
        me.stats[A_CHR] = 3;

        me.skills.dis = 10;
        me.skills.dev = 20;
        me.skills.sav = 15;
        me.skills.stl =  5;
        me.skills.srh = 20;
        me.skills.fos = 20;
        me.skills.thn = 40;
        me.skills.thb = 24;

        me.life = 150;
        me.exp = 50;

        me.birth = _munchkin_birth;
        me.calc_bonuses = _munchkin_calc_bonuses;
        me.get_flags = _munchkin_get_flags;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Mundane
 ****************************************************************/

static void _mundane_calc_bonuses(void)
{
    p_ptr->anti_magic = TRUE;
}
static void _mundane_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_NO_MAGIC);
}
static personality_ptr _get_mundane_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Mundane";
        me.desc = "Mundane adventurers have few talents to speak of, and "
                    "are incapable of using spells and not very good with "
                    "magical devices; but their anti-magic allows them to "
                    "resist the effects of many dangerous curses.";

        me.stats[A_STR] = -1;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] = -1;
 
        me.skills.dev = -3;
        me.skills.sav = 10;

        me.life = 97;
        me.exp = 100;

        me.calc_bonuses = _mundane_calc_bonuses;
        me.get_flags = _mundane_get_flags;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Nimble
 ****************************************************************/
static personality_ptr _get_nimble_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Nimble";
        me.desc = "Nimbleness is good for your skills and allows you to move less noisily; but your "
                    "raw strength is somewhat lacking, and you cannot survive quite as many hits.";

        me.stats[A_STR] = -1;
        me.stats[A_INT] =  1;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] =  2;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] =  0;

        me.skills.dis =  7;
        me.skills.dev =  2;
        me.skills.sav = -1;
        me.skills.stl =  1;
        me.skills.srh =  5;
        me.skills.fos =  5;
        me.skills.thn =  0;
        me.skills.thb =  7;

        me.life = 99;
        me.exp = 100;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Noble
 ****************************************************************/
static void _noble_birth(void)
{
    p_ptr->au *= 2;
    p_ptr->au += 2000;
}
static personality_ptr _get_noble_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Noble";
        me.desc = "As a Noble person, you have the benefits of an upper-class upbringing "
                    "(at least, you consider them benefits). You start with extra "
                    "money, and seem to have a relatively easy time acquiring even "
                    "more money; but whether other aspects of the adventuring life "
                    "will come to you as naturally remains to be seen.";

        me.stats[A_STR] = 0;
        me.stats[A_INT] = 0;
        me.stats[A_WIS] = 0;
        me.stats[A_DEX] = -2;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] = 2;

        me.skills.dis = -2;
        me.skills.dev =  2;
        me.skills.sav =  2;
        me.skills.stl =  0;
        me.skills.srh =  0;
        me.skills.fos =  0;
        me.skills.thn =  3;
        me.skills.thb = -3;

        me.life = 99;
        me.exp = 110;

        me.birth = _noble_birth;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Ordinary
 ****************************************************************/
static personality_ptr _get_ordinary_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Ordinary";
        me.desc = "An ordinary person is average in every respect, gaining neither "
                    "skills, talents nor stat adjustments.";
        me.life = 100;
        me.exp = 100;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Patient
 ****************************************************************/
static void _patient_calc_bonuses(void)
{
    p_ptr->to_m_chance += 1;
}
static personality_ptr _get_patient_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Patient";
        me.desc = "A Patient adventurer does everything with great care, but not always "
                    "with great expertise. Patient characters have high constitution and "
                    "resilience but fairly mediocre skills, and get a slight penalty to spell fail rates.";

        me.stats[A_STR] = -1;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] =  1;
        me.stats[A_DEX] = -2;
        me.stats[A_CON] =  2;
        me.stats[A_CHR] =  0;

        me.skills.dis = -5;
        me.skills.dev = -2;
        me.skills.sav =  3;
        me.skills.stl =  1;
        me.skills.srh =  0;
        me.skills.fos = -3;
        me.skills.thn = -6;
        me.skills.thb = -3;

        me.life = 102;
        me.exp = 100;

        me.calc_bonuses = _patient_calc_bonuses;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Pious
 ****************************************************************/
static personality_ptr _get_pious_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Pious";
        me.desc = "Pious deepens your faith in your God. It makes your physical "
                    "ability average, and your stats suitable for a priest.";

        me.stats[A_STR] =  0;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] =  2;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] =  0;
        me.stats[A_CHR] =  1;

        me.skills.dis = -5;
        me.skills.dev =  1;
        me.skills.sav =  4;
        me.skills.stl = -1;
        me.skills.srh =  3;
        me.skills.fos = -2;
        me.skills.thn = -3;
        me.skills.thb = -3;

        me.life = 100;
        me.exp = 100;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Sexy
 ****************************************************************/
static void _sexy_birth(void)
{
    if ( p_ptr->prace != RACE_MON_SWORD
      && p_ptr->prace != RACE_MON_ARMOR
      && !demon_is_(DEMON_BALROG) )
    {
        object_type forge = {0};
        object_prep(&forge, lookup_kind(TV_HAFTED, SV_WHIP));
        if (p_ptr->pclass == CLASS_RUNE_KNIGHT)
            rune_add(&forge, RUNE_ABSORPTION, FALSE);
        py_birth_obj(&forge);
        p_ptr->proficiency[PROF_BLUNT] = WEAPON_EXP_BEGINNER;
        if (p_ptr->proficiency_cap[PROF_BLUNT] < WEAPON_EXP_EXPERT)
            p_ptr->proficiency_cap[PROF_BLUNT] = WEAPON_EXP_EXPERT;
    }
}
static void _sexy_calc_bonuses(void)
{
    p_ptr->cursed |= OFC_AGGRAVATE;
}
static void _sexy_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_AGGRAVATE);
}
static personality_ptr _get_sexy_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Sexy";
        me.desc = "Sexy adventurers have excellent stats and skills, but their haughty "
                    "attitude aggravates all monsters.";

        me.stats[A_STR] = 1;
        me.stats[A_INT] = 1;
        me.stats[A_WIS] = 1;
        me.stats[A_DEX] = 1;
        me.stats[A_CON] = 1;
        me.stats[A_CHR] = 3;

        me.skills.dis = 10;
        me.skills.dev =  5;
        me.skills.sav =  3;
        me.skills.stl =  0;
        me.skills.srh =  4;
        me.skills.fos =  2;
        me.skills.thn = 10;
        me.skills.thb =  7;

        me.life = 100;
        me.exp = 100;

        me.calc_bonuses = _sexy_calc_bonuses;
        me.get_flags = _sexy_get_flags;
        me.birth = _sexy_birth;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Shrewd
 ****************************************************************/
static void _shrewd_calc_bonuses(void)
{
    p_ptr->to_m_chance -= 3;
}
static personality_ptr _get_shrewd_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Shrewd";
        me.desc = "Being Shrewd does not give you an impressive physique, but good device handling "
                    "embiggens the smallest man; this is the stereotypical Mage personality, "
                    "yet can also help a warrior complement his weaknesses. "
                    "Shrewdness has a direct positive influence on spell fail rates.";

        me.stats[A_STR] = -2;
        me.stats[A_INT] =  2;
        me.stats[A_WIS] =  0;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] = -1;

        me.skills.dis =  3;
        me.skills.dev =  8;
        me.skills.sav =  2;
        me.skills.stl =  0;
        me.skills.srh = -2;
        me.skills.fos =  5;
        me.skills.thn = -8;
        me.skills.thb = -3;

        me.life = 97;
        me.exp = 100;

        me.calc_bonuses = _shrewd_calc_bonuses;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Sneaky
 ****************************************************************/
static void _sneaky_calc_weapon_bonuses(object_type *o_ptr, weapon_info_t *info_ptr)
{
    if ((o_ptr) && (o_ptr->weight))
    {
        int bonus = (75 - o_ptr->weight) / 4;
        info_ptr->to_h += bonus;
        info_ptr->dis_to_h += bonus;
    }
}

static personality_ptr _get_sneaky_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Sneaky";
        me.desc = "Sneaky adventurers are stealthy and at home with light stabbing "
                    "weapons; but face-to-face combat with heavier weapons plays "
                    "against their strengths.";

        me.stats[A_STR] = -3;
        me.stats[A_INT] =  1;
        me.stats[A_WIS] =  1;
        me.stats[A_DEX] =  2;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] = -1;

        me.skills.dis =  5;
        me.skills.dev =  0;
        me.skills.sav =  0;
        me.skills.stl =  3;
        me.skills.srh =  3;
        me.skills.fos =  0;
        me.skills.thn =  0;
        me.skills.thb =  0;

        me.life = 97;
        me.exp = 100;
        me.calc_weapon_bonuses = _sneaky_calc_weapon_bonuses;

        init = TRUE;
    }
    return &me;
}

/****************************************************************
 * Split
 ****************************************************************/
void split_copy_status(byte status[MAX_PERSONALITIES], bool uusi)
{
    int i;
    if ((uusi) || (!_split_status_initialized))
    {
        for (i = 0; i < MAX_PERSONALITIES; i++)
        {
            _split_status[i] = status[i];
        }
        _split_status_initialized = TRUE;
    }
    else
    {
        for (i = 0; i < MAX_PERSONALITIES; i++)
        {
            status[i] = MIN(1, _split_status[i]);
        }
    }    
}

void split_shuffle(bool birth)
{
     byte _new_status[MAX_PERSONALITIES] = {0}, laskuri = 0, i;
     int prob;
     byte muutos = 0;
     bool vaihto = (one_in_(7)) ? FALSE : TRUE;
     for (i = 0; i < MAX_PERSONALITIES; i++)
     {
         if (_split_status[i])
         {
             laskuri++;
             if ((one_in_(laskuri)) && ((birth) || (vaihto))) _split_dominant = i;
         }
     }
//     msg_format("Dominant personality: %s", get_personality_aux(_split_dominant)->name);
     _new_status[_split_dominant] = SPLIT_DOMINANT; 
     prob = MIN(65, 30 + (100 / laskuri));
     for (i = 0; i < MAX_PERSONALITIES; i++)
     {
         if (i == _split_dominant) continue;
         if (_split_status[i])
         {
             _new_status[i] = magik(prob) ? SPLIT_NEGATIVE : SPLIT_SUPPRESSED;
             if ((_split_dominant == PERS_UNLUCKY) && (i == PERS_LUCKY)) _new_status[i] = SPLIT_SUPPRESSED;
             if ((_split_dominant == PERS_LUCKY) && (i == PERS_UNLUCKY)) _new_status[i] = SPLIT_SUPPRESSED;
         }
     }
     if ((_new_status[PERS_UNLUCKY] == SPLIT_NEGATIVE) && (_new_status[PERS_LUCKY] == SPLIT_NEGATIVE))
     {
         if (one_in_(2)) _new_status[PERS_UNLUCKY] = SPLIT_SUPPRESSED;
         else _new_status[PERS_UNLUCKY] = SPLIT_SUPPRESSED;
     }
     if ((_new_status[PERS_LUCKY] == SPLIT_SUPPRESSED) && (_split_status[PERS_LUCKY] != SPLIT_SUPPRESSED) 
      && (!prace_is_(RACE_MON_LEPRECHAUN)))
     {
         mut_unlock(MUT_GOOD_LUCK);
         mut_lose(MUT_GOOD_LUCK);
     }
     if ((_new_status[PERS_UNLUCKY] == SPLIT_SUPPRESSED) && (_split_status[PERS_UNLUCKY] != SPLIT_SUPPRESSED))
     {
         mut_unlock(MUT_BAD_LUCK);
         mut_lose(MUT_BAD_LUCK);
     }
     if ((_split_status[PERS_FRAGILE] == SPLIT_DOMINANT) && (_split_dominant != PERS_FRAGILE))
     {
         mut_unlock(MUT_EASY_TIRING);
         mut_lose(MUT_EASY_TIRING);
         mut_unlock(MUT_EASY_TIRING2);
         mut_lose(MUT_EASY_TIRING2);
     }
     if ((_split_status[PERS_LUCKY] == SPLIT_SUPPRESSED) && (_new_status[PERS_LUCKY] != SPLIT_SUPPRESSED)) 
     {
         mut_gain(MUT_GOOD_LUCK);
         mut_lock(MUT_GOOD_LUCK);
     }
     if ((_split_status[PERS_UNLUCKY] == SPLIT_SUPPRESSED) && (_new_status[PERS_UNLUCKY] != SPLIT_SUPPRESSED))
     {
         mut_gain(MUT_BAD_LUCK);
         mut_lock(MUT_BAD_LUCK);
     }
     if ((_split_status[_split_dominant] != SPLIT_DOMINANT) && (_split_dominant == PERS_FRAGILE))
     {
         mut_gain(MUT_EASY_TIRING);
         mut_lock(MUT_EASY_TIRING);
         mut_gain(MUT_EASY_TIRING2);
         mut_lock(MUT_EASY_TIRING2);
     }
     if (_split_status[_split_dominant] != SPLIT_DOMINANT) muutos |= 0x02;
     for (i = 0; i < MAX_PERSONALITIES; i++)
     {
         if (_split_status[i] != _new_status[i]) muutos |= 0x01;
         _split_status[i] = _new_status[i];
     }  
     if ((birth) || (!muutos)) return;
     _split_recalc_bonuses = TRUE;
     p_ptr->update |= (PU_BONUS | PU_HP);
     if (muutos & 0x01)
     {
         msg_print("Your thoughts are jolted...");
     }
     if (muutos & 0x02)
     {
         msg_format("You suddenly feel very %s!", ((_split_dominant == PERS_COMBAT) ? "Combative" : get_personality_aux(_split_dominant)->name));
     }
}

static void _split_birth(void)
{
    int i; 
    _split_dominant = 0;
    split_shuffle(TRUE);
    _split_recalc_bonuses = TRUE;
    for (i = 0; i < MAX_PERSONALITIES; i++)
    {
        if ((i == PERS_SPLIT) || (i == PERS_MUNCHKIN)) continue; /* paranoia */
        if ((_split_status[i]) && (get_personality_aux(i)->birth)) get_personality_aux(i)->birth();
    }
}
static void _split_calc_bonuses(void)
{
    if (_split_dominant >= MAX_PERSONALITIES) return;
    if (_split_dominant == PERS_SPLIT) return; /* paranoia */
    else
    {
        personality_t *pers_ptr = get_personality_aux(_split_dominant);
        if (pers_ptr->calc_bonuses) (pers_ptr->calc_bonuses)();
    }
}
static void _split_calc_weapon_bonuses(object_type *o_ptr, weapon_info_t *info_ptr)
{
    if (_split_dominant >= MAX_PERSONALITIES) return;
    if (_split_dominant == PERS_SPLIT) return; /* paranoia */
    else
    {
        personality_t *pers_ptr = get_personality_aux(_split_dominant);
        if (pers_ptr->calc_weapon_bonuses) (pers_ptr->calc_weapon_bonuses)(o_ptr, info_ptr);
    }
}
static void _split_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    if (_split_dominant >= MAX_PERSONALITIES) return;
    if (_split_dominant == PERS_SPLIT) return; /* paranoia */
    else
    {
        personality_t *pers_ptr = get_personality_aux(_split_dominant);
        if (pers_ptr->get_flags) (pers_ptr->get_flags)(flgs);
    }
}

void split_save(savefile_ptr file)
{
    int i;
    savefile_write_byte(file, MAX_PERSONALITIES);
    for (i = 0; i < MAX_PERSONALITIES; i++)
    {
        savefile_write_byte(file, _split_status[i]);
    }
}

void split_load(savefile_ptr file)
{
    byte i, kohde = savefile_read_byte(file);
    if (!_split_status_initialized) _split_dominant = 0;
    _split_recalc_bonuses = TRUE;
    _split_status_initialized = TRUE;
    for (i = 0; i < MAX(kohde, MAX_PERSONALITIES); i++)
    {
        byte j = 0;
        if (i < kohde) j = savefile_read_byte(file);
        if (i < MAX_PERSONALITIES) 
        {
            _split_status[i] = j;
            if (j == SPLIT_DOMINANT) _split_dominant = i;
        }
    }
}

void split_dump(doc_ptr doc, int mode)
{
    switch (mode)
    {
        case 0:
        {
            doc_printf(doc, " Personality: <color:B>Split (%s)</color>\n", get_personality_aux(_split_dominant)->name);
            break;
        }
        case 1:
        {
            byte i;
            char old_color = 'y';
            int edellinen = -1;
            doc_printf(doc, " Personalities: <indent><color:G>%s; </color>", get_personality_aux(_split_dominant)->name);
            for (i = 0; i < MAX_PERSONALITIES; i++)
            {
                if (i == PERS_SPLIT) continue; /* paranoia */
                if (_split_status[i] == SPLIT_NEGATIVE) 
                {
                    if (edellinen >= 0) doc_printf(doc, "<color:%c>%s, </color>", old_color, get_personality_aux(edellinen)->name);
                    edellinen = i;
                }
            }
            for (i = 0; i < MAX_PERSONALITIES; i++)
            {
                if (i == PERS_SPLIT) continue; /* paranoia */
                if (_split_status[i] == SPLIT_SUPPRESSED)
                {
                    if (edellinen >= 0) doc_printf(doc, "<color:%c>%s%c </color>", old_color, get_personality_aux(edellinen)->name, ((old_color == 'y') ? ';' : ','));
                    edellinen = i;
                    old_color = 'r';
                }
            }
            doc_printf(doc, "<color:%c>%s</color></indent>\n\n", old_color, get_personality_aux(edellinen)->name);
            break;
        }
        default: break;
    }
}

static personality_ptr _get_split_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Split";
        me.desc = "You have multiple personalities, and inherit the upsides and "
                    "downsides of each. At all times, one of your personalities is "
                    "dominant, and you receive the full effects of that personality; "
                    "the remaining personalities are either partially suppressed "
                    "or fully suppressed. A partially suppressed personality gives "
                    "reduced stat and skill bonuses, and some other effects "
                    "(like the walking speed bonus of Craven or the aggravation from "
                    "Sexy) will be absent entirely, but other parts of the personality "
                    "apply normally. A fully dormant personality has no effect on you "
                    "at the moment, but may resurface later! Level gains, and the "
                    "trauma of taking heavy damage, can cause previously suppressed "
                    "personalities to become dominant and vice versa.";
    } 

    if (_split_recalc_bonuses) 
    {   
        int i;
        int _sbon[MAX_STATS] = {0};
        me.stats[A_STR] = 0;
        me.stats[A_INT] = 0;
        me.stats[A_WIS] = 0;
        me.stats[A_DEX] = 0;
        me.stats[A_CON] = 0;
        me.stats[A_CHR] = 0;

        me.skills.dis = 0;
        me.skills.dev = 0;
        me.skills.sav = 0;
        me.skills.stl = 0;
        me.skills.srh = 0;
        me.skills.fos = 0;
        me.skills.thn = 0;
        me.skills.thb = 0;

        me.life = 100;
        me.exp = 100;
        me.birth = _split_birth;
        me.calc_bonuses = _split_calc_bonuses;
        me.get_flags = _split_get_flags;
        me.calc_weapon_bonuses = _split_calc_weapon_bonuses;

        init = TRUE;
        if ((spoiler_hack) || (birth_hack)) return &me;
        for (i = PERS_ORDINARY + 1; i < MAX_PERSONALITIES; i++)
        { 
            personality_t *temp;  
            if (_split_status[i] < SPLIT_NEGATIVE) continue;
            if (i == PERS_SPLIT) continue; /* Paranoia */
            if (i == PERS_MUNCHKIN) continue; /* Paranoia */
            temp = get_personality_aux(i);

            if (_split_status[i] == SPLIT_NEGATIVE)
            {
                int j;
                for (j = A_STR; j < MAX_STATS; j++)
                {
                    if (temp->stats[j] > 0) _sbon[j] += temp->stats[j];
                    else me.stats[j] += temp->stats[j];
                }
                me.skills.dis += (temp->skills.dis > 0 ? temp->skills.dis / 2 : temp->skills.dis);
                me.skills.dev += (temp->skills.dev > 0 ? temp->skills.dev / 2 : temp->skills.dev);
                me.skills.sav += (temp->skills.sav > 0 ? temp->skills.sav / 2 : temp->skills.sav);
                me.skills.stl += (temp->skills.stl > 0 ? temp->skills.stl / 2 : temp->skills.stl);
                me.skills.srh += (temp->skills.srh > 0 ? temp->skills.srh / 2 : temp->skills.srh);
                me.skills.fos += (temp->skills.fos > 0 ? temp->skills.fos / 2 : temp->skills.fos);
                me.skills.thn += (temp->skills.thn > 0 ? temp->skills.thn / 2 : temp->skills.thn);
                me.skills.thb += (temp->skills.thb > 0 ? temp->skills.thb / 2 : temp->skills.thb);
                me.life += (temp->life - 100);
            }  
            else if (_split_status[i] == SPLIT_DOMINANT)
            {
                int j;
                for (j = A_STR; j < MAX_STATS; j++)
                {
                    me.stats[j] += temp->stats[j];
                }
                me.skills.dis += temp->skills.dis;
                me.skills.dev += temp->skills.dev;
                me.skills.sav += temp->skills.sav;
                me.skills.stl += temp->skills.stl;
                me.skills.srh += temp->skills.srh;
                me.skills.fos += temp->skills.fos;
                me.skills.thn += temp->skills.thn;
                me.skills.thb += temp->skills.thb;
                me.life += (temp->life - 100);
                me.exp = temp->exp; 
            }   
        }
        for (i = 0; i < MAX_STATS; i++)
        {
            me.stats[i] += _sbon[i] / 2;
        }
        _split_recalc_bonuses = FALSE;
    }
    return &me;
}


/****************************************************************
 * Unlucky
 ****************************************************************/
static void _unlucky_birth(void)
{
    if ((p_ptr->personality == PERS_SPLIT) && (_split_status[PERS_UNLUCKY] < SPLIT_NEGATIVE)) return;
    mut_gain(MUT_BAD_LUCK);
    mut_lock(MUT_BAD_LUCK);
}
static personality_ptr _get_unlucky_personality(void)
{
    static personality_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Unlucky";
        me.desc = "An Unlucky adventurer looks impressive on paper and "
                    "should, by rights, have an easy time - but somehow "
                    "their fights don't go quite so smoothly, and the items "
                    "they most need never seem to drop.";

        me.stats[A_STR] = 2;
        me.stats[A_INT] = 2;
        me.stats[A_WIS] = 2;
        me.stats[A_DEX] = 2;
        me.stats[A_CON] = 2;
        me.stats[A_CHR] = 2;

        me.skills.dis = -5;
        me.skills.dev =  3;
        me.skills.sav = -8;
        me.skills.stl =  0;
        me.skills.srh =  0;
        me.skills.fos =  4;
        me.skills.thn = -2;
        me.skills.thb = -2;

        me.life = 104;
        me.exp = 100;

        me.birth = _unlucky_birth;

        init = TRUE;
    }
    return &me;
}


/****************************************************************
 * Public Interface
 ****************************************************************/
personality_ptr get_personality_aux(int index)
{
    personality_ptr result = NULL;
    switch (index)
    {
    case PERS_COMBAT:
        result = _get_combat_personality();
        break;
    case PERS_CRAVEN:
        result = _get_craven_personality();
        break;
    case PERS_FEARLESS:
        result = _get_fearless_personality();
        break;
    case PERS_FRAGILE:
        result = _get_fragile_personality();
        break;
    case PERS_HASTY:
        result = _get_hasty_personality();
        break;
    case PERS_LAZY:
        result = _get_lazy_personality();
        break;
    case PERS_LUCKY:
        result = _get_lucky_personality();
        break;
    case PERS_MIGHTY:
        result = _get_mighty_personality();
        break;
    case PERS_MUNCHKIN:
        result = _get_munchkin_personality();
        break;
    case PERS_MUNDANE:
        result = _get_mundane_personality();
        break;
    case PERS_NIMBLE:
        result = _get_nimble_personality();
        break;
    case PERS_NOBLE:
        result = _get_noble_personality();
        break;
    case PERS_ORDINARY:
        result = _get_ordinary_personality();
        break;
    case PERS_PATIENT:
        result = _get_patient_personality();
        break;
    case PERS_PIOUS:
        result = _get_pious_personality();
        break;
    case PERS_SEXY:
        result = _get_sexy_personality();
        break;
    case PERS_SHREWD:
        result = _get_shrewd_personality();
        break;
    case PERS_SNEAKY:
        result = _get_sneaky_personality();
        break;
    case PERS_SPLIT:
        result = _get_split_personality();
        break;
    case PERS_UNLUCKY:
        result = _get_unlucky_personality();
        break;
    }
    assert(result);
    result->id = index;
    return result;
}

personality_ptr get_personality(void)
{
    return get_personality_aux(p_ptr->personality);
}

