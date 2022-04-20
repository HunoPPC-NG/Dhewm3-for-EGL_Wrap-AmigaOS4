/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.	If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
//hunoppc
#include <SDL/SDL.h>

#include "../sys/platform.h"
#include "../idlib/containers/List.h"
#include "../idlib/Heap.h"
#include "../framework/Common.h"
#include "../framework/KeyInput.h"
#include "../renderer/RenderSystem.h"
#include "../renderer/tr_local.h"

#include "../sys/sys_public.h"

#if !SDL_VERSION_ATLEAST(2, 0, 0)
#define SDL_Keycode SDLKey
#define SDLK_APPLICATION SDLK_COMPOSE
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_LGUI SDLK_LSUPER
#define SDLK_RGUI SDLK_RSUPER
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_PRINTSCREEN SDLK_PRINT
#endif

const char *kbdNames[] = {
	"english", "french", "german", "italian", "spanish", "turkish", "norwegian", "brazilian", NULL
};

idCVar in_kbd("in_kbd", "english", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_NOCHEAT, "keyboard layout", kbdNames, idCmdSystem::ArgCompletion_String<kbdNames> );

struct kbd_poll_t {
	int key;
	bool state;

	kbd_poll_t() {
	}

	kbd_poll_t(int k, bool s) {
		key = k;
		state = s;
	}
};

struct mouse_poll_t {
	int action;
	int value;

	mouse_poll_t() {
	}

	mouse_poll_t(int a, int v) {
		action = a;
		value = v;
	}
};

static idList<kbd_poll_t> kbd_polls;
static idList<mouse_poll_t> mouse_polls;

struct scancodename_t {
	int sdlScancode;
	const char* name;
};

// scancodenames[keynum - K_FIRST_SCANCODE] belongs to keynum
static scancodename_t scancodemappings[] = {
	// NOTE: must be kept in sync with the K_SC_* section of keyNum_t in framework/KeyInput.h !

#if SDL_VERSION_ATLEAST(2, 0, 0)
	#define D3_SC_MAPPING(X) { SDL_SCANCODE_ ## X , "SC_" #X }
#else // SDL1.2 doesn't have scancodes
	#define D3_SC_MAPPING(X) { 0 , "SC_" #X }
#endif

	D3_SC_MAPPING(A), // { SDL_SCANCODE_A, "SC_A" },
	D3_SC_MAPPING(B),
	D3_SC_MAPPING(C),
	D3_SC_MAPPING(D),
	D3_SC_MAPPING(E),
	D3_SC_MAPPING(F),
	D3_SC_MAPPING(G),
	D3_SC_MAPPING(H),
	D3_SC_MAPPING(I),
	D3_SC_MAPPING(J),
	D3_SC_MAPPING(K),
	D3_SC_MAPPING(L),
	D3_SC_MAPPING(M),
	D3_SC_MAPPING(N),
	D3_SC_MAPPING(O),
	D3_SC_MAPPING(P),
	D3_SC_MAPPING(Q),
	D3_SC_MAPPING(R),
	D3_SC_MAPPING(S),
	D3_SC_MAPPING(T),
	D3_SC_MAPPING(U),
	D3_SC_MAPPING(V),
	D3_SC_MAPPING(W),
	D3_SC_MAPPING(X),
	D3_SC_MAPPING(Y),
	D3_SC_MAPPING(Z),
	// leaving out SDL_SCANCODE_1 ... _0, we handle them separately already
	// also return, escape, backspace, tab, space, already handled as keycodes
	D3_SC_MAPPING(MINUS),
	D3_SC_MAPPING(EQUALS),
	D3_SC_MAPPING(LEFTBRACKET),
	D3_SC_MAPPING(RIGHTBRACKET),
	D3_SC_MAPPING(BACKSLASH),
	D3_SC_MAPPING(NONUSHASH),
	D3_SC_MAPPING(SEMICOLON),
	D3_SC_MAPPING(APOSTROPHE),
	D3_SC_MAPPING(GRAVE),
	D3_SC_MAPPING(COMMA),
	D3_SC_MAPPING(PERIOD),
	D3_SC_MAPPING(SLASH),
	// leaving out lots of key incl. from keypad, we already handle them as normal keys
	D3_SC_MAPPING(NONUSBACKSLASH),
	D3_SC_MAPPING(INTERNATIONAL1), /**< used on Asian keyboards, see footnotes in USB doc */
	D3_SC_MAPPING(INTERNATIONAL2),
	D3_SC_MAPPING(INTERNATIONAL3), /**< Yen */
	D3_SC_MAPPING(INTERNATIONAL4),
	D3_SC_MAPPING(INTERNATIONAL5),
	D3_SC_MAPPING(INTERNATIONAL6),
	D3_SC_MAPPING(INTERNATIONAL7),
	D3_SC_MAPPING(INTERNATIONAL8),
	D3_SC_MAPPING(INTERNATIONAL9),
	D3_SC_MAPPING(THOUSANDSSEPARATOR),
	D3_SC_MAPPING(DECIMALSEPARATOR),
	D3_SC_MAPPING(CURRENCYUNIT),
	D3_SC_MAPPING(CURRENCYSUBUNIT)

#undef D3_SC_MAPPING
};

