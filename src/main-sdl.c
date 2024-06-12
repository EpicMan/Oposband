/** \file main-sdl.c
    \brief Angband SDL port

 * Copyright (c) 2007 Ben Harrison, Gregory Velichansky, Eric Stevens,
 * Leon Marrick, Iain McFall, and others
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 *
 *
 * Comments and suggestions are welcome. The UI probably needs some
 * adjustment, and I need comments from you.
 *perhaps also something like "Angband 3.0.8 by Andrew Sidwell and others;
 SDL port by Iain McFall an others, please see the accompanying documentation
 for credits" or something
 *


 * This file helps Angband work with at least some computers capable of running
 * SDL, a set of simple and quite popular game development libraries that work
 * on many different operating systems, including Windows, most flavours of
 * UNIX or similar, and Mac OS X.  It requires a 32-bit (or higher) machine
 * capable of displaying at least 640x480 in 256 colors.  A Pentium or better
 * is strongly recommended (for speed reasons treated more fully below).
 *
 * To use this file, use an appropriate "Makefile" or "Project File", install
 * the required libraries (described below), make sure that "USE_SDL" is
 * defined somewhere, and obtain various extra files (described below).  If
 * you are new to all this, read "makefile.sdl".
 *
 * This port uses the following libraries:  SDL (v1.2+) and  SDL_ttf.
 * All are available as source code, pre-compiled libs for developers,
 * and libs (or dlls) for players from www.libsdl.org
 *
 *
 * Other files used by this port:
 * - The game must have a collection of bitmap .fon files in /lib/xtra/font.
 *
 * - If "USE_GRAPHICS" is defined, then it also needs some bitmapped (.bmp)
 *   graphics files in /lib/xtra/graf, such as "16x16.bmp" and "16x16m.bmp".
 *
 * - The "lib/pref/pref-sdl.prf" file contains keymaps, macro definitions,
 *   and/or color redefinitions.
 * - The "lib/pref/font-sdl.prf" contains attr/char mappings for use with the
 *   normal "*.fon" font files in the "lib/xtra/font/" directory.
 *
 *
 *
 * "Term" framework by Ben Harrison (benh@phial.com).
 *
 * Original Sangband SDL port and the "intrface" module by Leon Marrick
 * (www.runegold.org/sangband).
 *
 * Additional helpful ideas by:
 * 2001 Gregory Velichansky <hmaon@bumba.net>, creator of the first Angband SDL
 * port.
 * 2006 Eric Stevens <sdltome@gmail.com>, main author of the TOME SDL port.



 * Comments on using SDL with Angband:
 *
 * The good news:
 * - SDL is cross-platform.  Really.  No joke.  If this port doesn't work on
 *   your system, it probably isn't SDL's fault.
 * - SDL is relatively easy to use, allowing you you to cobble up feature-rich
 *   apps that look half-way decent without too much fuss and bother.  It's
 *   wonderful for prototyping.
 * - SDL does most of what *band developers are likely to need, plus a whole
 *   lot more we haven't realized we want yet.
 * - SDL is a cleanly written open-source API; it is much less painful to get
 *   the straight word then it is with most other libraries.  Also, the SDL
 *   community offers active discussion boards, solid documentation, and
 *   numerous code examples relating to almost any question you might have
 *   occasion to ask.
 *
 * The bad news:
 * - SDL can be tedious to install.  Each individual library is straightforward
 *   enough, but *band development work requires several, the number growing as
 *   you get more sophisticated.
 * - SDL (as a stand-alone lib, without the assistance of OpenGL) can be very
 *   sluggish if you aren't careful.  It is poor at detecting, let alone making
 *   fullest use of, available video hardware, which cripples speed.  So,
 *   getting half-way decent performance in a game like Angband takes some
 *   skill and vast amounts of effort.  Speed - the lack thereof - is this
 *   port's biggest problem.  More comments below.
 * - SDL is not a complete game development library (although the add-ons help
 *   tremendously).  Much-needed functionality - text output, blit stretching,
 *   and video refresh synching being three examples - were either missing
 *   altogether or covered by functions that proved too slow or delicate for
 *   production use.  I ended up having to spend at least as much time, and
 *   write at least as much low-level code, as I did using the Win32 API.
 * - SDL, like Allegro but to a lesser extent, is falling behind current tech-
 *   nology.  Development progresses, but obsolescence looms, especially on
 *   Windows machines.
 */
#include "angband.h"

#ifdef USE_SDL

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

/**
 * SDL flags used for the main window surface
 */
static Uint32 vflags = SDL_ANYFORMAT;

/**
 * Current screen dimensions
 */
static int screen_w = 800;
static int screen_h = 600;

/**
 * Fullscreen dimensions
 */
static int full_w;
static int full_h;

/**
 * Want fullscreen?
 */
static bool fullscreen = FALSE;

/* XXXXXXXXX */
static char *ANGBAND_DIR_USER_SDL;

/**
 * Used as 'system' font
 */
static cptr DEFAULT_FONT_FILE = "8x8xb.fon";

#define MAX_FONTS 20
char *FontList[MAX_FONTS];
static int num_fonts = 0;

/* Global defines etc from Angband 3.5-dev - NRM */
# include <sys/types.h>
# include <dirent.h>
#define ANGBAND_TERM_MAX 8
/**
* Specifies what kind of thing a file is, when writing. See file_open().
*/
typedef enum
{
        FTYPE_TEXT = 1,
        FTYPE_SAVE,
        FTYPE_RAW,
        FTYPE_HTML
} file_type;


#define MAX_COLORS 256
#define MSG_MAX SOUND_MAX

/**
* Keyset mappings for various keys.
*/
#define ARROW_DOWN 0x80
#define ARROW_LEFT 0x81
#define ARROW_RIGHT 0x82
#define ARROW_UP 0x83

#define KC_F1 0x84
#define KC_F2 0x85
#define KC_F3 0x86
#define KC_F4 0x87
#define KC_F5 0x88
#define KC_F6 0x89
#define KC_F7 0x8A
#define KC_F8 0x8B
#define KC_F9 0x8C
#define KC_F10 0x8D
#define KC_F11 0x8E
#define KC_F12 0x8F
#define KC_F13 0x90
#define KC_F14 0x91
#define KC_F15 0x92

#define KC_HELP 0x93
#define KC_HOME 0x94
#define KC_PGUP 0x95
#define KC_END 0x96
#define KC_PGDOWN 0x97
#define KC_INSERT 0x98
#define KC_PAUSE 0x99
#define KC_BREAK 0x9a
#define KC_BEGIN 0x9b
#define KC_ENTER 0x9c /* ASCII \r */
#define KC_TAB 0x9d /* ASCII \t */
#define KC_DELETE 0x9e
#define KC_BACKSPACE 0x9f /* ASCII \h */
//#define ESCAPE 0xE000

static cptr ANGBAND_DIR_XTRA_FONT;
static cptr ANGBAND_DIR_XTRA_GRAF;
/* End Angband stuff - NRM */

/**
 * A font structure
 * Note that the data is only valid for a surface with matching
 * values for pitch & bpp. If a surface is resized the data _must_ be
 * recalculated.
 */
typedef struct sdl_Font sdl_Font;
struct sdl_Font
{
    int       width;     /* The dimensions of this font (in pixels)*/
    int       height;
    char      name[255]; /* The name of this font */

    Uint16    pitch;     /* Pitch of the surface this font is made for */
    Uint8     bpp;       /* Bytes per pixel of the surface */
    Uint8     something; /* Padding */

    TTF_Font *sdl_font;  /* The native font */
};

static sdl_Font SystemFont;

#define NUM_GLYPHS 256

/**
 * Window information
 * Each window has its own surface and coordinates
 */
typedef struct term_window term_window;
struct term_window
{
    term term_data;

    SDL_Surface *surface; /* The surface for this window */
#ifdef USE_GRAPHICS
    SDL_Surface *tiles;   /* The appropriately sized tiles for this window */
#endif
    byte Term_idx;        /* Index of term that relates to this */

    int top;              /* Window Coordinates on the main screen */
    int left;

    int keys;             /* Size of keypress storage */

    sdl_Font font;        /* Font info */
    char *req_font;       /* Requested font */
    int rows;             /* Dimension in tiles */
    int cols;

    int border;           /* Border width */
    int title_height;     /* Height of title bar */

    int width;            /* Dimension in pixels ==
                           * tile_wid * cols + 2 x border */
    int height;

    int tile_wid;         /* Size in pixels of a char */
    int tile_hgt;

    bool visible;         /* Can we see this window? */

    SDL_Rect uRect;       /* The part that needs to be updated */
};




typedef struct mouse_info mouse_info;
struct mouse_info
{
    int left;            /* Is it pressed? */
    int right;

    int leftx;           /* _IF_ left button is pressed these */
    int lefty;           /* show where it was pressed */

    int rightx;
    int righty;

    int x;               /* Current position of mouse */
    int y;

};

#define WINDOW_DRAW (SDL_USEREVENT + 1)

typedef struct sdl_ButtonBank sdl_ButtonBank;
typedef struct sdl_Button sdl_Button;
typedef struct sdl_Window sdl_Window;

typedef void (*button_press_func)(sdl_Button *sender);
struct sdl_Button
{
    SDL_Rect pos;               /* Position & Size */
    bool selected;              /* Selected? */
    bool visible;               /* Visible? */
    button_press_func activate; /* A function to call when pressed */
    sdl_ButtonBank *owner;      /* Which bank is this in? */
    char caption[50];           /* Text for this button */
    SDL_Color unsel_colour;     /* Button unselected colour */
    SDL_Color sel_colour;       /* Selected colour*/
    SDL_Color cap_colour;       /* Caption colour */
    void *data;                 /* Something */
    int tag;                    /* Something */

};

struct sdl_ButtonBank
{
    sdl_Button *buttons;        /* A collection of buttons */
    bool *used;                 /* What buttons are available? */
    sdl_Window *window;         /* The window that these buttons are on */
    bool need_update;
};

/**
 * Other 'windows' (basically a surface with a position and buttons on it)
 * Currently used for the top status bar and popup windows
 */
typedef void (*sdl_WindowCustomDraw)(sdl_Window *window);
struct sdl_Window
{
    int top;
    int left;

    int width;
    int height;

    bool visible;

    SDL_Surface *surface;

    sdl_ButtonBank buttons;

    sdl_Font font;

    SDL_Surface *owner;              /* Who shall I display on */

    sdl_WindowCustomDraw draw_extra; /* Stuff to draw on the surface */
    bool need_update;
};




/**
 * The main surface of the application
 */
static SDL_Surface *AppWin;

/**
 * The status bar
 */
static sdl_Window StatusBar;

/**
 * The Popup window
 */
static sdl_Window PopUp;
static bool popped;

/**
 * Term windows
 */
static term_window windows[ANGBAND_TERM_MAX];
static int Zorder[ANGBAND_TERM_MAX];

/** Keep track of the mouse status */
static mouse_info mouse;

/**
 * Hack -- define which keys should be ignored
 */
static bool ignore_key[1024];

/**
 * We ignore all keypresses involving only modifier keys
 */
static int ignore_key_list[] =
{
    SDLK_NUMLOCK, SDLK_CAPSLOCK, SDLK_SCROLLOCK, SDLK_RSHIFT,
    SDLK_LSHIFT, SDLK_RCTRL, SDLK_LCTRL, SDLK_RALT, SDLK_LALT,
    SDLK_RMETA, SDLK_LMETA, SDLK_LSUPER, SDLK_RSUPER, SDLK_MODE,
    SDLK_COMPOSE, 0
};

/**
 * The number pad consists of 10 keys, each with an SDL identifier
 */
#define is_numpad(k) \
((k == SDLK_KP0) || (k == SDLK_KP1) || (k == SDLK_KP2) || (k == SDLK_KP3) || \
 (k == SDLK_KP4) || (k == SDLK_KP5) || (k == SDLK_KP6) || \
 (k == SDLK_KP7) || (k == SDLK_KP8) || (k == SDLK_KP9) || (k == SDLK_KP_ENTER))

static int SnapRange = 5;          /* Window snap range (pixels) */
static int StatusHeight;           /* The height in pixels of the status bar */
static int SelectedTerm;           /* Current selected Term */

static int AboutSelect;            /* About button */
static int TermSelect;             /* Term selector button */
static int FontSelect;             /* Font selector button */
static int VisibleSelect;          /* Hide/unhide window button*/
static int MoreSelect;             /* Other options button */
static int QuitSelect;             /* Quit button */

/* Buttons on the 'More' panel */
static int MoreOK;                 /* Accept changes */
static int MoreFullscreen;         /* Fullscreen toggle button */
static int MoreSnapPlus;           /* Increase snap range */
static int MoreSnapMinus;          /* Decrease snap range */

static bool Moving;                /* Moving a window */
static bool Sizing;                /* Sizing a window */
static SDL_Rect SizingSpot;        /* Rect to descibe the sizing area */
static bool Sizingshow = FALSE;    /* Is the resize thingy displayed? */
static SDL_Rect SizingRect;        /* Rect to describe the current resize window */

#ifdef USE_GRAPHICS
typedef struct GfxInfo GfxInfo;
struct GfxInfo
{
    cptr name;    /* Name to show on button */
    cptr gfxfile; /* The file with tiles */
    int width;    /* Width of a tile */
    int height;   /* Height of a tile */
    cptr pref;    /* Preference file to use */
    int x;        /* Yuk - Pixel location of colour key */
    int y;        /* ditto */
    bool avail;   /* Are the appropriate files available? */
};

static SDL_Surface *GfxSurface = NULL;    /* A surface for the graphics */

