#ifndef _EVENT
#define _EVENT

#include "platform.h"
#include "types.h"
#include "window.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <vector>
#include <functional> // std::function
#include <lua.hpp>

class Event // uses call_back functions: dokun.load, dokun.draw, dokun.keyboard, dokun.mouse,, (dokun.input) dokun.update, dokun.focus, dokun.quit (Insprired by love2d)
{
    public:
	    Event();
		Event(const Event& event);
		Event(const dokun_Event& event);
		~Event();
		// normal
		void peek();
		void wait();
		// setters
		void set_type(int event);
		void set_type(const dokun_Event& event);
        void set_callback(std::function<void (void)>, std::string callback_t); // C++
		void set_callback(lua_State * L, std::string function_name, std::string callback_t); // Lua
		// getters
		dokun_Event get_type() const; // returns event type	
        // C++ Callbacks - used with window->loop()
        void on_load();
		void on_draw();	
		void on_update();
		void on_focus();
		void on_quit();	        		
		// Lua Callbacks - used with window:loop()
		void on_load(lua_State *L);
		void on_draw(lua_State *L);	
		void on_input(lua_State *L);
		void on_update(lua_State *L);
		void on_focus(lua_State *L);
		void on_quit(lua_State *L);	
		// devices
		int open_mouse_device  ();
		void close_mouse_device();
		// events
		void get_mouse_event();
		//void get_input_event(unsigned int * value, );	
	private:
	    dokun_Event type; // event type
		std::function<void (void)> load_function;
		std::function<void (void)> draw_function;
		std::function<void (void)> mouse_function;
		std::function<void (void)> keyboard_function;
		std::function<void (void)> update_function;
		std::function<void (void)> focus_function;
		std::function<void (void)> quit_function;
		std::vector<std::function<void (void)>> custom_function;
		// devices
        // REL_WHEEL and REL_HWHEEL should be used to report scroll wheel events where available.
        // REL_{X,Y} must be reported when the mouse moves.
        // BTN_LEFT must be used to report the primary button press. BTN_{MIDDLE,RIGHT,4,5,etc.} should be used to report further buttons of the device.		
		int mouse_device;
		// ABS_{X,Y} must be reported with the location of the touch.
		// BTN_TOUCH must be used to report when a touch is active on the screen.
		// BTN_{MOUSE,LEFT,MIDDLE,RIGHT} must not be reported as the result of touch contact.
		// BTN_TOOL_<name> events should be reported where possible.
		// For new hardware, INPUT_PROP_DIRECT should be set.
		int touch_device; // for touchscreens
		// EV_KEY: Used to describe state changes of keyboards, buttons, or other key-like devices (event type).
		// EV_KEY events take the form KEY_<name> or BTN_<name>. For example, KEY_A is used to represent the 'A' key on a keyboard.
		// Key press value is 1, Key release value is 0, Key repeat value is 2
		// In general, KEY_<name> is used for keyboard keys, and BTN_<name> is used for other types of momentary switch events.		
		int keyboard_device;
	// input events : More types are defined in include/uapi/linux/input-event-codes.h
	// https://www.kernel.org/doc/html/latest/input/event-codes.html#input-event-codes
	//struct input_event is, among others, defined in include/linux/input.h.
	/*struct input_event {
        struct timeval time; // time is the timestamp, it returns the time at which the event happened
        unsigned short type; // Type is for example EV_REL for relative moment, EV_KEY for a keypress or release
        unsigned short code; // code is event code, for example REL_X or KEY_BACKSPACE, again a complete list is in include/uapi/linux/input-event-codes.h
        unsigned int value;  // value is the value the event carries. Either a relative change for EV_REL, absolute new value for EV_ABS (joysticks ...), or 0 for EV_KEY for release, 1 for keypress and 2 for autorepeat
    };*/	
};
#endif
#endif
/*
USAGES:
    void draw_function()
    {
	    std::cout << "I am drawing something\n";
    }

	Event * event = new Event();
	event->set_callback(draw_function, "draw");	// only use this function with Window::loop
	
	Window * window = new Window();
	window->create("Test", 800, 600);
	window->show();
	
	window->set_event(event);
	window->get_event()->peek();
	
	window->loop(); // calls 'draw_function'
*/