// for keynums between K_FIRST_SCANCODE and K_LAST_SCANCODE
// returns e.g. "SC_A" for K_SC_A
const char* Sys_GetScancodeName( int key ) {
	if ( key >= K_FIRST_SCANCODE && key <= K_LAST_SCANCODE ) {
		int scIdx = key - K_FIRST_SCANCODE;
		return scancodemappings[scIdx].name;
	}
	return NULL;
}

#if SDL_VERSION_ATLEAST(2, 0, 0)
static bool isAscii( const char* str_ ) {
	const unsigned char* str = (const unsigned char*)str_;
	while(*str != '\0') {
		if(*str > 127) {
			return false;
		}
		++str;
	}
	return true;
}

// convert inbuf (which is expected to be in UTF-8) to outbuf (in ISO-8859-1)
static bool utf8ToISO8859_1(const char* inbuf, char* outbuf, size_t outsize) {
	if ( iconvDesc == (SDL_iconv_t)-1 ) {
		return false;
	}

	size_t outbytesleft = outsize;
	size_t inbytesleft = strlen( inbuf ) + 1; // + terminating \0
	size_t ret = SDL_iconv( iconvDesc, &inbuf, &inbytesleft, &outbuf, &outbytesleft );

	while(inbytesleft > 0) {
		switch ( ret ) {
			case SDL_ICONV_E2BIG:
				outbuf[outbytesleft-1] = '\0'; // whatever, just cut it off..
				common->DPrintf( "Cutting off UTF-8 to ISO-8859-1 conversion to '%s' because destination is too small for '%s'\n", outbuf, inbuf );
				SDL_iconv( iconvDesc, NULL, NULL, NULL, NULL ); // reset descriptor for next conversion
				return true;
			case SDL_ICONV_EILSEQ:
				// try skipping invalid input data
				++inbuf;
				--inbytesleft;
				break;
			case SDL_ICONV_EINVAL:
			case SDL_ICONV_ERROR:
				// we can't recover from this
				SDL_iconv( iconvDesc, NULL, NULL, NULL, NULL ); // reset descriptor for next conversion
				return false;
		}
	}
	SDL_iconv( iconvDesc, NULL, NULL, NULL, NULL ); // reset descriptor for next conversion
	return outbytesleft < outsize; // return false if no char was written
}
#endif // SDL2

