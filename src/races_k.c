#include "angband.h"

/****************************************************************
 * Klackon
 ****************************************************************/
static power_info _klackon_powers[] =
{
    { A_DEX, {9, 9, 50, spit_acid_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _klackon_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _klackon_powers);
}
static void _klackon_calc_bonuses(void)
{
    res_add(RES_CONF);
    res_add(RES_ACID);
    p_ptr->pspeed += (p_ptr->lev) / 10;
}
static void _klackon_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_CONF);
    add_flag(flgs, OF_RES_ACID);
    if (p_ptr->lev > 9)
        add_flag(flgs, OF_SPEED);
}
race_t *klackon_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Klackon";
        me.desc = "Klackons are bizarre semi-intelligent ant-like insectoid creatures. "
                    "They make great fighters, but their mental abilities are severely limited. "
                    "Obedient and well-ordered, they are resistant to confusion. They are very "
                    "nimble, and become faster as they advance levels. They are also very acidic, "
                    "inherently resist acid, and become capable of spitting it at higher levels.";

        me.stats[A_STR] =  2;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] =  2;
        me.stats[A_CHR] = -1;

        me.skills.dis = 10;
        me.skills.dev = -2;
        me.skills.sav = 3;
        me.skills.stl = 0;
        me.skills.srh = -1;
        me.skills.fos = 10;
        me.skills.thn = 5;
        me.skills.thb = 3;

        me.life = 105;
        me.base_hp = 23;
        me.exp = 170;
        me.infra = 2;
        me.shop_adjust = 115;

        me.calc_bonuses = _klackon_calc_bonuses;
        me.get_powers = _klackon_get_powers;
        me.get_flags = _klackon_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Kobold
 ****************************************************************/
static power_info _kobold_powers[] =
{
    { A_DEX, {12, 8, 50, poison_dart_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _kobold_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _kobold_powers);
}
static void _kobold_calc_bonuses(void)
{
    res_add(RES_POIS);
}
static void _kobold_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_POIS);
}
race_t *kobold_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Kobold";
        me.desc = "Kobolds are a weak goblin race. They love poisoned weapons, and can learn to throw "
                    "poisoned darts (of which they carry an unlimited supply). They are also inherently "
                    "resistant to poison.";

        me.stats[A_STR] =  1;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] =  0;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] =  0;
        me.stats[A_CHR] = -2;

        me.skills.dis = -2;
        me.skills.dev = -2;
        me.skills.sav = -1;
        me.skills.stl = -1;
        me.skills.srh =  1;
        me.skills.fos =  8;
        me.skills.thn = 10;
        me.skills.thb =  3;

        me.life = 98;
        me.base_hp = 19;
        me.exp = 90;
        me.infra = 3;
        me.shop_adjust = 120;

        me.calc_bonuses = _kobold_calc_bonuses;
        me.get_powers = _kobold_get_powers;
        me.get_flags = _kobold_get_flags;

        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Kutar
 ****************************************************************/
static power_info _kutar_powers[] =
{
    { A_CHR, {20, 15, 70, kutar_expand_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _kutar_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _kutar_powers);
}
static void _kutar_calc_bonuses(void)
{
    res_add(RES_CONF);
}
static void _kutar_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_CONF);
}
race_t *kutar_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Kutar";
        me.desc = "A Kutar is not a dog, cat or bear. Despite their characteristic "
                    "vacant expression, Kutars are beautiful and so have a high charisma. "
                    "Their absent-mindedness hurts their searching and perception skills, "
                    "but renders them resistant to being confused. Due to their unearthly "
                    "calmness and serenity, Kutars are extremely stealthy. They learn the "
                    "special ability to expand their body horizontally; this increases "
                    "their armor class, but renders them vulnerable to curses.";

        me.stats[A_STR] =  0;
        me.stats[A_INT] = -1;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] =  2;
        me.stats[A_CHR] =  3;

        me.skills.dis = -2;
        me.skills.dev = 3;
        me.skills.sav = 5;
        me.skills.stl = 5;
        me.skills.srh = -2;
        me.skills.fos = 6;
        me.skills.thn = 0;
        me.skills.thb = -3;

        me.life = 102;
        me.base_hp = 21;
        me.exp = 175;
        me.infra = 0;
        me.shop_adjust = 95;

        me.calc_bonuses = _kutar_calc_bonuses;
        me.get_powers = _kutar_get_powers;
        me.get_flags = _kutar_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Mindflayer
 ****************************************************************/
