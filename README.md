# ABOUT

_dhewm 3_ is a _Doom 3_ GPL source port, know to work on at least Windows, Linux, Mac OS X and FreeBSD.

The goal of _dhewm 3_ is bring _DOOM 3_ with the help of SDL to all suitable
platforms.

Bugs present in the original _DOOM 3_ will be fixed (when identified) without
altering the original gameplay.

This project 

Dhewm3 for AmigaOS4 PowerPC with EGL_Wrap, Gl4es, Warp3D NOVA and Opengles2 library
- A-EON (NOVA and Opengles 2.0) http://www.a-eon.com/
- GL4es https://github.com/ptitSeb/gl4es
- EGL_Wrap library http://hunoppc.amiga-projects.net/

This distribution uses these functions of these 4 projects :

**The project is hosted at:** https://github.com/HunoPPC-NG/Dhewm3-for-EGL_Wrap-AmigaOS4

Dhewm3 for Update and support a standard code of engine  : https://github.com/dhewm

Dante OpenglES2 for a source code of GLES2 : https://github.com/omcfadde/dante

Doom3 Original source code for arb Driver : https://github.com/id-Software/DOOM-3

Gl4ES for Opengl renderer : https://github.com/ptitSeb/gl4es



# CHANGES

Compared to the original _DOOM 3_, the changes of _dhewm 3_ worth mentioning are:

- SDL for low level OS support, OpenGL and input handling
- OpenAL for audio output, all OS specific audio backends are gone
- OpenAL EFX for EAX reverb effects (read: EAX-like sound effects on all platforms/hardware)
- Better support for widescreen (and arbitrary display resolutions)



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

The build system is based on CMake: http://cmake.org/

Required libraries are not part of the tree. These are:

-AmigaOS4 4.1
- Warp3D NOVA from A-EON
- OpenglES2 from A-EON
- EGL_Wrap library from HunoPPC 
- zlib (static)
- libjpeg (static)
- libogg (static)
- libvorbis (static)
- libvorbisfile (may be part of libvorbis) (static)
- OpenAL (OpenAL Soft required, Creative's and Apple's versions are made of fail) (static)
- SDL v1.2 (static)

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
