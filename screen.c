/*
Copyright 2011 Marcus Stjärnås

This file is part of Grorld.

Grorld is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Grorld is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Grorld.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file screen.c
 * The screen capture component uses the Xlib API for window handling.
 * It also uses the Xlib extention called XShm (MIT-SHM) for the screen
 * grabbing because the normal Xlib API was to slow.
 * @par More info about the used libraries:
 * - http://en.wikipedia.org/wiki/Xlib
 * - http://en.wikipedia.org/wiki/MIT-SHM
 * 
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief The screen capture component implementation.
 */

// C Standard Library headers
#include <assert.h>
#include <stdio.h>
#include <string.h>

// POSIX headers
#include <sys/ipc.h>
#include <sys/shm.h>

// Xlib headers
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

// Local C headers
#include "screen.h"

static Display *display = NULL;
static XImage *buffer = NULL;

static XShmSegmentInfo shminfo;

static Window window = 0;
static XWindowAttributes window_attr;
static int window_x;
static int window_y;

/**
 * @brief Recursively find a window with the desired name.
 * @param [in] top The parent window.
 * @param [in] name The name of the desired window to return.
 * @return The image id of desired window.
 * @retval 0 Unable to find desired window.
 */
static Window
Window_WithName(Window top, const char *name)
{
	Window *children, dummy;
	unsigned int nchildren;
	Window w = 0;
	char *window_name;

	if (XFetchName(display, top, &window_name) && !strncmp(window_name, name, strlen(name)))
	{
		fprintf(stdout, "Window: %s\n", window_name);
		return top;
	}

	if (!XQueryTree(display, top, &dummy, &dummy, &children, &nchildren))
	{
		return 0;
	}

	int i;
	for (i = 0; i < nchildren; ++i)
	{
		w = Window_WithName(children[i], name);
		if (w)
		{
			break;
		}
	}

	if (children)
	{
		XFree(children);
	}

	return w;
}

XImage *
Screen_Initialize(const char *name)
{
	assert(name);

	display = XOpenDisplay(NULL);
	assert(display);

	// Retrieve selected window
	window = Window_WithName(DefaultRootWindow(display), name);
	if (window == 0)
	{
		fprintf(stderr, "Window: No window found (%s)\n", name);
		return NULL;
	}

	// Raise window to the top
	XEvent xev;
	xev.type = ClientMessage;
	xev.xclient.display = display;
	xev.xclient.window = window;
	xev.xclient.message_type = XInternAtom(display, "_NET_ACTIVE_WINDOW", 0);
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = 2L;
	xev.xclient.data.l[1] = CurrentTime;

	int res =XSendEvent(display,
						XDefaultRootWindow(display),
						0,
						SubstructureNotifyMask | SubstructureRedirectMask,
						&xev);
	assert(res != 0);

	// Retrieve window attributes (size matters...)
	res = XGetWindowAttributes(display, window, &window_attr);
	assert(res != 0);

	Window junkwin;
	XTranslateCoordinates(	display,
							window,
							window_attr.root,
							-window_attr.border_width,
							-window_attr.border_width,
							&window_x,
							&window_y,
							&junkwin);

	// Check for the XShm extension
	int ignore, major, minor;
	Bool pixmaps;
	if (XQueryExtension(display, "MIT-SHM", &ignore, &ignore, &ignore))
	{
		if (XShmQueryVersion(display, &major, &minor, &pixmaps) == True)
		{
			fprintf(stdout, "MIT-SHM: %d.%d %s shared pixmaps\n", major, minor, (pixmaps==True) ? "with" : "without");
		} else
		{
			return NULL;
		}
	} else
	{
		return NULL;
	}

	// Allocate a shared buffer
	buffer = XShmCreateImage(display, DefaultVisual(display, 0), 24, ZPixmap, NULL, &shminfo, window_attr.width, window_attr.height);
	assert(buffer);
	shminfo.shmid = shmget(IPC_PRIVATE, buffer->bytes_per_line*buffer->height, IPC_CREAT | 0777);
	assert(shminfo.shmid >= 0);

	shminfo.shmaddr = buffer->data = (char*)shmat(shminfo.shmid, 0, 0);
	shminfo.readOnly = False;

	XShmAttach(display, &shminfo);
	XSync(display, False);

	shmctl(shminfo.shmid, IPC_RMID, 0);

	return buffer;
}

void
Screen_Deinitialize(void)
{
	assert(display);
	XShmDetach(display, &shminfo);

	assert(buffer);
	XDestroyImage(buffer);
	shmdt(shminfo.shmaddr);

	XFree(display);
	display = NULL;
}

void
Screen_Get(void)
{
	assert(display);

	XShmGetImage(display, DefaultRootWindow(display), buffer, window_x, window_y, AllPlanes);
	assert(buffer);
}

void
Screen_TranslateCoordinates(int *x, int *y)
{
	assert(x);
	assert(y);
	
	Window junkwin;
	XTranslateCoordinates(	display,
							window,
							window_attr.root,
							*x,
							*y,
							x,
							y,
							&junkwin);
}
