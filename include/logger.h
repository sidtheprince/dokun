#ifndef _LOGGER
#define _LOGGER

#define DOKUN_FUNCTION  String(__FUNCTION__).str()
#define DOKUN_FILE      String(__FILE__).str()
#define DOKUN_LINE      String(__LINE__).str()
#define DOKUN_DATE      String(__DATE__).str()
#define DOKUN_TIME      String(__TIME__).str()
#define DOKUN_TAG       String("dokun: ") +
#define DOKUN_LOGTAG    "dokun: " + DOKUN_FILE.substr(DOKUN_FILE.find_last_of("\\/") + 1) + " (" + DOKUN_LINE + "): "      //+ "function (" + DOKUN_FUNCTION + "): "  // added on 10-1-2018
#define DOKUN_CLASS     String(typeid(*this).name()).str()
#define DOKUN_LOGFILE   "doklog.txt"

#include "platform.h" // for platform-specific functions
#include "types.h"
#include "utility.h"
#include "string.h"
#include "system.h"
#include "factory.h"
#ifdef __cplusplus // if c++
#include <iostream>
#include <typeinfo>
#include <lua.hpp>

static std::vector<std::string> dokun_session = {};// stores all logs from current dokun_session
struct Logger {
	Logger() {}
	Logger(int code) { std::cerr << print_error(code) << std::endl; }
	Logger(const String& message, /*std::string function = DOKUN_FUNCTION,*/ std::string filename = DOKUN_FILE, std::string line = DOKUN_LINE) // set print_out to 0 if using std::cout << operator
	{
		std::clog << DOKUN_TAG message << std::endl; // print message on console   
#ifdef DOKUN_DEBUG0
		push("dokun: " + filename.substr(filename.find_last_of("\\/") + 1) + " (" + line + "): " + "function (" + function + "): " + message); // push message to log (session)
#endif
	}
	~Logger() {}
	/////////	
	static void box(const String& text, std::string caption = "dokun", unsigned int type = 0x00000000L)
	{
#ifdef __windows__
    int id = MessageBox(nullptr, static_cast<LPCTSTR>(text.c_str()), static_cast<LPCTSTR>(caption.c_str()), static_cast<UINT>(type)); 
    switch(id)
	{
		case IDABORT:
		    std::abort();
		    break;
		case IDCANCEL:
		    break;
		case IDCONTINUE:
		    break;
		case IDIGNORE:
		    break;
		case IDNO:
		    break;
		case IDYES:
		    break;			
		case IDOK:
		    break;
		case IDRETRY:
		    system("dokun");
			std::exit(0);
		    break;
		case IDTRYAGAIN:
		    system("dokun");
			std::exit(0);
		    break;
	}		
#endif	
#ifdef __gnu_linux__
#endif	
	}
	static void push(const String& message) { dokun_session.push_back(message.str());} // stores a log message from current dokun_session
	static void pop () { dokun_session.pop_back();}
    static void pop(int index) { dokun_session.erase(dokun_session.begin() + index);}
    static void pop(const String& content) {
        for (unsigned int i = 0; i < dokun_session.size(); i++)
        {
            if(String::match(dokun_session[i], content.str(), true) || String::contains(dokun_session[i], content.str()) ) // case sensative by default  (if strings match or string contains specific words)
            {
                dokun_session.erase(dokun_session.begin() + i);
            }
        }
    }
    static std::string get_string(int index) { return dokun_session[index];} // get string at index
    static int get_index(const String& content) { for (unsigned int i = 0; i < dokun_session.size(); i++) { if(String::match(dokun_session[i], content.str(), true) || String::contains(dokun_session[i], content.str()) ) return i;}return -1;} // get index from string
	/////////
	static std::string format(const std::string& fmt) // returns date and time as a string
	{
    #if defined(__cplusplus) && (__cplusplus >= 201103L) // C++11 style - Unfortunately a few compilers dont support std::put_time :c
		auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now); // current time
		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), fmt.c_str());
		return ss.str();
    #endif
    #if defined(__cplusplus) && (__cplusplus < 201103L)
        std::time_t timer;
        char buffer[26];
        struct std::tm* tm_info;
        std::time(&timer);
        tm_info = std::localtime(&timer);	
		std::strftime(buffer, 26, fmt.c_str(), tm_info);
		return buffer;		
	#endif	
	}
	/////////
	static bool save(const String& file_name) // saves all the logs from the current dokun_session to file_name
	{
		std::ofstream file(file_name.c_str(), std::ios::app);
		if(!file.is_open())
			return false;
		for(int i=0; i < dokun_session.size(); i++) {	
			file << dokun_session[i] << std::endl;
		}
		file.close();
		return true;
	}
	/////////
	static int print_error(int error)
	{
		std::string message;
		while(error != 0) // 0 = no error
		{
		    switch(error)
		    {
			    case 1:
			        message = "An unknown error";
			        break;
			case 2:
			    message = "Initialization failed";
			    break;
			case 3:
			    message = "Object is null";
			    break;
			case 4:
			    message = "Out of memory";
			    break;
			case 5:
			    message = "File not found";
			    break;
			case 6:
			    message = "File is invalid";
			    break;
			case 7:
			    message = "File not suported";
			    break;	
			case 8:
			    message = "File is broken";
			    break;
			case 9:
			    message = "";
			    break;
			case 10:
			    message = "";
			    break;
			case 11:
			    message = "";
			    break;
			case 12:
			    message = "";
			    break;
			case 13:
			    message = "";
			    break;				
			case 14:
			    message = "";
			    break;
			case 15:
			    message = "";
			    break;
			case 16:
			    message = "";
			    break;
			case 17:
			    message = "";
			    break;
			case 18:
			    message = "";
			    break;
			case 19:
			    message = "";
			    break;
			    case 20:
			        message = "";
			        break;				
		    }
		    std::cout << message << std::endl;
			push(message);
		}
		return error;
	}
	////////////////////
	static unsigned int opengl_error(std::string filename = DOKUN_FILE, std::string line = DOKUN_LINE)
	{
		unsigned int error_code = 0;
    #ifdef DOKUN_OPENGL	
	    while((error_code = glGetError()) != GL_NO_ERROR)
	    {
			std::string error;
            switch(error_code)
		    {
			    case GL_INVALID_ENUM:
			        error = "OpenGL Error: INVALID ENUM";
			        break;
			    case GL_INVALID_VALUE:
			        error = "OpenGL Error: INVALID VALUE";
			        break;
			    case GL_INVALID_OPERATION:
			        error = "OpenGL Error: INVALID OPERATION";
			        break;
			    case GL_STACK_OVERFLOW:
			        error = "OpenGL Error: STACK OVERFLOW";
			        break;
			    case GL_STACK_UNDERFLOW:
			        error = "OpenGL Error: STACK UNDERFLOW";
			        break;
			    case GL_OUT_OF_MEMORY:
			        error = "OpenGL Error: OUT OF MEMORY";
			        break;
			    case GL_INVALID_FRAMEBUFFER_OPERATION:
			        error = "OpenGL Error: INVALID FRAMEBUFFER OPERATION";
			        break;				
		    }
		    Logger(error, filename, line);
	    }
	#endif  	
		return error_code; 
	}
	////////////////////
	static int vulkan_error(int result, std::string filename = DOKUN_FILE, std::string line = DOKUN_LINE)
	{
#ifdef DOKUN_VULKAN
        switch(result)
        {
            case VK_SUCCESS                          : Logger("VK_SUCCESS", filename, line); break;
            case VK_NOT_READY                        : Logger("VK_NOT_READY", filename, line); break;
            case VK_TIMEOUT                          : Logger("VK_TIMEOUT", filename, line); break;
            case VK_EVENT_SET                        : Logger("VK_EVENT_SET", filename, line); break;
            case VK_EVENT_RESET                      : Logger("VK_EVENT_RESET", filename, line); break;
            case VK_INCOMPLETE                       : Logger("VK_INCOMPLETE", filename, line); break;
            case VK_ERROR_OUT_OF_HOST_MEMORY         : Logger("VK_ERROR_OUT_OF_HOST_MEMORY", filename, line); break;
            case VK_ERROR_OUT_OF_DEVICE_MEMORY       : Logger("VK_ERROR_OUT_OF_DEVICE_MEMORY", filename, line); break;
            case VK_ERROR_INITIALIZATION_FAILED      : Logger("VK_ERROR_INITIALIZATION_FAILED", filename, line); break;
            case VK_ERROR_DEVICE_LOST                : Logger("VK_ERROR_DEVICE_LOST", filename, line); break;
            case VK_ERROR_MEMORY_MAP_FAILED          : Logger("VK_ERROR_MEMORY_MAP_FAILED", filename, line); break;
            case VK_ERROR_LAYER_NOT_PRESENT          : Logger("VK_ERROR_LAYER_NOT_PRESENT", filename, line); break;
            case VK_ERROR_EXTENSION_NOT_PRESENT      : Logger("VK_ERROR_EXTENSION_NOT_PRESENT", filename, line); break;
            case VK_ERROR_FEATURE_NOT_PRESENT        : Logger("VK_ERROR_FEATURE_NOT_PRESENT", filename, line); break;
            case VK_ERROR_INCOMPATIBLE_DRIVER        : Logger("VK_ERROR_INCOMPATIBLE_DRIVER", filename, line); break;
            case VK_ERROR_TOO_MANY_OBJECTS           : Logger("VK_ERROR_TOO_MANY_OBJECTS", filename, line); break;
            case VK_ERROR_FORMAT_NOT_SUPPORTED       : Logger("VK_ERROR_FORMAT_NOT_SUPPORTED", filename, line); break;
            //case VK_ERROR_FRAGMENTED_POOL            : Logger("VK_ERROR_FRAGMENTED_POOL", filename, line); break;
            //case VK_ERROR_OUT_OF_POOL_MEMORY         : Logger("VK_ERROR_OUT_OF_POOL_MEMORY", filename, line); break;
            //case VK_ERROR_INVALID_EXTERNAL_HANDLE    : Logger("VK_ERROR_INVALID_EXTERNAL_HANDLE", filename, line); break;
            case VK_ERROR_SURFACE_LOST_KHR           : Logger("VK_ERROR_SURFACE_LOST", filename, line); break;
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR   : Logger("VK_ERROR_NATIVE_WINDOW_IN_USE", filename, line); break;
            case VK_SUBOPTIMAL_KHR                   : Logger("VK_SUBOPTIMAL", filename, line); break;
            case VK_ERROR_OUT_OF_DATE_KHR            : Logger("VK_ERROR_OUT_OF_DATE", filename, line); break;
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR   : Logger("VK_ERROR_INCOMPATIBLE_DISPLAY", filename, line); break;
            case VK_ERROR_VALIDATION_FAILED_EXT      : Logger("VK_ERROR_VALIDATION_FAILED_EXT", filename, line); break;
            //case VK_ERROR_INVALID_SHADER_NV          : Logger("VK_ERROR_INVALID_SHADER_NV", filename, line); break;
            //case VK_ERROR_FRAGMENTATION_EXT          : Logger("VK_ERROR_FRAGMENTATION_EXT", filename, line); break;
            //case VK_ERROR_NOT_PERMITTED_EXT          : Logger("VK_ERROR_NOT_PERMITTED_EXT", filename, line); break;
            //case VK_ERROR_OUT_OF_POOL_MEMORY_KHR     : Logger("VK_ERROR_OUT_OF_POOL_MEMORY", filename, line); break;
            //case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: Logger("VK_ERROR_INVALID_EXTERNAL_HANDLE", filename, line); break;
            //case : Logger("", DOKUN_FILE, DOKUN_LINE); break;
        }
#endif	
        return result;
	}
	////////////////////
	static void lua_err(lua_State *L, const String& message, bool print_out = 1) // posts a Lua error + saves to LOG
	{
	    std::ofstream file("log/doklog_lua.txt", std::ios::out | std::ios::app);
		lua_Debug info;
        for(int level = 0; level < 5; level++) 
		{
		    lua_getstack(L, level, &info); // get stack
		    lua_getinfo(L, "nSltufL", &info); // store data in debug struct
		    if(level == 0) // level 0 = main function
		    {
		        file << "stdin:1: " << message.str() << std::endl;
		        file << "stack traceback:" << std::endl;
		        file << "\t[C]: in function '" << info.name << "'" << std::endl;
				//file << lua_file_name << std::endl; // file name
		    }
		    if(level > 0) // within other functions
		    {
			    if(info.name) // function_name
			        file << "\t" << info.short_src << ":" << info.currentline << ":" << " in function '" << info.name << "'" << std::endl;
            }
		}
		file << "\tstdin:1: in main chunk" << std::endl;
		file << "\t[C]: in ?" << std::endl << std::endl;			
		// show error
		if(print_out != 0)
		{
		    std::string mes_str = "stdin:1: " + message.str();
		    lua_pushstring(L, mes_str.c_str());
	        lua_error(L);
		}
		// close file
	    file.close();			
	}
	/////////
	static int lua_error_test(lua_State * L) // same as function "error" except this saves it to a log
	{
		luaL_checktype(L, 1, LUA_TSTRING);
		bool print_out = 1; // print to console
        luaL_optinteger(L, 2, print_out);
		lua_err(L, lua_tostring(L, 1), lua_tointeger(L, 2));
		return 0;
	}
	/////////	
	static void print_instance(const String& class_name)
	{
		if(String::lower(class_name.str()) == "sprite") std::cout << "Sprite count: " << Factory::get_sprite_factory()->get_size() << std::endl;
		if(String::lower(class_name.str()) == "model" || String::lower(class_name.str()) == "mesh") std::cout << "Model  count: " << Factory::get_model_factory ()->get_size() << std::endl;
		if(String::lower(class_name.str()) == "gui"   || String::lower(class_name.str()) == "ui"  ) std::cout << "GUI    count: " << Factory::get_gui_factory   ()->get_size() << std::endl;
		if(String::lower(class_name.str()) == "image") std::cout   << "Image count " << Factory::get_image_factory()->get_size() << std::endl;
		if(String::lower(class_name.str()) == "texture") std::cout << "Texture count " << Factory::get_texture_factory()->get_size() << std::endl;
		if(String::lower(class_name.str()) == "font") std::cout << "Font count " << Factory::get_font_factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
		//if(String::lower(class_name.str()) == "") std::cout << " count " << Factory::get_._factory()->get_size() << std::endl;
	    
	    //push("instance count \n\t")
	    //push(Factory::get_._factory()->get_size());
	}
	/////////	all of these classes listed below have access to the private members of Logger
	friend class Engine;
	friend class Entity;
	friend class Model;
	friend class Sprite;
	friend class GUI;
	friend class Texture;
	friend class Renderer;
	friend class FONT;
