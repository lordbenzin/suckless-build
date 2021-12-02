/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/XF86keysym.h>
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const int gappx     = 6;                 /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=14:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=10";
static const char *mutecmd[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *volupcmd[] = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *voldowncmd[] = { "pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *brupcmd[] = { "light", "-A", "5", NULL };
static const char *brdowncmd[] = {"light", "-U", "5", NULL };
static const char *mpcup_cmd[] = {"mpc", "volume", "+5", NULL };
static const char *mpcpause_cmd[] = {"mpc", "toggle", NULL };
static const char *mpcdown_cmd[] = {"mpc", "volume", "-5", NULL };
static const char *mpcnext_cmd[] = {"mpc", "next", NULL };
static const char *mpcprev_cmd[] = {"mpc", "prev", NULL };
static const char *rs1_cmd[] = {"redshift", "-P", "-O", "3500", NULL };
static const char *rs2_cmd[] = {"redshift", "-P", "-O", "2300", NULL };
static const char *rsx_cmd[] = {"redshift", "-x", NULL };
// background color
static const char col_gray1[]       = "#222222";
// inactive window border color
static const char col_gray2[]       = "#444444";
// font color
static const char col_gray3[]       = "#bbbbbb";
// current tag and window font color
static const char col_gray4[]       = "#eeeeee";
// bar foreground color
static const char col_cyan[]        = "#005577";
// active window border color
static const char col_bgsel[]        = "#b90d00";
 
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_bgsel  },
};
 

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, 
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "|M|",      centeredmaster },
        { ">M>",      centeredfloatingmaster },
	{ "[M]",      monocle },
	{ NULL,       NULL },

};

/* key definitions */
#define MODKEY Mod4Mask
#define MODKEY2 Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-hp", "dino,pcmanfm,mpv,brave,gimp,discord,screenl", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *aerccmd[]  = { "st", "-e", "aerc", NULL };
static const char *bravecmd[]  = { "brave", NULL };
static const char *ncmcmd[]  = { "st", "-e", "ncmpcpp", NULL };
static const char *newscmd[]  = { "st", "-e", "newsboat", NULL };
static const char *emojicmd[]  = { "unicode", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_a,      spawn,          {.v = aerccmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = bravecmd } },
	{ MODKEY,                       XK_n,      spawn,          {.v = ncmcmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = newscmd } },
	{ MODKEY,		        XK_Return, spawn,          {.v = termcmd } },
        { MODKEY,                       XK_uring,  togglescratch,  {.v = scratchpadcmd } },
	{ ControlMask|ShiftMask,        XK_p, spawn,          {.v = emojicmd } },
	{ MODKEY2,		        XK_comma, spawn,          {.v = mpcprev_cmd } },
	{ MODKEY2,		        XK_period, spawn,          {.v = mpcpause_cmd } },
	{ MODKEY2,		        XK_minus, spawn,          {.v = mpcnext_cmd } },
	{ MODKEY2,		        XK_minus, spawn,          {.v = mpcnext_cmd } },
	{ MODKEY2,		        XK_uring, spawn,          {.v = mpcdown_cmd } },
	{ MODKEY2,		        XK_section, spawn,          {.v = mpcup_cmd } },
	{ MODKEY2,		        XK_n, spawn,          {.v = rs1_cmd } },
	{ MODKEY2,		        XK_m, spawn,          {.v = rs2_cmd } },
	{ MODKEY2,		        XK_b, spawn,          {.v = rsx_cmd } },
	{ MODKEY2,			XK_p,  cyclelayout,    {.i = -1 } },
	{ MODKEY2,           		XK_o, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_z,      setlayout,      {.v = &layouts[4]} },
        { MODKEY|ShiftMask,             XK_z,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = +3 } },
	{ MODKEY,                       XK_period,  setgaps,        {.i = -3 } },
	{ MODKEY,	                XK_equal,  setgaps,        {.i = 0  } },
	{ 0, XF86XK_AudioMute, spawn, {.v = mutecmd } },
	{ 0, XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
	{ 0, XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
	{ 0, XF86XK_MonBrightnessUp, spawn, {.v = brupcmd} },
	{ 0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd} },
	TAGKEYS(                        XK_plus,                      0)
	TAGKEYS(                        XK_ecaron,                      1)
	TAGKEYS(                        XK_scaron,                      2)
	TAGKEYS(                        XK_ccaron,                      3)
	TAGKEYS(                        XK_rcaron,                      4)
	TAGKEYS(                        XK_zcaron,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_x,      quit,           {0} },
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