// returns localized name of the key (between K_FIRST_SCANCODE and K_LAST_SCANCODE),
// regarding the current keyboard layout - if that name is in ASCII or corresponds
// to a "High-ASCII" char supported by Doom3.
// Otherwise return same name as Sys_GetScancodeName()
// !! Returned string is only valid until next call to this function !!
const char* Sys_GetLocalizedScancodeName( int key ) {
	if ( key >= K_FIRST_SCANCODE && key <= K_LAST_SCANCODE ) {
		int scIdx = key - K_FIRST_SCANCODE;
#if SDL_VERSION_ATLEAST(2, 0, 0)
		SDL_Scancode sc = ( SDL_Scancode ) scancodemappings[scIdx].sdlScancode;
		SDL_Keycode k = SDL_GetKeyFromScancode( sc );
		if ( k >= 0xA1 && k <= 0xFF ) {
			// luckily, the "High-ASCII" (ISO-8559-1) chars supported by Doom3
			// have the same values as the corresponding SDL_Keycodes.
			static char oneCharStr[2] = {0, 0};
			oneCharStr[0] = (unsigned char)k;
			return oneCharStr;
		} else if ( k != SDLK_UNKNOWN ) {
			const char *ret = SDL_GetKeyName( k );
			// the keyname from SDL2 is in UTF-8, which Doom3 can't print,
			// so only return the name if it's ASCII, otherwise fall back to "SC_bla"
			if ( ret && *ret != '\0' ) {
				if( isAscii( ret ) ) {
					return ret;
				}
				static char isoName[32];
				// try to convert name to ISO8859-1 (Doom3's supported "High ASCII")
				if ( utf8ToISO8859_1( ret, isoName, sizeof(isoName) ) && isoName[0] != '\0' ) {
					return isoName;
				}
			}
		}
#endif  // SDL1.2 doesn't support this, use unlocalized name (also as fallback if we couldn't get a keyname)
		return scancodemappings[scIdx].name;

	}
	return NULL;
}

// returns keyNum_t (K_SC_* constant) for given scancode name (like "SC_A")
// only makes sense to call it if name starts with "SC_" (or "sc_")
// returns -1 if not found
int Sys_GetKeynumForScancodeName( const char* name ) {
	for( int scIdx = 0; scIdx < K_NUM_SCANCODES; ++scIdx ) {
		if ( idStr::Icmp( name, scancodemappings[scIdx].name ) == 0 ) {
			return scIdx + K_FIRST_SCANCODE;
		}
	}
	return -1;
}

#if SDL_VERSION_ATLEAST(2, 0, 0)
static int getKeynumForSDLscancode( SDL_Scancode scancode ) {
	int sc = scancode;
	for ( int scIdx=0; scIdx < K_NUM_SCANCODES; ++scIdx ) {
		if ( scancodemappings[scIdx].sdlScancode == sc ) {
			return scIdx + K_FIRST_SCANCODE;
		}
	}
	return 0;
}
#endif

