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
std::string Console::execute(const std::string& cmd) // executes a command on the terminal, and sends the result to a txt file
{
    std::string file_name = "output.txt";
    std::system( ( cmd + " | tee -a " + file_name ).c_str() ) ; // redirect output to file // " > " // https://askubuntu.com/questions/420981/how-do-i-save-terminal-output-to-a-file
    // open file for input, return string containing characters in the file
    std::ifstream file(file_name) ;
    return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() } ;
}
int Console::execute(lua_State *L)
{
    std::string cmd = lua_tostring(L, -1);
    lua_pushstring(L, Console::execute(cmd).c_str());
    return 1;
}
/////////////
std::string Console::color(const std::string& text, const std::string& foreground_color, const std::string& background_color, std::string format_attribute, std::string reset_attribute)
{   // "\033[0;33;42mI am a Banana!\033[0m"
#ifdef DOKUN_LINUX
    unsigned int foreground_number = 39; // 39=Default
    unsigned int background_number = 49; // 49=Default
    unsigned int format_attribute_number = 0; // 0=Default
    unsigned int reset_attribute_number  = 0; // 0=All
    // format attribute
    if(String::lower(format_attribute) == "default") format_attribute_number = 0;
    if(String::lower(format_attribute) == "bold") format_attribute_number = 1;
    if(String::lower(format_attribute) == "dim") format_attribute_number = 2;
    if(String::lower(format_attribute) == "underlined") format_attribute_number = 3;
    if(String::lower(format_attribute) == "blink") format_attribute_number = 5;
    if(String::lower(format_attribute) == "reverse") format_attribute_number = 7;
    if(String::lower(format_attribute) == "hidden") format_attribute_number = 8;
    // foreground
    if(String::lower(foreground_color) == "default") foreground_number = 39;
    if(String::lower(foreground_color) == "black") foreground_number = 30;
    if(String::lower(foreground_color) == "red") foreground_number = 31;
    if(String::lower(foreground_color) == "green") foreground_number = 32;
    if(String::lower(foreground_color) == "yellow") foreground_number = 33;
    if(String::lower(foreground_color) == "blue") foreground_number = 34;
    if(String::lower(foreground_color) == "magenta") foreground_number = 35;
    if(String::lower(foreground_color) == "cyan") foreground_number = 36;
    if(String::lower(foreground_color) == "light gray") foreground_number = 37;
    if(String::lower(foreground_color) == "dark gray") foreground_number = 90;
    if(String::lower(foreground_color) == "light red") foreground_number = 91;
    if(String::lower(foreground_color) == "light green") foreground_number = 92;
    if(String::lower(foreground_color) == "light yellow") foreground_number = 93;
    if(String::lower(foreground_color) == "light blue") foreground_number = 94;
    if(String::lower(foreground_color) == "light magenta") foreground_number = 95;
    if(String::lower(foreground_color) == "light cyan") foreground_number = 96;
    if(String::lower(foreground_color) == "white") foreground_number = 97;
    // background
    if(String::lower(background_color) == "default") background_number = 49;
    if(String::lower(background_color) == "black") background_number = 40;
    if(String::lower(background_color) == "red") background_number = 41;
    if(String::lower(background_color) == "green") background_number = 42;
    if(String::lower(background_color) == "yellow") background_number = 43;
    if(String::lower(background_color) == "blue") background_number = 44;
    if(String::lower(background_color) == "megenta") background_number = 45;
    if(String::lower(background_color) == "cyan") background_number = 46;
    if(String::lower(background_color) == "light gray") background_number = 47;
    if(String::lower(background_color) == "dark gray") background_number = 100;
    if(String::lower(background_color) == "light red") background_number = 101;
    if(String::lower(background_color) == "light green") background_number = 102;
    if(String::lower(background_color) == "light yellow") background_number = 103;
    if(String::lower(background_color) == "light blue") background_number = 104;
    if(String::lower(background_color) == "light magenta") background_number = 105;
    if(String::lower(background_color) == "light cyan") background_number = 106;
    if(String::lower(background_color) == "white") background_number = 107;
    // reset attribute
    if(String::lower(reset_attribute) == "all") reset_attribute_number = 0;
    if(String::lower(reset_attribute) == "bold") reset_attribute_number = 21;
    if(String::lower(reset_attribute) == "dim") reset_attribute_number = 22;
    if(String::lower(reset_attribute) == "underlined") reset_attribute_number = 24;
    if(String::lower(reset_attribute) == "blink") reset_attribute_number = 25;
    if(String::lower(reset_attribute) == "reverse") reset_attribute_number = 27;
    if(String::lower(reset_attribute) == "hidden") reset_attribute_number = 28;    

    return "\033[" + std::to_string(format_attribute_number) + ";" + std::to_string(foreground_number) + ";" + std::to_string(background_number) + "m" + text + "\033[" + std::to_string(reset_attribute_number) + "m";
#endif    
    return text;
}
/////////////
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
/////////////
/////////////
