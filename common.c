#include "common.h"
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>

//struct timespec prev = {0, 0};

void safe_push_timer(timespec_t *timer)
{
	assert(clock_gettime(CLOCK_MONOTONIC_RAW, timer) == 0);
}

double safe_pop_timer(timespec_t *prev)
{
	timespec_t timer, dur;

	assert(clock_gettime(CLOCK_MONOTONIC_RAW, &timer) == 0);

	if ((timer.tv_nsec - prev->tv_nsec) < 0) {
		dur.tv_sec = timer.tv_sec - prev->tv_sec - 1;
		dur.tv_nsec = 1E9 + timer.tv_nsec - prev->tv_nsec;
	} else {
		dur.tv_sec = timer.tv_sec - prev->tv_sec;
		dur.tv_nsec = timer.tv_nsec - prev->tv_nsec;
	}

	return dur.tv_sec + dur.tv_nsec / 1E9;
}
