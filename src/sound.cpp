#include "../include/sound.h"

//////////
Sound::Sound()
{
	// store
	Factory::get_sound_factory()->store(this);
}
//////////
Sound::Sound(const std::string& file_name)
{
	// store
	Factory::get_sound_factory()->store(this);
	// load
	if(!load(file_name)) 
	{
		Logger("Could not load file " + file_name);
	}	
}
//////////
Sound::~Sound()
{
	Factory::get_sound_factory()->release(this);
}
//////////
bool Sound::load(const std::string& file_name)
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
int Sound::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // string or lightuserdata
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sound * sound = *static_cast<Sound **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TLIGHTUSERDATA) // works!
		{
			void * data = (void *)lua_touserdata(L, 2); // get data
			luaL_checktype(L, 3, LUA_TNUMBER);  // get size
			luaL_checktype(L, 4, LUA_TNUMBER);  // get bitrate
			luaL_checktype(L, 5, LUA_TNUMBER);  // get channels
			luaL_checktype(L, 6, LUA_TNUMBER);  // get samplerate
			//lua_pushboolean(L, static_cast<int>(sound->load(data, static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)), static_cast<int>(lua_tonumber(L, 5)), static_cast<int>(lua_tonumber(L, 6)) )));
			return 1;
		}
		if(lua_type(L, 2) == LUA_TSTRING)
	    {
		    lua_pushboolean(L, sound->load(lua_tostring(L, 2)));
		    return 1;
		}
	}
	lua_pushboolean(L, false);
	return 1;
}
//////////
//////////
// BOOLEAN
////////// 
bool Sound::is_sound()
{
	return ((this != 0) && (dokun::instanceof<Sound>(this) != 0));
}
////////// 
bool Sound::is_sound(Audible * audible)
{
	return ((audible != 0) && (dokun::instanceof<Sound>(audible) != 0));
}
//////////
int Sound::is_sound(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Audible * audible = *static_cast<Audible **>(lua_touserdata(L, -1));
		lua_pushboolean(L, dynamic_cast<Sound *>(audible)->is_sound());
		return 1;
	}
    lua_pushboolean(L, false);
    return 1;
}
//////////
//////////
int Sound::sound_new(lua_State *L)
{
	std::string file_name ;
	if(lua_type(L, -1) == LUA_TSTRING) 
	{
		file_name = lua_tostring(L, -1); // keep it at -1 just in case base class is pushed into "new" function Sprite:new()
	}	
	// remove all arguments pushed into the parameter
    lua_settop(L, 0);
    
	lua_createtable(L, 0, 0);
	
    lua_getglobal(L, "Sound"); // #2
    lua_setmetatable(L, 1); // pop mt
	
    Sound **sound = static_cast<Sound**>(lua_newuserdata(L, sizeof(Sound*)));
	if(!file_name.empty()) 
		*sound = new Sound(file_name);
	else
	    *sound = new Sound();
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, -1)) 
	{
	    return 1;
	}	
    lua_pushnil(L);
    return 1;	
}
