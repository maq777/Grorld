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
 * @file timer.h
 * @author Marcus Stjärnås
 * @date July, 2011
 * @version 1
 * @brief A simplistic timer API & implementation.
 */

#ifndef __TIMER_H__
#define __TIMER_H__

// C Standard Library headers
#include <assert.h>
#include <time.h>

/**
 * @brief Subtract a timespec value from another.
 * @param [in] ts1 The minuend.
 * @param [in] ts2 The subtrahend.
 * @return The difference.
 */
inline struct timespec
timespec_sub(const struct timespec &ts1, const struct timespec &ts2)
{
    assert(ts1.tv_sec > ts2.tv_sec || (ts1.tv_sec >= ts2.tv_sec && ts1.tv_nsec >= ts2.tv_nsec));

    struct timespec result;

	result.tv_sec = ts1.tv_sec - ts2.tv_sec;
	if (ts1.tv_nsec < ts2.tv_nsec)
	{
		result.tv_nsec = ts1.tv_nsec + 1000000000l - ts2.tv_nsec ;
		--result.tv_sec;
	}
	else
	{
		result.tv_nsec = ts1.tv_nsec - ts2.tv_nsec ;
	}

    return result;
}

/**
 * @brief Helper function to convert a timespec instance into milliseconds.
 * @param [in] ts A time in timespec format.
 * @return The time as milliseconds.
 */
inline long
timespec_to_millis(const struct timespec &ts)
{
	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000l;
}

/**
 * @brief Helper function to convert a milliseconds instance into timespec.
 * @param [in] ms A time in milliseconds.
 * @return The time as a timespec.
 */
inline struct timespec
millis_to_timespec(long ms)
{
	assert(ms >= 0);

	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms & 999) * 1000000l;

	return ts;
}

#endif
