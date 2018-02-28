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
#include "include/events_list_impl.h"
#include "include/utils.h"

#include <limits.h>
#include "Arduino.h"

void enable_timer(volatile struct timer_state *state)
{
	state->enabled = true;
}

void disable_timer(volatile struct timer_state *state)
{
	state->enabled = false;
	state->is_running = false;
}

int add_timed_event(volatile struct timer_state *state,
						struct timed_event event)
{
	if (realloc_timed_events_list(state, state->list_length + 1) != 0)
		return -1;

	memcpy_volatile(&state->timed_events_list[state->list_length++],
					&event, sizeof(struct timed_event));
	return 0;
}
int schedule_timed_event(volatile struct timer_state *state,
				struct timed_event event, unsigned long time)
{
	if (!state->is_running)
		return -1;

	event.last_run = micros();
	event.period = time;

	if (add_timed_event(state, event) != 0)
		return -2;
	else
		return 0;
}

static int remove_timed_event_index(volatile struct timer_state *state,
							unsigned int index)
{
	for (unsigned int i = index; i < state->list_length - 1; i++) {
		memcpy_volatile(&state->timed_events_list[i],
					&state->timed_events_list[i + 1],
						sizeof(struct timed_event));
	}

	if (realloc_timed_events_list(state, state->list_length - 1) != 0)
		return 1;

	state->list_length--;
	return 0;
}

int remove_timed_event(volatile struct timer_state *state,
						struct timed_event event)
{
	for (unsigned int i = 0; i < state->list_length; i++) {
		if (state->timed_events_list[i].isr == event.isr
			&& state->timed_events_list[i].period == event.period)
			return remove_timed_event_index(state, i);
	}

	return 1;
}

static unsigned long compute_micros_delta(unsigned long last_micros)
{
	unsigned long current_micros = micros();

	/* Only one wrap-around (overflow) can be taken into account */
	if (current_micros < last_micros) /* Overflow in micros() occured */
		return (ULONG_MAX - last_micros) + current_micros + 1;
	else
		return current_micros - last_micros;
}

static bool execute_event(volatile struct timer_state *state,
						unsigned int event_index)
{
	if (!state->timed_events_list[event_index].isr(state)) {
		if (remove_timed_event_index(state, event_index) == 0)
			return true;
	}

	/*
	 * We add the period and not the current micros() value to keep
	 * the timed_events synchronized.
	 */
	state->timed_events_list[event_index].last_run +=
				state->timed_events_list[event_index].period;

	return false;
}

void run_timer(volatile struct timer_state *state)
{
	if (!state->is_running) {
		unsigned long start_time = micros();

		for (unsigned int i = 0; i < state->list_length; i++)
			state->timed_events_list[i].last_run = start_time;

		state->is_running = true;
	}

	for (unsigned int i = 0; i < state->list_length; i++) {
		unsigned long delta = compute_micros_delta(
					state->timed_events_list[i].last_run);

		if (delta >= state->timed_events_list[i].period) {
			if (execute_event(state, i))
				i--;
		}
	}
}

void run_timer_loop(volatile struct timer_state *state)
{
	while (state->enabled)
		run_timer(state);
}

void run_timer_loop_infinite(volatile struct timer_state *state)
{
	while (true)
		run_timer(state);
}
