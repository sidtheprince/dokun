#include "../include/engine.h"
////////////
Engine::Engine(void) 
{}
////////////
Engine::~Engine(void)
{
	close();
}
////////////
Engine * Engine::engine_ptr (new Engine());
////////////
bool Engine::status (false);
////////////
bool Engine::open()
{
	return on_open();
}
////////////
int Engine::open(lua_State *L)
{
    lua_pushboolean(L, open());
	return 1;
}
////////////
void Engine::close()
{
	on_close();
}
////////////
int Engine::close (lua_State *L) 
{
	on_close();
	lua_close(L);
	exit(0);
	return 0;
}
////////////
////////////
////////////
Engine * Engine::get_engine()
{
	return engine_ptr;
}
//////////// 
bool Engine::get_status()
{
	return status;
}
//////////// 
bool Engine::on_open()
{
#ifdef __gnu_linux__
    std::atexit(Engine::close);
#endif
#ifdef DOKUN_VULKAN
#ifdef __windows__
	HMODULE vulkan = LoadLibrary("vulkan-1.dll");
#endif
#ifdef __gnu_linux__
	void * vulkan = dlopen("libvulkan.so", RTLD_NOW);
#endif
	if(vulkan == nullptr)
	{
		Logger("Vulkan not detected");
		status = 0;
		return false;
	}
#ifdef __windows__
	const char * vulkan_ext[] = { "VK_KHR_surface", "VK_KHR_win32_surface" }; // enable extensions
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    const char * vulkan_ext[] = { "VK_KHR_surface", "VK_KHR_xlib_surface"  }; // enable extensions
#endif
#endif	 
	get_engine()->app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    get_engine()->app_info.pNext = nullptr;
    get_engine()->app_info.pApplicationName   = nullptr;
    get_engine()->app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    get_engine()->app_info.pEngineName   = "Dokun";
    get_engine()->app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);	
    get_engine()->app_info.apiVersion    = VK_MAKE_VERSION(1, 0, 2);			
	Renderer::get_pointer()->instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	Renderer::get_pointer()->instance_info.pNext = nullptr;
	Renderer::get_pointer()->instance_info.flags = 0;
	Renderer::get_pointer()->instance_info.pApplicationInfo = &get_engine()->app_info;
	Renderer::get_pointer()->instance_info.enabledLayerCount = 0;
	Renderer::get_pointer()->instance_info.ppEnabledLayerNames = nullptr;
	Renderer::get_pointer()->instance_info.enabledExtensionCount = 2;
#ifndef DOKUN_GLFW
	Renderer::get_pointer()->instance_info.ppEnabledExtensionNames = vulkan_ext;
#endif	
    VkResult result = vkCreateInstance(& Renderer::get_pointer()->instance_info, nullptr, & Renderer::get_pointer()->instance);
	if(result != VK_SUCCESS) 
	{
		Logger("Could not start Vulkan");
		DOKUN_VULKAN_ERROR(result, DOKUN_FILE, DOKUN_LINE);
		status = 0;
		return false;
	}
#endif
    if(!FONT::open())
	{
		Logger("Could not start FreeType");
		status = 0;
		return false;
	}
#ifdef __windows__
	if (WSAStartup(MAKEWORD(2, 2), & get_engine()->wsa_data) != 0)
    {
		Logger("Could not start Winsocket : " + std::to_string(WSAGetLastError()));
		status = 0;
		return false;
    }
#endif
    if(!Audio::open())
	{
		Logger("Could not start OpenAL");
		status = 0;
		return false;
	}
    /////////////////////////////////////////
    Renderer::start(); // initializes Renderer by setting default values
    // load the default font (ONLY once so that you do not have to load it a hundred times)
    if(!Label::default_font->load(DEFAULT_FONT_PATH)) Logger("Could not load default font from " + String(DEFAULT_FONT_PATH).str());
	/////////////////////////////////////////
    Logger::push("=======================================================================================================");
	Logger::push("Started dokun at " + Logger::format("[%Y-%m-%d  %H:%M:%S %p]"));
	Logger::push("=======================================================================================================");	
    Logger::push("\n\n");	
	Logger::push(String("Running dokun version  ") + String(ENGINE_VERSION));
	Logger::push(String("Running Lua version    ") + String::to_string(LUA_VERSION_MAJOR) + "." + String::to_string(LUA_VERSION_MINOR) + "." + String::to_string(LUA_VERSION_RELEASE));
	Logger::push("\n\n");
//#define DOKUN_DEBUG_VERSION
#ifdef DOKUN_DEBUG_VERSION
    Logger::push(String("Using freetype version ") + String(FREETYPE_MAJOR).str() + "." + String(FREETYPE_MINOR).str() +  "." + String(FREETYPE_PATCH).str());
    Logger::push(String("Using giflib version   ") + String(GIFLIB_MAJOR).str() + "." + String(GIFLIB_MINOR).str() +  "." + String(GIFLIB_RELEASE).str());
