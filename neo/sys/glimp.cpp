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
//hunoppc
#include <EGLSDL/SDL.h>
#include <EGLSDL/SDL_syswm.h>

#include "../sys/platform.h"
#include "../framework/Licensee.h"

#include "../renderer/tr_local.h"


static SDL_Surface *window = NULL;
#define SDL_WINDOW_OPENGL SDL_OPENGL
#define SDL_WINDOW_FULLSCREEN SDL_FULLSCREEN


static void SetSDLIcon()
{
#ifndef __amigaos4__
	Uint32 rmask, gmask, bmask, amask;

	// ok, the following is pretty stupid.. SDL_CreateRGBSurfaceFrom() pretends to use a void* for the data,
	// but it's really treated as endian-specific Uint32* ...
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	#include "doom_icon.h" // contains the struct d3_icon

	SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*)d3_icon.pixel_data, d3_icon.width, d3_icon.height,
			d3_icon.bytes_per_pixel*8, d3_icon.bytes_per_pixel*d3_icon.width,
			rmask, gmask, bmask, amask);


	SDL_WM_SetIcon(icon, NULL);

	SDL_FreeSurface(icon);
    #endif
}

/*
===================
GLimp_Init
===================
*/
bool GLimp_Init(glimpParms_t parms) {
	common->Printf("Initializing OpenGL subsystem\n");

	assert(SDL_WasInit(SDL_INIT_VIDEO));

	Uint32 flags = SDL_WINDOW_OPENGL;

	if (parms.fullScreen)
		flags |= SDL_WINDOW_FULLSCREEN;
    #ifndef __amigaos4__
	int colorbits = 24;
	int depthbits = 24;
	int stencilbits = 8;
    #else
    int colorbits = 24;
	int depthbits = 24;
	int stencilbits = 8;
    #endif


	for (int i = 0; i < 16; i++) {
		// 0 - default
		// 1 - minus colorbits
		// 2 - minus depthbits
		// 3 - minus stencil
		if ((i % 4) == 0 && i) {
			// one pass, reduce
			switch (i / 4) {
			case 2 :
				if (colorbits == 24)
					colorbits = 16;
				break;
			case 1 :
				if (depthbits == 24)
					depthbits = 16;
				else if (depthbits == 16)
					depthbits = 8;
			case 3 :
				if (stencilbits == 24)
					stencilbits = 16;
				else if (stencilbits == 16)
					stencilbits = 8;
			}
		}

		int tcolorbits = colorbits;
		int tdepthbits = depthbits;
		int tstencilbits = stencilbits;

		if ((i % 4) == 3) {
			// reduce colorbits
			if (tcolorbits == 24)
				tcolorbits = 16;
		}

		if ((i % 4) == 2) {
			// reduce depthbits
			if (tdepthbits == 24)
				tdepthbits = 16;
			else if (tdepthbits == 16)
				tdepthbits = 8;
		}

		if ((i % 4) == 1) {
			// reduce stencilbits
			if (tstencilbits == 24)
				tstencilbits = 16;
			else if (tstencilbits == 16)
				tstencilbits = 8;
			else
				tstencilbits = 0;
		}

		int channelcolorbits = 4;
		if (tcolorbits == 24)
			channelcolorbits = 8;

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, channelcolorbits);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, channelcolorbits);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, channelcolorbits);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, tdepthbits);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, tstencilbits);

		
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, channelcolorbits);

		SDL_GL_SetAttribute(SDL_GL_STEREO, parms.stereo ? 1 : 0);

		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, parms.multiSamples ? 1 : 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, parms.multiSamples);

		#ifdef __amigaos4__
        #ifdef _choice_dhewm3
        SDL_WM_SetCaption(GAME_NAME, GAME_NAME);
        #endif
        #ifdef _choice_hardcorps
        SDL_WM_SetCaption(GAME_NAME_HARDCORPS, GAME_NAME_HARDCORPS);
        #endif
        #ifdef _choice_d3xp
        SDL_WM_SetCaption(GAME_NAME_D3XP, GAME_NAME_D3XP);
        #endif
        #ifdef _choice_rivensin
        SDL_WM_SetCaption(GAME_NAME_RIVENSIN, GAME_NAME_RIVENSIN);
        #endif
        #ifdef _choice_cdoom
        SDL_WM_SetCaption(GAME_NAME_CDOOM, GAME_NAME_CDOOM);
        #endif
        #endif

		#ifndef __amigaos4__
		SetSDLIcon();
		#endif

		if (SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, r_swapInterval.GetInteger()) < 0)
			common->Warning("SDL_GL_SWAP_CONTROL not supported");

		window = SDL_SetVideoMode(parms.width, parms.height, colorbits, flags);
		if (!window) {
			common->DPrintf("Couldn't set GL mode %d/%d/%d: %s",
							channelcolorbits, tdepthbits, tstencilbits, SDL_GetError());
			continue;
		}

		glConfig.vidWidth = window->w;
		glConfig.vidHeight = window->h;

		glConfig.isFullscreen = (window->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN;

		common->Printf("Using %d color bits, %d depth, %d stencil display\n",
						channelcolorbits, tdepthbits, tstencilbits);

		glConfig.colorBits = tcolorbits;
		glConfig.depthBits = tdepthbits;
		glConfig.stencilBits = tstencilbits;

		glConfig.displayFrequency = 0;

		break;
	}

	if (!window) {
		common->Warning("No usable GL mode found: %s", SDL_GetError());
		return false;
	}

	return true;
}

