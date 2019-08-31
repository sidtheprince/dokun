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
// vorbis (lossy - meaning quality is lost; like mp3 but better; good for streaming and listening experience)
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h> 
#include <vorbis/vorbisenc.h>
// flac   (lossless- meaning no loss in quality, compressed; like wav but better; good for preserving quality)
#include "share/compat.h"
#include <FLAC/all.h>
// opus   (lossy - meaning quality is lost; may replace ogg due to its better quality) //#include <opus_multistream.h> //#include <opus.h> // may consider using opus another time ...
// openal-soft
#ifdef DOKUN_OPENAL 
#include <AL/al.h>
#include <AL/alc.h>
#endif

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

class Audio // Audio Engine - only purpose is to initialize audio engine
{
public:
	// setters
	static void set_master_volume(double volume); static int set_master_volume(lua_State *L);// volume for all audio
#ifdef DOKUN_OPENAL    
    static void set_device (ALCdevice  *);
    static void set_context(ALCcontext *);
#endif    	
	// getters
	static double get_master_volume();            static int get_master_volume(lua_State *L);
#ifdef DOKUN_OPENAL	
	static ALCdevice  * get_device ();
	static ALCcontext * get_context();
#endif	
	// sound, music, and engine have access to private and protected members of Audio
	friend class Sound;
	friend class Music;
	friend class Voice;
	friend class Engine;
private:	
    static bool open (); // initializes audio_engine
	static void close(); // destroys audio_engine
	static double master_volume;
#ifdef DOKUN_OPENAL
	static ALCdevice  * device;
	static ALCcontext * context;
#endif
#ifdef DOKUN_PORTAUDIO
	PaStream *stream;
#endif
};
/////////////
/////////////
class Audible { // represents an audio object
public:
    // constructor
	Audible();                                                                      static int audible_new(lua_State *L);
	Audible(const std::string& file_name);
	Audible(const std::string& file_name, double volume, bool loop = false);
	Audible(void * data, size_t size, int format, int sample_rate);
	Audible(void * data, size_t size, int bit_rate, int channels, int sample_rate);
	virtual ~Audible();
    // normal
    virtual bool load(const std::string& file_name);                                          static int load(lua_State *L);
	virtual bool load(void * data, size_t size, int format, int sample_rate);
    virtual bool load(void * data, size_t size, int bit_rate, int channels, int sample_rate);                
    void play();                                                                              static int play(lua_State *L);
    void pause();                                                                             static int pause(lua_State *L);
    void stop();	                                                                          static int stop(lua_State *L);
	void rewind();                                                                            static int rewind(lua_State *L);
	// setters
	void set_volume(double volume);                                                           static int set_volume(lua_State *L);
	static void set_default_volume(double volume);
	static void set_master_volume(double volume);                                             static int set_master_volume(lua_State *L);// volume for all audio
	void set_pitch(double pitch);                                                             static int set_pitch(lua_State *L);
	void set_loop(bool loop);                                                                 static int set_loop(lua_State *L);
	void set_position(double x, double y);                                                    static int set_position(lua_State *L);
	void set_position(double x, double y, double z); 
    void set_position(const Vector2& p); 
    void set_position(const Vector3& p);
    void set_listener(double x, double y);                                                    static int set_listener(lua_State *L);
    void set_listener(double x, double y, double z);
    void set_listener(const Vector2& p);
    void set_listener(const Vector3& p);
	// getter - be sure to add const at the end of each "get" function parameter
	double get_volume();                                                                      static int get_volume(lua_State *L);
	static double get_default_volume();                                                       static int get_default_volume(lua_State *L);
	double get_pitch();                                                                       static int get_pitch(lua_State *L);
	double get_duration();                                                                    static int get_duration(lua_State *L);
	Vector3 get_position();                                                                   static int get_position(lua_State *L);
	Vector3 get_listener();                                                                   static int get_listener(lua_State *L);
	void * get_data();                                                                        static int get_data(lua_State *L);
	std::string get_file() const;                                                             static int get_file(lua_State *L);
	std::string get_status();                                                                 static int get_status(lua_State *L);
	unsigned int get_source();
	unsigned int get_buffer();
	// userdata
	std::string get_title () const;                                                           static int get_title(lua_State * L);
	std::string get_artist() const;                                                           static int get_artist(lua_State * L);
	std::string get_album () const;                                                           static int get_album(lua_State * L);
	std::string get_genre () const;                                                           static int get_genre(lua_State * L);
	unsigned int get_track_number() const;                                                    static int get_track_number(lua_State * L);
	std::string get_date() const;                                                             static int get_date(lua_State * L);
	void * get_cover() const;                                                                 static int get_cover(lua_State * L);
	// boolean (status)
	bool is_playing();                                                                        static int is_playing(lua_State *L);
	bool is_paused();                                                                         static int is_paused(lua_State *L);
	bool is_stopped();                                                                        static int is_stopped(lua_State *L);
	bool is_looped();                                                                         static int is_looped(lua_State *L); 
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
	static bool decode_flac(const std::string& file_name); // converts .flac to .wav
	static bool encode_flac(const std::string& file_name); // converts .wav to .flac	
	// callbacks (this)
	FLAC__StreamDecoderWriteStatus write_callback(const FLAC__Frame *frame, const FLAC__int32 * const buffer[]);
	void metadata_callback(const FLAC__StreamMetadata *metadata);
	void error_callback(FLAC__StreamDecoderErrorStatus status);
	// callbacks (decoder - for reading) // will not work unless callbacks are static
	static FLAC__StreamDecoderWriteStatus decoder_write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);
	static void decoder_metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
	static void decoder_error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);
	// callbacks (encoder - for writing) // will not work unless callbacks are static
	static void encoder_progress_callback(const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data);
	// helper
	static FLAC__bool write_little_endian_uint16(FILE* f, FLAC__uint16 x);
	static FLAC__bool write_little_endian_int16(FILE* f, FLAC__int16 x);
	static FLAC__bool write_little_endian_uint32(FILE* f, FLAC__uint32 x);
	// sound and music have access to private and protected members of Audible
	friend class Sound;
	friend class Music;
	friend class Voice;
	friend class Engine;	   	
private:
	// property (internal)
	std::vector <char> data;
	std::string file;
	unsigned int source;
	unsigned int buffer;
	// format		
	unsigned int bits_per_sample; // 8 bit, 16 bit for 2 channel stereo
	unsigned int sample_rate;     // same as frequency (eg. 44100, 22050, etc.)
	unsigned int channels;        // 1=mono, 2=stereo
	unsigned int total_samples;
	unsigned int size;
	unsigned int index;
	// for decoding
	static unsigned int bits_per_sample_dec; // 8 bit, 16 bit for 2 channel stereo
	static unsigned int sample_rate_dec;     // same as frequency (eg. 44100, 22050, etc.)
	static unsigned int channels_dec;        // 1=mono, 2=stereo
	static FLAC__uint64 total_samples_dec; // for decoding
	static unsigned int format_dec;
	// for encoding
	static unsigned int total_samples_enc;
	// frames = 4096
	// property (external) 
    static int default_volume;
	double volume;
	double pitch;
	double duration;
	Vector3 position;
	Vector3 listener;
	// property (status)
	bool playing;
	bool paused;
	bool stopped;
	bool looped;
	// user data
	std::string title;
    std::string artist;
    std::string genre;
    std::string album;
    unsigned int track_number;
    std::string date;
    void */*unsigned char **/ cover_art; 
};
/////////////
#endif
#endif
