/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 21;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Fira Code:size=11", "Symbols Nerd Font:pixelsize=14:antialias=true:autohint=true" };
static const char dmenufont[]       = "Fira Code:size=11";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = { TERMINAL, "-n", "spterm", "-g", "101x34", NULL };
const char *spcmd2[] = { TERMINAL, "-n", "spcalc", "-z", "20", "-g", "21x10", "-e", "bc", "-lq", NULL };
const char *spcmd3[] = { TERMINAL, "-n", "spmusc", "-g", "71x35", "-e", "ncmpcpp", NULL };
const char *spcmd4[] = { TERMINAL, "-n", "spytub", "-g", "101x34", "-e", "ytfzf", "-fls", "--type=all", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
	{"spmusc",      spcmd3},
	{"spytub",      spcmd4},
};

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7" };
static const char *tags[] = { "󰣇", "", "󰓌", "󰢹", "󰼪", "", "", "󱓇", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "qutebrowser",  NULL,       NULL,            1 << 1,    0,          0,          -1,        -1 },
	{ "firefox",      NULL,       NULL,            1 << 1,    0,          0,          -1,        -1 },
	{ "Gimp",         NULL,       NULL,            1 << 5,    1,          0,          -1,        -1 },
	{ "St",           NULL,       NULL,            0,         0,          1,           0,        -1 },
	{ NULL,           "spterm",   NULL,            SPTAG(0),  1,          0,          -1,        -1 },
	{ NULL,           "spcalc",   NULL,            SPTAG(1),  1,          0,          -1,        -1 },
	{ NULL,           "spmusc",   NULL,            SPTAG(2),  1,          0,          -1,        -1 },
	{ NULL,           "spytub",   NULL,            SPTAG(3),  1,          0,          -1,        -1 },
	{ NULL,           NULL,       "Event Tester",  0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier                     key                function        argument */
	{ MODKEY,                       XK_l,              setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_k,              focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_j,              focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,              setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_g,              togglebar,      {0} },
	{ MODKEY,                       XK_f,	           zoom,           {0} },
	{ MODKEY,                       XK_d,              incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_s,              incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_q,              killclient,     {0} },
	{ MODKEY,                       XK_w,              setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_e,              setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_r,              setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_y,              setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_y,              setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_o,  	           togglefloating, {0} },
	{ MODKEY,                       XK_t,  	           setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_b,              spawn,          {.v = (const char*[]){ "setwp", NULL } } },
	{ MODKEY,                       XK_backslash,      spawn,          {.v = (const char*[]){ "emacs", NULL } } },
	{ MODKEY|ShiftMask,             XK_backslash,      spawn,          {.v = (const char*[]){ "wallp", NULL } } },
	{ MODKEY,                       XK_b,              spawn,          {.v = (const char*[]){ "pskill", NULL } } },
	{ MODKEY,                       XK_o,              spawn,          {.v = (const char*[]){ "mpc", "toggle", NULL } } },
	{ MODKEY,                       XK_o,              spawn,          {.v = (const char*[]){ "pkill", "-SIGRTMIN+11", "dwmblocks", NULL } } },
	{ MODKEY,                       XK_p,              spawn,          {.v = (const char*[]){ "mpc", "prev", NULL } } },
	{ MODKEY|ShiftMask,             XK_p,              spawn,          {.v = (const char*[]){ "mpc", "seek", "0%", NULL } } },
	{ MODKEY,                       XK_n,              spawn,          {.v = (const char*[]){ "mpc", "next", NULL } } },
	{ MODKEY|ShiftMask,             XK_n,              spawn,          {.v = (const char*[]){ "clipmenu", NULL } } },
	{ MODKEY|ShiftMask,             XK_a,              spawn,          {.v = (const char*[]){ TERMINAL, "-e", "pulsemixer", NULL } } },
	{ MODKEY,                       XK_grave,          spawn,          {.v = (const char*[]){ TERMINAL, "-e", "cava", NULL } } },
	{ MODKEY,                       XK_i,              spawn,          {.v = (const char*[]){ "firefox", NULL } } },
	{ MODKEY|ShiftMask,             XK_i,              spawn,          {.v = (const char*[]){ "qutebrowser", NULL } } },
	{ MODKEY|ShiftMask,             XK_m,              spawn,          {.v = (const char*[]){ "emog", NULL } } },
/*	{ MODKEY,                       XK_F12,            spawn,          {.v = (const char*[]){"sudo", "brightnessctl", "s", "+5%", NULL } } },
	{ MODKEY,                       XK_F11,            spawn,          {.v = (const char*[]){"sudo", "brightnessctl", "s", "5%-", NULL } } },*/
/*	{ MODKEY,                       XK_F9,             spawn,          {.v = (const char*[]){ "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL } } },
	{ MODKEY,                       XK_F11,            spawn,          {.v = (const char*[]){ "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL } } },
  	{ MODKEY,                       XK_F12,            spawn,          {.v = (const char*[]){ "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL } } },*/
	{ MODKEY|Mod1Mask,              XK_Return,         spawn,          {.v = (const char*[]){ "tabbed", "-cr", "2", "st", "-w", "''", NULL } } },
	{ MODKEY|ShiftMask,             XK_Return,         togglescratch,  {.ui = 0 } },
	{ MODKEY,                       XK_apostrophe,     togglescratch,  {.ui = 1 } },
	{ MODKEY,                       XK_m,              togglescratch,  {.ui = 2 } },
	{ MODKEY|ShiftMask,             XK_grave,          togglescratch,  {.ui = 3 } },
	{ MODKEY,                       XK_minus,          setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,          setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,          setgaps,        {.i = 0  } },
	{ MODKEY,                       XK_space,          spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,         spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_BackSpace,      spawn,          {.v = (const char*[]){ "uxterm", NULL } } },
	{ MODKEY,                       XK_Tab,            view,           {0} },
	{ MODKEY|Mod1Mask,              XK_0,              view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,              tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,          focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,         focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,          tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,         tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,             xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                              0)
	TAGKEYS(                        XK_2,                              1)
	TAGKEYS(                        XK_3,                              2)
	TAGKEYS(                        XK_4,                              3)
	TAGKEYS(                        XK_5,                              4)
	TAGKEYS(                        XK_6,                              5)
	TAGKEYS(                        XK_7,                              6)
	TAGKEYS(                        XK_8,                              7)
	TAGKEYS(                        XK_9,                              8)
	TAGKEYS(                        XK_0,                              9)
	{ MODKEY|ShiftMask,             XK_q,              quit,           {0} },
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

