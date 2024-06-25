/* File: tables.c */

/*
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies. Other copyrights may also apply.
 */

/* Purpose: Angband Tables */

#include "angband.h"

/*
 * Global array for looping through the "keypad directions"
 */
s16b ddd[9] =
{ 2, 8, 6, 4, 3, 1, 9, 7, 5 };

/*
 * Global arrays for converting "keypad direction" into offsets
 */
s16b ddx[10] =
{ 0, -1, 0, 1, -1, 0, 1, -1,  0,  1 };

s16b ddy[10] =
{ 0, 1,  1, 1,  0, 0, 0, -1, -1, -1 };

/*
 * Global arrays for optimizing "ddx[ddd[i]]" and "ddy[ddd[i]]"
 */
s16b ddx_ddd[9] =
{ 0, 0, 1, -1, 1, -1, 1, -1, 0 };

s16b ddy_ddd[9] =
{ 1, -1, 0, 0, 1, 1, -1, -1, 0 };


/*
 * Circular keypad direction array
 */
s16b cdd[8] =
{ 2, 3, 6, 9, 8, 7, 4, 1 };

/*
 * Global arrays for optimizing "ddx[cdd[i]]" and "ddy[cdd[i]]"
 */
s16b ddx_cdd[8] =
{ 0, 1, 1, 1, 0, -1, -1, -1 };

s16b ddy_cdd[8] =
{ 1, 1, 0, -1, -1, -1, 0, 1 };



/*
 * Global array for converting numbers to uppercase hecidecimal digit
 * This array can also be used to convert a number to an octal digit
 */
char hexsym[16] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


/*
 * Global array for converting numbers to a logical list symbol
 */
char listsym[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '+',
    '=',
    '\0'
};


/*
 * Encode the screen colors
 */
char color_char[] = "dwsorgbuDWvyRGBULPICtSmMTOVcnKpi";

/*
 * Global mixed-case alphanumeric array
 */
char multicase[84] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789#$%&'()*+,-./:;<=>{|}";

/*
 * Stat Table (INT/WIS) -- Number of half-spells per level
 */
byte adj_mag_study[] =
{
    0    /* 3 */,
    0    /* 4 */,
    0    /* 5 */,
    0    /* 6 */,
    0    /* 7 */,
    1    /* 8 */,
    1    /* 9 */,
    1    /* 10 */,
    1    /* 11 */,
    2    /* 12 */,
    2    /* 13 */,
    2    /* 14 */,
    2    /* 15 */,
    2    /* 16 */,
    2    /* 17 */,
    2    /* 18 */,
    2    /* 19 */,
    2    /* 20 */,
    2    /* 21 */,
    2    /* 22 */,
    3    /* 23 */,
    3    /* 24 */,
    3    /* 25 */,
    3    /* 26 */,
    4    /* 27 */,
    4    /* 28 */,
    4    /* 29 */,
    5    /* 30 */,
    5    /* 31 */,
    5    /* 32 */,
    5    /* 33 */,
    5    /* 34 */,
    5    /* 35 */,
    5    /* 36 */,
    5    /* 37 */,
    5    /* 38 */,
    6    /* 39 */,
    6    /* 40+ */
};


/*
 * Stat Table (INT/WIS) -- extra 1/4-mana-points per level
 */
byte adj_mag_mana[] =
{
    0       /* 3 */,
    0       /* 4 */,
    0       /* 5 */,
    0       /* 6 */,
    0       /* 7 */,
    1       /* 8 */,
    2       /* 9 */,
    3       /* 10 */,
    4       /* 11 */,
    5       /* 12 */,
    5       /* 13 */,
    6       /* 14 */,
    7       /* 15 */,
    8       /* 16 */,
    9       /* 17 */,
    10      /* 18 */,
    11      /* 19 */,
    11      /* 20 */,
    12      /* 21 */,
    12      /* 22 */,
    13      /* 23 */,
    14      /* 24 */,
    15      /* 25 */,
    16      /* 26 */,
    17      /* 27 */,
    18      /* 28 */,
    19      /* 29 */,
    20      /* 30 */,
    21      /* 31 */,
    22      /* 32 */,
    23      /* 33 */,
    24      /* 34 */,
    25      /* 35 */,
    26      /* 36 */,
    27      /* 37 */,
    28      /* 38 */,
    29      /* 39 */,
    30      /* 40+ */
};

/* Pseudo-id is currently 1 in (N/f(L)) where f(L) might be L*L+50 or L+5, etc, (class dependent)
   Wisdom will modify N = N * adj / 100 according to the following table. (sense_inventory1 and 2())

   Level feelings take X turns to occur (depending on level and player FOS).
   Wisdom will modify X = X * adj / 100 according to the following table (update_dungeon_feeling())
*/
byte adj_pseudo_id[] =
{
    150      /* 3 */,
    135      /* 4 */,
    127      /* 5 */,
    122      /* 6 */,
    118      /* 7 */,
    115      /* 8 */,
    112      /* 9 */,
    110      /* 10 */,
    108      /* 11 */,
    106      /* 12 */,
    105      /* 13 */,
    104      /* 14 */,
    103      /* 15 */,
    102      /* 16 */,
    101      /* 17 */,
    100      /* 18 */,
     99      /* 19 */,
     98      /* 20 */,
     97      /* 21 */,
     96      /* 22 */,
     95      /* 23 */,
     94      /* 24 */,
     93      /* 25 */,
     92      /* 26 */,
     91      /* 27 */,
     90      /* 28 */,
     88      /* 29 */,
     86      /* 30 */,
     84      /* 31 */,
     82      /* 32 */,
     80      /* 33 */,
     78      /* 34 */,
     76      /* 35 */,
     74      /* 36 */,
     72      /* 37 */,
     70      /* 38 */,
     68      /* 39 */,
     65      /* 40+ */
};

/* XP = XP * adj / 100;
*/
byte adj_exp_gain[] =
{
    90      /* 3 */,
    92      /* 4 */,
    94      /* 5 */,
    96      /* 6 */,
    97      /* 7 */,
    98      /* 8 */,
    99      /* 9 */,
    100     /* 10 */,
    101     /* 11 */,
    102     /* 12 */,
    103     /* 13 */,
    104     /* 14 */,
    105     /* 15 */,
    106     /* 16 */,
    107     /* 17 */,
    108     /* 18 */,
    109     /* 19 */,
    110     /* 20 */,
    111     /* 21 */,
    112     /* 22 */,
    113     /* 23 */,
    114     /* 24 */,
    115     /* 25 */,
    116     /* 26 */,
    117     /* 27 */,
    118     /* 28 */,
    119     /* 29 */,
    120     /* 30 */,
    121     /* 31 */,
    122     /* 32 */,
    123     /* 33 */,
    124     /* 34 */,
    125     /* 35 */,
    126     /* 36 */,
    127     /* 37 */,
    128     /* 38 */,
    130     /* 39 */,
    135     /* 40+ */
};

s16b adj_fear_m[] =
{
     50      /* 3 */,
     60      /* 4 */,
     70      /* 5 */,
     80      /* 6 */,
     85      /* 7 */,
     90      /* 8 */,
     95      /* 9 */,
    100      /* 10 */,
    100      /* 11 */,
    100      /* 12 */,
    100      /* 13 */,
    105      /* 14 */,
    105      /* 15 */,
    105      /* 16 */,
    105      /* 17 */,
    110      /* 18 */,
    110      /* 19 */,
    110      /* 20 */,
    115      /* 21 */,
    115      /* 22 */,
    120      /* 23 */,
    125      /* 24 */,
    130      /* 25 */,
    135      /* 26 */,
    140      /* 27 */,
    145      /* 28 */,
    150      /* 29 */,
    155      /* 30 */,
    160      /* 31 */,
    170      /* 32 */,
    180      /* 33 */,
    190      /* 34 */,
    200      /* 35 */,
    210      /* 36 */,
    220      /* 37 */,
    230      /* 38 */,
    240      /* 39 */,
    250      /* 40+ */
};

s16b adj_stat_save_fear[] =
{
     0     /* 3 */,
     0     /* 4 */,
     1     /* 5 */,
     1     /* 6 */,
     2     /* 7 */,
     3     /* 8 */,
     4     /* 9 */,
     5     /* 10 */,
     6     /* 11 */,
     7     /* 12 */,
     8     /* 13 */,
    10     /* 14 */,
    12     /* 15 */,
    14     /* 16 */,
    16     /* 17 */,
    18     /* 18 */,
    19     /* 19 */,
    20     /* 20 */,
    21     /* 21 */,
    22     /* 22 */,
    23     /* 23 */,
    24     /* 24 */,
    25     /* 25 */,
    26     /* 26 */,
    27     /* 27 */,
    28     /* 28 */,
    29     /* 29 */,
    30     /* 30 */,
    32     /* 31 */,
    34     /* 32 */,
    36     /* 33 */,
    38     /* 34 */,
    40     /* 35 */,
    42     /* 36 */,
    44     /* 37 */,
    47     /* 38 */,
    50     /* 39 */,
    55     /* 40+ */
};

s16b adj_stat_save[] =
{
   -25     /* 3 */,
   -15     /* 4 */,
   -10     /* 5 */,
    -7     /* 6 */,
    -6     /* 7 */,
    -5     /* 8 */,
    -4     /* 9 */,
    -3     /* 10 */,
    -2     /* 11 */,
    -2     /* 12 */,
    -1     /* 13 */,
    -1     /* 14 */,
     0     /* 15 */,
     0     /* 16 */,
     0     /* 17 */,
     0     /* 18 */,
     1     /* 19 */,
     2     /* 20 */,
     3     /* 21 */,
     4     /* 22 */,
     5     /* 23 */,
     6     /* 24 */,
     7     /* 25 */,
     8     /* 26 */,
     9     /* 27 */,
    10     /* 28 */,
    12     /* 29 */,
    14     /* 30 */,
    16     /* 31 */,
    18     /* 32 */,
    20     /* 33 */,
    23     /* 34 */,
    26     /* 35 */,
    29     /* 36 */,
    33     /* 37 */,
    37     /* 38 */,
    42     /* 39 */,
    50     /* 40+ */
};

/*
 * Stat Table (INT/WIS) -- Minimum failure rate (percentage)
 */
byte adj_mag_fail[] =
{
    99      /* 3 */,
    99      /* 4 */,
    99      /* 5 */,
    99      /* 6 */,
    99      /* 7 */,
    50      /* 8 */,
    30      /* 9 */,
    20      /* 10 */,
    15      /* 11 */,
    12      /* 12 */,
    11      /* 13 */,
    10      /* 14 */,
    9       /* 15 */,
    8       /* 16 */,
    7       /* 17 */,
    6       /* 18 */,
    6       /* 19 */,
    5       /* 20 */,
    5       /* 21 */,
    5       /* 22 */,
    4       /* 23 */,
    4       /* 24 */,
    4       /* 25 */,
    4       /* 26 */,
    3       /* 27 */,
    3       /* 28 */,
    2       /* 29 */,
    2       /* 30 */,
    2       /* 31 */,
    2       /* 32 */,
    1       /* 33 */,
    1       /* 34 */,
    1       /* 35 */,
    1       /* 36 */,
    1       /* 37 */,
    0       /* 38 */,
    0       /* 39 */,
    0       /* 40+ */
};


/*
 * Stat Table (INT/WIS) -- Various things
 */
byte adj_mag_stat[] =
{
    0       /* 3 */,
    0       /* 4 */,
    0       /* 5 */,
    0       /* 6 */,
    0       /* 7 */,
    1       /* 8 */,
    1       /* 9 */,
    1       /* 10 */,
    1       /* 11 */,
    1       /* 12 */,
    1       /* 13 */,
    1       /* 14 */,
    2       /* 15 */,
    2       /* 16 */,
    2       /* 17 */,
    3       /* 18 */,
    3       /* 19 */,
    3       /* 20 */,
    3       /* 21 */,
    3       /* 22 */,
    4       /* 23 */,
    4       /* 24 */,
    5       /* 25 */,
    6       /* 26 */,
    7       /* 27 */,
    8       /* 28 */,
    9       /* 29 */,
    10      /* 30 */,
    11      /* 31 */,
    12      /* 32 */,
    13      /* 33 */,
    14      /* 34 */,
    15      /* 35 */,
    16      /* 36 */,
    17      /* 37 */,
    18      /* 38 */,
    19      /* 39 */,
    20      /* 40+ */
};


/*
 * Stat Table (CHR) -- payment percentages
 */
byte adj_gold[] =
{
    130     /* 3 */,
    125     /* 4 */,
    122     /* 5 */,
    120     /* 6 */,
    118     /* 7 */,
    116     /* 8 */,
    114     /* 9 */,
    112     /* 10 */,
    110     /* 11 */,
    108     /* 12 */,
    106     /* 13 */,
    104     /* 14 */,
    103     /* 15 */,
    102     /* 16 */,
    101     /* 17 */,
    100     /* 18 */,
    99      /* 19 */,
    98      /* 20 */,
    97      /* 21 */,
    96      /* 22 */,
    95      /* 23 */,
    94      /* 24 */,
    93      /* 25 */,
    92      /* 26 */,
    91      /* 27 */,
    90      /* 28 */,
    89      /* 29 */,
    88      /* 30 */,
    87      /* 31 */,
    86      /* 32 */,
    85      /* 33 */,
    84      /* 34 */,
    83      /* 35 */,
    82      /* 36 */,
    81      /* 37 */,
    80      /* 38 */,
    79      /* 39 */,
    78      /* 40+ */
};