#ifdef __windows__
	friend class Window;
#endif
#ifdef __gnu_linux__
	friend class XWindow;
#endif
	friend std::ostream& operator << (std::ostream& os, const Logger & l) // String s; std::cout << s ;
	{
	    int size = dokun_session.size() - 1;
		if(size >= 0) 
	        os << dokun_session[size] << "\n";
		return os;
	}
private:
	/////////
	static void open()
	{/*
	    Logger::push("=======================================================================================================");
	    Logger::push("Started dokun at " + Logger::format("[%Y-%m-%d  %H:%M:%S %p]"));
	    Logger::push("=======================================================================================================");	
        Logger::push("\n\n");	
	    Logger::push(String("Running dokun version  ") + "1.0.0";//String(ENGINE_VERSION));
	    Logger::push(String("Running Lua version    ") + String::to_string(LUA_VERSION_MAJOR) + "." + String::to_string(LUA_VERSION_MINOR) + "." + String::to_string(LUA_VERSION_RELEASE));
	    // create dir for logfiles
	    if(!File::checkdir("log")) File::makedir("log");
	    // save in append mode
	    if(!Logger::save("log/doklog.txt")) Logger("Failure to save log");
	    // pop a number of times after saving first section
	    for(int i = 0; i < 6; i++) Logger::pop();*/	
	}
	static void close()
	{/*
        Logger::push("\n=======================================================================================================");
        Logger::push("Closed dokun at " + Logger::format("[%Y-%m-%d  %H:%M:%S %p]"));
        Logger::push("=======================================================================================================\n");
		if(!Logger::save("log/doklog.txt")) Logger("Failure to save log");// save session		*/
	}
	/////////	
	static void error_push(const String& error, String file =__FILE__,
	    String function=__FUNCTION__, int line=__LINE__, 
		String date=__DATE__, String time=__TIME__)
	{
		dokun_session.push_back("\n");
		dokun_session.push_back("ERROR_START ===========================================================================================");
		dokun_session.push_back(" message  : " + error.str());
		dokun_session.push_back(" file     : " + file.str());
		dokun_session.push_back(" function : " + function.str());
		dokun_session.push_back(" line     : " + String::to_string(line));
		dokun_session.push_back(" date     : " + date.str());
		dokun_session.push_back(" time     : " + time.str());
	#ifdef __windows__
		dokun_session.push_back(" system   : Win32");
	#endif
	#ifdef __gnu_linux__
		dokun_session.push_back(" system   : GNU/Linux");
	#endif	
	    dokun_session.push_back(" user     : " + System::get_user());
		dokun_session.push_back("ERROR_END =============================================================================================");
	}		
	/////////
	static void error_pop()
	{
		for(int i = 0; i < 11; i++)
			pop();
	}
}; 
/* 
ex. Logger(String("I" " love " "you"));
    Logger("I love you");
    Logger(String("I") + String(" love ") + String("you"));
	Logger(String("I") + std::string(" love ") + std::string("you"));
		
	String str0 = "I love ";
	String str1 = str0 + "Sid";
    Logger(str0 + "Denice" + " and she is " + 20);	
	
	std::cout << Logger("My name is Sid", 0) << std::endl;
*/
#define Logger_full_push(error) Logger::error_push(error, __FILE__, __FUNCTION__, __LINE__, __DATE__, __TIME__)
#define Logger_full_pop ()      Logger::error_pop()
#define DOKUN_OPENGL_ERROR_CHECK(fn, ln)      Logger::opengl_error(fn, ln)
#define DOKUN_OPENGL_ERROR                    DOKUN_OPENGL_ERROR_CHECK
#define DOKUN_VULKAN_ERROR_CHECK(err, fn, ln) Logger::vulkan_error(err, fn, ln)
#define DOKUN_VULKAN_ERROR                    DOKUN_VULKAN_ERROR_CHECK
#endif
#endif
