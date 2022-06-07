/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Krzysztof Klinikowski <kkszysiu@gmail.com>
Copyright (C) 2012 Havlena Petr <havlenapetr@gmail.com>
Copyright (C) 2022 Hugues Nouvel

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

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

HunoPPC 'hugues nouvel' just for AmigaOS4 PPC and EGL_wrap library 2022 

===========================================================================
*/
//hunoppc
#include "../sys/platform.h"
#include "../framework/Licensee.h"

#include "../renderer/tr_local.h"
#include "localAOS4.h"

#if defined(EGL_WRAP_GL_ES)
//OpenGLES2 is Here HunoPPC 2022
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "egl/egl_wrap.h"
//
#endif


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

idCVar sys_videoRam("sys_videoRam", "0", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_INTEGER,
                    "Texture memory on the video card (in megabytes) - 0: autodetect", 0, 512);

struct Window *nativeWindow = NULL;
BOOL nativeWindowOK = 1;
static EGLDisplay eglDisplay = EGL_NO_DISPLAY;
static EGLContext eglContext = EGL_NO_CONTEXT;
static EGLSurface eglSurface = EGL_NO_SURFACE;
static EGLConfig eglConfig = NULL;
static EGLint eglNumConfig = 0;

static void GLimp_AmigaOS4SetResolution(int32_t width, int32_t height)
{
	cvarSystem->SetCVarBool("r_fullscreen",  true);
	cvarSystem->SetCVarInteger("r_mode", -1);

	cvarSystem->SetCVarInteger("r_customWidth", width);
	cvarSystem->SetCVarInteger("r_customHeight", height);

	float r = (float) width / (float) height;

	if (r > 1.7f) {
		cvarSystem->SetCVarInteger("r_aspectRatio", 1);    // 16:9
	} else if (r > 1.55f) {
		cvarSystem->SetCVarInteger("r_aspectRatio", 2);    // 16:10
	} else {
		cvarSystem->SetCVarInteger("r_aspectRatio", 0);    // 4:3
	}

	Sys_Printf("r_mode(%i), r_customWidth(%i), r_customHeight(%i)\n",
	           -1, width, height);
}

void GLimp_AmigaOS4Init(struct Window *win)
{
	assert(win);
	ANativeWindow_acquire(win);
	nativeWindow = win;
    nativeWindowOK = 0;
}

void GLimp_AmigaOS4Quit()
{
	if (nativeWindowOK) {
		ANativeWindow_release(nativeWindow);
        nativeWindowOK=1;
	}
}

void GLimp_WakeBackEnd(void *a)
{
	common->DPrintf("GLimp_WakeBackEnd stub\n");
}

void GLimp_EnableLogging(bool log)
{
	//common->DPrintf("GLimp_EnableLogging stub\n");
}

void GLimp_FrontEndSleep()
{
	common->DPrintf("GLimp_FrontEndSleep stub\n");
}

void *GLimp_BackEndSleep()
{
	common->DPrintf("GLimp_BackEndSleep stub\n");
	return 0;
}

bool GLimp_SpawnRenderThread(void (*a)())
{
	common->DPrintf("GLimp_SpawnRenderThread stub\n");
	return false;
}

void GLimp_ActivateContext()
{
    //HunoPPC context for EGL_Wrap library 2022
    ESContext esContext;

	if (eglSurface == EGL_NO_SURFACE && eglContext == EGL_NO_CONTEXT) {
		Sys_Printf("Fetching EGL surface and context\n");

		EGLint context_attrs[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};

        static EGLint const pbuffer_attrs[] = {
		EGL_WIDTH, 640,
		EGL_HEIGHT, 480,
		EGL_LARGEST_PBUFFER, EGL_TRUE,
		EGL_NONE
	    };

		eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, context_attrs);
        if (eglContext == EGL_NO_CONTEXT) {
        fprintf(stderr, "failed to create Doom3 EGL context\n");
        return 1;
        }

        eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, pbuffer_attrs);
        if (eglSurface == EGL_NO_SURFACE) {
        fprintf(stderr, "failed to create Doom3 EGL surface\n");
        return 1;
        }

        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, pbuffer_attrs);
        if (eglSurface == EGL_NO_SURFACE) {
        fprintf(stderr, "failed to create Doom3 EGL Window surface\n");
        return 1;
        }

        int32_t screenWidth, screenHeight;

        if (!eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &screenWidth)) {
        fprintf(stderr, "failed to query Doom3 EGL surface width\n");
        return 1;
        }

        if (!eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &screenHeight)) {
        fprintf(stderr, "failed to query Doom3 EGL surface height\n");
        return 1;
        }

        GLimp_AmigaOS4SetResolution(screenWidth, screenHeight);

        Sys_Printf("Doom3 EGL Surface parms: width(%i), height(%i)\n", screenWidth, screenHeight);

		if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
			Sys_Error("Unable to Doom3 eglMakeCurrent\n");
			return;
		}

        glFlush();

        //initContext EGL_wrap
        esInitContext ( &esContext );
	}

	
}

