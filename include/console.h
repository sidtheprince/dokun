#ifndef _CONSOLE
#define _CONSOLE
#include "platform.h"

#ifdef __cplusplus
#include <iostream>
#include <cstdio>
#include <lua.hpp>

class Console // static class
{
	public:
	    Console();
		~Console();
		
		static void set_title(std::string title);     static int set_title(lua_State *L);// SetConsoleTitle(TEXT("Win32 Console Control Demo"));
	    static void set_size(int width, int height);  static int set_size(lua_State *L); 
	    static void set_background(int red, int green, int blue);
		static void set_font(int red, int green, int blue);
	
		static void create();                         static int create(lua_State *L);
		static void destroy();                        static int destroy(lua_State *L); 	
		static void write();                          static int write(lua_State *L);
		static void read();                           static int read(lua_State *L);
	private: // 
	    #ifdef __windows__
		    static HANDLE rHnd;
			static HANDLE wHnd;
		#endif // 735, 345
};
#endif
#endif