#define GfxModes 4
static GfxInfo GfxDesc[GfxModes] =
{
    /* No gfx (GRAPHICS_NONE) */
    {"None", NULL, -1, -1, NULL, 0, 0, TRUE},
    /* 8x8 tiles (GRAPHICS_ORIGINAL) */
    {"8x8", "8x8.png", 8, 8, "old", 0, 0, TRUE},
    /* 16x16 tiles (GRAPHICS_ADAM_BOLT) */
    {"16x16", "16x16.png", 16, 16, "new", 0, 65, TRUE},
    /* XXX (GRAPHICS_DAVID_GERVAIS) */
    {"32x32", "32x32.png", 32, 32, "david", 0, 0, TRUE},

    /* XXX (GRAPHICS_PSEUDO ???) */
    /*{NULL, NULL, NULL, -1, -1},    */
};

static int MoreBigtile;                 /* Toggle bigtile button */
static int MoreDbltile;                 /* Toggle dbltile button */
static int MoreTrptile;                 /* Toggle trptile button */
static int GfxButtons[GfxModes];        /* Graphics mode buttons */
static int SelectedGfx;                 /* Current selected gfx */
#endif

static SDL_Color text_colours[MAX_COLORS];

SDL_Color back_colour;                  /* Background colour */
Uint32 back_pixel_colour;

/**
 * Fill in an SDL_Rect structure.
 * Note it also returns the value adjusted
 */
static SDL_Rect *RECT(int x, int y, int w, int h, SDL_Rect *rect)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;

    return rect;
}

/**
 * Is a point(x, y) in a rectangle?
 */
static bool point_in(SDL_Rect *rect, int x, int y)
{
    if (x < rect->x) return (FALSE);
    if (y < rect->y) return (FALSE);
    if (x >= rect->x + rect->w) return (FALSE);
    if (y >= rect->y + rect->h) return (FALSE);

    /* Must be inside */
    return (TRUE);
}

/**
 * Draw an outline box
 * Given the top, left, width & height
 */
static void sdl_DrawBox(SDL_Surface *surface, SDL_Rect *rect, SDL_Color colour, int width)
{
    SDL_Rect rc;
    int left = rect->x;
    int right = rect->x + rect->w - width;
    int top = rect->y;
    int bottom = rect->y + rect->h - width;
    Uint32 pixel_colour = SDL_MapRGB(surface->format, colour.r, colour.g, colour.b);

    /* Top left -> Top Right */
    RECT(left, top, rect->w, width, &rc);
    SDL_FillRect(surface, &rc, pixel_colour);

    /* Bottom left -> Bottom Right */
    RECT(left, bottom, rect->w, width, &rc);
    SDL_FillRect(surface, &rc, pixel_colour);

    /* Top left -> Bottom left */
    RECT(left, top, width, rect->h, &rc);
    SDL_FillRect(surface, &rc, pixel_colour);

    /* Top right -> Bottom right */
    RECT(right, top, width, rect->h, &rc);
    SDL_FillRect(surface, &rc, pixel_colour);
}

#if 0
#define _TEST_FONT "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf"
#define _TEST_FONT_SIZE 14
#endif

/**
 * Get the width and height of a given font file
 */
static errr sdl_CheckFont(cptr fontname, int *width, int *height)
{
    char buf[1024];

    TTF_Font *ttf_font;

    /* Build the path */
    path_build(buf, sizeof(buf), ANGBAND_DIR_XTRA_FONT, fontname);

    /* Attempt to load it */
#if defined(_TEST_FONT)
    fontname = _TEST_FONT;
    ttf_font = TTF_OpenFont(fontname, _TEST_FONT_SIZE);
#else
    ttf_font = TTF_OpenFont(buf, 0);
#endif

    /* Bugger */
    if (!ttf_font) return (-1);

    /* Get the size */
    if (TTF_SizeText(ttf_font, "M", width, height)) return (-1);

    /* Finished with the font */
    TTF_CloseFont(ttf_font);

    return (0);
}

/**
* The sdl_Font routines
 */

/**
 * Free any memory assigned by Create()
 */
static void sdl_FontFree(sdl_Font *font)
{
    /* Finished with the font */
    TTF_CloseFont(font->sdl_font);
}


/**
 * Create new font data with font fontname, optimizing the data
 * for the surface given
 */
static errr sdl_FontCreate(sdl_Font *font, const char *fontname, SDL_Surface *surface)
{
    char buf[1024];

    TTF_Font *ttf_font;

    /* Build the path */
    path_build(buf, sizeof(buf), ANGBAND_DIR_XTRA_FONT, fontname);

    /* Attempt to load it */
#if defined(_TEST_FONT)
    fontname = _TEST_FONT;
    ttf_font = TTF_OpenFont(fontname, _TEST_FONT_SIZE);
#else
    ttf_font = TTF_OpenFont(buf, 0);
#endif

    /* Bugger */
    if (!ttf_font) return (-1);

    /* Get the size */
    if (TTF_SizeText(ttf_font, "M", &font->width, &font->height)) return (-1);

    /* Fill in some of the font struct */
    if (font->name != fontname) my_strcpy(font->name, fontname, 30);
    font->pitch = surface->pitch;
    font->bpp = surface->format->BytesPerPixel;
    font->sdl_font = ttf_font;

    /* Success */
    return (0);
}

/**
 * Draw some text onto a surface
 * The surface is first checked to see if it is compatible with
 * this font, if it isn't the the font will be 're-precalculated'
 *
 * You can, I suppose, use one font on many surfaces, but it is
 * definitely not recommended. One font per surface is good enough.
 */
static errr sdl_FontDraw(sdl_Font *font, SDL_Surface *surface, SDL_Color colour,
                         int x, int y, int n , const char *s)
{
    Uint8        bpp = surface->format->BytesPerPixel;
    Uint16       pitch = surface->pitch;
    SDL_Rect     rc;
    SDL_Surface *text;
    string_ptr   copy;

    if (bpp != font->bpp || pitch != font->pitch)
        sdl_FontCreate(font, font->name, surface);

    /* Lock the window surface (if necessary) */
    if (SDL_MUSTLOCK(surface))
        if (SDL_LockSurface(surface) < 0)
            return (-1);

    RECT(x, y, n * font->width, font->height, &rc);
    copy = string_copy_sn(s, n); /* s is not null terminated ... */
    text = TTF_RenderText_Solid(font->sdl_font, string_buffer(copy), colour);
    string_free(copy);
    if (text) {
        SDL_BlitSurface(text, NULL, surface, &rc);
        SDL_FreeSurface(text);
    }

    /* Unlock the surface */
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    /* Success */
    return 0;
}

/**
 * Draw a button on the screen
 */
static void sdl_ButtonDraw(sdl_Button *button)
{
    SDL_Surface *surface = button->owner->window->surface;
    sdl_Font *font = &button->owner->window->font;

    SDL_Color colour = button->selected ? button->sel_colour : button->unsel_colour;

    if (!button->visible) return;

    SDL_FillRect(surface, &button->pos, SDL_MapRGB(surface->format, colour.r,
                                                   colour.g, colour.b));

    if (strlen(button->caption))
    {
        unsigned max = button->pos.w / font->width;
        int n = strlen(button->caption) > max ? max : strlen(button->caption);
        int l = n * font->width / 2;
        int x = button->pos.x + ((button->pos.w) / 2) - l;

        sdl_FontDraw(font, surface, button->cap_colour,
                     x, button->pos.y + 1, n, button->caption);
    }
}

/**
 * Adjust the position of a button
 */
static void sdl_ButtonMove(sdl_Button *button, int x, int y)
{
    button->pos.x = x;
    button->pos.y = y;
    button->owner->need_update = TRUE;
}

/**
 * Adjust the size of a button
 */
static void sdl_ButtonSize(sdl_Button *button, int w, int h)
{
    button->pos.w = w;
    button->pos.h = h;
    button->owner->need_update = TRUE;
}

/**
 * Set the caption
 */
static void sdl_ButtonCaption(sdl_Button *button, cptr s)
{
    my_strcpy(button->caption, s, sizeof(button->caption));
    button->owner->need_update = TRUE;
}

/**
 * Set the visibility of a button
 */
static void sdl_ButtonVisible(sdl_Button *button, bool visible)
{
    if (button->visible != visible)
    {
        button->visible = visible;
        button->owner->need_update = TRUE;
    }
}


/** Maximum amount of buttons in a bank */
#define MAX_BUTTONS 20

/*
 * The button_bank package
 */
/**
 * Initialize it
 */
static void sdl_ButtonBankInit(sdl_ButtonBank *bank, sdl_Window *window)
{
    bank->window = window;

    C_MAKE(bank->buttons, MAX_BUTTONS, sdl_Button);
    C_MAKE(bank->used, MAX_BUTTONS, bool);
    bank->need_update = TRUE;
}

/**
 * Clear the bank
 */
static void sdl_ButtonBankFree(sdl_ButtonBank *bank)
{
    (void)FREE(bank->buttons, sdl_Button);
    (void)FREE(bank->used, bool);
}

/**
 * Draw all the buttons on the screen
 */
static void sdl_ButtonBankDrawAll(sdl_ButtonBank *bank)
{
    int i;

    for (i = 0; i < MAX_BUTTONS; i++)
    {
        sdl_Button *button = &bank->buttons[i];

        if (!bank->used[i]) continue;
        if (!button->visible) continue;

        sdl_ButtonDraw(button);
    }
    bank->need_update = FALSE;
}

/**
 * Get a new button index
 */
static int sdl_ButtonBankNew(sdl_ButtonBank *bank)
{
    int i = 0;
    sdl_Button *new_button;

    while (bank->used[i] && (i < MAX_BUTTONS)) i++;

    if (i == MAX_BUTTONS)
    {
        /* Bugger! */
        return (-1);
    }

    /* Get the button */
    new_button = &bank->buttons[i];

    /* Mark the button as used */
    bank->used[i] = TRUE;

    /* Clear it */
    WIPE(new_button, sdl_Button);

    /* Mark it as mine */
    new_button->owner = bank;

    /* Default colours */
    new_button->unsel_colour.r = 160;
    new_button->unsel_colour.g = 160;
    new_button->unsel_colour.b = 60;
    new_button->sel_colour.r = 210;
    new_button->sel_colour.g = 210;
    new_button->sel_colour.b = 110;
    new_button->cap_colour.r = 0;
    new_button->cap_colour.g = 0;
    new_button->cap_colour.b = 0;

    /* Success */
    return (i);
}

/**
 * Retrieve button 'idx' or NULL
 */
static sdl_Button *sdl_ButtonBankGet(sdl_ButtonBank *bank, int idx)
{
    /* Check the index */
    if ((idx < 0) || (idx >= MAX_BUTTONS)) return (NULL);
    if (!bank->used[idx]) return (NULL);

    /* Return it */
    return &bank->buttons[idx];
}

#if 0
/**
 * Remove a Button by its index
 */
static void sdl_ButtonBankRemove(sdl_ButtonBank *bank, int idx)
{
    sdl_Button *button;

    /* Check the index */
    if ((idx < 0) || (idx >= MAX_BUTTONS)) return;
    if (!bank->used[idx]) return;

    /* Grab it */
    button = &bank->buttons[idx];

    /* Hide it */
    button->visible = FALSE;

    /* Draw it */
    bank->need_update = TRUE;

    /* Forget */
    bank->used[idx] = FALSE;
}

#endif

/**
 * Examine and respond to mouse presses
 * Return if we 'handled' the click
 */
static bool sdl_ButtonBankMouseDown(sdl_ButtonBank *bank, int x, int y)
{
    int i;

    /* Check every button */
    for (i = 0; i < MAX_BUTTONS; i++)
    {
        sdl_Button *button = &bank->buttons[i];

        /* Discard some */
        if (!bank->used[i]) continue;
        if (!button->visible) continue;

        /* Check the coordinates */
        if (point_in(&button->pos, x, y))
        {
            button->selected = TRUE;

            /* Draw it */
            bank->need_update = TRUE;

            return (TRUE);
        }
    }
    return (FALSE);
}

/**
 * Respond to a mouse button release
 */
static bool sdl_ButtonBankMouseUp(sdl_ButtonBank *bank, int x, int y)
{
    int i;

    /* Check every button */
    for (i = 0; i < MAX_BUTTONS; i++)
    {
        sdl_Button *button = &bank->buttons[i];

        /* Discard some */
        if (!bank->used[i]) continue;
        if (!button->visible) continue;

        /* Check the coordinates */
        if (point_in(&button->pos, x, y))
        {
            /* Has this butoon been 'selected'? */
            if (button->selected)
            {
                /* Activate the button (usually) */
                if (button->activate) (*button->activate)(button);

                /* Now not selected */
                button->selected = FALSE;

                /* Draw it */
                bank->need_update = TRUE;

                return (TRUE);
            }
        }
        else
        {
            /* This button was 'selected' but the release of the */
            /* mouse button was outside the area of this button */
            if (button->selected)
            {
                /* Now not selected */
                button->selected = FALSE;

                /* Draw it */
                bank->need_update = TRUE;
            }
        }
    }

    return (FALSE);
}

/**
 * sdl_Window functions
 */
static void sdl_WindowFree(sdl_Window* window)
{
    if (window->surface)
    {
        SDL_FreeSurface(window->surface);
        sdl_ButtonBankFree(&window->buttons);
        sdl_FontFree(&window->font);
        WIPE(window, sdl_Window);
    }
}

/**
 * Initialize a window
 */
static void sdl_WindowInit(sdl_Window* window, int w, int h,
                           SDL_Surface *owner, cptr fontname)
{
    sdl_WindowFree(window);
    window->owner = owner;
    window->width = w;
    window->height = h;
    window->surface = SDL_CreateRGBSurface(
        SDL_SWSURFACE, w, h,
        owner->format->BitsPerPixel,
        owner->format->Rmask,
        owner->format->Gmask,
        owner->format->Bmask,
        owner->format->Amask
    );
    sdl_ButtonBankInit(&window->buttons, window);
    sdl_FontCreate(&window->font, fontname, window->surface);
    window->visible = TRUE;
    window->need_update = TRUE;
}


