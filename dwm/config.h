/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {"Font Awesome:size=11","adobe source code pro:size=11"};//adobe source code pro "adobe source code pro:size=11" "Font Awesome:size=11",
static const char dmenufont[]       = "Font Awesome:size=11" ;
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#ff0000";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_red ,  col_red   },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       5,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       4,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_red, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
// moje
// jasność ekranu
static const char *ht1[]  = { "brightnessctl","set","+5%", NULL };
static const char *ht2[]  = { "brightnessctl","set","5%-", NULL };
static const char *hts1[]  = { "brightnessctl","set","100%", NULL };
static const char *hts2[]  = { "brightnessctl","set","0%", NULL };
//static const char *ht3[]  = { "st","~/skrypty/.brs", NULL }; //ni działa
//dzwięk
static const char *am0[]  = { "amixer", "set","Master", "toggle", NULL };
static const char *am1[]  = { "amixer", "set","Master", "5%+", NULL };
//static const char *ams1[]  = { "amixer", "set","Master", "100%", NULL };
static const char *am2[]  = { "amixer", "set","Master", "5%-", NULL };
//static const char *ams2[]  = { "amixer", "set","Master", "0%", NULL };
//mikrofon
static const char *am3[]  = { "amixer", "set","Capture", "toggle", NULL };
static const char *am4[]  = { "amixer", "set","Capture", "5%+", NULL };
static const char *am5[]  = { "amixer", "set","Capture", "5%-", NULL };
//inne
static const char *update[]  = { "st","sudo","pacman","-Syu", NULL };
static const char *ss[]  = { "gnome-screenshot", NULL };
static const char *dolphin[]  = { "dolphin", NULL };
static const char *ff[]  = { "firefox", NULL };
static const char *wifi[]  = { "~/skrypty/wifitoggle.sh", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
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
	//moje keybindy
	
	{ MODKEY,                       XK_u,      spawn,          {.v = update} },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = dolphin} },
	{ MODKEY,                       XK_w,      spawn,          {.v = ff} },
	{ 0,                   XF86XK_RFKill,      spawn,          {.v = wifi} },
	
	{ 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = ht1 } },
	{ 0,          XF86XK_MonBrightnessDown,    spawn,          {.v = ht2 } },
	{ ShiftMask,  XF86XK_MonBrightnessUp,      spawn,          {.v = hts1 } },
	{ ShiftMask,  XF86XK_MonBrightnessDown,    spawn,          {.v = hts2 } },
	//{ 0,          XF86XK_Display,  spawn,          {.v = ht3 } },

	{ 0,          XF86XK_AudioMute,            spawn,          {.v = am0 } },
	{ 0,          XF86XK_AudioRaiseVolume,     spawn,          {.v = am1 } },
	{ 0,          XF86XK_AudioLowerVolume,     spawn,          {.v = am2 } },
	{ 0,          XF86XK_AudioMicMute,         spawn,          {.v = am3 } },
	{ ShiftMask,  XF86XK_AudioRaiseVolume,     spawn,          {.v = am4 } },
	{ ShiftMask,  XF86XK_AudioLowerVolume,     spawn,          {.v = am5 } },
	{ 0,          XK_Print,                    spawn,          {.v = ss } },
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

