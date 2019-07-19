#ifndef _JOYSTICK
#define _JOYSTICK

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Joystick 
{
    public:
        Joystick();
        ~Joystick();
        enum Axis {};
  
        bool connect(); // checks if it has been connected
        bool is_pressed(unsigned int button);
  private:

};
#endif
#endif