static power_info _mindflayer_powers[] =
{
    { A_INT, {5, 3, 50, mind_blast_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _mindflayer_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _mindflayer_powers);
}
static void _mindflayer_calc_bonuses(void)
{
    p_ptr->sustain_int = TRUE;
    p_ptr->sustain_wis = TRUE;
    if (p_ptr->lev >= 15) p_ptr->see_inv++;
    if (p_ptr->lev >= 30) p_ptr->telepathy = TRUE;
}
static void _mindflayer_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_SUST_INT);
    add_flag(flgs, OF_SUST_WIS);
    if (p_ptr->lev >= 15)
        add_flag(flgs, OF_SEE_INVIS);
    if (p_ptr->lev >= 30)
        add_flag(flgs, OF_TELEPATHY);
}
race_t *mindflayer_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Mindflayer";
        me.desc = "Mindflayers are a secretive and mysterious ancient race. Their civilization may well "
                    "be older than any other; their intelligence and wisdom are naturally sustained, "
                    "and so great as to make Mindflayers highly effective spellcasters, but their "
                    "physical attributes are a good deal less admirable. As they gain levels, they "
                    "learn to see invisible creatures, and eventually develop a telepathic sight.";

        me.stats[A_STR] = -3;
        me.stats[A_INT] =  4;
        me.stats[A_WIS] =  4;
        me.stats[A_DEX] =  0;
        me.stats[A_CON] = -2;
        me.stats[A_CHR] = -3;

        me.skills.dis = 10;
        me.skills.dev = 11;
        me.skills.sav = 9;
        me.skills.stl = 2;
        me.skills.srh = 5;
        me.skills.fos = 12;
        me.skills.thn = -10;
        me.skills.thb = -5;

        me.life = 97;
        me.base_hp = 18;
        me.exp = 150;
        me.infra = 4;
        me.shop_adjust = 115;

        me.calc_bonuses = _mindflayer_calc_bonuses;
        me.get_powers = _mindflayer_get_powers;
        me.get_flags = _mindflayer_get_flags;
        init = TRUE;
    }

    return &me;
}


/****************************************************************
 * Nibelung
 ****************************************************************/
static power_info _nibelung_powers[] =
{
    { A_WIS, {10, 5, 50, detect_doors_stairs_traps_spell}},
    { A_CHR, {10, 5, 50, detect_treasure_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _nibelung_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _nibelung_powers);
}
static void _nibelung_calc_bonuses(void)
{
    res_add(RES_DISEN);
    res_add(RES_DARK);
}
static void _nibelung_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_DISEN);
    add_flag(flgs, OF_RES_DARK);
}
race_t *nibelung_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Nibelung";
        me.desc = "The hated and persecuted race of nocturnal dwarves, these cave-dwellers are "
                    "not much bothered by darkness. Their natural inclination to magical items "
                    "has made them resistant to effects which could disenchant magical energy.";

        me.stats[A_STR] =  0;
        me.stats[A_INT] =  1;
        me.stats[A_WIS] =  0;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] =  1;
        me.stats[A_CHR] = -2;

        me.skills.dis =  3;
        me.skills.dev =  3;
        me.skills.sav =  6;
        me.skills.stl =  1;
        me.skills.srh =  5;
        me.skills.fos = 10;
        me.skills.thn = 10;
        me.skills.thb =  0;

        me.life = 101;
        me.base_hp = 21;
        me.exp = 150;
        me.infra = 5;
        me.shop_adjust = 115;

        me.calc_bonuses = _nibelung_calc_bonuses;
        me.get_powers = _nibelung_get_powers;
        me.get_flags = _nibelung_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Ogre
 ****************************************************************/