static void sdl_WindowBlit(sdl_Window* window)
{
    SDL_Rect rc;

    if (!window->visible) return;

    RECT(window->left, window->top, window->width, window->height, &rc);

    SDL_BlitSurface(window->surface, NULL, window->owner, &rc);
    SDL_UpdateRects(window->owner, 1, &rc);
}

static void sdl_WindowText(sdl_Window* window, SDL_Color c, int x, int y, cptr s)
{
    sdl_FontDraw(&window->font, window->surface, c, x, y, strlen(s), s);
}

static void sdl_WindowUpdate(sdl_Window* window)
{
    if ((window->need_update || window->buttons.need_update) && (window->visible))
    {
        SDL_Event Event;

        SDL_FillRect(window->surface, NULL, back_pixel_colour);

        if (window->draw_extra) (*window->draw_extra)(window);

        sdl_ButtonBankDrawAll(&window->buttons);

        window->need_update = FALSE;

        WIPE(&Event, SDL_Event);

        Event.type = WINDOW_DRAW;

        Event.user.data1 = (void*)window;

        SDL_PushEvent(&Event);
    }
}



static void term_windowFree(term_window* win)
{
    if (win->surface)
    {
        SDL_FreeSurface(win->surface);
        win->surface = NULL;
#ifdef USE_GRAPHICS
        /* Invalidate the gfx surface */
        if (win->tiles)
        {
            SDL_FreeSurface(win->tiles);
            win->tiles = NULL;
        }
#endif
        term_nuke(&win->term_data);
    }

    sdl_FontFree(&win->font);
}

static errr save_prefs(void);
static void hook_quit(cptr str)
{
    int i;

    save_prefs();

    z_string_free(ANGBAND_DIR_USER_SDL);

    /* Free the surfaces of the windows */
    for (i = 0; i < ANGBAND_TERM_MAX; i++)
    {
        term_windowFree(&windows[i]);
        z_string_free(windows[i].req_font);
    }

#ifdef USE_GRAPHICS
    /* Free the graphics surface */
    if (GfxSurface) SDL_FreeSurface(GfxSurface);
#endif
    /* Free the 'System font' */
    sdl_FontFree(&SystemFont);

    /* Free the statusbar window */
    sdl_WindowFree(&StatusBar);

    /* free the popup window */
    sdl_WindowFree(&PopUp);

    /* Free the main surface */
    SDL_FreeSurface(AppWin);

    /* Shut down the font library */
    TTF_Quit();

    /* Shut down SDL */
        SDL_Quit();

        for (i = 0; i < MAX_FONTS; i++)
        {
                if (FontList[i]) z_string_free(FontList[i]);
        }
}

static void BringToTop(void)
{
    int i, idx;

    for (idx = 0; idx < ANGBAND_TERM_MAX; idx++)
    {
        if (Zorder[idx] == SelectedTerm) break;
    }

    if (idx == ANGBAND_TERM_MAX) return;

    for (i = idx; i < ANGBAND_TERM_MAX - 1; i++)
    {
        Zorder[i] = Zorder[i + 1];
    }

    Zorder[ANGBAND_TERM_MAX - 1] = SelectedTerm;
}



bool file_exists(const char *fname)
{
    FILE *f = my_fopen(fname, "r");

    if (f) my_fclose(f);
    return (f ? TRUE : FALSE);
}

/**
 * Validate a file
 */
static void validate_file(cptr s)
{
    if (!file_exists(s))
        quit_fmt("Cannot find required file:\n%s", s);
}

/**
 * Find a window that is under the points x,y on
 * the main screen
 */

static int sdl_LocateWin(int x, int y)
{
    int i;

    for (i = ANGBAND_TERM_MAX - 1; i >= 0; i--)
    {
        term_window *win = &windows[Zorder[i]];
        SDL_Rect rc;

        if (!win->visible) continue;
        if (!point_in(RECT(win->left, win->top, win->width, win->height, &rc), x, y)) continue;
        return (Zorder[i]);
    }
    return -1;
}

static void draw_statusbar(sdl_Window *window)
{
    char buf[128];
    term_window *win = &windows[SelectedTerm];
    int fw = window->font.width;
    int x = 1;
    sdl_Button *button;

    SDL_Rect rc;

    SDL_Color c = {160, 160, 60, 0};

    RECT(0, StatusBar.height - 1, StatusBar.width, 1, &rc);
    SDL_FillRect(StatusBar.surface, &rc, SDL_MapRGB(StatusBar.surface->format,
                                                    c.r, c.g, c.b));

    button = sdl_ButtonBankGet(&StatusBar.buttons, AboutSelect);
    x += button->pos.w + 20;

    sdl_WindowText(&StatusBar, c, x, 1, "Term:");
    x += 5 * fw;

    button = sdl_ButtonBankGet(&StatusBar.buttons, TermSelect);
    button->pos.x = x;
    x += button->pos.w + 10;

    my_strcpy(buf, format("(%dx%d)", win->cols, win->rows), sizeof(buf));
    sdl_WindowText(&StatusBar, c, x, 1, buf);
    x += strlen(buf) * fw + 20;

    sdl_WindowText(&StatusBar, c, x, 1, "Visible:");
    x += 8 * fw;

    button = sdl_ButtonBankGet(&StatusBar.buttons, VisibleSelect);
    button->pos.x = x;
    x += button->pos.w + 20;

    button = sdl_ButtonBankGet(&StatusBar.buttons, FontSelect);
    if (button->visible) sdl_WindowText(&StatusBar, c, x, 1, "Font:");
    x += 5 * fw;

    button->pos.x = x;
    x += button->pos.w + 20;

    button = sdl_ButtonBankGet(&StatusBar.buttons, MoreSelect);
    button->pos.x = x;

    x += button->pos.w + 20;
}



static void sdl_BlitWin(term_window *win)
{
    SDL_Rect rc;

    if (!win->surface) return;
    if (!win->visible) return;
    if (win->uRect.x == -1) return;

    /* Select the area to be updated */
    RECT(win->left + win->uRect.x, win->top + win->uRect.y, win->uRect.w,
         win->uRect.h, &rc);

    SDL_BlitSurface(win->surface, &win->uRect, AppWin, &rc);
    SDL_UpdateRects(AppWin, 1, &rc);

    /* Mark the update as complete */
    win->uRect.x = -1;
}

static void sdl_BlitAll(void)
{
    SDL_Rect rc;
    sdl_Window *window = &StatusBar;
    int i;
    SDL_Color colour = {160, 160, 60, 0};
    SDL_FillRect(AppWin, NULL, back_pixel_colour);

    for (i = 0; i < ANGBAND_TERM_MAX; i++)
    {
        term_window *win = &windows[Zorder[i]];

        if (!win->surface) continue;
        if (!win->visible) continue;

        RECT(win->left, win->top, win->width, win->height, &rc);
        SDL_BlitSurface(win->surface, NULL, AppWin, &rc);

        if (Zorder[i] == SelectedTerm)
        {
            SizingSpot.w = 10;
            SizingSpot.h = 10;
            SizingSpot.x = win->left + win->width - 10;
            SizingSpot.y = win->top + win->height - 10;
            //SDL_FillRect(AppWin, &SizingSpot, ccolour);

            if (Sizing)
            {
                int width = 2;
                int grabsize = 10;
                rc = SizingRect;
                SizingSpot.w = grabsize;
                SizingSpot.h = grabsize;
                SizingSpot.x = SizingRect.x + SizingRect.w - grabsize;
                SizingSpot.y = SizingRect.y + SizingRect.h - grabsize;
                sdl_DrawBox(AppWin, &rc, colour, width);
            }
        }
    }

    RECT(window->left, window->top, window->width, window->height, &rc);

    SDL_BlitSurface(window->surface, NULL, AppWin, &rc);

    SDL_UpdateRect(AppWin, 0, 0, AppWin->w, AppWin->h);
}

static void RemovePopUp()
{
    PopUp.visible = FALSE;
    popped = FALSE;
    sdl_BlitAll();
}

static void QuitActivate(sdl_Button *sender)
{
    SDL_Event Event;

    Event.type = SDL_QUIT;

    SDL_PushEvent(&Event);
}

static void SetStatusButtons(void)
{
    term_window *win = &windows[SelectedTerm];
    sdl_Button *button = sdl_ButtonBankGet(&StatusBar.buttons, TermSelect);
    sdl_Button *fontbutton = sdl_ButtonBankGet(&StatusBar.buttons, FontSelect);
    sdl_Button *visbutton = sdl_ButtonBankGet(&StatusBar.buttons, VisibleSelect);

    sdl_ButtonCaption(button, angband_term_name[SelectedTerm]);

    if (!win->visible)
    {
        sdl_ButtonVisible(fontbutton, FALSE);
        sdl_ButtonCaption(visbutton, "No");
    }
    else
    {
        sdl_ButtonVisible(fontbutton, TRUE);
        sdl_ButtonCaption(fontbutton, win->font.name);
        sdl_ButtonCaption(visbutton, "Yes");
    }
}

static void TermFocus(int idx)
{
    if (SelectedTerm == idx) return;
    SelectedTerm = idx;
    BringToTop();
    SetStatusButtons();
    sdl_BlitAll();
}

static void AboutDraw(sdl_Window *win)
{
    SDL_Rect rc;

    /* Wow - a different colour! */
    SDL_Color colour = {160, 60, 60, 0};

    RECT(0, 0, win->width, win->height, &rc);

    /* Draw a nice box */
    sdl_DrawBox(win->surface, &rc, colour, 5);
}


static void AboutActivate(sdl_Button *sender)
{
    int width = 300;
    int height = 300;

    sdl_WindowInit(&PopUp, width, height, AppWin, StatusBar.font.name);
    PopUp.left = (AppWin->w / 2) - width / 2;
    PopUp.top = (AppWin->h / 2) - height / 2;
    PopUp.draw_extra = AboutDraw;

    popped = TRUE;
}

static void SelectTerm(sdl_Button *sender)
{
    RemovePopUp();

    TermFocus(sender->tag);
}

static void TermActivate(sdl_Button *sender)
{
    int i, maxl = 0;
    int width, height;
    for (i = 0; i < ANGBAND_TERM_MAX; i++)
    {
        int l = strlen(angband_term_name[i]);
        if (l > maxl) maxl = l;
    }
    width = maxl * StatusBar.font.width + 20;
    height = ANGBAND_TERM_MAX * (StatusBar.font.height + 1);

    sdl_WindowInit(&PopUp, width, height, AppWin, StatusBar.font.name);
    PopUp.left = sender->pos.x;
    PopUp.top = sender->pos.y;

    for (i = 0; i < ANGBAND_TERM_MAX; i++)
    {
        int h = PopUp.font.height;
        int b = sdl_ButtonBankNew(&PopUp.buttons);
        sdl_Button *button = sdl_ButtonBankGet(&PopUp.buttons, b);
        sdl_ButtonSize(button, width - 2 , h);
        sdl_ButtonMove(button, 1, i * (h + 1));
        sdl_ButtonCaption(button, angband_term_name[i]);
        sdl_ButtonVisible(button, TRUE);
        button->tag = i;
        button->activate = SelectTerm;
    }
    popped = TRUE;
}

static void ResizeWin(term_window* win, int w, int h);
static void term_data_link_sdl(term_window *win);

static void VisibleActivate(sdl_Button *sender)
{
    term_window *window = &windows[SelectedTerm];

    if (SelectedTerm == 0) return;

    if (window->visible)
    {
        window->visible = FALSE;
        term_windowFree(window);
        angband_term[SelectedTerm] = NULL;

    }
    else
    {
        window->visible = TRUE;
        ResizeWin(window, window->width, window->height);
    }

    SetStatusButtons();
    sdl_BlitAll();
}

static void SelectFont(sdl_Button *sender)
{
    term_window *window = &windows[SelectedTerm];
    int w, h;

    sdl_FontFree(&window->font);
    z_string_free(window->req_font);

    window->req_font = (char *)z_string_make(sender->caption);

    sdl_CheckFont(window->req_font, &w, &h);
#ifdef USE_GRAPHICS
    /* Invalidate the gfx surface */
    if (window->tiles)
    {
        SDL_FreeSurface(window->tiles);
        window->tiles = NULL;
    }
#endif

    ResizeWin(window, (w * window->cols) + (2 * window->border),
              (h * window->rows) + window->border + window->title_height);

    SetStatusButtons();

    RemovePopUp();
}


static void FontActivate(sdl_Button *sender)
{
    int i, maxl = 0;
    int width, height;
    for (i = 0; i < num_fonts; i++)
    {
        int l = strlen(FontList[i]);
        if (l > maxl) maxl = l;
    }
    width = maxl * StatusBar.font.width + 20;
    height = num_fonts * (StatusBar.font.height + 1);

    sdl_WindowInit(&PopUp, width, height, AppWin, StatusBar.font.name);
    PopUp.left = sender->pos.x;
    PopUp.top = sender->pos.y;

    for (i = 0; i < num_fonts; i++)
    {
        int h = PopUp.font.height;
        int b = sdl_ButtonBankNew(&PopUp.buttons);
        sdl_Button *button = sdl_ButtonBankGet(&PopUp.buttons, b);
        sdl_ButtonSize(button, width - 2 , h);
        sdl_ButtonMove(button, 1, i * (h + 1));
        sdl_ButtonCaption(button, FontList[i]);
        sdl_ButtonVisible(button, TRUE);
        button->activate = SelectFont;
    }
    popped = TRUE;
}

#ifdef USE_GRAPHICS
static errr load_gfx(void);

static void SelectGfx(sdl_Button *sender)
{
    SelectedGfx = sender->tag;
}
#endif