/*
 * Stat Table (INT) -- Magic devices
 */
s16b adj_int_dev[] =
{
   -5       /* 3 */,
   -3       /* 4 */,
   -2       /* 5 */,
   -1       /* 6 */,
   -1       /* 7 */,
    0       /* 8 */,
    0       /* 9 */,
    0       /* 10 */,
    1       /* 11 */,
    1       /* 12 */,
    1       /* 13 */,
    1       /* 14 */,
    2       /* 15 */,
    2       /* 16 */,
    2       /* 17 */,
    3       /* 18 */,
    3       /* 19 */,
    4       /* 20 */,
    4       /* 21 */,
    5       /* 22 */,
    5       /* 23 */,
    6       /* 24 */,
    6       /* 25 */,
    7       /* 26 */,
    7       /* 27 */,
    8       /* 28 */,
    9       /* 29 */,
    10      /* 30 */,
    11      /* 31 */,
    12      /* 32 */,
    13      /* 33 */,
    14      /* 34 */,
    15      /* 35 */,
    16      /* 36 */,
    17      /* 37 */,
    18      /* 38 */,
    20      /* 39 */,
    23      /* 40+ */
};


/*
 * Stat Table (WIS) -- Saving throw
 */
byte adj_wis_sav[] =
{
    0       /* 3 */,
    0       /* 4 */,
    0       /* 5 */,
    0       /* 6 */,
    0       /* 7 */,
    1       /* 8 */,
    1       /* 9 */,
    1       /* 10 */,
    1       /* 11 */,
    1       /* 12 */,
    1       /* 13 */,
    1       /* 14 */,
    2       /* 15 */,
    2       /* 16 */,
    2       /* 17 */,
    3       /* 18 */,
    3       /* 19 */,
    3       /* 20 */,
    3       /* 21 */,
    3       /* 22 */,
    4       /* 23 */,
    4       /* 24 */,
    5       /* 25 */,
    5       /* 26 */,
    6       /* 27 */,
    7       /* 28 */,
    8       /* 29 */,
    9       /* 30 */,
    10      /* 31 */,
    11      /* 32 */,
    12      /* 33 */,
    13      /* 34 */,
    14      /* 35 */,
    15      /* 36 */,
    17      /* 37 */,
    19      /* 38 */,
    21      /* 39 */,
    25      /* 40+ */
};


/*
 * Stat Table (DEX) -- disarming
 */
byte adj_dex_dis[] =
{
    0       /* 3 */,
    0       /* 4 */,
    0       /* 5 */,
    0       /* 6 */,
    0       /* 7 */,
    0       /* 8 */,
    0       /* 9 */,
    0       /* 10 */,
    0       /* 11 */,
    0       /* 12 */,
    1       /* 13 */,
    1       /* 14 */,
    1       /* 15 */,
    2       /* 16 */,
    2       /* 17 */,
    4       /* 18 */,
    4       /* 19 */,
    4       /* 20 */,
    4       /* 21 */,
    5       /* 22 */,
    5       /* 23 */,
    5       /* 24 */,
    6       /* 25 */,
    6       /* 26 */,
    7       /* 27 */,
    8       /* 28 */,
    8       /* 29 */,
    8       /* 30 */,
    8       /* 31 */,
    8       /* 32 */,
    9       /* 33 */,
    9       /* 34 */,
    9       /* 35 */,
    9       /* 36 */,
    9       /* 37 */,
    10      /* 38 */,
    10      /* 39 */,
    10      /* 40+ */
};


/*
 * Stat Table (INT) -- disarming
 */
byte adj_int_dis[] =
{
    0       /* 3 */,
    0       /* 4 */,
    0       /* 5 */,
    0       /* 6 */,
    0       /* 7 */,
    1       /* 8 */,
    1       /* 9 */,
    1       /* 10 */,
    1       /* 11 */,
    1       /* 12 */,
    1       /* 13 */,
    1       /* 14 */,
    2       /* 15 */,
    2       /* 16 */,
    2       /* 17 */,
    3       /* 18 */,
    3       /* 19 */,
    3       /* 20 */,
    4       /* 21 */,
    4       /* 22 */,
    5       /* 23 */,
    6       /* 24 */,
    7       /* 25 */,
    8       /* 26 */,
    9       /* 27 */,
    10      /* 28 */,
    10      /* 29 */,
    11      /* 30 */,
    12      /* 31 */,
    13      /* 32 */,
    14      /* 33 */,
    15      /* 34 */,
    16      /* 35 */,
    17      /* 36 */,
    18      /* 37 */,
    19      /* 38 */,
    19      /* 39 */,
    20      /* 40+ */
};


/*
 * Stat Table (DEX) -- bonus to ac (plus 128)
 */
byte adj_dex_ta[] =
{
    128 + -4    /*  3 */,
    128 + -3    /*  4 */,
    128 + -2    /*  5 */,
    128 + -1    /*  6 */,
    128 + 0     /*  7 */,
    128 + 0     /*  8 */,
    128 + 0     /*  9 */,
    128 + 0     /* 10 */,
    128 + 0     /* 11 */,
    128 + 0     /* 12 */,
    128 + 0     /* 13 */,
    128 + 0     /* 14 */,
    128 + 1     /* 15 */,
    128 + 1     /* 16 */,
    128 + 1     /* 17 */,
    128 + 2     /* 18 */,
    128 + 2     /* 19 */,
    128 + 2     /* 20 */,
    128 + 2     /* 21 */,
    128 + 2     /* 22 */,
    128 + 3     /* 23 */,
    128 + 3     /* 24 */,
    128 + 3     /* 25 */,
    128 + 4     /* 26 */,
    128 + 5     /* 27 */,
    128 + 6     /* 28 */,
    128 + 7     /* 29 */,
    128 + 8     /* 30 */,
    128 + 9     /* 31 */,
    128 + 9     /* 32 */,
    128 + 10    /* 33 */,
    128 + 11    /* 34 */,
    128 + 12    /* 35 */,
    128 + 13    /* 36 */,
    128 + 14    /* 37 */,
    128 + 16    /* 38 */,
    128 + 18    /* 39 */,
    128 + 20    /* 40+ */
};

/*
 * Stat Table (STR) -- bonus to dam (plus 128)
 */
byte adj_str_td[] =
{
    128 + -2    /*  3 */,
    128 + -2    /*  4 */,
    128 + -1    /*  5 */,
    128 + -1    /*  6 */,
    128 + 0     /*  7 */,
    128 + 0     /*  8 */,
    128 + 0     /*  9 */,
    128 + 0     /* 10 */,
    128 + 0     /* 11 */,
    128 + 0     /* 12 */,
    128 + 0     /* 13 */,
    128 + 0     /* 14 */,
    128 + 0     /* 15 */,
    128 + 1     /* 16 */,
    128 + 2     /* 17 */,
    128 + 2     /* 18 */,
    128 + 2     /* 19 */,
    128 + 3     /* 20 */,
    128 + 3     /* 21 */,
    128 + 3     /* 22 */,
    128 + 3     /* 23 */,
    128 + 3     /* 24 */,
    128 + 4     /* 25 */,
    128 + 5     /* 26 */,
    128 + 5     /* 27 */,
    128 + 6     /* 28 */,
    128 + 7     /* 29 */,
    128 + 8     /* 30 */,
    128 + 9     /* 31 */,
    128 + 10    /* 32 */,
    128 + 11    /* 33 */,
    128 + 12    /* 34 */,
    128 + 13    /* 35 */,
    128 + 14    /* 36 */,
    128 + 15    /* 37 */,
    128 + 16    /* 38 */,
    128 + 18    /* 39 */,
    128 + 20    /* 40+ */
};


/*
 * Stat Table (DEX) -- bonus to hit (plus 128)
 */
byte adj_dex_th[] =
{
    128 + -3        /* 3 */,
    128 + -2        /* 4 */,
    128 + -2        /* 5 */,
    128 + -1        /* 6 */,
    128 + -1        /* 7 */,
    128 + 0 /* 8 */,
    128 + 0 /* 9 */,
    128 + 0 /* 10 */,
    128 + 0 /* 11 */,
    128 + 0 /* 12 */,
    128 + 0 /* 13 */,
    128 + 0 /* 14 */,
    128 + 0 /* 15 */,
    128 + 1 /* 16 */,
    128 + 2 /* 17 */,
    128 + 3 /* 18 */,
    128 + 3 /* 19 */,
    128 + 3 /* 20 */,
    128 + 3 /* 21 */,
    128 + 3 /* 22 */,
    128 + 4 /* 23 */,
    128 + 4 /* 24 */,
    128 + 4 /* 25 */,
    128 + 4 /* 26 */,
    128 + 5 /* 27 */,
    128 + 6 /* 28 */,
    128 + 7 /* 29 */,
    128 + 8 /* 30 */,
    128 + 9 /* 31 */,
    128 + 9 /* 32 */,
    128 + 10        /* 33 */,
    128 + 11        /* 34 */,
    128 + 12        /* 35 */,
    128 + 13        /* 36 */,
    128 + 14        /* 37 */,
    128 + 15        /* 38 */,
    128 + 15        /* 39 */,
    128 + 16        /* 40+ */
};


/*
 * Stat Table (STR) -- bonus to hit (plus 128)
 */
byte adj_str_th[] =
{
    128 + -3        /* 3 */,
    128 + -2        /* 4 */,
    128 + -1        /* 5 */,
    128 + -1        /* 6 */,
    128 + 0 /* 7 */,
    128 + 0 /* 8 */,
    128 + 0 /* 9 */,
    128 + 0 /* 10 */,
    128 + 0 /* 11 */,
    128 + 0 /* 12 */,
    128 + 0 /* 13 */,
    128 + 0 /* 14 */,
    128 + 0 /* 15 */,
    128 + 0 /* 16 */,
    128 + 0 /* 17 */,
    128 + 1 /* 18 */,
    128 + 1 /* 19 */,
    128 + 1 /* 20 */,
    128 + 1 /* 21 */,
    128 + 1 /* 22 */,
    128 + 1 /* 23 */,
    128 + 1 /* 24 */,
    128 + 2 /* 25 */,
    128 + 3 /* 26 */,
    128 + 4 /* 27 */,
    128 + 5 /* 28 */,
    128 + 6 /* 29 */,
    128 + 7 /* 30 */,
    128 + 8 /* 31 */,
    128 + 9 /* 32 */,
    128 + 10        /* 33 */,
    128 + 11        /* 34 */,
    128 + 12        /* 35 */,
    128 + 13        /* 36 */,
    128 + 14        /* 37 */,
    128 + 15        /* 38 */,
    128 + 15        /* 39 */,
    128 + 16        /* 40+ */
};


/*
 * Stat Table (STR) -- weight limit in deca-pounds
 */
byte adj_str_wgt[] =
{
    10      /* 3 */,
    11      /* 4 */,
    12      /* 5 */,
    13      /* 6 */,
    14      /* 7 */,
    15      /* 8 */,
    16      /* 9 */,
    17      /* 10 */,
    18      /* 11 */,
    19      /* 12 */,
    20      /* 13 */,
    21      /* 14 */,
    22      /* 15 */,
    23      /* 16 */,
    24      /* 17 */,
    25      /* 18 */,
    26      /* 19 */,
    27      /* 20 */,
    28      /* 21 */,
    29      /* 22 */,
    30      /* 23 */,
    31      /* 24 */,
    31      /* 25 */,
    32      /* 26 */,
    32      /* 27 */,
    33      /* 28 */,
    33      /* 29 */,
    34      /* 30 */,
    34      /* 31 */,
    35      /* 32 */,
    35      /* 33 */,
    36      /* 34 */,
    36      /* 35 */,
    37      /* 36 */,
    37      /* 37 */,
    38      /* 38 */,
    38      /* 39 */,
    39      /* 40+ */
};


/*
 * Stat Table (STR) -- weapon weight limit in pounds
 */
byte adj_str_hold[] =
{
    4       /* 3 */,
    5       /* 4 */,
    6       /* 5 */,
    7       /* 6 */,
    8       /* 7 */,
    9       /* 8 */,
    10      /* 9 */,
    11      /* 10 */,
    12      /* 11 */,
    13      /* 12 */,
    14      /* 13 */,
    15      /* 14 */,
    16      /* 15 */,
    17      /* 16 */,
    18      /* 17 */,
    19      /* 18 */,
    20      /* 19 */,
    21      /* 20 */,
    22      /* 21 */,
    23      /* 22 */,
    24      /* 23 */,
    25      /* 24 */,
    26      /* 25 */,
    27      /* 26 */,
    28      /* 27 */,
    30      /* 28 */,
    31      /* 29 */,
    32      /* 30 */,
    33      /* 31 */,
    34      /* 32 */,
    35      /* 33 */,
    37      /* 34 */,
    40      /* 35 */,
    44      /* 36 */,
    48      /* 37 */,
    50     /* 38 */,
    50     /* 39 */,
    50     /* 40+ */
};


/*
 * Stat Table (STR) -- digging value
 */