static byte mapkey(SDL_Keycode key) {
	switch (key) {
	case SDLK_BACKSPACE:
		return K_BACKSPACE;
	case SDLK_PAUSE:
		return K_PAUSE;
	}

	if (key <= SDLK_z)
		return key & 0xff;

	switch (key) {
	case SDLK_APPLICATION:
		return K_COMMAND;
	case SDLK_CAPSLOCK:
		return K_CAPSLOCK;
	case SDLK_SCROLLLOCK:
		return K_SCROLL;
	case SDLK_POWER:
		return K_POWER;

	case SDLK_UP:
		return K_UPARROW;
	case SDLK_DOWN:
		return K_DOWNARROW;
	case SDLK_LEFT:
		return K_LEFTARROW;
	case SDLK_RIGHT:
		return K_RIGHTARROW;

	case SDLK_LGUI:
		return K_LWIN;
	case SDLK_RGUI:
		return K_RWIN;
	case SDLK_MENU:
		return K_MENU;

	case SDLK_LALT:
	case SDLK_RALT:
		return K_ALT;
	case SDLK_RCTRL:
	case SDLK_LCTRL:
		return K_CTRL;
	case SDLK_RSHIFT:
	case SDLK_LSHIFT:
		return K_SHIFT;
	case SDLK_INSERT:
		return K_INS;
	case SDLK_DELETE:
		return K_DEL;
	case SDLK_PAGEDOWN:
		return K_PGDN;
	case SDLK_PAGEUP:
		return K_PGUP;
	case SDLK_HOME:
		return K_HOME;
	case SDLK_END:
		return K_END;

	case SDLK_F1:
		return K_F1;
	case SDLK_F2:
		return K_F2;
	case SDLK_F3:
		return K_F3;
	case SDLK_F4:
		return K_F4;
	case SDLK_F5:
		return K_F5;
	case SDLK_F6:
		return K_F6;
	case SDLK_F7:
		return K_F7;
	case SDLK_F8:
		return K_F8;
	case SDLK_F9:
		return K_F9;
	case SDLK_F10:
		return K_F10;
	case SDLK_F11:
		return K_F11;
	case SDLK_F12:
		return K_F12;
	// K_INVERTED_EXCLAMATION;
	case SDLK_F13:
		return K_F13;
	case SDLK_F14:
		return K_F14;
	case SDLK_F15:
		return K_F15;

	case SDLK_KP_7:
		return K_KP_HOME;
	case SDLK_KP_8:
		return K_KP_UPARROW;
	case SDLK_KP_9:
		return K_KP_PGUP;
	case SDLK_KP_4:
		return K_KP_LEFTARROW;
	case SDLK_KP_5:
		return K_KP_5;
	case SDLK_KP_6:
		return K_KP_RIGHTARROW;
	case SDLK_KP_1:
		return K_KP_END;
	case SDLK_KP_2:
		return K_KP_DOWNARROW;
	case SDLK_KP_3:
		return K_KP_PGDN;
	case SDLK_KP_ENTER:
		return K_KP_ENTER;
	case SDLK_KP_0:
		return K_KP_INS;
	case SDLK_KP_PERIOD:
		return K_KP_DEL;
	case SDLK_KP_DIVIDE:
		return K_KP_SLASH;
	// K_SUPERSCRIPT_TWO;
	case SDLK_KP_MINUS:
		return K_KP_MINUS;
	// K_ACUTE_ACCENT;
	case SDLK_KP_PLUS:
		return K_KP_PLUS;
	case SDLK_NUMLOCKCLEAR:
		return K_KP_NUMLOCK;
	case SDLK_KP_MULTIPLY:
		return K_KP_STAR;
	case SDLK_KP_EQUALS:
		return K_KP_EQUALS;

	// K_MASCULINE_ORDINATOR;
	// K_GRAVE_A;
	// K_AUX1;
	// K_CEDILLA_C;
	// K_GRAVE_E;
	// K_AUX2;
	// K_AUX3;
	// K_AUX4;
	// K_GRAVE_I;
	// K_AUX5;
	// K_AUX6;
	// K_AUX7;
	// K_AUX8;
	// K_TILDE_N;
	// K_GRAVE_O;
	// K_AUX9;
	// K_AUX10;
	// K_AUX11;
	// K_AUX12;
	// K_AUX13;
	// K_AUX14;
	// K_GRAVE_U;
	// K_AUX15;
	// K_AUX16;

	case SDLK_PRINTSCREEN:
		return K_PRINT_SCR;
	case SDLK_MODE:
		return K_RIGHT_ALT;
	}

	return 0;
}

static void PushConsoleEvent(const char *s) {
	char *b;
	size_t len;

	len = strlen(s) + 1;
	b = (char *)Mem_Alloc(len);
	strcpy(b, s);

	SDL_Event event;

	event.type = SDL_USEREVENT;
	event.user.code = SE_CONSOLE;
	event.user.data1 = (void *)len;
	event.user.data2 = b;

	SDL_PushEvent(&event);
}

/*
=================
Sys_InitInput
=================
*/
void Sys_InitInput() {
	kbd_polls.SetGranularity(64);
	mouse_polls.SetGranularity(64);

#if !SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#endif

	in_kbd.SetModified();
}

/*
=================
Sys_ShutdownInput
=================
*/
void Sys_ShutdownInput() {
	kbd_polls.Clear();
	mouse_polls.Clear();
}

/*
===========
Sys_InitScanTable
===========
*/
// Windows has its own version due to the tools
#ifndef _WIN32
void Sys_InitScanTable() {
}
#endif

/*
===============
Sys_GetConsoleKey
===============
*/
unsigned char Sys_GetConsoleKey(bool shifted) {
	static unsigned char keys[2] = { '`', '~' };

	if (in_kbd.IsModified()) {
		idStr lang = in_kbd.GetString();

		if (lang.Length()) {
			if (!lang.Icmp("french")) {
				keys[0] = '<';
				keys[1] = '>';
			} else if (!lang.Icmp("german")) {
				keys[0] = '^';
				keys[1] = 176; // °
			} else if (!lang.Icmp("italian")) {
				keys[0] = '\\';
				keys[1] = '|';
			} else if (!lang.Icmp("spanish")) {
				keys[0] = 186; // º
				keys[1] = 170; // ª
			} else if (!lang.Icmp("turkish")) {
				keys[0] = '"';
				keys[1] = 233; // é
			} else if (!lang.Icmp("norwegian")) {
				keys[0] = 124; // |
				keys[1] = 167; // §
			} else if (!lang.Icmp("brazilian")) {
				keys[0] = '\'';
				keys[1] = '"';
			}
		}

		in_kbd.ClearModified();
	}

	return shifted ? keys[1] : keys[0];
}

