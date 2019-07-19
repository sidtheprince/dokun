#include "../include/game.h"

Game::Game()
{
	window = new WINDOW();
	window->create();
}
Game::Game(const std::string& title)
{
	window = new WINDOW();
	window->create(title);
}
Game::Game(int width, int height, int mode)
{
	window = new WINDOW();
	window->create(width, height, mode);
}
Game::Game(const std::string& title, int width, int height, int mode)
{
	window = new WINDOW();
	window->create(title, width, height, mode);
}
Game::Game(int x, int y, int width, int height, int mode)
{
	window = new WINDOW();
	window->create(x, y, width, height, mode);	
}
Game::~Game()
{
	destroy();
}
void Game::start(void)
{
	Engine::open(); // start the engine
	if(Engine::get_status() == 0) 
	{
		Logger("Could not start engine");
		exit(0);
	}
	state = luaL_newstate();
	if(state == nullptr)  
	{
		Logger("Could not start lua");
		exit(0);
	}
	//level = new Level(); // create the world
	window->show(); // show the window
}
void Game::update(void)
{
	if(on_load) {
		on_load();
	}	
	while (1)
	{
		if(on_draw) on_draw();
		//if(level) level->draw();
	    window->update();
	}
	if(on_close) {
		on_close();
	}
}
void Game::destroy(void)
{
	window->destroy(); // destroy window
	Engine::close();   // close engine
	lua_close(state);  // close state
}

void Game::set_callback(const std::string& type, std::function<void (void)> func)
{
	if(String::lower(type) == "on_start")
	{
		on_load = func;
	}
	if(String::lower(type) == "on_load")
	{
		on_load = func;
	}
	if(String::lower(type) == "on_draw")
	{
		on_draw = func;
	}
	if(String::lower(type) == "on_focus")
	{
		on_focus = func;
	}	
	if(String::lower(type) == "on_interact")
	{
		on_interact = func;
	}	
	if(String::lower(type) == "on_update")
	{
		on_update = func;
	}
	if(String::lower(type) == "on_close")
	{
		on_close = func;
	}
	if(String::lower(type) == "on_")
	{}
}
    // getters
WINDOW * Game::get_window  () const
{
	return window;
}
lua_State * Game::get_state() const
{
	return state;
}
/* Level * Game::get_level    () const
{
	return level;
}	 */
std::function<void (void)> Game::get_callback(const std::string& type) const
{
	if(String::lower(type) == "on_start")
	{
		return on_load;
	}
	if(String::lower(type) == "on_load")
	{
		return on_load;
	}
	if(String::lower(type) == "on_draw")
	{
		return on_draw;
	}
	if(String::lower(type) == "on_focus")
	{
		return on_focus;
	}	
	if(String::lower(type) == "on_interact")
	{
		return on_interact;
	}	
	if(String::lower(type) == "on_update")
	{
		return on_update;
	}
	if(String::lower(type) == "on_close")
	{
		return on_close;
	}
	if(String::lower(type) == "on_")
	{}
}
/////////////////////////////////
static void on_load()
{}
static void on_draw()
{}
/////////////////////////////////
int main(int argc, char * argv[])
{
    Game game("Sid's Game", 1280, 720);	
	//game.set_callback("on_", on_);
	game.set_callback("on_load", on_load);
	game.set_callback("on_draw", on_draw);
	game.start();
	game.update();
	game.destroy();
	return 0;
}