byte adj_str_dig[] =
{
    0       /* 3 */,
    0       /* 4 */,
    1       /* 5 */,
    2       /* 6 */,
    3       /* 7 */,
    4       /* 8 */,
    4       /* 9 */,
    5       /* 10 */,
    5       /* 11 */,
    6       /* 12 */,
    6       /* 13 */,
    7       /* 14 */,
    7       /* 15 */,
    8       /* 16 */,
    8       /* 17 */,
    9       /* 18 */,
    10      /* 19 */,
    12      /* 20 */,
    15      /* 21 */,
    20      /* 22 */,
    25      /* 23 */,
    30      /* 24 */,
    35      /* 25 */,
    40      /* 26 */,
    45      /* 27 */,
    50      /* 28 */,
    55      /* 29 */,
    60      /* 30 */,
    65      /* 31 */,
    70      /* 32 */,
    75      /* 33 */,
    80      /* 34 */,
    85      /* 35 */,
    90      /* 36 */,
    95      /* 37 */,
    100     /* 38 */,
    100     /* 39 */,
    100     /* 40+ */
};


/*
 * Stat Table (STR) -- help index into the "blow" table
 */
byte adj_str_blow[] =
{
    3       /* 3 */,
    4       /* 4 */,
    5       /* 5 */,
    6       /* 6 */,
    7       /* 7 */,
    8       /* 8 */,
    9       /* 9 */,
    10      /* 10 */,
    11      /* 11 */,
    12      /* 12 */,
    13      /* 13 */,
    14      /* 14 */,
    15      /* 15 */,
    16      /* 16 */,
    17      /* 17 */,
    20 /* 18 */,
    30 /* 19 */,
    40 /* 20 */,
    50 /* 21 */,
    60 /* 22 */,
    70 /* 23 */,
    80 /* 24 */,
    90 /* 25 */,
    100 /* 26 */,
    110 /* 27 */,
    120 /* 28 */,
    130 /* 29 */,
    140 /* 30 */,
    150 /* 31 */,
    160 /* 32 */,
    170 /* 33 */,
    180 /* 34 */,
    190 /* 35 */,
    200 /* 36 */,
    210 /* 37 */,
    220 /* 38 */,
    230 /* 39 */,
    240 /* 40+ */
};


/*
 * Stat Table (DEX) -- index into the "blow" table
 */
byte adj_dex_blow[] =
{
    0       /* 3 */,
    0       /* 4 */,
    0       /* 5 */,
    0       /* 6 */,
    0       /* 7 */,
    0       /* 8 */,
    0       /* 9 */,
    1       /* 10 */,
    1       /* 11 */,
    1       /* 12 */,
    1       /* 13 */,
    1       /* 14 */,
    2       /* 15 */,
    2       /* 16 */,
    2       /* 17 */,
    2       /* 18 */,
    3       /* 19 */,
    3       /* 20 */,
    3       /* 21 */,
    4       /* 22 */,
    4       /* 23 */,
    5       /* 24 */,
    5       /* 25 */,
    6       /* 26 */,
    6       /* 27 */,
    7       /* 28 */,
    7       /* 29 */,
    8       /* 30 */,
    8       /* 31 */,
    9      /* 32 */,
    9      /* 33 */,
    10      /* 34 */,
    10      /* 35 */,
    11      /* 36 */,
    11      /* 37 */,
    12      /* 38 */,
    12      /* 39 */,
    13      /* 40+ */
};


/*
 * Stat Table (DEX) -- chance of avoiding "theft" and "falling"
 */
byte adj_dex_safe[] =
{
    0       /* 3 */,
    1       /* 4 */,
    2       /* 5 */,
    3       /* 6 */,
    4       /* 7 */,
    5       /* 8 */,
    5       /* 9 */,
    6       /* 10 */,
    6       /* 11 */,
    7       /* 12 */,
    7       /* 13 */,
    8       /* 14 */,
    8       /* 15 */,
    9       /* 16 */,
    9       /* 17 */,
    10      /* 18 */,
    10      /* 19 */,
    15      /* 20 */,
    15      /* 21 */,
    20      /* 22 */,
    25      /* 23 */,
    30      /* 24 */,
    35      /* 25 */,
    40      /* 26 */,
    45      /* 27 */,
    50      /* 28 */,
    60      /* 29 */,
    70      /* 30 */,
    80      /* 31 */,
    90      /* 32 */,
    100     /* 33 */,
    100     /* 34 */,
    100     /* 35 */,
    100     /* 36 */,
    100     /* 37 */,
    100     /* 38 */,
    100     /* 39 */,
    100     /* 40+ */
};


/*
 * Stat Table (CON) -- base regeneration rate
 */
byte adj_con_fix[] =
{
    0       /* 3 */,
    0       /* 4 */,
    0       /* 5 */,
    0       /* 6 */,
    0       /* 7 */,
    0       /* 8 */,
    0       /* 9 */,
    0       /* 10 */,
    0       /* 11 */,
    0       /* 12 */,
    0       /* 13 */,
    1       /* 14 */,
    1       /* 15 */,
    1       /* 16 */,
    1       /* 17 */,
    2       /* 18 */,
    2       /* 19 */,
    2       /* 20 */,
    2       /* 21 */,
    2       /* 22 */,
    3       /* 23 */,
    3       /* 24 */,
    3       /* 25 */,
    3       /* 26 */,
    3       /* 27 */,
    4       /* 28 */,
    4       /* 29 */,
    5       /* 30 */,
    6       /* 31 */,
    6       /* 32 */,
    7       /* 33 */,
    7       /* 34 */,
    8       /* 35 */,
    8       /* 36 */,
    8       /* 37 */,
    9       /* 38 */,
    9       /* 39 */,
    9       /* 40+ */
};


/*
 * Stat Table (CON): hp adjustment percentage
 */
byte adj_con_mhp[] =
{
     80 /* 3 */,
     84 /* 4 */,
     87 /* 5 */,
     90 /* 6 */,
     92 /* 7 */,
     94 /* 8 */,
     96 /* 9 */,
     97 /* 10 */,
     98 /* 11 */,
     99 /* 12 */,
    100 /* 13 */,
    101 /* 14 */,
    102 /* 15 */,
    103 /* 16 */,
    104 /* 17 */,
    105 /* 18 */,
    106 /* 19 */,
    107 /* 20 */,
    109 /* 21 */,
    111 /* 22 */,
    113 /* 23 */,
    115 /* 24 */,
    117 /* 25 */,
    119 /* 26 */,
    121 /* 27 */,
    124 /* 28 */,
    127 /* 29 */,
    130 /* 30 */,
    133 /* 31 */,
    136 /* 32 */,
    139 /* 33 */,
    142 /* 34 */,
    145 /* 35 */,
    148 /* 36 */,
    151 /* 37 */,
    154 /* 38 */,
    157 /* 39 */,
    160 /* 40+ */
};


/*
 * Stat Table (CHR) -- charm
 */
byte adj_chr_chm[] =
{
    0       /* 3 */,
    0       /* 4 */,
    1       /* 5 */,
    2       /* 6 */,
    3       /* 7 */,
    4       /* 8 */,
    4       /* 9 */,
    5       /* 10 */,
    5       /* 11 */,
    6       /* 12 */,
    6       /* 13 */,
    7       /* 14 */,
    7       /* 15 */,
    8       /* 16 */,
    8       /* 17 */,
    9       /* 18 */,
    10      /* 19 */,
    12      /* 20 */,
    15      /* 21 */,
    18      /* 22 */,
    21      /* 23 */,
    24      /* 24 */,
    28      /* 25 */,
    32      /* 26 */,
    36      /* 27 */,
    39      /* 28 */,
    42      /* 29 */,
    45      /* 30 */,
    49      /* 31 */,
    53      /* 32 */,
    57      /* 33 */,
    61      /* 34 */,
    65      /* 35 */,
    69      /* 36 */,
    73      /* 37 */,
    77      /* 38 */,
    81      /* 39 */,
    85      /* 40+ */
};

/* For devices, the sval is actually the effect you want, and it needs
 * to be present in the corresponding device effect table in devices.c */
arena_type arena_info[MAX_ARENA_MONS + 2] =
{
    { MON_NOBORTA,       0,         0                             },
    { MON_MORI_TROLL,    0,         0                             },
    { MON_TIGER,         0,         0                             },
    { MON_DRUID,         0,         0                             },
    { MON_HILL_GIANT,    TV_POTION, SV_POTION_SPEED               },
    { MON_WERERAT,       0,         0                             },
    { MON_ORC_CAPTAIN,   0,         0                             },
    { MON_BERSERKER,     TV_ROD,    EFFECT_DETECT_TRAPS           },
    { MON_STONE_GIANT,   0,         0                             },
    { MON_D_ELF_LORD,    TV_POTION, SV_POTION_HEALING             },
    { MON_IRON_GOLEM,    0,         0                             },
    { MON_C_CRAWLER,     0,         0                             },
    { MON_XICLOTLAN,     0,         0                             },
    { MON_TROLL_PRIEST,  0,         0                             },
    { MON_ANGEL,         TV_ROD,    EFFECT_DETECT_MONSTERS        },
    { MON_SPIRIT_NAGA,   0,         0                             },
    { MON_BASILISK,      0,         0                             },
    { MON_MITHRIL_GOLEM, TV_WAND,   EFFECT_BALL_ACID              },
    { MON_SHADOW_DRAKE,  TV_WAND,   EFFECT_CONFUSE_MONSTER        },
    { MON_OGRE_SHAMAN,   TV_SCROLL, SV_SCROLL_ACQUIREMENT         },
    { MON_BICLOPS,       TV_POTION, SV_POTION_SELF_KNOWLEDGE      },
    { MON_ETHER_DRAKE,   TV_SCROLL, SV_SCROLL_RUNE_OF_PROTECTION  },
    { MON_HALFLING_S,    0,         0                             },
    { MON_ELDER_THING,   TV_STAFF,  EFFECT_HEAL                   },
    { MON_M_MH_DRAGON,   0,         0                             },
    { MON_ETTIN,         TV_SCROLL, SV_SCROLL_STAR_ENCHANT_WEAPON },
    { MON_VAMPIRE_LORD,  TV_ROD,    EFFECT_DETECT_ALL             },
    { MON_BARNEY,        TV_SCROLL, SV_SCROLL_GENOCIDE            },
    { MON_GROO,          TV_SCROLL, SV_SCROLL_MUNDANITY           },
    { MON_D_ELF_SORC,    TV_WAND,   EFFECT_METEOR                 },
    { MON_IRON_LICH,     TV_POTION, SV_POTION_NEW_LIFE            },
    { MON_G_TITAN,       TV_POTION, SV_POTION_STAR_ENLIGHTENMENT  },
    { MON_SPEC_TYRANNO,  TV_ROD,    EFFECT_ENLIGHTENMENT          },
    { MON_MUMMY_KING,    TV_ROD,    EFFECT_BEAM_DISINTEGRATE      },
    { MON_NIGHTWALKER,   TV_STAFF,  EFFECT_SPEED                  },
    { MON_GREAT_UNCLE,   TV_POTION, SV_POTION_STAR_HEALING        },
    { MON_SHAMBLER,      TV_SCROLL, SV_SCROLL_STAR_ACQUIREMENT    },
    { MON_BLACK_REAVER,  TV_SCROLL, SV_SCROLL_STAR_DESTRUCTION    },
    { MON_FENGHUANG,     TV_STAFF,  EFFECT_STARBURST              },
    { MON_G_BALROG,      TV_ROD,    EFFECT_HEAL_CURING_HERO       },
    { MON_WYRM_POWER,    TV_STAFF,  EFFECT_GENOCIDE               },
    { 0,                 0,         0                             }, /* Victory prizing */
    { MON_HAGURE,        TV_SCROLL, SV_SCROLL_ARTIFACT            },
};

/*
 * This table allows quick conversion from "speed" to "energy"
 * The basic function WAS ((S>=110) ? (S-110) : (100 / (120-S)))
 * Note that table access is *much* quicker than computation.
 *
 * Note that the table has been changed at high speeds. From
 * "Slow (-40)" to "Fast (+30)" is pretty much unchanged, but
 * at speeds above "Fast (+30)", one approaches an asymptotic
 * effective limit of 50 energy per turn. This means that it
 * is relatively easy to reach "Fast (+30)" and get about 40
 * energy per turn, but then speed becomes very "expensive",
 * and you must get all the way to "Fast (+50)" to reach the
 * point of getting 45 energy per turn. After that point,
 * further increases in speed are more or less pointless,
 * except to balance out heavy inventory.
 *                                             v---- Liar!! There are +80 speed monsters! :D
 * Note that currently the fastest monster is "Fast (+30)".
 *
 * It should be possible to lower the energy threshold from
 * 100 units to 50 units, though this may interact badly with
 * the (compiled out) small random energy boost code. It may
 * also tend to cause more "clumping" at high speeds.
 */
