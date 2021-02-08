/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 7;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 7;       /* vert inner gap between windows */
static const unsigned int gappoh    = 7;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 7;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=11" };
static const char dmenufont[]       = "monospace:size=11";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor, selbgcolor,  selbordercolor  },
	[SchemeTabActive]  = { normfgcolor, normbgcolor,  normbgcolor },
	[SchemeTabInactive]  = { normfgcolor, normbgcolor,  normbgcolor }
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* grid of tags */
#define DRAWCLASSICTAGS             1 << 0
#define DRAWTAGGRID                 1 << 1

#define SWITCHTAG_UP                1 << 0
#define SWITCHTAG_DOWN              1 << 1
#define SWITCHTAG_LEFT              1 << 2
#define SWITCHTAG_RIGHT             1 << 3
#define SWITCHTAG_TOGGLETAG         1 << 4
#define SWITCHTAG_TAG               1 << 5
#define SWITCHTAG_VIEW              1 << 6
#define SWITCHTAG_TOGGLEVIEW        1 << 7

static const unsigned int drawtagmask = DRAWCLASSICTAGS; /* | DRAWCLASSICTAGS to show classic row of tags */
static const int tagrows = 1;

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

/* Bartabgroups properties */
#define BARTAB_BORDERS 0       // 0 = off, 1 = on
#define BARTAB_BOTTOMBORDER 0  // 0 = off, 1 = on
#define BARTAB_TAGSINDICATOR 0 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#define BARTAB_TAGSPX 8        // # pixels for tag grid boxes
#define BARTAB_TAGSROWS 1      // # rows in tag grid (9 tags, e.g. 3x3)
static void (*bartabmonfns[])(Monitor *) = { monocle /* , customlayoutfn */ };
static void (*bartabfloatfns[])(Monitor *) = { NULL /* , customlayoutfn */ };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "D[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "qutebrowser", NULL };
static const char *filemancmd[]  = { "st", "ranger", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = filemancmd } },
	{ Mod1Mask,                     XK_space,  spawn,          SHCMD("keymapswitch") },
	{ Mod1Mask,                     XK_F1,     spawn,          SHCMD("pamixer --toggle-mute; kill -49 $(pidof dwmblocks)") },
	{ Mod1Mask,                     XK_F2,     spawn,          SHCMD("pamixer --decrease 5; kill -49 $(pidof dwmblocks)") },
	{ Mod1Mask,                     XK_F3,     spawn,          SHCMD("pamixer --increase 5; kill -49 $(pidof dwmblocks)") },
	{ Mod1Mask|ShiftMask,           XK_F3,     spawn,          SHCMD("pamixer --set-volume 100; kill -49 $(pidof dwmblocks)") },
	{ MODKEY,             		XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    zoom,           {0} },
	//{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_BackSpace,killclient,   {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_d,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,             XK_equal,  togglegaps,     {0} },
	//{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

    //{ MODKEY,           XK_Up,     switchtag,      { .ui = SWITCHTAG_UP     | SWITCHTAG_VIEW } },
    //{ MODKEY,           XK_Down,   switchtag,      { .ui = SWITCHTAG_DOWN   | SWITCHTAG_VIEW } },
    { MODKEY,           XK_o,  switchtag,      { .ui = SWITCHTAG_RIGHT  | SWITCHTAG_VIEW } },
    { MODKEY,           XK_i,   switchtag,      { .ui = SWITCHTAG_LEFT   | SWITCHTAG_VIEW } },

    //{ MODKEY|ShiftMask,              XK_Up,     switchtag,      { .ui = SWITCHTAG_UP     | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
    //{ MODKEY|ShiftMask,              XK_Down,   switchtag,      { .ui = SWITCHTAG_DOWN   | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
    { MODKEY|ShiftMask,              XK_o,  switchtag,      { .ui = SWITCHTAG_RIGHT  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
    { MODKEY|ShiftMask,              XK_i,   switchtag,      { .ui = SWITCHTAG_LEFT   | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

