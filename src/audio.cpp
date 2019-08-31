#include "../include/audio.h"
/////////////
#ifdef DOKUN_OPENAL
ALCdevice  * Audio::device  (nullptr);
ALCcontext * Audio::context (nullptr);
#endif
/////////////
double Audio::master_volume (100);
/////////////
/////////////
bool Audio::open()
{
#ifdef DOKUN_OPENAL
	device = alcOpenDevice(NULL);
	if (!device) {
		return false;
	}
    context = alcCreateContext(device, NULL);
	if(!context) {
		return false;
	}
	alcMakeContextCurrent(context);
    int openal_major, openal_minor;
    alcGetIntegerv(device, ALC_MAJOR_VERSION, sizeof(openal_major), &openal_major);
    alcGetIntegerv(device, ALC_MINOR_VERSION, sizeof(openal_minor), &openal_minor);
    Logger::push(String("Running OpenAL version ") + String(openal_major).str() + "." + String(openal_minor).str());//+ ".x");//std::cout << "OpenAL version is: " << openal_major << "." << openal_minor << std::endl;	
	return true;
#endif
    return false;	
}
void Audio::close()
{
#ifdef DOKUN_OPENAL
    alcMakeContextCurrent (nullptr);
    alcDestroyContext     (context) ;
    alcCloseDevice        (device ) ;
#endif    
}
/////////////
#ifdef DOKUN_OPENAL
void Audio::set_device(ALCdevice * device)
{
	device = device;
}
#endif
/////////////
#ifdef DOKUN_OPENAL
void Audio::set_context(ALCcontext * context)
{
	context = context;
}
#endif
/////////////
void Audio::set_master_volume(double master_volume)
{
    Audio::master_volume = master_volume;
}
/////////////
int Audio::set_master_volume(lua_State *L)
{
    Audio::set_master_volume(lua_tonumber(L, -1));
    return 0;
}
/////////////
#ifdef DOKUN_OPENAL
ALCdevice *
Audio::get_device()
{
	return device;
}
#endif
/////////////
#ifdef DOKUN_OPENAL
ALCcontext *
Audio::get_context()
{
	return context;
}
#endif
/////////////
double Audio::get_master_volume()
{
    return master_volume;
}
/////////////
int Audio::get_master_volume(lua_State *L)
{
    lua_pushnumber(L, Audio::get_master_volume());
    return 1;
}
/////////////
/////////////
/////////////
/////////////
Audible::Audible() : volume(100), pitch(1.0), duration(0),
    playing(false), paused(false), stopped(false), looped(false),
	bits_per_sample(0), sample_rate(0), channels(0),
	title(""), artist(""), genre(""), album(""), track_number(0), date(""), cover_art(nullptr)
{
	// generate
#ifdef DOKUN_OPENAL
	alGenBuffers(1, static_cast<ALuint*>(&buffer));
	alGenSources(1, static_cast<ALuint*>(&source));
#endif
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
Audible::Audible(const std::string& file_name) : playing(false), paused(false), stopped(false), looped(false), bits_per_sample(0), sample_rate(0), channels(0),
	title(""), artist(""), genre(""), album(""), track_number(0), date(""), cover_art(nullptr)
{
	// generate
#ifdef DOKUN_OPENAL
	alGenBuffers(1, static_cast<ALuint*>(&buffer));
	alGenSources(1, static_cast<ALuint*>(&source));
#endif
	// load
	if(!load(file_name))
    {
        Logger("Could not load file " + file_name);
    }
	// set
	set_volume(Audio::get_master_volume());
	set_pitch(1.0);
	set_position(0, 0, 0);
	set_listener(0, 0, 0);
	// store
	Factory::get_audio_factory()->store(this);
}
/////////////
Audible::Audible(const std::string& file_name, double volume, bool loop) : playing(false), paused(false), stopped(false), bits_per_sample(0), sample_rate(0), channels(0),
	title(""), artist(""), genre(""), album(""), track_number(0), date(""), cover_art(nullptr)
{
	// generate
#ifdef DOKUN_OPENAL
	alGenBuffers(1, static_cast<ALuint*>(&buffer));
	alGenSources(1, static_cast<ALuint*>(&source));
#endif
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
Audible::Audible(void * data, size_t size, int format, int sample_rate) : playing(false), paused(false), stopped(false), looped(false), bits_per_sample(0), sample_rate(0), channels(0),
	title(""), artist(""), genre(""), album(""), track_number(0), date(""), cover_art(nullptr)
{
	// generate
#ifdef DOKUN_OPENAL
	alGenBuffers(1, static_cast<ALuint*>(&buffer));
	alGenSources(1, static_cast<ALuint*>(&source));
#endif
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
Audible::Audible(void * data, size_t size, int bit_rate, int channels, int sample_rate) : playing(false), paused(false), stopped(false), looped(false), bits_per_sample(0), sample_rate(0), channels(0),
	title(""), artist(""), genre(""), album(""), track_number(0), date(""), cover_art(nullptr)
{
	// generate
#ifdef DOKUN_OPENAL
	alGenBuffers(1, static_cast<ALuint*>(&buffer));
	alGenSources(1, static_cast<ALuint*>(&source));
#endif
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
Audible::~Audible()
{
	if (is_playing())
		stop();
#ifdef DOKUN_OPENAL
    alDeleteSources(1, static_cast<ALuint*>(&source));
    alDeleteBuffers(1, static_cast<ALuint*>(&buffer));
#endif

#ifdef DOKUN_PORTAUDIO
	Pa_CloseStream (stream); // free up resources
#endif
	Factory::get_audio_factory()->release(this);
}
/////////////
int Audible::default_volume (100);
/////////////
/////////////
    // normal
bool Audible::load(const std::string& file_name)
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
	if(extension == "flac")
	{
	    return load_flac(file_name);    
	}
	return load_ogg(file_name); // vorbis
}
/////////////
bool Audible::load(void * data, size_t size, int format, int sample_rate)
{
	if(data == nullptr) return false;
#ifdef DOKUN_OPENAL
	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(static_cast<ALuint>(buffer), format, data, static_cast<ALsizei>(size), sample_rate);
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
#endif
	return true;
}
/////////////
bool Audible::load(void * data, size_t size, int bit_rate, int channels, int sample_rate)
{
	unsigned int/*ALuint*/ format;
    if(data == nullptr)
	{
		return false;
	}
    if(bit_rate == 8) // 8-bit
	{
    #ifdef DOKUN_OPENAL
	    (channels == 1) ? format = AL_FORMAT_MONO8 : format = AL_FORMAT_STEREO8;
	#endif    
	}
	if(bit_rate == 16) // 16-bit
	{
	#ifdef DOKUN_OPENAL
		(channels == 1) ? format = AL_FORMAT_MONO16 : format = AL_FORMAT_STEREO16;
	#endif	
	}
#ifdef DOKUN_OPENAL
	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(static_cast<ALuint>(buffer), format, data, static_cast<ALsizei>(size), sample_rate);
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
#endif
	return true;
}
/////////////
int Audible::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // string or lightuserdata
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
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
void Audible::play()
{
#ifdef DOKUN_OPENAL
    alSourcePlay (static_cast<ALuint>(source));
#endif
#ifdef DOKUN_PORTAUDIO
	Pa_StartStream (stream);
#endif
    playing = true;
	paused  = false;
	stopped = false;
}
//////////
int Audible::play(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->play();
	}
	return 0;
}
/////////////
void Audible::pause()
{
#ifdef DOKUN_OPENAL
    alSourcePause (static_cast<ALuint>(source));
#endif
#ifdef DOKUN_PORTAUDIO
#endif
    playing = false;
	paused  = true;
	stopped = false;
}
//////////
int Audible::pause(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->pause();
	}
	return 0;
}
/////////////
void Audible::stop()
{
#ifdef DOKUN_OPENAL
    alSourceStop(static_cast<ALuint>(source));
#endif
#ifdef DOKUN_PORTAUDIO
	Pa_StopStream(stream);
#endif
    playing = false;
	paused  = false;
	stopped = true;
}
//////////
int Audible::stop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->stop();
	}
	return 0;
}
void Audible::rewind()
{
#ifdef DOKUN_OPENAL
	alSourceRewind(static_cast<ALuint>(source));
#endif
#ifdef DOKUN_PORTAUDIO
#endif
}
//////////
int Audible::rewind(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->rewind();
	}
	return 0;
}
/////////////
/////////////
// setter
void Audible::set_volume(double volume)
{

#ifdef DOKUN_OPENAL
	alSourcef(static_cast<ALuint>(source), AL_GAIN, volume * 0.1);
#endif
#ifdef DOKUN_PORTAUDIO
#endif
	this->volume = volume;
}
//////////
int Audible::set_volume(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->set_volume(static_cast<int>(lua_tonumber(L, 2)));
	}
	return 0;
}
/////////////
void Audible::set_default_volume(double volume)
{
	Audio::set_master_volume(volume);
}
/////////////
void Audible::set_master_volume(double volume)
{
	for(int i = 0; i < Factory::get_audio_factory()->get_size(); i++)
	{
		Audible * audio = static_cast<Audible *>(Factory::get_audio_factory()->get_object(i));
		audio->set_volume(static_cast<int>(volume) % 100); // set volume of all current audio
	}
	set_default_volume(static_cast<int>(volume) % 100); // set volume for all newly created audio
}
//////////
int Audible::set_master_volume(lua_State *L)
{
	luaL_checktype(L, -1, LUA_TNUMBER);
	Audible::set_master_volume(static_cast<int>(lua_tonumber(L, -1)));
	return 0;
}
/////////////
void Audible::set_loop(bool loop)
{
#ifdef DOKUN_OPENAL
	alSourcei(static_cast<ALuint>(source), AL_LOOPING, static_cast<ALint>(loop));
#endif
#ifdef DOKUN_PORTAUDIO
#endif
	this->looped = loop;
}
//////////
int Audible::set_loop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->set_loop((lua_toboolean(L, 2) != 0));
	}
	return 0;
}
/////////////
void Audible::set_pitch(double pitch)
{
#ifdef DOKUN_OPENAL
	alSourcef(static_cast<ALuint>(source), AL_PITCH, static_cast<float>(pitch));
#endif
#ifdef DOKUN_PORTAUDIO
#endif
	this->pitch = pitch;
}
//////////
int Audible::set_pitch(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->set_pitch(static_cast<double>(lua_tonumber(L, 2)));
	}
	return 0;
}
/////////////
void Audible::set_position(double x, double y)
{
	position.x = x;
	position.y = y;
#ifdef DOKUN_OPENAL
	alSource3f(source, AL_POSITION, static_cast<ALfloat>(x), static_cast<ALfloat>(y), 0.0f);
#endif
#ifdef DOKUN_PORTAUDIO
#endif
}
/////////////
void Audible::set_position(double x, double y, double z)
{
	position.x = x;
	position.y = y;
	position.z = z;
#ifdef DOKUN_OPENAL
	alSource3f(source, AL_POSITION, static_cast<ALfloat>(x), static_cast<ALfloat>(y), static_cast<ALfloat>(z));
#endif
#ifdef DOKUN_PORTAUDIO
#endif
}
/////////////
void Audible::set_position(const Vector2& p)
{
	set_position(p.x, p.y);
}
/////////////
void Audible::set_position(const Vector3& p)
{
	set_position(p.x, p.y, p.z);
}
//////////
int Audible::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_optnumber(L, 4, 0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		audio->set_position(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	}
	return 0;
}
/////////////
void Audible::set_listener(double x, double y)
{
#ifdef DOKUN_OPENAL
	alListener3f(AL_POSITION, static_cast<float>(x), static_cast<float>(y), 0.0f);
#endif
#ifdef DOKUN_PORTAUDIO
#endif
}
/////////////
void Audible::set_listener(double x, double y, double z)
{
#ifdef DOKUN_OPENAL
	alListener3f(AL_POSITION, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
#endif
#ifdef DOKUN_PORTAUDIO
#endif
}
/////////////
void Audible::set_listener(const Vector2& p)
{
	set_listener(p.x, p.y);
}
/////////////
void Audible::set_listener(const Vector3& p)
{
	set_listener(p.x, p.y, p.z);
}
/////////////
/////////////
// getter
double Audible::get_volume()
{
	return volume;
}
//////////
int Audible::get_volume(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_volume());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Audible::get_default_volume()
{
	return Audio::get_master_volume();
}
//////////
int Audible::get_default_volume(lua_State *L)
{
	lua_pushnumber(L, Audible::get_default_volume());
	return 1;
}
/////////////
double Audible::get_pitch()
{
	return pitch;
}
//////////
int Audible::get_pitch(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_pitch());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
double Audible::get_duration() // in seconds
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
// duration = BUFFER_SIZE / (sound.freqency * sound.channels * sound.bitsPerChannel / 8) (eq. 1) // sound frequency (eg. 44100 Hz) // number of channels (eg. Mono = 1, Stereo = 2) //sound bitrate //number of bits per channel (eg. 16 for 2 channel stereo)
	return duration;
}
//////////
int Audible::get_duration(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_duration());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector3 Audible::get_position()
{
	return position;
}
//////////
int Audible::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushnumber(L, audio->get_position().x);
		lua_pushnumber(L, audio->get_position().y);
		lua_pushnumber(L, audio->get_position().z);
		return 3;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector3 Audible::get_listener()
{
	return listener;
}
/////////////
void * Audible::get_data()
{
	return &data[0];
}
//////////
int Audible::get_data(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible**>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, audio->get_data());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
std::string Audible::get_file() const
{
    return file;
}
/////////////
int Audible::get_file(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible**>(lua_touserdata(L, -1));
		lua_pushstring(L, audio->get_file().c_str());
		return 1;
	}
	lua_pushstring(L, ""); // empty string
	return 1;
}
/////////////
std::string Audible::get_status()
{
	if(is_playing()) return "playing";
	if(is_paused ()) return "paused";
	if(is_stopped()) return "stopped";
	return "not playing";
}
//////////
int Audible::get_status(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible**>(lua_touserdata(L, -1));
		lua_pushstring(L, audio->get_status().c_str());
		return 1;
	}
    lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Audible::get_source()
{
	return source;
}
/////////////
unsigned int Audible::get_buffer()
{
	return buffer;
}
/////////////
/////////////
// userdata
/////////////
std::string Audible::get_title () const
{
    return title;
}
int Audible::get_title(lua_State * L)
{
    return 1;
}
/////////////
std::string Audible::get_artist() const
{
    return artist;
}
int Audible::get_artist(lua_State * L)
{
    return 1;
}
/////////////
std::string Audible::get_album () const
{
    return album;
}
int Audible::get_album(lua_State * L)
{
    return 1;
}
/////////////
std::string Audible::get_genre () const
{
    return genre;
}
int Audible::get_genre(lua_State * L)
{
    return 1;
}
/////////////
unsigned int Audible::get_track_number() const
{
    return track_number;
}
int Audible::get_track_number(lua_State * L)
{
    return 1;
}
/////////////
std::string Audible::get_date() const
{
    return date;
}
int Audible::get_date(lua_State * L)
{
    return 1;
}
/////////////
/*unsigned char **/void * Audible::get_cover() const
{
    return cover_art;
}
int Audible::get_cover(lua_State * L)
{
    return 1;
}
/////////////
/////////////
// boolean
bool Audible::is_playing()
{
#ifdef DOKUN_OPENAL
    ALenum playing;
    alGetSourcei(source, AL_SOURCE_STATE, &playing);
	this->playing = (playing == AL_PLAYING);
#endif
#ifdef DOKUN_PORTAUDIO
	return (Pa_IsStreamActive (stream) == 1); // if stream is playing or recording
#endif
	return this->playing;
}
//////////
int Audible::is_playing(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_playing());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
bool Audible::is_paused()
{
#ifdef DOKUN_OPENAL
    ALenum paused;
    alGetSourcei(source, AL_SOURCE_STATE, &paused);
	this->paused = (paused == AL_PAUSED);
#endif
#ifdef DOKUN_PORTAUDIO
#endif
	return this->paused;
}
//////////
int Audible::is_paused(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_paused());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
bool Audible::is_stopped()
{
#ifdef DOKUN_OPENAL
    ALenum stopped;
    alGetSourcei(source, AL_SOURCE_STATE, &stopped);
	this->stopped = (stopped == AL_STOPPED);
#endif
#ifdef DOKUN_PORTAUDIO
	return (Pa_IsStreamStopped (stream) == 1);
#endif
	return this->stopped;
}
//////////
int Audible::is_stopped(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_stopped());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
bool Audible::is_looped()
{
	return looped;
}
//////////
int Audible::is_looped(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audio = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushboolean(L, audio->is_looped());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
/*
bool Music::is_music(Audible * audio)
{
	return ((dokun::is_nullptr(audio) == 0) && (dokun::instanceof<Music>(audio) != 0));
}*/
/////////////
/////////////
// format
bool Audible::is_raw(const std::string& file_name)
{
	return true;
}
/////////////
bool Audible::is_pcm(const std::string& file_name)
{
	return is_raw(file_name);
}
/////////////
bool Audible::is_ogg(const std::string& file_name)
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
bool Audible::is_wav(const std::string& file_name)
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
	}*/
    return true;
}
/////////////
/////////////
// loader
bool Audible::load_ogg(const std::string& file_name)
{
    data.clear();//delete any previous data // newly added on 07-14-2019

    char pcmout[4096]; // buffer and buffer size (4096) // PCM file is a raw file format which OpenAL reads
    OggVorbis_File vf;
    int endian = 0; // 1 for big endian
    int bitstream = 0;
	std::FILE * f;
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	fopen_s(&f, file_name.c_str(), "rb");
#else
	f = fopen(file_name.c_str(), "rb");
#endif
	if(f == nullptr)
	{
	    Logger("Audible::load_ogg : Could not open file: " + file_name);
		return false;
	}
	if(ov_open_callbacks(f, &vf, nullptr, 0, OV_CALLBACKS_NOCLOSE) != 0)
	{
	    Logger("Audible::load_ogg : Input does not appear to be an Ogg bitstream.\n");
        return false;
    }
    file = file_name.substr(file_name.find_last_of("\\/") + 1); // save filename
    char **ptr = ov_comment(&vf,-1)->user_comments;
	vorbis_info * vi = ov_info(&vf,-1); // contains information about ogg vorbis
	// now lets store that information into a stringstream
	std::stringstream stream;
	while(*ptr){
      //fprintf(stderr,"%s\n",*ptr);
      stream << *ptr << "\n";
      ++ptr;
    }
    std::vector<std::string> content = String::split(stream.str(), "\n"); // get content from stringstream 
    for(int i = 0; i < content.size(); i++)
    {
        //if(String::contains(content[i], "ENCODER=")) 
        if(String::contains(content[i], "TITLE=" )) { content[i] = String::swap_first_of(content[i], "TITLE=" , ""); title  = content[i]; }
        if(String::contains(content[i], "ARTIST=")) { content[i] = String::swap_first_of(content[i], "ARTIST=", ""); artist = content[i]; }
        if(String::contains(content[i], "ALBUM=" )) { content[i] = String::swap_first_of(content[i], "ALBUM=" , ""); album  = content[i]; }
        if(String::contains(content[i], "GENRE=" )) { content[i] = String::swap_first_of(content[i], "GENRE=" , ""); genre  = content[i]; }
        if(String::contains(content[i], "DATE="  )) { content[i] = String::swap_first_of(content[i], "DATE="  , ""); date   = content[i]; }
        if(String::contains(content[i], "TRACKNUMBER=")) { content[i] = String::swap_first_of(content[i], "TRACKNUMBER=", ""); track_number = String::extract(content[i]); } 
        if(String::contains(content[i], "METADATA_BLOCK_PICTURE=")) { content[i] = String::swap_first_of(content[i], "METADATA_BLOCK_PICTURE=", ""); cover_art = const_cast<unsigned char *>(reinterpret_cast<const unsigned char*>(content[i].c_str()) );
            //cover_art = const_cast<unsigned char *>(cover);
        } 
        //if(String::contains(content[i], "")) 
    }
    // sensative audio data
	sample_rate = vi->rate;
	channels    = vi->channels;
	bits_per_sample = vi->bitrate_nominal;
    duration = (long)ov_time_total(&vf, -1); // Returns the total time in seconds of the physical bitstream
    std::string vendor = ov_comment(&vf,-1)->vendor; // fprintf(stderr,"Encoded by: %s\n\n",ov_comment(&vf,-1)->vendor);
    
    
    long bytes = -1;
    while(bytes != 0) {
        bytes = ov_read(&vf, pcmout, sizeof(pcmout), endian, 2, 1, &bitstream); // 512, 2304, 4096
        data.insert(data.end(), pcmout, pcmout + bytes);
        if(bytes < 0)
        {
            if(bytes == OV_EBADLINK) {std::cerr << "dokun: Corrupt bitstream section!" << std::endl; return false;}
            std::cout << "unknown error in stream" << std::endl;
            return false;
        }
        if(bytes > 0)
        {
            fwrite(pcmout, 1, bytes, f);
        }
    }
    /*int eof = false;
    while(!eof)
	{
        long bytes = ov_read(&vf, pcmout, sizeof(pcmout), endian, 2, 1, &bitstream);
		data.insert(data.end(), pcmout, pcmout + bytes);
        if (bytes == 0)
		{
            eof = 1;
        }
		else if (bytes < 0)
		{
			if(bytes == OV_EBADLINK)
			{
                std::cerr << "dokun: Corrupt bitstream section!" << std::endl;
                return false;
            }
	        // some other error in the stream.  Not a problem.
        }
		else
		{
            fwrite(pcmout, 1, bytes, f);
        }
    }*/
#ifdef DOKUN_OPENAL
    ALuint format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16; // ogg vorbis is 16-bit ONLY. No 8-bit! unlike flac which can have both 8 and 16 bits.//if(format == AL_FORMAT_MONO16) std::cout << "16-bit Mono\n"; else std::cout << "16-bit Stereo\n";
	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), vi->rate);
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
#endif
#ifdef DOKUN_PORTAUDIO
#endif
	ov_clear(&vf);
    fclose(f);
    return(true);
} // https://xiph.org/vorbis/doc/vorbisfile/example.html
/////////////
bool Audible::load_vorbis(const std::string& file_name)
{
	return load_ogg(file_name);
}
/////////////
/////////////
bool Audible::load_flac(const std::string& file_name)
{
	if(decode_flac(file_name))
	{
	    if(load_wav(file_name.substr(0, file_name.find(".")) + ".wav"))
	    {
	        std::remove(std::string(file_name.substr(0, file_name.find(".")) + ".wav").c_str()); // delete export flac wav
	        return true;
	    }
	}    
	return false;
	////////////////////////////////////////////////////////////////////////////////////////////
	/*FILE * file = fopen(file_name.c_str(), "rb");
	if(!file) { return false; }
    
	FLAC__StreamDecoder * decoder = 0;
	if((decoder = FLAC__stream_decoder_new()) == nullptr) {
		std::cerr << "dokun: FLAC__stream_decoder_new failed to allocate decoder" << std::endl;
		fclose(file);
		return false;
	}
    
    (void)FLAC__stream_decoder_set_md5_checking(decoder, true);
    
    FLAC__StreamDecoderInitStatus init_status;
    FLAC__bool ok = true; 
    std::cout << "stored contents of : " << file_name << " to " << "Audible::data" << std::endl;
    init_status = FLAC__stream_decoder_init_FILE(decoder, file, static_cast<FLAC__StreamDecoderWriteCallback>(this->write_callback), static_cast<FLAC__StreamDecoderMetadataCallback>(this->metadata_callback), static_cast<FLAC__StreamDecoderErrorCallback>(this->error_callback), data);//FLAC__stream_decoder_init_file(decoder, file_name.c_str(), write_callback, metadata_callback, error_callback, (void *)data); // FLAC__stream_decoder_init_FILE(decoder, file, write_callback, nullptr, error_callback, nullptr); // argv[1] should be file_name : https://xiph.org/flac/api/group__flac__stream__decoder.html#ga4021ead5cff29fd589c915756f902f1a //FLAC__stream_decoder_init_stream (decoder, FLAC__StreamDecoderReadCallback read_callback, nullptr, nullptr, nullptr, nullptr, write_callback, metadata_callback, error_callback, nullptr);
    if(init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
		std::cerr << "ERROR: initializing decoder: " << FLAC__StreamDecoderInitStatusString[init_status] << std::endl;
		ok = false;
	}
	if(ok) {
		ok = FLAC__stream_decoder_process_until_end_of_stream(decoder);
		fprintf(stderr, "decoding: %s\n", ok? "succeeded" : "FAILED");
		fprintf(stderr, "   state: %s\n", FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(decoder)]);
	}*/
	//std::cout << "decoder channels: " << channels = FLAC__stream_decoder_get_channels (decoder)    << std::endl;
	//std::cout << "decoder channels: " << FLAC__stream_decoder_get_sample_rate (decoder) << std::endl;
/*#ifdef DOKUN_OPENAL
	ALuint format;
	(vi->channels == 1) ? format = AL_FORMAT_MONO16 : format = AL_FORMAT_STEREO16;

	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), vi->rate);
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
#endif*/
	//FLAC__stream_decoder_delete(decoder);
	//fclose(file);
	//return true;    
}
/////////////
/////////////
//FLAC__StreamDecoderReadStatus Audible::read_callback(FLAC__byte buffer[], unsigned *bytes)
//{return }
/////////////
FLAC__StreamDecoderWriteStatus Audible::write_callback(const FLAC__Frame *frame, const FLAC__int32 * const buffer[])
{
    for(size_t i = 0; i < frame->header.blocksize; i++)
    {
        data[index] = buffer[0][i]; // channel audio on the left
        ++index;
        
        data[index] = buffer[1][i]; // what about the right channel?
    }
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}
/////////////
void Audible::metadata_callback(const FLAC__StreamMetadata *metadata)
{
	if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
		this->total_samples   = metadata->data.stream_info.total_samples;
		this->sample_rate     = metadata->data.stream_info.sample_rate;
		this->channels        = metadata->data.stream_info.channels;
		this->bits_per_sample = metadata->data.stream_info.bits_per_sample;
	}
	// set format depending on bits_per_second and number of channels
	ALuint format;
	switch(this->bits_per_sample) {
	    case 8:
		    if(this->channels > 1) { format = AL_FORMAT_STEREO8;  Logger("8-bits Stereo", "info"); } else { format = AL_FORMAT_MONO8;  Logger("8-bits Mono", "info"); }
		    break;
		case 16:
		    if(this->channels > 1) { format = AL_FORMAT_STEREO16; Logger("16-bits Stereo", "info");} else { format = AL_FORMAT_MONO16; Logger("16-bits Mono", "info");}
		    break;
	}
	// set size and resize data_vector based on number of total_samples, reset index
	size = static_cast<ALuint>( this->sample_rate * this->channels * (this->bits_per_sample / 8) );
	data.resize( total_samples );
	index = 0;
}
/////////////
void Audible::error_callback(FLAC__StreamDecoderErrorStatus status)
{
    fprintf(stderr, "Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
}
/////////////
/////////////
bool Audible::load_wav(const std::string& file_name) // Every time you convert to lossy(ogg, mp3) there is loss of quality
{
	Audible::wav_to_ogg(file_name.c_str(), file_name.substr(0, file_name.find(".")) + "_dokun.ogg"); // export wav to ogg
	int result = load_ogg(file_name.substr(0, file_name.find(".")) + "_dokun.ogg"); // load ogg
	std::remove(std::string(file_name.substr(0, file_name.find(".")) + "_dokun.ogg").c_str()); // delete export ogg wav
	return result;
	/*FILE * fp = fopen(file_name.c_str(),"rb");
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
#ifdef DOKUN_OPENAL
	    alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer)); // pop oldest queue buffer
	    alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), sample_rate); // pass data to OpenAL
	    alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer)); // requeue buffer
#endif

#ifdef DOKUN_PORTAUDIO
#endif
	fclose(fp);*/
	return true;
}
/////////////
bool Audible::load_raw(const std::string& file_name)
{
	return true;
}
/////////////
bool Audible::load_pcm(const std::string& file_name)
{
	return load_raw(file_name);
}
/////////////
/////////////
// exporter
void Audible::wav_to_ogg(const std::string& in_file, const std::string& out_file)
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
void Audible::wave_to_vorbis(const std::string& in_file, const std::string& out_file)
{
	wav_to_ogg(in_file, out_file);
}
/////////////
FLAC__uint64 Audible::total_samples_dec  (0);
unsigned int Audible::bits_per_sample_dec(0); // 8 bit, 16 bit for 2 channel stereo
unsigned int Audible::sample_rate_dec    (0); // same as frequency (eg. 44100, 22050, etc.)
unsigned int Audible::channels_dec       (0); // 1=mono, 2=stereo
unsigned int Audible::format_dec         (0);
/////////////
bool Audible::decode_flac(const std::string& file_name) // converts .flac to .wav
{
	FILE * file = fopen(std::string(file_name.substr(0, file_name.find(".")) + ".wav").c_str(), "wb"); // "wb"  in examples_decode.c // output file - opens file for writing
	if(!file) {
		return false;
	}
	
	FLAC__StreamDecoder * decoder = 0;
	if((decoder = FLAC__stream_decoder_new()) == nullptr) {
		std::cerr << "dokun: FLAC__stream_decoder_new failed to allocate decoder" << std::endl;
		fclose(file);
		return false;
	}
    
    (void)FLAC__stream_decoder_set_md5_checking(decoder, true);
    
    FLAC__StreamDecoderInitStatus init_status;
    FLAC__bool ok = true; 
    std::cout << "converted: " << file_name << " to " << std::string(file_name.substr(0, file_name.find(".")) + ".wav").c_str() << std::endl;
    init_status = FLAC__stream_decoder_init_file(decoder, file_name.c_str(), decoder_write_callback, decoder_metadata_callback, decoder_error_callback, file); // FLAC__stream_decoder_init_FILE(decoder, file, write_callback, nullptr, error_callback, nullptr); // argv[1] should be file_name : https://xiph.org/flac/api/group__flac__stream__decoder.html#ga4021ead5cff29fd589c915756f902f1a
    if(init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
		std::cerr << "ERROR: initializing decoder: " << FLAC__StreamDecoderInitStatusString[init_status] << std::endl;
		ok = false;
	}
	if(ok) {
		ok = FLAC__stream_decoder_process_until_end_of_stream(decoder);
		fprintf(stderr, "decoding: %s\n", ok? "succeeded" : "FAILED");
		fprintf(stderr, "   state: %s\n", FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(decoder)]);
	}

	FLAC__stream_decoder_delete(decoder);
	fclose(file);
	return true;
}
/////////////
unsigned int Audible::total_samples_enc(0);
/////////////
bool Audible::encode_flac(const std::string& file_name) // converts .wav to .flac
{
    FLAC__byte buf[1024/*samples*/ * 2/*bytes_per_sample*/ * 2/*channels*/]; /* we read the WAVE data into here */
    FLAC__int32 pcm[1024/*samples*/ * 2/*channels*/]; // #define READSIZE 1024

	FLAC__bool ok = true;
	FLAC__StreamEncoder *encoder = 0;
	FLAC__StreamEncoderInitStatus init_status;
	FLAC__StreamMetadata *metadata[2];
	FLAC__StreamMetadata_VorbisComment_Entry entry;
	unsigned sample_rate = 0;
	unsigned channels = 0;
	unsigned bits_per_sample = 0;
	
	FILE * file = fopen(file_name.c_str(), "rb");  // input file, open for reading //  file_name.substr(0, file_name.find(".")) + ".wav").c_str()
	if(!file) {
		return false;
	}
	/* read wav header and validate it */
	if(
		fread(buf, 1, 44, file) != 44 ||
		memcmp(buf, "RIFF", 4) ||
		memcmp(buf+8, "WAVEfmt \020\000\000\000\001\000\002\000", 16) ||
		memcmp(buf+32, "\004\000\020\000data", 8)
	) {
		fprintf(stderr, "ERROR: invalid/unsupported WAVE file, only 16bps stereo WAVE in canonical form allowed\n");
		fclose(file);
		return 1;
	}
	sample_rate = ((((((unsigned)buf[27] << 8) | buf[26]) << 8) | buf[25]) << 8) | buf[24];
	channels = 2;
	bits_per_sample = 16;
	total_samples_enc = (((((((unsigned)buf[43] << 8) | buf[42]) << 8) | buf[41]) << 8) | buf[40]) / 4;

	/* allocate the encoder */
	if((encoder = FLAC__stream_encoder_new()) == NULL) {
		fprintf(stderr, "ERROR: allocating encoder\n");
		fclose(file);
		return 1;
	}
	
	ok &= FLAC__stream_encoder_set_verify(encoder, true);
	ok &= FLAC__stream_encoder_set_compression_level(encoder, 5);
	ok &= FLAC__stream_encoder_set_channels(encoder, channels);
	ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, bits_per_sample);
	ok &= FLAC__stream_encoder_set_sample_rate(encoder, sample_rate);
	ok &= FLAC__stream_encoder_set_total_samples_estimate(encoder, total_samples_enc);
	
	/* now add some metadata; we'll add some tags and a padding block */
	if(ok) {
		if(
			(metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT)) == NULL ||
			(metadata[1] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING)) == NULL ||
			/* there are many tag (vorbiscomment) functions but these are convenient for this particular use: */
			!FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "ARTIST", "Some Artist") ||
			!FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false) || /* copy=false: let metadata object take control of entry's allocated string */
			!FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "YEAR", "1984") ||
			!FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false)
		) {
			fprintf(stderr, "ERROR: out of memory or tag error\n");
			ok = false;
		}

		metadata[1]->length = 1234; /* set the padding length */

		ok = FLAC__stream_encoder_set_metadata(encoder, metadata, 2);
	}
	/* initialize encoder */
	if(ok) {
		init_status = FLAC__stream_encoder_init_file(encoder, std::string(file_name.substr(0, file_name.find(".")) + ".flac").c_str(), encoder_progress_callback, /*client_data=*/NULL);
		if(init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
			fprintf(stderr, "ERROR: initializing encoder: %s\n", FLAC__StreamEncoderInitStatusString[init_status]);
			ok = false;
		}
	}		
	/* read blocks of samples from WAVE file and feed to encoder */
	if(ok) {
		size_t left = (size_t)total_samples_enc;
		while(ok && left) {
			size_t need = (left>1024? (size_t)1024 : (size_t)left);
			//data.insert(data.end(), pcm, pcm + need); // get data and pass it to openAL --------------------------------------------------------------------
			if(fread(buf, channels*(bits_per_sample/8), need, file) != need) {
				fprintf(stderr, "ERROR: reading from WAVE file\n");
				ok = false;
			}
			else {
				/* convert the packed little-endian 16-bit PCM samples from WAVE into an interleaved FLAC__int32 buffer for libFLAC */
				size_t i;
				for(i = 0; i < need*channels; i++) {
					/* inefficient but simple and works on big- or little-endian machines */
					pcm[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)buf[2*i+1] << 8) | (FLAC__int16)buf[2*i]);
				}
				/* feed samples to encoder */
				ok = FLAC__stream_encoder_process_interleaved(encoder, pcm, need);
			}
			left -= need;
		}
	}

	ok &= FLAC__stream_encoder_finish(encoder);

	fprintf(stderr, "encoding: %s\n", ok? "succeeded" : "FAILED");
	fprintf(stderr, "   state: %s\n", FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state(encoder)]);
    std::cout << "converted: " << file_name << " to " << std::string(file_name.substr(0, file_name.find(".")) + ".flac").c_str() << std::endl;
    
	/* now that encoding is finished, the metadata can be freed */
	FLAC__metadata_object_delete(metadata[0]);
	FLAC__metadata_object_delete(metadata[1]);
