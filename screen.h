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
 * @file screen.h
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief The screen capture component API.
 */

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <X11/Xlib.h>

/**
 * @brief Initialize the screen capture component.
 * 
 * Setup the X11 & XShm extension and searches for a window
 * with the supplied name. If found, it will bring the
 * window to the front and is prepared for frame grabbing.
 * 
 * @param [in] name The name on the window that want to be captured.
 * @return The pixmap memory address of the captured window.
 * @retval NULL Unable to locate window or initialize the XShm X11 extension.
 */
XImage *
Screen_Initialize(const char *name);

/**
 * @brief Deinitialization the screen capture component.
 *
 * Free used system resources.
 */
void
Screen_Deinitialize(void);

/**
 * @brief Grabs a new (current) frame of the captured window.
 *
 * Updates the memory area with a new frame grab. Hence the previously
 * acquired pixmap pointer is now updated with a new/current frame.
 *
 * @attention A successful call to Screen_Initialize() has to be performed
 * before a call to this function.
 */
void
Screen_Get(void);

/**
 * @brief Translate local coordinates in system wide world coordinates.
 * 
 * This function translates coordinates that a local inside one window
 * into coordinates into is on your screen.
 * 
 * @param [in,out] x The x-coordinate.
 * @param [in,out] y The y-coordinate.
 */
void
Screen_TranslateCoordinates(int *x, int *y);

#endif
