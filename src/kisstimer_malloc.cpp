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

#ifndef KT_STATIC_SIZE

#include "kisstimer.h"
#include "include/events_list_impl.h"
#include <stdlib.h>

void initialize_malloc_timer(volatile struct timer_state *state)
{
	state->timed_events_list = NULL;
	state->list_length = 0;
	state->enabled = false;
}

int realloc_timed_events_list(volatile struct timer_state *state,
						unsigned int new_length)
{
	struct timed_event *new_list = realloc(state->timed_events_list,
				sizeof(struct timed_event [new_length]));

	if (new_list == NULL)
		return -1;

	state->timed_events_list = new_list;
	return 0;
}

#endif
