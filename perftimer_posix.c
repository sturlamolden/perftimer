/*

Copyright (c) 2014, Sturla Molden.
All rights reserved.

See license.txt.

*/


#include <stdlib.h>
#include <time.h>

typedef struct {
    int status;
    clockid_t clock_id;
    struct timespec t0;
    struct timespec t1;
    struct timespec resolution;
} perf_timer_t;

const int PERF_TIMER_CLEAR = 0;
const int PERF_TIMER_RUNNING = 1;
const int PERF_TIMER_STOPPED = 2;


perf_timer_t *create_perf_timer(void)
{
    perf_timer_t *timer = (perf_timer_t *)malloc(sizeof(perf_timer_t));
    if (timer == NULL) goto error;
    timer->status = PERF_TIMER_CLEAR;
    timer->clock_id = CLOCK_REALTIME;
    if (clock_getres(timer->clock_id, &timer->resolution) < 0) goto error;
    return timer;
error:
    if (timer != NULL) free(timer);
    return NULL;
}

void destroy_perf_timer(perf_timer_t *timer)
{
    if (timer != NULL) free(timer);
}

int start_perf_timer(perf_timer_t *timer)
{
    if (timer == NULL) goto error;
    if (timer->status == PERF_TIMER_RUNNING) goto error;
    if (clock_gettime(timer->clock_id, &timer->t0) < 0) goto error;
    timer->status = PERF_TIMER_RUNNING;    
    return 0;
error:
    return -1;
}

int stop_perf_timer(perf_timer_t *timer)
{
    if (timer == NULL) goto error;
    if (timer->status != PERF_TIMER_RUNNING) goto error;
    if (clock_gettime(timer->clock_id, &timer->t1) < 0) goto error;
    timer->status = PERF_TIMER_STOPPED;    
    return 0;
error:
    return -1;
}

int clear_perf_timer(perf_timer_t *timer)
{
    if (timer == NULL) return -1;
    if (timer->status != PERF_TIMER_STOPPED) return -1;
    timer->status = PERF_TIMER_CLEAR;
    return 0;
}

int nanodiff_perf_timer(const perf_timer_t *timer, long double *nanosec)
{
    const long double nanosec_per_sec = 1E9;
    long double t0, t1;    
    if (timer == NULL) return -1;
    if (timer->status != PERF_TIMER_STOPPED) return -1;        
    t0 = (long double)(timer->t0.tv_sec)*nanosec_per_sec + (long double)(timer->t0.tv_nsec);
    t1 = (long double)(timer->t1.tv_sec)*nanosec_per_sec + (long double)(timer->t1.tv_nsec);
    if (nanosec != NULL) {
        *nanosec = t1 - t0;
        return 0;
    } else return -1;
}

int nanores_perf_timer(const perf_timer_t *timer, long double *nanosec_resolution)
{
    const long double nanosec_per_sec = 1E9;
    long double res; 
    if (timer == NULL) return -1;
    res = (long double)(timer->resolution.tv_sec)*nanosec_per_sec 
            + (long double)(timer->resolution.tv_nsec);
    if (nanosec_resolution != NULL) {
        *nanosec_resolution = res;
        return 0;
    } else return -1;
}











