/****************************************************************
 * The Elementalist
 ****************************************************************/

#include "angband.h"


static power_info _get_powers[] =
{
    { A_WIS, { 15, 0, 10, clear_mind_spell}},
    { -1, {-1, -1, -1, NULL}}
};

#define ELEMENT1 0
#define ELEMENT2 1
#define ELEMENT3 2

cptr element_name(int which)
{
    switch (p_ptr->psubclass)
    {
    case ELEMENT_FIRE:
        switch (which)
        {
        case ELEMENT1: return "Fire";
        case ELEMENT2: return "Hellfire";
        case ELEMENT3: return "Plasma";
        };
    case ELEMENT_ICE:
        switch (which)
        {
        case ELEMENT1: return "Cold";
        case ELEMENT2: return "Ice";
        case ELEMENT3: return "Time";
        };
    case ELEMENT_SKY:
        switch (which)
        {
        case ELEMENT1: return "Lightning";
        case ELEMENT2: return "Light";
        case ELEMENT3: return "Mana";
        };
    case ELEMENT_SEA:
        switch (which)
        {
        case ELEMENT1: return "Acid";
        case ELEMENT2: return "Water";
        case ELEMENT3: return "Disintegrate";
        };
    case ELEMENT_DARKNESS:
        switch (which)
        {
        case ELEMENT1: return "Darkness";
        case ELEMENT2: return "Nether";
        case ELEMENT3: return "Inertia";
        };
    case ELEMENT_CHAOS:
        switch (which)
        {
        case ELEMENT1: return "Confusion";
        case ELEMENT2: return "Chaos";
        case ELEMENT3: return "Nexus";
        };
    case ELEMENT_EARTH:
        switch (which)
        {
        case ELEMENT1: return "Shards";
        case ELEMENT2: return "Force";
        case ELEMENT3: return "Meteor";
        };
    case ELEMENT_DEATH:
        switch (which)
        {
        case ELEMENT1: return "Poison";
        case ELEMENT2: return "Toxic Waste";
        case ELEMENT3: return "Disenchantment";
        };
    }

    return "BUG!";
}

int element_gf(int which)
{
    switch (p_ptr->psubclass)
    {
    case ELEMENT_FIRE:
        switch (which)
        {
        case ELEMENT1: return GF_FIRE;
        case ELEMENT2: return GF_HELL_FIRE;
        case ELEMENT3: return GF_PLASMA;
        };
    case ELEMENT_ICE:
        switch (which)
        {
        case ELEMENT1: return GF_COLD;
        case ELEMENT2: return GF_ICE;
        case ELEMENT3: return GF_TIME;
        };
    case ELEMENT_SKY:
        switch (which)
        {
        case ELEMENT1: return GF_ELEC;
        case ELEMENT2: return GF_LITE;
        case ELEMENT3: return GF_MANA;
        };
    case ELEMENT_SEA:
        switch (which)
        {
        case ELEMENT1: return GF_ACID;
        case ELEMENT2: return GF_WATER;
        case ELEMENT3: return GF_DISINTEGRATE;
        };
    case ELEMENT_DARKNESS:
        switch (which)
        {
        case ELEMENT1: return GF_DARK;
        case ELEMENT2: return GF_NETHER;
        case ELEMENT3: return GF_INERT;
        };
    case ELEMENT_CHAOS:
        switch (which)
        {
        case ELEMENT1: return GF_CONFUSION;
        case ELEMENT2: return GF_CHAOS;
        case ELEMENT3: return GF_NEXUS;
        };
    case ELEMENT_EARTH:
        switch (which)
        {
        case ELEMENT1: return GF_SHARDS;
        case ELEMENT2: return GF_FORCE;
        case ELEMENT3: return GF_METEOR;
        };
    case ELEMENT_DEATH:
        switch (which)
        {
        case ELEMENT1: return GF_POIS;
        case ELEMENT2: return GF_NUKE;
        case ELEMENT3: return GF_DISENCHANT;
        };
    }

    return GF_NONE;
}

