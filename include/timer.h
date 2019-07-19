#ifndef _TIMER
#define _TIMER

#include "platform.h"
#include "logger.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <thread>
#include <lua.hpp> 

#ifndef MAX_TARGET_FPS
#define MAX_TARGET_FPS 60
#endif
#ifndef MIN_TARGET_FPS
#define MIN_TARGET_FPS 20
#endif

class Timer 
{
    public:
	    Timer(); static int new_(lua_State *L);
		~Timer();
		// normal functions
		void start();                 static int start(lua_State *L);// increment by second
		void stop();                  static int stop(lua_State *L);// stop incrementing
		void reset();                 static int reset(lua_State *L); // resets timer to default values
		static void reset_s();        
        // newly added!
		// increment a number by ..
		static void increment_by_second(double * value_ptr, int seconds = 1);
		static void increment_by_minute(double * value_ptr, int minutes = 1);
		static void increment_by_hour(double * value_ptr, int hours = 1);
		// setters
		// getters
		double get_milliseconds();
		double get_seconds(); // seconds since start()
		double get_minutes();
		double get_hours();
	#ifdef __windows__
		static LONGLONG get_overhead(); 
	#endif
		bool get_status();
		// get elapsed time in..
		static double milliseconds();  static int milliseconds(lua_State *L);// return current miliseconds since program started
		static double seconds();      static int seconds(lua_State *L);// return current seconds since program started
		static double minutes();      static int minutes(lua_State *L);
		static double hours();        static int hours(lua_State *L);
		static double days();         static int days(lua_State *L);
		static double weeks();        static int weeks(lua_State *L);
		static double months();       static int months(lua_State *L);
		static double years();        static int years(lua_State *L);
		
		// get delta time
		static double delta();        static int delta(lua_State *L); // return delta time
		// get frames per second
		static double framerate();    static int framerate(lua_State *L); // return frames per second
		static void set_target_framerate(double limit);
		static void set_interval(double interval); // report interval (how often the FPS is reported)
		// new
		static void update_dt();
		// boolean
		// ??
	#ifdef __gnu_linux__	
	    static bool QueryPerformanceFrequency(int64_t *frequency);
		static bool QueryPerformanceCounter  (int64_t *performance_count);
		static double GetTickCount(void);
    #endif		
	private:
	#ifdef __windows__
        LARGE_INTEGER start_tick, stop_tick;           // ticks
        static LARGE_INTEGER frequency;        // ticks per second
		static LONGLONG overhead;
	#endif
	#ifdef __gnu_linux__
	    int64_t start_tick, stop_tick;
		int64_t frequency;
		static const unsigned usec_per_sec = 1000000;
		static const unsigned usec_per_msec = 1000;
	#endif
	public:
	    // stopwatch time
		double milisecond;
	    double second;
	    double minute;
		double hour;
		// elapse time
		static double second_e;
	    static double minute_e;
		static double hour_e  ;
		static double day_e   ;
		static double week_e  ;
		static double month_e ;
		static double year_e  ;
		static double elapsed_now;
		static double elapsed_last;
		static double counter ;
		//
		static double old_time; 
        static double new_time;
        static double delta_time;
		// status
		bool status; // status is 1 if time is being incremented, otherwise returns 0
		// frame rate
		static double frames;
	    static double start_time;
	    static double frame_rate;
		static double target_fps;
		static double report_interval;
		// delta time
		static double delta_last;
		static double delta_now ;	
};
/*


Timer timer;
timer.start();  // start timer

// do something ...

timer.stop()    // stop timer
std::cout << "seconds passed: " << timer.get_seconds() << std::endl;



*/
#endif
#endif