static power_info _ogre_powers[] =
{
    { A_INT, {25, 35, 70, explosive_rune_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _ogre_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _ogre_powers);
}
static void _ogre_calc_bonuses(void)
{
    p_ptr->sustain_int = TRUE;
}
static void _ogre_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_SUST_INT);
}
race_t *ogre_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Ogre";
        me.desc = "Ogres are big and ugly, but possessed of a low cunning and great strength. "
                    "They have all the necessary attributes to be warriors, yet are also "
                    "surprisingly adept as mages; and at high level, all Ogres learn the Ogre "
                    "Mage skill of setting trapped runes. Being simple-minded, Ogres have their "
                    "intelligence sustained.";

        me.stats[A_STR] =  3;
        me.stats[A_INT] =  0;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] =  3;
        me.stats[A_CHR] = -2;

        me.skills.dis = -3;
        me.skills.dev = -3;
        me.skills.sav = -3;
        me.skills.stl = -2;
        me.skills.srh = -1;
        me.skills.fos =  5;
        me.skills.thn = 20;
        me.skills.thb =  -5;

        me.life = 106;
        me.base_hp = 23;
        me.exp = 140;
        me.infra = 0;
        me.shop_adjust = 125;

        me.calc_bonuses = _ogre_calc_bonuses;
        me.get_powers = _ogre_get_powers;
        me.get_flags = _ogre_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Shadow-Fairy
 ****************************************************************/
static void _shadow_fairy_calc_bonuses(void)
{
    p_ptr->levitation = TRUE;
    p_ptr->fairy_stealth = TRUE;
    res_add_vuln(RES_LITE);
}
static void _shadow_fairy_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_LEVITATION);
    add_flag(flgs, OF_VULN_LITE);
}
race_t *shadow_fairy_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Shadow-Fairy";
        me.desc = "Shadow-fairies are one of the several fairy races. They have wings, and can fly over "
                    "traps that may open up beneath them. Being creatures of darkness, shadow-fairies are "
                    "vulnerable to bright light. They are physically weak, but naturally adept at using "
                    "magic. Shadow-fairies are amazingly stealthy, and have a wonderful advantage in "
                    "that they almost never aggravate monsters; aggravating equipment only gives them a slight "
                    "penalty to stealth, but if they have an aggravating personality the advantage is lost.";

        me.stats[A_STR] = -2;
        me.stats[A_INT] =  2;
        me.stats[A_WIS] =  2;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] =  0;

        me.skills.dis =  7;
        me.skills.dev =  6;
        me.skills.sav =  0;
        me.skills.stl =  6;
        me.skills.srh = 12;
        me.skills.fos = 15;
        me.skills.thn =-10;
        me.skills.thb = -3;

        me.life = 91;
        me.base_hp = 13;
        me.exp = 140;
        me.infra = 4;
        me.shop_adjust = 110;

        me.calc_bonuses = _shadow_fairy_calc_bonuses;
        me.get_flags = _shadow_fairy_get_flags;
        init = TRUE;
    }

    return &me;
}


/****************************************************************
 * Skeleton
 ****************************************************************/
static power_info _skeleton_powers[] =
{
    { A_WIS, {30, 30, 70, restore_life_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _skeleton_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _skeleton_powers);
}
static void _skeleton_calc_bonuses(void)
{
    res_add(RES_SHARDS);
    p_ptr->hold_life++;
    p_ptr->see_inv++;
    res_add(RES_POIS);
    if (p_ptr->lev >= 10) res_add(RES_COLD);
}
static void _skeleton_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_SEE_INVIS);
    add_flag(flgs, OF_RES_SHARDS);
    add_flag(flgs, OF_HOLD_LIFE);
    add_flag(flgs, OF_RES_POIS);
    if (p_ptr->lev >= 10)
        add_flag(flgs, OF_RES_COLD);
}
static void _skeleton_birth(void)
{
    py_birth_obj_aux(TV_STAFF, EFFECT_NOTHING, 1);
    py_birth_light();
}
race_t *skeleton_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Skeleton";
        me.desc = "As undead beings, skeletons need worry very little about poison or attacks "
                    "that can drain life. Skeletons do not really use eyes for perceiving things, "
                    "and are thus not fooled by invisibility. Their bones are resistant to sharp shrapnel, "
                    "and they will quickly become resistant to cold. Although the magical effects of "
                    "potions and mushrooms will affect the skeleton even without entering its (non-existent) "
                    "belly, the food itself will fall through the skeleton's jaws, giving no nutritional benefit; "
                    "instead, skeletons feed themselves by absorbing the energy of magical devices.";

        me.stats[A_STR] =  0;
        me.stats[A_INT] =  1;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] =  0;
        me.stats[A_CON] =  1;
        me.stats[A_CHR] = -2;

        me.skills.dis = -5;
        me.skills.dev = 0;
        me.skills.sav = 3;
        me.skills.stl = -1;
        me.skills.srh = -1;
        me.skills.fos = 8;
        me.skills.thn = 10;
        me.skills.thb = 0;

        me.life = 100;
        me.base_hp = 21;
        me.exp = 115;
        me.infra = 2;
        me.flags = RACE_IS_NONLIVING | RACE_IS_UNDEAD;
        me.shop_adjust = 125;

        me.birth = _skeleton_birth;
        me.calc_bonuses = _skeleton_calc_bonuses;
        me.get_powers = _skeleton_get_powers;
        me.get_flags = _skeleton_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Snotling (A Joke Race)
 ****************************************************************/
