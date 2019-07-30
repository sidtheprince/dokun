#ifndef _RESOURCE
#define _RESOURCE

#include "vector.h"
#include "shader.h" // for default shader
#include "texture.h" // for default texture
#include "font.h"
#include "audio.h"
#include "model.h"

#ifdef __windows__
#define DEFAULT_FONT_PATH "C:/Windows/Fonts/ariblk.ttf"
#endif
#ifdef __gnu_linux__
#define DEFAULT_FONT_PATH "/usr/share/fonts/X11/Type1/c0583bt_.pfb"
#endif
#ifdef __macosx__
#define DEFAULT_FONT_PATH "System/Library/Fonts/HelveticaNeue.dfont"
#endif

#ifdef __cplusplus // if c++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Resource // Built-in engine assets; contains all engine-embeded assets(images, fonts, etc.);
{
    public:
        Resource();
		~Resource();

		static bool load();   static int load(lua_State *L); // calls all load functions

		static Texture * get_image(const std::string& name);	
		static Model * get_model(const std::string& name);
		static void * get_font();
		
		static bool is_loaded();
	public:
	    static bool image_load ();
		static bool model_load ();
		static bool audio_load ();
		static bool font_load  ();
        static bool shader_load();
	    // containers
		static std::vector<Texture *> image_array;
		static std::vector<Model *> mesh_array;
		static std::vector<Audio *> audio_array;
		static std::vector<FONT *> font_array;
		static std::vector<Shader *> shader_array;
		// properties
		static bool loaded;
};
#endif
#endif