/*

Copyright (c) 2014, Sturla Molden.
All rights reserved.

See license.txt.

*/

#ifndef PERFTIMER_H
#define PERFTIMER_H

#if (defined(PERFTIMER_POSIX))

   #incluse <stdlib.h>
   #include <time.h>

   typedef struct {
       int status;
       clockid_t clock_id;
       struct timespec t0;
       struct timespec t1;
       struct timespec resolution;
   } perf_timer_t;


#elif (defined(PERFTIMER_MACOSX))

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
   

#elif (defined(PERFTIMER_MSWINDOS))


   #include <Windows.h>
   #include <mmsystem.h> 

   typedef struct {
       __int64 status;
       LONG_INTEGER t0;  
       LONG_INTEGER t1;
       LONG_INTEGER frequency;
   } perf_timer_t;


#endif

#ifdef __cplusplus
   extern "C" {
#endif

   extern perf_timer_t *create_perf_timer(void);
   extern void destroy_perf_timer(perf_timer_t *timer);
   extern int start_perf_timer(perf_timer_t *timer);
   extern int stop_perf_timer(perf_timer_t *timer);
   extern int clear_perf_timer(perf_timer_t *timer);
   extern int nanores_perf_timer(const perf_timer_t *timer, long double *nanosec_resolution);
   extern int nanodiff_perf_timer(const perf_timer_t *timer, long double *nanosec);

#ifdef __cplusplus
   } /* extern "C" */
#endif

#endif /* PERFTIMER_H */

