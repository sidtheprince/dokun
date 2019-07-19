#ifndef _LOADER
#define _LOADER

#include "types.h"
#include "utility.h"
#include "file.h"
#include "vector.h"
#include "string.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <string>
#include <cstring> // memcpy
#include <vector>
#include <map> // std::map
#include <fstream>
#include <sstream>
#include <lua.hpp>

class Loader // loads custom engine file formats and config files
{
    public:
	    Loader();
		Loader(const std::string& file_name);
		~Loader();
        // functions		
		void load(const std::string& file_name);
		void clear(); // clears loader cache
		void remove(std::string key); // remove a key(data) from loader cache
		// setters
		void set_key(std::string keyword); // registers a key allowing access to the key'value through get_data()                           adds a keyword to aquire and reloads script
		// getters
		void get_data(std::string key, void * value); // store key's value in 'value'
		void get_data(std::string key, std::string& value);
		void get_data(std::string key, String * value);
		void get_data(std::string key, int * value);
		void get_data(std::string key, double * value);
		void get_data(std::string key, Vector2 * value);
		void get_data(std::string key, Vector3 * value);
		void get_data(std::string key, Vector4 * value);
		void get_data(std::string key, std::function<void (void)>); // functions
		int get_size(std::string type); // returns number of function, string, number, boolean_, etc. that has been loaded 
		int get_size(); // content size; returns number of lines in file 
		static Loader * get_dokun_loader();
		
		std::string get_line(int index);
		std::string get_content(); // returns entire file as string
		
		// boolean_
		bool is_pointer(std::string key);
		bool is_string(std::string key);
		bool is_number(std::string key);
		bool is_boolean_(std::string key);
		bool is_vector(std::string key);
		bool is_function(std::string key);
		bool is_key(std::string key);
		bool is_empty(); // returns true if loader's cache is empty
		// loaders
	    // image formats
		bool load_png(const std::string& file_name, void * data, int * width, int * height);
		void load_png(const std::string& file_name, dokun_Error * error);
	    bool load_jpeg(const std::string& file_name);
		void load_jpeg(const std::string& file_name, dokun_Error * error);		
	    bool load_bmp(const std::string& file_name);
		void load_bmp(const std::string& file_name, dokun_Error * error);
	    bool load_gif(const std::string& file_name);
		void load_gif(const std::string& file_name, dokun_Error * error);
	    bool load_tiff(const std::string& file_name);
		void load_tiff(const std::string& file_name, dokun_Error * error);
		// model formats
		bool load_obj(std::string file_name, std::vector<Vector3>& vertex/*, std::vector<int>& index*/); // custom
		void load_obj(const std::string& file_name, dokun_Error * error);
		bool load_dae(std::string file_name); // OpenCollada : (open-source)
		void load_dae(const std::string& file_name, dokun_Error * error);
		bool load_3ds(std::string file_name); 
		void load_3ds(const std::string& file_name, dokun_Error * error);
		bool load_fbx(std::string file_name); // Autodesk : not open-sourced(might not be used)
		void load_fbx(const std::string& file_name, dokun_Error * error);
        // material formats
		bool load_mtl(const std::string& file_name);
		void load_mtl(const std::string& file_name, dokun_Error * error);
		// audio formats
		bool load_vorbis(const std::string& file_name);
		void load_vorbis(const std::string& file_name, dokun_Error * error);
		// video formats
		bool load_theora(const std::string& file_name);
		// script formats
		bool load_lua(const std::string& file_name);
		bool load_lua(const std::string& file_name, dokun_Error * error);
		// custom formats
		bool load_dokun(const std::string& file_name);  bool load_dokun(lua_State *L, std::string file_name); // ext = .dokun = used as a project file in the ui version of the engine
	    bool load_level(const std::string& file_name); // ext = .level = saves game level data
		bool load_bone(const std::string& file_name); // ext  = .bone = saves skeleton animation data
	private:
		// read methods
		void copy(const std::string& file_name); // instant copy - does not copy line by line :c
		void getline(const std::string& file_name); // get line by line
		void stream(const std::string& file_name); // stream (takes longer)
		
		std::vector<std::string> content;
		
		static Loader * dokun_loader; // just a lua file with another name; only one dokun file per project
		static std::vector<std::string> keyword; // stored .dokun keywords
		std::string file; // name of file
		// data received from loading
		// a data consists of two pieces, a key and a value  e.g   [key] = [value]
		std::map<std::string, void *> userdata;
		std::map<std::string, std::string> string;
        std::map<std::string, double> number;
        std::map<std::string, int> boolean_;	// can also be integer	
        std::map<std::string, Vector4> vector; 
        std::map<std::string, std::function<void (void)>> function;	
};
#endif
#endif