byte extract_energy[200] =
{
    /* Slow */     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* Slow */     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* Slow */     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* Slow */     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* Slow */     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* Slow */     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* S-50 */     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* S-40 */     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* S-30 */     2,  2,  2,  2,  2,  2,  2,  3,  3,  3,
    /* S-20 */     3,  3,  3,  3,  3,  4,  4,  4,  4,  4,
    /* S-10 */     5,  5,  5,  5,  6,  6,  7,  7,  8,  9,
    /* Norm */    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    /* F+10 */    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    /* F+20 */    30, 31, 32, 33, 34, 35, 36, 36, 37, 37,
    /* F+30 */    38, 38, 39, 39, 40, 40, 40, 41, 41, 41,
    /* F+40 */    42, 42, 42, 43, 43, 43, 44, 44, 44, 44,
    /* F+50 */    45, 45, 45, 45, 45, 46, 46, 46, 46, 46,
    /* F+60 */    47, 47, 47, 47, 47, 48, 48, 48, 48, 48,
    /* F+70 */    49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
    /* Fast */    49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
};




/*
 * Player Sexes
 *
 *      Title,
 *      Winner
 */
player_sex sex_info[MAX_SEXES] =
{
    {
        "Female",
        "Queen"
    },
    {
        "Male",
        "King"
    }
};

/* Warlock Pacts */
player_pact pact_info[WARLOCK_MAX] =
{
    {
        "Undead",
        "LWVzs"
    },
    {
        "Dragon",
        "Dd"
    },
    {
        "Angel",
        "A"
    },
    {
        "Demon",
        "Uu"
    },
    {
        "Aberrations",
        "hp"
    },
};

magic_type technic_info[NUM_TECHNIC][32] =
{
    {
        /* Law */
        { REALM_LAW, 0, 1,  1,  10,   2}, /* Detect Money */
        { REALM_LAW, 1, 2,  1,  10,   2}, /* Detect Traps */
        { REALM_LAW, 2, 3,  2,  10,   4}, /* Satisfy Hunger */
        { REALM_LAW, 3, 4,  2,  20,   4}, /* Detect Objects */
        { REALM_LAW, 4, 6,  3,  20,   8}, /* Basic Trap */
        { REALM_LAW, 5, 8,  4,  20,   8}, /* Disarm Traps */
        { REALM_LAW, 6, 10, 5,  30,   12}, /* Identify */
        { REALM_LAW, 7, 12, 5,  30,   12}, /* Dig */

        { REALM_LAW, 8, 10,  5,  25,   20}, /* Detect Monsters */
        { REALM_LAW, 9, 12,  6,  30,   35}, /* Slow Monster */
        { REALM_LAW, 10, 14,  6,  30,   25}, /* Confuse Monster */
        { REALM_LAW, 11, 15,  7,  30,   30}, /* Scare Monster */
        { REALM_LAW, 12, 25, 25,  50,   50}, /* Semicolon of Punishment */
        { REALM_LAW, 13, 27, 15,  50,   50}, /* Confuse Everybody */
        { REALM_LAW, 14, 35, 30,  60,   70}, /* Create Doors */
        { REALM_LAW, 15, 42, 50,  70,   80}, /* Semicolon of Warding */

        { REALM_LAW, 16, 12,  8,  25,   13}, /* Charm Monster */
        { REALM_LAW, 17, 18, 18,  30,   45}, /* Expert Trap */
        { REALM_LAW, 18, 20, 10,  35,   30}, /* Getaway */
        { REALM_LAW, 19, 30, 20,  40,   35}, /* Blame Undead */
        { REALM_LAW, 20, 32, 20,  50,   45}, /* Probe */
        { REALM_LAW, 21, 35, 25,  50,   60}, /* Spin */
        { REALM_LAW, 22, 40, 40,  60,   70}, /* Advanced Bloodsucking */
        { REALM_LAW, 23, 50, 125, 65,  200}, /* Alter Reality */

        { REALM_LAW, 24, 10,  10,  25,   15}, /* Blink */
        { REALM_LAW, 25, 15,  12,  30,   25}, /* Tread Softly */
        { REALM_LAW, 26, 20,  18,  35,   35}, /* Map Surroundings */
        { REALM_LAW, 27, 25,  25,  45,   45}, /* Dig Deep */
        { REALM_LAW, 28, 30,  30,  45,   55}, /* Unholy Rage */
        { REALM_LAW, 29, 35,  32,  55,   65}, /* Subpoena */
        { REALM_LAW, 30, 40,  35,  55,   75}, /* Teleport */
        { REALM_LAW, 31, 45,  60,  70,   85}  /* Dazzle */
    },

    {
        /* Music */
        { REALM_MUSIC, 0, 1,  1,  10,   2},
        { REALM_MUSIC, 1, 2,  1,  10,   2},
        { REALM_MUSIC, 2, 3,  2,  20,   3},
        { REALM_MUSIC, 3, 4,  2,  20,   4},
        { REALM_MUSIC, 4, 5,  2,  20,   6},
        { REALM_MUSIC, 5, 7,  4,  30,   8},
        { REALM_MUSIC, 6, 9,  3,  30,   10},
        { REALM_MUSIC, 7, 10, 2,  30,   12},

        { REALM_MUSIC, 8, 12,  3,   40,   20},
        { REALM_MUSIC, 9, 15, 16,  42,   35},
        { REALM_MUSIC, 10, 17, 18,  40,   25},
        { REALM_MUSIC, 11, 18,  2,  45,   30},
        { REALM_MUSIC, 12, 23,  8,  50,   38},
        { REALM_MUSIC, 13, 28, 30,  50,   41},
        { REALM_MUSIC, 14, 33, 35,  60,   42},
        { REALM_MUSIC, 15, 38, 35,  70,   46},

        { REALM_MUSIC, 16, 10,  4,  20,   13},
        { REALM_MUSIC, 17, 22,  5,  30,   26},
        { REALM_MUSIC, 18, 23,  3,  35,   27},
        { REALM_MUSIC, 19, 26,  28,  37,   29},
        { REALM_MUSIC, 20, 32,  37,  41,   36},
        { REALM_MUSIC, 21, 33,  22,  43,   40},
        { REALM_MUSIC, 22, 37,  35,  46,   42},
        { REALM_MUSIC, 23, 45,  60,  50,   56},

        { REALM_MUSIC, 24, 23,  18,  20,   23},
        { REALM_MUSIC, 25, 30,  30,  30,   26},
        { REALM_MUSIC, 26, 33,  65,  41,   30},
        { REALM_MUSIC, 27, 37,  35,  43,   35},
        { REALM_MUSIC, 28, 40,  30,  46,   50},
        { REALM_MUSIC, 29, 42,  75,  50,   68},
        { REALM_MUSIC, 30, 45,  58,  62,   73},
        { REALM_MUSIC, 31, 49,  48,  70,  200}
    },

    {
        /* Hissatsu */
        { REALM_HISSATSU, 0, 1,   15,   0,   0},
        { REALM_HISSATSU, 1, 3,   10,   0,   0},
        { REALM_HISSATSU, 2, 6,   15,   0,   0},
        { REALM_HISSATSU, 3, 9,    8,   0,   0},
        { REALM_HISSATSU, 4, 10,  12,   0,   0},
        { REALM_HISSATSU, 5, 12,  25,   0,   0},
        { REALM_HISSATSU, 6, 14,   7,   0,   0},
        { REALM_HISSATSU, 7, 17,  20,   0,   0},

        { REALM_HISSATSU, 8, 19,  10,   0,   0},
        { REALM_HISSATSU, 9, 22,  20,   0,   0},
        { REALM_HISSATSU, 10, 24,  30,   0,   0},
        { REALM_HISSATSU, 11, 25,  10,   0,   0},
        { REALM_HISSATSU, 12, 27,  15,   0,   0},
        { REALM_HISSATSU, 13, 29,  45,   0,   0},
        { REALM_HISSATSU, 14, 32,  70,   0,   0},
        { REALM_HISSATSU, 15, 35,  50,   0,   0},

        { REALM_HISSATSU, 16, 18,  40,   0,   0},
        { REALM_HISSATSU, 17, 22,  22,   0,   0},
        { REALM_HISSATSU, 18, 24,  30,   0,   0},
        { REALM_HISSATSU, 19, 26,  35,   0,   0},
        { REALM_HISSATSU, 20, 30,  30,   0,   0},
        { REALM_HISSATSU, 21, 32,  60,   0,   0},
        { REALM_HISSATSU, 22, 36,  40,   0,   0},
        { REALM_HISSATSU, 23, 39,  80,   0,   0},

        { REALM_HISSATSU, 24, 26,  20,   0,   0},
        { REALM_HISSATSU, 25, 29,  40,   0,   0},
        { REALM_HISSATSU, 26, 31,  35,   0,   0},
        { REALM_HISSATSU, 27, 36,  80,   0,   0},
        { REALM_HISSATSU, 28, 39, 100,   0,   0},
        { REALM_HISSATSU, 29, 42, 110,   0,   0},
        { REALM_HISSATSU, 30, 45, 130,   0,   0},
        { REALM_HISSATSU, 31, 50, 255,   0,   0}
    },

    {
        /* Hex */
        { REALM_HEX, 0,  1,  2, 20,   2},
        { REALM_HEX, 1,  1,  2, 20,   2},
        { REALM_HEX, 2,  3,  2, 30,   3},
        { REALM_HEX, 3,  5,  3, 30,   4},
        { REALM_HEX, 4,  7,  3, 40,   6},
        { REALM_HEX, 5,  8, 10, 60,   8},
        { REALM_HEX, 6,  9,  3, 30,  10},
        { REALM_HEX, 7, 10,  5, 40,  12},

        { REALM_HEX, 8, 12,  8, 40,  15},
        { REALM_HEX, 9, 12,  9, 35,  15},
        { REALM_HEX, 10, 15, 10, 50,  20},
        { REALM_HEX, 11, 20, 12, 45,  35},
        { REALM_HEX, 12, 25, 15, 50,  50},
        { REALM_HEX, 13, 30, 12, 60,  70},
        { REALM_HEX, 14, 35, 10, 60,  80},
        { REALM_HEX, 15, 40, 16, 70, 100},

        { REALM_HEX, 16, 15,  8, 20,  20},
        { REALM_HEX, 17, 18, 15, 50,  20},
        { REALM_HEX, 18, 22, 10, 65,  35},
        { REALM_HEX, 19, 25, 28, 70,  50},
        { REALM_HEX, 20, 28, 10, 70,  60},
        { REALM_HEX, 21, 30, 20, 60,  60},
        { REALM_HEX, 22, 36, 22, 70,  80},
        { REALM_HEX, 23, 40, 28, 70, 100},

        { REALM_HEX, 24,  5,  6, 35,   5},
        { REALM_HEX, 25, 22, 24, 70,  40},
        { REALM_HEX, 26, 25,  2, 65,  50},
        { REALM_HEX, 27, 32, 20, 50,  70},
        { REALM_HEX, 28, 35, 35, 70,  80},
        { REALM_HEX, 29, 38, 32, 70,  90},
        { REALM_HEX, 30, 42, 24, 70, 120},
        { REALM_HEX, 31, 46, 45, 80, 200}
    },

    {
        /* Unused */
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},

        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},

        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},

        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0},
        { REALM_RAGE, 0, 0,   0,   0,   0}
    },

    {
        /* Burglary */
        { REALM_BURGLARY,   0,  1,  1, 30,   2},
        { REALM_BURGLARY,   1,  3,  2, 30,   2},
        { REALM_BURGLARY,   2,  5,  3, 40,   3},
        { REALM_BURGLARY,   3,  7,  3, 40,   4},
        { REALM_BURGLARY,   4,  9,  3, 40,   6},
        { REALM_BURGLARY,   5, 11, 10, 45,   8},
        { REALM_BURGLARY,   6, 13, 10, 45,  10},
        { REALM_BURGLARY,   7, 15, 15, 50,  12},

        { REALM_BURGLARY,   8, 17, 15, 50,  15},
        { REALM_BURGLARY,   9, 20, 20, 60,  15},
        { REALM_BURGLARY,  10, 22, 20, 60,  20},
        { REALM_BURGLARY,  11, 23, 25, 60,  35},
        { REALM_BURGLARY,  12, 25, 25, 40,  50},
        { REALM_BURGLARY,  13, 30, 15, 40,  70},
        { REALM_BURGLARY,  14, 32, 30, 60,  80},
        { REALM_BURGLARY,  15, 33, 25, 50, 100},

        { REALM_BURGLARY,  16, 10, 20, 60,  20},
        { REALM_BURGLARY,  17, 15, 10, 60,  20},
        { REALM_BURGLARY,  18, 17, 10, 60,  35},
        { REALM_BURGLARY,  19, 25, 20, 60,  40},
        { REALM_BURGLARY,  20, 27, 30, 70,  50},
        { REALM_BURGLARY,  21, 30, 30, 70,  60},
        { REALM_BURGLARY,  22, 32, 35, 70,  60},
        { REALM_BURGLARY,  23, 40, 40, 70,  80},

        { REALM_BURGLARY,  24,  5,  7, 50,   5},
        { REALM_BURGLARY,  25, 12,  9, 60,  40},
        { REALM_BURGLARY,  26, 35, 20, 70,  50},
        { REALM_BURGLARY,  27, 39, 30, 70,  70},
        { REALM_BURGLARY,  28, 42, 30, 70,  80},
        { REALM_BURGLARY,  29, 45, 50, 85,  90},
        { REALM_BURGLARY,  30, 47,100, 90, 120},
        { REALM_BURGLARY,  31, 50,100, 95, 200}
    },

};


