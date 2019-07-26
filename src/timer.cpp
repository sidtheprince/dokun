#include "../include/timer.h"

Timer::Timer() : second(0), minute(0), hour(0), status(false)
{}
/////////////
Timer::~Timer(void)
{}
/////////////
#ifdef __windows__
LARGE_INTEGER Timer::frequency = (QueryPerformanceFrequency(&Timer::frequency), Timer::frequency); // initialize the resolution of the timer
LONGLONG Timer::overhead = Timer::get_overhead();                                                      // calculate the overhead of the timer
#endif
/////////////
double Timer::frames (0.0);
/////////////
double Timer::start_time (clock() / CLOCKS_PER_SEC);
/////////////
double Timer::frame_rate(0);
/////////////
double Timer::counter (0);
/////////////
double Timer::second_e(0);
/////////////
double Timer::minute_e(0);
/////////////
double Timer::hour_e  (0);
/////////////
double Timer::day_e  (0);
/////////////
double Timer::week_e  (0);
/////////////
double Timer::month_e  (0);
/////////////
double Timer::year_e  (0);
/////////////
double Timer::elapsed_now(clock());
/////////////
double Timer::elapsed_last(elapsed_now);
/////////////
double Timer::target_fps (MAX_TARGET_FPS);
/////////////
double Timer::report_interval (0.1);
/////////////
double Timer::delta_now (clock() / CLOCKS_PER_SEC);
/////////////
double Timer::delta_last(clock() / CLOCKS_PER_SEC);
/////////////
double Timer::old_time(0); 
double Timer::new_time(clock() / (double)CLOCKS_PER_SEC);
double Timer::delta_time(0);
/////////////
/////////////
void Timer::start()
{
	//if(!get_status()) // if timer is off
	//{
		//status = true; // turn the timer on  
	#ifdef __windows__
        QueryPerformanceFrequency(&frequency);   // get ticks per second
		QueryPerformanceCounter(&start_tick );   // start timer       
    #endif
    #ifdef __gnu_linux__
        QueryPerformanceFrequency(&frequency);  // get ticks per second
		QueryPerformanceCounter(&start_tick );	// start timer
    #endif			
/* 	    increment_by_second(&(this)->second); // increase by sec
	    increment_by_minute(&(this)->minute); // increase by min
	    increment_by_hour(&(this)->hour);	   // increase by hr */	
	//}
}
/////////////
int Timer::start(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->start();
	}		
	return 0;
}
/////////////
void Timer::stop()
{
	//if(get_status()) // if timer is on
	//{
	#ifdef __windows__
        QueryPerformanceCounter(&stop_tick); // stop timer
    #endif	
	#ifdef __gnu_linux__
	    QueryPerformanceCounter(&stop_tick); // stop timer
	#endif
		//status = false; // turn it off
	//}
}
/////////////
int Timer::stop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->stop();
	}		
	return 0;
}
/////////////
void Timer::reset() // reset time
{
	second = 0;
	minute = 0;
	hour   = 0;
	status = false;
}
/////////////
void Timer::reset_s() // reset time
{
    start_time = Timer::seconds(); 
	// Reset Frames.
    frames = 0;
}
/////////////
int Timer::reset(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Timer * timer = *static_cast<Timer **>(lua_touserdata(L, -1));
		timer->reset();
		return 0;
	}
	return 0;
}
/////////////
double Timer::get_milliseconds()
{
	#ifdef __windows__
	    milisecond = (stop_tick.QuadPart - start_tick.QuadPart - overhead) * 1000.0 / frequency.QuadPart;
	#endif
	#ifdef __gnu_linux__
	    milisecond = (stop_tick - start_tick) * 1000.0 / frequency;
	#endif
    return milisecond;	
}
/////////////
double Timer::get_seconds()
{
	#ifdef __windows__
	    second = (stop_tick.QuadPart - start_tick.QuadPart - overhead) / frequency.QuadPart;
	#endif
	#ifdef __gnu_linux__
	    milisecond = (stop_tick - start_tick) / frequency;
	#endif
	return second;
}
/////////////
double Timer::get_minutes()
{
	return minute;
}
/////////////
double Timer::get_hours()
{
	return hour;
}
/////////////
#ifdef __windows__
LONGLONG Timer::get_overhead()
{
	Timer timer;
    timer.start();
    timer.stop();
    return timer.stop_tick.QuadPart - timer.start_tick.QuadPart;	
}
#endif
/////////////
bool Timer::get_status()
{
	return status;
}
/////////////
double Timer::seconds() // Works! get seconds since program started
{
	#ifdef __windows__
	    return GetTickCount() * 1000; //  number of seconds that have elapsed since the system was started (up to 49.7 days)
	#endif	
	#ifdef __gnu_linux__
		struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec + now.tv_nsec / 1000000000.0;	
	#endif
    return (clock() / (double)CLOCKS_PER_SEC);
}
/////////////
int Timer::seconds(lua_State *L)
{
	lua_pushnumber(L, Timer::seconds());
	return 1;
}
/////////////
double Timer::milliseconds() // Works!
{
	#ifdef __windows__
	    return GetTickCount(); //  number of miliseconds that have elapsed since the system was started (up to 49.7 days)
	#endif		
	#ifdef __gnu_linux__
	#endif	
	//return (clock() / (double)CLOCKS_PER_SEC) * 1000;
	std::chrono::milliseconds m(1000);
	return m.count();
}
/////////////
int Timer::milliseconds(lua_State *L)
{
	lua_pushnumber(L, Timer::milliseconds());
	return 1;
}
/////////////
double Timer::minutes() // Works!
{
	elapsed_now = clock();
	counter = counter + (double)(elapsed_now - elapsed_last);
	elapsed_last = elapsed_now;
	
	if(counter  > (60.0 * CLOCKS_PER_SEC)) 
	{
		counter -= (60.0 * CLOCKS_PER_SEC);
		minute_e = minute_e + 1;
	}
	return minute_e;
}
/////////////
int Timer::minutes(lua_State *L)
{
	lua_pushnumber(L, Timer::minutes());
	return 0;
}
/////////////
double Timer::hours() // Not working when using same values as minutes, days + :(
{
	double min_to_hr = (static_cast<int>(Timer::minutes()) / 60);
	if(min_to_hr == floor(min_to_hr)    &&     min_to_hr > 0) // if min_to_hr is a whole number and is greater than 0
		hour_e = hour_e + 1;
	return hour_e;
}        
/////////////
int Timer::hours(lua_State *L)
{
	lua_pushnumber(L, Timer::hours());
	return 0;
}
/////////////
double Timer::days() // Not yet tested :o
{
	double hr_to_day = (Timer::hours() / 24); // a day is 24 hours
	if(hr_to_day == floor(hr_to_day)    && hr_to_day > 0)
		day_e = day_e + 1;
	return day_e;	
	
}
int Timer::days(lua_State *L)
{
	lua_pushnumber(L, Timer::days());
	return 0;
}
/////////////
double Timer::weeks() // Not yet tested :o
{
	double day_to_week = (Timer::days() / 7); // 7 days in a week
	if(day_to_week == floor(day_to_week)   &&  day_to_week > 0)
		week_e = week_e + 1;
	return week_e;	
}
int Timer::weeks(lua_State *L)
{
	lua_pushnumber(L, Timer::weeks());
	return 0;
}
/////////////
double Timer::months() // Not yet tested :o
{
	double week_per_month = (Timer::weeks() / 4.34524);
	month_e = week_per_month;
	return month_e;
}
int Timer::months(lua_State *L)
{
	lua_pushnumber(L, Timer::months());
	return 0;
}
/////////////
double Timer::years() // Not yet tested :o
{
	double month_per_year = (Timer::months() / 12);
	if(month_per_year == floor(month_per_year)   && month_per_year > 0)
		year_e = year_e + 1;
	return year_e;
}
int Timer::years(lua_State *L)
{
	lua_pushnumber(L, Timer::years());
	return 0;
}
/////////////
double Timer::delta() // get delta time - not sure how to implement it
{/*
	delta_now  = (clock() / CLOCKS_PER_SEC);
	double delta_time = ((delta_now - delta_last)/ 1000000);	std::cout << delta_time << " = delta\n";
	delta_last = delta_now;
	*/
    const clock_t begin_time = clock();
	return double( clock () - begin_time ) /  CLOCKS_PER_SEC;//delta_time;
}
/////////////
int Timer::delta(lua_State *L)
{
	lua_pushnumber(L, Timer::delta());
	return 1;
}
/////////////
double Timer::framerate() 
{
	frames = frames + 1; // increase counter by 1
	
	if((Timer::seconds() - start_time) > 1.0)
	{
		frame_rate = frames / (Timer::seconds() - start_time);
		start_time = Timer::seconds(); // reset
		frames = 0;                    // reset
	}
	return (frame_rate);
}
/////////////
int Timer::framerate(lua_State *L)
{
	lua_pushnumber(L, Timer::framerate());
	return 1;
}
/////////////
void Timer::set_target_framerate(double limit)
{
	target_fps = limit;
	if(limit > MAX_TARGET_FPS)
		target_fps = MAX_TARGET_FPS;
	if(limit < MIN_TARGET_FPS)
		target_fps = MIN_TARGET_FPS;
}