static void _devour_flesh_spell(int cmd, variant *res)
{
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, "Devour Flesh");
        break;
    case SPELL_DESC:
        var_set_string(res, "Devour flesh (yours) in order to fill your belly.");
        break;
    case SPELL_FLAGS:
        var_set_int(res, PWR_CONFUSED);
        break;
    case SPELL_CAST:
        var_set_bool(res, FALSE);
        if (!get_check("It might hurt a bit. Are you sure?")) return;
        msg_print("You devour your own flesh!");
        set_food(PY_FOOD_MAX - 1);
        set_cut(p_ptr->cut + CUT_SEVERE, FALSE);
        take_hit(DAMAGE_USELIFE, p_ptr->mhp / 3, "devouring your own flesh");
        var_set_bool(res, TRUE);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static power_info _snotling_powers[] =
{
    { A_CHR, {1, 0, 0, _devour_flesh_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _snotling_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _snotling_powers);
}
static void _snotling_calc_bonuses(void)
{
}
static void _snotling_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
}
static void _snotling_birth(void)
{
    py_birth_obj_aux(TV_FOOD, SV_FOOD_FAST_RECOVERY, randint1(3));
    py_birth_food();
    py_birth_light();
}
race_t *snotling_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Snotling";
        me.desc = "Snotlings are greenskins, and are cousins of Goblins and Orcs, smaller than the "
                    "former and dumber than the latter, often used by them as cannon fodder, food "
                    "or even cannon missiles. They are on the lowest rung of greenskin society and "
                    "are bullied by all. Snotlings often wield mushrooms or sticks into battle.";

        me.stats[A_STR] = -2;
        me.stats[A_INT] = -2;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] = -2;
        me.stats[A_CON] = -2;
        me.stats[A_CHR] = -5;

        me.skills.dis = -3;
        me.skills.dev = -2;
        me.skills.sav = -2;
        me.skills.stl = 2;
        me.skills.srh = 0;
        me.skills.fos = 7;
        me.skills.thn = -10;
        me.skills.thb = -5;

        me.life = 85;
        me.base_hp = 10;
        me.exp = 45;
        me.infra = 2;
        me.shop_adjust = 125;

        me.birth = _snotling_birth;
        me.calc_bonuses = _snotling_calc_bonuses;
        me.get_powers = _snotling_get_powers;
        me.get_flags = _snotling_get_flags;

        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Spectre
 ****************************************************************/
