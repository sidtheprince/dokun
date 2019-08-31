#ifndef _CONSOLE
#define _CONSOLE
#include "platform.h"
#include "string.h"
#include "box.h"
#include "edit.h"
#include "scrollbar.h"

#ifdef __cplusplus
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <lua.hpp>

class Console // static class
{
	public:
	    Console();
		~Console();
		
		static void set_title(std::string title);     static int set_title(lua_State *L);// SetConsoleTitle(TEXT("Win32 Console Control Demo"));
	    static void set_size(int width, int height);  static int set_size(lua_State *L);
		static std::string color(const std::string& text, const std::string& foreground_color, const std::string& background_color, std::string format_attribute = "Default", std::string reset_attribute = "All");		
	    static void draw(); // draws box with console messages
	    static void init();
	
		static void create();                         static int create(lua_State *L);
		static void destroy();                        static int destroy(lua_State *L); 	
		static void write(const std::string& text);   static int write(lua_State *L);
		static void read();                           static int read(lua_State *L);
		static std::string execute(const std::string& cmd);  static int execute(lua_State *L);
	private: // 
	    #ifdef __windows__
		    static HANDLE rHnd;
			static HANDLE wHnd;
		#endif // 735, 345
		static std::string on_enter();
		static Console * console_ptr; // singleton
		Box * box;
		Edit * edit;
		Scrollbar * scrollbar;
		std::vector<Label *> label_list;
		std::vector<std::string> discarded_string_list;
		std::string cache; // last saved string in edit
};
#endif
#endif

/*
Usage:

    std::string colored_text = Console::color("Hello there Sid", "Yellow", "Blue");
    std::cout << colored_text << std::endl;
    
    // place this in loop ...
    Console::draw()

*/
