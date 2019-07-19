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