static power_info _spectre_powers[] =
{
    { A_INT, {4, 6, 50, scare_monster_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _spectre_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _spectre_powers);
}
static void _spectre_calc_bonuses(void)
{
    p_ptr->levitation = TRUE;
    res_add(RES_NETHER);
    p_ptr->hold_life++;
    p_ptr->see_inv++;
    res_add(RES_POIS);
    p_ptr->slow_digest = TRUE;
    res_add(RES_COLD);
    p_ptr->pass_wall = TRUE;
}
static void _spectre_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_LEVITATION);
    add_flag(flgs, OF_RES_COLD);
    add_flag(flgs, OF_SEE_INVIS);
    add_flag(flgs, OF_HOLD_LIFE);
    add_flag(flgs, OF_RES_NETHER);
    add_flag(flgs, OF_RES_POIS);
    add_flag(flgs, OF_SLOW_DIGEST);
}
static void _spectre_birth(void)
{
    py_birth_obj_aux(TV_STAFF, EFFECT_NOTHING, 1);
    py_birth_light();
}
race_t *spectre_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Spectre";
        me.desc = "A powerful undead creature, the Spectre is a ghastly apparition, surrounded by "
                    "an unearthly green glow. Spectres exist only partially on our plane of existence: "
                    "half-corporeal, they can pass through walls, although the density of the wall "
                    "will hurt them in the process of doing this. As undead, they have a firm hold "
                    "on their life force, can see invisible creatures, and resist poison, cold and "
                    "nether. Spectres make superb spellcasters, but their physical form is very weak. "
                    "They gain very little nutrition from the food of mortals, but can absorb mana "
                    "from magical devices as their energy source.";

        me.stats[A_STR] = -5;
        me.stats[A_INT] =  4;
        me.stats[A_WIS] =  2;
        me.stats[A_DEX] =  2;
        me.stats[A_CON] = -2;
        me.stats[A_CHR] = -3;

        me.skills.dis = 10;
        me.skills.dev = 10;
        me.skills.sav = 12;
        me.skills.stl =  5;
        me.skills.srh =  5;
        me.skills.fos = 14;
        me.skills.thn =-15;
        me.skills.thb = -5;

        me.life = 90;
        me.base_hp = 13;
        me.exp = 250;
        me.infra = 5;
        me.flags = RACE_IS_NONLIVING | RACE_IS_UNDEAD;
        me.shop_adjust = 135;

        me.birth = _spectre_birth;
        me.calc_bonuses = _spectre_calc_bonuses;
        me.get_powers = _spectre_get_powers;
        me.get_flags = _spectre_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Sprite
 ****************************************************************/
static power_info _sprite_powers[] =
{
    { A_INT, {12, 12, 50, sleeping_dust_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _sprite_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _sprite_powers);
}
static void _sprite_calc_bonuses(void)
{
    p_ptr->levitation = TRUE;
    res_add(RES_LITE);
    p_ptr->pspeed += (p_ptr->lev) / 10;
}
static void _sprite_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_LITE);
    add_flag(flgs, OF_LEVITATION);
    if (p_ptr->lev >= 10)
        add_flag(flgs, OF_SPEED);
}
race_t *sprite_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Sprite";
        me.desc = "One of the several fairy races, Sprites are very small. They have tiny wings and can "
                    "fly over traps that may open up beneath them. They enjoy sunlight intensely, and "
                    "need worry little about light based attacks. Although physically among the weakest "
                    "races, Sprites are very talented in magic, and can become highly skilled wizards. "
                    "Sprites have the special power of spraying Sleeping Dust, and at higher levels they "
                    "learn to fly faster.";

        me.stats[A_STR] = -4;
        me.stats[A_INT] =  3;
        me.stats[A_WIS] =  3;
        me.stats[A_DEX] =  3;
        me.stats[A_CON] = -2;
        me.stats[A_CHR] =  2;

        me.skills.dis = 10;
        me.skills.dev =  6;
        me.skills.sav =  6;
        me.skills.stl =  4;
        me.skills.srh = 10;
        me.skills.fos = 10;
        me.skills.thn =-12;
        me.skills.thb =  0;

        me.life = 92;
        me.base_hp = 14;
        me.exp = 135;
        me.infra = 4;
        me.shop_adjust = 90;

        me.calc_bonuses = _sprite_calc_bonuses;
        me.get_powers = _sprite_get_powers;
        me.get_flags = _sprite_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Tomte
 ****************************************************************/