/*
===============
Sys_MapCharForKey
===============
*/
unsigned char Sys_MapCharForKey(int key) {
	return key & 0xff;
}

/*
===============
Sys_GrabMouseCursor
===============
*/
void Sys_GrabMouseCursor(bool grabIt) {
	int flags;

	if (grabIt)
		flags = GRAB_ENABLE | GRAB_HIDECURSOR | GRAB_SETSTATE;
	else
		flags = GRAB_SETSTATE;

	GLimp_GrabInput(flags);
}

/*
================
Sys_GetEvent
================
*/
sysEvent_t Sys_GetEvent() {
	SDL_Event ev;
	sysEvent_t res = { };
	byte key;

	static const sysEvent_t res_none = { SE_NONE, 0, 0, 0, NULL };

#if SDL_VERSION_ATLEAST(2, 0, 0)
	static char s[SDL_TEXTINPUTEVENT_TEXT_SIZE] = {0};
	static size_t s_pos = 0;

	if (s[0] != '\0') {
		res.evType = SE_CHAR;
		res.evValue = s[s_pos];

		++s_pos;

		if (!s[s_pos] || s_pos == SDL_TEXTINPUTEVENT_TEXT_SIZE) {
			memset(s, 0, sizeof(s));
			s_pos = 0;
		}

		return res;
	}
#endif

	static byte c = 0;

	if (c) {
		res.evType = SE_CHAR;
		res.evValue = c;

		c = 0;

		return res;
	}

	// loop until there is an event we care about (will return then) or no more events
	while(SDL_PollEvent(&ev)) {
		switch (ev.type) {
#if SDL_VERSION_ATLEAST(2, 0, 0)
		case SDL_WINDOWEVENT:
			switch (ev.window.event) {
				case SDL_WINDOWEVENT_FOCUS_GAINED: {
						// unset modifier, in case alt-tab was used to leave window and ALT is still set
						// as that can cause fullscreen-toggling when pressing enter...
						SDL_Keymod currentmod = SDL_GetModState();
					
						int newmod = KMOD_NONE;
						if (currentmod & KMOD_CAPS) // preserve capslock
							newmod |= KMOD_CAPS;

						SDL_SetModState((SDL_Keymod)newmod);
					} // new context because visual studio complains about newmod and currentmod not initialized because of the case SDL_WINDOWEVENT_FOCUS_LOST

					
					common->ActivateTool( false );
					GLimp_GrabInput(GRAB_ENABLE | GRAB_REENABLE | GRAB_HIDECURSOR); // FIXME: not sure this is still needed after the ActivateTool()-call

					// start playing the game sound world again (when coming from editor)
					session->SetPlayingSoundWorld();

					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					GLimp_GrabInput(0);
					break;
			}

			continue; // handle next event
#else
		case SDL_ACTIVEEVENT:
			{
				int flags = 0;

				if (ev.active.gain) {
					flags = GRAB_ENABLE | GRAB_REENABLE | GRAB_HIDECURSOR;

					// unset modifier, in case alt-tab was used to leave window and ALT is still set
					// as that can cause fullscreen-toggling when pressing enter...
					SDLMod currentmod = SDL_GetModState();
					int newmod = KMOD_NONE;
					if (currentmod & KMOD_CAPS) // preserve capslock
						newmod |= KMOD_CAPS;

					SDL_SetModState((SDLMod)newmod);
				}

				GLimp_GrabInput(flags);
			}

			continue; // handle next event

		case SDL_VIDEOEXPOSE:
			continue; // handle next event
#endif

		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_RETURN && (ev.key.keysym.mod & KMOD_ALT) > 0) {
				cvarSystem->SetCVarBool("r_fullscreen", !renderSystem->IsFullScreen());
				PushConsoleEvent("vid_restart");
				return res_none;
			}

			// fall through
		case SDL_KEYUP:
#if !SDL_VERSION_ATLEAST(2, 0, 0)
			key = mapkey(ev.key.keysym.sym);
			if (!key) {
				unsigned char c;
				// check if its an unmapped console key
				if (ev.key.keysym.unicode == (c = Sys_GetConsoleKey(false))) {
					key = c;
				} else if (ev.key.keysym.unicode == (c = Sys_GetConsoleKey(true))) {
					key = c;
				} else {
					if (ev.type == SDL_KEYDOWN)
						common->Warning("unmapped SDL key %d (0x%x)", ev.key.keysym.sym, ev.key.keysym.unicode);
					continue; // handle next event
				}
			}
#else
		{
			// workaround for AZERTY-keyboards, which don't have 1, 2, ..., 9, 0 in first row:
			// always map those physical keys (scancodes) to those keycodes anyway
			// see also https://bugzilla.libsdl.org/show_bug.cgi?id=3188
			SDL_Scancode sc = ev.key.keysym.scancode;
			if(sc == SDL_SCANCODE_0)
			{
				key = '0';
			}
			else if(sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_9)
			{
				// note that the SDL_SCANCODEs are SDL_SCANCODE_1, _2, ..., _9, SDL_SCANCODE_0
				// while in ASCII it's '0', '1', ..., '9' => handle 0 and 1-9 separately
				// (doom3 uses the ASCII values for those keys)
				key = '1' + (sc - SDL_SCANCODE_1);
			}
			else
			{
				key = mapkey(ev.key.keysym.sym);
			}

			if(!key) {
				if (ev.key.keysym.scancode == SDL_SCANCODE_GRAVE) { // TODO: always do this check?
					key = Sys_GetConsoleKey(true);
				} else {
					if (ev.type == SDL_KEYDOWN) {
						common->Warning("unmapped SDL key %d", ev.key.keysym.sym);
					}
					continue; // handle next event
				}
			}
		}
#endif

			res.evType = SE_KEY;
			res.evValue = key;
			res.evValue2 = ev.key.state == SDL_PRESSED ? 1 : 0;

			kbd_polls.Append(kbd_poll_t(key, ev.key.state == SDL_PRESSED));

#if SDL_VERSION_ATLEAST(2, 0, 0)
			if (key == K_BACKSPACE && ev.key.state == SDL_PRESSED)
				c = key;
#else
			if (ev.key.state == SDL_PRESSED && (ev.key.keysym.unicode & 0xff00) == 0)
				c = ev.key.keysym.unicode & 0xff;
#endif

			return res;

#if SDL_VERSION_ATLEAST(2, 0, 0)
		case SDL_TEXTINPUT:
			if (ev.text.text[0]) {
				res.evType = SE_CHAR;
				res.evValue = ev.text.text[0];

				if (ev.text.text[1] != '\0')
				{
					memcpy(s, ev.text.text, SDL_TEXTINPUTEVENT_TEXT_SIZE);
					s_pos = 1; // pos 0 is returned
				}
				return res;
			}

			continue; // handle next event

		case SDL_TEXTEDITING:
			// on windows we get this event whenever the window gains focus.. just ignore it.
			continue;
#endif

		case SDL_MOUSEMOTION:
			res.evType = SE_MOUSE;
			res.evValue = ev.motion.xrel;
			res.evValue2 = ev.motion.yrel;

			mouse_polls.Append(mouse_poll_t(M_DELTAX, ev.motion.xrel));
			mouse_polls.Append(mouse_poll_t(M_DELTAY, ev.motion.yrel));

			return res;

#if SDL_VERSION_ATLEAST(2, 0, 0)
		case SDL_MOUSEWHEEL:
			res.evType = SE_KEY;

			if (ev.wheel.y > 0) {
				res.evValue = K_MWHEELUP;
				mouse_polls.Append(mouse_poll_t(M_DELTAZ, 1));
			} else {
				res.evValue = K_MWHEELDOWN;
				mouse_polls.Append(mouse_poll_t(M_DELTAZ, -1));
			}

			res.evValue2 = 1;

			return res;
#endif

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			res.evType = SE_KEY;

			switch (ev.button.button) {
			case SDL_BUTTON_LEFT:
				res.evValue = K_MOUSE1;
				mouse_polls.Append(mouse_poll_t(M_ACTION1, ev.button.state == SDL_PRESSED ? 1 : 0));
				break;
			case SDL_BUTTON_MIDDLE:
				res.evValue = K_MOUSE3;
				mouse_polls.Append(mouse_poll_t(M_ACTION3, ev.button.state == SDL_PRESSED ? 1 : 0));
				break;
			case SDL_BUTTON_RIGHT:
				res.evValue = K_MOUSE2;
				mouse_polls.Append(mouse_poll_t(M_ACTION2, ev.button.state == SDL_PRESSED ? 1 : 0));
				break;

#if !SDL_VERSION_ATLEAST(2, 0, 0)
			case SDL_BUTTON_WHEELUP:
				res.evValue = K_MWHEELUP;
				if (ev.button.state == SDL_PRESSED)
					mouse_polls.Append(mouse_poll_t(M_DELTAZ, 1));
				break;
			case SDL_BUTTON_WHEELDOWN:
				res.evValue = K_MWHEELDOWN;
				if (ev.button.state == SDL_PRESSED)
					mouse_polls.Append(mouse_poll_t(M_DELTAZ, -1));
				break;
#endif
			default:
#if SDL_VERSION_ATLEAST(2, 0, 0)
				// handle X1 button and above
				if( ev.button.button < SDL_BUTTON_LEFT + 8 ) // doesn't support more than 8 mouse buttons
				{
					int buttonIndex = ev.button.button - SDL_BUTTON_LEFT;
					res.evValue = K_MOUSE1 + buttonIndex;
					mouse_polls.Append( mouse_poll_t( M_ACTION1 + buttonIndex, ev.button.state == SDL_PRESSED ? 1 : 0 ) );
				}
				else
#endif
				continue; // handle next event
			}

			res.evValue2 = ev.button.state == SDL_PRESSED ? 1 : 0;

			return res;

		case SDL_QUIT:
			PushConsoleEvent("quit");
			return res_none;

		case SDL_USEREVENT:
			switch (ev.user.code) {
			case SE_CONSOLE:
				res.evType = SE_CONSOLE;
				res.evPtrLength = (intptr_t)ev.user.data1;
				res.evPtr = ev.user.data2;
				return res;
			default:
				common->Warning("unknown user event %u", ev.user.code);
				continue; // handle next event
			}
		default:
			// ok, I don't /really/ care about unknown SDL events. only uncomment this for debugging.
			// common->Warning("unknown SDL event 0x%x", ev.type);
			continue; // handle next event
		}
	}

	return res_none;
}

