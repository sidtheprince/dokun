#include "../include/console.h"

Console::Console(void)
{}
/////////////
Console::~Console(void)
{}
/////////////
#ifdef __windows__
HANDLE Console::rHnd (GetStdHandle(STD_INPUT_HANDLE));
/////////////
HANDLE Console::wHnd (GetStdHandle(STD_OUTPUT_HANDLE));
#endif
/////////////
void Console::destroy() // destroys console
{
	#ifdef __windows__
        FreeConsole();
    #endif
}
/////////////
int Console::destroy(lua_State *L)
{
	Console::destroy();
	return 0;
}
/////////////
void Console::create() // creates a new console
{
	#ifdef __windows__
	    // don't do anything if we already have a console
    if (GetStdHandle(STD_OUTPUT_HANDLE))
        return;
	    AllocConsole();
	#endif
}
/////////////
int Console::create(lua_State *L)
{
	Console::create();
	return 0;
}
/////////////
void Console::write()
{
	#ifdef __windows__
	//wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	#endif
}
/////////////
int Console::write(lua_State *L)
{
	return 0;
}
/////////////
void Console::read()
{
	#ifdef __windows__
	//rHnd = GetStdHandle(STD_INPUT_HANDLE);
	#endif
}
/////////////
int Console::read(lua_State *L)
{
	return 0;
}
/////////////
void Console::set_title(std::string title)
{
	#ifdef __windows__
	    SetConsoleTitle(TEXT(title.c_str()) );
	#endif
}
/////////////
int Console::set_title(lua_State *L)
{
	Console::set_title(lua_tostring(L, -1));
	return 0;
}
/////////////
void Console::set_size(int width, int height)
{
	#ifdef __windows__
        HWND console = GetConsoleWindow();
        RECT console_rect;
        GetWindowRect(console, &console_rect); //stores the console's current dimensions
        MoveWindow(console, console_rect.left, console_rect.top, width, height, TRUE);	
        //SMALL_RECT console_size = {0, 0, static_cast<SHORT>(width), static_cast<SHORT>(height)};
        //SetConsoleWindowInfo(wHnd, TRUE, &console_size);
    #endif	
}
/////////////
int Console::set_size(lua_State *L)
{
	Console::set_size(static_cast<int>(lua_tonumber(L, -2)), static_cast<int>(lua_tonumber(L, -1)));
	return 0;
}
/////////////
/////////////