/*
 * Zangband uses this array instead of the spell flags table, as there
 * are 5 realms of magic, each with 4 spellbooks and 8 spells per book -- TY
 */
u32b fake_spell_flags[4]=
{
    0x000000ff,
    0x0000ff00,
    0x00ff0000,
    0xff000000
};


s32b realm_choices1[MAX_CLASS] =
{
    (CH_NONE),                /* Warrior */
    (CH_LIFE | CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW),  /* Mage */
    (CH_LIFE | CH_DEATH | CH_DAEMON |
     CH_CRUSADE),                           /* Priest */
    (CH_SORCERY | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_LAW | CH_BURGLARY), /* Rogue */
    (CH_NATURE),                            /* Ranger */
    (CH_CRUSADE | CH_DEATH | CH_LIFE | CH_DAEMON | CH_LAW),/* Paladin */
    (CH_ARCANE),                            /* Warrior-Mage */
    (CH_CHAOS | CH_DAEMON),                 /* Chaos-Warrior */
    (CH_LIFE | CH_NATURE | CH_DEATH |
     CH_ENCHANT | CH_TRUMP),                /* Monk */
    (CH_NONE),                              /* Mindcrafter */
    (CH_LIFE | CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_HEX | CH_ARMAGEDDON | CH_LAW),  /* High-Mage */
    (CH_ARCANE),                            /* Tourist */
    (CH_NONE),                              /* Imitator */
    (CH_TRUMP),                             /* Beastmaster */
    (CH_NONE),                              /* Sorcerer */
    (CH_NONE),                              /* Archer */
    (CH_NONE),                              /* Magic eater */
    (CH_MUSIC),                             /* Bard */
    (CH_NONE),                              /* Red Mage */
    (CH_HISSATSU),                          /* Samurai */
    (CH_LIFE | CH_NATURE | CH_DEATH |
     CH_ENCHANT | CH_CRUSADE),                 /* ForceTrainer */
    (CH_NONE),                              /* Blue Mage */
    (CH_NONE),                /* Cavalry */
    (CH_NONE),                /* Berserker */
    (CH_NONE),                /* Weaponsmith */
    (CH_NONE),                /* Mirror-master */
    (CH_NONE),                /* Ninja */
    (CH_NONE),                /* Sniper */
    (CH_NONE),                /* Time Lord */
    (CH_NONE),                /* Blood Knight */
    (CH_NONE),                /* Warlock */
    (CH_NONE),                /* Archaeologist */
    (CH_NONE),                /* Duelist */
    (CH_NONE),                /* Wild-Talent */
    (CH_NONE),                /* Rune-Knight */
    (CH_NONE),                /* Weaponmaster */
    (CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW), /* Blood-Mage */
    CH_NECROMANCY,            /* Necromancer */
    CH_NONE,                /* Psion */
    CH_RAGE,                /* Rage-Mage */
    CH_NONE,                /* Scout */
    CH_NONE,                /* Mauler */
    CH_NONE,                /* Monster */
    CH_NONE,                /* Mystic */
    CH_NONE,                /* Devicemaster */
    (CH_LIFE | CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW),  /* Yellow-Mage */
    CH_NONE,                /* Gray-Mage */
    CH_NONE,                /* Skillmaster */
    CH_LAW,                 /* Lawyer */
    CH_LAW,                 /* Ninja-Lawyer */
    CH_NONE,                /* Alchemist */
    CH_NONE,                /* Politician */
    CH_NONE,                /* Disciple */
	CH_NONE,                /* Elementalist */
};


s32b realm_choices2[MAX_CLASS] =
{
    (CH_NONE),                              /* Warrior */
    (CH_LIFE | CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW),  /* Mage */
    (CH_LIFE | CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW),  /* Priest */
    (CH_NONE),                              /* Rogue */
    (CH_SORCERY | CH_CHAOS | CH_DEATH |
     CH_TRUMP | CH_ARCANE | CH_DAEMON),     /* Ranger */
    (CH_NONE),                              /* Paladin */
    (CH_LIFE | CH_NATURE | CH_CHAOS |
     CH_DEATH | CH_TRUMP | CH_ARCANE |
     CH_SORCERY | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW),  /* Warrior-Mage */
    (CH_NONE),                              /* Chaos-Warrior */
    (CH_NONE),                              /* Monk */
    (CH_NONE),                              /* Mindcrafter */
    (CH_NONE),                              /* High-Mage */
    (CH_NONE),                              /* Tourist */
    (CH_NONE),                              /* Imitator */
    (CH_NONE),                              /* Beastmanster */
    (CH_NONE),                              /* Sorcerer */
    (CH_NONE),                              /* Archer */
    (CH_NONE),                              /* Magic eater */
    (CH_NONE),                              /* Bard */
    (CH_NONE),                              /* Red Mage */
    (CH_NONE),                              /* Samurai */
    (CH_NONE),                              /* ForceTrainer */
    (CH_NONE),                              /* Blue Mage */
    (CH_NONE),                /* Cavalry */
    (CH_NONE),                /* Berserker */
    (CH_NONE),                /* Weaponsmith */
    (CH_NONE),                /* Mirror-master */
    (CH_NONE),                /* Ninja */
    (CH_NONE),                /* Sniper */
    (CH_NONE),                /* Time Lord */
    (CH_NONE),                /* Blood Knight */
    (CH_NONE),                /* Warlock */
    (CH_NONE),                /* Archaeologist */
    (CH_NONE),                /* Duelist */
    (CH_NONE),                /* Wild-Talent */
    (CH_NONE),                /* Rune-Knight */
    (CH_NONE),                /* Weaponmaster */
    (CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW), /* Blood-Mage */
    CH_NONE,                /* Necromancer */
    CH_NONE,                /* Psion */
    CH_NONE,                /* Rage-Mage */
    CH_NONE,                /* Scout */
    CH_NONE,                /* Mauler */
    CH_NONE,                /* Monster */
    CH_NONE,                /* Mystic */
    CH_NONE,                /* Devicemaster */
    (CH_LIFE | CH_SORCERY | CH_NATURE |
     CH_CHAOS | CH_DEATH | CH_TRUMP |
     CH_ARCANE | CH_ENCHANT | CH_DAEMON |
     CH_CRUSADE | CH_ARMAGEDDON | CH_LAW),  /* Yellow-Mage */
    CH_NONE,                /* Gray-Mage */
    CH_NONE,                /* Skillmaster */
    CH_NONE,                /* Lawyer */
    CH_NONE,                /* Ninja-Lawyer */
    CH_NONE,                /* Alchemist */
    CH_NONE,                /* Politician */
    CH_NONE,                /* Disciple */
	CH_NONE,				/* Elementalist */
};



cptr realm_names[]
= {
    "none",
    "Life",
    "Sorcery",
    "Nature",
    "Chaos",
    "Death",
    "Trump",
    "Arcane",
    "Craft",
    "Daemon",
    "Crusade",
    "Necromancy",
    "Armageddon",
    "unknown",
    "unknown",
    "Law",
    "Music",
    "Kendo",
    "Hex",
    "Rage",
    "Burglary",
    "unknown"
};


/*
 * Each chest has a certain set of traps, determined by pval
 * Each chest has a "pval" from 1 to the chest level (max 55)
 * If the "pval" is negative then the trap has been disarmed
 * The "pval" of a chest determines the quality of its treasure
 * Note that disarming a trap on a chest also removes the lock.
 */
int chest_traps[64] =
{
    0,                                      /* 0 == empty */
    (CHEST_POISON),
    (CHEST_LOSE_STR),
    (CHEST_LOSE_CON),
    (CHEST_LOSE_STR),
    (CHEST_LOSE_CON),                       /* 5 == best small wooden */
    0,
    (CHEST_ALARM),
    (CHEST_ALARM),
    (CHEST_LOSE_STR),
    (CHEST_LOSE_CON),
    (CHEST_POISON),
    (CHEST_SCATTER),
    (CHEST_LOSE_STR | CHEST_LOSE_CON),
    (CHEST_LOSE_STR | CHEST_LOSE_CON),
    (CHEST_SUMMON),                 /* 15 == best large wooden */
    0,
    (CHEST_ALARM),
    (CHEST_SCATTER),
    (CHEST_PARALYZE),
    (CHEST_LOSE_STR | CHEST_LOSE_CON),
    (CHEST_SUMMON),
    (CHEST_PARALYZE),
    (CHEST_LOSE_STR),
    (CHEST_LOSE_CON),
    (CHEST_EXPLODE),                        /* 25 == best small iron */
    0,
    (CHEST_E_SUMMON),
    (CHEST_POISON | CHEST_LOSE_CON),
    (CHEST_LOSE_STR | CHEST_LOSE_CON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_BIRD_STORM),
    (CHEST_POISON | CHEST_SUMMON),
    (CHEST_E_SUMMON | CHEST_ALARM),
    (CHEST_EXPLODE),
    (CHEST_EXPLODE | CHEST_SUMMON), /* 35 == best large iron */
    0,
    (CHEST_SUMMON | CHEST_ALARM),
    (CHEST_EXPLODE),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_POISON | CHEST_PARALYZE),
    (CHEST_EXPLODE),
    (CHEST_BIRD_STORM),
    (CHEST_EXPLODE | CHEST_E_SUMMON | CHEST_ALARM),
    (CHEST_H_SUMMON),        /* 45 == best small steel */
    0,
    (CHEST_EXPLODE | CHEST_SUMMON | CHEST_ALARM),
    (CHEST_BIRD_STORM),
    (CHEST_RUNES_OF_EVIL),
    (CHEST_EXPLODE | CHEST_SUMMON | CHEST_ALARM),
    (CHEST_BIRD_STORM | CHEST_ALARM),
    (CHEST_H_SUMMON | CHEST_ALARM),
    (CHEST_RUNES_OF_EVIL),
    (CHEST_H_SUMMON | CHEST_SCATTER | CHEST_ALARM),
    (CHEST_RUNES_OF_EVIL | CHEST_EXPLODE),  /* 55 == best large steel */
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
    (CHEST_EXPLODE | CHEST_SUMMON),
};


/*
 * Hack -- the "basic" color names (see "TERM_xxx")
 */
cptr color_names[32] =
{
    "Dark",
    "White",
    "Slate",
    "Orange",
    "Red",
    "Green",
    "Blue",
    "Umber",
    "Light Dark",
    "Light Slate",
    "Violet",
    "Yellow",
    "Light Red",
    "Light Green",
    "Light Blue",
    "Light Umber",
    "Int. Green",
    "Pink",
    "Int. Blue",
    "Purple",
    "Teal",
    "Sky-Blue",
    "Mud",
    "Dark Yellow",
    "Turquoise",
    "Light Orange",
    "Lilac",
    "Dark Purple",
    "Dark Sky-Blue",
    "Pale Blue",
    "Dark Pink",
    "Chestnut"
};


/*
 * Abbreviations of healthy stats
 * Really? And what about not displaying for the character sheet? Sigh ...
 */
cptr stat_names[6] =
{
    "STR : ", "INT : ", "WIS : ", "DEX : ", "CON : ", "CHR : "

};

cptr stat_name_true[6] =
{
    "Strength", "Intelligence", "Wisdom", "Dexterity", "Constitution", "Charisma"
};

cptr stat_abbrev_true[6] =
{
    "Str", "Int", "Wis", "Dex", "Con", "Chr"
};


/*
 * Abbreviations of damaged stats
 */
cptr stat_names_reduced[6] =
{
    "Str : ", "Int : ", "Wis : ", "Dex : ", "Con : ", "Chr : "

};

cptr empty_lv_description[EMPTY_MAX] =
{
    "Sometimes", "Never", "Always"
};

/*
 * Certain "screens" always use the main screen, including News, Birth,
 * Dungeon, Tomb-stone, High-scores, Macros, Colors, Visuals, Options.
 *
 * Later, special flags may allow sub-windows to "steal" stuff from the
 * main window, including File dump (help), File dump (artifacts, uniques),
 * Character screen, Small scale map, Previous Messages, Store screen, etc.
 *
 * The "ctrl-i" (tab) command flips the "Display inven/equip" and "Display
 * equip/inven" flags for all windows.
 *
 * The "ctrl-g" command (or pseudo-command) should perhaps grab a snapshot
 * of the main screen into any interested windows.
 */
cptr window_flag_desc[32] =
{
    "Display inven/equip",

    "Display equip/inven",

    NULL,

    NULL,

    "Display object list",
    "Display monster list",
    "Display messages",

    "Display overhead view",

    "Display monster recall",

    "Display spell recall",

    "Display dungeon view",

    NULL,

    NULL,
    NULL,
//    "Display borg messages",
//    "Display borg status",
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};


/*
 * Available Options
 *
 * 0,* and 1,* space are now fully occupied
 */