void GLimp_DeactivateContext()
{
	if (eglDisplay == EGL_NO_DISPLAY) {
		return;
	}

	if (eglContext != EGL_NO_CONTEXT) {
		eglDestroyContext(eglDisplay, eglContext);
		eglContext = EGL_NO_CONTEXT;
	}
}

/*
=================
GLimp_SaveGamma

save and restore the original gamma of the system
=================
*/
void GLimp_SaveGamma()
{
}

/*
=================
GLimp_RestoreGamma

save and restore the original gamma of the system
=================
*/
void GLimp_RestoreGamma()
{
}

/*
=================
GLimp_SetGamma

gamma ramp is generated by the renderer from r_gamma and r_brightness for 256 elements
the size of the gamma ramp can not be changed on X (I need to confirm this)
=================
*/
void GLimp_SetGamma(unsigned short red[256], unsigned short green[256], unsigned short blue[256])
{
}

void GLimp_Shutdown()
{
	GLimp_DeactivateContext();

	if (eglDisplay == EGL_NO_DISPLAY) {
		return;
	}

	if (eglSurface != EGL_NO_SURFACE) {
		eglDestroySurface(eglDisplay, eglSurface);
        glFlush();
	}

	if (eglDisplay != EGL_NO_DISPLAY) {
		eglTerminate(eglDisplay);
	}

	eglDisplay = EGL_NO_DISPLAY;
	eglContext = EGL_NO_CONTEXT;
	eglSurface = EGL_NO_SURFACE;
}

void GLimp_SwapBuffers()
{
	assert(eglDisplay && eglSurface);
	eglSwapBuffers(eglDisplay, eglSurface);
}

bool GLimp_OpenDisplay(void)
{
	if (cvarSystem->GetCVarInteger("net_serverDedicated") == 1) {
		common->DPrintf("not opening the display: dedicated server\n");
		return false;
	}

	common->Printf("Setup AmigaOS4 SurfaceFlinger display connection\n");

	if (!(eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY))) {
		common->Printf("Couldn't open the Doom3 EGL display\n");
		return false;
	}

	if (!eglInitialize(eglDisplay, NULL, NULL)) {
		common->Printf("Couldn't initialize Doom3 EGL\n");
		return false;
	}

	return true;
}

/*
===============
GLES_Init
===============
*/