static void AcceptChanges(sdl_Button *sender)
{
    sdl_Button *button;
    bool do_update = FALSE;
    bool do_video_reset = FALSE;

#ifdef USE_GRAPHICS
#if 0
    sdl_Button *button1, *button2, *button3;
    bool check_size = FALSE;
    /* Check to see if tile size has changed */
    button1 = sdl_ButtonBankGet(&PopUp.buttons, MoreBigtile);
    button2 = sdl_ButtonBankGet(&PopUp.buttons, MoreDbltile);
    button3 = sdl_ButtonBankGet(&PopUp.buttons, MoreTrptile);

    if (button1->tag != use_bigtile)
    {
        do_update = TRUE;
        use_bigtile = !use_bigtile;
    }

    if (button2->tag != use_dbltile)
    {
        do_update = TRUE;

        use_dbltile = !use_dbltile;
        if (use_trptile)
        {
            use_trptile = FALSE;
            check_size = TRUE;
        }
    }

    if ((button3->tag != use_trptile) && !check_size)
    {
        do_update = TRUE;

        use_trptile = !use_trptile;
        if (use_dbltile) use_dbltile = FALSE;
    }
#endif
    if (use_graphics != SelectedGfx)
    {
        do_update = TRUE;

        use_graphics = SelectedGfx;

        if (use_graphics)
        {
            arg_graphics = TRUE;
            load_gfx();
        }
        else
        {
            arg_graphics = FALSE;
            //use_bigtile = FALSE;
            //use_dbltile = FALSE;
            //use_trptile = FALSE;
            reset_visuals();
        }
    }

    /* Invalidate all the gfx surfaces */
    if (do_update)
    {
        int i;
        for (i = 0; i < ANGBAND_TERM_MAX; i++)
        {
            term_window *win = &windows[i];
            if (win->tiles)
            {
                SDL_FreeSurface(win->tiles);
                win->tiles = NULL;
            }
        }
    }

#endif

    button = sdl_ButtonBankGet(&PopUp.buttons, MoreFullscreen);

    if (button->tag != fullscreen)
    {
        fullscreen = !fullscreen;

        do_video_reset = TRUE;
    }


    SetStatusButtons();

    RemovePopUp();

    if (do_update)
    {
        if (character_dungeon) do_cmd_redraw();
    }

    if (do_video_reset)
    {
        SDL_Event Event;

        WIPE(&Event, SDL_Event);

        Event.type = SDL_VIDEORESIZE;
        Event.resize.w = screen_w;
        Event.resize.h = screen_h;

        SDL_PushEvent(&Event);
    }

}

static void FlipTag(sdl_Button *sender)
{
    if (sender->tag)
    {
        sender->tag = 0;
        sdl_ButtonCaption(sender, "Off");
    }
    else
    {
        sender->tag = 1;
        sdl_ButtonCaption(sender, "On");
    }
}

static void SnapChange(sdl_Button *sender)
{
    SnapRange += sender->tag;
    if (SnapRange < 0) SnapRange = 0;
    if (SnapRange > 20) SnapRange = 20;
    PopUp.need_update = TRUE;
}

static void MoreDraw(sdl_Window *win)
{
    SDL_Rect rc;
    sdl_Button *button, *button1, *button2, *button3;
    int y = 20, i;
    SDL_Color colour = {160, 60, 60, 0};
    SDL_Color select_colour = {210, 110, 110, 0};

    RECT(0, 0, win->width, win->height, &rc);

    /* Draw a nice box */
    sdl_DrawBox(win->surface, &rc, colour, 5);

#ifdef USE_GRAPHICS

    button1 = sdl_ButtonBankGet(&win->buttons, MoreBigtile);
    button2 = sdl_ButtonBankGet(&win->buttons, MoreDbltile);
    button3 = sdl_ButtonBankGet(&win->buttons, MoreTrptile);

    if (SelectedGfx)
    {
        sdl_ButtonVisible(button1, TRUE);
        sdl_ButtonVisible(button2, TRUE);
        sdl_ButtonVisible(button3, TRUE);
        sdl_WindowText(win, colour, 20, y, "Bigtile is:");
        sdl_WindowText(win, colour, 20, y + 20, "Dbltile is:");
        sdl_WindowText(win, colour, 20, y + 40, "Trptile is:");

        sdl_ButtonMove(button1, 200, y);
        sdl_ButtonMove(button2, 200, y + 20);
        sdl_ButtonMove(button3, 200, y + 40);

        y += 60;
    }
    else
    {
        sdl_ButtonVisible(button1, FALSE);
        sdl_ButtonVisible(button2, FALSE);
        sdl_ButtonVisible(button3, FALSE);
    }

    sdl_WindowText(win, colour, 20, y, "Selected Graphics:");
    sdl_WindowText(win, select_colour,
                   200, y, GfxDesc[SelectedGfx].name);

    y += 20;

    sdl_WindowText(win, colour, 20, y, "Available Graphics:");

    for (i = 0; i < GfxModes; i++)
    {
        if (!GfxDesc[i].avail) continue;
        button = sdl_ButtonBankGet(&win->buttons, GfxButtons[i]);
        sdl_ButtonMove(button, 200, y);
        y += 20;
    }
#endif

    button = sdl_ButtonBankGet(&win->buttons, MoreFullscreen);
    sdl_WindowText(win, colour, 20, y, "Fullscreen is:");

    sdl_ButtonMove(button, 200, y);
    y+= 20;

    sdl_WindowText(win, colour, 20, y, format("Snap range is %d.", SnapRange));
    button = sdl_ButtonBankGet(&win->buttons, MoreSnapMinus);
    sdl_ButtonMove(button, 200, y);

    button = sdl_ButtonBankGet(&win->buttons, MoreSnapPlus);
    sdl_ButtonMove(button, 230, y);
}

static void MoreActivate(sdl_Button *sender)
{
    int width = 300;
    int height = 300;
    int i;
    sdl_Button *button/*, *button1, *button2, *button3*/;

    SDL_Color ucolour = {160, 60, 60, 0};
    SDL_Color scolour = {210, 110, 110, 0};

    sdl_WindowInit(&PopUp, width, height, AppWin, StatusBar.font.name);
    PopUp.left = (AppWin->w / 2) - width / 2;
    PopUp.top = (AppWin->h / 2) - height / 2;
    PopUp.draw_extra = MoreDraw;

#ifdef USE_GRAPHICS
#if 0
    MoreBigtile = sdl_ButtonBankNew(&PopUp.buttons);
    button1 = sdl_ButtonBankGet(&PopUp.buttons, MoreBigtile);

    button1->unsel_colour = ucolour;
    button1->sel_colour = scolour;
    sdl_ButtonSize(button1, 50 , PopUp.font.height + 2);
    sdl_ButtonVisible(button1, TRUE);
    sdl_ButtonCaption(button1, use_bigtile ? "On" : "Off");
    button1->tag = use_bigtile;
    button1->activate = FlipTag;

    MoreDbltile = sdl_ButtonBankNew(&PopUp.buttons);
    button2 = sdl_ButtonBankGet(&PopUp.buttons, MoreDbltile);

    button2->unsel_colour = ucolour;
    button2->sel_colour = scolour;
    sdl_ButtonSize(button2, 50 , PopUp.font.height + 2);
    sdl_ButtonVisible(button2, TRUE);
    sdl_ButtonCaption(button2, use_dbltile ? "On" : "Off");
    button2->tag = use_dbltile;
    button2->activate = FlipTag;

    MoreTrptile = sdl_ButtonBankNew(&PopUp.buttons);
    button3 = sdl_ButtonBankGet(&PopUp.buttons, MoreTrptile);

    button3->unsel_colour = ucolour;
    button3->sel_colour = scolour;
    sdl_ButtonSize(button3, 50 , PopUp.font.height + 2);
    sdl_ButtonVisible(button3, TRUE);
    sdl_ButtonCaption(button3, use_trptile ? "On" : "Off");
    button3->tag = use_trptile;
    button3->activate = FlipTag;
#endif
    SelectedGfx = use_graphics;

    for (i = 0; i < GfxModes; i++)
    {
        if (!GfxDesc[i].avail) continue;
        GfxButtons[i] = sdl_ButtonBankNew(&PopUp.buttons);
        button = sdl_ButtonBankGet(&PopUp.buttons, GfxButtons[i]);

        button->unsel_colour = ucolour;
        button->sel_colour = scolour;
        sdl_ButtonSize(button, 50 , PopUp.font.height + 2);
        sdl_ButtonVisible(button, TRUE);
        sdl_ButtonCaption(button, GfxDesc[i].name);
        button->tag = i;
        button->activate = SelectGfx;
    }
#endif
    MoreFullscreen = sdl_ButtonBankNew(&PopUp.buttons);
    button = sdl_ButtonBankGet(&PopUp.buttons, MoreFullscreen);

    button->unsel_colour = ucolour;
    button->sel_colour = scolour;
    sdl_ButtonSize(button, 50 , PopUp.font.height + 2);
    sdl_ButtonVisible(button, TRUE);
    sdl_ButtonCaption(button, fullscreen ? "On" : "Off");
    button->tag = fullscreen;
    button->activate = FlipTag;

    MoreSnapPlus = sdl_ButtonBankNew(&PopUp.buttons);
    button = sdl_ButtonBankGet(&PopUp.buttons, MoreSnapPlus);

    button->unsel_colour = ucolour;
    button->sel_colour = scolour;
    sdl_ButtonSize(button, 20, PopUp.font.height + 2);
    sdl_ButtonCaption(button, "+");
    button->tag = 1;
    sdl_ButtonVisible(button, TRUE);
    button->activate = SnapChange;

    MoreSnapMinus = sdl_ButtonBankNew(&PopUp.buttons);
    button = sdl_ButtonBankGet(&PopUp.buttons, MoreSnapMinus);

    button->unsel_colour = ucolour;
    button->sel_colour = scolour;
    sdl_ButtonSize(button, 20, PopUp.font.height + 2);
    sdl_ButtonCaption(button, "-");
    button->tag = -1;
    sdl_ButtonVisible(button, TRUE);
    button->activate = SnapChange;

    MoreOK = sdl_ButtonBankNew(&PopUp.buttons);
    button = sdl_ButtonBankGet(&PopUp.buttons, MoreOK);

    button->unsel_colour = ucolour;
    button->sel_colour = scolour;
    sdl_ButtonSize(button, 50 , PopUp.font.height + 2);
    sdl_ButtonVisible(button, TRUE);
    sdl_ButtonCaption(button, "OK");
    sdl_ButtonMove(button, width / 2 - 25, height - 40);
    button->activate = AcceptChanges;

    popped = TRUE;
}

static errr Term_xtra_sdl_clear(void);

/**
 * Make a window with size (x,y) pixels
 * Note: The actual size of the window may end up smaller.
 * This may be called when a window wants resizing,
 * is made visible, or the font has changed.
 * This function doesn't go in for heavy optimization, and doesn't need it-
 * it may initialize a few too many redraws or whatnot, but everything gets done!
 */
static void ResizeWin(term_window* win, int w, int h)
{
    /* Don't bother */
    if (!win->visible) return;

    win->border = 2;
    win->title_height = StatusHeight;

    /* No font - a new font is needed -> get dimensions */
    if (!win->font.sdl_font)
    {
        /* Get font dimensions */
        sdl_CheckFont(win->req_font, &win->tile_wid, &win->tile_hgt);

        /* Oops */
        if (!win->tile_wid || !win->tile_hgt)
            quit(format("Unable to find font '%s'.", win->req_font));
    }

    /* Get the amount of columns & rows */
    win->cols = (w - (win->border * 2)) / win->tile_wid;
    win->rows = (h - win->border - win->title_height) / win->tile_hgt;

    /* Calculate the width & height */
    win->width = (win->cols * win->tile_wid) + (win->border * 2);
    win->height = (win->rows * win->tile_hgt) + win->border + win->title_height;

    /* Delete the old surface */
    if (win->surface) SDL_FreeSurface(win->surface);

    /* Create a new surface */
    win->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, win->width, win->height,
                                           AppWin->format->BitsPerPixel,
                                           AppWin->format->Rmask, AppWin->format->Gmask,
                                           AppWin->format->Bmask, AppWin->format->Amask);

    /* Fill it */
    SDL_FillRect(win->surface, NULL, SDL_MapRGB(AppWin->format, 160, 160, 60));

    /* Label it */
    sdl_FontDraw(&SystemFont, win->surface, back_colour, 1, 1,
                 strlen(angband_term_name[win->Term_idx]), angband_term_name[win->Term_idx]);

    /* Mark the whole window for redraw */
    RECT(0, 0, win->width, win->height, &win->uRect);

    /* Create the font if we need to */
    if (!win->font.sdl_font)
    {
        sdl_FontCreate(&win->font, win->req_font, win->surface);
    }

    /* This window was never visible before */
    if (!angband_term[win->Term_idx])
    {
        term *old = Term;

        /* Initialize the term data */
        term_data_link_sdl(win);

        /* Make it visible to angband */
        angband_term[win->Term_idx] = &win->term_data;

        /* Activate it */
        Term_activate((term*)&win->term_data);

        /* Redraw */
        Term_redraw();

        /* Restore */
        Term_activate(old);
    }
    /* Resize the term */
    else
    {
        term *old = Term;

        /* Activate it */
        Term_activate((term*)&win->term_data);

        /* Resize */
        Term_resize(win->cols, win->rows);

        /* Redraw */
        Term_redraw();

        /* Restore */
        Term_activate(old);
    }

    /* Calculate the hotspot */
    if (win->Term_idx == SelectedTerm)
    {
        SizingSpot.w = 10;
        SizingSpot.h = 10;
        SizingSpot.x = win->left + win->width - 10;
        SizingSpot.y = win->top + win->height - 10;
    }

    StatusBar.need_update = TRUE;

    /* HACK - Redraw all windows */
    if (character_dungeon) do_cmd_redraw();
}


