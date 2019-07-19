#ifndef _GAME
#define _GAME

#include <lua.hpp>
#include "dokun.h"
class Game {
public:
    Game();
    Game(const std::string& title);
    Game(int width, int height, int mode = 0);
    Game(const std::string& title, int width, int height, int mode = 0);
	Game(int x, int y, int width, int height, int mode = 0);
    ~Game();
	// normal
    void start(void);
	void update(void);
	void destroy(void);
	// setters
	void set_callback(const std::string& type, std::function<void (void)> func);
    // getters
	WINDOW * get_window  () const;
	lua_State * get_state() const;
	Level * get_level    () const; // game world	
	std::function<void (void)> get_callback(const std::string& type) const;
private:
    WINDOW * window;	
	lua_State * state;
	// callback
	std::function<void (void)> on_load;
	std::function<void (void)> on_draw;
	std::function<void (void)> on_focus;
	std::function<void (void)> on_interact;
	std::function<void (void)> on_update;
	std::function<void (void)> on_close;	
};
#endif