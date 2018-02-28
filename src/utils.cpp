/*
 * Copyright © 2018  Maël A
 *
 * This file is part of kisstimer, a KISS arduino timer.
 *
 * kisstimer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kisstimer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "include/utils.h"
#include "kisstimer.h"
#include <stddef.h>

VOLATILE void *memcpy_volatile(VOLATILE void *s1,
					const VOLATILE void *s2, size_t n)
{
	VOLATILE unsigned char *s1_bytes = s1;
	const VOLATILE unsigned char *s2_bytes = s2;

	for (size_t i = 0; i < n; i++)
		s1_bytes[i] = s2_bytes[i];

	return s1;
}
