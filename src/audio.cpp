#include "../include/audio.h"

Audio::Audio() : volume(100), pitch(1.0), duration(0),
    playing(false), paused(false), stopped(false), looped(false),
	bits_per_sample(0), sample_rate(0), channels(0)
{
	// generate
	//#ifdef DOKUN_OPENAL
	    alGenBuffers(1, static_cast<ALuint*>(&buffer));
	    alGenSources(1, static_cast<ALuint*>(&source));
	//#endif
	#ifdef DOKUN_PORTAUDIO
	/*
     Pa_OpenDefaultStream	(& stream,
int 	numInputChannels,
int 	numOutputChannels,
PaSampleFormat 	sampleFormat,
sample_rate,
unsigned long 	framesPerBuffer,
PaStreamCallback * 	streamCallback,
void * 	userData
)			*/
	#endif
	// set
	set_volume(get_default_volume());
	// store
	Factory::get_audio_factory()->store(this);
}
/////////////
Audio::Audio(const std::string& file_name) : playing(false), paused(false), stopped(false), looped(false), bits_per_sample(0), sample_rate(0), channels(0)
{
	// generate
	//#ifdef DOKUN_OPENAL
	    alGenBuffers(1, static_cast<ALuint*>(&buffer));
	    alGenSources(1, static_cast<ALuint*>(&source));
	//#endif
	// load
	if(!load(file_name))
    {
        Logger("Could not load file " + file_name);
    }
	// set
	set_volume(get_default_volume());
	set_pitch(1.0);
	set_position(0, 0, 0);
	set_listener(0, 0, 0);
	// store
	Factory::get_audio_factory()->store(this);
}
/////////////
Audio::Audio(const std::string& file_name, double volume, bool loop) : playing(false), paused(false), stopped(false), bits_per_sample(0), sample_rate(0), channels(0)
{
	// generate
	//#ifdef DOKUN_OPENAL
	    alGenBuffers(1, static_cast<ALuint*>(&buffer));
	    alGenSources(1, static_cast<ALuint*>(&source));
	//#endif
	// load
	if(!load(file_name))
    {
        Logger("Could not load file " + file_name);
    }
	// set
	set_volume(volume);
	set_pitch(1.0);
	set_loop(loop);
	set_position(0, 0, 0);
	set_listener(0, 0, 0);
	// store
	Factory::get_audio_factory()->store(this);
}
/////////////
Audio::Audio(void * data, size_t size, int format, int sample_rate) : playing(false), paused(false), stopped(false), looped(false), bits_per_sample(0), sample_rate(0), channels(0)
{
	// generate
	//#ifdef DOKUN_OPENAL
	    alGenBuffers(1, static_cast<ALuint*>(&buffer));
	    alGenSources(1, static_cast<ALuint*>(&source));
	//#endif
	// load
	if(!load(data, size, format, sample_rate))
	{
		Logger("Could not read data");
	}
	// set
	set_volume(get_default_volume());
	set_pitch(1.0);
	set_position(0, 0, 0);
	set_listener(0, 0, 0);
	// store
	Factory::get_audio_factory()->store(this);
}
/////////////
Audio::Audio(void * data, size_t size, int bit_rate, int channels, int sample_rate) : playing(false), paused(false), stopped(false), looped(false), bits_per_sample(0), sample_rate(0), channels(0)
{
	// generate
	//#ifdef DOKUN_OPENAL
	    alGenBuffers(1, static_cast<ALuint*>(&buffer));
	    alGenSources(1, static_cast<ALuint*>(&source));
	//#endif
	// load
	if(!load(data, size, bit_rate, channels, sample_rate))
	{
		Logger("Could not read data");
	}
	// set
	set_volume(get_default_volume());
	set_pitch(1.0);
	set_position(0, 0, 0);
	set_listener(0, 0, 0);
	// store
	Factory::get_audio_factory()->store(this);
}
/////////////
Audio::~Audio()
{
	if (is_playing())
		stop();
//#ifdef DOKUN_OPENAL
    alDeleteSources(1, static_cast<ALuint*>(&source));
    alDeleteBuffers(1, static_cast<ALuint*>(&buffer));
//#endif

#ifdef DOKUN_PORTAUDIO
	Pa_CloseStream (stream); // free up resources
#endif
	Factory::get_audio_factory()->release(this);
}
/////////////
//#ifdef DOKUN_OPENAL
    ALCdevice  *
