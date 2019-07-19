#ifndef _BRAIN
#define _BRAIN

#include "platform.h"
#ifdef __cplusplus // if c++ 
extern "C" {       // run as c code
#endif
//#include <biosig.h>
#ifdef __cplusplus
}
#endif
/*
#define delta 0
#define theta 1
#define alpha 2
#define beta 3
#define low_beta 4
#define midrange_beta 5
#define high_beta 6
#define gamma 7
*/
#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Brain // class representing EEG which converts electrical signals from the brain into real-time input
{
    public:
		Brain();
		~Brain();
	private:
};
#endif
#endif