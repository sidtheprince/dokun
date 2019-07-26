#ifndef _FONT
#define _FONT

#include "vector.h"
#include "logger.h"
#include "texture.h"
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
#include <string>
#include <map>
#include <lua.hpp>
// To avoid naming conflictions with X11
#ifndef __gnu_linux__
class Font;
#define FONT Font
#endif

#ifdef __gnu_linux__
class XFont;
#define FONT XFont
#endif

class FONT
{
public:
	    FONT();                                                 static int font_new(lua_State *L);
		FONT(const FONT& font);
		FONT(const std::string& file_name);
		~FONT(); 
        // methods
	    bool load(const std::string& file_name);                static int load(lua_State *L);
		bool load(void * data, unsigned char size);
		void generate();                                        static int generate(lua_State *L); /* stores all characters and their sizes in array*/
		void destroy();                                         static int destroy(lua_State *L);
        void copy(const FONT& font);                            static int copy(lua_State *L);
        // setters
        void set_width(unsigned int width);                     static int set_width(lua_State *L); 
        void set_height(unsigned int height);                   static int set_height(lua_State *L);
        void set_size(unsigned int width, unsigned int height); static int set_size(lua_State *L);
		// getters
		int get_width()const;                                   static int get_width(lua_State *L);
		int get_width(char glyph)const;                         static int get_width_of_glyph(lua_State *L);
		int get_width(const std::string& text) const;		
		int get_height()const;                                  static int get_height(lua_State *L);
		int get_height(char glyph)const;                        static int get_height_of_glyph(lua_State *L);	
		int get_height(const std::string& text)const;
		Vector2 get_size()const;                                static int get_size(lua_State *L);
		Vector2 get_size(char glyph)const;                      //static int get_size(lua_State *L);
		Vector4 get_rect()const;                                static int get_rect(lua_State *L);
		FT_Face get_data()const;                                static int get_data(lua_State *L);
		FT_Face get_face()const;                                static int get_face(lua_State *L);
		FT_Byte * get_data_ptr()const;                          static int get_data_ptr(lua_State *L);
		std::string get_file()const;                            static int get_file(lua_State *L);
		FT_Library get_library()const;
		// default
		static FONT * get_system_font();
		// font info
		//std::map<char, Character> get_glyph(); // get_glyph()[]  // std::map is just a vector with pairs      
		unsigned int get_buffer(); 
	    // boolean
		bool is_generated() const;                              static int is_generated(lua_State * L);
		// friends
		friend class Engine;
		friend class Logger;
		friend class Label;
		static FT_Library library;
    private:
	    static bool open ();
		static void close();
		////////////////////
		unsigned int  width;
		unsigned int height;
		FT_Face data; // face = collection of glyphs (single characters)
		std::string file;
		unsigned int buffer;
		static FONT * system_font; 
		// data->family_name // long
		// data->num_glyphs // string
		// data->style_name // string
		// data->bbox
		// data->units_per_EM
		// data->
		// data->
		// data->
		// data->
	public:	
		typedef struct Character {
			unsigned int id;         // opengl texture / ID handle of the glyph texture
            unsigned int width;      // width  of glyph
			unsigned int height;     // height of glyph
            int bearing_x;           // offset from baseline to left of glyph
            int bearing_y;			 // offset from baseline to top of glyph
            long int advance;        // offset to advance to next glyph
		} Character;	
		std::map<char, Character> character_array; // map is like a std::vector that stores two types
};
#endif
#endif
