#include "../include/voice.h"

/////////////
Voice::Voice() : recording(false), device(nullptr)
{
	#ifdef DOKUN_OPENAL
	// generate    
		alGenBuffers(1, static_cast<ALuint*>(&buffer));
	    alGenSources(1, static_cast<ALuint*>(&source));	
	// source
	    alSourcef(static_cast<ALuint>(source), AL_GAIN, volume * 0.1);    
	    alSource3f(source, AL_POSITION, static_cast<ALfloat>(0.0), static_cast<ALfloat>(0.0), static_cast<ALfloat>(0.0));	
	// listener
	    alListener3f(AL_POSITION, static_cast<ALfloat>(0.0), static_cast<ALfloat>(0.0), static_cast<ALfloat>(0.0));
	#endif
	Factory::get_voice_factory()->store(this);
}
/////////////
Voice::~Voice()
{
	stop(); // stop recording
	// delete buffers
#ifdef DOKUN_OPENAL
	alDeleteBuffers(1, static_cast<ALuint*>(&buffer));
	alDeleteSources(1, static_cast<ALuint*>(&source));
    // close the device	
	alcCaptureCloseDevice(device);	  
#endif
	Factory::get_voice_factory()->release(this);
}
/////////////
double Voice::volume (100);
/////////////
void Voice::start()
{
	#ifdef DOKUN_OPENAL
	    device = alcCaptureOpenDevice(alcGetString(nullptr, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER), 44100, AL_FORMAT_MONO16, 44100); // sample rate at 44100
        if(device == nullptr)
	    {   
		    Logger("Could not start voice"); return;
	    }
	    if (!is_recording())
	    {
            alcCaptureStart(device);	 // Start the device
            recording = (true);    
	    }
	    while(is_recording())
	    {
		    alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &sample); // deviced audio
            alcCaptureSamples(device, static_cast<ALCvoid *>(&data[0]), static_cast<ALint>(sample)); // grab the sound
	    }	
	// pass data (to openal)
 	    alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer)); 
	    alBufferData(static_cast<ALuint>(buffer), AL_FORMAT_STEREO16, &data[0], static_cast<ALsizei>(data.size()), 44100);
	    alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer)); 
	#endif
}
/////////////
int Voice::start(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		voice->start();
	}	
	return 0;
}
/////////////
void Voice::stop()
{
	if(is_recording())
	{ 
        #ifdef DOKUN_OPENAL
            alcCaptureStop(device);  // Stop the device	
		#endif
		recording = false;
	}
	if(is_playing())
	{
		#ifdef DOKUN_OPENAL
		    alSourceStop(static_cast<ALuint>(source));
		#endif
	}
}
/////////////
int Voice::stop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		voice->stop();
	}	return 0;
}
/////////////
void Voice::play()
{
	#ifdef DOKUN_OPENAL
	    alSourcePlay(static_cast<ALuint>(source));
	#endif
}
/////////////
int Voice::play(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		voice->play();
	}	
	return 0;
}
/////////////
void Voice::pause()
{
	#ifdef DOKUN_OPENAL
	    alSourcePause(static_cast<ALuint>(source));
	#endif
}
/////////////
int Voice::pause(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		voice->pause();
	}	
	return 0;	
}
/////////////
void Voice::save(const std::string& file_name)
{
    // save recording to file_name
	std::ofstream file(file_name.c_str());
	if(!file.is_open()) 
	{
		Logger("Cannot save " + file_name);
		return;
	}
	for(int i = 0; i < data.size(); i++)
	{
		file << data[i];
	}
	file.close();
}
/////////////
int Voice::save(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_optstring(L, 2, "test.ogg");
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TSTRING)
		{
			voice->save(lua_tostring(L, 2));
		}
	}
	return 0;
}
/////////////
void * Voice::get_device()
{
	return device;
}
/////////////
int Voice::get_device(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, voice->get_device());
	}		
	return 0;
}
/////////////
void * Voice::get_data()
{
	return &data[0];
}
/////////////
int Voice::get_data(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, voice->get_data());
	}		
	return 0;	
}
/////////////
bool Voice::is_recording()
{
	return recording;
}
/////////////
int Voice::is_recording(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		lua_pushboolean(L, voice->is_recording() );
		return 1;
	}	
	return 0;
}
/////////////
bool Voice::is_playing()
{
	#ifdef DOKUN_OPENAL
        ALenum playing;
        alGetSourcei(source, AL_SOURCE_STATE, &playing);
        return (playing == AL_PLAYING);	
	#endif
	return false;
}
/////////////
int Voice::is_playing(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		lua_pushboolean(L, voice->is_playing());
		return 1;
	}	
	return 0;	
}
/////////////
bool Voice::is_paused()
{
	#ifdef DOKUN_OPENAL
        ALenum paused;
        alGetSourcei(source, AL_SOURCE_STATE, &paused);
        return paused = (paused == AL_PAUSED);	
	#endif
	return false;
}
/////////////
int Voice::is_paused(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		lua_pushboolean(L, voice->is_paused());
		return 1;
	}	
	return 0;		
}
/////////////
bool Voice::is_stopped()
{
	#ifdef DOKUN_OPENAL
        ALenum stopped;
        alGetSourcei(source, AL_SOURCE_STATE, &stopped);
        return stopped = (stopped == AL_STOPPED);	
	#endif
	return false;
}
/////////////
int Voice::is_stopped(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Voice * voice = *static_cast<Voice **>(lua_touserdata(L, -1));
		lua_pushboolean(L, voice->is_stopped());
		return 1;
	}	
	return 0;	
}
/////////////
bool Voice::encode_opus(const std::string& output)
{
	/*
	opus_int32 Fs = 48000; // sample_rate(Hz) must be 8000, 12000, 16000, 24000, or 48000
	int channels = 2; // must be 1(mono) or 2(stereo)
	int bitrate = 64000;// bits_per_second = 8 bit or 16 bit
	int complexity = 5; // complexity = 1(lowest) to 10(highest)
	int signal_type = OPUS_AUTO;// signal_type is either OPUS_AUTO (default), OPUS_SIGNAL_VOICE, or OPUS_SIGNAL_MUSIC
	int application = OPUS_APPLICATION_VOIP;// coding mode(s): OPUS_APPLICATION_VOIP (for voice), OPUS_APPLICATION_AUDIO (both music and voice)
    //const opus_int16 *ppcm;
    int frame_size = 960; // frame_size = 120, 240, 480, 960, 1920, and 2880
   
	int max_frame_size = 6 * 960;
	int max_packet_size = 3 * 1276;
    opus_int16 pcm[frame_size * channels * sizeof(opus_int16)]; // 960 * 2
  unsigned char cbits[max_packet_size];
  
   int error;


/////////////
    char pcmout[6 * 960 * 2 * 2];//[4096]; // buffer and buffer size (4096)
    OggVorbis_File vf;
    int eof = false;
    int current_section = 0;
    // open File
	std::FILE * f; 
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&f, "res/hagane.ogg", "rb");
    #else		
	    f = fopen("res/hagane.ogg", "rb");
	#endif
	if(!f)
	{
		return false;
	}
	if(ov_open_callbacks(f, &vf, nullptr, 0, OV_CALLBACKS_NOCLOSE) != 0)
	{
        return false;
    }
  // read loop
    while(!eof)
	{
        long ret = ov_read(&vf, pcmout, sizeof(pcmout), 0, 2, 1, &current_section);
		(this)->data.insert((this)->data.end(), pcmout, pcmout + ret);
        if (ret == 0) 
		{
            eof = 1;
        } 
		else if (ret < 0) 
		{
	        std::cout << "(WRANING): Error in stream\n";
        } 
		else 
		{
            fwrite(pcmout,1,ret,stdin);
        }
    }	
///////////////////

	OpusEncoder * opus_enc = opus_encoder_create(Fs, channels, OPUS_APPLICATION_VOIP, &error);
	if(error != OPUS_OK)
	{
		std::cerr << "Encoding failed\n";
	}
	opus_encoder_ctl(opus_enc, OPUS_SET_BITRATE(bitrate)); 
    opus_encoder_ctl(opus_enc, OPUS_SET_COMPLEXITY(complexity));
    opus_encoder_ctl(opus_enc, OPUS_SET_SIGNAL(signal_type)); 
   

    FILE * fout = fopen(output.c_str(), "w");
    if (fout == NULL)
    {
        fprintf(stderr, "failed to open output file: %s\n", strerror(errno));
        return 0;
    }
//pcm = 
unsigned char pcm_bytes[6 * 960 * 2 * 2];
// Convert from little-endian ordering. 
      for (int i=0; i < 2 * 960 *sizeof(opus_int16); i++)
         pcm[i] = pcmout[2 * i + 1] << 8 | pcmout[2 * i];
	 
    opus_encode (opus_enc, pcm, frame_size, cbits, max_packet_size); // encode opus data

	fwrite(pcmout, sizeof(short), frame_size * channels * sizeof(opus_int16), fout); // write to file
	
ALuint format = AL_FORMAT_STEREO16;
	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(static_cast<ALuint>(buffer), format, cbits, static_cast<ALsizei>(max_packet_size), 48000);
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer)); 		
	
    opus_encoder_destroy (opus_enc);*/
	return true;
}/////////////////
bool Voice::decode_opus(const std::string& file_name)
{
	/*
	int error; 
	unsigned char cbits[3 * 1276]; 
    opus_int16 pcm[6 * 960 * 2];  // pcm output
    int nbBytes;
	int sample_rate, bit_rate, channels, pitch, volume;
	
	OpusDecoder * decoder = opus_decoder_create(48000, 2, & error); // sample_rate, channels
	if(error != OPUS_OK)
	{
		std::cerr << "Decoding failed" << std::endl;
		return false;
	}
	

    FILE * fp = fopen(file_name.c_str(), "r");
    if (fp == NULL)
    {
		std::cerr << "Could not open " << file_name << std::endl;
		return false;
	}	

	while(1)
	{
        int i;
      int frame_size;		
		// Read a 16 bits/sample audio frame. 
        fread(pcm, sizeof(short) * 2, 960, fp);
        if (feof(fp))
            break; // break on file end	  
	 
	 

	 

      frame_size = opus_decode(decoder, cbits, nbBytes, pcm, 6 * 960, 0);
      if (frame_size < 0)
      {
         fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
      }

  for(i = 0; i < 2 * frame_size; i++)
      {
         pcm[2 * i] = pcm[i]&0xFF;
         pcm[2 * i + 1] = (pcm[i]>>8)&0xFF;
      }	 
	  
	  data.insert(data.end(), pcm, pcm + frame_size);
	  
 

      
 // Write the decoded audio to file. 
      //fwrite(pcm_bytes, sizeof(short), frame_size*CHANNELS, fout);	  
	}
ALuint format = AL_FORMAT_STEREO16;
	
	alSourceUnqueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer));
	alBufferData(static_cast<ALuint>(buffer), format, &data[0], static_cast<ALsizei>(data.size()), 48000);
	alSourceQueueBuffers(static_cast<ALuint>(source), 1, static_cast<ALuint*>(&buffer)); 	
    	
   opus_decoder_destroy(decoder);
   fclose(fp);*/
   return true;
}
/////////////
void Voice::vorbis_to_opus(const std::string& in, std::string out)
{

	
	
	
	
}
/////////////
int Voice::new_(lua_State *L)
{
	lua_settop(L, 0); // clear stack
	// create table
	lua_createtable(L, 0, 0);
	// set metatable
	lua_getglobal(L, "Voice");
	lua_setmetatable(L, 1);
	// set userdata
	Voice **voice = static_cast<Voice**>(lua_newuserdata(L, sizeof(Voice*)));
    *voice = new Voice;
    lua_setfield(L, 1, "udata");	
	// return table
	if(lua_istable(L, -1)) 
		return 1;
	lua_pushnil(L);
	return 1;
}
