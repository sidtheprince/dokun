#ifndef _VOICE
#define _VOICE

#include "audio.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Voice { // for voice recording and playback; will use the .opus for interactive speech; can handle a wide range of interactive audio applications, including Voice over IP, videoconferencing, in-game chat, and even live, distributed music performances
    public:
        Voice(); static int new_(lua_State *L);
        ~Voice();
  
        void start();  static int start(lua_State *L);
        void stop();   static int stop(lua_State *L); // stops recording or playing
        void play();   static int play(lua_State *L);
        void pause();  static int pause(lua_State *L);
		void save(const std::string& file_name);   static int save(lua_State *L); // save as .ogg
	    // setters
		static void set_volume(int volume); static int set_volume(lua_State *L);
		// getters
		void * get_data();    static int get_data(lua_State *L); // returns sound data
		// additional feature: text-to-speech coming soon!      void say(const std::string& text);
	    bool is_recording();  static int is_recording(lua_State *L); // returns true if recording otherwise returns false
		bool is_playing();    static int is_playing(lua_State *L); // returns true if playing otherwise returns false
		bool is_paused();     static int is_paused(lua_State *L);
		bool is_stopped();    static int is_stopped(lua_State *L);
	//private:   
	    bool encode_opus(const std::string& out); // write opus
		bool decode_opus(const std::string& file_name); // read opus
		void vorbis_to_opus(const std::string& in, std::string out);
	    void * get_device();  static int get_device(lua_State *L); // returns device handle
        ALCdevice * device;	
		unsigned int source;
		unsigned int buffer;
 		std::vector<unsigned char> data;
        int sample;
		bool recording;
		static double volume; // default play volume
		struct mic_data
		{
			double volume;
		} mic_data;
};
#endif
#endif