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
 * @file mouse.c
 * The mouse moving component uses the Xlib API for all the dirty work.
 * @par More info about the used library:
 * - http://en.wikipedia.org/wiki/Xlib
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief The mouse moving component implementation.
 */

// C Standard Library headers
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Local C headers
#include "mouse.h"

static Display *display = NULL;

void
Mouse_Initialize(void)
{
	display = XOpenDisplay(NULL);
	assert(display);
}

void
Mouse_Deinitialize(void)
{
	assert(display);
	XCloseDisplay(display);
	display = NULL;
}

void
Mouse_Click(int button)
{
	// Create and setting up the event
	XEvent event;
	memset(&event, 0, sizeof (event));
	event.xbutton.button = button;
	event.xbutton.same_screen = True;
	event.xbutton.subwindow = DefaultRootWindow(display);
	while (event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;
		XQueryPointer(	display,
						event.xbutton.window,
						&event.xbutton.root,
						&event.xbutton.subwindow,
						&event.xbutton.x_root,
						&event.xbutton.y_root,
						&event.xbutton.x,
						&event.xbutton.y,
						&event.xbutton.state);
	}

	// Press
	event.type = ButtonPress;
	if (XSendEvent(display, PointerWindow, True, ButtonPressMask, &event) == 0)
	{
		fprintf (stderr, "Error to send the event!\n");
	}
	XSync(display, False);

	// Release
	event.type = ButtonRelease;
	if (XSendEvent(display, PointerWindow, True, ButtonReleaseMask, &event) == 0)
	{
		fprintf(stderr, "Error to send the event!\n");
	}
	XSync(display, False);
}

void
Mouse_GetCoords(int *x, int *y)
{
	assert(x);
	assert(y);

	XEvent event;
	XQueryPointer(	display,
					DefaultRootWindow(display),
					&event.xbutton.root, &event.xbutton.window,
					&event.xbutton.x_root, &event.xbutton.y_root,
					&event.xbutton.x, &event.xbutton.y,
					&event.xbutton.state);

	*x = event.xbutton.x;
	*y = event.xbutton.y;
}

void
Mouse_SetCoords(int x, int y)
{
	assert(display);

	int oy, ox;
	Mouse_GetCoords(&ox, &oy);

	XWarpPointer(display, None, None, 0, 0, 0, 0, x-ox, y-oy);
	XSync(display, False);
}
