#ifndef CHAOS_PATRON_H
#define CHAOS_PATRON_H

/*
 * chaos_patron.h:
 * Definitions for Chaos Patron code (from Composband)
 */

 /* patron interaction triggers */
#define PATRON_HIT 0
#define PATRON_KILL_WEAK 1
#define PATRON_KILL 2
#define PATRON_KILL_UNIQUE 3
#define PATRON_KILL_FAMOUS 4
#define PATRON_KILL_GOOD 5
#define PATRON_KILL_DEMON 6
#define PATRON_CAST 7
#define PATRON_VILLIANY 8
#define PATRON_CHANCE 9
#define PATRON_TAKE_HIT 10
#define PATRON_LEVEL_UP 11
#define PATRON_EFFECT_MAX 12

/* patrons by name */
#define PATRON_SLORTAR 0
#define PATRON_MABELODE 1
#define PATRON_CHARDROS 2
#define PATRON_HIONHURN 3
#define PATRON_XIOMBARG 4
#define PATRON_PYARAY 5
#define PATRON_BALAAN 6
#define PATRON_ARIOCH 7
#define PATRON_EEQUOR 8
#define PATRON_NARJHAN 9
#define PATRON_BALO 10
#define PATRON_KHORNE 11
#define PATRON_SLAANESH 12
#define PATRON_NURGLE 13
#define PATRON_TZEENTCH 14
#define PATRON_KHAINE 15


#define REW_POLY_SLF    1
#define REW_GAIN_EXP    2
#define REW_LOSE_EXP    3
#define REW_GOOD_OBJ    4
#define REW_GREA_OBJ    5
#define REW_CHAOS_WP    6
#define REW_GOOD_OBS    7
#define REW_GREA_OBS    8
#define REW_TY_CURSE    9
#define REW_SUMMON_M    10
#define REW_H_SUMMON    11
#define REW_DO_HAVOC    12
#define REW_GAIN_ABL    13
#define REW_LOSE_ABL    14
#define REW_RUIN_ABL    15
#define REW_AUGM_ABL    16
#define REW_POLY_WND    17
#define REW_HEAL_FUL    18
#define REW_HURT_LOT    19
#define REW_CURSE_WP    20
#define REW_CURSE_AR    21
#define REW_PISS_OFF    22
#define REW_WRATH       23
#define REW_DESTRUCT    24
#define REW_GENOCIDE    25
#define REW_MASS_GEN    26
#define REW_DISPEL_C    27
#define REW_DAMNATION   28 /* new chaos mage rewards */
#define REW_DISORDER    29
#define REW_EXULTATION  30
#define REW_ENERGISE    31
#define REW_DEVICE      32 /* fin */
#define REW_IGNORE      33
#define REW_SER_UNDE    34
#define REW_SER_DEMO    35
#define REW_SER_MONS    36  
#define REW_MUTATE		37

/* reward types */
#define REW_TYPE_PUNISH  0
#define REW_TYPE_ANNOY   1
#define REW_TYPE_REWARD  2
#define REW_TYPE_FAVOUR  3
#define REW_CATEGORIES   4

/* patron attitudes */
#define AMBIVALENT 0
#define SCORNFUL 1
#define AMUSED 2
#define INTERESTED 3
#define APPROVING 4
#define ATTITUDE_MAX 5


/*todo: this*/
struct chaos_patron
{
	int stat;
	int rewards[REW_CATEGORIES][5];
	cptr name;
	cptr title;
	int noticechance[PATRON_EFFECT_MAX];
	int attitudes[PATRON_EFFECT_MAX];
};

/* chaos_patron.c */
extern void chaos_choose_effect(int);
extern bool worships_chaos();
extern cptr chaos_patron_name(int);
extern void chaos_patron_reward(int category);
extern void do_energise(void);
extern void nonlethal_ty_substitute(bool do_dec);

extern int chaos_stats[MAX_PATRON];
#endif // !CHAOS_PATRON_H
