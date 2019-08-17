#ifndef _EDITOR
#define _EDITOR
//--------
#include "build.h"
#include DOKUN_HEADER
//--------

#ifdef __cplusplus
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>
#include <tuple>

class Editor // drag-drop level editor (also generates lua files)
{
public:
    Editor();
	~Editor();
	void setup(); // set up all ui elements
	// normal functions
	bool create();
	bool create(const std::string& project, std::string path = ""); // current path of editor executable or use User/Documents/ path
    bool load();
	void unload(); // unloads project
	void open(); // iniitialize editor
	void close(); // close editor
	
	void draw(); // draws the ui elements
	void execute(); // executes the program
	void update(); // updates the editor (window)
	// cache
	void cache(); // writes a cache file
	void clear(); // clears the cache
	// executable
	void play();  // runs executable
	void run();   // runs script
	void stop();  // terminates the executable
	void build(); // builds executable from source
	bool clean(); // deletes executable
	// getters
	bool get_status();
	std::string get_project(); // returns name of project
	std::string get_project_path(); // returns path of project
	std::string get_default_path(); // returns default project path
	// special getters
	WINDOW * get_window()const;
	int get_error(); // returns last error
	lua_State * get_state()const;
	std::vector<std::tuple<GUI *, int, lua_CFunction>> get_connection(int index);
	// boolean
	bool is_open();
	bool has_project();
	bool in_cache(); // checks if project is in cache_file
	// callbacks
	void on_open();
	void on_close();
	void on_create(void); // create new project
	void on_update(void); // updates ui properties
	void on_draw(void);
	void on_interact(void);
	// load functions
	bool load_file(std::string file_name = ""); // loads a level or dokun file with project data
	bool load_directory(); // loads a folder and retrieves project file from folder
	bool load_level();
	bool load_sprite(); // uses OS-specific file dialog
	bool load_sprite_custom(); // uses custom file dialog
private:
    // private functions
	void connect(); // gets user input
    // viewer, grid (with sprites, etc.; can drag items from grid to viewer)
	static bool status;
	std::vector<std::string> recent_list;
	WINDOW * window;
	int error;
	lua_State * state;
	Engine * engine;
public:	
    // level
	Level * level;
    // titlebar
	Box * titlebar;
	Box * titlebar_close_button;
	Box * titlebar_maximize_button;
	Box * titlebar_iconify_button;
	Label * titlebar_text;
    // menubar
    Menubar * menubar;
	// toolbox
	Box * sidebar;
	Box * mainbar;
	Box * toolbar;
	// tool buttons
    Image * play_button;
	Image * pause_button;
	Image * stop_button;
	Image * run_button;
	Image * build_button;	
	// property_box
	Box * property_box;
	Entity * selected_entity;
	Image * selected_image;
	Box * selection_box;
	// geometry
	Edit * position_edit;
	Edit * position_edit1;
	Edit * rotation_edit;
	Edit * scale_edit;
	// texture
	// material
	Edit * color_red_edit;
	Edit * color_green_edit;
	Edit * color_blue_edit;
	// collection
	Grid * collection_box;
	Box * mouse_box;
	// messagebox
	Box * dialog;
	// button
	Button * ok_button;
	//Button * cancel_button;
	Edit * project_edit;
	bool dragged;
	// messagebox
	Box * quit_dialog;
	// close button
	Image * cross;
	
	Texture * tile_sheet;
	Sprite * map;
	int frame;
	Combobox * combo;
	// file dialog
	Box * file_dialog;
	List * file_list;
	Button * open_button;
	Button * save_button;
	Button * cancel_button;
	Edit * file_edit;
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////	
	struct editor_data
	{
		std::string mode; // TWO MODES = edit_mode, play_mode
		void * selection; // selected object
	} editor_data;
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////	
	struct project_data
	{
		std::string name;
		std::string path;
		std::string default_path; // C:/User/~/Documents
	} project_data;
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////	
};
#endif
#endif
/*
Editor -
mode Play Mode
----------------
Menubar
file - new, open, save, save_as, print, export, recent
edit - undo, redo, cut, copy, paste, select all, 
view - zoom(in, out)
----------------
Toolbar
play - switches to Play Mode
stop - switches to Edit Mode
build - builds an executable
----------------
Recent -
// open cache.txt
----------------
Objects - // list of created objects (if an object is clicked in this list, the object's properties will appear)
- Scene0
    - Camera0 
    - Light0
    - Sprite0
----------------
Properties - // object properties box (is shown when object is selected in viewer)
[DEFAULT]
name     Sprite0
id       0

[GEOMETRY]
position 0, 0, 0
rotation 0, 0, 0
scale    1, 1, 1

[TEXTURE]
texture  "player/player.png"
size     0, 0, 1

[MATERIAL]
color    0, 0, 0, 255

[ANIMATION]
animation_0:
    frame_0  "player/player.png"  
	frame_1  "player/player_dead.png"
	OR
	frame_0  "player/player_full_sprite_sheet"  32, 32, 0, 0   <= RECT for full sprite sheet
	frame_1  "player/player_full_sprite_sheet"  0, 32, 32, 0
	
[SCRIPTS]
    main "player/player.lua"	
[COMPONENTS] // you can create custom components
    Component0
    	name=Component0
		value=1   
		type=number	
	Component1
	    name=visible
		value=true
		type=boolean
    + add
	- delete
----------------	
[EVENT CREATOR]
[object]->[component] [==, !=, <, >, <=, >=] [userdata(number, string, etc.)] [object]->[function]
----------------	
Assets -	// a grid full of resources
	[image_here][model_here][][][][]
----------------
Console - // shows console log 
	------------------------   
	|> error at line:5      |
	|> sprite destroyed     |
	|-----------------------|


Random code:
// from dragging an object from collection
	Box * mouse_box = new Box();
	mouse_box->set_size(32, 32);
	mouse_box->set_outline(true);
	mouse_box->set_outline_width(2.0);
	mouse_box->set_outline_color(255, 255, 255);
	std::vector<Box *> block_stored;
	
// in loop	
		// draw mouse box
		mouse_box->draw();
		mouse_box->set_position(Mouse::get_position(window));
		// draw object stored on mouse click
		for(int i = 0; i < block_stored.size(); i++)
            block_stored[i]->draw();		
if(Mouse::is_pressed(1))
			{
				// create object on mouse press
				Box * widget = new Box(*mouse_box);
				//if(widget->get_position() != Mouse::get_position(window))
				widget->set_position(Mouse::get_position(window));
			    // store object
				block_stored.push_back(widget);
			}
			
			
// on drag
if(Mouse::over(obj->get_rect()))
{
	if(Mouse::is_pressed(1))
	{
		obj->set_position(Mouse::get_position(window));
	} else obj->set_position(obj->get_position());
}	
	
*/
