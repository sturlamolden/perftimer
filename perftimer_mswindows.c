/*

Copyright (c) 2014, Sturla Molden.
All rights reserved.

See license.txt.

*/

#include <Windows.h>
#include <mmsystem.h> 
   
typedef struct {
    __int64 status;
    LONG_INTEGER t0;  
    LONG_INTEGER t1;
    LONG_INTEGER frequency;
} perf_timer_t;

const __int64 PERF_TIMER_CLEAR = 0;
const __int64 PERF_TIMER_RUNNING = 1;
const __int64 PERF_TIMER_STOPPED = 2;

#define min(x,y) (x < y ? x : y)
#define max(x,y) (x > y ? x : y)

static volatile LONG _multimedia_timer_on = 0;

perf_timer_t *create_perf_timer(void)
{
    perf_timer_t *timer;
    TIMECAPS tc;
    UINT wTimerRes;
    UINT TARGET_RESOLUTION = 1;
    if (InterlockedExchange(&_multimedia_timer_on, 1)==0) {
        if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) goto init_error;
        wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
        if (timeBeginPeriod(wTimerRes) != TIMERR_NOERROR) goto init_error;
    }    
    timer = (perf_timer_t *)malloc(sizeof(perf_timer_t));
    if (timer == NULL) goto error;
    timer->status = PERF_TIMER_CLEAR;
    if(!QueryPerformanceFrequency(&timer->frequency)) goto error;
    return timer;
init_error:
    InterlockedExchange(&_multimedia_timer_on, 0);
    return NULL;
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
    if(timer == NULL) goto error;
    if(timer->status == PERF_TIMER_RUNNING) goto error;
    if(!QueryPerformanceCounter(&timer->t0)) goto error;
    timer->status = PERF_TIMER_RUNNING;    
    return 0;
error:
    return -1;
}

int stop_perf_timer(perf_timer_t *timer)
{
    if(timer == NULL) goto error;
    if(timer->status != PERF_TIMER_RUNNING) goto error;
    if(!QueryPerformanceCounter(&timer->t1)) goto error;
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
    long double t0, t1, freq;    
    if (timer == NULL) return -1;
    if (timer->status != PERF_TIMER_STOPPED) return -1;           
    freq = (long double)*(unsigned __int64 *)&timer->frequency;
    t0 = (long double)*(unsigned __int64 *)&timer->t0;
    t1 = (long double)*(unsigned __int64 *)&timer->t1;
    if (nanosec != NULL) {
        *nanosec = nanosec_per_sec*((t1 - t0)/freq);
        return 0;
    } else return -1;
}

int nanores_perf_timer(const perf_timer_t *timer, long double *nanosec_resolution)
{
    const long double nanosec_per_sec = 1E9;
    long double freq; 
    if (timer == NULL) return -1;
    freq = (long double)*(unsigned __int64 *)&timer->frequency;
    if (nanosec_resolution != NULL) {
        *nanosec_resolution = freq / nanosec_per_sec;
        return 0;
    } else return -1;
}













