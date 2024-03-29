# ABOUT

_dhewm 3_ is a _Doom 3_ GPL source port, know to work on at least Windows, Linux, Mac OS X, FreeBSD and now! AmigaOS4.

Copyright (C) 2018-2022 Hugues Nouvel
_CODE NAME_ _dhewm 3_ AOS4 "ALWAYS ULTRA"

The goal of _dhewm 3_ is bring _DOOM 3_ with the help of SDL to all suitable
platforms.

Bugs present in the original _DOOM 3_ will be fixed (when identified) without
altering the original gameplay.

This project 

Dhewm3 for AmigaOS4 PowerPC with EGL_Wrap, Gl4es, Warp3D NOVA and Opengles2 library
- A-EON (NOVA and Opengles 2.0) http://www.a-eon.com/
- GL4es https://github.com/ptitSeb/gl4es
- EGL_Wrap library http://hunoppc.amiga-projects.net/

This distribution uses these functions of these 5 projects :

**The project is hosted at:** https://github.com/HunoPPC-NG/Dhewm3-for-EGL_Wrap-AmigaOS4

Dhewm3 for Update and support a standard code of engine  : https://github.com/dhewm

Dante OpenglES2 for a source code of GLES2 : https://github.com/omcfadde/dante

Dante's GLSL Shaders : https://github.com/omcfadde/gl2progs

Doom3 Original source code for ARB Driver : https://github.com/id-Software/DOOM-3

Gl4ES for Opengl renderer : https://github.com/ptitSeb/gl4es

# RELEASES

## Dhewm3 V1.5.3 release Named ALWAYS ULTRA AOS4(betatest) by HunoPPC (14.09.2022)

- Now ARB working fine for low machine (increase speed on LibEGL_wrap), on fullHD 40 FPS, this renderer is not supported on the future
- Now compiled with GCC Version 11 and a new SDK for AmigaOS4
- Added a new versions of compatible MODS (One individual executable per Module by HunoPPC 2022):
  * Vanilla  W.I.P
  * Edge of chaos  W.I.P
  * Grimm  W.I.P
  * Sikkmod  W.I.P
- Now a Mod "The Lost Mission"  WORKING
- Use a GLSL native on EGL_Wrap with new renderer effects W.I.P 45%
- Now on ARB2 on fullHD 20 FPS (HIGH textures and effects renderer) W.I.P 30%

## Dhewm3 V1.5.2 release Named ALWAYS ULTRA AOS4(betatest) by HunoPPC (09.07.2022)

- Added Joypad with EGLSDL v1.3 (HunoPPC 2022)
- Added Icons and installer (HunoPPC 2022)  
- Support multithreading in EGL_wrapper library for low CPU on DOOM3 now! (HunoPPC 2022)
- Support ARB2 (HunoPPC 2022)
- Optimized sound support to relieve CPU resources (HunoPPC 2022)
- Support for the full Polish version of Doom3 (c) LEM
- Gamma and Brightness are now applied in the shaders instead of by setting hardware gamma, can be disabled (so hardware gamma is used again) with r_gammaInShaders 0
- s_alReverbGain CVar to reduce EFX reverb effect intensity
- Fixes for looped sounds
- Replaced most usages of strncpy() with something safer to prevent buffer overflows (remaining cases should be safe)
- Support loading some mods known to need fs_game_base d3xp via Mods menu (currently, The Lost Mission and LibreCoop d3xp are supported)
- Disable assertion in idSampleDecoderLocal::DecodeOGG() that triggered when starting a new Classic Doom3 game
- Versions of compatible MODS (One individual executable per Module by HunoPPC 2022):
  * LibreCoop W.I.P
  * The Lost Mission  W.I.P
  * Classic Doom3 Working
  * HardCorps Working
  * Resurrection of Evil Working
  * Rivensin Working
  * Fitz Packerton  W.I.P
  * Denton's Enhanced Doom3  W.I.P
  > > See https://dhewm3.org/mods.html for more details
- dhewm3 now supports the Doom3 Demo gamedata (HunoPPC 2022) 
- Restore "Carmack's Reverse" Z-Fail stencil shadows; use glStencilOpSeparate() if available
- Scale menus, fullscreen videos and the PDA to 4:3 (with black bars left/right) on (HunoPPC 2022)
- Widescreen displays so they don't look stretched/distorted. Can be disabled with r_scaleMenusTo43 0. (HunoPPC 2022)
- Lags in starting to play a sound which for example caused the machinegun or plasmagun sounds to stutter have been eliminated
- Looping .wav sounds with leadin now work
- Added support of PNG images for additional MODS (HunoPPC 2022)
- Added support Network, now it's possible playing on NET and LOCAL networks (HunoPPC 2022)
- Added new package of Enhanced Shaders (HunoPPC 2022)
- Addition of several video modes (HunoPPC 2022):
		Mode  0: 320x240"
	    Mode  2: 512x384"
		Mode  3: 640x480"
		Mode  4: 720x405"
		Mode  5: 720x480"
		Mode  6: 720x576"
		Mode  7: 800x600"
		Mode  8: 960x540"
		Mode  9: 960x600"
		Mode  10: 960x720"
		Mode  11: 1024x576"
		Mode  12: 1024x640"
		Mode  13: 1024x768"
		Mode  14: 1152x864"
		Mode  15: 1280x720"
		Mode  16: 1280x768"
		Mode  17: 1280x960"
		Mode  18: 1280x1024"
		Mode  19: 1440x810"
		Mode  20: 1440x900"
		Mode  21: 1440x1080"
		Mode  22: 1600x900"
		Mode  23: 1600x1000"
		Mode  24: 1600x1200"
		Mode  25: 1920x1080"
		Mode  26: 1920x1200"
		Mode  27: 1920x1440"
		Mode  28: 2048x1152"
		Mode  29: 2560x1600"
		Mode  30: 3200x2400"
		Mode  31: 3840x2160"
		Mode  32: 4096x2304"
		Mode  33: 2880x1800"
		Mode  34: 2560x1440"