//#else
	//void *
//#endif
Audio::device  (nullptr);
//#ifdef DOKUN_OPENAL
    ALCcontext *
//#else
	//void *
//#endif
Audio::context (nullptr);
/////////////
int Audio::default_volume (100);
/////////////
/////////////
bool Audio::open()
{
	device = alcOpenDevice(NULL);
	if (!device) {
		return false;
	}
    context = alcCreateContext(device, NULL);
	if(!context) {
		return false;
	}
	alcMakeContextCurrent(context);
	return true;
}
void Audio::close()
{
    alcMakeContextCurrent (nullptr);
    alcDestroyContext     (context) ;
    alcCloseDevice        (device ) ;
}
/////////////
/////////////
    // normal
bool Audio::load(const std::string& file_name)
{
	std::string extension = String::lower(file_name.substr(file_name.find_last_of(".") + 1));
	if(extension == "wav" || extension == "wave")
	{
		return load_wav(file_name);
	}
	if(extension == "raw" || extension == "pcm")
	{
		return load_pcm(file_name);
	}
	return load_ogg(file_name);
}
/////////////
bool Audio::load(void * data, size_t size, int format, int sample_rate)
{
	if(data == nullptr) return false;
//#ifdef DOKUN_OPENAL
	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(static_cast<ALuint>(buffer), format, data, static_cast<ALsizei>(size), sample_rate);
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
//#endif
	return true;
}
/////////////
bool Audio::load(void * data, size_t size, int bit_rate, int channels, int sample_rate)
{
	ALuint format;
    if(data == nullptr)
	{
		return false;
	}
    if(bit_rate == 8) // 8-bit
	{
	    (channels == 1) ? format = AL_FORMAT_MONO8 : format = AL_FORMAT_STEREO8;
	}
	if(bit_rate == 16) // 16-bit
	{
		(channels == 1) ? format = AL_FORMAT_MONO16 : format = AL_FORMAT_STEREO16;
	}
	//#ifdef DOKUN_OPENAL
	    alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	    alBufferData(static_cast<ALuint>(buffer), format, data, static_cast<ALsizei>(size), sample_rate);
	    alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	//#endif
	return true;
}
/////////////
int Audio::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // string or lightuserdata
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TLIGHTUSERDATA) // works!
		{
			void * data = (void *)lua_touserdata(L, 2); // get data
			luaL_checktype(L, 3, LUA_TNUMBER);  // get size
			luaL_checktype(L, 4, LUA_TNUMBER);  // get bitrate
			luaL_checktype(L, 5, LUA_TNUMBER);  // get channels
			luaL_checktype(L, 6, LUA_TNUMBER);  // get samplerate
			lua_pushboolean(L, static_cast<int>(audio->load(data, static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)), static_cast<int>(lua_tonumber(L, 5)), static_cast<int>(lua_tonumber(L, 6)))));
			return 1;
		}
		if(lua_type(L, 2) == LUA_TSTRING)
	    {
		    lua_pushboolean(L, audio->load(lua_tostring(L, 2)));
		    return 1;
		}
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
void Audio::play()
{
	//#ifdef DOKUN_OPENAL
        alSourcePlay (static_cast<ALuint>(source));
	//#endif

	#ifdef DOKUN_PORTAUDIO
	    Pa_StartStream (stream);
	#endif

    playing = true;
	paused  = false;
	stopped = false;
}
//////////
int Audio::play(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->play();
	}
	return 0;
}
/////////////
void Audio::pause()
{
	//#ifdef DOKUN_OPENAL
        alSourcePause (static_cast<ALuint>(source));
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif

    playing = false;
	paused  = true;
	stopped = false;
}
//////////
int Audio::pause(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->pause();
	}
	return 0;
}
/////////////
void Audio::stop()
{
	//#ifdef DOKUN_OPENAL
        alSourceStop(static_cast<ALuint>(source));
	//#endif

	#ifdef DOKUN_PORTAUDIO
	    Pa_StopStream(stream);
	#endif

    playing = false;
	paused  = false;
	stopped = true;
}
//////////
int Audio::stop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->stop();
	}
	return 0;
}
void Audio::rewind()
{
	//#ifdef DOKUN_OPENAL
	    alSourceRewind(static_cast<ALuint>(source));
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif
}
//////////
int Audio::rewind(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->rewind();
	}
	return 0;
}
/////////////
/////////////
// setter
void Audio::set_volume(double volume)
{

	//#ifdef DOKUN_OPENAL
	    alSourcef(static_cast<ALuint>(source), AL_GAIN, volume * 0.1);
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif

	this->volume = volume;
}
//////////
int Audio::set_volume(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->set_volume(static_cast<int>(lua_tonumber(L, 2)));
	}
	return 0;
}
/////////////
void Audio::set_default_volume(double volume)
{
	Audio::default_volume = volume;
}
/////////////
void Audio::set_master_volume(double volume)
{
	for(int i = 0; i < Factory::get_audio_factory()->get_size(); i++)
	{
		Audio * audio = static_cast<Audio *>(Factory::get_audio_factory()->get_object(i));
		audio->set_volume(static_cast<int>(volume) % 100); // set volume of all current audio
	}
	set_default_volume(static_cast<int>(volume) % 100); // set volume for all newly created audio
}
//////////
int Audio::set_master_volume(lua_State *L)
{
	luaL_checktype(L, -1, LUA_TNUMBER);
	Audio::set_master_volume(static_cast<int>(lua_tonumber(L, -1)));
	return 0;
}
/////////////
void Audio::set_loop(bool loop)
{
	//#ifdef DOKUN_OPENAL
	    alSourcei(static_cast<ALuint>(source), AL_LOOPING, static_cast<ALint>(loop));
	//#endif

    #ifdef DOKUN_PORTAUDIO
	#endif

	(this)->looped = loop;
}
//////////
int Audio::set_loop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->set_loop((lua_toboolean(L, 2) != 0));
	}
	return 0;
}
/////////////
void Audio::set_pitch(double pitch)
{
	//#ifdef DOKUN_OPENAL
	    alSourcef(static_cast<ALuint>(source), AL_PITCH, static_cast<float>(pitch));
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif

	(this)->pitch = pitch;
}
//////////
int Audio::set_pitch(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->set_pitch(static_cast<double>(lua_tonumber(L, 2)));
	}
	return 0;
}
/////////////
void Audio::set_position(double x, double y)
{
	position.x = x;
	position.y = y;
	//#ifdef DOKUN_OPENAL
	    alSource3f(source, AL_POSITION, static_cast<ALfloat>(x), static_cast<ALfloat>(y), 0.0f);
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif
}
/////////////
void Audio::set_position(double x, double y, double z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	//#ifdef DOKUN_OPENAL
	    alSource3f(source, AL_POSITION, static_cast<ALfloat>(x), static_cast<ALfloat>(y), static_cast<ALfloat>(z));
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif
}
/////////////
void Audio::set_position(const Vector2& p)
{
	set_position(p.x, p.y);
}
/////////////
void Audio::set_position(const Vector3& p)
{
	set_position(p.x, p.y, p.z);
}
//////////
int Audio::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_optnumber(L, 4, 0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		audio->set_position(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	}
	return 0;
}
/////////////
void Audio::set_listener(double x, double y)
{
	//#ifdef DOKUN_OPENAL
	    alListener3f(AL_POSITION, static_cast<float>(x), static_cast<float>(y), 0.0f);
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif
}
/////////////
void Audio::set_listener(double x, double y, double z)
{
	//#ifdef DOKUN_OPENAL
	    alListener3f(AL_POSITION, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif
}
/////////////
void Audio::set_listener(const Vector2& p)
{
	set_listener(p.x, p.y);
}
/////////////
void Audio::set_listener(const Vector3& p)
{
	set_listener(p.x, p.y, p.z);
}
/////////////
void Audio::set_device(ALCdevice*device)
{
	device = device;
}
void Audio::set_context(ALCcontext*context)
{
	context = context;
}
/////////////
/////////////
// getter
double Audio::get_volume()
{
	return volume;
}
//////////
int Audio::get_volume(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_volume());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Audio::get_default_volume()
{
	return Audio::default_volume;
}
//////////
int Audio::get_default_volume(lua_State *L)
{
	lua_pushnumber(L, Audio::get_default_volume());
	return 1;
}
/////////////
double Audio::get_pitch()
{
	return pitch;
}
//////////
int Audio::get_pitch(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_pitch());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Audio::get_duration()
{
	/*
    int size_bytes, channels, bits, length;

    alGetBufferi(buffer, AL_SIZE,     static_cast<ALint*> (&size_bytes));
    alGetBufferi(buffer, AL_CHANNELS,   static_cast<ALint*> (&channels));
    alGetBufferi(buffer, AL_BITS,           static_cast<ALint*> (&bits));

    float length_samples = size_bytes * 8 / (channels * bits);


   alGetBufferi(buffer, AL_FREQUENCY, static_cast<ALint*> (& length));

   double duration = static_cast<double>(length_samples) / static_cast<double>(length);
*/
	return duration;
}
//////////
int Audio::get_duration(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_duration());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector3 Audio::get_position()
{
	return position;
}
//////////
int Audio::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_position().x);
		lua_pushnumber(L, audio->get_position().y);
		lua_pushnumber(L, audio->get_position().z);
		return 3;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector3 Audio::get_listener()
{
	return listener;
}
/////////////
void * Audio::get_data()
{
	return &data[0];
}
//////////
int Audio::get_data(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio**>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, audio->get_data());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
std::string Audio::get_status()
{
	if(is_playing()) return "playing";
	if(is_paused()) return "paused";
	if(is_stopped()) return "stopped";
	return "not playing";
}
//////////
int Audio::get_status(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio**>(lua_touserdata(L, -1));
		lua_pushstring(L, audio->get_status().c_str());
		return 1;
	}
    lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Audio::get_source()
{
	return source;
}
/////////////
unsigned int Audio::get_buffer()
{
	return buffer;
}
/////////////
//#ifdef DOKUN_OPENAL
	ALCdevice *
