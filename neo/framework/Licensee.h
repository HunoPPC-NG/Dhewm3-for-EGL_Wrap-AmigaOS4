/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2022 Hugues Nouvel

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
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/*
===============================================================================

	Definitions for information that is related to a licensee's game name and location.

===============================================================================
*/

#define GAME_NAME						"Doom 3 EGL_wrap AmigaOS4"	    // appears on window titles and errors

#define ENGINE_VERSION					"Dhewm3 1.5.2 EGL_wrap AmigaOS4"    // printed in console

#define GAME_NAME_HARDCORPS				"HardCorps MOD EGL_wrap AmigaOS4"      // appears on window titles and errors

#define GAME_NAME_D3XP					"Resurrection of Evil MOD EGL_wrap AmigaOS4"      // appears on window titles and errors

#define GAME_NAME_RIVENSIN				"Rivensin MOD EGL_wrap AmigaOS4"      // appears on window titles and errors

#define GAME_NAME_CDOOM				    "Classic Doom MOD EGL_wrap AmigaOS4"      // appears on window titles and errors

// paths
#define	BASE_GAMEDIR					"base"

// filenames
#ifndef CONFIG_FILE
#define CONFIG_FILE						"doom3.cfg"
#endif

// base folder where the source code lives
#define SOURCE_CODE_BASE_FOLDER			"neo"


// default idnet host address
#ifndef IDNET_HOST
	#define IDNET_HOST					"idnet.ua-corp.com"
#endif

// default idnet master port
#ifndef IDNET_MASTER_PORT
	#define IDNET_MASTER_PORT			"27650"
#endif

// default network server port
#ifndef PORT_SERVER
	#define	PORT_SERVER					27666
#endif

// broadcast scan this many ports after PORT_SERVER so a single machine can run multiple servers
#define	NUM_SERVER_PORTS				4

// see ASYNC_PROTOCOL_VERSION
// use a different major for each game
#define ASYNC_PROTOCOL_MAJOR			1

// Savegame Version
// Update when you can no longer maintain compatibility with previous savegames
// NOTE: a seperate core savegame version and game savegame version could be useful
// 16: Doom v1.1
// 17: Doom v1.2 / D3XP. Can still read old v16 with defaults for new data
#define SAVEGAME_VERSION				17

// <= Doom v1.1: 1. no DS_VERSION token ( default )
// Doom v1.2: 2
#define RENDERDEMO_VERSION				2

// editor info
#define EDITOR_DEFAULT_PROJECT			"doom.qe4"
#define EDITOR_REGISTRY_KEY				"DOOMRadiant"
#define EDITOR_WINDOWTEXT				"DOOMEdit"

// win32 info
#define WIN32_CONSOLE_CLASS				"dhewm 3 WinConsole"

// Linux info
#define LINUX_DEFAULT_PATH				"/PROGDIR"

// AmigaOS4 info
#define AMIGAOS4_DEFAULT_PATH				"/PROGDIR"

// CD Key file info
// goes into BASE_GAMEDIR whatever the fs_game is set to
// two distinct files for easier win32 installer job
#define CDKEY_FILE						"doomkey"
#define XPKEY_FILE						"xpkey"
#define CDKEY_TEXT						"\n// Do not give this file to ANYONE.\n" \
										"// id Software or Zenimax will NEVER ask you to send this file to them.\n"

#define CONFIG_SPEC						"config.spec"