# CHANGES ON THIS VERSION

Compared to the original _DOOM 3_, the changes of _dhewm 3_ worth mentioning are:

- EGLSDL for low level OS support, OpenGL and input handling
- OpenAL for audio output, all OS specific audio backends are gone
- OpenAL EFX for EAX reverb effects (read: EAX-like sound effects on all platforms/hardware)
- Better support for widescreen (and arbitrary display resolutions)
- New several video modes
- New enhanced shaders
- Support multithread for future multicore with EGL_Wrap library
- Support PNG images 
- Support Joypad with EGLSDL v1.3



# GENERAL NOTES

## Game data and patching

This source release does not contain any game data, the game data is still
covered by the original EULA and must be obeyed as usual.

You must patch the game to the latest version (1.3.1). See the FAQ for details, including
how to get the game data from Steam on Linux or OSX.

Note that _Doom 3_ and _Doom 3: Resurrection of Evil_ are available from the Steam store at

http://store.steampowered.com/app/9050/

http://store.steampowered.com/app/9070/

You can also buy Steam keys at the Humble Store; currently this seems the only way to
buy a Doom3 Steam key in Germany (Note that Germans still can't buy the RoE Addon there):

https://www.humblebundle.com/store/p/doom3_storefront

https://www.humblebundle.com/store/p/doom3_resofevil_storefront

## Compiling

Required libraries are not part of the tree. These are:

- AmigaOS 4.1 Machine PowerPC
- Warp3D NOVA from A-EON
- OpenglES2 from A-EON
- EGL_Wrap library from HunoPPC 
- zlib (static)
- libcurl (static)
- libjpeg8 (static)
- libogg (static)
- libvorbis (static)
- libvorbisfile (may be part of libvorbis) (static)
- OpenAL Soft  (static)
- EGLSDL v1.3 (static)


## Back End Rendering of Stencil Shadows

The Doom 3 GPL source code release does not include functionality enabling rendering
of stencil shadows via the "depth fail" method, a functionality commonly known as
"Carmack's Reverse".

***Note*** that this **does *not* change the visual appereance** of the game.
The shadows look the same, they're just created in a slightly different way.
In theory there might be a small performance impact, but on hardware less than
ten years old it shouldn't make a difference.

## MayaImport

The code for our Maya export plugin is included, if you are a Maya licensee
you can obtain the SDK from Autodesk.


# LICENSE

See COPYING.txt for the GNU GENERAL PUBLIC LICENSE

ADDITIONAL TERMS:  The Doom 3 GPL Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU GPL which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

EXCLUDED CODE:  The code described below and contained in the Doom 3 GPL Source Code release is not part of the Program covered by the GPL and is expressly excluded from its terms.  You are solely responsible for obtaining from the copyright holder a license for such code and complying with the applicable license terms.

## PropTree

neo/tools/common/PropTree/*

Copyright (C) 1998-2001 Scott Ramsay

sramsay@gonavi.com

http://www.gonavi.com

This material is provided "as is", with absolutely no warranty expressed
or implied. Any use is at your own risk.

Permission to use or copy this software for any purpose is hereby granted
without fee, provided the above notices are retained on all copies.
Permission to modify the code and to distribute modified code is granted,
provided the above notices are retained, and a notice that the code was
modified is included with the above copyright notice.

If you use this code, drop me an email.  I'd like to know if you find the code
useful.

## Base64 implementation

neo/idlib/Base64.cpp

Copyright (c) 1996 Lars Wirzenius.  All rights reserved.

June 14 2003: TTimo <ttimo@idsoftware.com>

modified + endian bug fixes

http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=197039

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.


## IO on .zip files using minizip

src/framework/minizip/*

Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

Modifications of Unzip for Zip64
Copyright (C) 2007-2008 Even Rouault

Modifications for Zip64 support
Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

## MD4 Message-Digest Algorithm

neo/idlib/hashing/MD4.cpp

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD4 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD4 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

## MD5 Message-Digest Algorithm

neo/idlib/hashing/MD5.cpp

This code implements the MD5 message-digest algorithm.
The algorithm is due to Ron Rivest.  This code was
written by Colin Plumb in 1993, no copyright is claimed.
This code is in the public domain; do with it what you wish.

## CRC32 Checksum

neo/idlib/hashing/CRC32.cpp

Copyright (C) 1995-1998 Mark Adler

## Brandelf utility

neo/sys/linux/setup/brandelf.c

Copyright (c) 1996 Søren Schmidt
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer
   in this position and unchanged.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software withough specific prior written permission

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

$FreeBSD: src/usr.bin/brandelf/brandelf.c,v 1.16 2000/07/02 03:34:08 imp Exp $

## makeself - Make self-extractable archives on Unix

neo/sys/linux/setup/makeself/*, neo/sys/linux/setup/makeself/README
Copyright (c) Stéphane Peter
Licensing: GPL v2

