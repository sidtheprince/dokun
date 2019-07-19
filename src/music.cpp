#include "../include/music.h"

///////////////////
Music::Music()
{	
	Factory::get_music_factory()->store(this); // store
}
///////////////////
Music::Music(const std::string& file_name) 
{
	// load
	if(!load(file_name)) 
    {
        Logger("Could not load file " + file_name);
    }
	Factory::get_music_factory()->store(this);
}
///////////////////
Music::~Music()
{
	Factory::get_music_factory()->release(this);	
}
///////////////////
/*
bool Music::load(const std::string& file_name)
{
std::string extension = String::lower(file_name.substr(file_name.find_last_of(".") + 1));
	if(extension == "wav" || extension == "wave")
	{
		return load_wav(file_name);
	}		
	return load_ogg(file_name);	
}

int Music::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // string or lightuserdata
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Music * music = *static_cast<Music **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TLIGHTUSERDATA)
		{
			void * data = static_cast<void *>(lua_touserdata(L, 2));
			luaL_checktype(L, 3, LUA_TNUMBER);  // size of data
			luaL_checktype(L, 4, LUA_TNUMBER); // format AL_FORMAT_MONO or AL_FORMAT_STEREO
			luaL_checktype(L, 5, LUA_TNUMBER); // sample_rate 44100
			//lua_pushboolean(L, static_cast<int>(music->load(data, static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)), static_cast<int>(lua_tonumber(L, 5)) )));
			return 1;
		}
		if(lua_type(L, 2) == LUA_TSTRING)
	    {
		    lua_pushboolean(L, music->load(lua_tostring(L, 2)));
		    return 1;
		}
	}
	lua_pushboolean(L, false);
	return 1;
}
*/
/*
///////////////////
void Music::play()
{
	alSourcePlay (static_cast<ALuint>(source));
}

int Music::play(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Music * music = *static_cast<Music **>(lua_touserdata(L, -1));
		music->play();
	}	
	return 0;	
}
///////////////////
void Music::pause()
{
	alSourcePause (static_cast<ALuint>(source));
}

int Music::pause(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Music * music = *static_cast<Music **>(lua_touserdata(L, -1));
		music->pause();
	}	
	return 0;	
}
///////////////////
void Music::stop()
{
	alSourceStop(static_cast<ALuint>(source));
}

int Music::stop(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Music * music = *static_cast<Music **>(lua_touserdata(L, -1));
		music->stop();
	}	
	return 0;	
}
///////////////////
void Music::rewind()
{
	alSourceRewind(static_cast<ALuint>(source));
}

int Music::rewind(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Music * music = *static_cast<Music **>(lua_touserdata(L, -1));
		music->rewind();
	}	
	return 0;	
}
///////////////////
///////////////////
void Music::set_volume(int volume)
{
	alSourcef(static_cast<ALuint>(source), AL_GAIN, volume * 0.1);
	(this)->volume (volume);
}

int Music::set_volume(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Music * music = *static_cast<Music **>(lua_touserdata(L, -1));	
		music->set_volume(lua_tonumber(L, 2));
	}
	return 0;
}
///////////////////
/////////////////// 
int Music::get_volume()
{
	return volume;
}

int Music::get_volume(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Music * music = *static_cast<Music **>(lua_touserdata(L, -1));	
		lua_pushnumber(L, music->get_volume());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
*/
/////////////////// 
bool Music::is_music(Audio * audio)
{
	return ((audio != 0) && (dokun::instanceof<Music>(audio) != 0));
}
/////////////////// 
///////////////////
/*
bool Music::load_ogg(const std::string& file_name)
{
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
	
	ov_clear(&vf);
    fclose(f);
    return(true);	
}*/
///////////////////
int Music::new_(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		file_name = lua_tostring(L, -1);
	}
	// remove all arguments pushed into the parameter
    lua_settop(L, 0);
    
	lua_createtable(L, 0, 0);
	
    lua_getglobal(L, "Music_mt"); // #2
    lua_setmetatable(L, 1); // pop mt
	
    Music **music = static_cast<Music**>(lua_newuserdata(L, sizeof(Music*)));
	if(!file_name.empty())
		*music = new Music(file_name);
	else
	    *music = new Music();
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, -1)) 
	{
	    return 1;
	}		
	lua_pushnil(L);
	return 1;
}