#endif    
    // create dir for logfiles
	if(!File::checkdir("log")) File::makedir("log");
	// save in append mode
	if(!Logger::save("log/doklog.txt")) Logger("Failure to save log");
	// pop a number of times after saving first section
	for(int i = 0; i < 6; i++) Logger::pop();
	/////////////////////////////////////////
	// set status to 1 (proof that engine has been initialized)
	status = 1;
	return true;
}
////////////
void Engine::on_close()
{
	if(status != 0) // if engine is not off
	{
	    FONT::close(); // close freetype
#ifdef DOKUN_VULKAN
        if(Renderer::get_instance() != VK_NULL_HANDLE ) {
            vkDestroyInstance (Renderer::get_instance(), nullptr);
        }
#endif
#ifdef __windows__
#ifdef DOKUN_VULKAN
        //FreeLibrary(vulkan);
#endif
	    WSACleanup ();  // close winsocket
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_VULKAN
        //dlclose(vulkan);
#endif        
#endif    
        Audio::close(); // close openal
		/////////////////////////////////////////
        Logger::push("\n=======================================================================================================");
        Logger::push("Closed dokun at " + Logger::format("[%Y-%m-%d  %H:%M:%S %p]"));
        Logger::push("=======================================================================================================\n");
		if(!Logger::save("log/doklog.txt")) Logger("Failure to save log");// save session
		/////////////////////////////////////////
        // exit message
		Logger("Exiting with code 0 ...");
	    // reset status to its default
		status = 0;		
	}
}
////////////
////////////
////////////
int Engine::test(lua_State *L)
{
	WINDOW window;
	window.create("Test", 1280, 720, 0); // titlebar is 16x39 //window.set_size(1500, 900);
	window.show();
    //=================== script
	//Script * script = new Script(L, "config.lua"); // load script
    //=================== misc
    //=================== sprite
	Sprite * sprite = new Sprite();
	Texture * texture = new Texture();//("res/image.png");
	sprite->set_texture(*texture); // set empty texture to sprite
	sprite->set_color(0, 51, 102);
	sprite->set_position(200, 200);
    //=================== 
    Model * model = new Model();
    model->load("res/monkey.obj");
    model->set_position(0, 0, -6);
    //=================== music
    Music * music = new Music("res/Oniku.ogg");
    music->set_loop(true);
    music->set_volume(1);
    music->play();
    //=================== variable time step (recommended)
    double old_time = 0; 
    double new_time = (clock() / (double)CLOCKS_PER_SEC);
    double delta_time;
	double fps_desired = 60;
    //===================
	//Voice * voice = new Voice();
    std::vector<Sprite *> sprite_array;
    for(int i = 0; i < 10; i++)
    {
        Sprite * sprite_obj = new Sprite();
        sprite_obj->set_position((rand() % 1000) + 1, (rand() % 100) + 1); // rand
        sprite_obj->set_texture(* new Texture());
        sprite_obj->set_color((rand() % 100) + 1, (rand() % 100) + 1, (rand() % 100) + 1);
        sprite_array.push_back(sprite_obj);
    }
    //===================
    Widget * widget = new Widget();
	widget->set_title_bar(true);
	widget->set_title_bar_size(20); // update titlebar_size before doing any operations with it
    widget->set_title_bar_button_close(true);
    widget->set_position(0, 0 + widget->get_title_bar_size().y); // box_position should also include titlebar_position if "has_title_bar()" (or else the box's titlebar will be overlaped by the window's titlebar) // So basically the titlebar is a seperate entity with its own size and position though a part/extension of box

    Label * label = new Label();
    label->set_string("Sid"); 
    label->set_background_color(32, 32, 32);
    label->set_position(window.get_client_width() / 2, 10);

    Grid * grid = new Grid();
	grid->set_row(2);
	grid->set_column(2);
	grid->set_position(100, 400);
	grid->get_block(0, 0)->set_color(255, 51, 51);
	std::cout << "Grid: " << grid->get_row_count() << " x " << grid->get_column_count() << std::endl;
	//===================
    while(window.is_open()) // main loop
    {
		window.set_viewport(window.get_client_width(), window.get_client_height()); // set to dimensions of render client rather than the entire window (for pixel coordinates)
		window.clear(14, 19, 29);//(32, 32, 32);//(33, 39, 60)//darkgreyblue
		
        old_time = new_time;
        new_time = (clock() / (double)CLOCKS_PER_SEC) * 1000;// get time in milliseconds or something; increments by the 100s
        delta_time = new_time - old_time;
        //std::cout << "delta_time: " << delta_time << "\n"; 
		//////////////////////////////////////////////////////
		//voice->start();
        Camera * camera = Renderer::get_camera();            //std::cout<<"Rendering area: " << Renderer::get_display_size() << std::endl;//Renderer::set_display_size(window.get_client_width(), window.get_client_height());//on linux the rendering area is the usual full window size, on windows, the rendering area is the client area and not the full window_size
		// get_sprite_position_after_rotated
        Vector3 cam_pos    = camera->get_position();
        Vector3 model_pos  = model->get_position();
        Vector3 sprite_pos = Vector3(sprite->get_position(), -1);

        sprite->scale(3, 3);
        ////////////////////////////////////////////////////// Camera
        // camera follow sprite test
        int camera_height = -(window.get_client_height() / 2);// keep camera at center height of screen;  negative = up   positive = down
        camera->set_position(Vector3(sprite->get_position(), 0.0) + Vector3(-(window.get_client_width() - window.get_client_height() / 2), camera_height, 0) );//Vector3(0, camera_height, 0)//Vector3(cam_x_position_away_from_sprite, camera_height, 0)
		// rotate camera at a point (an origin)
        //std::cout << "Label size: " << label->get_size() << " (all glyphs' width combined, highest glyph height)" << std::endl;
        //std::cout << "Model position: " << model->get_position()<< std::endl;
        ////////////////////////////////////////////////////// Controls
	    if(window.is_focused()) // if window is focused
	    {
            if(music->is_paused()) music->play();

			if(Keyboard::is_pressed(DOKUN_KEY_UP   )){/*label->set_position( label->get_x(), label->get_y()-1 );*/ model->translate(0, 0, -1);sprite->translate(0       ,-5 * 0.5);}
			if(Keyboard::is_pressed(DOKUN_KEY_DOWN )){/*label->set_position( label->get_x(), label->get_y()+1 );*/ model->translate(0, 0, 1); sprite->translate(0       , 5 * 0.5);}
			if(Keyboard::is_pressed(DOKUN_KEY_LEFT )){/*label->set_position( label->get_x()-1, label->get_y() );*/ model->translate(1, 0, 0); sprite->translate(-5 * 0.5, 0 );}
			if(Keyboard::is_pressed(DOKUN_KEY_RIGHT)){/*label->set_position( label->get_x()+1, label->get_y() );*/ model->translate(-1, 0, 0);sprite->translate(5 * 0.5 , 0 );}
			
			if(Keyboard::is_pressed(DOKUN_KEY_1)) camera->set_zoom(camera->get_zoom() + 1); // zoom_out
			if(Keyboard::is_pressed(DOKUN_KEY_2)) camera->set_zoom(camera->get_zoom() - 1); // zoom_in
            if(Keyboard::is_pressed(DOKUN_KEY_3)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_4)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_5)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_6)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_7)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_8)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_9)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_0)) ;

            if(Keyboard::is_pressed(DOKUN_KEY_W)) camera->set_position( camera->get_position().x, camera->get_position().y - 1, camera->get_position().z );
            if(Keyboard::is_pressed(DOKUN_KEY_A)) camera->set_position( camera->get_position().x - 1, camera->get_position().y, camera->get_position().z );
            if(Keyboard::is_pressed(DOKUN_KEY_S)) camera->set_position( camera->get_position().x, camera->get_position().y + 1, camera->get_position().z );
            if(Keyboard::is_pressed(DOKUN_KEY_D)) camera->set_position( camera->get_position().x + 1, camera->get_position().y, camera->get_position().z );
            
            if(Keyboard::is_pressed(DOKUN_KEY_Q)) camera->set_position( camera->get_position().x, camera->get_position().y, camera->get_position().z - 1);
            if(Keyboard::is_pressed(DOKUN_KEY_E)) camera->set_position( camera->get_position().x, camera->get_position().y, camera->get_position().z + 1);

            if(Keyboard::is_pressed(DOKUN_KEY_Z)) camera->rotate(2, 0, 0, 1); // rotate camera z-coord     - normal rotation for 2d graphics
            if(Keyboard::is_pressed(DOKUN_KEY_X)) sprite->rotate(90 * (clock() / (double)CLOCKS_PER_SEC));          // rotate object
            if(Keyboard::is_pressed(DOKUN_KEY_C)) ;
            if(Keyboard::is_pressed(DOKUN_KEY_V)) ;
            //if(Keyboard::is_pressed(DOKUN_KEY_)) ;
            // Mouse
            if(Mouse::is_pressed(1)) ;//label->set_string("Mouse pressed 1");//std::cout << "Mouse button LEFT is pressed" << std::endl;
            if(Mouse::is_pressed(2)) ;//label->set_string("Mouse pressed 2");
            if(Mouse::is_pressed(3)) ;//label->set_string("Mouse pressed 3");
            if(Mouse::is_over(label->get_position(), label->get_size())){label->set_color(160, 160, 160, 255);}else label->set_color(255, 255, 255, 255);
		} else music->pause();
		//  draw here
        sprite->draw();
        //std::cout<<"Sprite position: " << sprite->get_position() << std::endl;std::cout<<"Camera position: " << Renderer::get_camera()->get_position() << std::endl;
        // draw multiple sprites
        for(int i = 0; i < sprite_array.size(); i++) {
            sprite_array[i]->draw();
            if(Sprite::collide(*sprite, *sprite_array[i])) label->set_string("Sprite " + String(sprite).str() + " has collided with another Sprite " + String(sprite_array[i]).str()); //std::cout << "Sprite " << String(sprite) << " has collided with another Sprite " << String(sprite_array[i]) << "!\n";
        }
        //model->draw();
        widget->draw();
        label->draw();
		// update window
		window.update();
	}
	window.destroy();       // destroy window
	//====================
	if(!window.is_open()) { // window is closed
		Engine::close(L);   // close engine too
	}
	/*
	Matrix4 m = Matrix4(0); // new matrix allocated with all values set to 0
	m = Matrix4::translate(m, 0, 0, -1.0);
	m = Matrix4::rotate(m, 0 * 0.0174533, 0, 0, 1);
	m = Matrix4::scale(m, 1, 1, 0.0);
	m = Matrix4::translate(m, -0, -0, 1.0);
	//m = Matrix4::ortho(0.0f, 1280, 720, 0.0f, -1.0f, 1.0f);
	//m = Matrix4::perspective(67.5f, static_cast<float>(1280 / 720), 1.0f, 10024.0f);
	m = Matrix4::look_at(0, 0, 0,   0, 0,-1,   0, 1, 0);*/
	return 0;
}
////////////	
#ifdef __cplusplus // if c++
extern "C" {
#endif	
int Engine::regstr(lua_State *L)
{	
    static const luaL_Reg module[] = 
    {
		{"start" , Engine::open  },
	    {"close" , Engine::close },
		{"test"  , Engine::test  },
        {nullptr , nullptr       },
    };	
	
    // -----------------------------------------------------------
    Script::global(L, "_ENGINE", "Dokun");
	Script::global(L, "DOKUN_VERSION", std::string(ENGINE_VERSION));
	Script::global(L, "DOKUN_VERSION_MAJOR", std::string(ENGINE_VERSION_MAJOR));
	Script::global(L, "DOKUN_VERSION_MINOR", std::string(ENGINE_VERSION_MINOR));
	Script::global(L, "DOKUN_VERSION_PATCH", std::string(ENGINE_VERSION_PATCH));
	Script::global(L, "DOKUN_STATUS", (Engine::get_status() != 0));
	Script::global(L, "DOKUN_PLATFORM",         dokun::platform());
	Script::global(L, "set_current_API", Renderer::set_current_API);
	Script::global(L, "lua_error_test", Logger::lua_error_test    );
	// window -----------------------------------------------------------
	Script::table   (L, "Window");
	Script::function(L, "Window", "new"   ,  WINDOW::window_new );
	Script::function(L, "Window", "create",  WINDOW::create );
	Script::function(L, "Window", "show"  ,  WINDOW::show   );
	Script::function(L, "Window", "update",  WINDOW::update );	
	Script::function(L, "Window", "clear" ,  WINDOW::clear  );
	Script::function(L, "Window", "destroy", WINDOW::destroy);
	Script::function(L, "Window", "iconify", WINDOW::iconify);		
	Script::function(L, "Window", "maximize", WINDOW::maximize);		
	Script::function(L, "Window", "restore", WINDOW::restore);				
	Script::function(L, "Window", "loop", WINDOW::loop); 
	Script::function(L, "Window", "set_title", WINDOW::set_title);
	Script::function(L, "Window", "set_width", WINDOW::set_width);
	Script::function(L, "Window", "set_height", WINDOW::set_height);
	Script::function(L, "Window", "set_size", WINDOW::set_size);	
	Script::function(L, "Window", "set_mode", WINDOW::set_mode);
	Script::function(L, "Window", "set_position", WINDOW::set_position);
	Script::function(L, "Window", "set_style"  , WINDOW::set_style); // sets the window's style
	Script::function(L, "Window", "set_icon"   , WINDOW::set_icon);  // sets the window's icon
	Script::function(L, "Window", "set_cursor" , WINDOW::set_cursor); // sets the window cursor			
	//Script::function (L, "Window", "set_event"  , WINDOW::set_event);	// sets the how messages are retrieved		
	Script::function(L, "Window", "set_context", WINDOW::set_context); // set the current context
	Script::function(L, "Window", "set_vsync", WINDOW::set_vertical_synchronization); // set vertical synchronization	
	Script::function(L, "Window", "set_viewport", WINDOW::set_viewport);		
	Script::function(L, "Window", "get_title", WINDOW::get_title);
	Script::function(L, "Window", "get_width", WINDOW::get_width);
	Script::function(L, "Window", "get_height", WINDOW::get_height);
	Script::function(L, "Window", "get_size", WINDOW::get_size);
	Script::function(L, "Window", "get_client_width", WINDOW::get_client_width);
	Script::function(L, "Window", "get_client_height", WINDOW::get_client_height);
	Script::function(L, "Window", "get_client_size", WINDOW::get_client_size);
	Script::function(L, "Window", "get_mode", WINDOW::get_mode);
	Script::function(L, "Window", "get_x", WINDOW::get_x);
	Script::function(L, "Window", "get_y", WINDOW::get_y);
	Script::function(L, "Window", "get_position", WINDOW::get_position);
	Script::function(L, "Window", "is_open", WINDOW::is_open);  
	Script::function(L, "Window", "is_visible", WINDOW::is_visible);	
	Script::function(L, "Window", "is_focused", WINDOW::is_focused);
	Script::function(L, "Window", "is_iconified", WINDOW::is_iconified);
	Script::function(L, "Window", "is_context", WINDOW::is_context);		
	#ifdef __gnu_linux__
	    Script::function (L, "Window", "get_display", WINDOW::get_display);
	#endif
	// sprite -----------------------------------------------------------
	Script::table   (L, "Sprite");
	Script::function(L, "Sprite", "new"         , Sprite::sprite_new  );
	Script::function(L, "Sprite", "destroy"     , Sprite::destroy     );		
	Script::function(L, "Sprite", "load"        , Sprite::load        );
	Script::function(L, "Sprite", "draw"        , Sprite::draw        );
	Script::function(L, "Sprite", "translate"   , Sprite::translate   );
	Script::function(L, "Sprite", "rotate"      , Sprite::rotate      );
	Script::function(L, "Sprite", "scale"       , Sprite::scale       );
	Script::function(L, "Sprite", "shear"       , Sprite::shear       );		
	Script::function(L, "Sprite", "reflect"     , Sprite::reflect     );
	Script::function(L, "Sprite", "resize"      , Sprite::resize      );
	Script::function(L, "Sprite", "lock"        , Sprite::lock        );
	Script::function(L, "Sprite", "lock_all"    , Sprite::lock_all    );
	Script::function(L, "Sprite", "set_position", Sprite::set_position);
	Script::function(L, "Sprite", "set_angle"   , Sprite::set_angle   );
	Script::function(L, "Sprite", "set_scale"   , Sprite::set_scale   );		
	Script::function(L, "Sprite", "set_texture" , Sprite::set_texture );
	Script::function(L, "Sprite", "set_size"    , Sprite::set_size    );	
	Script::function(L, "Sprite", "set_color"   , Sprite::set_color   );			
	Script::function(L, "Sprite", "get_position", Sprite::get_position);
	Script::function(L, "Sprite", "get_angle"   , Sprite::get_angle   );
	Script::function(L, "Sprite", "get_scale"   , Sprite::get_scale   );		
	Script::function(L, "Sprite", "get_texture" , Sprite::get_texture );
	Script::function(L, "Sprite", "get_width"   , Sprite::get_width   );
	Script::function(L, "Sprite", "get_height"  , Sprite::get_height  );
	Script::function(L, "Sprite", "get_size"    , Sprite::get_size    ); // pixel size in width, height	
	Script::function(L, "Sprite", "get_color"   , Sprite::get_color   );		
	Script::function(L, "Sprite", "is_sprite"   , Sprite::is_sprite   );
    Script::function(L, "Sprite", "collide"     , Sprite::collide     );		
    //Script::function(L, "Sprite", ""        , Sprite::);	
	// model -----------------------------------------------------------
	Script::table   (L, "Model"                                     );  // can load only Wavefront .OBJ
	Script::function(L, "Model", "new"         , Model::create      );
	Script::function(L, "Model", "destroy"     , Model::destroy     );		
	Script::function(L, "Model", "load"        , Model::load        );
	Script::function(L, "Model", "draw"        , Model::draw        );
	Script::function(L, "Model", "translate"   , Model::translate   );
	Script::function(L, "Model", "rotate"      , Model::rotate      );
	Script::function(L, "Model", "scale"       , Model::scale       );
	Script::function(L, "Model", "shear"       , Model::shear       );
	Script::function(L, "Model", "reflect"     , Model::reflect     );		
	Script::function(L, "Model", "set_position", Model::set_position);
	Script::function(L, "Model", "set_rotation", Model::set_rotation);
	Script::function(L, "Model", "set_scale"   , Model::set_scale   );		
	Script::function(L, "Model", "set_texture" , Model::set_texture );			
	Script::function(L, "Model", "set_material", Model::set_material);	
	Script::function(L, "Model", "get_position", Model::get_position);
	Script::function(L, "Model", "get_rotation", Model::get_rotation);	
	Script::function(L, "Model", "get_scale"   , Model::get_scale   );			
	Script::function(L, "Model", "get_texture" , Model::get_texture );		
	Script::function(L, "Model", "get_material", Model::get_material);		
	Script::function(L, "Model", "get_size"    , Model::get_size    );		
	Script::function(L, "Model", "is_model"    , Model::is_model    );
	//Script::function(L, "", ""        , ::);
	// Vertex -----------------------------------------------------------
	Script::table   (L, "Vertex");
    Script::function(L, "Vertex", "new", Vertex::new_);	
	Script::function(L, "Vertex", "draw", Vertex::draw);	
	Script::function(L, "Vertex", "draw_object", Vertex::draw_object);	
	Script::function(L, "Vertex", "add", Vertex::add);	
	Script::function(L, "Vertex", "set_position", Vertex::set_position);	
	Script::function(L, "Vertex","get_position", Vertex::get_position);	
	Script::function(L, "Vertex", "get_size", Vertex::size);		
	Script::function(L, "Vertex", "", 0);		
	//Script::function(L, "", ""        , ::);
	// texture -----------------------------------------------------------
	Script::table   (L, "Texture"                                    );
	Script::function(L, "Texture", "new"       , Texture::texture_new);	
	Script::function(L, "Texture", "load"      , Texture::load       );
	Script::function(L, "Texture", "copy"      , Texture::copy       );
	Script::function(L, "Texture", "generate"        , Texture::generate);
    Script::function(L, "Texture", "bind"        , Texture::bind);
	Script::function(L, "Texture", "unbind"        , Texture::unbind);	
	Script::function(L, "Texture", "destroy"        , Texture::destroy);
	Script::function(L, "Texture", "set_rect"        , Texture::set_rect);
	Script::function(L, "Texture", "set_filter"        , Texture::set_filter);
	Script::function(L, "Texture", "set_wrap"        , Texture::set_wrap);
	Script::function(L, "Texture", "set_internal_format"        , Texture::set_internal_format);
	Script::function(L, "Texture", "set_format"        , Texture::set_format);
	Script::function(L, "Texture", "set_param" , Texture::set_param  );
	//Script::function(L, "Texture", "set_"        , Texture::set_);
	Script::function(L, "Texture", "set_type"        , Texture::set_type);
	Script::function(L, "Texture", "get_width" , Texture::get_width  );
	Script::function(L, "Texture", "get_height", Texture::get_height );
	Script::function(L, "Texture", "get_size"  , Texture::get_size   );
	Script::function(L, "Texture", "get_data"  , Texture::get_data   );
	Script::function(L, "Texture", "get_file"  , Texture::get_file   );	
	Script::function(L, "Texture", "get_buffer", Texture::get_buffer );
    Script::function(L, "Texture", "get_filter"         , Texture::get_filter);	
    Script::function(L, "Texture", "get_wrap"           , Texture::get_wrap);	
    Script::function(L, "Texture", "get_internal_format", Texture::get_internal_format);	
    Script::function(L, "Texture", "get_format"         , Texture::get_format);	
	Script::function(L, "Texture", "get_info"  , Texture::get_info   );	
	Script::function(L, "Texture", "is_texture", Texture::is_texture ); // returns true if texture data is valid(texture has been loaded without error)	
    Script::function(L, "Texture", "is_generated", Texture::is_generated);			
	Script::function(L, "Texture", "", 0);
    //Script::function(L, "Texture", ""        , ::);	
	// image -----------------------------------------------------------
	Script::table   (L, "Image"                         ); // can load multiple image formats unlike texture
	Script::function(L, "Image", "new", Image::new_     );
	Script::function(L, "Image", "load", Image::load    );
	Script::function(L, "Image", "draw", Image::draw    );
	Script::function(L, "Image", "copy", Image::copy    );			
	Script::function(L, "Image", "copy_texture", Image::copy_texture    );			
	Script::function(L, "Image", "save", Image::save    );
	Script::function(L, "Image", "flip", Image::flip    );		
	Script::function(L, "Image", "resize", Image::resize);
    //Script::function(L, "Image", "scale_to_fit", Image::scale_to_fit);
    //Script::function(L, "Image", "generate"        , Image::generate);
	Script::function (L, "Image", "destroy", Image::destroy);
	Script::function(L, "Image", "translate", 0);
	Script::function(L, "Image", "rotate", 0);
	Script::function(L, "Image", "scale", 0);
	Script::function(L, "Image", "shear", 0);		
    Script::function(L, "Image", "set_x", Image::set_x);
    Script::function(L, "Image", "set_y", Image::set_y);
	Script::function(L, "Image", "set_position", Image::set_position);
    Script::function(L, "Image", "set_relative_position", Image::set_relative_position);
	Script::function(L, "Image", "set_angle", Image::set_angle);		
	Script::function(L, "Image", "set_scale", Image::set_scale);
    Script::function(L, "Image", "set_color", Image::set_color);
    Script::function(L, "Image", "set_alignment", Image::set_alignment);// gui-related
    Script::function(L, "Image", "set_visible", Image::set_visible);
    Script::function(L, "Image", "set_filter", Image::set_filter); // opengl-related
    Script::function(L, "Image", "set_wrap", Image::set_wrap);
    Script::function(L, "Image", "set_internal_format", Image::set_internal_format);
    Script::function(L, "Image", "set_format", Image::set_format);
    Script::function(L, "Image", "set_param", Image::set_param);
    //Script::function(L, "Image", "set_", Image::set_);
	Script::function(L, "Image", "get_width", Image::get_width);
	Script::function(L, "Image", "get_height", Image::get_height);
	Script::function(L, "Image", "get_depth", Image::get_depth);			
	Script::function(L, "Image", "get_size", Image::get_size);
	Script::function(L, "Image", "get_data", Image::get_data); // pixel data			
	Script::function(L, "Image", "get_file", Image::get_file);
	Script::function(L, "Image", "get_channel", Image::get_channel);
	Script::function(L, "Image", "get_format", 0);		
    Script::function(L, "Image", "get_color", Image::get_color);	   
    Script::function(L, "Image", "get_x", Image::get_x);	   
    Script::function(L, "Image", "get_y", Image::get_y);	   
    Script::function(L, "Image", "get_position", Image::get_position);	  
    Script::function(L, "Image", "get_relative_x", Image::get_relative_x);
    Script::function(L, "Image", "get_relative_y", Image::get_relative_y);
    Script::function(L, "Image", "get_relative_position", Image::get_relative_position); 
    Script::function(L, "Image", "get_angle", Image::get_angle);	   
    Script::function(L, "Image", "get_scale", Image::get_scale);	   
    Script::function(L, "Image", "get_aspect_ratio_correction", Image::get_aspect_ratio_correction);
    Script::function(L, "Image", "get_alignment", Image::get_alignment);
    Script::function(L, "Image", "get_rect", Image::get_rect);
    Script::function(L, "Image", "get_buffer", Image::get_buffer); // opengl-related
    Script::function(L, "Image", "get_filter", Image::get_filter);
    Script::function(L, "Image", "get_wrap", Image::get_wrap);
    Script::function(L, "Image", "get_internal_format", Image::get_internal_format);
    Script::function(L, "Image", "get_format", Image::get_format);	
    //Script::function(L, "Image", "get_", Image::get_);   
	Script::function(L, "Image", "is_png", Image::is_png);
	Script::function(L, "Image", "is_jpeg", 0);		
	Script::function(L, "Image", "is_bmp", 0);
	Script::function(L, "Image", "is_tga", 0);		
	Script::function(L, "Image", "is_gif", 0);    // Image:: 
	Script::function(L, "Image", "is_tiff", 0);			
	Script::function(L, "Image", "is_image", Image::is_image);
	//Script::function(L, "", ""        , ::);
    // mouse -----------------------------------------------------------
	Script::table   (L, "Mouse"                                     );
    Script::function(L, "Mouse", "show"        , Mouse::show        ); // make mouse visible
	Script::function(L, "Mouse", "hide"        , Mouse::hide        ); // make mouse invisible
	Script::function(L, "Mouse", "set_position", Mouse::set_position); // set mouse position
	Script::function(L, "Mouse", "set_cursor", Mouse::set_cursor); // set mouse position
	Script::function(L, "Mouse", "get_position", Mouse::get_position); // get mouse position
	Script::function(L, "Mouse",  "get_delta"  , Mouse::get_delta   ); // get mouse wheel delta		
	Script::function(L, "Mouse", "get_color"   , Mouse::get_color   ); // get pixel at a specific coordinate	
	Script::function(L, "Mouse", "is_moved"    , Mouse::is_moved    ); // checks if mouse has moved
	Script::function(L, "Mouse", "is_scrolled" , Mouse::is_scrolled );	// checks if mouse is being scrolled
	Script::function(L, "Mouse", "is_pressed"  , Mouse::is_pressed  ); // specific mouse button pressed
	Script::function(L, "Mouse", "is_released" , Mouse::is_released ); 
	Script::function(L, "Mouse", "is_down"     , Mouse::is_down     ); // any mouse button pressed
	Script::function(L, "Mouse", "is_up"       , Mouse::is_up       ); 	
	Script::function(L, "Mouse", "is_over"     , Mouse::is_over     );
    //Script::function(L, "", ""        , ::);	
	// keyboard -----------------------------------------------------------
	Script::table   (L, "Keyboard"                                      );
	Script::function(L, "Keyboard", "is_pressed" , Keyboard::is_pressed );// specific key is pressed
	Script::function(L, "Keyboard", "is_released", Keyboard::is_released);// specific key is released
	Script::function(L, "Keyboard", "is_down"    , Keyboard::is_down    );// any key is pressed 
	Script::function(L, "Keyboard", "is_up"      , Keyboard::is_up      ); // any key is released
	//Script::function(L, "", ""        , ::);
	// -----------------------------------------------------------
	Script::table(L, "Joystick");
	//Script::function(L, "", ""        , ::);
	Script::table(L, "Touch"   ); // Touch pad stuff
	//Script::function(L, "", ""        , ::);
	Script::table(L, "Virtual" );
	//Script::function(L, "", ""        , ::);
	Script::table(L, "Brain"   );
	//Script::function(L, "", ""        , ::);
	// audio -----------------------------------------------------------
	Script::table   (L, "Audio");
	Script::function(L, "Audio", "play",   Audio::play);
	Script::function(L, "Audio", "pause", Audio::pause);
	Script::function(L, "Audio", "stop", Audio::stop);
	Script::function(L, "Audio", "set_volume", Audio::set_volume);
	Script::function(L, "Audio", "set_loop", Audio::set_loop);		
	Script::function(L, "Audio", "set_pitch", Audio::set_pitch);
	Script::function(L, "Audio", "set_position", Audio::set_position);		
	Script::function(L, "Audio", "get_volume", Audio::get_volume);
	Script::function(L, "Audio", "get_pitch", Audio::get_pitch);		
	Script::function(L, "Audio", "get_duration", Audio::get_duration);
	Script::function(L, "Audio", "get_position", Audio::get_position);
	Script::function(L, "Audio", "get_data", Audio::get_data);		
	Script::function(L, "Audio", "is_playing", Audio::is_playing);
	Script::function(L, "Audio", "is_paused", Audio::is_paused);		
	Script::function(L, "Audio", "is_stopped", Audio::is_stopped);
	Script::function(L, "Audio", "is_looped", Audio::is_looped);
	//Script::function(L, "", ""        , ::);
	// sound -----------------------------------------------------------
	Script::table   (L, "Sound");
	Script::function(L, "Sound", "new", Sound::new_);
	Script::function(L, "Sound", "load", Sound::load);
	//Script::function(L, "", ""        , ::);
	// music -----------------------------------------------------------
	Script::table   (L, "Music");
	Script::function(L, "Music", "new",  Music::new_);
	Script::function(L, "Music", "load", Music::load);		
	Script::function(L, "Music", "play", Music::play);	
    // sound, music inherit audio metatable (and functions) 		
    Script::inherit (L, "Audio", "Sound");
    Script::inherit (L, "Audio", "Music");	
	//Script::function(L, "", ""        , ::);
	// voice -----------------------------------------------------------
	Script::table   (L, "Voice");
	Script::function(L, "Voice", "new", Voice::new_);	
	Script::function(L, "Voice", "start", Voice::start);
	Script::function(L, "Voice", "stop", Voice::stop);		
	Script::function(L, "Voice", "play", Voice::play);
	Script::function(L, "Voice", "pause", Voice::pause);		
	Script::function(L, "Voice", "save", Voice::save);
	Script::function(L, "Voice","set_playback_volume", 0);
	Script::function(L, "Voice", "set_microphone_volume", 0);		
	Script::function(L, "Voice","get_data", Voice::get_data);
	Script::function(L, "Voice","is_recording", Voice::is_recording);	
	Script::function(L, "Voice","is_playing", Voice::is_playing);
	Script::function(L, "Voice","is_paused", Voice::is_paused);
	Script::function(L, "Voice","is_stopped", Voice::is_stopped);
	//Script::function(L, "", ""        , ::);
	// ui -----------------------------------------------------------
	Script::table   (L, "GUI");
	Script::function(L, "GUI","destroy", GUI::destroy);			
	Script::function(L, "GUI","show", GUI::show);
	Script::function(L, "GUI","hide", GUI::hide);
	Script::function(L, "GUI","show_all", GUI::show_all);
	Script::function(L, "GUI", "hide_all", GUI::hide_all);	
	Script::function(L, "GUI","set_width", GUI::set_width);		
	Script::function(L, "GUI","set_height", GUI::set_height);		
	Script::function(L, "GUI","set_size", GUI::set_size);
	Script::function(L, "GUI","set_position", GUI::set_position);
	Script::function(L, "GUI", "set_relative_position", GUI::set_relative_position);
    Script::function(L, "GUI", "set_scale"        , GUI::set_scale);
    Script::function(L, "GUI", "set_angle"        , GUI::set_angle);		
	Script::function(L, "GUI","set_orientation", GUI::set_orientation);
	Script::function(L, "GUI","set_parent", GUI::set_parent);		
	Script::function(L, "GUI","set_visible", GUI::set_visible);
	Script::function(L, "GUI","set_active", GUI::set_active);
	Script::function(L, "GUI","set_draggable", GUI::set_draggable);
	Script::function(L, "GUI","set_droppable", GUI::set_droppable);
	Script::function(L, "GUI","set_resizeable", GUI::set_resizeable);
	Script::function(L, "GUI","set_sortable", GUI::set_sortable);		
	Script::function(L, "GUI","get_width", GUI::get_width);
	Script::function(L, "GUI","get_height", GUI::get_height);
	Script::function(L, "GUI","get_size", GUI::get_size);
	Script::function(L, "GUI", "get_rect", GUI::get_rect);
	Script::function(L, "GUI","get_x", GUI::get_x);
	Script::function(L, "GUI","get_y", GUI::get_y);
	Script::function(L, "GUI", "get_position", GUI::get_position);
    Script::function(L, "GUI", "get_relative_position", GUI::get_relative_position);
    Script::function(L, "GUI", "get_scale"        , GUI::get_scale);
    Script::function(L, "GUI", "get_angle"        , GUI::get_angle);
	Script::function(L, "GUI","get_orientation", GUI::get_orientation);
	Script::function(L, "GUI","get_parent", GUI::get_parent);
	Script::function(L, "GUI","is_visible", GUI::is_visible);
	Script::function(L, "GUI","is_active", GUI::is_active); 
	Script::function(L, "GUI", "is_draggable", GUI::is_draggable);
	Script::function(L, "GUI","is_droppable", GUI::is_droppable);
	Script::function(L, "GUI","is_resizeable", GUI::is_resizeable);
	Script::function(L, "GUI", "is_sortable", GUI::is_sortable);
	Script::function(L, "GUI", "is_hovered" , GUI::is_hovered);
	Script::function(L, "GUI", "is_pressed" , GUI::is_pressed);
	Script::function(L, "GUI", "is_selected", 0);
	Script::function(L, "GUI", "is_released", 0);		
	Script::function(L, "GUI", "is_focused" , 0);
	Script::function(L, "GUI", "is_dragged" , 0);		
	Script::function(L, "GUI", "is_dropped" , 0);
	Script::function(L, "GUI", "is_sorted"  , 0);
	Script::function(L, "GUI", "is_gui"     , GUI::is_gui);	
	Script::function(L, "GUI", "has_parent" , GUI::has_parent);
	Script::function(L, "GUI", "is_child_of", GUI::is_child_of);
	Script::function(L, "GUI", "is_parent_of", GUI::is_parent_of);
	//Script::function(L, "GUI", ""        , GUI::);
	// widget -----------------------------------------------------------
    Script::table   (L, "Widget"          ); 
	Script::attach  (L, "GUI"   , "Widget"); // attach Widget to GUI : GUI.Widget 
	Script::inherit (L, "GUI"   , "Widget"); 
	Script::function(L, "Widget", "new" , Widget::widget_new);
	Script::function(L, "Widget", "draw", Widget::draw      );
	Script::function(L, "Widget", "set_color", Widget::set_color);
	//Script::function(L, "Widget", "set_alpha", Widget::set_alpha);
	//Script::function(L, "Widget", "set_fill", Widget::set_fill);
	Script::function(L, "Widget", "set_outline", Widget::set_outline);
	Script::function(L, "Widget", "set_outline_width", Widget::set_outline_width);
	Script::function(L, "Widget", "set_outline_color", Widget::set_outline_color);
	//Script::function(L, "Widget", "set_outline_style", Widget::set_outline_style);
	//Script::function(L, "Widget", "set_outline_antialiased", Widget::set_outline_antialiased);
	//Script::function(L, "Widget", "set_border", Widget::set_border);
	//Script::function(L, "Widget", "set_border_size", Widget::set_border_size);
	//Script::function(L, "Widget", "set_border_width", Widget::set_border_width);
	//Script::function(L, "Widget", "set_border_color", Widget::set_border_color);
	//Script::function(L, "Widget", "set_border_style", Widget::set_border_style);
	//Script::function(L, "Widget", "set_border_radius", Widget::set_border_radius);
	//Script::function(L, "Widget", "set_radius", Widget::set_radius);
	//Script::function(L, "Widget", "set_trim", Widget::set_trim);
	Script::function(L, "Widget", "set_gradient", Widget::set_gradient);
	//Script::function(L, "Widget", "set_gradient_color", Widget::set_gradient_color);
	Script::function(L, "Widget", "set_title_bar", Widget::set_title_bar);
	Script::function(L, "Widget", "set_title_bar_size", Widget::set_title_bar_size);
	Script::function(L, "Widget", "set_title_bar_text", Widget::set_title_bar_text);
	Script::function(L, "Widget", "set_title_bar_text_color", Widget::set_title_bar_text_color);
	Script::function(L, "Widget", "set_title_bar_icon", Widget::set_title_bar_icon);
	Script::function(L, "Widget", "set_title_bar_color", Widget::set_title_bar_color);
	Script::function(L, "Widget", "set_title_bar_button_iconify", Widget::set_title_bar_button_iconify);
	Script::function(L, "Widget", "set_title_bar_button_maximize", Widget::set_title_bar_button_maximize);
	Script::function(L, "Widget", "set_title_bar_button_close", Widget::set_title_bar_button_close);
	//Script::function(L, "Widget", "set_as_icon", Widget::set_as_icon);
	Script::function(L, "Widget", "set_image", Widget::set_image);
	Script::function(L, "Widget", "set_label", Widget::set_label);
	Script::function(L, "Widget", "set_text", Widget::set_text);
	//Script::function(L, "Widget", "", Widget::);
	//Script::function(L, "Widget", "", Widget::);
	Script::function(L, "Widget", "get_image", Widget::get_image);
	Script::function(L, "Widget", "get_label", Widget::get_label);
	//Script::function(L, "Widget", "get_text", Widget::get_text);
	//Script::function(L, "Widget", "", Widget::);
	//Script::function(L, "Widget", "", Widget::);
	//Script::function(L, "Widget", "", Widget::);
	//Script::function(L, "Widget", "", Widget::);
    Script::execute(L, "Box = Widget"); // assign Box to Widget as another name for Widget (You can also use Box in place of Widget)
	// button -----------------------------------------------------------
	Script::table   (L, "Button"); // Button = {}  Button_mt = {__index = Button }
	Script::attach  (L, "GUI", "Button"); // GUI.Button = Button
	Script::inherit (L, "GUI", "Button"); // setmetatable(Button, GUI_mt)
	Script::function(L, "Button", "new", Button::new_ ); // default, radio, check_box
	Script::function(L, "Button", "draw", Button::draw);
	Script::function(L, "Button", "", 0);
	Script::function(L, "Button", "", 0);
	Script::function(L, "Button", "set_color", Button::set_color);		
	Script::function(L, "Button", "set_text", Button::set_text  ); // generates Label		
	Script::function(L, "Button", "set_label", Button::set_label);
	Script::function(L, "Button", "set_image", Button::set_image);
	Script::function(L, "Button", "get_color", Button::get_color);
	Script::function(L, "Button", "get_text", Button::get_text  ); // shortcut to button:get_label():get_string()
	Script::function(L, "Button", "get_label", Button::get_label); 
	Script::function(L, "Button", "get_image", Button::get_image);
	//Script::function(L, "", ""        , ::);
	// label -----------------------------------------------------------
	Script::table   (L, "Label"                                 ); // create a table Label with a Label_mt metatable
	Script::attach  (L, "GUI"  , "Label"                        ); // attach Label to GUI : GUI.Label
	Script::inherit (L, "GUI"  , "Label"                        ); // inherit from GUI (Label inherits GUI functions)
	Script::function(L, "Label", "new"       , Label::new_      );
	Script::function(L, "Label", "draw"      , Label::draw      );
	Script::function(L, "Label", "set_font"  , Label::set_font  );	
	Script::function(L, "Label", "set_string", Label::set_string);
	Script::function(L, "Label", "set_color" , Label::set_color );
    Script::function(L, "Label", "set_style", Label::set_style);
    Script::function(L, "Label", "set_background_color", Label::set_background_color);
    Script::function(L, "Label", "set_alignment", Label::set_alignment); // test
    //Script::function(L, "Label", "set_", Label::set_);
    // getters	
	Script::function(L, "Label", "get_font"  , Label::get_font  );
	Script::function(L, "Label", "get_string", Label::get_string);
    Script::function(L, "Label", "get_color", Label::get_color  );
    Script::function(L, "Label", "get_style", Label::get_style  );
    Script::function(L, "Label", "get_background_color", Label::get_background_color);
    Script::function(L, "Label", "get_alignment", Label::get_alignment); // test
    //Script::function(L, "Label", "get_", Label::get_);
    //Script::function(L, "Label", "get_width", Label::get_width  ); // function crashes in Lua
    //Script::function(L, "Label", "get_height", Label::get_height);
    //Script::function(L, "Label", "get_", Label::get_);
	// overridden functioms
	Script::function(L, "Label", "set_size"        , Label::set_size);
    //Script::function(L, "Label", ""        , Label::);	
	// edit -----------------------------------------------------------
	Script::table   (L, "Edit"                    );
	Script::function(L, "Edit", "new" , Edit::new_);
    Script::function(L, "Edit", "draw", Edit::draw);
    //Script::function(L, "", ""        , ::);	
	// progressbar -----------------------------------------------------------
	Script::table   (L, "Progressbar");
	Script::inherit (L, "GUI", "Progressbar");
	Script::function(L, "Progressbar", "new", Progressbar::new_);
	Script::function(L, "Progressbar", "draw", Progressbar::draw);
	Script::function(L, "Progressbar", "set_text", Progressbar::set_text);
	Script::function(L, "Progressbar", "set_label", Progressbar::set_label);
	Script::function(L, "Progressbar", "set_foreground_color", Progressbar::set_foreground_color);
	Script::function(L, "Progressbar", "set_background_color", Progressbar::set_background_color);
	Script::function(L, "Progressbar", "set_range", Progressbar::set_range);
	Script::function(L, "Progressbar", "set_minimum_value", Progressbar::set_minimum_value);
	Script::function(L, "Progressbar", "set_maximum_value", Progressbar::set_maximum_value);
	Script::function(L, "Progressbar", "set_value", Progressbar::set_value);
	Script::function(L, "Progressbar", "reset", Progressbar::reset);
	Script::function(L, "Progressbar", "set_outline", Progressbar::set_outline);
	Script::function(L, "Progressbar", "set_outline_width", Progressbar::set_outline_width);
	Script::function(L, "Progressbar", "set_outline_color", Progressbar::set_outline_color);
	Script::function(L, "Progressbar", "set_outline_antialiased", Progressbar::set_outline_antialiased);
	//Script::function(L, "Progressbar", "set_", Progressbar::set_);
	//Script::function(L, "Progressbar", "", Progressbar::);	
	Script::function(L, "Progressbar", "get_text"         , Progressbar::get_text                  );
	Script::function(L, "Progressbar", "get_label"        , Progressbar::get_label                 );
	Script::function(L, "Progressbar", "get_color"        , Progressbar::get_color                 );
    Script::function(L, "Progressbar", "get_foreground_color", Progressbar::get_foreground_color   );
    Script::function(L, "Progressbar", "get_background_color", Progressbar::get_background_color   );
	Script::function(L, "Progressbar", "get_range"        , Progressbar::get_range                 );
	Script::function(L, "Progressbar", "get_minimum_value", Progressbar::get_minimum_value         );
	Script::function(L, "Progressbar", "get_maximum_value", Progressbar::get_maximum_value         );
	Script::function(L, "Progressbar", "get_value"        , Progressbar::get_value                 );
	//Script::function(L, "Progressbar", "get_", Progressbar::get_);
	//Script::function(L, "Progressbar", "", Progressbar::);
	///Script::function(L, "Progressbar", "", Progressbar::);
	//Script::function(L, "Progressbar", "", Progressbar::);
	//Script::function(L, "Progressbar", "", Progressbar::);
	//Script::function(L, "Progressbar", "", Progressbar::);
	// slider -----------------------------------------------------------
	Script::table   (L, "Slider");
	//Script::function(L, "Slider", ""        , Slider::);
	//Script::inherit (L, "GUI", "");
	// toggle -----------------------------------------------------------
	Script::table   (L, "Toggle");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "Toggle", ""        , ::);
	// scrollbar -----------------------------------------------------------
	Script::table   (L, "Scrollbar");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// list -----------------------------------------------------------
	Script::table   (L, "List");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// spinner -----------------------------------------------------------
	Script::table   (L, "Spinner");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// menubar -----------------------------------------------------------
	Script::table   (L, "Menubar");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// tooltip -----------------------------------------------------------
	Script::table   (L, "Tooltip");
	//Script::inherit (L, "GUI", "");
	//Script::function(L, "", ""        , ::);
	// grid -----------------------------------------------------------
	Script::table   (L, "Grid");
	Script::inherit (L, "GUI", "Grid");
	Script::function(L, "Grid", "new"       , Grid::grid_new);
	Script::function(L, "Grid", "draw"      , Grid::draw);
	Script::function(L, "Grid", "update"    , Grid::update);
	Script::function(L, "Grid", "set_row"   , Grid::set_row);
	Script::function(L, "Grid", "set_column", Grid::set_column);
	//Script::function(L, "Grid", ""        , Grid::);
	// font -----------------------------------------------------------
	Script::table   (L, "Font"                          );
	Script::function(L, "Font", "new"    , FONT::create );
	Script::function(L, "Font", "load"   , FONT::load   );
	Script::function(L, "Font", "destroy", FONT::destroy);
	Script::function(L, "Font", "get_size", FONT::get_size);
	Script::function(L, "Font", "get_rect", FONT::get_rect);
	Script::function(L, "Font", "get_data", FONT::get_data);
	//Script::function(L, "Font", ""        , FONT::);
	// network -----------------------------------------------------------
	//Script::global (L, "socket", Socket::new_); // returns a socket
	//Script::table (L, "Network"); // Network_mt
	//Script::function(L, "Network", "connect", Socket::connect);
	//Script::function(L, "Network", "read", Socket::read); // receives data
	//Script::function(L, "Network", "write", Socket::write); // sends data
	//Script::function(L, "Network", "bind", 0);
	//Script::function(L, "", ""        , ::);
    // entity -----------------------------------------------------------
    Script::table   (L, "Entity"                                           );
	Script::function(L, "Entity", "new"             , Entity::create       );
	Script::function(L, "Entity", "destroy"         , Entity::destroy      );
	//Script::function(L, "Entity", "draw"            , Entity::draw         ); // cannot use this in lua as it will override Sprite.draw, Model.draw(), etc.
    Script::function(L, "Entity", "add_component"   , Entity::add_component);
    Script::function(L, "Entity", "remove_component", 0);//Entity::remove_component);		
    Script::function(L, "Entity", "set_component"   , Entity::set_component);
    Script::function(L, "Entity", "set_shader"      , Entity::set_shader   );
    Script::function(L, "Entity", "set_script"      , Entity::set_script   );		
    Script::function(L, "Entity", "get_component"   , Entity::get_component);
    Script::function(L, "Entity", "get_shader"      , Entity::get_shader   );
    Script::function(L, "Entity", "get_script"      , Entity::get_script   );	
    Script::function(L, "Entity", "set_visible"     , Entity::set_visible  );			
	Script::function(L, "Entity", "is_entity"       , Entity::is_entity    );	
	Script::function(L, "Entity", "is_visible"      , Entity::is_visible   );			
	Script::function(L, "Entity", "has_component"   , Entity::has_component);
    //Script::function(L, "", ""        , ::);	
    // component -----------------------------------------------------------	
	Script::table   (L, "Component"                               );
	Script::function(L, "Component", "new"    , Component::create );
    Script::function(L, "Component", "destroy", Component::destroy);
    //Script::function(L, "", ""        , ::);	
	// sub-entities -----------------------------------------------------------
	Script::inherit(L, "Entity", "Sprite"); 
	Script::inherit(L, "Entity", "Model" );
    Script::inherit(L, "Entity", "GUI"   );	
    // camera -----------------------------------------------------------
	Script::table   (L, "Camera"                                      );
	Script::function(L, "Camera", "new"         , Camera::new_        );
	Script::function(L, "Camera", "move"        , Camera::move        );		
	Script::function(L, "Camera", "rotate"      , Camera::rotate      );
	Script::function(L, "Camera", "zoom"        , Camera::zoom        );
	Script::function(L, "Camera", "strafe"      , Camera::strafe      );
	Script::function(L, "Camera", "look"        , Camera::look        );
	Script::function(L, "Camera", "draw"        , Camera::draw        );
	Script::function(L, "Camera", "set_position", Camera::set_position);
	Script::function(L, "Camera", "set_view"    , Camera::set_view    ); // same as look()
	Script::function(L, "Camera", "set_up"      , Camera::set_up      );
	Script::function(L, "Camera", "set_right"   , Camera::set_right   );
	Script::function(L, "Camera", "set_default" , Camera::set_default );
	Script::function(L, "Camera", "get_position", Camera::get_position);	
	Script::function(L, "Camera", "get_view"    , Camera::get_view    );
	Script::function(L, "Camera", "get_up"      , Camera::get_up      );	
	Script::function(L, "Camera", "get_right"   , Camera::get_right   );
	Script::function(L, "Camera", "get_default" , Camera::get_default );
	//Script::function(L, "", ""        , ::);
	// scene -----------------------------------------------------------
	Script::table   (L, "Level"                                                 );
	Script::function(L, "Level", "new"               , Level::new_              );
	Script::function(L, "Level", "add"               , Level::add               );	
	Script::function(L, "Level", "remove"            , Level::remove            );			
	Script::function(L, "Level", "draw"              , Level::draw);
	Script::function(L, "Level", "draw_sky"          , Level::draw_sky          );
	Script::function(L, "Level", "draw_grid"         , Level::draw_grid         );
	Script::function(L, "Level", "draw_axis"         , Level::draw_axis         );
	Script::function(L, "Level", "zoom"              , Level::zoom              );		
	Script::function(L, "Level", "update"            , Level::update            );
	Script::function(L, "Level", "save"              , Level::save              );		
	Script::function(L, "Level", "set_active"        , Level::set_active        );
	Script::function(L, "Level", "set_gravity"       , Level::set_gravity       );
    Script::function(L, "Level", "set_global_ambient", Level::set_global_ambient); // global light
	Script::function(L, "Level", "set_master_volume" , Level::set_master_volume ); // master volume
    Script::function(L, "Level", "set_background"    , Level::set_background    ); // either the color or image of background	
    Script::function(L, "Level", "set_graphics_mode" , Level::set_graphics_mode );	// 2d or 3d
	Script::function(L, "Level", "hide_all_ui"       , 0                        );
	Script::function(L, "Level", "show_all_ui"       , 0                        );
	Script::function(L, "Level", "get_active"        , Level::get_active        );
	Script::function(L, "Level", "get_gravity"       , Level::get_gravity       );
	Script::function(L, "Level", "get_object"        , Level::get_object        );  // by name or id			
	Script::function(L, "Level", "get_camera"        , Level::get_camera        );
	Script::function(L, "Level", "get_light"         , Level::get_light         );		
	Script::function(L, "Level", "get_global"        , Level::get_global        ); // by name or id + type
	Script::function(L, "Level", "is_active"         , Level::is_active         );
	//Script::function(L, "Level", "", Level::);
	// timer -----------------------------------------------------------
	Script::table   (L, "Timer"                                   ); // start, stop
	Script::function(L, "Timer", "new"        , Timer::new_       );
	Script::function(L, "Timer", "reset", Timer::reset            );		
	Script::function(L, "Timer", "milliseconds", Timer::milliseconds);			
	Script::function(L, "Timer", "seconds", Timer::seconds        );
	Script::function(L, "Timer", "minutes", Timer::minutes        );			
	Script::function(L, "Timer", "hours", Timer::hours            );
	Script::function(L, "Timer", "delta", Timer::delta            );
	Script::function(L, "Timer", "framerate", Timer::framerate    );
	//Script::function(L, "", ""        , ::);
	// physics -----------------------------------------------------------
	// lighting -----------------------------------------------------------
	Script::table (L, "Light");
	Script::function(L, "Light", "new"               , Light::new_              );
	Script::function(L, "Light", "on"                , Light::on                );
	Script::function(L, "Light", "off"               , Light::off               );
	Script::function(L, "Light", "set_enabled"       , Light::set_enabled);
	//Script::function(L, "Light", "set_mode"          , Light::set_mode          );
	Script::function(L, "Light", "set_ambient"       , Light::set_ambient       );
	Script::function(L, "Light", "set_diffuse"       , Light::set_diffuse       );
	Script::function(L, "Light", "set_specular"      , Light::set_specular      );
	Script::function(L, "Light", "set_position"      , Light::set_position      );
	Script::function(L, "Light", "set_spot_direction", Light::set_spot_direction);
	Script::function(L, "Light", "set_spot_exponent" , Light::set_spot_exponent );
	Script::function(L, "Light", "set_spot_cutoff"   , Light::set_spot_cutoff   );
	Script::function(L, "Light", "set_constant"      , Light::set_constant      );		
	Script::function(L, "Light", "set_linear"        , Light::set_linear        );
	Script::function(L, "Light", "set_quadratic"     , Light::set_quadratic     );
	Script::function(L, "Light", "set_attenuation"   , Light::set_attenuation   );		
	//Script::function(L, "Light", "get_mode"          , Light::get_mode          );
	Script::function(L, "Light", "get_ambient"       , Light::get_ambient       );
	Script::function(L, "Light", "get_diffuse"       , Light::get_diffuse       );
	Script::function(L, "Light", "get_specular"      , Light::get_specular      );
	Script::function(L, "Light", "get_position"      , Light::get_position      );
	Script::function(L, "Light", "get_spot_direction", Light::get_spot_direction);
	Script::function(L, "Light", "get_spot_exponent" , Light::get_spot_exponent );
	Script::function(L, "Light", "get_spot_cutoff"   , Light::get_spot_cutoff   );
	Script::function(L, "Light", "get_constant"      , Light::get_constant      );
	Script::function(L, "Light", "get_linear"        , Light::get_linear        );
	Script::function(L, "Light", "get_quadratic"     , Light::get_quadratic     );
	Script::function(L, "Light", "get_attenuation"   , Light::get_attenuation   );
	Script::function(L, "Light", "is_enabled"        , Light::is_enabled);
	//Script::function(L, "", ""        , ::);
	// shader -----------------------------------------------------------
	
	Script::table   (L, "Shader"                                  );
    //Script::function(L, "Shader", "new"       , Shader::new_      );
	//Script::function(L, "Shader", "create"    , Shader::create    );		
	//Script::function(L, "Shader", "destroy"   , Shader::destroy   );			
	//Script::function(L, "Shader", "load"      , Shader::load      );
	//Script::function(L, "Shader", "compile"   , Shader::compile   );
	//Script::function(L, "Shader", "set_source", Shader::set_source);
	//Script::function(L, "Shader", "get_id"    , Shader::get_id    );
	//Script::function(L, "Shader", "get_type"  , Shader::get_type  );		
	//Script::function(L, "Shader", "get_source", Shader::get_source);
	//Script::function(L, "Shader", "is_shader" , Shader::is_shader );
	//Script::function(L, "", ""        , ::);
	// resources -----------------------------------------------------------
	Script::table   (L, "Resource"                        ); 
    Script::function(L, "Resource", "load", Resource::load); // loads all resources
    //Script::function(L, "Resource", "", Resource::);		
	// animation -----------------------------------------------------------
	Script::table   (L, "Animator"                                      );
	Script::function(L, "Animator", "new"        , Animator::new_       );	
	Script::function(L, "Animator", "load"       , Animator::load       );
	Script::function(L, "Animator", "animate"    , Animator::animate    );		
	Script::function(L, "Animator", "set_speed"  , Animator::set_speed  );
	Script::function(L, "Animator", "get_actions", Animator::get_actions);
	Script::function(L, "Animator", "get_frames" , Animator::get_frames );
	Script::function(L, "Animator", "get_speed"  , Animator::get_speed  );
    //Script::function(L, "", ""        , ::);	
	// data serialization -----------------------------------------------------------
	Script::table   (L, "Data"                          );
	Script::function(L, "Data", "new"    , Data::new_   ); 
	Script::function(L, "Data", "open"   , Data::open   ); // creates an sql database
	Script::function(L, "Data", "close"  , Data::close  );		
	Script::function(L, "Data", "execute", Data::execute); // executes an sql statement		
	Script::function(L, "Data", "exec"   , Data::execute); // executes an sql statement
	Script::function(L, "Data", "table"  , Data::table  );
	Script::function(L, "Data", "insert" , Data::insert );
	Script::function(L, "Data", "select" , Data::select );	
	Script::function(L, "Data", "update" , Data::update );
	Script::function(L, "Data", "column" , Data::column );	
	Script::function(L, "Data", "drop"   , Data::drop   ); // deletes a SQL table
	Script::function(L, "Data", "", 0);
    //Script::function(L, "", ""        , ::);	
	// factory -----------------------------------------------------------
	Script::table   (L, "Factory"                                    ); // new !
	Script::function(L, "Factory","new"       , Factory::new_        );
	Script::function(L, "Factory","store"     , Factory::store       );
	Script::function(L, "Factory","release"   , Factory::release     );
	Script::function(L, "Factory","get_object", Factory::get_object  );
	Script::function(L, "Factory","get_size"  , Factory::get_size    );
	Script::function(L, "Factory","get_index" , Factory::get_location); // can be used as an id
	Script::function(L, "Factory","is_empty"  , Factory::is_empty    );
	Script::function(L, "Factory","is_stored" , Factory::is_stored   );
	//Script::function(L, "", ""        , ::);
	// script -----------------------------------------------------------
	Script::table   (L, "Script"                                          );
	Script::function(L, "Script", "new"           , Script::new_          );	
	Script::function(L, "Script", "load"          , Script::load_script   );	
	Script::function(L, "Script", "load_directory", Script::load_directory);		
	Script::function(L, "Script", "generate"      , Script::generate      );
	Script::function(L, "Script", "write"         , Script::write         );	
	Script::function(L, "Script", "copy"          , Script::copy          );			
	Script::function(L, "Script", "save"          , Script::save          );			
	Script::function(L, "Script", "count"         , Script::get_count     );
	Script::function(L, "Script", "get_file"      , Script::get_file      );		
	Script::function(L, "Script", "is_loaded"     , Script::is_script     );
    //Script::function(L, "", ""        , ::);	
	// console -----------------------------------------------------------
	Script::table   (L, "Console"                          ); 
	Script::function(L, "Console", "free", Console::destroy);   // destroys the console window
	Script::function(L, "Console", "show", Console::create );	 // recreates the console window (and brings it forward)
	//Script::function(L, "", ""        , ::);
    // vector3 -----------------------------------------------------------
	Script::table   (L, "Vector"                                 ); // a vector class (3d)
	Script::function(L, "Vector", "new"      , Vector3::new_     );
	Script::function(L, "Vector", "length"   , Vector3::length   );
	Script::function(L, "Vector", "normalize", Vector3::normalize);
	Script::function(L, "Vector", "dot"      , Vector3::dot      );
	Script::function(L, "Vector", "cross"    , Vector3::cross    );		
	Script::function(L, "Vector", "add"      , Vector3::add      );
	Script::function(L, "Vector", "sub"      , Vector3::sub      );
	Script::function(L, "Vector", "mul"      , Vector3::mul      );
	Script::function(L, "Vector", "div"      , Vector3::div      );	
	Script::function(L, "Vector", "set"      , Vector3::set      );				 
	Script::function(L, "Vector", "get"      , Vector3::get      );	
    // Math --------------------------------------------------------------	
	Script::table(L, "Math");
	Script::function(L, "Math", "distance", Math::distance2d);
	//Script::function(L, "Math", "", Math::);
	//--------------------------------------------------------------------
	//Script::function(L, "", ""        , ::);
	// globals -----------------------------------------------------------
	// utilities
	// conversion
	Script::global(L, "tosprite", Sprite::to_sprite); // inspired by tonumber() and tostring()
	Script::global(L, "tomodel",    Model::to_model);    // inspired by tonumber() and tostring()
	// entity
	//Script::global(L, "set_shader", Entity::set_shader);
	// global variables -----
	// vector functions
	Script::global (L, "Vector2", Vector2::Vector_new);
    Script::global (L, "Vector3", Vector3::Vector_new);
    Script::global (L, "Vector4", Vector4::Vector_new);	
	// buttons
	Script::global(L, "MOUSE_LEFT",     DOKUN_MOUSE_LEFT);
	Script::global(L, "MOUSE_MIDDLE", DOKUN_MOUSE_MIDDLE);
	Script::global(L, "MOUSE_RIGHT",   DOKUN_MOUSE_RIGHT);	
	// keys
	Script::global(L,"KEY_UP",       DOKUN_KEY_UP);
	Script::global(L,"KEY_DOWN",   DOKUN_KEY_DOWN);
	Script::global(L,"KEY_LEFT",   DOKUN_KEY_LEFT);
	Script::global(L,"KEY_RIGHT", DOKUN_KEY_RIGHT);
    // special
    Script::global(L,"KEY_SPACE", DOKUN_KEY_SPACE);	
    Script::global(L,"KEY_RETURN", DOKUN_KEY_RETURN);
    Script::global(L,"KEY_ESCAPE", DOKUN_KEY_ESCAPE);	
    Script::global(L,"KEY_BACKSPACE", DOKUN_KEY_BACKSPACE);	
    Script::global(L,"KEY_TAB", DOKUN_KEY_TAB);
    Script::global(L,"KEY_CAPS_LOCK", DOKUN_KEY_CAPS_LOCK);
    Script::global(L,"KEY_SHIFT_L", DOKUN_KEY_SHIFT_L);
    Script::global(L,"KEY_SHIFT_R", DOKUN_KEY_SHIFT_R);
    Script::global(L,"KEY_CONTROL_L", DOKUN_KEY_CONTROL_L);
    Script::global(L,"KEY_CONTROL_R", DOKUN_KEY_CONTROL_R);
    Script::global(L,"KEY_ALT_L", DOKUN_KEY_ALT_L);
    Script::global(L,"KEY_ALT_R", DOKUN_KEY_ALT_R);
    Script::global(L,"KEY_MENU", DOKUN_KEY_MENU);
    Script::global(L,"KEY_", DOKUN_KEY_PRINT_SCREEN);
    Script::global(L,"KEY_", DOKUN_KEY_SCROLL_LOCK);
    Script::global(L,"KEY_", DOKUN_KEY_PAUSE);
    Script::global(L,"KEY_", DOKUN_KEY_INSERT);
    Script::global(L,"KEY_", DOKUN_KEY_DELETE);
    Script::global(L,"KEY_", DOKUN_KEY_HOME);
    Script::global(L,"KEY_", DOKUN_KEY_END);
    Script::global(L,"KEY_", DOKUN_KEY_PRIOR);
    Script::global(L,"KEY_", DOKUN_KEY_NEXT);
    Script::global(L,"KEY_", DOKUN_KEY_NUM_LOCK);
    // Numpad
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_DIVIDE);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_MULTIPLY);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_SUBTRACT);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_ADD);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_ENTER);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_DECIMAL);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_DELETE);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_INSERT);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_DELETE);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_INSERT);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_END);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_DOWN);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_NEXT);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_LEFT);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_BEGIN);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_RIGHT);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_HOME);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_UP);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_PRIOR);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_0);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_1);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_2);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_3);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_4);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_5);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_6);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_7);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_8);
    Script::global(L,"KEY_KP_", DOKUN_KEY_KP_9);
    // Symbols
    Script::global(L,"KEY_GRAVE", DOKUN_KEY_GRAVE);	
    Script::global(L,"KEY_ASCIITILDE", DOKUN_KEY_ASCIITILDE);	
    Script::global(L,"KEY_EXCLAMATION", DOKUN_KEY_EXCLAMATION);	
    Script::global(L,"KEY_AT", DOKUN_KEY_AT);	
    Script::global(L,"KEY_NUMBERSIGN", DOKUN_KEY_NUMBERSIGN);	
    Script::global(L,"KEY_DOLLAR", DOKUN_KEY_DOLLAR);	
    Script::global(L,"KEY_PERCENT", DOKUN_KEY_PERCENT);	
    Script::global(L,"KEY_ASCIICIRCUM", DOKUN_KEY_ASCIICIRCUM);	
    Script::global(L,"KEY_AMPERSAND", DOKUN_KEY_AMPERSAND);	
    Script::global(L,"KEY_ASTERISK", DOKUN_KEY_ASTERISK);	
    Script::global(L,"KEY_PARENLEFT", DOKUN_KEY_PARENLEFT);	
    Script::global(L,"KEY_PARENRIGHT", DOKUN_KEY_PARENRIGHT);	
    Script::global(L,"KEY_MINUS", DOKUN_KEY_MINUS);	
    Script::global(L,"KEY_UNDERSCORE", DOKUN_KEY_UNDERSCORE);	
    Script::global(L,"KEY_EQUAL", DOKUN_KEY_EQUAL);	
    Script::global(L,"KEY_PLUS", DOKUN_KEY_PLUS);	
    Script::global(L,"KEY_", DOKUN_KEY_BRACKETLEFT);	
    Script::global(L,"KEY_", DOKUN_KEY_BRACKETRIGHT);	
    Script::global(L,"KEY_", DOKUN_KEY_BRACELEFT);	
    Script::global(L,"KEY_", DOKUN_KEY_BRACERIGHT);	
    Script::global(L,"KEY_", DOKUN_KEY_BACKSLASH);
    Script::global(L,"KEY_", DOKUN_KEY_BAR);
    Script::global(L,"KEY_", DOKUN_KEY_SEMICOLON);
    Script::global(L,"KEY_", DOKUN_KEY_COLON);	
    Script::global(L,"KEY_", DOKUN_KEY_APOSTROPHE);
    Script::global(L,"KEY_QUOTE", DOKUN_KEY_QUOTEDBL);
    Script::global(L,"KEY_", DOKUN_KEY_COMMA);
    Script::global(L,"KEY_", DOKUN_KEY_PERIOD);
    Script::global(L,"KEY_", DOKUN_KEY_LESS);
    Script::global(L,"KEY_", DOKUN_KEY_GREATER);
    Script::global(L,"KEY_", DOKUN_KEY_SLASH);
    Script::global(L,"KEY_", DOKUN_KEY_QUESTION);
    //Script::global(L,"KEY_", DOKUN_KEY_);
    // F1-F12
	Script::global(L,"KEY_F1", DOKUN_KEY_F1);	
	Script::global(L,"KEY_F2", DOKUN_KEY_F2);	
	Script::global(L,"KEY_F3", DOKUN_KEY_F3);	
	Script::global(L,"KEY_F4", DOKUN_KEY_F4);	
	Script::global(L,"KEY_F5", DOKUN_KEY_F5);	
	Script::global(L,"KEY_F6", DOKUN_KEY_F6);	
	Script::global(L,"KEY_F7", DOKUN_KEY_F7);	
	Script::global(L,"KEY_F8", DOKUN_KEY_F8);	
	Script::global(L,"KEY_F9", DOKUN_KEY_F9);	
	Script::global(L,"KEY_F10", DOKUN_KEY_F10);	
	Script::global(L,"KEY_F11", DOKUN_KEY_F11);	
	Script::global(L,"KEY_F12", DOKUN_KEY_F12);	
	// 0-9
	Script::global(L,"KEY_0", DOKUN_KEY_0);
	Script::global(L,"KEY_1", DOKUN_KEY_1);
	Script::global(L,"KEY_2", DOKUN_KEY_2);
	Script::global(L,"KEY_3", DOKUN_KEY_3);
	Script::global(L,"KEY_4", DOKUN_KEY_4);
	Script::global(L,"KEY_5", DOKUN_KEY_5);
	Script::global(L,"KEY_6", DOKUN_KEY_6);
	Script::global(L,"KEY_7", DOKUN_KEY_7);
	Script::global(L,"KEY_8", DOKUN_KEY_8);
	Script::global(L,"KEY_9", DOKUN_KEY_9);	
    //	A-Z
	Script::global(L,"KEY_A", DOKUN_KEY_A);
    Script::global(L,"KEY_B", DOKUN_KEY_B);		
    Script::global(L,"KEY_C", DOKUN_KEY_C);
    Script::global(L,"KEY_D", DOKUN_KEY_D);	
    Script::global(L,"KEY_E", DOKUN_KEY_E);		
    Script::global(L,"KEY_F", DOKUN_KEY_F);		
    Script::global(L,"KEY_G", DOKUN_KEY_G);		
    Script::global(L,"KEY_H", DOKUN_KEY_H);		
    Script::global(L,"KEY_I", DOKUN_KEY_I);		
    Script::global(L,"KEY_J", DOKUN_KEY_J);		
    Script::global(L,"KEY_K", DOKUN_KEY_K);		
    Script::global(L,"KEY_L", DOKUN_KEY_L);		
    Script::global(L,"KEY_M", DOKUN_KEY_M);		
    Script::global(L,"KEY_N", DOKUN_KEY_N);		
    Script::global(L,"KEY_O", DOKUN_KEY_O);		
    Script::global(L,"KEY_P", DOKUN_KEY_P);		
    Script::global(L,"KEY_Q", DOKUN_KEY_Q);		
    Script::global(L,"KEY_R", DOKUN_KEY_R);	
    Script::global(L,"KEY_S", DOKUN_KEY_S);	
    Script::global(L,"KEY_T", DOKUN_KEY_T);		
    Script::global(L,"KEY_U", DOKUN_KEY_U);		
    Script::global(L,"KEY_V", DOKUN_KEY_V);
    Script::global(L,"KEY_W", DOKUN_KEY_W);	
    Script::global(L,"KEY_X", DOKUN_KEY_X);		
    Script::global(L,"KEY_Y", DOKUN_KEY_Y);		
    Script::global(L,"KEY_Z", DOKUN_KEY_Z);	
    //Script::global(L,"KEY_", DOKUN_KEY_);	
   	
	//Script::global (L, "", 0);		
	// -----------------------------------------------------------
#ifndef DOKUN_LUA51
    luaL_newlibtable(L, module);
    luaL_setfuncs   (L, module, 0);	// luaL_newlib(L, module) is equivalent to (luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))
#endif
#ifdef DOKUN_LUA51
	lua_newtable (L);
	luaL_register(L, "dokun", module);
#endif	
	return 1;	// return table library		
}
/////////////////////////////
#ifdef DOKUN_BUILD_LIBRARY
    LUA_API int luaopen_dokun (lua_State *L)
    {
		return Engine::regstr(L);
    }
#endif
/////////////////////////////
#ifdef DOKUN_BUILD_MODULE
    DOKUN_MODULE int luaopen_dokun (lua_State *L)
    {
        return Engine::regstr(L);
    }
#endif
/////////////////////////////
#ifdef DOKUN_BUILD_CONSOLE
    LUA_API int luaopen_dokun (lua_State *L)
    {
	    Engine::open          (L); // initialize engine by default in lua
		return Engine::regstr(L);
    }
#endif
#ifdef __cplusplus // if c++
}
#endif