option_type option_info[] =
{
    /*** Input Options ***/

    { &rogue_like_commands,         FALSE, OPT_PAGE_INPUT, 0, 0,
    "rogue_like_commands",          "Rogue-like commands" },

    { &always_pickup,               FALSE, OPT_PAGE_INPUT, 0, 5,
    "always_pickup",                "Pick things up by default" },

    { &online_macros,               FALSE, OPT_PAGE_INPUT, 1, 11,
    "online_macros",                "Substitute Walk for Run in macros" },

    { &quick_messages,              TRUE,  OPT_PAGE_INPUT, 0, 1,
    "quick_messages",               "Activate quick messages" },

    { &command_menu,                TRUE,  OPT_PAGE_INPUT, 2, 7,
    "command_menu",                 "Enable command selection menu" },

    { &use_old_target,              FALSE, OPT_PAGE_INPUT, 0, 4,
    "use_old_target",               "Use old target by default" },

    { &auto_target,                 FALSE, OPT_PAGE_INPUT, 0, 15,
    "auto_target",                  "Automatically target nearest monster" },

    { &always_repeat,               TRUE,  OPT_PAGE_INPUT, 0, 6,
    "always_repeat",                "Repeat obvious commands" },

    { &confirm_destroy,             FALSE, OPT_PAGE_INPUT, 5, 3,
    "confirm_destroy",              "Prompt for destruction of known worthless items" },

    { &confirm_wear,                TRUE,  OPT_PAGE_INPUT, 5, 4,
    "confirm_wear",                 "Confirm to wear/wield known cursed items" },

    { &target_pet,                  FALSE, OPT_PAGE_INPUT, 2, 5,
    "target_pet",                   "Allow targetting pets" },

#ifdef ALLOW_EASY_OPEN
    { &easy_open,                   TRUE,  OPT_PAGE_INPUT, 5, 7,
    "easy_open",                    "Automatically open doors" },
#endif /* ALLOW_EASY_OPEN */

#ifdef ALLOW_EASY_DISARM
    { &easy_disarm,                 TRUE,  OPT_PAGE_INPUT, 5, 8,
    "easy_disarm",                  "Automatically disarm traps" },
#endif /* ALLOW_EASY_DISARM */

    { &auto_detect_traps,           FALSE, OPT_PAGE_INPUT, 6, 9,
    "auto_detect_traps",            "Automatically detect traps while running" },

    { &auto_map_area,               FALSE, OPT_PAGE_INPUT, 6, 10,
    "auto_map_area",                "Automatically map area while running" },

    { &auto_get_ammo,               FALSE, OPT_PAGE_INPUT, 6, 5,
    "auto_get_ammo",                "Ctrl-G automatically gets nearby ammo" },

    { &auto_get_objects,            FALSE, OPT_PAGE_INPUT, 6, 7,
    "auto_get_objects",             "Ctrl-G automatically gets nearby wanted objects" },

    { &limit_shop_prompts,          TRUE, OPT_PAGE_INPUT, 6, 0,
    "limit_shop_prompts",           "Limit shop quantity prompts by money available" },

    { &numpad_as_cursorkey,         TRUE, OPT_PAGE_INPUT, 2, 31,
    "numpad_as_cursorkey",          "Use numpad keys as cursor keys in editor mode" },

    { &use_pack_slots,              TRUE,  OPT_PAGE_INPUT, 0, 3,
    "use_pack_slots",               "Retain backpack slot labels in other inventories" },
    /*** Map Screen Options ***/

    { &center_player,               FALSE, OPT_PAGE_MAPSCREEN, 5, 11,
    "center_player",                "Center map on player while walking"},

    { &center_running,              TRUE,  OPT_PAGE_MAPSCREEN, 5, 12,
    "center_running",               "Center map even while running" },

    { &view_yellow_lite,            TRUE,  OPT_PAGE_MAPSCREEN, 1, 28,
    "view_yellow_lite",             "Use special colors for torch-lit grids" },

    { &view_bright_lite,            TRUE,  OPT_PAGE_MAPSCREEN, 1, 29,
    "view_bright_lite",             "Use special colors for 'viewable' grids" },

    { &view_granite_lite,           TRUE,  OPT_PAGE_MAPSCREEN, 1, 30,
    "view_granite_lite",            "Use special colors for wall grids (slow)" },

    { &view_special_lite,           TRUE,  OPT_PAGE_MAPSCREEN, 1, 31,
    "view_special_lite",            "Use special colors for floor grids (slow)" },

    { &view_perma_grids,            TRUE,  OPT_PAGE_MAPSCREEN, 1, 6,
    "view_perma_grids",             "Map remembers all perma-lit grids" },

    { &view_torch_grids,            FALSE, OPT_PAGE_MAPSCREEN, 1, 7,
    "view_torch_grids",             "Map remembers all torch-lit grids" },

    { &view_unsafe_grids,           FALSE, OPT_PAGE_MAPSCREEN, 1, 8,
    "view_unsafe_grids",            "Map marked by detect traps" },

    { &fresh_before,                TRUE,  OPT_PAGE_MAPSCREEN, 1, 23,
    "fresh_before",                 "Fresh screen before automated input" },

    { &fresh_after,                 FALSE, OPT_PAGE_MAPSCREEN, 1, 24,
    "fresh_after",                  "Fresh screen after processing monsters" },

    { &fresh_message,               FALSE, OPT_PAGE_MAPSCREEN, 1, 25,
    "fresh_message",                "Fresh screen after every message" },

    { &hilite_player,               FALSE, OPT_PAGE_MAPSCREEN, 1, 27,
    "hilite_player",                "Hilite the player with the cursor" },

    { &display_path,                FALSE, OPT_PAGE_MAPSCREEN, 2, 8,
    "display_path",                 "Display actual path before shooting" },

    { &square_delays,               FALSE, OPT_PAGE_MAPSCREEN, 5, 23,
    "square_delays",                "Use square-based delay factors" },

    /*** Text Display Options ***/

    { &plain_descriptions,          TRUE,  OPT_PAGE_TEXT, 5, 1,
    "plain_descriptions",           "Plain object descriptions" },

    { &depth_in_feet,               FALSE, OPT_PAGE_TEXT, 0, 7,
    "depth_in_feet",                "Show dungeon level in feet" },

    { &effective_speed,             FALSE, OPT_PAGE_TEXT, 0, 29,
    "effective_speed",              "Show speeds as energy multipliers" },

    { &equippy_chars,               TRUE,  OPT_PAGE_TEXT, 1, 12,
    "equippy_chars",                "Display 'equippy' chars" },

    { &display_hp_bar,              FALSE,  OPT_PAGE_TEXT, 1, 1,
    "display_hp_bar",               "Display player hp bar" },

    { &display_sp_bar,              FALSE,  OPT_PAGE_TEXT, 1, 2,
    "display_sp_bar",               "Display player sp bar" },

    { &display_food_bar,            FALSE,  OPT_PAGE_TEXT, 1, 13,
    "display_food_bar",             "Display detailed food status" },

    { &angband_stats,               FALSE,  OPT_PAGE_TEXT, 2, 25,
    "angband_stats",                "Display stats Angband-style (18/XXX)" },

    { &percentage_life,             FALSE,  OPT_PAGE_TEXT, 4, 2,
    "percentage_life",              "Display life rating on a scale of 87% to 117%" },

    { &show_rogue_keys,		    FALSE, OPT_PAGE_TEXT, 6, 3,
    "show_rogue_keys",		    "Display roguelike movement keys as reminder" },

    { &show_energy_cost,            FALSE, OPT_PAGE_TEXT, 2, 9,
    "show_energy_cost",             "Display energy cost of most recent action" },

//    { &display_percentages,         FALSE,  OPT_PAGE_TEXT, 1, 10,
//    "display_percentages",          "Display percentages rather than status bars" },

    { &compress_savefile,           FALSE, OPT_PAGE_TEXT, 1, 26,
    "compress_savefile",            "Compress messages in savefiles" },

    { &abbrev_extra,                FALSE, OPT_PAGE_TEXT, 2, 10,
    "abbrev_extra",                 "Describe extra object attributes by abbreviation" },

    { &abbrev_all,                  FALSE, OPT_PAGE_TEXT, 2, 11,
    "abbrev_all",                   "Describe all object attributes by abbreviation" },

    { &mark_dragon,                 TRUE, OPT_PAGE_TEXT, 2, 4,
    "mark_dragon",                  "Extend use of '?' to random dragon resistances" },

    { &exp_need,                    FALSE, OPT_PAGE_TEXT, 2, 12,
    "exp_need",                     "Show the experience needed for next level" },

    { &ignore_unview,               FALSE, OPT_PAGE_TEXT, 2, 13,
    "ignore_unview",                "Ignore actions of out of sight monsters" },

    { &display_race,		        TRUE, OPT_PAGE_TEXT, 2, 29,
    "display_race",                 "Player character depends on race" },

    { &show_origins,		        TRUE, OPT_PAGE_TEXT, 2, 28,
    "show_origins",                 "Show object origins on examination" },

    { &show_discovery,		        FALSE, OPT_PAGE_TEXT, 2, 26,
    "show_discovery",                 "Show time of discovery on examination" },

    { &final_dump_origins,		TRUE, OPT_PAGE_TEXT, 2, 18,
    "final_dump_origins",             "Show origins in final character dumps" },

    { &always_dump_origins,		FALSE, OPT_PAGE_TEXT, 2, 19,
    "always_dump_origins",             "Show origins in all character dumps" },

    { &easy_mimics,		        FALSE, OPT_PAGE_TEXT, 2, 27,
    "easy_mimics",                  "Use 'x' for monsters that look like things" },

    { &display_skill_num,            FALSE,  OPT_PAGE_TEXT, 2, 22,
    "display_skill_num",            "Display skills as numbers on character sheet" },

    { &reforge_details,              TRUE,  OPT_PAGE_TEXT, 2, 16,
    "reforge_details",              "Show statistics before proceeding with a reforge" },

    { &show_damage_range,            FALSE, OPT_PAGE_TEXT, 4, 22,
    "show_damage_range",            "Show non-melee damages as ranges in monster info" },


    /*** Game-Play ***/

    { &stack_force_notes,           TRUE,  OPT_PAGE_GAMEPLAY, 0, 8,
    "stack_force_notes",            "Merge inscriptions when stacking" },

    { &stack_force_costs,           TRUE, OPT_PAGE_GAMEPLAY, 0, 9,
    "stack_force_costs",            "Merge discounts when stacking" },

    { &expand_list,                 TRUE,  OPT_PAGE_GAMEPLAY, 1, 5,
    "expand_list",                  "Allow query option lists to loop to beginning" },

    { &bound_walls_perm,            FALSE, OPT_PAGE_GAMEPLAY, 2, 1,
    "bound_walls_perm",             "Display dungeon boundaries as permanent walls" },

    { &last_words,                  TRUE,  OPT_PAGE_GAMEPLAY, 0, 28,
    "last_words",                   "Leave last words when your character dies" },

#ifdef ALLOW_WIZARD
    { &allow_debug_opts,            FALSE, OPT_PAGE_GAMEPLAY, 6, 11,
    "allow_debug_opts",             "Allow use of debug/cheat options" },
#endif

    /*** Disturbance ***/

    { &find_ignore_stairs,          FALSE, OPT_PAGE_DISTURBANCE, 0, 16,
    "find_ignore_stairs",           "Run past stairs" },

    { &find_ignore_doors,           TRUE,  OPT_PAGE_DISTURBANCE, 0, 17,
    "find_ignore_doors",            "Run through open doors" },

    { &find_ignore_veins,           TRUE,  OPT_PAGE_DISTURBANCE, 0, 19,
    "find_ignore_veins",            "Run past buried treasure" },

    { &find_cut,                    FALSE, OPT_PAGE_DISTURBANCE, 0, 18,
    "find_cut",                     "Run past known corners" },

    { &travel_ignore_items,         TRUE, OPT_PAGE_DISTURBANCE, 2, 30,
    "travel_ignore_items",          "Ignore identified items while travelling" },

    { &check_abort,                 TRUE,  OPT_PAGE_DISTURBANCE, 1, 18,
    "check_abort",                  "Check for user abort of automated input" },

    { &flush_failure,               TRUE,  OPT_PAGE_DISTURBANCE, 1, 20,
    "flush_failure",                "Flush input on various failures" },

    { &prompt_on_failure,           FALSE, OPT_PAGE_DISTURBANCE, 0, 26,
    "prompt_on_failure",            "Give -more- prompts on device/spell failures" },

    { &flush_disturb,               FALSE, OPT_PAGE_DISTURBANCE, 1, 21,
    "flush_disturb",                "Flush input whenever disturbed" },

    { &disturb_move,                FALSE, OPT_PAGE_DISTURBANCE, 0, 20,
    "disturb_move",                 "Disturb whenever any monster moves" },

    { &disturb_high,                FALSE, OPT_PAGE_DISTURBANCE, 1, 3,
    "disturb_high",                 "Disturb whenever high-level monster moves" },

    { &disturb_near,                TRUE,  OPT_PAGE_DISTURBANCE, 0, 21,
    "disturb_near",                 "Disturb whenever viewable monster moves" },

    { &disturb_pets,                FALSE, OPT_PAGE_DISTURBANCE, 5, 6,
    "disturb_pets",                 "Disturb when visible pets move" },

    { &disturb_panel,               TRUE,  OPT_PAGE_DISTURBANCE, 0, 22,
    "disturb_panel",                "Disturb whenever map panel changes" },

    { &disturb_state,               TRUE,  OPT_PAGE_DISTURBANCE, 0, 23,
    "disturb_state",                "Disturb whenever player state changes" },

    { &disturb_minor,               TRUE,  OPT_PAGE_DISTURBANCE, 0, 24,
    "disturb_minor",                "Disturb whenever boring things happen" },

    { &town_no_disturb,             FALSE, OPT_PAGE_DISTURBANCE, 1, 17,
    "town_no_disturb",              "Never disturb when a town monster moves" },

    { &ring_bell,                   FALSE, OPT_PAGE_DISTURBANCE, 0, 14,
    "ring_bell",                    "Audible bell (on errors, etc.)" },

    { &disturb_trap_detect,         TRUE,  OPT_PAGE_DISTURBANCE, 0, 27,
    "disturb_trap_detect",          "Disturb when leaving trap-detected area" },

    { &alert_trap_detect,           FALSE, OPT_PAGE_DISTURBANCE, 0, 25,
    "alert_trap_detect",            "Alert when leaving trap-detected area" },

    { &alert_device_gone,           TRUE,  OPT_PAGE_DISTURBANCE, 0, 13,
    "alert_device_gone",            "Alert when carried device is destroyed or stolen" },

    { &alert_insc_gone,             TRUE,  OPT_PAGE_DISTURBANCE, 0, 2,
    "alert_insc_gone",              "Alert when inscribed item is destroyed or stolen" },

    { &alert_wanted_kill,           FALSE,  OPT_PAGE_DISTURBANCE, 0, 30,
    "alert_wanted_kill",            "Alert on killing wanted uniques" },

    { &alert_poison,                TRUE,  OPT_PAGE_DISTURBANCE, 2, 2,
    "alert_poison",                 "Alert on dangerously high poison counter" },

    /*** Birth Options ***/

//	{ &coffee_break,		FALSE, OPT_PAGE_BIRTH, 6, 3,
//	"coffee_break",			"Coffee-break mode (accelerated game) (*)" },

	{ &easy_id,			FALSE, OPT_PAGE_BIRTH, 6, 31,
	"easy_id",			"Easy Identify (*)" },
	
	{ &easy_lore,			FALSE, OPT_PAGE_BIRTH, 6, 30,
	"easy_lore",			"Easy Lore" },

	{ &easy_damage,			FALSE, OPT_PAGE_BIRTH, 6, 16,
	"easy_damage",			"Easy monster health and damage information (*)" },

	{ &allow_spoilers,		FALSE, OPT_PAGE_BIRTH, 6, 29,
	"allow_spoilers",		"Allow spoilers" },
	
	{ &power_tele,			FALSE, OPT_PAGE_BIRTH, 6, 6,
	"power_tele",			"Use enhanced telepathy (*)" },

	{ &easy_thalos,			TRUE, OPT_PAGE_BIRTH, 6, 28,
	"easy_thalos",			"Allow easy teleportation to Thalos" },

	{ &never_forget,		FALSE, OPT_PAGE_BIRTH, 6, 27,
	"never_forget",			"Easy Labyrinth and no amnesia" },

	{ &empty_lore,			FALSE, OPT_PAGE_BIRTH, 1, 16,
	"empty_lore",			"Always forget item lore between games" },

    { &smart_learn,                 TRUE,  OPT_PAGE_BIRTH, 1, 14,
    "smart_learn",                  "Monsters learn from their mistakes (*)" },

    { &smart_cheat,                 FALSE, OPT_PAGE_BIRTH, 1, 15,
    "smart_cheat",                  "Monsters exploit player's weaknesses (*)" },

    { &no_wilderness,               FALSE, OPT_PAGE_BIRTH, 6, 1,
    "no_wilderness",                "Play without a wilderness" },

    { &ironman_shops,               FALSE, OPT_PAGE_BIRTH, 6, 2,
    "ironman_shops",                "Stores are permanently closed (*)" },

    { &ironman_downward,            FALSE, OPT_PAGE_BIRTH, 6, 4,
    "ironman_downward",             "Disable recall and use of up stairs (*)" },

    { &ironman_empty_levels,        FALSE, OPT_PAGE_BIRTH, 6, 8,
    "ironman_empty_levels",         "Generate empty 'arena' levels (*)" },

    { &ironman_nightmare,           FALSE, OPT_PAGE_BIRTH, 6, 18,
    "ironman_nightmare",            "Nightmare mode(it isn't even remotely fair!)(*)" },

    { &thrall_mode,                 FALSE, OPT_PAGE_BIRTH, 1, 19,
    "thrall_mode",                  "Thrall mode (start in extreme danger) (*)" },

    { &wacky_rooms,                 FALSE, OPT_PAGE_BIRTH, 1, 22,
    "wacky_rooms",                  "Always generate very unusual rooms" },

    { &melee_challenge,             FALSE, OPT_PAGE_BIRTH, 2, 21,
    "melee_challenge",              "Monsters can only be damaged in melee (*)" },

    { &no_melee_challenge,          FALSE, OPT_PAGE_BIRTH, 2, 23,
    "no_melee_challenge",           "Monsters cannot be damaged in melee (*)" },

    { &enable_virtues,              FALSE, OPT_PAGE_BIRTH, 6, 13,
    "enable_virtues",               "Enable the virtue system" },

    { &no_wanted_points,            TRUE, OPT_PAGE_BIRTH, 2, 24,
    "no_wanted_points",             "Deeper wanted uniques give better rewards" },

    { &preserve_mode,               TRUE,  OPT_PAGE_BIRTH, 6, 14,
    "preserve_mode",                "Preserve artifacts (*)" },

    { &allow_friendly_monster,      TRUE, OPT_PAGE_BIRTH, 6, 17,
    "allow_friendly_monster",       "Allow monsters friendly to player" },

    { &allow_hostile_monster,       TRUE, OPT_PAGE_BIRTH, 6, 19,
    "allow_hostile_monster",        "Allow monsters hostile to each other" },

    { &allow_pets,                  TRUE, OPT_PAGE_BIRTH, 6, 20,
    "allow_pets",                   "Allow player to summon/charm monsters" },

    { &quest_unique,                TRUE, OPT_PAGE_BIRTH, 6, 21,
    "quest_unique",                 "Random quests for unique monsters only" },

    { &random_artifacts,            FALSE, OPT_PAGE_BIRTH, 6, 23,
    "random_artifacts",             "Randomize standard artifacts" },

    { &no_artifacts,                FALSE, OPT_PAGE_BIRTH, 6, 24,
    "no_artifacts",                 "Never create artifacts (*)" },

    { &no_egos,                     FALSE, OPT_PAGE_BIRTH, 6, 25,
    "no_egos",                      "Never create non-jewelry ego items (*)" },

    { &no_selling,                  FALSE, OPT_PAGE_BIRTH, 6, 22,
    "no_selling",                   "Disable selling but increase gold drops" },

    { &reduce_uniques,              FALSE, OPT_PAGE_BIRTH, 6, 26,
    "reduce_uniques",               "Reduce the number of uniques (randomly)" },

    { &single_pantheon,             FALSE, OPT_PAGE_BIRTH, 5, 9,
    "single_pantheon",              "Active pantheons" },

    { &guaranteed_pantheon,         FALSE, OPT_PAGE_BIRTH, 2, 3,
    "guaranteed_pantheon",          "Guaranteed pantheon" },

    { &always_small_levels,         FALSE, OPT_PAGE_BIRTH, 4, 14,
    "always_small_levels",          "Level size" },

    { &increase_density,            FALSE, OPT_PAGE_BIRTH, 4, 15,
    "increase_density",             "Increase monster density on small levels" },
                          { &no_big_dungeons,             FALSE, OPT_PAGE_BIRTH, 4, 17,
    "no_big_dungeons",              "Allow small levels in all dungeons except Arena" },

    { &even_proportions,            FALSE, OPT_PAGE_BIRTH, 4, 16,
    "even_proportions",             "Sometimes reshape very flat \"pipe\" levels" },

    { &no_chris,			FALSE, OPT_PAGE_BIRTH, 6, 12,
    "no_chris",				"Nexus attacks never teleport to another dungeon" },

    { &no_scrambling,			FALSE, OPT_PAGE_BIRTH, 6, 15,
    "no_scrambling",			"Disallow stat scrambling from monster attacks" },

    { &comp_mode,			FALSE, OPT_PAGE_BIRTH, 5, 18,
    "comp_mode",			"Disallow permanent changes to player race" },

    /*** Easy Object Auto-Destroyer ***/

    { &destroy_items,               FALSE, OPT_PAGE_AUTODESTROY, 7, 0,
    "destroy_items",                "Use easy auto-destroyer" },

    { &destroy_feeling,             FALSE, OPT_PAGE_AUTODESTROY, 7, 8,
    "destroy_feeling",              "Apply auto-destroy on sensing an item" },

    { &destroy_identify,            FALSE, OPT_PAGE_AUTODESTROY, 7, 9,
    "destroy_identify",             "Apply auto-destroy on identifying an item" },

    { &destroy_debug,               FALSE, OPT_PAGE_AUTODESTROY, 7, 10,
    "destroy_debug",                "List the rule used when applying the Mogaminator" },

    { &no_mogaminator,              FALSE, OPT_PAGE_AUTODESTROY, 7, 11,
    "no_mogaminator",               "Never apply the Mogaminator" },

    { &leave_mogaminator,           FALSE, OPT_PAGE_AUTODESTROY, 7, 12,
    "leave_mogaminator",            "Leave items the Mogaminator wants to destroy" },

    { &check_full_pack,             TRUE, OPT_PAGE_AUTODESTROY, 2, 15,
    "check_full_pack",              "Limit pickup prompts if pack is already full" },

    { &delay_autopick,              FALSE, OPT_PAGE_AUTODESTROY, 1, 9,
    "delay_autopick",               "Allow manual pickup before applying auto-pickup" },

    { &leave_worth,                 TRUE,  OPT_PAGE_AUTODESTROY, 7, 2,
    "leave_worth",                  "Auto-destroyer leaves known worthy items" },

    { &leave_equip,                 FALSE, OPT_PAGE_AUTODESTROY, 7, 3,
    "leave_equip",                  "Auto-destroyer leaves weapons and armour" },

    { &leave_chest,                 TRUE,  OPT_PAGE_AUTODESTROY, 7, 7,
    "leave_chest",                  "Auto-destroyer leaves closed chests" },

    { &leave_wanted,                TRUE,  OPT_PAGE_AUTODESTROY, 7, 4,
    "leave_wanted",                 "Auto-destroyer leaves wanted corpses" },

    { &leave_corpse,                FALSE, OPT_PAGE_AUTODESTROY, 7, 5,
    "leave_corpse",                 "Auto-destroyer leaves corpses and skeletons" },

    { &leave_junk,                  FALSE, OPT_PAGE_AUTODESTROY, 7, 6,
    "leave_junk",                   "Auto-destroyer leaves junk" },

    { &leave_special,               TRUE,  OPT_PAGE_AUTODESTROY, 7, 1,
    "leave_special",                "Auto-destroyer leaves items your race/class needs" },

    /* List Options */

    { &describe_slots,              TRUE,  OPT_PAGE_LIST, 0, 10,
    "describe_slots",               "Show equipment slot descriptions" },

    { &show_weights,                TRUE,  OPT_PAGE_LIST, 0, 11,
    "show_weights",                 "Show weights in object listings" },

    { &show_discounts,              TRUE,  OPT_PAGE_LIST, 0, 12,
    "show_discounts",               "Show discounts in object listings" },

    { &show_power,                  FALSE, OPT_PAGE_LIST, 2, 14,
    "show_power",                   "Show device power in object listings (if known)" },

    { &show_item_graph,             TRUE,  OPT_PAGE_LIST, 2, 0,
    "show_item_graph",              "Show item graphics in object listings" },

    { &shops_mark_unseen,           FALSE, OPT_PAGE_LIST, 0, 31,
    "shops_mark_unseen",            "Indicate unknown flavors in shop inventories" },

    { &black_curses,                TRUE,  OPT_PAGE_LIST, 4, 23,
    "black_curses",                 "Show cursed items as black in unwield menu" },

    { &display_distance,            FALSE,  OPT_PAGE_LIST, 1, 4,
    "display_distance",             "Display distance in monster list" },

    { &list_stairs,                 FALSE,  OPT_PAGE_LIST, 1, 10,
    "list_stairs",                  "Display stairs in object list" },

    { &show_future_powers,          TRUE,  OPT_PAGE_LIST, 1, 0,
    "show_future_powers",           "Include known future race/class powers in lists" },

    { &show_future_spells,          FALSE,  OPT_PAGE_LIST, 2, 20,
    "show_future_spells",           "Include known future class spells in lists" },

    { &auto_sticky_labels,          TRUE,  OPT_PAGE_LIST, 2, 17,
    "auto_sticky_labels",           "Automatically make all power labels sticky" },

    { &obj_list_width,              TRUE,  OPT_PAGE_LIST, 4, 0,
    "object_list_width",            "Maximum width of the object list" },

    { &mon_list_width,              TRUE,  OPT_PAGE_LIST, 4, 1,
    "monster_list_width",           "Maximum width of the monster list" },

    /*** End of Table ***/

    { NULL,                         0, 0, 0, 0,
    NULL,                           NULL }
};