static errr load_prefs(void)
{
    char buf[1024];
    FILE *fff;
    term_window *win;
    int i;

    /* Initialize the windows with crappy defaults! */
    for (i = 0; i < ANGBAND_TERM_MAX; i++)
    {
        win = &windows[i];

        /* Clear the data */
        WIPE(win, term_window);

        /* Who? */
        win->Term_idx = i;

        win->req_font = (char *)z_string_make(DEFAULT_FONT_FILE);

        if (i == 0)
        {
            win->top = StatusHeight;
            win->width = 600;
            win->height = 380;
            win->keys = 1024;
            win->visible = TRUE;
        }
        else
        {
            win->top = 400 + (i * 10);
            win->left = (i - 1) * 10;
            win->width = 400;
            win->height = 200;
            win->keys = 32;

            win->visible = FALSE;
        }
    }

    /* Build the path */
    path_build(buf, sizeof(buf), ANGBAND_DIR_USER, "sdlinit.txt");

    /* XXXXX */
    ANGBAND_DIR_USER_SDL = (char *)z_string_make(buf);

    /* Open the file */
    fff = my_fopen(buf, "r");

    /* Check it */
    if (!fff) return (1);

    /* Process the file */
    while (0 == my_fgets(fff, buf, sizeof(buf)))
    {
        char *s;

        if (!buf[0]) continue;
        if (buf[0] == '#') continue;

        s = strchr(buf, '=');
        s++;
        while (!isalnum(*s)) s++;

        if (strstr(buf, "Resolution"))
        {
            screen_w = atoi(s);
            s = strchr(buf, 'x');
            screen_h = atoi(s + 1);
        }
        else if (strstr(buf, "Fullscreen"))
        {
            fullscreen = atoi(s);
        }
        else if (strstr(buf, "Graphics"))
        {
            use_graphics = atoi(s);
            if (use_graphics) arg_graphics = TRUE;
        }
#if 0
        else if (strstr(buf, "Bigtile"))
        {
            use_bigtile = atoi(s);
        }
        else if (strstr(buf, "Dbltile"))
        {
            use_dbltile = atoi(s);
        }
        else if (strstr(buf, "Trptile"))
        {
            use_trptile = atoi(s);
        }
#endif
        else if (strstr(buf, "Window"))
        {
            win = &windows[atoi(s)];
        }
        else if (strstr(buf, "Visible"))
        {
            win->visible = atoi(s);
        }
        else if (strstr(buf, "Left"))
        {
            win->left = atoi(s);
        }
        else if (strstr(buf, "Top"))
        {
            win->top = atoi(s);
        }
        else if (strstr(buf, "Width"))
        {
            win->width = atoi(s);
        }
        else if (strstr(buf, "Height"))
        {
            win->height = atoi(s);
        }
        else if (strstr(buf, "Keys"))
        {
            win->keys = atoi(s);
        }
        else if (strstr(buf, "Font"))
        {
            win->req_font = (char *)z_string_make(s);
        }
    }

    if (screen_w < 640) screen_w = 640;
    if (screen_h < 480) screen_h = 480;

    my_fclose(fff);

    return (0);
}

static errr save_prefs(void)
{
    FILE *fff;
    int i;

    /* Open the file */
    fff = my_fopen(ANGBAND_DIR_USER_SDL, "w");

    /* Check it */
    if (!fff) return (1);

    fprintf(fff, "Resolution = %dx%d\n", screen_w, screen_h);
    fprintf(fff, "Fullscreen = %d\n", fullscreen);
    fprintf(fff, "Graphics = %d\n", use_graphics);
    //fprintf(fff, "Bigtile = %d\n\n", use_bigtile);
    //fprintf(fff, "Dbltile = %d\n\n", use_dbltile);
    //fprintf(fff, "Trptile = %d\n\n", use_trptile);

    for (i = 0; i < ANGBAND_TERM_MAX; i++)
    {
        term_window *win = &windows[i];

        fprintf(fff, "Window = %d\n", i);
        fprintf(fff, "Visible = %d\n", (int)win->visible);
        fprintf(fff, "Left = %d\n", win->left);
        fprintf(fff, "Top = %d\n", win->top);
        fprintf(fff, "Width = %d\n", win->width);
        fprintf(fff, "Height = %d\n", win->height);

        fprintf(fff, "Keys = %d\n", win->keys);

        fprintf(fff, "Font = %s\n\n", win->req_font);
    }

    my_fclose(fff);

    /* Done */
    return (0);
}

static void set_update_rect(term_window *win, SDL_Rect *rc);

static void DrawSizeWidget(void)
{
    Uint32 colour = SDL_MapRGB(AppWin->format, 30, 160, 70);
    SDL_FillRect(AppWin, &SizingSpot, colour);
    SDL_UpdateRects(AppWin, 1, &SizingSpot);
}

static int Movingx;
static int Movingy;

/**
 * Is What within Range units of Origin
 */

#define closeto(Origin, What, Range) \
    ((ABS((Origin) - (What))) < (Range))

/**
 * This function keeps the 'mouse' info up to date,
 * and reacts to mouse buttons appropriately.
 */
static void sdl_HandleMouseEvent(SDL_Event *event)
{
    term_window *win;
    switch (event->type)
    {
        /* Mouse moved */
        case SDL_MOUSEMOTION:
        {
            mouse.x = event->motion.x;
            mouse.y = event->motion.y;
            win = &windows[SelectedTerm];

            /* We are moving a window */
            if (Moving)
            {
                int i;

                /* Move the window */
                win->left = (mouse.x - Movingx);
                win->top = (mouse.y - Movingy);

                /* Left bounds check */
                if (win->left < 0)
                {
                    win->left = 0;
                    Movingx = mouse.x;
                }

                /* Right bounds check */
                if ((win->left + win->width) > AppWin->w)
                {
                    win->left = AppWin->w - win->width;
                    Movingx = mouse.x - win->left;
                }

                /* Top bounds check */
                if (win->top < StatusHeight)
                {
                    win->top = StatusHeight;
                    Movingy = mouse.y - win->top;
                }

                /* Bottom bounds check */
                if ((win->top + win->height) > AppWin->h)
                {
                    win->top = AppWin->h - win->height;
                    Movingy = mouse.y - win->top;
                }

                for (i = 0; i < ANGBAND_TERM_MAX; i++)
                {
                    term_window *snapper = &windows[i];

                    /* Can't snap to self... */
                    if (i == SelectedTerm) continue;

                    /* Can't snap to the invisible */
                    if (!snapper->visible) continue;

                    /* Check the windows are across from each other */
                    if ((snapper->top < win->top + win->height) && (win->top < snapper->top + snapper->height))
                    {
                        /* Lets try to the left... */
                        if (closeto(win->left, snapper->left + snapper->width, SnapRange))
                        {
                            win->left = snapper->left + snapper->width;
                            Movingx = mouse.x - win->left;
                        }
                        /* Maybe to the right */
                        if (closeto(win->left + win->width, snapper->left, SnapRange))
                        {
                            win->left = snapper->left - win->width;
                            Movingx = mouse.x - win->left;
                        }
                    }

                    /* Check the windows are above/below each other */
                    if ((snapper->left < win->left + win->width) && (win->left < snapper->left + snapper->width))
                    {
                        /* Lets try to the top... */
                        if (closeto(win->top, snapper->top + snapper->height, SnapRange))
                        {
                            win->top = snapper->top + snapper->height;
                            Movingy = mouse.y - win->top;
                        }
                        /* Maybe to the bottom */
                        if (closeto(win->top + win->height, snapper->top, SnapRange))
                        {
                            win->top = snapper->top - win->height;
                            Movingy = mouse.y - win->top;
                        }
                    }

                }

                /* Show on the screen */
                sdl_BlitAll();
            }

            /* A window is being re-sized */
            else if (Sizing)
            {
                /* Adjust the sizing rectangle */
                SizingRect.w = win->width - win->left + (mouse.x - Movingx);
                SizingRect.h = win->height - win->top + (mouse.y - Movingy);

                /* XXX - The main window can't be too small */
                if (SelectedTerm == 0)
                {
                    int minwidth = (win->tile_wid * 80) + 2 * win->border;
                    int minheight = (win->tile_hgt * 24) + win->border + win->title_height;
                    if (SizingRect.w < minwidth)
                        SizingRect.w = minwidth;
                    if (SizingRect.h < minheight)
                        SizingRect.h = minheight;
                }

                /* Show on the screen */
                sdl_BlitAll();
            }
            else if (!popped)
            {
                /* Have a look for the corner stuff */
                if (point_in(&SizingSpot, mouse.x, mouse.y))
                {
                    if (!Sizingshow)
                    {
                        /* Indicate the hotspot */
                        Sizingshow = TRUE;
                        DrawSizeWidget();
                    }
                }
                else if (Sizingshow)
                {
                    SDL_Rect rc;
                    Sizingshow = FALSE;
                    RECT(win->width - 10, win->height - 10, 10, 10, &rc);
                    set_update_rect(win, &rc);
                    sdl_BlitWin(win);
                }
            }
            break;
        }

        /* A button has been pressed */
        case SDL_MOUSEBUTTONDOWN:
        {

            sdl_Window *window;
            bool res;
            int idx = sdl_LocateWin(mouse.x, mouse.y);

            if (event->button.button == SDL_BUTTON_LEFT)
            {
                bool just_gained_focus = FALSE;
                mouse.left = 1;
                mouse.leftx = event->button.x;
                mouse.lefty = event->button.y;

                /* Pop up window gets priority */
                if (popped) window = &PopUp; else window = &StatusBar;

                /* React to a button press */
                res = sdl_ButtonBankMouseDown(&window->buttons, mouse.x - window->left, mouse.y - window->top);

                /* If pop-up window active and no reaction then cancel the popup */
                if (popped && !res)
                {
                    RemovePopUp();
                    break;
                }

                /* Has this mouse press been handled */
                if (res) break;

                /* Is the mouse press in a term_window? */
                if (idx < 0) break;

                /* The 'focused' window has changed */
                if (idx != SelectedTerm)
                {
                    TermFocus(idx);
                    just_gained_focus = TRUE;
                }

                /* A button press has happened on the focused term window */
                win = &windows[idx];

                /* Check for mouse press in the title bar */
                if (mouse.y < win->top + win->title_height)
                {
                    /* Let's get moving */
                    Moving = TRUE;

                    //BringToTop(idx);

                    /* Remember where we started */
                    Movingx = mouse.x - win->left;
                    Movingy = mouse.y - win->top;
                }

                /* Check for the little hotspot in the botton right corner */
                else if (point_in(&SizingSpot, mouse.x, mouse.y))
                {
                    /* Let's get sizing */
                    Sizing = TRUE;

                    /* Create the sizing rectangle */
                    RECT(win->left, win->top, win->width, win->height, &SizingRect);

                    /* Remember where we started */
                    Movingx = mouse.x - win->left;
                    Movingy = mouse.y - win->top;

                }

                /* Signal a mouse press to angband (only if the window is already focused) */
                else if (!just_gained_focus)
                {
#if 0
                    if (win->visible)
                    {
                        /* Calculate the 'cell' coords */
                        int x = (mouse.x - win->left - win->border) / win->tile_wid;
                        int y = (mouse.y - win->top - win->title_height) / win->tile_hgt;

                        /* Send the mousepress to the appropriate term */
                        Term_activate(angband_term[idx]);
                        Term_mousepress(x, y, 1);
                        Term_activate(old);
                    }
#endif
                }
            }

            /* Process right mouse button */
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                mouse.right = 1;
                mouse.rightx = event->button.x;
                mouse.righty = event->button.y;

                /* Right-click always cancels the popup */
                if (popped)
                {
                    popped = FALSE;
                }
                /* See if it's inside a term_window */
                else if (idx != -1)
                {
#if 0
                    win = &windows[idx];

                    /* Calculate the 'cell' coords */
                    int x = (mouse.x - win->left - win->border) / win->tile_wid;
                    int y = (mouse.y - win->top - win->title_height) / win->tile_hgt;

                    /* Bounds check */
                    if ((x >= 0) && (y >= 0) && (x < win->cols) && (y < win->rows))
                    {
                        /* Send the mousepress to the appropriate term */
                        Term_activate(angband_term[idx]);
                        Term_mousepress(x, y, 2);
                        Term_activate(old);
                    }
#endif
                }
            }

            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            /* Handle release of left button */
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                sdl_Window *window;
                bool res;
                mouse.left = 0;

                /* Pop up window gets priority */
                if (popped) window = &PopUp; else window = &StatusBar;

                /* React to a button release */
                res = sdl_ButtonBankMouseUp(&window->buttons, mouse.x - window->left, mouse.y - window->top);

                /* Cancel popup */
                if (popped && !res)
                {
                    RemovePopUp();
                }

                /* Finish moving */
                if (Moving)
                {
                    Moving = FALSE;

                    /* Update */
                    sdl_BlitAll();
                }

                /* Finish sizing */
                if (Sizing)
                {
                    /* Sort out the window */
                    ResizeWin(&windows[SelectedTerm], SizingRect.w, SizingRect.h);
                    Sizing = FALSE;

                    /* Update */
                    sdl_BlitAll();
                }
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                mouse.right = 0;
            }
            break;
        }

    }
}

/**
 * Handle keypresses.
 *
 * We treat left and right modifier keys as equivalent.
 * We ignore any key without a valid SDLK index.
 */
