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

/* CONFIGURATION */

//#define KT_STATIC_SIZE 10

/* END CONFIGURATION */

#define MILLISECONDS(x) ((x) * 1000UL)
#define SECONDS(x) MILLISECONDS((x) * 1000UL)
#define MINUTES(x) SECONDS((x) * 60UL)

struct timed_event {
	bool (*isr)(volatile struct timer_state *state, volatile void *arg);
	unsigned long period; /* In microseconds */
	volatile void *isr_arg;
	unsigned long last_run;/* Can be initialiazed to anything */
};

struct timer_state {
	bool enabled;
	bool is_running;
	unsigned int list_length;
#ifdef KT_STATIC_SIZE
	struct timed_event timed_events_list[KT_STATIC_SIZE];
#else
	struct timed_event *timed_events_list;
#endif
};

#ifdef KT_STATIC_SIZE

#define initialize_timer(...) initialize_static_timer(__VA_ARGS__)
void initialize_static_timer(volatile struct timer_state *state);

#else /* ifndef KT_STATIC_SIZE */

#define initialize_timer(...) initialize_malloc_timer(__VA_ARGS__)
void initialize_malloc_timer(volatile struct timer_state *state);

#endif /* ifndef KT_STATIC_SIZE */

int add_timed_event(volatile struct timer_state *state,
						struct timed_event event);
int remove_timed_event(volatile struct timer_state *state,
						struct timed_event event);
int schedule_timed_event(volatile struct timer_state *state,
				struct timed_event event, unsigned long time);

void enable_timer(volatile struct timer_state *state);
void disable_timer(volatile struct timer_state *state);

void run_timer(volatile struct timer_state *state);
void run_timer_loop(volatile struct timer_state *state);
void run_timer_loop_infinite(volatile struct timer_state *state);

#endif /* KISSTIMER_H */