cptr chaos_patrons[MAX_PATRON] =
{
    "Slortar",
    "Mabelode",
    "Chardros",
    "Hionhurn",
    "Xiombarg",

    "Pyaray",
    "Balaan",
    "Arioch",
    "Eequor",
    "Narjhan",

    "Balo",
    "Khorne",
    "Slaanesh",
    "Nurgle",
    "Tzeentch",

    "Khaine",
    "Random",

    "Karrot",
    "Yeqrezh",
    "Troika"
};

cptr elementalist_realms[MAX_ELEMENT] =
{
    "Fire",
    "Ice",
    "Sky",
    "Sea",
    "Darkness",
    "Chaos",
    "Earth",
    "Death"
};

cptr lv_size_options[SMALL_LVL_MAX + 1] =
{
    "Normal",
    "Coffee-break (allow big)",
    "Coffee-break (never big)",
    "Medium",
    "Instant Coffee (allow big)",
    "Instant Coffee (never big)",
    "Small (classic)",
    "Small Coffee",
    "Very Small",
    "Extremely Small",
    "Tiny",
    "Huge",
    "Huge (respect tiny)"
};

martial_arts ma_blows[MAX_MA] =
{
    { "Punch",         "You punch.",                                 1, 0, 1, 4, 0 },
    { "Kick",          "You kick.",                                  2, 0, 1, 6, 0 },
    { "Strike",        "You strike.",                                3, 0, 1, 7, 0 },
    { "Knee",          "You knee.",                                  5, 5, 2, 3, MA_KNEE },
    { "Elbow",         "You elbow.",                                 7, 5, 1, 8, 0 },
    { "Butt",          "You <color:U>butt</color>.",                 9, 10, 2, 5, 0 },
    { "Kick",          "You <color:U>kick</color>.",                 11, 10, 3, 4, MA_SLOW },
    { "Uppercut",      "You <color:U>uppercut</color>.",             13, 12, 4, 4, 6 },
    { "Double Kick",   "You <color:y>double-kick</color>.",          16, 15, 5, 4, 8 },
    { "Cat's Claw",    "You land a <color:y>Cat's Claw</color>.",     20, 20, 5, 5, 0 },
    { "Jump Kick",     "You <color:y>jump kick</color>.",            25, 25, 5, 6, 10 },
    { "Eagle's Claw",  "You land an <color:o>Eagle's Claw</color>.", 29, 25, 6, 6, 0 },
    { "Circle Kick",   "You <color:o>circle kick</color>.",          33, 30, 6, 8, 10 },
    { "Iron Fist",     "You land an <color:R>Iron Fist</color>.",    37, 35, 8, 8, 10 },
    { "Flying Kick",   "You land a <color:R>flying kick</color>.",   41, 35, 8, 10, 12 },
    { "Dragon Fist",   "You land a <color:r>Dragon Fist</color>.",   45, 35, 10, 10, 16 },
    { "Crushing Blow", "You land a <color:v>Crushing Blow</color>.", 48, 35, 10, 12, 18 },
};