static void sdl_keypress(SDL_keysym keysym)
{
    u16b key_code = keysym.unicode;
    SDLKey key_sym = keysym.sym;

    /* Store the value of various modifier keys */
    bool mc = BOOL(keysym.mod & (KMOD_CTRL));
    bool ms = BOOL(keysym.mod & (KMOD_SHIFT));
    bool ma = BOOL((keysym.mod & (KMOD_ALT)) != KMOD_NONE);
    bool mm = BOOL((keysym.mod & (KMOD_META)) != KMOD_NONE);


    /* Ignore if main term is not initialized */
    if (!Term) return;

    /* Handle print screen */
    if (key_sym == SDLK_PRINT)
    {
            //sdl_print_screen();
            return;
    }

    /* Ignore various keypress, including pure modifier keys */
    if (ignore_key[key_sym]) return;

    /*
     * If the keycode is 7-bit ASCII (except numberpad), and ALT and META are not
     * pressed, send it directly to the application.
     */
    if ((key_code) && !(key_code & (0xFF80)) && !is_numpad(key_sym) && !ma && !mm)
    {
        (void)Term_keypress(key_code);
    }

    /* Handle all other valid SDL keys */
    else if (key_sym < SDLK_LAST)
    {
        char buf[80];
        int i;

        /* Begin the macro trigger */
        (void)Term_keypress(31);

        /* Send the modifiers */
        if (mc) Term_keypress('C');
        if (ms) Term_keypress('S');
        if (ma) Term_keypress('A');
        if (mm) Term_keypress('M');

        /* Build the SDL key name */
        my_strcpy(buf, format("[%s]", SDL_GetKeyName(key_sym)), 80);

        /* Convert and store key name */
        for (i = 0; buf[i]; i++)
        {
            /* Make lowercase */
            buf[i] = tolower(buf[i]);

            /* Replace spaces with underscores */
            if (buf[i] == ' ') buf[i] = '_';

            /* Add to the keyqueue */
            (void)Term_keypress(buf[i]);
        }
    }
}


static void init_windows(void);
static void init_morewindows(void);
/**
 * Handle a single message sent to the application.
 *
 * Functions that are either called from a separate thread or which need to
 * create a separate thread (such as sounds) need to pass messages to this
 * function in order to execute most operations.  See the useage of
 * "SDL_USEREVENT".
 */
static errr sdl_HandleEvent(SDL_Event *event)
{

    /* Handle message */
    switch (event->type)
    {
        /* Keypresses */
        case SDL_KEYDOWN:
        {
            /* Handle keypress */
            sdl_keypress(event->key.keysym);

            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION:
        {
            /* Handle mouse stuff */
            sdl_HandleMouseEvent(event);
            break;
        }

        /* Shut down the game */
        /* XXX - check for stuck inside menu etc... */
        case SDL_QUIT:
        {
            /* We are playing a game with an active character */
            if (character_generated && inkey_flag)
            {
                /* Save the game */
                do_cmd_save_game(FALSE);
            }

            save_prefs();
            quit(NULL);
            break;
        }

        /* Resize the application */
        case SDL_VIDEORESIZE:
        {
            /* Free the surface */
            SDL_FreeSurface(AppWin);

            if (!fullscreen)
            {
                /* Make sure */
                vflags &= ~(SDL_FULLSCREEN);
                vflags |= SDL_RESIZABLE;

                screen_w = event->resize.w;
                screen_h = event->resize.h;

                if (screen_w < 640) screen_w = 640;
                if (screen_h < 480) screen_h = 480;

                /* Resize the application surface */
                AppWin = SDL_SetVideoMode(screen_w, screen_h, 0, vflags);
            }
            else
            {
                /* Make sure */
                vflags |= SDL_FULLSCREEN;
                vflags &= ~(SDL_RESIZABLE);

                AppWin = SDL_SetVideoMode(full_w, full_h, 0, vflags);
            }
            init_morewindows();
            init_windows();

            break;
        }

        case WINDOW_DRAW:
        {
            /* Redraw window that have asked */
            sdl_Window *window = (sdl_Window*)event->user.data1;
            sdl_WindowBlit(window);
            break;
        }
        default:
        {
            /* Do nothing */
            break;
        }
    }
    sdl_WindowUpdate(&StatusBar);
    sdl_WindowUpdate(&PopUp);
    return (0);
}

/**
 * Update the redraw rect
 * A simple but effective way to keep track of what
 * parts of a window need to updated.
 * Any new areas that are updated before a blit are simply combined
 * into a new larger rectangle to encompass all changes.
 */
static void set_update_rect(term_window *win, SDL_Rect *rc)
{
    /* No outstanding update areas yet? */
    if (win->uRect.x == -1)
    {
        /* Simple copy */
        win->uRect = *rc;
    }
    else
    {
        /* Combine the old update area with the new */
        int x = MIN(win->uRect.x, rc->x);
        int y = MIN(win->uRect.y, rc->y);
        int x2 = MAX(win->uRect.x + win->uRect.w, rc->x + rc->w);
        int y2 = MAX(win->uRect.y + win->uRect.h, rc->y + rc->h);
        RECT(x, y, x2 - x, y2 - y, &win->uRect);
    }
}

#if 0
/**
 * Given a position in the ISO Latin-1 character set, return
 * the correct character on this system.
 */
static byte Term_xchar_sdl(byte c)
{
  /* The SDL port uses the Latin-1 standard.  I hope. */
  return (c);
}
#endif

/**
 * Clear a terminal window
 */
static errr Term_xtra_sdl_clear(void)
{
    term_window *win = (term_window*)(Term->data);
    SDL_Rect rc;

    /* Oops */
    if (!win->surface) return (1);

    /* Create the fill area */
    RECT(win->border, win->title_height, win->width - (2 * win->border),
         win->height - win->border - win->title_height, &rc);


    /* Fill the rectangle */
    SDL_FillRect(win->surface, &rc, back_pixel_colour);

    /* Rectangle to update */
    set_update_rect(win, &rc);

    /* Success */
    return (0);
}

/**
 * Process at least one event
 */
static errr Term_xtra_sdl_event(int v)
{
    SDL_Event event;
    errr error = 0;

    /* Wait for an event */
    if (v)
    {
        /* Wait for an event */
        if (SDL_WaitEvent(&event))
        {
            /* Handle it */
            error = sdl_HandleEvent(&event);
        }
        else return (1);
    }

    /* Check for an event */
    else
    {
        /* Get a single pending event */
        if (SDL_PollEvent(&event))
        {
            /* Handle it */
            error = sdl_HandleEvent(&event);
        }
    }

    /* Note success or failure */
    return (error);
}

/**
 * Process all pending events
 */
static errr Term_xtra_sdl_flush(void)
{
    SDL_Event event;

    /* Get all pending events */
    while (SDL_PollEvent(&event))
    {
        /* Handle them (ignore errors) */
        (void)sdl_HandleEvent(&event);
    }

    /* Done */
    return (0);
}

/**
 * Delay for "x" milliseconds
 */
static errr Term_xtra_sdl_delay(int v)
{
    /* Sleep */
    if (v > 0)
    {
        Term_xtra_sdl_event(0);
        SDL_Delay(v);
    }

    /* Success */
    return (0);
}

static errr Term_bigcurs_sdl(int col, int row)
{
    term_window *win = (term_window*)(Term->data);

    SDL_Color colour = text_colours[TERM_YELLOW];

    SDL_Rect rc;

    /* Make a rectangle */
    RECT(col * win->tile_wid, row * win->tile_hgt, win->tile_wid * 2, win->tile_hgt, &rc);

    /* Translate it */
    rc.x += win->border;
    rc.y += win->title_height;

    /* Draw it */
    sdl_DrawBox(win->surface, &rc, colour, 1);

    /* Update area */
    set_update_rect(win, &rc);

    /* Success */
    return (0);
}

static errr Term_curs_sdl(int col, int row)
{
    term_window *win = (term_window*)(Term->data);

    SDL_Color colour = text_colours[TERM_YELLOW];

    SDL_Rect rc;

    /* Make a rectangle */
    RECT(col * win->tile_wid, row * win->tile_hgt, win->tile_wid, win->tile_hgt, &rc);

    /* Translate it */
    rc.x += win->border;
    rc.y += win->title_height;

    /* Draw it */
    sdl_DrawBox(win->surface, &rc, colour, 1);

    /* Update area */
    set_update_rect(win, &rc);

    /* Success */
    return (0);
}

#ifdef SOUND_SDL


//#include "SDL.h"
#include "SDL_mixer.h"


/* Don't cache audio */
static bool no_cache_audio = FALSE;

/* Using mp3s */
static bool use_mp3 = FALSE;
static cptr ANGBAND_DIR_XTRA_SOUND;

/* Arbitary limit on number of samples per event */
#define MAX_SAMPLES      8
#define MSG_MAX SOUND_MAX

/* Struct representing all data about an event sample */
typedef struct
{
    int num;                        /* Number of samples for this event */
    Mix_Chunk *wavs[MAX_SAMPLES];   /* Sample array */
    Mix_Music *mp3s[MAX_SAMPLES];   /* Sample array */
    char *paths[MAX_SAMPLES];       /* Relative pathnames for samples */
} sample_list;


/*
 * Just need an array of SampInfos
 */
static sample_list samples[MSG_MAX];


/*
 * Shut down the sound system and free resources.
 */
static void close_audio(void)
{
    size_t i;
    int j;

    /* Free all the sample data*/
    for (i = 0; i < MSG_MAX; i++)
    {
        sample_list *smp = &samples[i];

        /* Nuke all samples */
        for (j = 0; j < smp->num; j++)
        {
            if (use_mp3)
                Mix_FreeMusic(smp->mp3s[j]);
            else
                Mix_FreeChunk(smp->wavs[j]);
            z_string_free(smp->paths[j]);
        }
    }

    /* Close the audio */
    Mix_CloseAudio();

    /* XXX This may conflict with the SDL port */
    SDL_Quit();
}


/*
 * Initialise SDL and open the mixer
 */
static bool open_audio(void)
{
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;

    /* Initialize variables */
    audio_rate = 22050;
    audio_format = AUDIO_S16;
    audio_channels = 2;

    /* Initialize the SDL library */
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        plog_fmt("Couldn't initialize SDL: %s", SDL_GetError());
        return FALSE;
    }

    /* Try to open the audio */
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 4096) < 0)
    {
        plog_fmt("Couldn't open mixer: %s", SDL_GetError());
        return FALSE;
    }

    /* Success */
    return TRUE;
}



/*
 * Read sound.cfg and map events to sounds; then load all the sounds into
 * memory to avoid I/O latency later.
 */
static bool sound_sdl_init(bool no_cache)
{
    char path[2048];
    char buffer[2048];
    FILE *fff;


    /* Initialise the mixer  */
    if (!open_audio())
        return FALSE;


    /* Build the "sound" path */
    path_build(path, sizeof(path), ANGBAND_DIR_XTRA, "sound");
    //if (ANGBAND_DIR_XTRA_SOUND)
    //    mem_free(ANGBAND_DIR_XTRA_SOUND);
    ANGBAND_DIR_XTRA_SOUND = z_string_make(path);

    /* Find and open the config file */
    path_build(path, sizeof(path), ANGBAND_DIR_XTRA_SOUND, "sound.cfg");
    fff = my_fopen(path, "r");

    /* Handle errors */
    if (!fff)
    {
        plog_fmt("Failed to open sound config (%s):\n    %s",
                  path, strerror(errno));
        return FALSE;
    }

    /* Parse the file */
    /* Lines are always of the form "name = sample [sample ...]" */
    while (0 == my_fgets(fff, buffer, sizeof(buffer)))
    {
        char *msg_name;
        char *sample_list;
        char *search;
        char *cur_token;
        char *next_token;
        int event;

        /* Skip anything not beginning with an alphabetic character */
        if (!buffer[0] || !isalpha((unsigned char)buffer[0])) continue;

        /* Split the line into two: message name, and the rest */
        search = strchr(buffer, ' ');
        sample_list = strchr(search + 1, ' ');
        if (!search) continue;
        if (!sample_list) continue;

        /* Set the message name, and terminate at first space */
        msg_name = buffer;
        search[0] = '\0';


        /* Make sure this is a valid event name */
        for (event = MSG_MAX - 1; event >= 0; event--)
        {
            if (strcmp(msg_name, angband_sound_name[event]) == 0)
                break;
        }
        if (event < 0) continue;

        /* Advance the sample list pointer so it's at the beginning of text */
        sample_list++;
        if (!sample_list[0]) continue;

        /* Terminate the current token */
        cur_token = sample_list;
        search = strchr(cur_token, ' ');
        if (search)
        {
            search[0] = '\0';
            next_token = search + 1;
        }
        else
        {
            next_token = NULL;
        }

        /*
         * Now we find all the sample names and add them one by one
         */
        while (cur_token)
        {
            int num = samples[event].num;
            bool got_file_type = FALSE;

            /* Don't allow too many samples */
            if (num >= MAX_SAMPLES) break;

            /* Build the path to the sample */
            path_build(path, sizeof(path), ANGBAND_DIR_XTRA_SOUND, cur_token);
            if (!file_exists(path)) goto next_token;

            if (!got_file_type)
            {
                if (streq(path + strlen(path) - 3, "mp3"))
                {
                    use_mp3 = TRUE;
                    got_file_type = TRUE;
                }
            }

            /* Don't load now if we're not caching */
            if (no_cache)
            {
                /* Just save the path for later */
                samples[event].paths[num] = (char *)z_string_make(path);
            }
            else
            {
                /* Load the file now */
                if (use_mp3)
                {
                    samples[event].mp3s[num] = Mix_LoadMUS(path);
                    if (!samples[event].mp3s[num])
                    {
                        plog_fmt("%s: %s", SDL_GetError(), strerror(errno));
                        goto next_token;
                    }
                }
                else
                {
                    samples[event].wavs[num] = Mix_LoadWAV(path);
                    if (!samples[event].wavs[num])
                    {
                        plog_fmt("%s: %s", SDL_GetError(), strerror(errno));
                        goto next_token;
                    }
                }
            }

            /* Imcrement the sample count */
            samples[event].num++;

        next_token:

            /* Figure out next token */
            cur_token = next_token;
            if (next_token)
            {
                /* Try to find a space */
                search = strchr(cur_token, ' ');

                /* If we can find one, terminate, and set new "next" */
                if (search)
                {
                    search[0] = '\0';
                    next_token = search + 1;
                }
                else
                {
                    /* Otherwise prevent infinite looping */
                    next_token = NULL;
                }
            }
        }
    }

    /* Close the file */
    my_fclose(fff);


    /* Success */
    return TRUE;
}

