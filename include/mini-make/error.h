/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Mini Make.
 *
 * Mini Make is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mini Make is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mini Make.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file */

#ifndef MINI_MAKE_ERROR_H
#define MINI_MAKE_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup make-errors Make Errors
 * @brief Error codes returned from make functions.
 * */

/** @brief A return code indicating the function
 * was successful.
 * @ingroup make-errors
 * */
extern const int make_success;

/** @brief A return code indicating the function
 * has failed.
 * @ingroup make-errors
 * */
extern const int make_failure;

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_ERROR_H */
