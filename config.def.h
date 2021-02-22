/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx             = 1;	/* border pixel of windows */
static unsigned int gappx                = 10;	/* gaps between windows */
static unsigned int smarthidegaps        = 0;	/* hide gaps when only one client is present */
static unsigned int snap                 = 32;	/* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;	/* 0 means no systray */
static const int swallowfloating         = 0;	/* 1 means swallow floating windows by default */
static int showbar                       = 1;	/* 0 means no bar */
static int topbar                        = 1;	/* 0 means bottom bar */
static const int user_bh                 = 26;	/* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static int vertpad                       = 10;	/* vertical padding of bar */
static int sidepad                       = 10;	/* horizontal padding of bar */
static const char *fonts[]               = { "Droid Sans Mono Nerd Font:size=10", "monospace:size=10" };
static const char dmenufont[]            = "monospace:size=12";
static char normbgcolor[]                = "#222222";
static char normbordercolor[]            = "#444444";
static char normfgcolor[]                = "#bbbbbb";
static char selfgcolor[]                 = "#eeeeee";
static char selbordercolor[]             = "#005577";
static char selbgcolor[]                 = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
 
/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                      instance    title                    tags mask iscentered isfloating  isterminal  noswallow  monitor */
	{ "firefox",                  NULL,       NULL,                    1 << 1,   1,         0,          0,          -1,        -1 },
	{ "firefoxdeveloperedition",  NULL,       NULL,                    1 << 1,   1,         0,          0,          -1,        -1 },
	{ "LibreWolf",                NULL,       NULL,                    1 << 1,   1,         0,          0,          -1,        -1 },

	{ "st",                       NULL,       NULL,                    0,        1,         0,          1,          -1,        -1 },
	{ "St",                       NULL,       NULL,                    0,        1,         0,          1,          -1,        -1 },
	{ "URxvt",                    NULL,       NULL,                    0,        1,         0,          1,          -1,        -1 },
	{ "MEGAsync",                 NULL,       NULL,                    0,        0,         1,          0,          -1,        -1 },
	{ "libreoffice-writer",       NULL,       NULL,                    0,        1,         0,          0,          -1,        -1 },
	{ "qBittorrent",              NULL,       NULL,                    1 << 5,   1,         0,          0,          -1,        -1 },
	{ "Steam",                    NULL,       NULL,                    1 << 3,   1,         0,          0,          -1,        -1 },
	{ "FreeTube",                 NULL,       NULL,                    1 << 2,   1,         0,          0,          -1,        -1 },
	{ "discord",                  NULL,       NULL,                    0,        1,         0,          0,          -1,         1 },

	{ "KeePassXC",                NULL,       "Auto-Type - KeePassXC", -1,       1,         0,          0,          -1,        -1 },
	{ "KeePassXC",                NULL,       NULL,                    1 << 4,   1,         0,          0,          -1,        -1 },

	{ NULL,                       NULL,       "Event Tester",          0,        1,         0,          0,           1,        -1 }, /* xev */
	{ "Xephyr",                   NULL,       NULL,                    0,        1,         0,          0,           1,         1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "gaplessgrid.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      gaplessgrid },
	{ "|M|",      centeredmaster },	
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|AltMask,               KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
//static const char *dmenucmd[]   = { "dmenu_run_history", "-wd", "900", "-m", dmenumon, "-fn", dmenufont, "-nb",  normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *dmenucmd[]   = { "dmenu_run_history", "-wd", "900", "-m", dmenumon, NULL };
static const char *termcmd[]    = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "background",  STRING,  &normbgcolor },
		{ "color0",      STRING,  &normbordercolor },
		{ "foreground",  STRING,  &normfgcolor },
		{ "color6",      STRING,  &selbgcolor },
		{ "color6",      STRING,  &selbordercolor },
		{ "color0",      STRING,  &selfgcolor },
		{ "borderpx",    INTEGER, &borderpx },
		{ "snap",        INTEGER, &snap },
		{ "showbar",     INTEGER, &showbar },
		{ "topbar",      INTEGER, &topbar },
		{ "nmaster",     INTEGER, &nmaster },
		{ "resizehints", INTEGER, &resizehints },
		{ "mfact",       FLOAT,   &mfact },
};

#include "movestack.c"
#include "customkeys.h"

static Key keys[] = {
	/* modifier                     key        function        argument */
	// Terminal and dmenu
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	
	// Browser
	{ MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("$BROWSER_PRIV") },
	
	// Toggle bar
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	
	// Stack manipulation
	{ MODKEY|ControlMask,           XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	
	// No. of masters
	{ MODKEY,                       XK_s,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_s,      incnmaster,     {.i = -1 } },
	
	// Size of master
	{ MODKEY|ControlMask,           XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_l,      setmfact,       {.f = +0.05} },
	
	// Select/switch master
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY|ControlMask,           XK_space,  focusmaster,    {0} },

	// Switch to last tag
	{ MODKEY,                       XK_Tab,    view,           {0} },
	
	// Kill window
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	
	// Layouts
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_p,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_p,      setlayout,      {.v = &layouts[5]} },
	
	// Set floating flag
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	
	// All tag operations
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	
	// Monitors
	{ MODKEY,                       XK_h,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_l,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      tagmon,         {.i = +1 } },

	// Tag keys
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	// Quitting
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} },
	{ MODKEY,                       XK_Escape, spawn,		   SHCMD("dwm-exit") },
	{ MODKEY,                       XK_x,      spawn,          SHCMD("dwm-exit lock") },

	// Language
	{ MODKEY,                       XK_z,      spawn,          SHCMD("toggle_lang") },

	// Autoclicker
	{ MODKEY|ShiftMask,             XK_F1,     spawn,          SHCMD("autoclicker") },

	// Gaps
	{ MODKEY,                       XK_g,       togglegaps,    {0} },

	// Audio stuff
	{ 0,                            XK_AuPlay,     spawn,  SHCMD("playerctl play-pause") },
	{ 0,                            XK_AuPrev,     spawn,  SHCMD("playerctl previous") },
	{ 0,                            XK_AuNext,     spawn,  SHCMD("playerctl next") },
	{ 0,                            XK_AuLVolume,  spawn,  SHCMD("pactl set-sink-volume 0 -5%") },
	{ 0,                            XK_AuRVolume,  spawn,  SHCMD("pactl set-sink-volume 0 +5%") },
	{ 0,                            XK_AuMute,     spawn,  SHCMD("pactl set-sink-mute 0 toggle") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