/*
 * Play a sound of type "event".
 */
static void play_sound(int event)
{
    Mix_Chunk *wave = NULL;
    Mix_Music *mp3 = NULL;
    int s;

    /* Paranoia */
    if (event < 0 || event >= MSG_MAX) return;

    /* Check there are samples for this event */
    if (!samples[event].num) return;

    /* Choose a random event */
    s = randint0(samples[event].num);
    if (use_mp3)
        mp3 = samples[event].mp3s[s];
    else
        wave = samples[event].wavs[s];

    /* Try loading it, if it's not cached */
    if (!(wave || mp3))
    {
        /* Verify it exists */
        const char *filename = samples[event].paths[s];
        if (!file_exists(filename)) return;

        /* Load */
        if (use_mp3)
            mp3 = Mix_LoadMUS(filename);
        else
            wave = Mix_LoadWAV(filename);
    }

    /* Check to see if we have a sound again */
    if (!(wave || mp3))
    {
        plog("SDL sound load failed.");
        return;
    }

    /* Actually play the thing */
    if (use_mp3)
        Mix_PlayMusic(mp3, 1);
    else
        Mix_PlayChannel(-1, wave, 0);
}


/*
 * Init the SDL sound "module".
 */
errr init_sound_sdl(int argc, char **argv)
{
    int i;

    /* Parse args */
    for (i = 1; i < argc; i++)
    {
        if (prefix(argv[i], "-c"))
        {
            no_cache_audio = TRUE;
            plog("Audio cache disabled.");
            continue;
        }
    }

    /* Load sound preferences if requested */
    if (!sound_sdl_init(no_cache_audio))
    {
        plog("Failed to load sound config");

        /* Failure */
        return (1);
    }

    /* Enable sound */
    //sound_hook = play_sound;
    atexit(close_audio);

    /* Success */
    return (0);
}


#endif /* SOUND_SDL */
static errr Term_xtra_sdl(int n, int v)
{
    switch (n)
    {
        /* Process an event */
        case TERM_XTRA_EVENT:
        {
            return (Term_xtra_sdl_event(v));
        }
        /* Flush all events */
        case TERM_XTRA_FLUSH:
        {
            return (Term_xtra_sdl_flush());
        }
        /* Clear the screen */
        case TERM_XTRA_CLEAR:
        {
            return (Term_xtra_sdl_clear());
        }
        /* Make a special sound */
        case TERM_XTRA_SOUND:
        {
            play_sound(v);
        return (0);
        }

        /* Show or hide the cursor */
        case TERM_XTRA_SHAPE:
        {
            int x, y;

            /* Obtain the cursor */
            (void)Term_locate(&x, &y);

            /* Show or hide the cursor */
            Term_curs_sdl(x, y);
            return (0);
        }
        case TERM_XTRA_FRESH:
        {
            /* Get the current window data */
            term_window *win = (term_window*)(Term->data);

            /* Blat it! */
            sdl_BlitWin(win);

            /* Done */
            return (0);
        }
        case TERM_XTRA_DELAY:
        {
            return (Term_xtra_sdl_delay(v));
        }
        case TERM_XTRA_REACT:
        {
            int i;
            /* Re-initialize the colours */
            for (i = 0; i < MAX_COLORS; i++)
            {
                text_colours[i].r = angband_color_table[i][1];
                text_colours[i].g = angband_color_table[i][2];
                text_colours[i].b = angband_color_table[i][3];
            }
        }
    }

    return (1);
}



static errr Term_wipe_sdl(int col, int row, int n)
{
    term_window *win = (term_window*)(Term->data);
    SDL_Rect rc;

    //if (use_bigtile) n*=2;
    /* Build the area to black out */
    rc.x = col * win->tile_wid;
    rc.y = row * win->tile_hgt;
    rc.w = win->tile_wid * n;
    rc.h = win->tile_hgt;

    /* Translate it */
    rc.x += win->border;
    rc.y += win->title_height;

    /* Wipe it */
    SDL_FillRect(win->surface, &rc, back_pixel_colour);

    /* Update */
    set_update_rect(win, &rc);

    return (0);
}

/**
 * Draw some text to a window
 */
static errr Term_text_sdl(int col, int row, int n, byte a, cptr s)
{
    term_window *win = (term_window*)(Term->data);
    SDL_Color colour = text_colours[a];
    int x = col * win->tile_wid;
    int y = row * win->tile_hgt;

    /* Translate */
    x += win->border;
    y += win->title_height;

    /* Not much point really... */
    if (!win->visible) return (0);

    /* Clear the way */
    Term_wipe_sdl(col, row, n);

    /* Draw it */
    return (sdl_FontDraw(&win->font, win->surface, colour, x, y, n, s));
}

#ifdef USE_GRAPHICS
/**
 * Do a 'stretched blit'
 * SDL has no support for stretching... What a bastard!
 *
 */
static void sdl_StretchBlit(SDL_Surface *src, SDL_Rect *srcRect, SDL_Surface *dest, SDL_Rect *destRect)
{
    int x, y;
    int sx, sy, dx, dy;
    Uint8 *ps, *pd;

    for (y = 0; y < destRect->h; y++)
    {
        for (x = 0; x < destRect->w; x++)
        {
            /* Actual source coords */
            sx = (srcRect->w * x / (destRect->w)) + srcRect->x;
            sy = (srcRect->h * y / (destRect->h)) + srcRect->y;

            /* Find a source pixel */
            ps = (Uint8 *)src->pixels + (sx * src->format->BytesPerPixel) + (sy * src->pitch);
#if 0
            /* Do we need to draw it? */
            switch (src->format->BytesPerPixel)
            {
                case 1:
                {
                    if (*ps == src->format->colorkey) continue;
                    break;
                }
                case 2:
                {
                    Uint16 *ps16 = (Uint16*) ps;
                    if (*ps16 == src->format->colorkey) continue;
                    break;
                }
                case 3:
                case 4:
                {
                    Uint32 *ps32 = (Uint32*) ps;
                    if (*ps32 == src->format->colorkey) continue;
                    break;
                }
            }
#endif

            /* Actual destination pixel coords */
            dx = x + destRect->x;
            dy = y + destRect->y;

            /* Destination pixel */
            pd = (Uint8 *)dest->pixels + (dx * dest->format->BytesPerPixel) + (dy * dest->pitch);

            switch (dest->format->BytesPerPixel)
            {
                case 1:
                {
                    *pd = *ps;
                    break;
                }
                case 2:
                {
                    Uint16 *ps16 = (Uint16*) ps;
                    Uint16 *pd16 = (Uint16*) pd;
                    *pd16 = *ps16;
                    break;
                }
                case 3:
                case 4:
                {
                    Uint32 *ps32 = (Uint32*) ps;
                    Uint32 *pd32 = (Uint32*) pd;
                    *pd32 = *ps32;
                }
            }

        }
    }

}

/**
 * Make the 'pre-stretched' tiles for this window
 * Assumes the tiles surface was freed elsewhere
 */
static errr sdl_BuildTileset(term_window *win)
{
    int x, y;
    int ta, td;
    int xx, yy;
    GfxInfo *info = &GfxDesc[(int)use_graphics];

    /* Calculate the number of tiles across & down*/
    ta = GfxSurface->w / info->width;
    td = GfxSurface->h / info->height;

    /* Calculate the size of the new surface */
    x = ta * win->tile_wid;
    y = td * win->tile_hgt;
#if 0
    if (use_bigtile) x *= 2;
    if (use_dbltile)
    {
        x *= 2;
        y *= 2;
    }
    else if (use_trptile)
    {
        x *= 3;
        y *= 3;
    }
#endif
    /* Make it */
    win->tiles = SDL_CreateRGBSurface(SDL_SWSURFACE, x, y,
                                  GfxSurface->format->BitsPerPixel,
                                  GfxSurface->format->Rmask, GfxSurface->format->Gmask,
                                  GfxSurface->format->Bmask, GfxSurface->format->Amask);

    /* Bugger */
    if (!win->tiles) return (1);

    /* For every tile... */
    for (xx = 0; xx < ta; xx++)
    {
        for (yy = 0; yy < td; yy++)
        {
            SDL_Rect src, dest;
            //int dwid = (use_trptile ? (use_bigtile ? win->tile_wid * 6 : win->tile_wid * 3) : (use_dbltile ? (use_bigtile ? win->tile_wid * 4 : win->tile_wid * 2) : (use_bigtile ? win->tile_wid * 2 : win->tile_wid)));
            int dwid = win->tile_wid;

            //int dhgt = (use_trptile ? win->tile_hgt * 3 : (use_dbltile ? win->tile_hgt * 2 : win->tile_hgt));
            int dhgt = win->tile_hgt;

            /* Source rectangle (on GfxSurface) */
            RECT(xx * info->width, yy * info->height, info->width, info->height, &src);

            /* Destination rectangle (win->tiles) */
            RECT(xx * dwid, yy * dhgt, dwid, dhgt, &dest);

            /* Do the stretch thing */
            sdl_StretchBlit(GfxSurface, &src, win->tiles, &dest);
        }
    }

    /* Copy across the colour key */
    SDL_SetColorKey(win->tiles, SDL_SRCCOLORKEY, GfxSurface->format->colorkey);

    return (0);
}
#endif

/**
 * Put some gfx on the screen
 * XXX - This function _never_ seems to get called with n > 1 ?
 * This needs improvement...
 */
static errr Term_pict_sdl(int col, int row, int n, const byte *ap, const char *cp,
                          const byte *tap, const char *tcp)
{

#ifdef USE_GRAPHICS
    /* Get the right window */
    term_window *win = (term_window*)(Term->data);

    SDL_Rect rc, src;
    int i;

    /* First time a pict is requested we load the tileset in */
    if (!win->tiles)
    {
        sdl_BuildTileset(win);
        if (!win->tiles) return (1);
    }

    /* Make the destination rectangle */
    RECT(col * win->tile_wid, row * win->tile_hgt, win->tile_wid, win->tile_hgt, &rc);

    /* Translate it */
    rc.x += win->border;
    rc.y += win->title_height;

#if 0
    /* Stretch for bigtile mode */
    if (use_bigtile) rc.w *= 2;
    if (use_dbltile)
    {
        rc.w *= 2;
        rc.h *= 2;
    }
    else if (use_trptile)
    {
        rc.w *= 3;
        rc.h *= 3;
    }
#endif 
    /* Get the dimensions of the graphic surface */
    src.w = rc.w;
    src.h = rc.h;

    /* Clear the way - slightly inefficient but safe -NRM- */
    Term_wipe_sdl(col, row, n);
#if 0
        if (use_bigtile)
          Term_wipe_sdl(col + 1, row, n);
        if (use_dbltile || use_trptile)
          {
            Term_wipe_sdl(col, row + 1, n);
            Term_wipe_sdl(col + 1, row + 1, n);
            if (use_bigtile)
              {
                Term_wipe_sdl(col + 2, row, n);
                Term_wipe_sdl(col + 2, row + 1, n);
                Term_wipe_sdl(col + 3, row, n);
                Term_wipe_sdl(col + 3, row + 1, n);
              }
          }
        if (use_trptile)
          {
            Term_wipe_sdl(col, row + 2, n);
            Term_wipe_sdl(col + 1, row + 2, n);
            Term_wipe_sdl(col + 2, row + 2, n);
            if (use_bigtile)
              {
                Term_wipe_sdl(col + 4, row, n);
                Term_wipe_sdl(col + 5, row, n);
                Term_wipe_sdl(col + 4, row + 1, n);
                Term_wipe_sdl(col + 5, row + 1, n);
                Term_wipe_sdl(col + 3, row + 2, n);
                Term_wipe_sdl(col + 4, row + 2, n);
                Term_wipe_sdl(col + 5, row + 2, n);
              }
          }
#endif
    /* Blit 'em! (it) */
    for (i = 0; i < n; i++)
    {
        /* Get the terrain tile */
        src.x = (tcp[i] & 0x7F) * src.w;
        src.y = (tap[i] & 0x7F) * src.h;

        SDL_BlitSurface(win->tiles, &src, win->surface, &rc);

        /* If foreground is the same as background, we're done */
        if ((tap[i] == ap[i]) && (tcp[i] == cp[i])) continue;

        /* Get the foreground tile */
        src.x = (cp[i] & 0x7F) * src.w;
        src.y = (ap[i] & 0x7F) * src.h;

        SDL_BlitSurface(win->tiles, &src, win->surface, &rc);
    }

    /* Update area */
    set_update_rect(win, &rc);
#endif

    return (0);
}

/**
 * Create and initialize the Term contined within this window.
 */
static void term_data_link_sdl(term_window *win)
{
    term *t = &win->term_data;

    /* Initialize the term */
    term_init(t, win->cols, win->rows, win->keys);

    t->higher_pict = TRUE;

    /* Use a "software" cursor */
    t->soft_cursor = TRUE;

    /* Erase with "white space" */
    t->attr_blank = TERM_WHITE;
    t->char_blank = ' ';

    /* Never refresh one row */
    t->never_frosh = TRUE;

    /* Ignore the init/nuke hooks */

    /* Prepare the template hooks */
    t->xtra_hook = Term_xtra_sdl;
    t->curs_hook = Term_curs_sdl;
    t->bigcurs_hook = Term_bigcurs_sdl;
    t->wipe_hook = Term_wipe_sdl;
    t->text_hook = Term_text_sdl;
    t->pict_hook = Term_pict_sdl;
    //t->xchar_hook = Term_xchar_sdl;

    /* Remember where we came from */
    t->data = win;
}

/**
 * Initialize the status bar:
 *  Populate it with some buttons
 *  Set the custom draw function for the bar
 */
