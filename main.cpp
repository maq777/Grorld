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
 * @file main.cpp
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief Grorld application
 * 
 * @mainpage Grorld
 * 
 * @par Description:
 * Grorld is an application to automatically grind bonus resources in
 * CivWorld. As soon as one of these bonus bubbles appear on the screen
 * this application will automatically move the mouse to that location.
 * Hence you can have the computer grab bonuses all night while you
 * sleep...
 * 
 * @par Usage:
 * - Use the command "./grorld" from the directory where you installed
 * it to start the application.
 * @par Installation:
 * - Run "sudo apt-get install cmake libx11-dev libcv-dev libcvaux-dev
 * libhighgui-dev && cmake . && make" from the directory where the
 * source is located. Follow the instructions.
 * 
 * @attention - Do NOT move the browser while playing
 * @attention - Always have the browser window at front
 */

// C++ Standard Library headers
#include <functional>
#include <iostream>
#include <random>

// C++ (C Standard Library) headers
#include <cassert>

// Local C headers
extern "C"
{
#include "mouse.h"
#include "screen.h"
#include "timer.h"
}

// Local C++ headers
#include "match.hpp"

/**
 * @def MATCHING_THRESHOLD
 * @brief A threshold determine template match or miss
 * 
 * The score each template matching need atleast reach to be treated
 * as a hit. If the score is below this value, the template is considered
 * NOT to be part of current search image.
 */
#define MATCHING_THRESHOLD 2.7

/// @todo blog and insert http adress into code header...

/**
 * @brief Grorld entry point
 * 
 * Grorld application entry point and main loop. It also contains
 * the logic to automatically grind CivWorld bonus resources!
 */
int main(const int argc, const char **argv)
{
	std::cout << "Grorld, version 1" << std::endl;

	// Create a pseudorandom number generator instance
	// (http://en.wikipedia.org/wiki/C%2B%2B0x#Extensible_random_number_facility)
	std::mt19937 engine(time(NULL));

	// Initialize mouse & screen, put the target window in front
	Mouse_Initialize();
	XImage *grab = Screen_Initialize("CivWorld on Facebook");
	assert(grab);

	// Create the macthing algoritm object
	Match m(grab);

	// Load images that we want to match/find on the screen
	const cv::Mat bonus = Match::loadTemplate("assets/bonus.png");
	const cv::Mat city = Match::loadTemplate("assets/city.png");

	// Main loop (http://en.wikipedia.org/wiki/Event_loop)
	while (true)
	{
		// Grab a new frame (much like doing a screenshot)
		Screen_Get();
		// Prepare the matching algoritm with the new frame...
		m.prepare();

		// Search (via a template matching algorithm) for a bonus bubbles
		std::tuple<cv::Point, double> mr = m.match(bonus);
#ifndef TEST
		if (std::get<1>(mr) > MATCHING_THRESHOLD)
		{
			// We got a hit on the serach image, translate that point into a screen coordinate for the mouse to hover
			Screen_TranslateCoordinates(&std::get<0>(mr).x, &std::get<0>(mr).y);

			// Hover the mouse over it (use some randomness for the pointer placement...)
			Mouse_SetCoords(std::get<0>(mr).x + std::uniform_int_distribution<int>(0, bonus.size().width)(engine), std::get<0>(mr).y + std::uniform_int_distribution<int>(0, bonus.size().height)(engine));

			std::cout << time(NULL) << "\tBONUS: at " << std::get<0>(mr).x << "x" << std::get<0>(mr).y << " (score: " << std::get<1>(mr) << ")" << std::endl;
			continue;
		}

		// Once in a while, check if we need to press the city button...
		else
		{
			std::tuple<cv::Point, double> mr = m.match(city);
			if (std::get<1>(mr) > MATCHING_THRESHOLD)
			{
				// We got a hit on the serach image, translate that point into a screen coordinate for the mouse to hover
				Screen_TranslateCoordinates(&std::get<0>(mr).x, &std::get<0>(mr).y);

				// Hover the mouse over it and click
				Mouse_SetCoords(std::get<0>(mr).x + (city.size().width / 2), std::get<0>(mr).y + (city.size().height / 2));
				Mouse_Click(Button1);

				std::cout << time(NULL) << "\tCITY: at " << std::get<0>(mr).x << "x" << std::get<0>(mr).y << " (score: " << std::get<1>(mr) << ")" << std::endl;

				// Wait for the window to redraw (it's slow) before trying something clever
				struct timespec delay_click = millis_to_timespec(std::uniform_int_distribution<int>(2000, 4000)(engine));
				clock_nanosleep(CLOCK_MONOTONIC, 0, &delay_click, NULL);
				continue;
			}
		}
#endif

		// Give the computer some time to rest before processing the next frame
		struct timespec sleep = millis_to_timespec(std::uniform_int_distribution<int>(40, 60)(engine));
		clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, NULL);
	}

	// Clean up and exit
	Screen_Deinitialize();
	Mouse_Deinitialize();

	return EXIT_SUCCESS;
}
