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

#include "kisstimer.h"

#ifdef KT_STATIC_SIZE

#include "include/events_list_impl.h"

void initialize_static_timer(volatile struct timer_state *state)
{
	state->enabled = false;
	state->list_length = 0;
}

int realloc_timed_events_list(volatile struct timer_state *state,
						unsigned int new_length)
{
	return new_length == 0 && state->list_length == KT_STATIC_SIZE;
}

#endif
