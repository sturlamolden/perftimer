/*

Copyright (c) 2014, Sturla Molden.
All rights reserved.

See license.txt.

*/

#include <assert.h>
/* #include <CoreServices/CoreServices.h> */
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int status;
    uint64_t t0;
    uint64_t t1; 
    mach_timebase_info_data_t tb_info;
} perf_timer_t;

const int PERF_TIMER_CLEAR = 0;
const int PERF_TIMER_RUNNING = 1;
const int PERF_TIMER_STOPPED = 2;

perf_timer_t *create_perf_timer(void)
{
    perf_timer_t *timer = (perf_timer_t *)malloc(sizeof(perf_timer_t));
    if (timer == NULL) goto error;
    timer->status = PERF_TIMER_CLEAR;
    mach_timebase_info(&timer->tb_info);
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
    timer->t0 = mach_absolute_time();
    timer->status = PERF_TIMER_RUNNING;    
    return 0;
error:
    return -1;
}

int stop_perf_timer(perf_timer_t *timer)
{
    if (timer == NULL) goto error;
    if (timer->status != PERF_TIMER_RUNNING) goto error;
    timer->t1 = mach_absolute_time();
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
    long double t0, t1, numer, denom;    
    if (timer == NULL) return -1;
    if (timer->status != PERF_TIMER_STOPPED) return -1;    
    numer = (long double)(timer->tb_info.numer);
    denom = (long double)(timer->tb_info.denom);    
    t0 = (long double)(timer->t0);
    t1 = (long double)(timer->t1);
    if (nanosec != NULL) {
        *nanosec = (t1 - t0) * numer / denom;
        return 0;
    } else return -1;
}

int nanores_perf_timer(const perf_timer_t *timer, long double *nanosec_resolution)
{
    long double numer, denom; 
    if (timer == NULL) return -1;
    numer = (long double)(timer->tb_info.numer);
    denom = (long double)(timer->tb_info.denom);
    if (nanosec_resolution != NULL) {
        *nanosec_resolution = numer / denom;
        return 0;
    } else return -1;
}