//#else
	//void *
//#endif
Audio::get_device()
{
	return device;
}
/////////////
//#ifdef DOKUN_OPENAL
	ALCcontext *
//#else
	//void *
//#endif
Audio::get_context()
{
	return context;
}
/////////////
/////////////
// boolean
bool Audio::is_playing()
{
	//#ifdef DOKUN_OPENAL
        ALenum playing;
        alGetSourcei(source, AL_SOURCE_STATE, &playing);
	    (this)->playing = (playing == AL_PLAYING);
	//#endif

	#ifdef DOKUN_PORTAUDIO
	    return (Pa_IsStreamActive (stream) == 1); // if stream is playing or recording
	#endif

	return (this)->playing;
}
//////////
int Audio::is_playing(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_playing());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
bool Audio::is_paused()
{
	//#ifdef DOKUN_OPENAL
        ALenum paused;
        alGetSourcei(source, AL_SOURCE_STATE, &paused);
	    (this)->paused = (paused == AL_PAUSED);
	//#endif

	#ifdef DOKUN_PORTAUDIO
	#endif

	return (this)->paused;
}
//////////
int Audio::is_paused(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_paused());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
bool Audio::is_stopped()
{
	//#ifdef DOKUN_OPENAL
        ALenum stopped;
        alGetSourcei(source, AL_SOURCE_STATE, &stopped);
	    (this)->stopped = (stopped == AL_STOPPED);
	//#endif

	#ifdef DOKUN_PORTAUDIO
	    return (Pa_IsStreamStopped (stream) == 1);
	#endif

	return (this)->stopped;
}
//////////
int Audio::is_stopped(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_stopped());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
bool Audio::is_looped()
{
	return looped;
}
//////////
int Audio::is_looped(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audio * audio = *static_cast<Audio **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_looped());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
/*
bool Music::is_music(Audio * audio)
{
	return ((dokun::is_nullptr(audio) == 0) && (dokun::instanceof<Music>(audio) != 0));
}*/
/////////////
/////////////
// format
bool Audio::is_raw(const std::string& file_name)
{
	return true;
}
/////////////
bool Audio::is_pcm(const std::string& file_name)
{
	return is_raw(file_name);
}
/////////////
bool Audio::is_ogg(const std::string& file_name)
{
    OggVorbis_File vf;
	std::FILE * f;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&f, file_name.c_str(), "rb");
    #else
	    f = fopen(file_name.c_str(), "rb");
	#endif
	if(f == nullptr)
	{
		return false;
	}
	if(ov_open_callbacks(f, &vf, nullptr, 0, OV_CALLBACKS_NOCLOSE) != 0)
	{
        return false;
    }
	ov_clear(&vf);
    fclose(f);
    return true;
}
/////////////
bool Audio::is_wav(const std::string& file_name)
{
    /*
    FILE * fp = fopen(file_name.c_str(),"rb");
	if(fp == nullptr)
	{
		return false;
	}
    unsigned char id[4];
    unsigned long size;
    fread(id, sizeof(unsigned char), 4, fp);
    if (strcmp((const char *)id,"RIFF")) // no RIFF
    {
        return false;
	}
    fread(&size,sizeof(unsigned long),1,fp);
    fread(id,sizeof(unsigned char),4,fp);
    if (strcmp(reinterpret_cast<const char *>(id),"WAVE")) // no WAVE
    {
	    return false;
	}
    return true;
    */
}
/////////////
/////////////
// loader
bool Audio::load_ogg(const std::string& file_name)
{
    data.clear();//delete any previous data // newly added on 07-14-2019

    char pcmout[4096]; // buffer and buffer size (4096)
    OggVorbis_File vf;
    int eof = false;
    int current_section = 0;
	std::FILE * f;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&f, file_name.c_str(), "rb");
    #else
	    f = fopen(file_name.c_str(), "rb");
	#endif
	if(f == nullptr)
	{
		return false;
	}
	if(ov_open_callbacks(f, &vf, nullptr, 0, OV_CALLBACKS_NOCLOSE) != 0)
	{
        return false;
    }

	vorbis_info * vi = ov_info(&vf,-1);
	sample_rate = vi->rate;
	channels = vi->channels;
	bits_per_sample = vi->bitrate_nominal;
    duration = ov_time_total(&vf, -1); // in seconds I think

    while(!eof)
	{
        long ret = ov_read(&vf, pcmout, sizeof(pcmout), 0, 2, 1, &current_section);
		data.insert(data.end(), pcmout, pcmout + ret);
        if (ret == 0)
		{
            eof = 1;
        }
		else if (ret < 0)
		{
			if(ret == OV_EBADLINK)
			{
                std::cerr << "dokun: Corrupt bitstream section!" << std::endl;
                return false;
            }
	        // some other error in the stream.  Not a problem.
        }
		else
		{
            fwrite(pcmout, 1, ret, f);
        }
    }
	//#ifdef DOKUN_OPENAL
	    ALuint format;
	    (vi->channels == 1) ? format = AL_FORMAT_MONO16 : format = AL_FORMAT_STEREO16;

	    alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	    alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), vi->rate);
	    alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
    //#endif

	#ifdef DOKUN_PORTAUDIO
	#endif

	ov_clear(&vf);
    fclose(f);
    return(true);
}
/////////////
bool Audio::load_vorbis(const std::string& file_name)
{
	return load_ogg(file_name);
}
/////////////
bool Audio::load_wav(const std::string& file_name)
{
	/*
	Audio::wav_to_ogg(file_name.c_str(), file_name.substr(0, file_name.find(".")) + "_dokun.ogg"); // export wav to ogg
	int result = load_ogg(file_name.substr(0, file_name.find(".")) + "_dokun.ogg"); // load ogg
	std::remove(std::string(file_name.substr(0, file_name.find(".")) + "_dokun.ogg").c_str()); // delete export ogg wav
	return result;
	*/
	/*
	FILE * fp = fopen(file_name.c_str(),"rb");
	if(fp == NULL)
	{
		return false;
	}

    unsigned char id[4]; //four bytes to hold 'RIFF'
    long unsigned int size; //32 bit value to hold file size
    short format_tag, channels, block_align, bits_per_sample; //our 16 values
    long unsigned int format_length, sample_rate, avg_bytes_sec, data_size, i; //our 32 bit values
	ALuint format = AL_FORMAT_STEREO8;
	unsigned char * sound_buffer;

    fread(id, sizeof(unsigned char), 4, fp); //read in first four bytes
    if (!strcmp((const char *)id,"RIFF"))
    { //we had 'RIFF' let's continue
        fread(&size,sizeof(long unsigned int),1,fp);
        //read in 32bit size value
        fread(id,sizeof(unsigned char),4,fp);
        //read in 4 byte string now
        if (!strcmp(reinterpret_cast<const char *>(id),"WAVE"))
        { //this is probably a wave file since it contained "WAVE"
            fread(id,sizeof(unsigned char),4,fp); //read in 4 bytes "fmt ";
	        fread(&format_length, sizeof(long unsigned int),1,fp);
            fread(&format_tag, sizeof(short), 1, fp); //check mmreg.h (i think?) for other                                              // possible format tags like ADPCM
            fread(&channels, sizeof(short),1,fp); //1 mono, 2 stereo
            fread(&sample_rate, sizeof(long unsigned int), 1, fp); //like 44100, 22050, etc...
            fread(&avg_bytes_sec, sizeof(long unsigned int), 1, fp); //probably won't need this
            fread(&block_align, sizeof(short), 1, fp); //probably won't need this
            fread(&bits_per_sample, sizeof(short), 1, fp); //8 bit or 16 bit file?
            fread(id, sizeof(unsigned char), 4, fp); //read in 'data'
            fread(&data_size, sizeof(long unsigned int), 1, fp); //how many bytes of sound data we have
            sound_buffer = (unsigned char *) malloc (sizeof(unsigned char) * data_size); //set aside sound buffer space
            fread(sound_buffer, sizeof(unsigned char), data_size, fp); //read in our whole sound data chunk
        }
    }
	if(format_tag != 1) // data not stored in PCM (compressed file)
	{
		return false;
	}

    if(bits_per_sample == 8) // 8-bit
	{
	    (channels == 1) ? format = AL_FORMAT_MONO8 : format = AL_FORMAT_STEREO8;
	}
	if(bits_per_sample == 16) // 16-bit
	{
		(channels == 1) ? format = AL_FORMAT_MONO16 : format = AL_FORMAT_STEREO16;
	}
    #ifdef DOKUN_DEBUG
	std::cout << "format tag      = " << format_tag << "\n";
	std::cout << "bits_per_sample = " << bits_per_sample << "\n";
	std::cout << "samplerate      = " << sample_rate << "\n";
	std::cout << "channels        = " << channels<< "\n";
    if(format == AL_FORMAT_MONO8)
	    std::cout << "format          = " << "Mono 8-bit" << std::endl;
    else if(format == AL_FORMAT_STEREO8)
	    std::cout << "format          = " << "Stereo 8-bit" << std::endl;
    else if(format == AL_FORMAT_MONO16)
	    std::cout << "format          = " << "Mono 16-bit" << std::endl;
    else if(format == AL_FORMAT_STEREO16)
		std::cout << "format          = " << "Stereo 16-bit" << std::endl;
    #endif

	data.insert(data.end(), sound_buffer, sound_buffer + data_size);
	//#ifdef DOKUN_OPENAL
	// pop oldest queue buffer
	    alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	// pass data to OpenAL
	    alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), sample_rate);
	// requeue buffer
	    alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
    //#endif

	#ifdef DOKUN_PORTAUDIO
	#endif

	fclose(fp);*/
	return true;
}
/////////////
bool Audio::load_raw(const std::string& file_name)
{
	return true;
}
/////////////
bool Audio::load_pcm(const std::string& file_name)
{
	return load_raw(file_name);
}
/////////////
/////////////
// exporter
void Audio::wav_to_ogg(const std::string& in_file, const std::string& out_file)
{
   signed char readbuffer[1024 * 4 + 44]; /* out of the data segment, not the stack */

  ogg_stream_state os; /* take physical pages, weld into a logical
                          stream of packets */
  ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
  ogg_packet       op; /* one raw packet of data for decode */

  vorbis_info      vi; /* struct that stores all the static vorbis bitstream
                          settings */
  vorbis_comment   vc; /* struct that stores all the user comments */

  vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
  vorbis_block     vb; /* local working space for packet->PCM decode */

  int eos = 0,ret;
  int i, founddata;

  /* we cheat on the WAV header; we just bypass 44 bytes (simplest WAV
     header is 44 bytes) and assume that the data is 44.1khz, stereo, 16 bit
     little endian pcm samples. This is just an example, after all. */

	FILE * fp = fopen(in_file.c_str(), "rb"); // wave (input)
	if(fp == NULL)
	{
		//return false;
	}
	FILE * fout = fopen(out_file.c_str(), "wb"); // vorbis (output)
	if(fout == NULL)
	{
		//return false;
	}

  /* we cheat on the WAV header; we just bypass the header and never
     verify that it matches 16bit/stereo/44.1kHz.  This is just an
     example, after all. */

  readbuffer[0] = '\0';
  for (i = 0, founddata = 0; i < 30 && ! feof(fp) && ! ferror(fp); i++)
  {
    fread(readbuffer,1,2,fp);

    if ( ! strncmp((char*)readbuffer, "da", 2) ){
      founddata = 1;
      fread(readbuffer,1,6,fp);
      break;
    }
  }

  /********** Encode setup ************/

  vorbis_info_init(&vi);

  /* choose an encoding mode.  A few possibilities commented out, one
     actually used: */

  /*********************************************************************
   Encoding using a VBR quality mode.  The usable range is -.1
   (lowest quality, smallest file) to 1. (highest quality, largest file).
   Example quality mode .4: 44kHz stereo coupled, roughly 128kbps VBR

   ret = vorbis_encode_init_vbr(&vi,2,44100,.4);

   ---------------------------------------------------------------------

   Encoding using an average bitrate mode (ABR).
   example: 44kHz stereo coupled, average 128kbps VBR

   ret = vorbis_encode_init(&vi,2,44100,-1,128000,-1);

   ---------------------------------------------------------------------

   Encode using a quality mode, but select that quality mode by asking for
   an approximate bitrate.  This is not ABR, it is true VBR, but selected
   using the bitrate interface, and then turning bitrate management off:

   ret = ( vorbis_encode_setup_managed(&vi,2,44100,-1,128000,-1) ||
           vorbis_encode_ctl(&vi,OV_ECTL_RATEMANAGE2_SET,NULL) ||
           vorbis_encode_setup_init(&vi));

   *********************************************************************/

  ret = vorbis_encode_init_vbr(&vi, 2, 44100, 0.1);

  /* do not continue if setup failed; this can happen if we ask for a
     mode that libVorbis does not support (eg, too low a bitrate, etc,
     will return 'OV_EIMPL') */

  if(ret) exit(1);

  /* add a comment */
  vorbis_comment_init(&vc);
  vorbis_comment_add_tag(&vc,"ENCODER","encoder_example.c");

  /* set up the analysis state and auxiliary encoding storage */
  vorbis_analysis_init(&vd,&vi);
  vorbis_block_init(&vd,&vb);

  /* set up our packet->stream encoder */
  /* pick a random serial number; that way we can more likely build
     chained streams just by concatenation */
  srand(time(NULL));
  ogg_stream_init(&os,rand());

  /* Vorbis streams begin with three headers; the initial header (with
     most of the codec setup parameters) which is mandated by the Ogg
     bitstream spec.  The second header holds any comment fields.  The
     third header holds the bitstream codebook.  We merely need to
     make the headers, then pass them to libvorbis one at a time;
     libvorbis handles the additional Ogg bitstream constraints */

  {
    ogg_packet header;
    ogg_packet header_comm;
    ogg_packet header_code;

    vorbis_analysis_headerout(&vd,&vc,&header,&header_comm,&header_code);
    ogg_stream_packetin(&os,&header); /* automatically placed in its own
                                         page */
    ogg_stream_packetin(&os,&header_comm);
    ogg_stream_packetin(&os,&header_code);

    /* This ensures the actual
     * audio data will start on a new page, as per spec
     */
    while(!eos){
      int result=ogg_stream_flush(&os,&og);
      if(result==0)break;
      fwrite(og.header,1,og.header_len,fout);
      fwrite(og.body,1,og.body_len,fout);
    }

  }

  while(!eos){
    long i;
    long bytes=fread(readbuffer,1,1024*4,fp); /* stereo hardwired here */
data.insert(data.end(), readbuffer, readbuffer + ret);
    if(bytes==0){
      /* end of file.  this can be done implicitly in the mainline,
         but it's easier to see here in non-clever fashion.
         Tell the library we're at end of stream so that it can handle
         the last frame and mark end of stream in the output properly */
      vorbis_analysis_wrote(&vd,0);

    }else{
      /* data to encode */

      /* expose the buffer to submit data */
      float **buffer=vorbis_analysis_buffer(&vd,1024);

      /* uninterleave samples */
      for(i=0;i<bytes/4;i++){
        buffer[0][i]=((readbuffer[i*4+1]<<8)|
                      (0x00ff&(int)readbuffer[i*4]))/32768.f;
        buffer[1][i]=((readbuffer[i*4+3]<<8)|
                      (0x00ff&(int)readbuffer[i*4+2]))/32768.f;
      }

      /* tell the library how much we actually submitted */
      vorbis_analysis_wrote(&vd,i);
    }

    /* vorbis does some data preanalysis, then divvies up blocks for
       more involved (potentially parallel) processing.  Get a single
       block for encoding now */
    while(vorbis_analysis_blockout(&vd,&vb)==1){

      /* analysis, assume we want to use bitrate management */
      vorbis_analysis(&vb,NULL);
      vorbis_bitrate_addblock(&vb);

      while(vorbis_bitrate_flushpacket(&vd,&op)){

        /* weld the packet into the bitstream */
        ogg_stream_packetin(&os,&op);

        /* write out pages (if any) */
        while(!eos){
          int result=ogg_stream_pageout(&os,&og);
          if(result==0)break;
          fwrite(og.header,1,og.header_len,fout);
          fwrite(og.body,1,og.body_len,fout);

          /* this could be set above, but for illustrative purposes, I do
             it here (to show that vorbis does know where the stream ends) */

          if(ogg_page_eos(&og))eos=1;
        }
      }
    }
  }
  /* clean up and exit.  vorbis_info_clear() must be called last */
 fclose(fp);
 fclose(fout);


  ogg_stream_clear(&os);
  vorbis_block_clear(&vb);
  vorbis_dsp_clear(&vd);
  vorbis_comment_clear(&vc);
  vorbis_info_clear(&vi);
}
/////////////
void Audio::wave_to_vorbis(const std::string& in_file, const std::string& out_file)
{
	wav_to_ogg(in_file, out_file);
}
/////////////
bool Audio::load_flac(const std::string& file_name)
{
	//FLAC__StreamDecoder * decoder = 0;
	FILE * file = fopen(file_name.c_str(), "rb");
	if(!file) {
		return false;
	}
	/*
	if((decoder = FLAC__stream_decoder_new()) == NULL) {
		std::cerr << "dokun: failed to allocate decoder" << std::endl;
		fclose(file);
		return false;
	}*/

	//FLAC__stream_decoder_delete(decoder);
	fclose(file);
	return true;
}
/////////////
