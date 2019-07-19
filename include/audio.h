#ifndef _AUDIO
#define _AUDIO

#include "platform.h"
#include "utility.h"
#include "factory.h"
#include "vector.h"
#include "logger.h"
#ifdef __cplusplus // if c++
extern "C" {      // run as c code
#endif
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h> 
#include <vorbis/vorbisenc.h>
//#include <FLAC/all.h>
//#include <opus_multistream.h>
//#include <opus.h>
#include <AL/al.h>
#include <AL/alc.h>
//#include <portaudio.h>
#ifdef __cplusplus // if c++
}
#endif

#ifdef __cplusplus
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <fstream>
#include <lua.hpp>

class Audio // Audio Manager - only purpose is to initialize audio engine
{
public:
    // constructor
	Audio();                                             static int create(lua_State *L);
	Audio(const std::string& file_name);
	Audio(const std::string& file_name, double volume, bool loop = false);
	Audio(void * data, size_t size, int format, int sample_rate);
	Audio(void * data, size_t size, int bit_rate, int channels, int sample_rate);
	virtual ~Audio();
    // normal
    virtual bool load(const std::string& file_name);     static int load(lua_State *L);
	virtual bool load(void * data, size_t size, int format, int sample_rate);
    virtual bool load(void * data, size_t size, int bit_rate, int channels, int sample_rate);                 
    void play();                                         static int play(lua_State *L);
    void pause();                                        static int pause(lua_State *L);
    void stop();	                                     static int stop(lua_State *L);
	void rewind();                                       static int rewind(lua_State *L);
	// setter
	void set_volume(double volume);                      static int set_volume(lua_State *L);
	static void set_default_volume(double volume);
	static void set_master_volume(double volume); static int set_master_volume(lua_State *L);// volume for all audio
	void set_pitch(double pitch);                        static int set_pitch(lua_State *L);
	void set_loop(bool loop);                            static int set_loop(lua_State *L);
	void set_position(double x, double y);               static int set_position(lua_State *L);
	void set_position(double x, double y, double z); 
    void set_position(const Vector2& p); 
    void set_position(const Vector3& p);
    void set_listener(double x, double y);               static int set_listener(lua_State *L);
    void set_listener(double x, double y, double z);
    void set_listener(const Vector2& p);
    void set_listener(const Vector3& p);
    // openal
    static void set_device(ALCdevice *);
    static void set_context(ALCcontext *);	
	// getter
	double get_volume();                                 static int get_volume(lua_State *L);
	static double get_default_volume();                  static int get_default_volume(lua_State *L);
	double get_pitch();                                  static int get_pitch(lua_State *L);
	double get_duration();                               static int get_duration(lua_State *L);
	Vector3 get_position();                              static int get_position(lua_State *L);
	Vector3 get_listener();                              static int get_listener(lua_State *L);
	// special
	void * get_data();                                   static int get_data(lua_State *L);
	std::string get_status();                            static int get_status(lua_State *L);
	unsigned int get_source();
	unsigned int get_buffer();
	// openal
	static ALCdevice * get_device  ();
	static ALCcontext * get_context();
	// boolean (status)
	bool is_playing();                                   static int is_playing(lua_State *L);
	bool is_paused();                                    static int is_paused(lua_State *L);
	bool is_stopped();                                   static int is_stopped(lua_State *L);
	bool is_looped();                                    static int is_looped(lua_State *L);
protected:
    // format
	bool is_raw(const std::string& file_name);
	bool is_pcm(const std::string& file_name); // same as is_raw
	bool is_ogg(const std::string& file_name);
	bool is_wav(const std::string& file_name); // also load flac, xspf
    // loader
    bool load_ogg(const std::string& file_name); bool load_vorbis(const std::string& file_name);
	bool load_wav(const std::string& file_name);
	bool load_raw(const std::string& file_name); bool load_pcm(const std::string& file_name);
	bool load_flac(const std::string& file_name);
	// exporter
	void wav_to_ogg(const std::string& in_file, const std::string& out_file); void wave_to_vorbis(const std::string& in_file, const std::string& out_file);
	// sound and music have access to private and protected members of Audio
	friend class Sound;
	friend class Music;
	friend class Voice;
	friend class Engine;
private:	
    static bool open ();
	static void close();
	// property (internal)
	std::vector <char> data;
	unsigned int source;
	unsigned int buffer;		
	int bits_per_sample; // 8 bit, 16 bit
	int sample_rate;    // like 44100, 22050, etc...
	int channels;        // 1 mono, 2 stereo
	// frames = 4096
	// property (external) 
    static int default_volume;
	double volume;
	double pitch;
	bool duration;
	Vector3 position;
	Vector3 listener;
	// property (status)
	bool playing;
	bool paused;
	bool stopped;
	bool looped;
private:	
	// openal
	static ALCdevice  *   device;
	static ALCcontext *  context;
private:	
    // portaudio
	#ifdef DOKUN_PORTAUDIO
	     PaStream *stream;
	#endif
};
#endif
#endif
