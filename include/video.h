#ifndef _VIDEO
#define _VIDEO // new!

#include "logger.h"
#include "utility.h"
#include "string.h"
#ifdef __cplusplus // if c++
extern "C" {      // run as c code
#endif	
    #include <theora/theora.h> // video codec (theora)
    #include <theora/theoradec.h>
    #include <theora/theoraenc.h>
    #include <png.h>     // libpng (png)	
#ifdef __cplusplus
}
#endif	

#ifdef __cplusplus // if c++
#include <iostream>
#include <vector>
#include <lua.hpp>

class Video // a cinema class; useful for video game cutscenes and animations
{
public:
	Video();                             static int new_(lua_State *L);
	Video(const std::string& file_name);
	~Video();                        
	bool load(std::string file_name, int vidfmt=0);    static int load(lua_State *L);
	//void draw();	                     static int draw(lua_State *L);
	void play();                         static int play(lua_State *L);
	void pause();                        static int pause(lua_State *L);
	void stop();                         static int stop(lua_State *L);
	// you can record the game or your camera
	static void record(int source);             static int record(lua_State *L); 
	static void screenshot(int window_width, int window_height, std::string file_name="out.png");    static int screenshot(lua_State *L);
	// getters
	int get_width();                     static int get_width (lua_State *L);
	int get_height();                    static int get_height(lua_State *L);
	double get_fps();                    static int get_fps(lua_State *L);
	int get_format();                    static int get_format(lua_State *L);
	void * get_data();                   static int get_data(lua_State *L);
	void * get_audio_data();
private:
	bool load_ogg(const std::string& file_name, int vidfmt); // loads a theora
	static void record_screen();  
	static void record_camera();
    // exporter
	void png_to_theora(); 
	// properties
	std::vector<unsigned char*> data;
	void * audio_data();
	// video
    unsigned int playms; // playback time
    double fps;
    unsigned int width;
    unsigned int height;
    unsigned int format;
    unsigned char *pixels;
	// video recording
	bool is_recording;
	//  audio
    unsigned int aplayms;  /* playback start time in milliseconds. */
    int channels;
    int freq;
    int frames;
    float *samples;  /* frames * channels float32 samples. */
}; // -lopenvx -lvxu
#endif
#endif