static power_info _tomte_powers[] =
{
    { A_INT, {1, 0, 20, detect_monsters_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _tomte_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _tomte_powers);
}

int tomte_heavy_armor(void)
{
    slot_t slot = equip_find_obj(TV_HELM, SV_ANY);
    if (!slot) slot = equip_find_obj(TV_CROWN, SV_ANY);
    if (slot)
    {
        object_type *lakki = equip_obj(slot);
        if ((lakki) && (lakki->weight > 10))
        {
            return (lakki->weight - 10);
        }
    }
    return 0;
}
static void _tomte_calc_bonuses(void)
{
    int ylipaino = tomte_heavy_armor();
    res_add(RES_COLD);
    p_ptr->pspeed += (p_ptr->lev) / 15;
    if (ylipaino)
    {
        p_ptr->stat_add[A_INT] -= ((ylipaino / 10) + 1);
        p_ptr->skills.dev -= ((ylipaino + 6) / 2);
    }
    else
    {
        p_ptr->auto_pseudo_id = TRUE;
        if (p_ptr->lev >= 40) p_ptr->auto_id = TRUE;
    }
}
static void _tomte_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_COLD);
    if (p_ptr->lev >= 15)
        add_flag(flgs, OF_SPEED);
}
static void _tomte_birth(void)
{
    py_birth_obj_aux(TV_HELM, SV_KNIT_CAP, 1);
    py_birth_food();
    py_birth_light();
}
race_t *tomte_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Tomte";
        me.desc = "Smaller and of lighter build than regular elves, Tomtes are nimble, intelligent, "
                  "extremely stealthy and at home with magical devices, but their size handicaps them "
                  "in physical combat. They greatly prefer light headwear, and cannot think clearly "
                  "with a heavy helmet squeezing their brains. Tomtes have the ability to sense "
                  "monsters and items at a glance (they gain auto-identify at level 40); and being "
                  "accustomed to long winters, they are not much bothered by cold.";

        me.stats[A_STR] = -4;
        me.stats[A_INT] =  2;
        me.stats[A_WIS] =  2;
        me.stats[A_DEX] =  3;
        me.stats[A_CON] = -3;
        me.stats[A_CHR] =  0;

        me.skills.dis = 10;
        me.skills.dev = 15;
        me.skills.sav =  0;
        me.skills.stl =  6;
        me.skills.srh = 10;
        me.skills.fos = 10;
        me.skills.thn = -1;
        me.skills.thb = -5;

        me.life = 94;
        me.base_hp = 13;
        me.exp = 150;
        me.infra = 4;
        me.shop_adjust = 100;

        me.calc_bonuses = _tomte_calc_bonuses;
        me.get_powers = _tomte_get_powers;
        me.get_flags = _tomte_get_flags;
        me.birth = _tomte_birth;
        me.boss_r_idx = MON_SANTACLAUS;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Tonberry
 ****************************************************************/
static void _tonberry_birth(void)
{
    p_ptr->proficiency[PROF_DAGGER] = WEAPON_EXP_BEGINNER;
    p_ptr->proficiency_cap[PROF_DAGGER] = WEAPON_EXP_MASTER;

    py_birth_food();
    py_birth_light();
}
static void _tonberry_calc_bonuses(void)
{
    p_ptr->sustain_str = TRUE;
    p_ptr->sustain_con = TRUE;
    res_add(RES_FEAR);

    p_ptr->pspeed -= 1;
    if (p_ptr->lev >= 30)
        p_ptr->pspeed -= 1;
    if (p_ptr->lev >= 40)
        p_ptr->pspeed -= 1;
    if (p_ptr->lev >= 45)
        p_ptr->pspeed -= 1;
    if (p_ptr->lev >= 50)
        p_ptr->pspeed -= 1;

    if (p_ptr->pclass != CLASS_DUELIST)
    {
        int hand;
        for (hand = 0; hand < MAX_HANDS; hand++)
        {
            if (p_ptr->weapon_info[hand].wield_how != WIELD_NONE)
            {
                p_ptr->weapon_info[hand].to_d += 2 * p_ptr->lev / p_ptr->weapon_ct;
                p_ptr->weapon_info[hand].dis_to_d += 2 * p_ptr->lev / p_ptr->weapon_ct;
                p_ptr->weapon_info[hand].xtra_blow -= 4 * p_ptr->lev;
            }
        }
    }
    /* Tonberries are also vulnerable to confusion ... cf res_pct_aux in resist.c */
}
static void _tonberry_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_SUST_STR);
    add_flag(flgs, OF_SUST_CON);
    add_flag(flgs, OF_RES_FEAR);
    add_flag(flgs, OF_DEC_SPEED);
    add_flag(flgs, OF_VULN_CONF);
}
race_t *tonberry_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Tonberry";
        me.desc = "Tonberries are lizard-like creatures who possess enormous strength "
                    "and have a preference for cleavers and large knives. They are, "
                    "however, sluggish in their movements and reactions; young and "
                    "inexperienced tonberries are often preyed on by the other races. "
                    "They possess human-like intelligence, but rarely become mages due "
                    "to their culture and physiology. Tonberries attack very powerfully "
                    "in melee, albeit with fewer attacks than normal. They are also "
                    "easily confused and move with reduced speed.";

        me.stats[A_STR] =  4;
        me.stats[A_INT] =  0;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] = -4;
        me.stats[A_CON] =  5;
        me.stats[A_CHR] = -1;

        me.skills.dis = -5;
        me.skills.dev = -3;
        me.skills.sav =  3;
        me.skills.stl =  1;
        me.skills.srh = -2;
        me.skills.fos =  5;
        me.skills.thn = 20;
        me.skills.thb = -7;

        me.life = 108;
        me.base_hp = 26;
        me.exp = 175;
        me.infra = 2;
        me.shop_adjust = 115;

		me.birth = _tonberry_birth;
		me.calc_bonuses = _tonberry_calc_bonuses;
        me.get_flags = _tonberry_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Vampire
 ****************************************************************/
