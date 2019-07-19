#ifndef _VIRTUAL
#define _VIRTUAL

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Virtual // a virtual reality class
{
    public:
	    Virtual();
	    ~Virtual();
		
		void start(); // vrStart();
		void close(); // vrExit();
    private:
};
#endif
#endif