/*
================
Sys_ClearEvents
================
*/
void Sys_ClearEvents() {
	SDL_Event ev;

	while (SDL_PollEvent(&ev))
		;

	kbd_polls.SetNum(0, false);
	mouse_polls.SetNum(0, false);
}

/*
================
Sys_GenerateEvents
================
*/
void Sys_GenerateEvents() {
	      //HunoPPC 2021
    #if !defined(__amigaos4__)
	char *s = Sys_ConsoleInput();

	if (s)
		PushConsoleEvent(s);
    #endif

	SDL_PumpEvents();
}

/*
================
Sys_PollKeyboardInputEvents
================
*/
int Sys_PollKeyboardInputEvents() {
	return kbd_polls.Num();
}

/*
================
Sys_ReturnKeyboardInputEvent
================
*/
int Sys_ReturnKeyboardInputEvent(const int n, int &key, bool &state) {
	if (n >= kbd_polls.Num())
		return 0;

	key = kbd_polls[n].key;
	state = kbd_polls[n].state;
	return 1;
}

/*
================
Sys_EndKeyboardInputEvents
================
*/
void Sys_EndKeyboardInputEvents() {
	kbd_polls.SetNum(0, false);
}

/*
================
Sys_PollMouseInputEvents
================
*/
int Sys_PollMouseInputEvents() {
	return mouse_polls.Num();
}

/*
================
Sys_ReturnMouseInputEvent
================
*/
int	Sys_ReturnMouseInputEvent(const int n, int &action, int &value) {
	if (n >= mouse_polls.Num())
		return 0;

	action = mouse_polls[n].action;
	value = mouse_polls[n].value;
	return 1;
}

/*
================
Sys_EndMouseInputEvents
================
*/
void Sys_EndMouseInputEvents() {
	mouse_polls.SetNum(0, false);
}