/*
 * Table of game-generated inscriptions (indexed by the defines in
 * defines.h). -- RG
 */
cptr game_inscriptions[] =
{
    NULL,            /* FEEL_NONE */
    "broken",        /* FEEL_BROKEN */
    "terrible",      /* FEEL_TERRIBLE */
    "awful",         /* FEEL_AWFUL */
    "cursed",        /* FEEL_CURSED */
    "enchanted",     /* FEEL_ENCHANTED */
    "average",       /* FEEL_AVERAGE */
    "good",          /* FEEL_GOOD */
    "excellent",     /* FEEL_EXCELLENT */
    "special",       /* FEEL_SPECIAL */
    "bad",           /* FEEL_BAD */
};

kamae kamae_shurui[MAX_KAMAE] =
{
    {"Genbu", 25, "(Black Tortoise) "},
    {"Byakko", 30, "(White Tiger) "},
    {"Seiryuu", 35, "(Blue Dragon) "},
    {"Suzaku", 40, "(Red Phoenix) "},
};

kamae kata_shurui[MAX_KATA] =
{
    {"Iai", 25, "Gives a single accuracy-boosted retaliatory blow against melee attacks, at the cost of increasing all damage taken by 20%. Any melee attack by you dispels this posture."},
    {"Huujin", 30, "Gives reflection unless blind, at the cost of -1.00 to blows."},
    {"Kouki", 35, "Gives +5 to all stats and +50 melee to-hit, at the cost of -50 AC, vulnerability to the base elements, and zero regeneration. Using most inventory items dispels this posture."},
    {"Musou", 40, "Gives full telepathy and ultimate resistance, halves most types of damage, and gives a full round of retaliatory blows against melee attacks, at the cost of 2 SP per turn. Almost any action other than retaliation dispels this posture."},
};

cptr exp_level_str[5]=
{"[Unskilled]", "[Beginner]", "[Skilled]", "[Expert]", "[Master]"};

/* Weird melee attack types when hallucinating */
cptr silly_attacks[MAX_SILLY_ATTACK] =
{
    "smothers",
    "hugs",
    "humiliates",
    "whips",
    "kisses",

    "disgusts",
    "pees all over",
    "passes the gas on",
    "makes obscene gestures at",
    "licks",

    "stomps on",
    "swallows",
    "drools on",
    "misses",
    "shrinks",

    "emasculates",
    "evaporates",
    "solidifies",
    "digitizes",
    "insta-kills",

    "massacres",
    "slaughters",
    "drugs",
    "psychoanalyzes",
    "deconstructs",

    "falsifies",
    "disbelieves",
    "molests",
    "pusupusu",
    "wipes",

    "bowdlerizes",
    "decomposes",
    "eats",
    "drinks",
    "disrobes",

    "minimizes",
    "sits on",
    "decompresses",
    "pulverises",
    "gropes",

    "dismembers",
    "sneezes at",
    "flays",
    "debones",
    "sucker-punches",

    "erases",
    "paws",
    "vomits on",
    "disembowels",
    "overwrites",

    "initializes",
    "headpats",
    "smacks",
    "bounces",
    "nerfs",

    "defenestrates",
    "deletes",
    "caresses",
    "petrifies",
    "nags at",

    "discombobulates",
    "violates",
    "bores",
    "sprays pink paint on",
    "autocorrects",
};


/*
 * The table of "symbol info" -- each entry is a string of the form
 * "X:desc" where "X" is the trigger, and "desc" is the "info".
 */
cptr ident_info[] =
{
    " :A dark grid",
    "!:A potion (or oil)",
    "\":An amulet (or necklace)",
    "#:A wall (or secret door) / a plant / a gas",
    "$:Treasure (gold or gems)",
    "%:A vein (magma or quartz)",
    "&:A chest",
    "':An open door",
    "(:Soft armor",
    "):A shield",
    "*:A vein with treasure or a ball monster",
    "+:A closed door",
    ",:Food (or mushroom patch)",
    "-:A wand (or rod)",
    ".:Floor",
    "/:A polearm (Axe/Pike/etc)",
    "0:Entrance to Museum",
    "1:Entrance to General Store",
    "2:Entrance to Armory",
    "3:Entrance to Weaponsmith",
    "4:Entrance to Temple",
    "5:Entrance to Alchemy shop",
    "6:Entrance to Magic store",
    "7:Entrance to Black Market",
    "8:Entrance to your home",
    "9:Entrance to the bookstore",
    "::Rubble",
    ";:A glyph of warding / an explosive rune",
    "<:An up staircase",
    "=:A ring",
    ">:A down staircase",
    "?:A scroll",
    "@:You",
    "A:Angel",
    "B:Bird",
    "C:Canine",
    "D:Ancient Dragon/Wyrm",
    "E:Elemental",
    "F:Dragon Fly",
    "G:Ghost",
    "H:Hybrid",
    "I:Insect",
    "J:Snake",
    "K:Killer Beetle",
    "L:Lich",
    "M:Multi-Headed Reptile",
    "N:Mystery Living",
    "O:Ogre",
    "P:Giant Humanoid",
    "Q:Quylthulg (Pulsing Flesh Mound)",
    "R:Reptile/Amphibian",
    "S:Spider/Scorpion/Tick",
    "T:Troll",
    "U:Major Demon",
    "V:Vampire",
    "W:Wight/Wraith/etc",
    "X:Xorn/Xaren/etc",
    "Y:Yeti",
    "Z:Zephyr Hound",
    "[:Hard armor",
    "\\:A hafted weapon (mace/whip/etc)",
    "]:Misc. armor",
    "^:A trap",
    "_:A staff",
    "`:A figurine or statue",
    "a:Ant",
    "b:Bat",
    "c:Centipede",
    "d:Dragon",
    "e:Floating Eye",
    "f:Feline/Fox",
    "g:Golem",
    "h:Hobbit/Elf/Dwarf",
    "i:Icky Thing",
    "j:Jelly",
    "k:Kobold",
    "l:Aquatic monster",
    "m:Mold",
    "n:Naga",
    "o:Orc",
    "p:Person/Human",
    "q:Quadruped",
    "r:Rodent",
    "s:Skeleton",
    "t:Townsperson",
    "u:Minor Demon",
    "v:Vortex",
    "w:Worm/Worm-Mass",
    /* "x:unused", */
    "y:Yeek",
    "z:Zombie/Mummy",
    "{:A missile (arrow/bolt/shot)",
    "|:An edged weapon (sword/dagger/etc)",
    "}:A launcher (bow/crossbow/sling)",
    "~:Fluid terrain (or miscellaneous item)",
    NULL
};

/*
 * The table of features' actions
 */
byte feature_action_flags[FF_FLAG_MAX] =
{
    0, /* LOS */
    0, /* PROJECT */
    0, /* MOVE */
    0, /* PLACE */
    0, /* DROP */
    0, /* SECRET */
    0, /* NOTICE */
    0, /* REMEMBER */
    0, /* OPEN */
    0, /* CLOSE */
    FAF_CRASH_GLASS, /* BASH */
    0, /* SPIKE */
    FAF_DESTROY, /* DISARM */
    0, /* STORE */
    FAF_DESTROY | FAF_CRASH_GLASS, /* TUNNEL */
    0, /* MAY_HAVE_GOLD */
    0, /* HAS_GOLD */
    0, /* HAS_ITEM */
    0, /* DOOR */
    0, /* TRAP */
    0, /* STAIRS */
    0, /* GLYPH */
    0, /* LESS */
    0, /* MORE */
    0, /* RUN */
    0, /* FLOOR */
    0, /* WALL */
    0, /* PERMANENT */
    0, /* INNER */
    0, /* OUTER */
    0, /* SOLID */
    0, /* HIT_TRAP */

    0, /* BRIDGE */
    0, /* RIVER */
    0, /* LAKE */
    0, /* BRIDGED */
    0, /* COVERED */
    0, /* GLOW */
    0, /* ENSECRET */
    0, /* WATER */
    0, /* LAVA */
    0, /* SHALLOW */
    0, /* DEEP */
    0, /* FILLED */
    FAF_DESTROY | FAF_CRASH_GLASS, /* HURT_ROCK */
    0, /* HURT_FIRE */
    0, /* HURT_COLD */
    0, /* HURT_ACID */
    0, /* ICE */
    0, /* ACID */
    0, /* OIL */
    0, /* XXX04 */
    0, /* CAN_CLIMB */
    0, /* CAN_FLY */
    0, /* CAN_SWIM */
    0, /* CAN_PASS */
    0, /* CAN_OOZE */
    0, /* CAN_DIG */
    0, /* HIDE_ITEM */
    0, /* HIDE_SNEAK */
    0, /* HIDE_SWIM */
    0, /* HIDE_DIG */
    0, /* KILL_HUGE */
    0, /* KILL_MOVE */

    0, /* PICK_TRAP */
    0, /* PICK_DOOR */
    0, /* ALLOC */
    0, /* CHEST */
    0, /* DROP_1D2 */
    0, /* DROP_2D2 */
    0, /* DROP_GOOD */
    0, /* DROP_GREAT */
    0, /* HURT_POIS */
    0, /* HURT_ELEC */
    0, /* HURT_WATER */
    0, /* HURT_BWATER */
    0, /* USE_FEAT */
    0, /* GET_FEAT */
    0, /* GROUND */
    0, /* OUTSIDE */
    0, /* EASY_HIDE */
    0, /* EASY_CLIMB */
    0, /* MUST_CLIMB */
    0, /* TREE */
    0, /* NEED_TREE */
    0, /* BLOOD */
    0, /* DUST */
    0, /* SLIME */
    0, /* PLANT */
    0, /* XXX2 */
    0, /* INSTANT */
    0, /* EXPLODE */
    0, /* TIMED */
    0, /* ERUPT */
    0, /* STRIKE */
    0, /* SPREAD */

    0, /* SPECIAL */
    FAF_DESTROY | FAF_NO_DROP | FAF_CRASH_GLASS, /* HURT_DISI */
    0, /* QUEST_ENTER */
    0, /* QUEST_EXIT */
    0, /* QUEST */
    0, /* SHAFT */
    0, /* MOUNTAIN */
    0, /* BLDG */
    0, /* MINOR_GLYPH */
    0, /* PATTERN */
    0, /* TOWN */
    0, /* ENTRANCE */
    0, /* MIRROR */
    0, /* UNPERM */
    0, /* TELEPORTABLE */
    0, /* CONVERT */
    0, /* GLASS */
    0, /* ROGUE_TRAP_1 */
    0, /* ROGUE_TRAP_2 */
    0, /* ROGUE_TRAP_3 */
    0, /* WEB */
    0, /* SEMI_PUN */
    0, /* SHADOW_ZAP */
};
