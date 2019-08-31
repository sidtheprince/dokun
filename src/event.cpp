#include "../include/event.h"

Event::Event() : type ( DOKUN_EVENT_PEEK )
{}
//////////
Event::~Event()
{}
//////////	
Event::Event(const Event& event)
{
	(this)->type = event.type;
}
//////////	
Event::Event(const dokun_Event& type)
{
	set_type(type);
}
//////////		
void Event::peek()
{
	set_type( DOKUN_EVENT_PEEK );
}
//////////
void Event::wait()
{
	set_type( DOKUN_EVENT_WAIT );
}
//////////
//////////
//////////
//////////
void Event::on_load(lua_State *L) // dokun.load
		{
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "load");
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.load is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);
		}
//////////
void Event::on_draw(lua_State *L) // dokun.draw
		{
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "draw");
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.draw is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);
		}
//////////
void Event::on_input(lua_State *L)
		{
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "input");
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.input is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);
			
			
			
			
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "mouse");
			// push mouse x, y pos
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.mouse is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);
			
	
	
	
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "keyboard");
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.keyboard is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);			
			
			
			
			
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "joystick");
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.joystick is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);		


			
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "vr");
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.vr is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);			
		}
//////////
void Event::on_update(lua_State *L)
{}
//////////
void Event::on_focus(lua_State *L)
		{
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "focus");
			#ifdef __windows__
			    HWND active = GetActiveWindow();
				if(!active) // no active window :C
			    {
				    lua_pushboolean(L, false); // push first argument (false)
			    }
			    if(active) // active window found :D
			    {
			        lua_pushboolean(L, true); // push first argument (true)
			    }
			#endif
			if(lua_type(L, -2) != LUA_TFUNCTION)
			{
			    std::cout << "dokun.focus is not  valid function" << std::endl;
				lua_pop(L, 3); // pop dokun and dokun.focus and boolean
				return;
		    }			
			lua_call(L, 1, 0); // call w/ 1 arg 0 returns	
		}
//////////
void Event::on_quit(lua_State *L) // dokun.quit
		{
			lua_getglobal(L, "dokun");
			lua_getfield(L, -1, "quit");
			if(lua_type(L, -1) != LUA_TFUNCTION)
			{
				std::cerr << "dokun.quit is not a valid function" <<std::endl;
				lua_pop(L, 2); // pop dokun and dokun.load
				return; // return if not a function
			}
			lua_call(L, 0, 0);		
		}
//////////
//////////
void Event::on_load()
{
	if(load_function)
	{
	    load_function();
	}
}
//////////
void Event::on_draw()
{
	if(draw_function)
	{
	    draw_function();
	}
}
//////////
void Event::on_update()
{
	if(update_function)
	{
	    update_function();
	}
}
//////////
void Event::on_focus()
{
	if(focus_function)
	{
	    focus_function();
	}
}
//////////
void Event::on_quit()
{
	if(quit_function)
	{
	    quit_function();
	}
}
//////////
/*
void Event::on_()
{
	();
}*/
//////////
//////////
//////////
//////////
void Event::set_type(int type)
{
	switch(type)
	{
		case 0:
		    set_type( DOKUN_EVENT_PEEK);
		    break;
		case 1:
		    set_type( DOKUN_EVENT_WAIT);
		    break;
	}
}
//////////
void Event::set_type(const dokun_Event& event)
{
	type = event;
}
//////////
void Event::set_callback(std::function<void (void)> f, std::string callback_t) // C++
{
	if(callback_t == "load")
	{
		load_function = f;
	}
	if(callback_t == "draw")
	{
		draw_function = f;
	}
	if(callback_t == "update")
	{
		update_function = f;
	}
	if(callback_t == "mouse")
	{
		mouse_function = f;
	}
	if(callback_t == "keyboard")
	{
		keyboard_function = f;
	}
	if(callback_t == "focus")
	{
		focus_function = f;
	}
	if(callback_t == "quit")
	{
		quit_function = f;
	}
}
//////////
void Event::set_callback(lua_State * L, std::string function_name, std::string callback_t) // Lua
{
	lua_getglobal(L, function_name.c_str()); // get function  1
    lua_getglobal(L, "dokun"); // get dokun  2
    lua_getfield(L, -1, callback_t.c_str()); // dokun.load, dokun.draw, etc.	3
	lua_pushvalue(L, 1); // push function
	lua_setfield(L, 2, callback_t.c_str()); // set dokun.load to function
}
//////////
//////////
//////////
//////////
dokun_Event Event::get_type()const
{
	return type;
}
//////////
//////////
int Event::open_mouse_device  ()
{
/*
    int fd, bytes;
    unsigned char data[3];//mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO
 #ifdef __gnu_linux__
    // check user's permission for a file
    if(::access("/dev/input/mice", R_OK) == -1) // #include <unistd.h>
        ::chmod("/dev/input/mice", S_IRWXU | S_IRWXG | S_IRWXO); // cat /proc/bus/input/devices # prints out all devices
    // before opening the device file, force admin privilidges
    //chmod, fchmod - change permissions of a file
    //int sudo = chmod("/dev/input/mice", S_IRWXU | S_IRWXG | S_IRWXO); // #include <sys/stat.h> //system("sudo chmod 744 /dev/input/mice"); // sudo gedit /dev/input/mice
    
    //
#endif
    int left, middle, right;
    signed char x, y; 

*/
#ifdef DOKUN_LINUX
    mouse_device = ::open("/dev/input/mice", O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO); // U=user, G=group, O=others// #include <sys/stat.h>, #include <fcntl.h>
    if(mouse_device == -1)
    {
        std::cerr << "Event::open_mouse_device_error : could not open: " << "/dev/input/mice" << std::endl;
        switch(mouse_device) // and give a reason for the error ...
        {
            case EACCES: std::cerr << "::open() : could not open \"/dev/input/mice\": permission denied" << std::endl; break;
        }
        return -1;
    }
#endif
    return 0; // no errors occured
}
//////////
void Event::close_mouse_device()
{
#ifdef DOKUN_LINUX
    ::close(mouse_device);
#endif
}
//////////
void Event::get_mouse_event()
{
#ifdef DOKUN_LINUX
    unsigned char data[3];
    if(::read(mouse_device, data, sizeof(data)) > 0)// read bytes bytes = ::read(fd, data, sizeof(data));
    {
        //int left = data[0] & 0x1; int right = data[0] & 0x2; int middle = data[0] & 0x4;
        //double x = data[1];
        //double y = data[2];
        //printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
    }
#endif    
}
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
