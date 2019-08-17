#ifndef _CONSOLE
#define _CONSOLE
#include "platform.h"
#include "string.h"

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
	
		static void create();                         static int create(lua_State *L);
		static void destroy();                        static int destroy(lua_State *L); 	
		static void write();                          static int write(lua_State *L);
		static void read();                           static int read(lua_State *L);
		static std::string execute(const std::string& cmd);  static int execute(lua_State *L);
	private: // 
	    #ifdef __windows__
		    static HANDLE rHnd;
			static HANDLE wHnd;
		#endif // 735, 345
};
#endif
#endif

/*
Usage:

    std::string colored_text = Console::color("Hello there Sid", "Yellow", "Blue");
    std::cout << colored_text << std::endl;

*/
