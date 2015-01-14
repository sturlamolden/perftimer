
# Copyright (c) 2014, Sturla Molden.
# All rights reserved.
#
# See license.txt.


cdef extern from "perftimer.h":
    
    ctypedef struct perf_timer_t:
        pass
    
    perf_timer_t *create_perf_timer(void);
    void destroy_perf_timer(perf_timer_t *timer);
    int start_perf_timer(perf_timer_t *timer);
    int stop_perf_timer(perf_timer_t *timer);
    int clear_perf_timer(perf_timer_t *timer);
    int nanores_perf_timer(const perf_timer_t *timer, long double *nanosec_resolution);
    int nanodiff_perf_timer(const perf_timer_t *timer, long double *nanosec);


class PerformanceTimerError(OSError):
    pass



cdef class PerformanceTimer:
    
    """ Nanosecond performance timer """
    
    cdef:
        perf_timer_t *timer
    
    def __cinit__(PerformanceTimer self):
        self.timer = create_perf_timer()
    
    def __init__(PerformanceTimer self):
        if self.timer == NULL: 
            raise PerformanceTimerError("Could not initialize performance timer")
            
    def __dealloc__(PerformanceTimer self):
        destroy_perf_timer(self.timer)
        
    def start(PerformanceTimer self):
        """ start the performance timer """
        if start_perf_timer(&self.timer) < 0: 
            raise PerformanceTimerError("Could not start performance timer")
    
    def stop(PerformanceTimer self):
        """ stop the performance timer """
        if stop_perf_timer(self.timer) < 0: 
            raise PerformanceTimerError("Could not stop performance timer") 

    def clear(PerformanceTimer self):
        """ clear the performance timer """
        if clear_perf_timer(self.timer) < 0: 
            raise PerformanceTimerError("Could not clear performance timer")  
        
    def nanodiff(PerformanceTimer self):
        """ compute time difference in nanoseconds """
        cdef long double nanodiff
        if nanodiff_perf_timer(self.timer, &nanodiff) < 0: 
            raise PerformanceTimerError("Could not compute time difference")
        return float(<double> nanodiff)
        
    def nanores(PerformanceTimer self):
        """ compute time resolution in nanoseconds """
        cdef long double nanores
        if nanores_perf_timer(self.timer, &nanores) < 0: 
            raise PerformanceTimerError("Could not compute time resolution")
        return float(<double> nanores)
        

