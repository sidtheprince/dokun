#ifndef _ENGINE
#define _ENGINE

/*  Dokun Engine 1.0.0            Copyright (C) 2015-2016 Sid Lartey Doku (no company or website)
   
    The following rules apply when using this software:
        1. You are free to use, modify, distribute this software and sell the games you make
         with this software without paying any fees or royalties.
        2. You must not misrepresent the origin of this software
         nor claim ownership of this software.
        3. It is not guaranteed that this software works and the creators
         will not take responsibility for any damages or claims in the use of this software.
        4.
*/

#include "build.h"
#include "platform.h"
#include "types.h" 
#include "math.h"
#include "vector.h"
#include "matrix_test.h"
//#include "quaternion.h" // not in using right now
#include "string.h"
#include "logger.h" 
#include "utility.h"
#include "system.h"
// containers.
#include "window.h"
// graphics.
#include "sprite.h"
#include "model.h"
#include "texture.h"
#include "image.h"
#include "vertex.h"
// input.
#include "mouse.h"
#include "keyboard.h"
#include "joystick.h"
#include "touch.h"
#include "virtual.h" // virtual reality
#include "brain.h" // eeg
// audio.
#include "sound.h"
#include "music.h"
#include "voice.h"
// font
#include "font.h"
// ui
#include "ui.h"
#include "box.h" 
#include "button.h" 
#include "label.h"
#include "edit.h"
#include "progressbar.h"
#include "slider.h"
#include "toggle.h"
#include "scrollbar.h"
#include "list.h"
#include "spinner.h"
#include "menubar.h"
#include "grid.h"
#include "combo.h"
#include "tab.h"
// network.
#include "server.h"
#include "client.h"
// utilities - the most essential part of the engine
#include "entity.h"
#include "component.h"
#include "camera.h"
#include "level.h"
#include "timer.h"
#include "light.h"
#include "shader.h"
#include "resource.h"
#include "animator.h" 
#include "data.h"
#include "factory.h" 
#include "script.h"
#include "console.h"
#include "video.h"
#include "plugin.h"
#include "file.h"
#include "event.h"
#include "renderer.h"
#include "process.h"
#include "http.h"
/////////////
#define ENGINE_NAME      "dokun"
#define ENGINE_AUTHOR    "Sid Lartey Doku"
#define ENGINE_LICENSE   "MIT License" 
#define ENGINE_COPYRIGHT "Copyright (C) 2019 Sid Lartey Doku" // just a placeholder name
/////////////
#define ENGINE_VERSION_MAJOR "1"
#define ENGINE_VERSION_MINOR "0"
#define ENGINE_VERSION_PATCH "0"
#define ENGINE_VERSION ENGINE_VERSION_MAJOR "." ENGINE_VERSION_MINOR "." ENGINE_VERSION_PATCH

#ifdef __cplusplus
class Engine { // The engine initializes the libraries it requires and registers c functions to lua
public:
    Engine(); 
	~Engine();
    // functions
	static bool open();                                           static int open(lua_State *L); // init engine
	static void close();                                          static int close(lua_State *L); // uninitializes engine and closes engine                                                   
	static int test(lua_State *L); // test function : temporary
	static int reg(lua_State *L); // registers the engine's lua module
	// getters
	static Engine    * get_engine(); // returns a singleton object representing the engine
	static bool        get_status(); // returns true if engine is open (initialized), otherwise returns false
private:
    // callbacks
	static bool on_open (); // do stuff on opening the engine
	static void on_close(); // do stuff before closing engine
	// registry (registers functions)
	// singleton
	static Engine * engine_ptr;
	// status
	static bool status;
	// third_party
	#ifdef DOKUN_VULKAN
	    VkApplicationInfo app_info;
	#endif
	#ifdef __windows__
	    WSADATA wsa_data;
	#endif
};
#endif
#endif // rewriting lua : lualib.h (declare engine), linit.c (register engine), engine.cpp(define engine)