static void _bolt_spell(int cmd, variant* res)
{
    int dd = 3 + ((p_ptr->lev - 1) / 5);
    int ds = 4;
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Bolt", element_name(ELEMENT1)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a weak bolt of %s.", element_name(ELEMENT1)));
        break;
    case SPELL_INFO:
        //printf(p, " %s%dd%d", KWD_DAM, 3 + ((plev - 1) / 5), 4);
        var_set_string(res, info_damage(spell_power(dd), ds, spell_power(p_ptr->to_d_spell)));
        break;
    case SPELL_CAST:
    {
        int dir;
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;

        fire_bolt_or_beam(
            beam_chance() - 10,
            element_gf(ELEMENT1),
            dir,
            spell_power(damroll(dd, ds) + p_ptr->to_d_spell)
        );
        var_set_bool(res, TRUE);
        break;
    }
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _detect_monsters_spell(int cmd, variant* res)
{
    int b = spell_power(0);
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, "Detect Monsters");
        break;
    case SPELL_DESC:
        var_set_string(res, "Detects all monsters in your vicinity unless invisible.");
        break;
    case SPELL_INFO:
        var_set_string(res, info_radius(DETECT_RAD_DEFAULT));
        break;
    case SPELL_CAST:
        detect_monsters_normal(DETECT_RAD_DEFAULT);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _psychometry_spell(int cmd, variant* res)
{
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, "Psychometry");
        break;
    case SPELL_DESC:
        var_set_string(res, "Returns a feeling about an item. Identifies the item at level 20 and above.");
        break;
    case SPELL_SPOIL_DESC:
        var_set_string(res, "Pseudo-identifies an object. At L20, identifies an object instead.");
        break;
    case SPELL_CAST:
    {
        if (p_ptr->lev < 20)
            var_set_bool(res, psychometry());
        else
            var_set_bool(res, ident_spell(NULL));
        break;
    }
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _cure_wounds_spell(int cmd, variant* res)
{
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, "Cure Light Wounds");
        break;
    case SPELL_DESC:
        var_set_string(res, "Heals cut and HP a little.");
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(2, spell_power(10), 0));
        break;
    case SPELL_CAST:
        hp_player(spell_power(damroll(2, 10)));
        set_cut(p_ptr->cut - 10, TRUE);
        var_set_bool(res, TRUE);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _bolt2_spell(int cmd, variant* res)
{
    int dd = 8 + ((p_ptr->lev - 5) / 4);
    int ds = 8;
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Bolt", element_name(ELEMENT2)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a bolt of %s.", element_name(ELEMENT2)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(spell_power(dd), ds, spell_power(p_ptr->to_d_spell)));
        break;
    case SPELL_CAST:
    {
        int dir;
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;

        fire_bolt_or_beam(
            beam_chance() - 10,
            element_gf(ELEMENT2),
            dir,
            spell_power(damroll(dd, ds) + p_ptr->to_d_spell)
        );
        var_set_bool(res, TRUE);
        break;
    }
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _detect_magic_spell(int cmd, variant* res)
{
    int rad = DETECT_RAD_DEFAULT;
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, "Detect Magical Objects");
        break;
    case SPELL_DESC:
        var_set_string(res, "Detects all magical items in your vicinity.");
        break;
    case SPELL_INFO:
        var_set_string(res, format("%s", info_radius(rad)));
        break;
    case SPELL_CAST:
        detect_objects_magic(rad);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _spout_spell(int cmd, variant* res)
{
    int dir = 0;
    int dam = spell_power(p_ptr->lev * 2 + 50 + p_ptr->to_d_spell);
    /*bool fire_ball(int typ, int dir, int dam, int rad)*/
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Spout", element_name(ELEMENT3)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a strong, short-range, ball of %s.", element_name(ELEMENT3)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        project_length = 4;
        fire_ball(element_gf(ELEMENT3), dir, dam, 1);
        project_length = 0;
        var_set_bool(res, TRUE);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _ball_spell(int cmd, variant* res)
{
    int dir = 0;
    int dam = spell_power(p_ptr->lev + 55 + p_ptr->to_d_spell);

    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Ball", element_name(ELEMENT1)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a ball of %s.", element_name(ELEMENT1)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        fire_ball(element_gf(ELEMENT1), dir, dam, 2);
        var_set_bool(res, TRUE);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _breath_spell(int cmd, variant* res)
{
    int dir = 0;
    int dam = spell_power(MIN(150, p_ptr->chp / 2) + p_ptr->to_d_spell);

    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("Breathe %s", element_name(ELEMENT2)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a breath of %s.", element_name(ELEMENT2)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        fire_ball(element_gf(ELEMENT2), dir, dam, 3);
        var_set_bool(res, TRUE);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _annihilation_spell(int cmd, variant* res)
{
    int power = spell_power(p_ptr->lev + 50);
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, "Annihilation");
        break;
    case SPELL_DESC:
        var_set_string(res, format("Erase a monster unless it resists %s.", element_name(ELEMENT1)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_power(power));
        break;
    case SPELL_CAST:
    {
        int dir = 0;
        //TODO: How to implement this?
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        fire_ball_hide(GF_E_ANNIHILATE, dir, power, 0);
        var_set_bool(res, TRUE);
        break;
    }
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _bolt3_spell(int cmd, variant* res)
{
    int dd = 12 + ((p_ptr->lev - 5) / 4);
    int ds = 8;
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Bolt", element_name(ELEMENT3)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a bolt of %s.", element_name(ELEMENT3)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(spell_power(dd), ds, spell_power(p_ptr->to_d_spell)));
        break;
    case SPELL_CAST:
    {
        int dir;
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;

        fire_bolt_or_beam(
            beam_chance() - 10,
            element_gf(ELEMENT3),
            dir,
            spell_power(damroll(dd, ds) + p_ptr->to_d_spell)
        );
        var_set_bool(res, TRUE);
        break;
    }
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _wave_spell(int cmd, variant* res)
{
    int dam = spell_power(50 + randint1(p_ptr->lev * 3) + p_ptr->to_d_spell);
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Wave", element_name(ELEMENT1)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Inflict %s damage on all monsters in sight.", element_name(ELEMENT1)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
        project_hack(element_gf(ELEMENT1), dam); /* LOS damage */
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

/* case ElementSpells::BALL_2ND:
sprintf(p, " %s%d", KWD_DAM, 75 + plev * 3 / 2);
break;*/
static void _ball2_spell(int cmd, variant* res)
{
    int dam = spell_power((p_ptr->lev * 3 / 2) + 75 + p_ptr->to_d_spell);

    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Ball", element_name(ELEMENT2)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a ball of %s.", element_name(ELEMENT2)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
    {
        int dir = 0;
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        fire_ball(element_gf(ELEMENT2), dir, dam, 3);
        var_set_bool(res, TRUE);
        /*if (typ == AttributeType::HYPODYNAMIA) {
                (void)hp_player(player_ptr, dam / 2);*/
    }
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

/* case ElementSpells::BURST_1ST:
sprintf(p, " %s%dd%d", KWD_DAM, 6 + plev / 8, 7);
break;*/
static void _blast_spell(int cmd, variant* res)
{
    int b = spell_power(0);
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Burst", element_name(ELEMENT1)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire bolts of %s in random directions.", element_name(ELEMENT1)));
        break;
    case SPELL_INFO:
        var_set_string(res, format("dur %d", b));
        break;
    case SPELL_CAST:
    {
        int k, x = 0, y = 0;
        int num = damroll(4, 3);
        int typ = element_gf(ELEMENT1);

        for (k = 0; k < num; k++)
        {
            int attempts = 1000;

            while (attempts--)
            {
                scatter(&y, &x, py, px, 4, 0);

                if (!player_bold(y, x)) break;
            }
            int dam = spell_power(damroll(6 + (p_ptr->lev / 8), 7) + p_ptr->to_d_spell);
            project(0, 0, y, x, dam, typ,
                (PROJECT_BEAM | PROJECT_THRU | PROJECT_GRID | PROJECT_KILL));
        }

        var_set_bool(res, TRUE);
    }
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

/* case ElementSpells::STORM_2ND:
sprintf(p, " %s%d", KWD_DAM, 115 + plev * 5 / 2);
break;*/

/*
if (!get_aim_dir(player_ptr, &dir))
            return false;
        dam = 115 + plev * 5 / 2;
        typ = get_element_spells_type(player_ptr, power.elem);
        if (fire_ball(player_ptr, typ, dir, dam, 4)) {
            if (typ == AttributeType::HYPODYNAMIA) {
                (void)hp_player(player_ptr, dam / 2);
            }
        }
        break;

*/
static void _storm_spell(int cmd, variant* res)
{
    int dam = spell_power(115 + p_ptr->lev * 5 / 2 + p_ptr->to_d_spell);
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Storm", element_name(ELEMENT2)));
        break;
    case SPELL_DESC:
        var_set_string(res,format("Fire a large ball of %s.", element_name(ELEMENT2)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
    {
        int dir = 0;
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        fire_ball(element_gf(ELEMENT2), dir, dam, 4);
        var_set_bool(res, TRUE);
        /*if (typ == AttributeType::HYPODYNAMIA) {
                (void)hp_player(player_ptr, dam / 2);*/
    }
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

/* case ElementSpells::BREATH_1ST:
sprintf(p, " %s%d", KWD_DAM, p_ptr->chp * 2 / 3);
break;*/
static void _breath2_spell(int cmd, variant* res)
{
    int dir = 0;
    int dam = spell_power(p_ptr->chp * 2 / 3 + p_ptr->to_d_spell);

    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("Breathe %s", element_name(ELEMENT1)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a breath of %s.", element_name(ELEMENT1)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        fire_ball(element_gf(ELEMENT1), dir, dam, 3);
        var_set_bool(res, TRUE);
        break;
    default:
        default_spell(cmd, res);
        break;
    }
}

static void _storm2_spell(int cmd, variant* res)
{
    int dam = spell_power(300 + p_ptr->lev * 5 + p_ptr->to_d_spell);
    switch (cmd)
    {
    case SPELL_NAME:
        var_set_string(res, format("%s Storm", element_name(ELEMENT3)));
        break;
    case SPELL_DESC:
        var_set_string(res, format("Fire a large ball of %s.", element_name(ELEMENT3)));
        break;
    case SPELL_INFO:
        var_set_string(res, info_damage(0, 0, dam));
        break;
    case SPELL_CAST:
    {
        int dir = 0;
        var_set_bool(res, FALSE);
        if (!get_fire_dir(&dir)) return;
        fire_ball(element_gf(ELEMENT3), dir, dam, 5);
        var_set_bool(res, TRUE);
        /*if (typ == AttributeType::HYPODYNAMIA) {
                (void)hp_player(player_ptr, dam / 2);*/
    }
    break;
    default:
        default_spell(cmd, res);
        break;
    }
}

/*
static element_power_list element_powers = {
    { ElementSpells::BOLT_1ST,   { 0, {  1,  1,  15, _("%s Bolt") }}},
    { ElementSpells::MON_DETECT, { 0, {  2,  2,  20, _("Detect Monsters") }}},
    { ElementSpells::PERCEPT,    { 0, {  5,  5,  50, _("Psychometry") }}},
    { ElementSpells::CURE,       { 0, {  6,  5,  35, _("Cure Wounds") }}},
    { ElementSpells::BOLT_2ND,   { 1, {  8,  6,  35, _("%s Bolt") }}},
    { ElementSpells::MAG_DETECT, { 0, { 10,  8,  60, _("Detect Magical Objs") }}},
    { ElementSpells::BALL_3RD,   { 2, { 15, 10,  55, _("%s Spout") }}},
    { ElementSpells::BALL_1ST,   { 0, { 18, 13,  65, _("%s Ball") }}},
    { ElementSpells::BREATH_2ND, { 1, { 21, 20,  70, _("Breath of %s") }}},
    { ElementSpells::ANNIHILATE, { 0, { 24, 20,  75, _("Annihilation") }}},
    { ElementSpells::BOLT_3RD,   { 2, { 25, 15,  60, _("%s Bolt") }}},
    { ElementSpells::WAVE_1ST,   { 0, { 28, 30,  75, _("Elemental Wave") }}},
    { ElementSpells::BALL_2ND,   { 1, { 28, 22,  75, _("%s Ball") }}},
    { ElementSpells::BURST_1ST,  { 0, { 33, 35,  75, _("%s Blast") }}},
    { ElementSpells::STORM_2ND,  { 1, { 35, 30,  75, _("%s Storm") }}},
    { ElementSpells::BREATH_1ST, { 0, { 42, 48,  75, _("Breath of %s") }}},
    { ElementSpells::STORM_3ND,  { 2, { 45, 60,  80, _("%s Storm") }}},
};
*/
static spell_info _get_spells[] =
{
    /*lvl cst fail spell */
    {  1, 1, 15, _bolt_spell},
    {  2, 2, 20, _detect_monsters_spell},
    {  5, 5, 50, _psychometry_spell},
    {  6, 5, 35, _cure_wounds_spell},
    {  8, 6, 35, _bolt2_spell},
    { 10, 8, 60, _detect_magic_spell},
    { 15, 10, 55, _spout_spell},
    { 18, 13, 65, _ball_spell},
    { 21, 20, 70, _breath_spell},
    { 24, 20, 75, _annihilation_spell},
    { 25, 15, 60, _bolt2_spell},
    { 28, 30, 75, _wave_spell},
    { 28, 22, 75, _ball2_spell},
    { 33, 35, 75, _blast_spell},
    { 35, 30, 75, _storm_spell},
    { 42, 48, 75, _breath2_spell},
    { 45, 60, 80, _storm2_spell},
    { -1, -1, -1, NULL},
};

static caster_info* _caster_info(void)
{
    static caster_info me = { 0 };
    static bool init = FALSE;
    if (!init)
    {
        me.which_stat = A_WIS;
        me.magic_desc = "spell";
        me.encumbrance.max_wgt = 430; /* Encumbered as a mage */
        me.encumbrance.weapon_pct = 100;
        me.encumbrance.enc_wgt = 600;
        /* Wizardstaff proficiency, No glove encumbrance or pointy penalty */
        me.options = CASTER_ALLOW_DEC_MANA; 
        init = TRUE;
    }
    return &me;
}

cptr elementalist_speciality_name(int psubclass)
{
    switch (psubclass)
    {
    case ELEMENT_FIRE: return "Fire";
    case ELEMENT_ICE: return "Frost";
    case ELEMENT_SKY: return "Sky";
    case ELEMENT_SEA: return "Sea";
    case ELEMENT_DARKNESS: return "Darkness";
    case ELEMENT_CHAOS: return "Chaos";
    case ELEMENT_EARTH: return "Earth";
    case ELEMENT_DEATH: return "Death";
    }
    return "";
}

cptr elementalist_speciality_desc(int psubclass)
{
    switch (psubclass)
    {
    case ELEMENT_FIRE: return "Great energy of Fire system will be able to burn out all of your enemies.";
    case ELEMENT_ICE: return "Ice system will freeze your enemies, even their souls.";
    case ELEMENT_SKY: return "Sky system can terminate all of your enemies powerfully with the energy of the great sky.";
    case ELEMENT_SEA: return "Sea system melts all of your enemies and returns them to the great ocean.";
    case ELEMENT_DARKNESS: return "Dark system draws terrifying power from the darkness and knocks your enemies into hell.";
    case ELEMENT_CHAOS: return "Chaos system twists and wraps your enemies, even their souls, and scatters them as dust in the wind.";
    case ELEMENT_EARTH: return "Earth system smashes all of your enemies massively using its huge powers.";
    case ELEMENT_DEATH: return "Death system is a tremendous poison for all living enemies.";
    }
    return "";
}

static void _birth(void)
{
    /*Ring of sustain wisdom*/
    py_birth_obj_aux(TV_SWORD, SV_DAGGER, 1);
	py_birth_obj_aux(TV_WAND, EFFECT_BOLT_MISSILE, 1);
    py_birth_obj_aux(TV_POTION, SV_POTION_SPEED, 1);
}

static void _calc_bonuses(void)
{
    if (p_ptr->lev >= 30) res_add(RES_TIME);
    p_ptr->pspeed += (p_ptr->lev) / 7;

    switch (p_ptr->psubclass)
    {
    case ELEMENT_FIRE:
        res_add(RES_FIRE);
        if (p_ptr->lev >= 30) res_add_immune(RES_FIRE);
        break;
    case ELEMENT_ICE:
        res_add(RES_COLD);
        if (p_ptr->lev >= 30) res_add_immune(RES_COLD);
        break;
    case ELEMENT_SKY:
        res_add(RES_ELEC);
        if (p_ptr->lev >= 30) res_add_immune(RES_ELEC);
        break;
    case ELEMENT_SEA:
        res_add(RES_ACID);
        if (p_ptr->lev >= 30)
        {
            res_add_immune(RES_ACID);
        }
        break;
    case ELEMENT_DARKNESS:
        res_add(RES_DARK);
        if (p_ptr->lev >= 30) res_add(RES_NETHER);
        break;
    case ELEMENT_CHAOS:
        res_add(RES_CONF);
        if (p_ptr->lev >= 30) res_add(RES_CHAOS);
        break;
    case ELEMENT_EARTH:
        res_add(RES_SHARDS);
        if (p_ptr->lev >= 30) p_ptr->reflect = TRUE;
        break;
    case ELEMENT_DEATH:
        res_add(RES_POIS);
        if (p_ptr->lev >= 30) res_add(RES_DISEN); 
        break;
    default:
        break;
    }
}

static void _get_flags(u32b flgs[OF_ARRAY_SIZE])
{
    switch (p_ptr->psubclass)
    {
    case ELEMENT_FIRE:
        add_flag(flgs, OF_RES_FIRE);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_IM_FIRE);
        break;
    case ELEMENT_ICE:
        add_flag(flgs, OF_RES_COLD);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_IM_COLD);
        break;
    case ELEMENT_SKY:
        add_flag(flgs, OF_RES_ELEC);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_IM_ELEC);
        break;
    case ELEMENT_SEA:
        add_flag(flgs, OF_RES_ACID);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_IM_ACID);
        break;
    case ELEMENT_DARKNESS:
        add_flag(flgs, OF_RES_DARK);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_RES_NETHER);
        break;
    case ELEMENT_CHAOS:
        add_flag(flgs, OF_RES_CONF);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_RES_CHAOS);
        break;
    case ELEMENT_EARTH:
        add_flag(flgs, OF_RES_SHARDS);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_REFLECT);
        break;
    case ELEMENT_DEATH:
        add_flag(flgs, OF_RES_POIS);
        if (p_ptr->lev >= 30) add_flag(flgs, OF_RES_DISEN);
        break;
    default:
        break;
    }
}

class_t* elementalist_get_class(int psubclass)
{
    static class_t me = { 0 };
    static bool init = FALSE;

    /* static info never changes */
    if (!init)
    {           /* dis, dev, sav, stl, srh, fos, thn, thb */
        skills_t bs = { 30, 40, 38, 3, 16, 20, 34, 20 };
        skills_t xs = {  7, 15, 11, 0,  0,  0,  6,  7 };

        me.name = "Elementalist";
        me.desc = "An Elementalist is a spell caster that specializes in tapping elemental "
            "forces.  They have a limited, but powerful, repertoire of spells. "
            "Wisdom determines an Elementalist's spell casting ability.";

        me.stats[A_STR] = -3;
        me.stats[A_INT] = 1;
        me.stats[A_WIS] = 3;
        me.stats[A_DEX] = 0;
        me.stats[A_CON] = -1;
        me.stats[A_CHR] = 0;
        
        me.base_skills = bs;
        me.extra_skills = xs;
        me.life = 95;
        me.base_hp = 0;
        me.exp = 100;
        me.pets = 30;
        me.flags = CLASS_SENSE1_MED | CLASS_SENSE1_WEAK |
            CLASS_SENSE2_FAST | CLASS_SENSE2_STRONG |
            CLASS_REGEN_MANA;

        me.birth = _birth;
        me.calc_bonuses = _calc_bonuses;
        me.get_flags = _get_flags;
        me.caster_info = _caster_info;
        me.get_spells = _get_spells;
        me.character_dump = py_dump_spells;
        me.get_powers = _get_powers;
        init = TRUE;
    }

    me.subname = elementalist_speciality_name(psubclass);
    me.subdesc = elementalist_speciality_desc(psubclass);
    return &me;
}