/*
===================
GLimp_SetScreenParms
===================
*/
bool GLimp_SetScreenParms(glimpParms_t parms) {
	common->DPrintf("TODO: GLimp_ActivateContext\n");
	return true;
}

/*
===================
GLimp_Shutdown
===================
*/
void GLimp_Shutdown() {
	common->Printf("Shutting down OpenGL subsystem\n");
}

/*
===================
GLimp_SwapBuffers
===================
*/
void GLimp_SwapBuffers() {
	SDL_GL_SwapBuffers();

}

static bool gammaOrigError = false;
static bool gammaOrigSet = false;
static unsigned short gammaOrigRed[256];
static unsigned short gammaOrigGreen[256];
static unsigned short gammaOrigBlue[256];

/*
=================
GLimp_SetGamma
=================
*/
void GLimp_SetGamma(unsigned short red[256], unsigned short green[256], unsigned short blue[256]) {
	if (!window) {
		common->Warning("GLimp_SetGamma called without window");
		return;
	}

	if ( !gammaOrigSet ) {
		gammaOrigSet = true;
		if ( SDL_GetGammaRamp( gammaOrigRed, gammaOrigGreen, gammaOrigBlue ) == -1 ) {
			gammaOrigError = true;
			common->Warning( "Failed to get Gamma Ramp: %s\n", SDL_GetError() );
		}
	}

	if (SDL_SetGammaRamp(red, green, blue))
		common->Warning("Couldn't set gamma ramp: %s", SDL_GetError());
}

/*
=================
GLimp_ResetGamma

Restore original system gamma setting
=================
*/
void GLimp_ResetGamma() {
	if( gammaOrigError ) {
		common->Warning( "Can't reset hardware gamma because getting the Gamma Ramp at startup failed!\n" );
		common->Warning( "You might have to restart the game for gamma/brightness in shaders to work properly.\n" );
		return;
	}

	if( gammaOrigSet ) {
		SDL_SetGammaRamp( gammaOrigRed, gammaOrigGreen, gammaOrigBlue );
	}
}


/*
=================
GLimp_ActivateContext
=================
*/
void GLimp_ActivateContext() {
	common->DPrintf("TODO: GLimp_ActivateContext\n");
}

/*
=================
GLimp_DeactivateContext
=================
*/
void GLimp_DeactivateContext() {
	common->DPrintf("TODO: GLimp_DeactivateContext\n");
}

/*
===================
GLimp_ExtensionPointer
===================
*/
GLExtension_t GLimp_ExtensionPointer(const char *name) {
	assert(SDL_WasInit(SDL_INIT_VIDEO));

	return (GLExtension_t)SDL_GL_GetProcAddress(name);
}

void GLimp_GrabInput(int flags) {
	if (!window) {
		common->Warning("GLimp_GrabInput called without window");
		return;
	}
	SDL_ShowCursor( (flags & GRAB_HIDECURSOR) ? SDL_DISABLE : SDL_ENABLE );
	// ignore GRAB_GRABMOUSE, SDL1.2 doesn't support grabbing without relative mode
	// so only grab if we want relative mode
	SDL_WM_GrabInput( (flags & GRAB_RELATIVEMOUSE) ? SDL_GRAB_ON : SDL_GRAB_OFF );
}
