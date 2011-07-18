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
 * @file match.cpp
 * The template matching component uses OpenCV for all the dirty work.
 * @par More info about the used library:
 * - http://en.wikipedia.org/wiki/OpenCV
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief The template matching component implementation.
 */

// C++ (C Standard Library) headers
#include <cassert>

// Xlib headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// OpenCV headers
#include <opencv/highgui.h>

// Local C headers
extern "C"
{
#include "screen.h"
}

// Local C++ headers
#include "match.hpp"

Match::Match(XImage *img)
{
	assert(img);
	this->img = img;

#ifndef COLOR
	mat = cv::Mat::zeros(this->img->height, this->img->width, CV_8U);
#else
	mat = cv::Mat::zeros(this->img->height, this->img->width, CV_8UC3);
#endif

#ifdef TEST
	cv::namedWindow("debug", CV_WINDOW_AUTOSIZE);
#endif
}

Match::~Match(void)
{
}

void
Match::prepare(void)
{
	// Convert screenshot to a opencv matrix
	for (int y = 0; y < img->height; ++y)
	{
		for (int x = 0; x < img->width; ++x)
		{
			unsigned long pixel = XGetPixel(img, x, y);

#ifndef COLOR
			unsigned char grey = ((pixel & 0xff) + ((pixel >> 8) & 0xff) + ((pixel >> 16) & 0xff)) / 3;
			mat.at<unsigned char>(y, x) = grey;
#else
			mat.at<unsigned char>(y, x*mat.channels()) = pixel & 0xff;
			mat.at<unsigned char>(y, x*mat.channels()+1) = (pixel >> 8) & 0xff;
			mat.at<unsigned char>(y, x*mat.channels()+2) = (pixel >> 16) & 0xff;
#endif
		}
	}
}

cv::Mat
Match::loadTemplate(const char *filename)
{
#ifndef COLOR
	return cv::imread(filename, 0);
#else
	return cv::imread(filename);
#endif
}

std::tuple<cv::Point, double>
Match::match(cv::Mat templ)
{
	// Do 'quick' template matching, http://en.wikipedia.org/wiki/Template_matching
	cv::Mat mres;
	cv::matchTemplate(mat, templ, mres, CV_TM_SQDIFF_NORMED);

	// Retrieve the absolute score for the best location
	double score;
	cv::Point local_position;
	cv::minMaxLoc(mres, &score, NULL, &local_position, NULL);

	// Calculate a real/relative score for the hit, in sigma (http://en.wikipedia.org/wiki/Standard_deviation)
	cv::Scalar mean, stddev;
	cv::meanStdDev(mres, mean, stddev);
	double sigma = std::abs(mean[0] - score) / stddev[0];

#ifdef TEST // Debug helper
	cv::Mat color;
#ifndef COLOR
	cv::cvtColor(mat, color, CV_GRAY2BGR);
#else
	mat.copyTo(color);
#endif
	char tmp[256];
	snprintf(tmp, 256, "sigma: %0.2lf", sigma);
	cv::circle(color, local_position, 20, cv::Scalar(255, 0, 255), 5);
	cv::putText(color, tmp, local_position, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 0), 2);
	cv::imshow("debug", color);
	cv::waitKey(20);
#endif

	return std::tuple<cv::Point, double>(local_position, sigma);
}
