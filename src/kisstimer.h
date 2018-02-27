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

#ifndef KISSTIMER_H
#define KISSTIMER_H

#include <stdbool.h>
#include <stddef.h>

#define MILLISECONDS(x) ((x) * 1000UL)
#define SECONDS(x) MILLISECONDS((x) * 1000UL)
#define MINUTES(x) SECONDS((x) * 60UL)

struct timed_event {
	void (*isr)(void);
	unsigned long period; /* In microseconds */
	unsigned long last_run;/* Can be initialiazed to anything */
};

struct timer_state {
	struct timed_event *timed_events_list;
	unsigned int list_length;
	bool enabled;
};

void initialize_timer(volatile struct timer_state *state);

void enable_timer(volatile struct timer_state *state);
void disable_timer(volatile struct timer_state *state);

int add_timed_event(volatile struct timer_state *state,
						struct timed_event event);
int remove_timed_event(volatile struct timer_state *state,
						struct timed_event event);

void run_timer(volatile struct timer_state *state);

#endif /* KISSTIMER_H */
