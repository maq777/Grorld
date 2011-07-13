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
 * @file mouse.h
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief The mouse moving component API.
 */

#ifndef __MOUSE_H__
#define __MOUSE_H__

// Xlib headers
#include <X11/Xlib.h>

/**
 * @brief Initialize the mouse moving component.
 * 
 * Allocate needed system resouces.
 */
void
Mouse_Initialize(void);

/**
 * @brief Deinitialize the mouse moving component.
 *
 * Free used system resources.
 */
void
Mouse_Deinitialize(void);

/**
 * @brief Simulate a mouse click.
 * 
 * The function simulates that a mouse button has been pressed and
 * released.
 * 
 * @param [in] button The button id to simluate (Button1 etc.).
 * @attention A successful call to Mouse_Initialize() has to be performed
 * before a call to this function.
 */
void
Mouse_Click(int button);

/**
 * @brief Retrieve current mouse pointer coordinates.
 * 
 * This function retrieves the current position of the mouse pointer.
 * 
 * @param [out] x The current absolute x-coordinate of the mouse pointer.
 * @param [out] y The current absolute y-coordinate of the mouse pointer.
 * @attention A successful call to Mouse_Initialize() has to be performed
 * before a call to this function.
 */
void
Mouse_GetCoords(int *x, int *y);

/**
 * @brief Simulate an instant mouse movement to the desired location.
 * 
 * This function sets position of the mouse pointer instantly.
 * 
 * @param [in] x The absolute x-coordinate of the desired location.
 * @param [in] y the absolute y-coordinate of the desired location.
 * @attention A successful call to Mouse_Initialize() has to be performed
 * before a call to this function.
 */
void
Mouse_SetCoords(int x, int y);

#endif
