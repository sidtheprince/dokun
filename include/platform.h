#ifndef _PLATFORM
#define _PLATFORM

#ifdef  _WIN32
#define __windows__
#endif

#ifdef __gnu_linux__
#define __ubuntu__ 
#endif

#ifdef __APPLE__
#ifdef __MACH__
#define __macosx__
#endif
#endif

#ifdef __ANDROID__
#define __android__
#endif

#ifdef __gnu_linux__
#ifndef DOKUN_X11 
#ifndef DOKUN_WAYLAND
#ifndef DOKUN_MIR
#ifndef DOKUN_SDL2
#ifndef DOKUN_GLFW
	#define DOKUN_X11
#endif	
#endif	
#endif
#endif
#endif
#endif

#ifdef DOKUN_OPENGL
#define DOKUN_USE_OPENGL
#endif
#ifdef DOKUN_VULKAN
#define DOKUN_USE_VULKAN
#endif
////////////
#ifdef __windows__
    #define __desktop__ // on a desktop
    #if defined(_MSC_VER) && (_MSC_VER >= 1020)
        #pragma warning(disable:4003)
		#pragma warning(disable:4018)
        #pragma warning(disable:4047) 
		#pragma warning(disable:4101) 
		#pragma warning(disable:4133)
		#pragma warning(disable:4244)
        #pragma warning(disable:4267)
        #pragma warning(disable:4273)
		#pragma warning(disable:4305)
		#pragma warning(disable:4311)
        #pragma warning(disable:4312)
        #pragma warning(disable:4390)
        #pragma warning(disable:4700)
        #pragma warning(disable:4715)
        #pragma warning(disable:4724)
		#pragma warning(disable:4800)
		#pragma warning(disable:4996) 
	#endif
	#if defined(__GNUC__) || defined(__GNUG__) // check for both gcc and g++
	    #pragma GCC diagnostic ignored "-Wwrite-strings"
	    #pragma GCC diagnostic ignored "-Wpsabi"
	#endif
	#if defined(__clang__)
	#endif
	#ifdef DOKUN_OPENGL               // use opengl (OPTIONAL), glew (REQUIRED IF USING OPENGL)
		#define GLEW_STATIC
        #include <GL/glew.h>
		#include <GL/wglew.h>
        #include <GL/gl.h>
        #include <GL/glu.h>
	#endif
	#ifdef DOKUN_VULKAN               // use vulkan (OPTIONAL)
	    #define VK_USE_PLATFORM_WIN32_KHR
	    #include <vulkan/vulkan.h>
	#endif
		
	#include "win32_header.h"
#endif
////////////
#ifdef __linux__ 
////////////
#ifdef __gnu_linux__              // GNU/Linux
    #define __desktop__           // on a desktop
	#ifdef DOKUN_OPENGL           // opengl (OPTIONAL), glew (REQUIRED IF USING OPENGL)
		#define GLEW_STATIC
        #include <GL/glew.h>
	    #ifdef DOKUN_X11
			#include <GL/glxew.h>
		#endif	
		#include <GL/gl.h>       				
		#include <GL/glu.h>	
		#ifdef DOKUN_X11
            #include <GL/glx.h>
            #define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
            #define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
            typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
        #endif			
	#endif
		
	#ifdef DOKUN_VULKAN
	    #ifdef DOKUN_X11
	        #define VK_USE_PLATFORM_XLIB_KHR
		#ifdef DOKUN_XCB
			#define VK_USE_PLATFORM_XCB_KHR
		#endif
		#endif
		#ifdef DOKUN_WAYLAND
		    #define VK_USE_PLATFORM_WAYLAND_KHR
		#endif
		#ifdef DOKUN_MIR
		    #define VK_USE_PLATFORM_MIR_KHR
		#endif
		#include <vulkan/vulkan.h>
	#endif
		
	#ifdef DOKUN_X11 
		#include <X11/Xlib.h>       // includes X11/X.h
		#include <X11/Xcms.h>
		#include <X11/Xutil.h>
		#include <X11/Xresource.h>
		#include <X11/Xatom.h>
		#include <X11/cursorfont.h>
		#include <X11/keysym.h>     // includes X11/keysymdef.h;  for keyboard codes
	    //#include <X11/Xlibint.h>    // includes X11/Xproto.h which includes <X11/Xprotostr.h>
        // x keyboard ext : https://www.x.org/releases/X11R7.7/doc/libX11/XKB/xkblib.html#ftn.id2522146
        #include <X11/XKBlib.h> // automatically includes: X11/extensions/XKBstr.h which auto includes X11/extensions/XKB.h, 
        #include <X11/extensions/XKBgeom.h>
	#ifdef DOKUN_XCB	
	    #include <X11/Xlib-xcb.h>
		#include <xcb/xcb.h>
	#endif	
	#endif
	#ifdef DOKUN_WAYLAND
	#endif
	#ifdef DOKUN_MIR
	#endif
			
	#include "linux_header.h"
#endif
////////////
#ifdef __android__                 // Android - based on linux kernel
	#define __mobile__             // on a mobile device
    #ifdef DOKUN_VULKAN            // use vulkan (OPTIONAL)
	    #define VK_USE_PLATFORM_ANDROID_KHR
		#include <vulkan/vulkan.h> 
	#endif
	#include "android_header.h"
#endif
////////////
#endif
////////////
#ifdef __macosx__               // Mac OSX - I never used a mac before
	#define __desktop__         // on a desktop
	#ifdef DOKUN_OPENGL         // use opengl (OPTIONAL)
	    #include <OpenGL/gl.h>
	    #include <OpenGL/glu.h>
    #endif
	#ifdef __OBJC__
        #import <Cocoa/Cocoa.h> // use cocoa (REQUIRED)
	#endif
#endif
//////////////
#endif