/*
#ifdef DOKUN_OPENAL
	ALuint format;
	(channels == 1) ? format = AL_FORMAT_MONO16 : format = AL_FORMAT_STEREO16;

	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), sample_rate); // get data and pass it to openAL -----------------------------------
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
#endif*/

	FLAC__stream_encoder_delete(encoder);
	fclose(file);
	return 0;	
}
/////////////
/////////////
// callbacks (decoder)
/////////////
FLAC__StreamDecoderWriteStatus Audible::decoder_write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	FILE *f = (FILE*)client_data;
	const FLAC__uint32 total_size = (FLAC__uint32)(total_samples_dec * channels_dec * (bits_per_sample_dec/8));
	size_t i;

	(void)decoder;

	if(total_samples_dec == 0) {
		fprintf(stderr, "ERROR: this example only works for FLAC files that have a total_samples count in STREAMINFO\n");
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
	if(channels_dec != 2 || bits_per_sample_dec != 16) {
		fprintf(stderr, "ERROR: this example only supports 16bit stereo streams\n");
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
	if(frame->header.channels != 2) {
		fprintf(stderr, "ERROR: This frame contains %d channels (should be 2)\n", frame->header.channels);
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
	if(buffer [0] == NULL) {
		fprintf(stderr, "ERROR: buffer [0] is NULL\n");
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
	if(buffer [1] == NULL) {
		fprintf(stderr, "ERROR: buffer [1] is NULL\n");
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}

	/* write WAVE header before we write the first frame */
	if(frame->header.number.sample_number == 0) {
		if(
			fwrite("RIFF", 1, 4, f) < 4 ||
			!write_little_endian_uint32(f, total_size + 36) ||
			fwrite("WAVEfmt ", 1, 8, f) < 8 ||
			!write_little_endian_uint32(f, 16) ||
			!write_little_endian_uint16(f, 1) ||
			!write_little_endian_uint16(f, (FLAC__uint16)channels_dec) ||
			!write_little_endian_uint32(f, sample_rate_dec) ||
			!write_little_endian_uint32(f, sample_rate_dec * channels_dec * (bits_per_sample_dec/8)) ||
			!write_little_endian_uint16(f, (FLAC__uint16)(channels_dec * (bits_per_sample_dec/8))) || /* block align */
			!write_little_endian_uint16(f, (FLAC__uint16)bits_per_sample_dec) ||
			fwrite("data", 1, 4, f) < 4 ||
			!write_little_endian_uint32(f, total_size)
		) {
			fprintf(stderr, "ERROR: write error\n");
			return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
		}
	}

	/* write decoded PCM samples */
	for(i = 0; i < frame->header.blocksize; i++) {
		if(
			!write_little_endian_int16(f, (FLAC__int16)buffer[0][i]) ||  /* left channel */
			!write_little_endian_int16(f, (FLAC__int16)buffer[1][i])     /* right channel */
		) {
			fprintf(stderr, "ERROR: write error\n");
			return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
		}
	}

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}
/////////////
void Audible::decoder_metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	(void)decoder, (void)client_data;

	/* print some stats */
	if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
		/* save for later */
		total_samples_dec = metadata->data.stream_info.total_samples;
		sample_rate_dec = metadata->data.stream_info.sample_rate;
		channels_dec = metadata->data.stream_info.channels;
		bits_per_sample_dec = metadata->data.stream_info.bits_per_sample;

		fprintf(stderr, "sample rate    : %u Hz\n", sample_rate_dec);
		fprintf(stderr, "channels       : %u\n", channels_dec);
		fprintf(stderr, "bits per sample: %u\n", bits_per_sample_dec);
		fprintf(stderr, "total samples  : %" PRIu64 "\n", total_samples_dec);
	}
}
/////////////
void Audible::decoder_error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
	(void)decoder, (void)client_data;

	fprintf(stderr, "Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
}
/////////////
/////////////
// callbacks (encoder)
///////////// 
void Audible::encoder_progress_callback(const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data)
{
	(void)encoder, (void)client_data;

	fprintf(stderr, "wrote %" PRIu64 " bytes, %" PRIu64 "/%u samples, %u/%u frames\n", bytes_written, samples_written, static_cast<unsigned int>(total_samples_enc), frames_written, total_frames_estimate);
}
/////////////
/////////////
// helper
/////////////
FLAC__bool Audible::write_little_endian_uint16(FILE* f, FLAC__uint16 x)
{
	return
		fputc(x, f) != EOF &&
		fputc(x >> 8, f) != EOF
	;
}
/////////////
FLAC__bool Audible::write_little_endian_int16(FILE* f, FLAC__int16 x)
{
    return write_little_endian_uint16(f, static_cast<FLAC__uint16>(x));
}
/////////////
FLAC__bool Audible::write_little_endian_uint32(FILE* f, FLAC__uint32 x)
{
	return
		fputc(x, f) != EOF &&
		fputc(x >> 8, f) != EOF &&
		fputc(x >> 16, f) != EOF &&
		fputc(x >> 24, f) != EOF
	;
}
/////////////
/////////////
