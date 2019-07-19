#include "../include/animator.h"

// set initial action and frame count to 0
Animator::Animator() : frames(0), actions(0), speed(5.0), update_time(1.0 / speed), last_time(0.0)
{}
//////////////
Animator::~Animator()
{}
//////////////
// lets assume a smart c++ programmer knows that arrays start at 0
bool Animator::load(const std::string& file_name, int action_num, int frame) 
{
    // Load an image.
    Texture * texture = new Texture();
    bool status = texture->load(file_name);
    // Load was success.
    if(status)
    {
        // Create an empty row.
        action_list.push_back(std::vector<Texture *>());
        // Store the texture in action_list as a frame.
        action_list[action_num].push_back(texture); // action_list[0][0] = texture1
	    // increment action count
	    if(frame == 0) // The frame being 0 indicates that a new action has been created
	        actions++;
    }
    return status;
} // eof
//////////////
bool Animator::load(Animator &animator, std::string file_name, int action_num, int frame)
{
    return animator.load(file_name, action_num, frame);
}
//////////////
bool Animator::load_rect(const std::string& file_name, int x, int y, int width, int height, int action_num, int frame)
{
	Texture * texture = new Texture();
	bool status = texture->load(file_name);
	if(status)
	{
		texture->set_rect(x, y, width, height);
        // Create an empty row.
        action_list.push_back(std::vector<Texture *>());
        // Store the texture in action_list as a frame.
        action_list[action_num].push_back(texture); // action_list[0][0] = texture1
	    // increment action count
	    if(frame == 0) // The frame being 0 indicates that a new action has been created
	        actions++;		
	}
	return status;
}
//////////////
bool Animator::load_rect(const std::string& file_name, const Vector2& position, const Vector2& size, int action_num, int frame)
{
	Texture * texture = new Texture();
	bool status = texture->load(file_name);
	if(status)
	{
		texture->set_rect(position.x, position.y, size.x, size.y);
        // Create an empty row.
        action_list.push_back(std::vector<Texture *>());
        // Store the texture in action_list as a frame.
        action_list[action_num].push_back(texture); // action_list[0][0] = texture1
	    // increment action count
	    if(frame == 0) // The frame being 0 indicates that a new action has been created
	        actions++;		
	}
	return status;	
}
//////////////
bool Animator::load_rect(const std::string& file_name, const Vector4& rect, int action_num, int frame)
{
	Texture * texture = new Texture();
	bool status = texture->load(file_name);
	if(status)
	{
		texture->set_rect(rect.x, rect.y, rect.z, rect.w);
        // Create an empty row.
        action_list.push_back(std::vector<Texture *>());
        // Store the texture in action_list as a frame.
        action_list[action_num].push_back(texture); // action_list[0][0] = texture1
	    // increment action count
	    if(frame == 0) // The frame being 0 indicates that a new action has been created
	        actions++;		
	}
	return status;	
}
//////////////
int Animator::load(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Animator * animator = *static_cast<Animator **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, animator->load( lua_tostring(L, 2), (unsigned int)lua_tonumber(L, 3), (unsigned int)lua_tonumber(L, 4)));
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
//////////////

void Animator::animate(Sprite *sprite, int action) 
{  // (NOTE): ANimation is not possible without drawing
    // if single frame (image) or less
    if(get_frames(action) < 2) 
    {
        std::cerr << "(Error) : Not enough frames in action " << action << " to start animation"<< std::endl;
    }
    if( get_frames(action) >= 2 )
    {
    // print all frames(file names) in action
        #ifdef DOKUN_DEBUG
            for(unsigned int i = 0; i < action_list[action].size(); i++)
            {  
                std::cout << action_list[action][i]->get_file() << std::endl;
            }
	    #endif
	    // update texture
        sprite->set_texture( *action_list[action][frames] );
        // run frames
		frames = frames + 1;
        // adjust speed
		last_time += Timer::delta();
		if(last_time > update_time)
			last_time = 0;
		// if frame is has reached its end
        if( frames >= get_frames(action) )
		{			
            // start from the beginning
            frames = 0;
		}  
    }
} // eof
//////////////
void Animator::animate(Animator &animator, Sprite *sprite, int action)
{
    animator.animate(sprite, action);
}
//////////////
int Animator::animate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite**>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
	        Animator * animator = *static_cast<Animator **>(lua_touserdata(L, -1));
	        animator->animate( sprite, (unsigned int)lua_tonumber(L, 3));
		    return 1;
	    }
	}
	return 0;
}

//////////////
// setters
void Animator::set_current_frame(std::string filename) { // Replaced with Sprite::set_texture
 // set_texture(filename);
 // or load(filename) if no 'set_texuture' function
 // sprite->set_texture(Tex);
 // or some other way to set the current sprite image
}
//////////////
void Animator::set_speed(double speed)
{
    (this)->speed = speed;
}
//////////////
void Animator::set_speed(Animator &animator, double speed)
{
    animator.set_speed(speed);
}
//////////////
int Animator::set_speed(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Animator * animator = *static_cast<Animator **>(lua_touserdata(L, -1));
		animator->set_speed( (double)lua_tonumber(L, 2));
	}
	return 0;
}
//////////////
// getters
int Animator::get_actions()const
{
    return actions;
}
//////////////
int Animator::get_actions(Animator& animator)
{
    return animator.get_actions();
}
//////////////
int Animator::get_actions(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Animator * animator = *static_cast<Animator **>(lua_touserdata(L, -1));
		lua_pushnumber(L, animator->get_actions());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
//////////////
int Animator::get_frames(int action_num)const // will crash if action does not exist
{
// number of frames in one action
    return action_list[action_num].size();
}
//////////////
int Animator::get_frames(Animator &animator, int action_num)
{
    return animator.get_frames(action_num);
}
//////////////
int Animator::get_frames(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Animator * animator = *static_cast<Animator **>(lua_touserdata(L, -1));
		lua_pushnumber(L, animator->get_frames( (unsigned int)lua_tonumber(L, 2)));
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
//////////////
double Animator::get_speed()
{
	return speed ;
}
//////////////
double Animator::get_speed(Animator & animator)
{
	return animator.get_speed ();
}
//////////////
int Animator::get_speed(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Animator * animator = *static_cast<Animator **>(lua_touserdata(L, -1));
		lua_pushnumber(L, animator->get_speed());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
//////////////
//////////////
int Animator::new_(lua_State *L)
{
     lua_settop(L, 0); // clear stack
	 // create table (object)
	 lua_createtable(L, 0, 0);
	 // set metatable
	 lua_getglobal(L, "Animator_mt");
	 lua_setmetatable(L, 1);
	 // set userdata
	 Animator **animator = static_cast<Animator **>(lua_newuserdata(L, sizeof(Animator *)));
	 *animator = new Animator();
	 lua_setfield(L, 1, "udata");
	 // return table
	 if(lua_istable(L, -1)) 
	 {
	    return 1;
	 }
	 lua_pushnil(L);
	 return 1;
}
//////////////