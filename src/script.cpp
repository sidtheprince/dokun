#include "../include/script.h"

Script::Script(void)
{
	Factory::get_script_factory()->store(this);
}
//////////
Script::Script(lua_State *L, const std::string& file_name)
{
	
	if(load(L, file_name) == 0)
	{
		Logger("Could not load " + file_name);
	}
	Factory::get_script_factory()->store(this);
}
//////////
Script::~Script(void)
{
	Factory::get_script_factory()->release(this);
}
//////////
std::vector<std::string> Script::cache	({});
//////////
bool Script::load(lua_State * L, const std::string& file_name)
{
	if(!File::exists(file_name))
	{
		return false;
	}
	if(is_script(file_name)) // checks if script object is already attached to a file (one file per script_ptr)
	{
		return true;
	}
    if(luaL_dofile(L, file_name.c_str()) != 0)
    {
		lua_error(L);
        return false;
    }
	cache.push_back(file_name); // stores file only once to cache
	this->file = file_name; // save file if load succeeds
	return true;
}
//////////
void Script::load(lua_State * L, const std::string& file_name, int * error)
{
	if(!File::exists(file_name))
	{
		*error = DOKUN_ERROR_FILE_NOT_FOUND;
		return;
	}
	if(is_script(file_name))
	{
		*error = DOKUN_ERROR_NONE;
		return;
	}
    if(luaL_dofile(L, file_name.c_str()) != 0)
    {
		*error = DOKUN_ERROR_FILE_BROKEN;
		lua_error(L);
		return;
    }	
    cache.push_back(file_name); // stores file only once to cache	
	this->file = file_name; // save file if load succeeds
	*error = DOKUN_ERROR_NONE;
}
//////////
int Script::load(lua_State *L) // non static
{ 
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Script * script = *static_cast<Script **>(lua_touserdata(L, -1));	
	    lua_pushboolean(L, script->load (L, lua_tostring(L, 2)));
	    return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
//////////
bool Script::load_script(lua_State *L, const std::string& file_name)
{	
	Script script_ptr;
	return script_ptr.load (L, file_name);
}
//////////
int Script::load_script(lua_State *L) // static
{
	lua_pushboolean(L, Script::load_script (L, lua_tostring(L, -1)));
	return 1;
}
//////////
bool Script::load_directory(lua_State * L, const std::string& path)
{
	if(!File::checkdir(path))
	{
	    File::makedir(path);
	}
	std::vector<std::string> scripts = File::getdir(path, "*lua");
	for(int i = 0; i < scripts.size(); i++)
	{
		Script script_ptr;
		scripts[i] = path + "/" + scripts[i];                                                              //std::clog << "opening file " << scripts[i] << std::endl;
        if(!script_ptr.load(L, scripts[i]))
		{
			return false;
		}			
	}
	return true;	
}
//////////
int Script::load_directory(lua_State *L)
{
	lua_pushboolean( L, Script::load_directory( L, lua_tostring(L, -1) ) );
	return 1;
}
//////////
bool Script::execute(lua_State * L, const std::string& command)
{
	if(luaL_dostring(L, command.c_str()) != 0) {
		return false;
	}
    return true;	
}
//////////
int Script::execute(lua_State *L)
{
	lua_pushboolean(L, Script::execute(L, lua_tostring(L, -1)));
	return 1;	
}
//////////
//////////
//////////
//////////
//////////
void Script::unload(std::string file_name)
{
	for(int i = 0; i < cache.size(); i++)
	{
        if(cache[i] == file_name) 
		{	
			// remove from storage
			cache.erase(cache.begin() + i);
		}		
	}
}
//////////
void Script::generate(const char * script) // generate a main.lua (sets up a window, draws all created objects, etc.)
{
	if( !script)
	{
	    script = "main.lua";
	}
    std::ofstream file(script, std::ios::out | std::ios::binary);
    if (!file.is_open()) 
	{
		std::cerr << "(ERROR): Could not generate script" << std::endl;
		return;
	}
	//////////
	file << "require('engine')" << std::endl;
	file << "engine:start()" << std::endl;
	file << "" << std::endl;
	file << "WINDOW_MODE = 0" << std::endl;
	file << "FULL_SCREEN = 1" << std::endl;
	file << "" << std::endl;
	file << "-- Create a window" << std::endl;
	file << "window = Window:new()" << std::endl;
	file << "window:create('Window', 640, 480, WINDOW_MODE)" << std::endl;
	file << "window:show()" << std::endl;
	file << "" << std::endl;
	file << "while window:is_open() do" << std::endl;
	file << "   window:resize(window:get_width(), window:get_height())" << std::endl;
	file << "   window:clear()" << std::endl;
	file << "" << std::endl;
	file << "" << std::endl;
	file << "   window:update()" << std::endl;
	file << "end" << std::endl;
	file << "window:destroy()" << std::endl;
	file << "engine:close  ()" << std::endl;	
	//////////
	file.close();
}                                  
//////////
int Script::generate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_optstring(L, 2, "main.lua");
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Script * script = *static_cast<Script **>(lua_touserdata(L, -1));
		script->generate(lua_tostring(L, 2));
	}	
	return 0;
}
//////////
void Script::write(std::string code)
{
	if(!is_script()) // not loaded?
	{
		Logger("Could not write to script");
		return;
	}
	std::ofstream file(get_file(), std::ios::app);
	if(!file.is_open())
	{
		Logger("Could not write to " + get_file());
		return;
	}
	file << " " << code << std::endl;
	file.close();
}  
//////////
int Script::write(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Script * script = *static_cast<Script **>(lua_touserdata(L, -1));
		script->write(lua_tostring(L, 2));
	}	
	return 0;
}
//////////
void Script::copy(std::string file_name)
{
	std::ifstream file(file_name.c_str());
	if(!file.is_open())
	{
	    return;
	}
	std::stringstream stream;
	stream << file.rdbuf(); // dump file contents	
	
	content = stream.str();
} // get_content() -> String::replace()   you can replace any string from the lua file with anything
//////////
int Script::copy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Script * script = *static_cast<Script **>(lua_touserdata(L, -1));
		script->copy(lua_tostring(L, 2));
	}		
	return 0;
}
//////////
//////////
bool Script::save(lua_State *L, const std::string& table) // saves a table and its keys to a text file
{
	std::string file_name = table + ".txt";
	std::ofstream file(file_name.c_str());
	if(!file.is_open())
		return false;

	lua_getglobal(L, table.c_str()); // get table    1
	lua_pushnil(L); // get first key
    while(lua_next(L, 2) != 0) // key at -2, value at -1
    {							
	    std::string key = lua_tostring(L, -2); // get key as string
	    if(lua_type(L, -1) == LUA_TTHREAD)
		{
			lua_getglobal(L, "tostring");
			lua_pushvalue(L, -2);
			lua_call(L, 1, 1);
				
			std::string value = lua_tostring(L, -1);
			std::string thread = key + "     " + value + "      " + "thread\n";
			file.write(thread.c_str(), thread.size());			
			
			lua_pop(L, 1); // pop string					
		}
		if(lua_type(L, -1) == LUA_TFUNCTION)
		{
			lua_getglobal(L, "tostring");
		    lua_pushvalue(L, -2);
			lua_call(L, 1, 1);
				
			std::string value = lua_tostring(L, -1);
			std::string function = key + "     " + value + "      " + "function\n";
			file.write(function.c_str(), function.size());			
			
			lua_pop(L, 1); // pop string					
		}
		if(lua_type(L, -1) == LUA_TUSERDATA)
		{
			lua_getglobal(L, "tostring");
		    lua_pushvalue(L, -2);
			lua_call(L, 1, 1);
					
			std::string value = lua_tostring(L, -1);
			std::string udata = key + "     " + value + "     " + "userdata\n";
			file.write(udata.c_str(), udata.size());
			
			lua_pop(L, 1); // pop string					
		}
		if(lua_type(L, -1) == LUA_TLIGHTUSERDATA)
		{
			lua_getglobal(L, "tostring");
			lua_pushvalue(L, -2);
			lua_call(L, 1, 1);
				
			std::string value = lua_tostring(L, -1);
			std::string lightuserdata = key + "     " + value + "      " + "lightuserdata\n";
			file.write(lightuserdata.c_str(), lightuserdata.size());			
			
			lua_pop(L, 1); // pop string					
		}
		if(lua_type(L, -1) == LUA_TTABLE)
		{
			lua_getglobal(L, "tostring");
		    lua_pushvalue(L, -2);
			lua_call(L, 1, 1);
			
			std::string value = lua_tostring(L, -1);
			std::string table_info = key + "     " + value + "      " + "table\n";
			file.write(table_info.c_str(), table_info.size());			
		
			lua_pop(L, 1); // pop string	
		}			
        if(lua_type(L, -1) == LUA_TSTRING)
		{
			
			std::string value = lua_tostring(L, -1);
			std::string str_info = key + "     " + value + "      " + "string\n";
			file.write(str_info.c_str(), str_info.size());	    
		}
	    if(lua_type(L, -1) == LUA_TBOOLEAN)
		{
            std::string value;
			if(lua_toboolean(L, -1) != 0)
			{
		 	    value = "true";
			}  
			if(lua_toboolean(L, -1) == 0)
			{
			    value = "false";
			}
			std::string bool_info = key + "     " + value + "      " + "boolean\n";
			file.write(bool_info.c_str(), bool_info.size());
		}
		if(lua_type(L, -1) == LUA_TNUMBER)
		{
			std::string value = lua_tostring(L, -1);
			std::string number_info = key + "     " + value + "      " + "number\n";
			file.write(number_info.c_str(), number_info.size());			
		}
        lua_pop(L, 1); // pop key
	}
	// close file
	file.close(); 
	return true;
}
//////////
int Script::save(lua_State *L)
{
	lua_pushboolean(L, save(L, lua_tostring(L, -1)) );
	return 1;
}
//////////
void Script::get_table(lua_State *L, const std::string& table) // can also get subtables
{
	if(!String::contains(table, ".")) // no dots
	{
	    lua_getglobal(L, table.c_str());
		if(lua_type(L, -1) != LUA_TTABLE) // not a table?   YOU can only get tables with this function
		{
			lua_pop(L, 1); // pop it from stack
			return;
		}
	}
	// contains dots
    std::vector<std::string> list = String::split(table, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str()); // get first table
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str()); // get sub_table(s)		
		}
		if(lua_type(L, -1) != LUA_TTABLE) // not a table?   YOU can only get tables with this function
		{
			lua_pop(L, 1); // pop it from stack
			return;
		}		
	}		
}
//////////
//////////
std::string Script::get_string(lua_State * L, const std::string& key)
{
    if(!String::contains(key, "."))
	{
		lua_getglobal(L, key.c_str());
		if(!lua_tostring(L, -1))
		{
			return "";
		}
		return lua_tostring(L, -1);
	}
	std::vector<std::string> list = String::split(key, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());		
			if(lua_type(L, -1) == LUA_TSTRING) // .field is a string
			{
				if(!lua_tostring(L, -1)) // string is null
				{
					return "";
				}
				return lua_tostring(L, -1);
			}			
		}
	}
	return "";	
}
//////////
int Script::get_boolean(lua_State * L, const std::string& key)
{
    if(!String::contains(key, "."))
	{
		lua_getglobal(L, key.c_str());
		if(!lua_isboolean(L, -1)) // not a boolean
		{
			return -1;
		}
		return lua_toboolean(L, -1);
	}
	std::vector<std::string> list = String::split(key, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
			if(lua_type(L, -1) == LUA_TBOOLEAN)
			{
				return lua_toboolean(L, -1);
			}				
		}
	}
	return -1;	
}
//////////
double Script::get_number(lua_State * L, const std::string& key)
{
    if(!String::contains(key, "."))
	{
		lua_getglobal(L, key.c_str());
		if(!lua_isnumber(L, -1)) // not a boolean
		{
			return -1;
		}
		return lua_tonumber(L, -1);
	}
	std::vector<std::string> list = String::split(key, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
			if(lua_type(L, -1) == LUA_TNUMBER)
			{
				return lua_tonumber(L, -1);
			}				
		}
	}
	return -1;		
}
//////////
void * Script::get_userdata(lua_State * L, const std::string& key)
{
	if(!String::contains(key, "."))
	{
		lua_getglobal(L, key.c_str());
		if(!lua_isuserdata(L, -1)) // either full or light userdata
		{
			return nullptr;
		}
		return lua_touserdata(L, -1);
	}
	std::vector<std::string> list = String::split(key, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
            if(lua_type(L, -1) == LUA_TUSERDATA)
			{
				return lua_touserdata(L, -1);
			}						
		}
	}
	return nullptr;	
}
//////////
lua_CFunction Script::get_function(lua_State * L, const std::string& key)
{
	if(!String::contains(key, "."))
	{
		lua_getglobal(L, key.c_str());
		if(!lua_iscfunction(L, -1)) // not a C/C++ function
		{
			return nullptr;
		}
		return lua_tocfunction(L, -1);
	}	
    std::vector<std::string> list = String::split(key, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
            if(lua_iscfunction(L, -1))
			{
				return lua_tocfunction(L, -1);
			}						
		}
	}	
	return nullptr;
}
//////////
lua_State * Script::get_thread (lua_State * L, const std::string& key)
{
    if(!String::contains(key, "."))
	{
		lua_getglobal(L, key.c_str());
		if(!lua_isthread(L, -1))
		{
			return nullptr;
		}
		return lua_tothread(L, -1);
	}
    std::vector<std::string> list = String::split(key, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
			if(lua_type(L, -1) == LUA_TTHREAD)
			{
				return lua_tothread(L, -1);
			}
		}
	}		
	return nullptr; // default
}
//////////
void * Script::get_pointer(lua_State * L, const std::string& key)
{
    if(!String::contains(key, "."))
	{
		lua_getglobal(L, key.c_str());
        if(lua_isuserdata(L, -1) || lua_isthread(L, -1) || lua_isfunction(L, -1)) // must be either a userdata, thread, or a function
		{
		    return const_cast<void *>(lua_topointer(L, -1));
		}
	}
    std::vector<std::string> list = String::split(key, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
			if(lua_isuserdata(L, -1) || lua_isthread(L, -1) || lua_isfunction(L, -1))
			{
				return const_cast<void *>(lua_topointer(L, -1));
			}
		}
	}		
	return nullptr; // default
}
//////////
std::vector<std::string> Script::get_table_string(lua_State * L, const std::string& table)
{
	std::vector<std::string> string_array;
    Script::get_table(L, table.c_str()); // get table
	if(lua_istable(L, 1))
	{
	// get table size
    #ifdef DOKUN_LUA51
	    int size = lua_objlen(L, -1);
	#endif
	#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
	#endif		
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i); // push string from table
			string_array.push_back(lua_tostring(L, -1));
			lua_pop(L, 1); // pop string
		}			
	}
    return 	string_array;
}
//////////
std::vector<double> Script::get_table_number(lua_State * L, const std::string& table)
{
	std::vector<double> number_array;
    Script::get_table(L, table.c_str()); // get table
	if(lua_istable(L, 1))
	{
	// get table size
    #ifdef DOKUN_LUA51
	    int size = lua_objlen(L, -1);
	#endif
	#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
	#endif		
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i); // push number from table
			number_array.push_back(lua_tonumber(L, -1));
			lua_pop(L, 1); // pop number
		}			
	}
	return number_array;
}
//////////
std::vector<int> Script::get_table_integer(lua_State * L, const std::string& table)
{
	std::vector<int> number_array;
    Script::get_table(L, table.c_str()); // get table
	if(lua_istable(L, 1))
	{
	// get table size
    #ifdef DOKUN_LUA51
	    int size = lua_objlen(L, -1);
	#endif
	#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
	#endif		
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i); // push number from table
			number_array.push_back(lua_tointeger(L, -1));
			lua_pop(L, 1); // pop number
		}			
	}
	return number_array;
}
//////////
std::vector<float> Script::get_table_float(lua_State * L, const std::string& table)
{
	std::vector<float> number_array;
    Script::get_table(L, table.c_str()); // get table
	if(lua_istable(L, 1))
	{
	// get table size
    #ifdef DOKUN_LUA51
	    int size = lua_objlen(L, -1);
	#endif
	#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
	#endif		
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i); // push number from table
			number_array.push_back(lua_tonumber(L, -1));
			lua_pop(L, 1); // pop number
		}			
	}
	return number_array;
}
//////////
std::vector<double> Script::get_table_double(lua_State * L, const std::string& table)
{
    return get_table_number(L, table);
}
//////////
std::vector<const void *> Script::get_table_pointer(lua_State * L, const std::string& table) // can be a userdata, a table, a thread, or a function
{
	std::vector<const void *> pointer_array;
    Script::get_table(L, table.c_str()); // get table
	if(lua_istable(L, 1))
	{
	// get table size
    #ifdef DOKUN_LUA51
	    int size = lua_objlen(L, -1);
	#endif
	#ifndef DOKUN_LUA51
		int size = luaL_len(L, -1);
	#endif		
		for(int i = 1; i <= size; i++)
		{
			lua_rawgeti(L, -1, i); // push  from table
			pointer_array.push_back(lua_topointer(L, -1)); // can be a userdata, a table, a thread, or a function
			lua_pop(L, 1); // pop 
		}			
	}
    return 	pointer_array;
}
//////////
bool Script::call(lua_State * L, const std::string& function, int returns)
{
	if(!String::contains(function, "."))
	{
		lua_getglobal(L, function.c_str());
		if(!lua_isfunction(L, -1)) // not a function
		{
			return false;
		}
		lua_call(L, 0, returns);
		return true;
	}
    std::vector<std::string> list = String::split(function, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
            if(lua_isfunction(L, -1))
			{
				lua_call(L, 0, returns); // call function (function must have 0 arguments)
				return true;
			}						
		}
	}	
    return false;	
}
//////////
std::string Script::get_type(lua_State * L, const std::string& object)
{
    if(!String::contains(object, "."))
	{
		lua_getglobal(L, object.c_str());
		if(lua_type(L, -1) == LUA_TTHREAD)
		{
			return "thread";
		}
		if(lua_isfunction(L, -1))
		{
			return "function";
		}			
		if(lua_type(L, -1) == LUA_TUSERDATA)
		{
			return "userdata";
		}
		if(lua_type(L, -1) == LUA_TLIGHTUSERDATA)
		{
			return "lightuserdata";
		}
		if(lua_type(L, -1) == LUA_TTABLE)
		{
			return "table";
		}
		if(lua_type(L, -1) == LUA_TSTRING)
		{
			return "string";
		}
		if(lua_type(L, -1) == LUA_TBOOLEAN)
		{
			return "boolean";
		}
		if(lua_type(L, -1) == LUA_TNUMBER)
		{
			return "number";
		}
		if(lua_type(L, -1) == LUA_TNIL)
		{
			return "nil";
		}
		if(lua_type(L, -1) == LUA_TNONE)
		{
			return "none";
		}
	}
    std::vector<std::string> list = String::split(object, ".");
	if(!list.empty())
	{
		lua_getglobal(L, list[0].c_str());
		for(unsigned int i = 1; i <= (list.size() - 1); i++)
		{
			lua_getfield(L, -1, list[i].c_str());
			if(lua_type(L, -1) == LUA_TTHREAD)
			{
				return "thread";
			}
			if(lua_isfunction(L, -1))
			{
				return "function";
			}			
			if(lua_type(L, -1) == LUA_TUSERDATA)
			{
				return "userdata";
			}
			if(lua_type(L, -1) == LUA_TLIGHTUSERDATA)
			{
				return "lightuserdata";
			}
			if(lua_type(L, -1) == LUA_TTABLE)
			{
				return "table";
			}
			if(lua_type(L, -1) == LUA_TSTRING)
			{
				return "string";
			}
			if(lua_type(L, -1) == LUA_TBOOLEAN)
			{
				return "boolean";
			}
			if(lua_type(L, -1) == LUA_TNUMBER)
			{
				return "number";
			}
			if(lua_type(L, -1) == LUA_TNIL)
			{
				return "nil";
			}
			if(lua_type(L, -1) == LUA_TNONE)
			{
				return "none";
			}
		}
	}		
	return "nil";
}
//////////
//////////
//////////
int Script::table(lua_State *L, std::string class_name)
{
	lua_newtable(L);
	lua_newtable(L); 
	lua_pushvalue(L, -2);
	lua_setfield(L, -2, "__index"); 
	std::string mt(class_name + "_mt");
	lua_setglobal(L, mt.c_str()); 
    lua_setglobal(L, class_name.c_str());	
	return 0;
}
////////////
int Script::function(lua_State *L, std::string table_name, std::string function_name, lua_CFunction function)
{
	lua_getglobal(L, table_name.c_str());
	if(!lua_istable(L, -1))	
	{
		lua_pop(L, 1);
		luaL_error(L, "Table %s not found", table_name.c_str());
	}
	lua_pushcfunction(L, function); 
	lua_setfield(L, -2, function_name.c_str());
    lua_pop(L, 1);
	return 0;
}
////////////
int Script:: member (lua_State *L, std::string table_name, std::string value_name, std::string value, int type)
{
	if( type == LUA_TSTRING )
	{
		lua_getglobal(L, table_name.c_str()); 
	    if(!lua_istable(L, -1))	
	    {
			lua_pop(L, 1);
		    luaL_error(L, "Table %s not found", table_name.c_str());
	    }
		lua_pushstring(L, value.c_str());
		lua_setfield(L, -2, value_name.c_str());
        lua_pop(L, 1);
	}
	if( type == LUA_TTABLE )
	{
		lua_getglobal(L, table_name.c_str());
	    if(!lua_istable(L, -1))	
	    {
		    lua_pop(L, 1);
			luaL_error(L, "Table %s not found", table_name.c_str());
	    }
		lua_getglobal(L, value.c_str());
		if(lua_istable(L, -1))
		{
			lua_setfield(L, -2, value_name.c_str());
			lua_pop(L, 1);
		}
		else
		{ 
		    lua_pop(L, 1); 
			lua_createtable(L, 0, 0);
			lua_setfield(L, -3, value_name.c_str());
			lua_pop(L, 1);
		}
	} 	
	lua_settop(L, 0);
	return 0;
}
////////////
int Script:: member (lua_State *L, std::string table_name, std::string value_name, int value, int type)
{
	if( type == LUA_TNUMBER )
	{
		lua_getglobal(L, table_name.c_str());
	    if(!lua_istable(L, -1))	
	    {
		    
		    lua_pop(L, 1);
			luaL_error(L, "Table %s not found", table_name.c_str());
	    }
		lua_pushnumber(L, value);
		lua_setfield(L, -2, value_name.c_str());
        lua_pop(L, 1);
	}
	if( type == LUA_TBOOLEAN )
	{
		lua_getglobal(L, table_name.c_str());
	    if(!lua_istable(L, -1))	
	    {
		    
		    lua_pop(L, 1);
			luaL_error(L, "Table %s not found", table_name.c_str());
	    }
		lua_pushboolean(L, value);
		lua_setfield(L, -2, value_name.c_str());
        lua_pop(L, 1);
	}
	lua_settop(L, 0);
	return 0;
}
////////////
int Script:: global (lua_State *L, std::string function_name, lua_CFunction function)
{
	lua_pushcfunction(L, function);
	lua_setglobal(L, function_name.c_str());
	// lua_register(L, function_name.c_str(), function );
	return 0;
}
////////////
int Script:: global (lua_State *L, std::string variable_name, std::string value)
{
	lua_pushstring(L, value.c_str());
	lua_setglobal(L, variable_name.c_str());
	return 0;
}
////////////
int Script:: global (lua_State *L, std::string variable_name, int value)
{
	lua_pushnumber(L, value);
	lua_setglobal(L, variable_name.c_str());
	return 0;
}
////////////
int Script:: global (lua_State *L, std::string variable_name, bool value)
{
	lua_pushboolean(L, value);
	lua_setglobal(L, variable_name.c_str());
	return 0;
}
////////////
int Script:: inherit (lua_State *L, std::string base_table, std::string sub_table)
{
	lua_getglobal(L, sub_table.c_str());
	if(!lua_istable(L, -1))	
	{
		lua_pop(L, 1);
	    luaL_error(L, "Table %s not found", sub_table.c_str()); 
	}
	lua_getglobal(L, base_table.append("_mt").c_str());
	if(!lua_istable(L, -1))	
	{
		lua_pop(L, 2);
		luaL_error(L, "Table %s not found", base_table.c_str());
	}
	lua_setmetatable(L, -2);
	lua_pop(L, 1);
	return 0;
}
////////////
int Script::attach(lua_State *L, std::string base_table, std::string sub_table)
{
    lua_getglobal(L, base_table.c_str());
	if(!lua_istable(L, -1))	
	{
		lua_pop(L, 1);
		luaL_error(L, "Table %s not found", base_table.c_str());
	}
	lua_getglobal(L, sub_table.c_str());
	if(lua_istable(L, -1))
	{
		lua_setfield(L, -2, sub_table.c_str());
		lua_pop(L, 1);
	}
	else
	{       
		    lua_pop(L, 1);
			lua_createtable(L, 0,0);
			lua_setfield(L, -2, sub_table.c_str());
			lua_newtable(L);
			lua_getfield(L, -2, sub_table.c_str());
			lua_setfield(L, -2, "__index");
			lua_setglobal(L, sub_table.append("_mt").c_str());
			lua_pop(L, 1);
	}	
	return 0;
}
////////////
//////////
//////////
//////////
//////////
std::string Script::get_file()
{
	return file;
}       
//////////         
int Script::get_file(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Script * script = *static_cast<Script **>(lua_touserdata(L, -1));
		lua_pushstring(L, script->get_file().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
//////////
std::string Script::get_content()
{
	if(!is_script()) // not loaded before?
	{
		return ("");
	}
	copy(get_file()); // updates script by copying file contents
	return (content);
}
//////////
//////////
//////////
int Script::get_count()
{
	return cache.size();
} 
//////////
int Script::get_count(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Script * script = *static_cast<Script **>(lua_touserdata(L, -1));
		lua_pushnumber(L, script->get_count() );
		return 1 ;
	}	
	lua_pushnil(L);
	return 1;
}
//////////
//////////
//////////
bool Script::is_script() // returns true if script has a file name( meaning the script is loaded)
{
	if(get_file().empty()) // no file attached to script object
	{
		return false;
	}
	return true;
}
//////////
bool Script::is_script(std::string file_name) // returns true if a script has been loaded (previously)
{
	for (unsigned int i = 0; i < cache.size(); i++)
	{	
		if(cache[i] == file_name)
		{
			return true;
		}
	}
	return false;
}
//////////
int Script::is_script(lua_State *L)
{
	lua_pushboolean(L, Script::is_script(lua_tostring(L, -1)));
	return 1;
}
//////////
//////////
int Script::new_(lua_State *L)
{
	lua_settop(L, 0);
	// create table
	lua_createtable(L, 0, 0);
	// create mt
	lua_getglobal(L, "Script_mt");
	lua_setmetatable(L, 1);
	// create udata
	Script **script = static_cast<Script **>(lua_newuserdata(L, sizeof(Script *)));
	*script = new Script();
	lua_setfield(L, 1, "udata");
	// non-static load function
	lua_pushcfunction(L, Script::load);
	lua_setfield(L, 1, "load");
	// return table
	if(lua_istable(L, -1))
		return 1 ;
	lua_pushnil(L);
	return 1;
}
//////////
