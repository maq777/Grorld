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
 * @file match.hpp
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief The template matching component API.
 */

#ifndef __MATCH_H__
#define __MATCH_H__

// C++ Standard Library headers
#include <tuple>

// OpenCV headers
#include <opencv/cv.h>

// Xlib headers
#include <X11/Xlib.h>

/**
 * @class Match
 * @brief An image template matching class.
 * 
 * A class that helps you perform template matching.
 * @par More info here:
 * - http://en.wikipedia.org/wiki/Template_matching
 */
class Match
{
public:
	/**
	 * @brief Constructor.
	 * 
	 * Initilaze the matching algorithm with a pointer to the search image.
	 * 
	 * @param [in] img The search image.
	 */
	Match(XImage *img);
	~Match(void);

	/**
	 * @brief Prepares the matching algorithm.
	 * 
	 * When the search image is replaced, this funcation has to be called
	 * to prepare the matching algorithm.
	 */
	void
	prepare(void);
	
	/**
	 * @brief Do template matching on the search image.
	 * 
	 * Search for the match for thios template on the search image.
	 * 
	 * @param [in] templ The template image
	 * @return The best match on the search image, the point and it's score
	 */
	std::tuple<cv::Point, double>
	match(cv::Mat templ);

	/**
	 * @brief Loads an template image
	 * 
	 * Use this helper to load a template image with the correct format.
	 * 
	 * @param [in] filename A file path to the desired template image.
	 * @return A template image in the correct format.
	 */
	static cv::Mat
	loadTemplate(const char *filename);

private:
	XImage *img;
	cv::Mat mat;
};

#endif
