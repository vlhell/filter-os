#pragma once

#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define safe_calloc(nb, size) \
	({ void *buf = calloc(nb, size); assert(buf != NULL); buf;})

#define MAX(a, b) a > b ? a : b

#define MIN(a, b) a < b ? a : b

typedef struct timespec timespec_t;

void safe_push_timer(timespec_t *timer);
double safe_pop_timer(timespec_t *prev);