int GLES_Init(glimpParms_t a)
{
	EGLint attrib[] = {
		EGL_RED_SIZE, 8,	//  1,  2
		EGL_GREEN_SIZE, 8,	//  3,  4
		EGL_BLUE_SIZE, 8,	//  5,  6
		EGL_ALPHA_SIZE, 8,	//  7,  8
		EGL_DEPTH_SIZE, 24,	//  9, 10
		EGL_STENCIL_SIZE, 8,	// 11, 12
		EGL_BUFFER_SIZE, 24,	// 13, 14
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,	// 15, 16
		EGL_NONE,	// 17
	};
	// these match in the array
#define ATTR_RED_IDX	1
#define ATTR_GREEN_IDX	3
#define ATTR_BLUE_IDX	5
#define ATTR_ALPHA_IDX	7
#define ATTR_DEPTH_IDX	9
#define ATTR_STENCIL_IDX	11
#define ATTR_BUFFER_SIZE_IDX	13

	int colorbits, depthbits, stencilbits;
	int tcolorbits, tdepthbits, tstencilbits;
	int actualWidth, actualHeight;
	int i;
	const char *glstring;
	EGLint format;
    EGLint major, minor;
    

	if (!nativeWindowOK) {
		Sys_Error("Can't init, because I haven't nativeWindow!\n");
		return false;
        //here add in the future close OpenAL and other stuffs
	}

	if (!GLimp_OpenDisplay()) {
		return false;
	}

	common->Printf("Initializing EGL_Wrap display for Doom3\n");

	actualWidth = glConfig.vidWidth;
	actualHeight = glConfig.vidHeight;

	// color, depth and stencil
	colorbits = 24;
	depthbits = 24;
	stencilbits = 8;

	for (i = 0; i < 16; i++) {
		// 0 - default
		// 1 - minus colorbits
		// 2 - minus depthbits
		// 3 - minus stencil
		if ((i % 4) == 0 && i) {
			// one pass, reduce
			switch (i / 4) {
				case 2:

					if (colorbits == 24)
						colorbits = 16;

					break;
				case 1:

					if (depthbits == 24)
						depthbits = 16;
					else if (depthbits == 16)
						depthbits = 8;

				case 3:

					if (stencilbits == 24)
						stencilbits = 16;
					else if (stencilbits == 16)
						stencilbits = 8;
			}
		}

		tcolorbits = colorbits;
		tdepthbits = depthbits;
		tstencilbits = stencilbits;

		if ((i % 4) == 3) {		// reduce colorbits
			if (tcolorbits == 24)
				tcolorbits = 16;
		}

		if ((i % 4) == 2) {		// reduce depthbits
			if (tdepthbits == 24)
				tdepthbits = 16;
			else if (tdepthbits == 16)
				tdepthbits = 8;
		}

		if ((i % 4) == 1) {		// reduce stencilbits
			if (tstencilbits == 24)
				tstencilbits = 16;
			else if (tstencilbits == 16)
				tstencilbits = 8;
			else
				tstencilbits = 0;
		}

		if (tcolorbits == 24) {
			attrib[ATTR_RED_IDX] = 8;
			attrib[ATTR_GREEN_IDX] = 8;
			attrib[ATTR_BLUE_IDX] = 8;
			attrib[ATTR_BUFFER_SIZE_IDX] = 24;
		} else {
			// must be 16 bit
			attrib[ATTR_RED_IDX] = 4;
			attrib[ATTR_GREEN_IDX] = 4;
			attrib[ATTR_BLUE_IDX] = 4;
			attrib[ATTR_BUFFER_SIZE_IDX] = 16;
		}

		attrib[ATTR_DEPTH_IDX] = tdepthbits;	// default to 24 depth
		attrib[ATTR_STENCIL_IDX] = tstencilbits;

        eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
        fprintf(stderr, "no EGL display found\n");
        return 1;
    }

    if (!eglInitialize(eglDisplay, &major, &minor)) {
        fprintf(stderr, "failed to initialize EGL\n");
        return 1;
    }

        printf("Doom3 EGL: %d.%d\n", major, minor);

		eglChooseConfig(eglDisplay, attrib, &eglConfig, 1, &eglNumConfig);

		if (!eglNumConfig) {
			common->Printf("Couldn't get a EGL config 0x%04x\n", eglGetError());
			continue;
		}else{
        printf("config:%p num_config:%d\n", eglConfig, eglNumConfig);
        }

		common->Printf("Using %d/%d/%d Color bits, %d Alpha bits, %d depth, %d stencil display.\n",
		               attrib[ATTR_RED_IDX], attrib[ATTR_GREEN_IDX],
		               attrib[ATTR_BLUE_IDX], attrib[ATTR_ALPHA_IDX],
		               attrib[ATTR_DEPTH_IDX],
		               attrib[ATTR_STENCIL_IDX]);

		glConfig.colorBits = tcolorbits;
		glConfig.depthBits = tdepthbits;
		glConfig.stencilBits = tstencilbits;
		break;
	}

	eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, format);

	if (!eglNumConfig) {
		common->Printf("No acceptable EGL configurations found!\n");
		return false;
	}

	GLimp_ActivateContext();

	glstring = (const char *) glGetString(GL_RENDERER);
	common->Printf("GL_RENDERER: %s\n", glstring);

	glstring = (const char *) glGetString(GL_EXTENSIONS);
	common->Printf("GL_EXTENSIONS: %s\n", glstring);

	glConfig.isFullscreen = true;

	if (glConfig.isFullscreen) {
		Sys_GrabMouseCursor(true);
	}

	return true;
}

/*
===================
GLimp_Init

This is the platform specific OpenGL initialization function.  It
is responsible for loading OpenGL, initializing it,
creating a window of the appropriate size, doing
fullscreen manipulations, etc.  Its overall responsibility is
to make sure that a functional OpenGL subsystem is operating
when it returns to the ref.

If there is any failure, the renderer will revert back to safe
parameters and try again.
===================
*/
bool GLimp_Init(glimpParms_t a)
{

	if (!GLimp_OpenDisplay()) {
		return false;
	}

	if (!GLES_Init(a)) {
		return false;
	}

	return true;
}

/*
===================
GLimp_SetScreenParms
===================
*/
bool GLimp_SetScreenParms(glimpParms_t parms)
{
	return true;
}

/*
================
Sys_GetVideoRam
returns in megabytes
open your own display connection for the query and close it
using the one shared with GLimp_Init is not stable
================
*/
int Sys_GetVideoRam(void)
{
	static int run_once = 0;

	if (run_once) {
		return run_once;
	}

	if (sys_videoRam.GetInteger()) {
		run_once = sys_videoRam.GetInteger();
		return sys_videoRam.GetInteger();
	}

	// try a few strategies to guess the amount of video ram
	common->Printf("guessing video ram ( use +set sys_videoRam to force ) ..\n");

	if (!GLimp_OpenDisplay()) {
		run_once = 64;
		return run_once;
	}

	if (int sysRam = Sys_GetSystemRam()) {
		run_once = sysRam / 2;
		return run_once;
	}

	common->Printf("guess failed, return default low-end VRAM setting ( 64MB VRAM )\n");
	run_once = 64;
	return run_once;
}

/*
=================
GLimp_ResetGamma

Restore original system gamma setting
=================
*/
void GLimp_ResetGamma() {
//NULL
}

/*
===================
GLimp_ExtensionPointer
===================
*/
GLExtension_t GLimp_ExtensionPointer(const char *name) {
//NULL
}

void GLimp_GrabInput(int flags) {
//NULL
}