static power_info _vampire_powers[] =
{
    { A_CON, {2, 1, 60, vampirism_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _vampire_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _vampire_powers);
}
static void _vampire_calc_bonuses(void)
{
    res_add(RES_DARK);
    res_add(RES_NETHER);
    res_add(RES_COLD);
    res_add(RES_POIS);
    res_add_vuln(RES_LITE);
    p_ptr->hold_life++;
    if (!player_is_ninja) p_ptr->lite = TRUE;
}
static void _vampire_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_VULN_LITE);

    add_flag(flgs, OF_HOLD_LIFE);
    add_flag(flgs, OF_RES_DARK);
    add_flag(flgs, OF_RES_NETHER);
    if (!player_is_ninja) add_flag(flgs, OF_LITE);
    add_flag(flgs, OF_RES_POIS);
    add_flag(flgs, OF_RES_COLD);
}
static void _vampire_birth(void)
{
    if (p_ptr->pclass != CLASS_BERSERKER) py_birth_obj_aux(TV_SCROLL, SV_SCROLL_DARKNESS, rand_range(2, 5));
}
race_t *vampire_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Vampire";
        me.desc = "One of the mightier undead creatures, the vampire is an awe-inspiring sight. Yet this "
                    "dread creature has a serious weakness: the bright rays of sun are its bane, and it "
                    "will need to flee the surface to the deep recesses of earth until the sun finally "
                    "sets. Darkness, on the other hand, holds few terrors to a vampire. Being undead, "
                    "vampires also resist nether, cold and poison and have a firm hold on their life force; "
                    "but they are susceptible to a perpetual hunger for fresh blood, which can only be "
                    "satiated by sucking the blood from a nearby monster.";

        me.stats[A_STR] =  3;
        me.stats[A_INT] =  3;
        me.stats[A_WIS] = -1;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] =  1;
        me.stats[A_CHR] =  2;

        me.skills.dis = 4;
        me.skills.dev = 5;
        me.skills.sav = 6;
        me.skills.stl = 4;
        me.skills.srh = 1;
        me.skills.fos = 8;
        me.skills.thn = 5;
        me.skills.thb = 0;

        me.life = 102;
        me.base_hp = 22;
        me.exp = 200;
        me.infra = 5;
        me.flags = RACE_IS_NONLIVING | RACE_IS_UNDEAD;
        me.shop_adjust = 130;

        me.birth = _vampire_birth;
        me.calc_bonuses = _vampire_calc_bonuses;
        me.get_powers = _vampire_get_powers;
        me.get_flags = _vampire_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Wood-Elf
 ****************************************************************/
static power_info _wood_elf_powers[] =
{
    { A_WIS, {20, 15, 50, nature_awareness_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _wood_elf_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _wood_elf_powers);
}
race_t *wood_elf_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Wood-Elf";
        me.desc = "Wood-Elves are the most common of elves. They prefer the seclusion of thick "
                    "forests and are unhampered when moving through dense foliage. Their skills "
                    "with tracking and bow are unsurpassed, and as they advance they gain the "
                    "power of Nature Awareness.";

        me.stats[A_STR] = -1;
        me.stats[A_INT] =  1;
        me.stats[A_WIS] =  2;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] =  2;

        me.skills.dis = 5;
        me.skills.dev = 4;
        me.skills.sav = 4;
        me.skills.stl = 3;
        me.skills.srh = 8;
        me.skills.fos = 12;
        me.skills.thn = -5;
        me.skills.thb = 12;

        me.life = 97;
        me.base_hp = 16;
        me.exp = 125;
        me.infra = 3;
        me.shop_adjust = 95;

        me.get_powers = _wood_elf_get_powers;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Yeek
 ****************************************************************/
