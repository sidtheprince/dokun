#ifndef _LOGGER
#define _LOGGER

#define DOKUN_FUNCTION  String(__FUNCTION__).str()
#define DOKUN_FILE      String(__FILE__).str()
#define DOKUN_LINE      String(__LINE__).str()
#define DOKUN_DATE      String(__DATE__).str()
#define DOKUN_TIME      String(__TIME__).str()
#define DOKUN_TAG       String("dokun: ") +
#define DOKUN_LOGTAG    "(dokun): " + DOKUN_FILE.substr(DOKUN_FILE.find_last_of("\\/") + 1) + " (" + DOKUN_LINE + "): "      //+ "function (" + DOKUN_FUNCTION + "): "  // added on 10-1-2018
#define DOKUN_CLASS     String(typeid(*this).name()).str()
#define DOKUN_LOGFILE   "doklog.txt"

#include "platform.h" // for platform-specific functions
#include "types.h"
#include "utility.h"
#include "string.h"
#include "system.h"
#include "factory.h"
// for rendering text
#ifdef __cplusplus // if c++ 
extern "C" {       // run as c code
#endif
    #include <ft2build.h>
    #include FT_FREETYPE_H
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus // if c++
#include <iostream>
#include <typeinfo>
#include <lua.hpp>
// glm (temporary)
//#define use_glm
/*#ifdef use_glm
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif
*/
static std::vector<std::string> dokun_session = {};// stores all logs from current dokun_session
struct Logger {
	Logger() {}
	Logger(int code) { std::cerr << print_error(code) << std::endl; }
	Logger(const String& message, /*std::string function = DOKUN_FUNCTION,*/ std::string filename = DOKUN_FILE, std::string line = DOKUN_LINE) // set print_out to 0 if using std::cout << operator
	{
		std::cout << DOKUN_TAG message << std::endl; // print message on console
        
        String LOGTAG = "(dokun): " + filename.substr(filename.find_last_of("\\/") + 1) + " (" + line + "): ";
		    //+ "function (" + function + "): ";
		push(LOGTAG + message); // push message to log
	}
	Logger(int x, int y, int width, int height, const String& text, void * data = nullptr, int red = 255, int green = 255, int blue = 255, int alpha = 255)
	{
		draw(x, y, width, height, text, data, red, green, blue, alpha);
	}
	~Logger() {}
	/////////	
	static void draw(int x, int y, int width, int height, const String& text, void * data = nullptr, int red = 255, int green = 255, int blue = 255, int alpha = 255)
	{/*
		push(text); // push text
		if(data == nullptr) {Logger("No font detected"); return;}
	    if(text.empty()    ) {Logger("String is empty "); return;}
		double sx = 0.5; double sy = 0.5;
		int window_width, window_height;		
    #ifdef DOKUN_OPENGL	// OpenGL is defined
    #ifdef __windows__
	    if(!wglGetCurrentContext())
	    {
		    Logger("Rendering Failed : No OpenGL Context found");
		    return;
	    }
		RECT rect;
        GetWindowRect(GetActiveWindow(), &rect);
        window_width  = rect.right - rect.left;
        window_height = rect.bottom - rect.top;
    #endif	
    #ifdef __gnu_linux__
    #ifdef DOKUN_X11
	    if(!glXGetCurrentContext())
	    {
		    Logger("Rendering failed : No OpenGL Context found");
		    return;
	    }
		Window winFocus;
        int    revert;
        Display * display = XOpenDisplay(nullptr);
		XGetInputFocus(display, &winFocus, &revert);
		XWindowAttributes gwa;
	    XGetWindowAttributes(display, winFocus, &gwa); // get attributes while app is running; only gets width and height no x and y
	    window_width  = gwa.width ;
	    window_height = gwa.height; 
		XCloseDisplay(display);
		display = nullptr;
    #endif
    #endif
	    glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		//glViewport(0, 0, window_width, window_height);
        // vertex shader
	    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	    const char * vertex_source[] =
        {
		    "#version 400\n"
            "layout (location = 0) in vec4 vertex;\n"
            "\n"
		    "out vec2 Texcoord;\n"
		    "\n"
		    "uniform mat4 model;\n"
		    "uniform mat4 proj;\n"
		    "uniform mat4 view;\n"
		    "\n"
            "void main(void)\n"
		    "{\n"
			    "Texcoord    = vec2(vertex.z, 1.0-vertex.w);\n" // flip tex if top-left corner
                "gl_Position = proj * vec4(vertex.xy, 0.0, 1.0);\n"
		    "}\n"
	    };
	    glShaderSource(vertex_shader, sizeof(vertex_source)/sizeof(vertex_source), vertex_source, nullptr);
	    glCompileShader(vertex_shader);
	    // fragment shader
	    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	    const char * fragment_source[] =
        {
	        "#version 400                                                          \n"
            "\n"
            "out vec4 out_color;\n"
            "in vec2 Texcoord  ;\n"
		    "\n"
            "uniform sampler2D font;\n"
            "uniform vec3 color    ;\n"
            "\n"
            "void main(void) {\n"
                "\n"
			    "out_color = vec4(color, 1.0) * vec4(1.0, 1.0, 1.0, texture(font, Texcoord).r);\n"
            "}\n"
	    };
	    glShaderSource(fragment_shader, sizeof(fragment_source)/sizeof(fragment_source), fragment_source, nullptr);
	    glCompileShader(fragment_shader);
	    // program
	    GLuint program = glCreateProgram();
	    glAttachShader(program, vertex_shader  );
	    glAttachShader(program, fragment_shader);
	    glLinkProgram(program);
	    GLint status; char buffer[512];
	    glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (!status) 
		{
			glGetProgramInfoLog(program, 512, nullptr, buffer);
            Logger(String("Program linking failed ") + String::to_string(buffer));
        }
	    glDetachShader(program, vertex_shader  );
	    glDetachShader(program, fragment_shader);
	    glDeleteShader(vertex_shader  );
	    glDeleteShader(fragment_shader);
	    glUseProgram(program);
	    // uniform
        glUniform3f(glGetUniformLocation(program, "color"), (red / 255), (green / 255), (blue / 255)); // alpha	
	    glm::mat4 model;
		model = glm::translate(model, glm::vec3(0, 0, -1));
	    model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
	    model = glm::scale(model, glm::vec3(1, 1, 1));
	    glm::mat4 proj = glm::ortho(0.0f,  static_cast<float>(window_width), static_cast<float>(window_height), 0.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0));
	    glUniformMatrix4fv(glGetUniformLocation(program, "model"),  1, false, glm::value_ptr(model));
	    glUniformMatrix4fv(glGetUniformLocation(program, "proj" ),  1, false, glm::value_ptr(proj) );
	    glUniformMatrix4fv(glGetUniformLocation(program, "view" ),  1, false, glm::value_ptr(view) );
	    /////////////////////////////////////
	    std::map<char, dokun_Character> Characters;
	    for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(static_cast<FT_Face>(data), c, FT_LOAD_RENDER)) { Logger("ERROR::FREETYTPE: Failed to load Glyph"); continue; }
		// generate texture for each glyph (character)
            GLuint texture;
		    glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<FT_Face>(data)->glyph->bitmap.width, static_cast<FT_Face>(data)->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, static_cast<FT_Face>(data)->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
		    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		    glBindTexture(GL_TEXTURE_2D, 0);
		    // Now store character for later use
            dokun_Character character = { texture, Vector2(static_cast<FT_Face>(data)->glyph->bitmap.width, static_cast<FT_Face>(data)->glyph->bitmap.rows), Vector2(static_cast<FT_Face>(data)->glyph->bitmap_left, static_cast<FT_Face>(data)->glyph->bitmap_top), static_cast<FT_Face>(data)->glyph->advance.x };
            Characters.insert(std::pair<GLchar, dokun_Character>(c, character));
        }	
	    // vertex array obj
	    GLuint vertex_array_obj;
        glGenVertexArrays(1, &vertex_array_obj);
	    // vertex buffer obj
        glBindVertexArray(vertex_array_obj);
		    GLuint vertex_buffer_obj;
			glGenBuffers(1, &vertex_buffer_obj);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);      
	    glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(vertex_array_obj);
        // Iterate through all characters
        for (std::string::const_iterator c = text.begin(); c != text.end(); c++) // scan through all characters in string
        {
            dokun_Character ch = Characters[*c];  // will change Character

            GLfloat xpos = x + ch.bearing.x               * sx;
            GLfloat ypos = y - (ch.size.y - ch.bearing.y) * sy;

            GLfloat w = ch.size.x * sx;
            GLfloat h = ch.size.y * sy;
		
            // Update vertex_buffer_obj for each character
            GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.texture);
            // Update content of vertex_buffer_obj memory
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * sx; // Bitshift by 6 to get value in pixels (2^6 = 64)
	    }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
	    // Clean textures - delete all textures in Character list
	    for(int i = 0; i < Characters.size(); i++)
	        glDeleteTextures(1, & Characters[i].texture);
	    Characters.clear(); // remove all elements from map
	    // buffers
	    glDeleteBuffers(1, & vertex_buffer_obj);
	    // arrays
	    glDeleteVertexArrays(1, & vertex_array_obj);
	    // disable
	    glDisable(GL_BLEND);
	    // program
	    glUseProgram(0);
        glDeleteProgram(program);
    #endif	
    */
	}
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
	/////////
	static std::string format(const std::string& fmt) // returns date and time as a string
	{
        // C++11 style - Unfortunately a few compilers dont support std::put_time :c
        /*
		auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now); // current time
		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), fmt.c_str());
		return ss.str();
        */		
        std::time_t timer;
        char buffer[26];
        struct std::tm* tm_info;
        std::time(&timer);
        tm_info = std::localtime(&timer);	
		std::strftime(buffer, 26, fmt.c_str(), tm_info);
		return buffer;		
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
	static void push(const String& message) // stores a log message from current dokun_session
	{
	    dokun_session.push_back(message.str());
	}
    //////////
	static void pop()
	{
		dokun_session.pop_back();
	}
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