static void init_morewindows(void)
{
    char buf[128];
    sdl_Button *button;
    int x;
    popped = FALSE;

    /* Make sure */
    sdl_WindowFree(&PopUp);

    /* Initialize the status bar */
    sdl_WindowInit(&StatusBar, AppWin->w, StatusHeight, AppWin, DEFAULT_FONT_FILE);

    /* Cusom drawing function */
    StatusBar.draw_extra = draw_statusbar;

    AboutSelect = sdl_ButtonBankNew(&StatusBar.buttons);
    button = sdl_ButtonBankGet(&StatusBar.buttons, AboutSelect);

    my_strcpy(buf, format("%s", VERSION_NAME), sizeof(buf));

    /* Initialize the 'about' button */
    sdl_ButtonSize(button, StatusBar.font.width * strlen(buf) + 5, StatusHeight - 2);
    sdl_ButtonMove(button, 1, 1);
    sdl_ButtonVisible(button, TRUE);
    sdl_ButtonCaption(button, buf);
    button->activate = AboutActivate;


    /* New button */
    TermSelect = sdl_ButtonBankNew(&StatusBar.buttons);
    button = sdl_ButtonBankGet(&StatusBar.buttons, TermSelect);

    /* Initialize the 'term' button */
    sdl_ButtonSize(button, 120, StatusHeight - 2);
    x = 100 + (StatusBar.font.width * 5);
    sdl_ButtonMove(button, x, 1);
    sdl_ButtonVisible(button, TRUE);
    button->activate = TermActivate;

    /* Another new button */
    VisibleSelect = sdl_ButtonBankNew(&StatusBar.buttons);
    button = sdl_ButtonBankGet(&StatusBar.buttons, VisibleSelect);

    /* Initialize the 'visible' button */
    sdl_ButtonSize(button, 60,  StatusHeight - 2);
    x = 200 + (StatusBar.font.width * 8);
    sdl_ButtonMove(button, x, 1);
    sdl_ButtonVisible(button, TRUE);
    button->activate = VisibleActivate;

    /* Another new button */
    FontSelect = sdl_ButtonBankNew(&StatusBar.buttons);
    button = sdl_ButtonBankGet(&StatusBar.buttons, FontSelect);

    /* Initialize the 'font_select' button */
    sdl_ButtonSize(button, 60,  StatusHeight - 2);
    sdl_ButtonMove(button, 400, 1);
    button->activate = FontActivate;

    /* Another new button */
    MoreSelect = sdl_ButtonBankNew(&StatusBar.buttons);
    button = sdl_ButtonBankGet(&StatusBar.buttons, MoreSelect);

    /* Initialize the 'more' button */
    sdl_ButtonSize(button, 60,  StatusHeight - 2);
    sdl_ButtonMove(button, 400, 1);
    sdl_ButtonVisible(button, TRUE);
    sdl_ButtonCaption(button, "Options");
    button->activate = MoreActivate;

    /* Another new button */
    QuitSelect = sdl_ButtonBankNew(&StatusBar.buttons);
    button = sdl_ButtonBankGet(&StatusBar.buttons, QuitSelect);

    /* Initialize the 'quit' button */
    sdl_ButtonSize(button, 60,  StatusHeight - 2);
    sdl_ButtonMove(button, AppWin->w - 61, 1);
    sdl_ButtonCaption(button, "Quit");
    button->activate = QuitActivate;
    sdl_ButtonVisible(button, TRUE);

    SetStatusButtons();

    TermFocus(0);
}

#ifdef USE_GRAPHICS

/**
 * The new streamlined graphics loader.
 * Only uses colour keys.
 * Much more tolerant of different bit-planes
 */
static errr load_gfx(void)
{
    char buf[1024];
    cptr filename = GfxDesc[(int)use_graphics].gfxfile;
    SDL_Surface *temp;
    Uint8 r, g, b;
    Uint32 key;
    Uint32 Pixel = 0;
    int x = GfxDesc[(int)use_graphics].x, y = GfxDesc[(int)use_graphics].y;

    /* This may be called when GRAPHICS_NONE is set */
    if (!filename) return (0);

    /* Free the old surface */
    if (GfxSurface) SDL_FreeSurface(GfxSurface);

    /* Find and load the file into a temporary surface */
    path_build(buf, sizeof(buf), ANGBAND_DIR_XTRA_GRAF, filename);
    temp = IMG_Load(buf);

    /* Oops */
    if (!temp) return (1);

    /* Change the surface type to the current video surface format */
    GfxSurface = SDL_DisplayFormat(temp);

    /* Use a colour key */
    /* Get a pixel value depending on bit-depth */
    switch (GfxSurface->format->BytesPerPixel)
    {
        case 1:
        {
            Uint8 *p = (Uint8*)GfxSurface->pixels + x + (y * GfxSurface->pitch);
            Pixel = *p;
        }
        case 2:
        {
            Uint16 *p = (Uint16*)GfxSurface->pixels + (x * 2) + (y * GfxSurface->pitch);
            Pixel = *p;
        }
        case 3:
        case 4:
        {
            Uint32 *p = (Uint32*)GfxSurface->pixels + (x * GfxSurface->format->BytesPerPixel) + (y * GfxSurface->pitch);
            Pixel = *p;
        }
    }

    /* Get the colour values */
    SDL_GetRGB(Pixel, GfxSurface->format, &r, &g, &b);

    /* Create a key */
    key = SDL_MapRGB(GfxSurface->format, r, g, b);

    /* Set the colour key */
    SDL_SetColorKey(GfxSurface, SDL_SRCCOLORKEY, key);

    /* Lose the temporary surface */
    SDL_FreeSurface(temp);

    /* Make sure we know what pref file to use */
    ANGBAND_GRAF = GfxDesc[(int)use_graphics].pref;

    /* Reset the graphics mapping for this tileset */
    if (character_dungeon) reset_visuals();

    /* All good */
    return (0);
}
#endif

/**
 * Initialize the graphics
 */
static void init_gfx(void)
{
#ifndef USE_GRAPHICS
    /* User requested gfx but USE_GRAPHICS not defined... */
    if (arg_graphics)
    {
        arg_graphics = FALSE;
    }

    /* Make sure */
    use_graphics = GRAPHICS_NONE;
    //use_bigtile = FALSE;
    //use_dbltile = FALSE;
    //use_trptile = FALSE;
#else
    GfxInfo *info = &GfxDesc[(int)use_graphics];
    int i;

    /* Check for existence of required files */
    for (i = 0; i < GfxModes; i++)
    {
        char path[1024];

        /* Check the graphic file */
        if (GfxDesc[i].gfxfile)
        {
            path_build(path, sizeof(path), ANGBAND_DIR_XTRA_GRAF, GfxDesc[i].gfxfile);

            if (!file_exists(path))
            {
                plog_fmt("Can't find file %s - graphics mode '%s' will be disabled.", path, GfxDesc[i].name);
                GfxDesc[i].avail = FALSE;
            }
        }
    }

    /* Check availability (default to no graphics) */
    if (!info->avail)
    {
        use_graphics = GRAPHICS_NONE;
        arg_graphics = FALSE;
        //use_bigtile = FALSE;
        //use_dbltile = FALSE;
        //use_trptile = FALSE;
    }

    /* Load the graphics stuff in */
    load_gfx();
#endif
}

/**
 * Create the windows
 * Called sometime after load_prefs()
 */
static void init_windows(void)
{
    int i;

    for (i = 0; i < ANGBAND_TERM_MAX; i++)
    {
        term_window *win = &windows[i];

        /* Only bother with visible windows */
        if (win->visible)
        {
            /* Don't crowd out the status bar... */
            if (win->top < StatusHeight) win->top = StatusHeight;
#ifdef USE_GRAPHICS
            /* Invalidate the gfx surface */
            if (win->tiles)
            {
                SDL_FreeSurface(win->tiles);
                win->tiles = NULL;
            }
#endif
            /* This will set up the window correctly */
            ResizeWin(win, win->width, win->height);
        }
        else
        {
            /* Doesn't exist */
            angband_term[i] = NULL;
        }

        /* Term 0 is at the top */
        Zorder[i] = ANGBAND_TERM_MAX - i - 1;
    }

    /* Good to go... */
    Term_activate(term_screen);
}

/**
 * Set up some SDL stuff
 *
 */
static void init_sdl_local(void)
{
    const SDL_VideoInfo *VideoInfo;

    int i;
    int h, w;

    /* Get information about the video hardware */
    VideoInfo = SDL_GetVideoInfo();

    /* Require at least 256 colors */
    if (VideoInfo->vfmt->BitsPerPixel < 8) quit(format("This %s port requires lots of colors.", VERSION_NAME));

    full_w = VideoInfo->current_w;
    full_h = VideoInfo->current_h;

    /* Use a software surface - A tad inefficient, but stable... */
    vflags |= SDL_SWSURFACE;

    /* Set fullscreen flag */
    if (fullscreen) vflags |= SDL_FULLSCREEN;

    /* otherwise we make this surface resizable */
    else vflags |= SDL_RESIZABLE;

    /* Create the main window */
    AppWin = SDL_SetVideoMode(fullscreen ? full_w : screen_w,fullscreen ? full_h : screen_h, 0, vflags);

    /* Handle failure */
    if (!AppWin) quit(format("Failed to create %dx%d window at %d bpp!",
                             screen_w, screen_h, VideoInfo->vfmt->BitsPerPixel));

    /* Set the window caption */
    SDL_WM_SetCaption(VERSION_NAME, NULL);

    /* Initialize the ignored keypresses */
    for (i = 0; ignore_key_list[i]; i++)
    {
        ignore_key[ignore_key_list[i]] = TRUE;
    }

    /* Enable key repeating; use defaults
     * ... not. Those defaults are really slow! */
    (void)SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY/2, SDL_DEFAULT_REPEAT_INTERVAL/2);

    /* Enable Unicode (so we can read key codes) */
    (void)SDL_EnableUNICODE(1);

    /* Build a color for "black" that matches the pixel depth of this surface */
    back_colour.r = angband_color_table[TERM_DARK][1];
    back_colour.g = angband_color_table[TERM_DARK][2];
    back_colour.b = angband_color_table[TERM_DARK][3];
    back_pixel_colour = SDL_MapRGB(AppWin->format, back_colour.r,
                           back_colour.g, back_colour.b);

    /* Initialize the colours */
    for (i = 0; i < MAX_COLORS; i++) {
        text_colours[i].r = angband_color_table[i][1];
        text_colours[i].g = angband_color_table[i][2];
        text_colours[i].b = angband_color_table[i][3];
    }


    /* Get the height of the status bar */
    sdl_CheckFont(DEFAULT_FONT_FILE, &w, &h);
    StatusHeight = h + 3;

    /* Font used for window titles */
    sdl_FontCreate(&SystemFont, DEFAULT_FONT_FILE, AppWin);
}

DIR *my_dopen(const char *dirname)
{
    DIR *d;

    /* Try to open the directory */
    d = opendir(dirname);
    if (!d) return NULL;

    /* Success */
    return d;
}

bool my_dread(DIR *dir, char *fname, cptr dirname, size_t len)
{
    struct dirent *entry;
    struct stat filedata;
    char path[1024];

    //assert(dir != NULL);

    /* Try reading another entry */
    while (1)
    {
        entry = readdir(dir);
        if (!entry) return FALSE;

        path_build(path, sizeof path, dirname, entry->d_name);

        /* Check to see if it exists */
        if (stat(path, &filedata) != 0)
            continue;

        /* Check to see if it's a directory */
        if (S_ISDIR(filedata.st_mode))
            continue;

        /* We've found something worth returning */
        break;
    }

    /* Copy the filename */
    my_strcpy(fname, entry->d_name, len);

    return TRUE;
}

void my_dclose(DIR *dir)
{
    /* Close directory */
    if (dir)
        closedir(dir);
}

static void init_paths(void)
{
    int i;
    char path[1024];
    char buf[1024];
    DIR *dir;

    /* Build the gfx path */
    path_build(path, sizeof(path), ANGBAND_DIR_XTRA, "graf");
    ANGBAND_DIR_XTRA_GRAF = z_string_make(path);

    /* Build the "font" path */
    path_build(path, sizeof(path), ANGBAND_DIR_XTRA, "font");
    ANGBAND_DIR_XTRA_FONT = z_string_make(path);

    /* Build the filename */
    path_build(path, sizeof(path), ANGBAND_DIR_XTRA_FONT, DEFAULT_FONT_FILE);

    /* Hack -- Validate the basic font */
    validate_file(path);

    for (i = 0; i < MAX_FONTS; i++)
        FontList[i] = NULL;


    /** Scan for fonts **/

    /* Open the directory */
    dir = my_dopen(ANGBAND_DIR_XTRA_FONT);
    if (!dir) return;

    /* Read every font to the limit */
    while (my_dread(dir, buf, ANGBAND_DIR_XTRA_FONT, sizeof buf))
    {
        /* Check for file extension */
        if (suffix(buf, ".fon")||suffix(buf, ".FON"))
            FontList[num_fonts++] = (char *)z_string_make(buf);

        /* Don't grow to long */
        if (num_fonts == MAX_FONTS) break;
    }

    /* Done */
    my_dclose(dir);
}


const char help_sdl[] = "SDL frontend";

/**
 * The SDL port's main() function.
 */
int init_sdl(int argc, char *argv[])
{
    /* Initialize SDL:  Timer, video, and audio functions */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
        return (2);
    }

    /* Initialize the TTF library */
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
        SDL_Quit();
        return (2);
    }

    /* Init some extra paths */
    init_paths();

    /* Load prefs */
    load_prefs();

    /* Get sdl going */
    init_sdl_local();

    /* Prepare the windows */
    init_windows();

    /* Prepare the gfx */
    init_gfx();

    /* Prepare some more windows(!) */
    init_morewindows();

    /* Activate  quit hook */
    quit_aux = hook_quit;

    /* Paranoia */
    return (0);
}

#endif /* USE_SDL */