void Timer::set_interval(double interval)
{
	report_interval = interval;
}
/////////////
void Timer::increment_by_second(double * value_ptr, int seconds)
{
	#ifdef __gnu_linux__
	#endif
}
/////////////
void Timer::increment_by_minute(double * value_ptr, int minutes)
{}
/////////////
void Timer::increment_by_hour(double * value_ptr, int hours)
{}
/////////////
/////////////
#ifdef __gnu_linux__ // re-create Win32's function
bool Timer::QueryPerformanceFrequency(int64_t *frequency)
{
  /* Sanity check. */
    assert(frequency != NULL);
    /* gettimeofday reports to microsecond accuracy. */
    *frequency = usec_per_sec;
    return true;	
}
bool Timer::QueryPerformanceCounter(int64_t *performance_count)
{/*
    struct timeval time;        // struct timespec time;
    // Sanity check.
    assert(performance_count != NULL);
    // Grab the current time.
    gettimeofday(&time, NULL); // clock_gettime(&time)
    *performance_count = time.tv_usec + // Microseconds. 
                         time.tv_sec * usec_per_sec; // Seconds. 
    return true;	
*/	
    struct timespec time;
    // Sanity check.
    assert(performance_count != NULL);
    // Grab the current time. - clock_gettime should be used instead of the the obsolescent gettimeofday function
    clock_gettime(CLOCK_MONOTONIC, &time); // monotonic clock cannot be set and represents time since some unspecified starting point; CLOCK_REALTIME = wall-clock_time; CLOCK_PROCESS_CPUTIME_ID = cpu_time   librt.a
    *performance_count = (time.tv_nsec * 0.001) +              // nanoseconds - convert to microsecond??
                         time.tv_sec * usec_per_sec; // Seconds
    return true;
}
double Timer::GetTickCount(void) 
{
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0)
        return 0;
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}
#endif
/////////////
void Timer::update_dt() // run in loop before drawing
{
    Timer::old_time = Timer::new_time;
    Timer::new_time = (clock() / (double)CLOCKS_PER_SEC) * 1000;// get time in milliseconds or something; increments by the 100s
    Timer::delta_time = Timer::new_time - Timer::old_time;
#ifdef DOKUN_DEBUG    
    std::cout << "delta_time: " << Timer::delta_time << "\n";
#endif    
} // usage: sprite->translate_x(1 * Timer::delta_time, 0)
/////////////
/////////////
int Timer::new_(lua_State *L)
{
    lua_settop(L, 0); // clear unnecessary arguments
	// create table
	lua_createtable(L, 0, 0);
	// set mt
	lua_getglobal(L, "Timer_mt");
	lua_setmetatable(L, 1);
	// set userdata
	Timer **timer = static_cast<Timer**>(lua_newuserdata(L, sizeof(Timer*)));
	*timer = new Timer();
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1)) // top arg is table 
	    return 1;
	lua_pushnil(L);
	return 1;
}
/////////////
// source: https://blogs.msdn.microsoft.com/nativeconcurrency/2011/12/27/high-resolution-timer-for-c/
/*
Getting elapse time since the program started:
        std::cout << Timer::miliseconds() << " miliseconds\n";
		std::cout << Timer::seconds    () << " seconds    \n";
		std::cout << Timer::minutes    () << " minutes    \n";
		std::cout << Timer::hours      () << " hours      \n";
Getting frame rate:
    std::cout << "Framerate: " << Timer::framerate() << std::endl;
	


wait function:
    double ms = 0; 
    luaL_checknumber(L, 1); 
    ms = lua_tonumber(L, 1);
  
    clock_t start_time_time = clock();
    clock_t end_time = ms * 1000 + start_time_time;
    while(clock() != end_time) {}
	
    return 0;


	std::chrono::time_point<std::chrono::system_clock> start_time, end;
	start_time = std::chrono::system_clock::elapsed_now();
	
	// do something here
	Model model;
	model.load("res/cube.obj");
	
	end = std::chrono::system_clock::elapsed_now();
	std::chrono::duration<double> elapse = end-start_time;
	
	std::cout << "It took " << elapse.count() << "seconds to load the model\n";
*/	