static power_info _yeek_powers[] =
{
    { A_WIS, {15, 15, 50, scare_monster_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _yeek_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _yeek_powers);
}
static void _yeek_calc_bonuses(void)
{
    res_add(RES_ACID);
    if (p_ptr->lev >= 20)
        res_add_immune(RES_ACID);
}
static void _yeek_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_RES_ACID);
    if (p_ptr->lev >= 20)
        add_flag(flgs, OF_IM_ACID);
}
race_t *yeek_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Yeek";
        me.desc = "Yeeks are among the most pathetic creatures. Fortunately, their horrible screams "
                    "can scare away less confident foes, and their skin becomes more and more resistant "
                    "to acid as they gain experience. They are also fairly good at magic; but in combat, "
                    "even a mediocre monster can wipe the proverbial floor with an unwary Yeek.";

        me.stats[A_STR] = -2;
        me.stats[A_INT] =  1;
        me.stats[A_WIS] = -2;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] = -2;
        me.stats[A_CHR] = -4;

        me.skills.dis = 2;
        me.skills.dev = 3;
        me.skills.sav = 6;
        me.skills.stl = 3;
        me.skills.srh = 5;
        me.skills.fos = 15;
        me.skills.thn = -5;
        me.skills.thb = -3;

        me.life = 92;
        me.base_hp = 14;
        me.exp = 70;
        me.infra = 2;
        me.shop_adjust = 105;

        me.calc_bonuses = _yeek_calc_bonuses;
        me.get_powers = _yeek_get_powers;
        me.get_flags = _yeek_get_flags;
        init = TRUE;
    }

    return &me;
}

/****************************************************************
 * Zombie
 ****************************************************************/
static power_info _zombie_powers[] =
{
    { A_WIS, {30, 30, 70, restore_life_spell}},
    { -1, {-1, -1, -1, NULL} }
};
static int _zombie_get_powers(spell_info* spells, int max)
{
    return get_powers_aux(spells, max, _zombie_powers);
}
static void _zombie_calc_bonuses(void)
{
    res_add(RES_NETHER);
    p_ptr->hold_life++;
    p_ptr->see_inv++;
    res_add(RES_POIS);
    p_ptr->slow_digest = TRUE;
    if (p_ptr->lev >= 5) res_add(RES_COLD);
}
static void _zombie_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    add_flag(flgs, OF_SEE_INVIS);
    add_flag(flgs, OF_HOLD_LIFE);
    add_flag(flgs, OF_RES_NETHER);
    add_flag(flgs, OF_RES_POIS);
    add_flag(flgs, OF_SLOW_DIGEST);
    if (p_ptr->lev >= 5)
        add_flag(flgs, OF_RES_COLD);
}
static void _zombie_birth(void)
{
    py_birth_obj_aux(TV_STAFF, EFFECT_NOTHING, 1);
    py_birth_light();
}
race_t *zombie_get_race(void)
{
    static race_t me = {0};
    static bool init = FALSE;

    if (!init)
    {
        me.name = "Zombie";
        me.desc = "Zombies are undead horrors, resistant to life draining and the forces "
            "of the netherworld. The grave is cold, but this does not bother the undead, and "
            "poison also scarcely affects the unliving. Zombies gain little nutrition from "
            "ordinary food; instead, they absorb energy from magical devices to maintain "
            "their undead existence.";

        me.stats[A_STR] =  2;
        me.stats[A_INT] = -6;
        me.stats[A_WIS] = -6;
        me.stats[A_DEX] =  1;
        me.stats[A_CON] =  4;
        me.stats[A_CHR] = -3;

        me.skills.dis = -5;
        me.skills.dev = -5;
        me.skills.sav = 5;
        me.skills.stl = -1;
        me.skills.srh = -1;
        me.skills.fos = 5;
        me.skills.thn = 15;
        me.skills.thb = 0;

        me.life = 108;
        me.base_hp = 24;
        me.exp = 180;
        me.infra = 2;
        me.flags = RACE_IS_NONLIVING | RACE_IS_UNDEAD;
        me.shop_adjust = 140;

        me.birth = _zombie_birth;
        me.calc_bonuses = _zombie_calc_bonuses;
        me.get_powers = _zombie_get_powers;
        me.get_flags = _zombie_get_flags;
        init = TRUE;
    }

    return &me;
}

