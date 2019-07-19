#ifndef _PLUGIN
#define _PLUGIN
#include "platform.h"
#include "file.h"
#include "logger.h"

#ifdef __cplusplus
#include <iostream>
#include <vector>
#include <string>
#include <lua.hpp>

class Plugin
{
    public:
	    Plugin();
		Plugin(const std::string& file_name);
		~Plugin();
		bool load(const std::string& file_name); // load a single module(.so, .dll, .dylib)

		#ifdef __windows__
		  bool load_directory(std::string folder, std::string filter = "*.dll*"); // load all modules from a folder
		#endif
		#ifdef __gnu_linux__
		  bool load_directory(std::string folder, std::string filter = "*.so*");
		#endif
		#ifdef __macosx__
		  bool load_directory(std::string folder, std::string filter = "*.dylib*");
		#endif
		void free();
		// getters
		void * get(const std::string& data); // function = dlsym(module, "my_function");
	private:
        #ifdef __windows__
	        HMODULE module;
        #endif
		#ifdef __gnu_linux__
		    void * module;
		#endif
};
#endif
#endif
