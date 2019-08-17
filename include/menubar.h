#ifndef _MENUBAR
#define _MENUBAR

#include "ui.h"
#include "label.h"
#include "image.h"
#include "box.h"
#ifdef __cplusplus
class Menubar : public GUI
{
public:
    Menubar();                             static int menu_new(lua_State *L);
	Menubar(int x, int y);
	Menubar(int x, int y, int width, int height);
	~Menubar();
		
	void draw();                           static int draw(lua_State *L);
	void add(const Box& menu);          static int add(lua_State * L);
	void add(const std::string& menu);
	void sub(const Box& sub_menu, int index);
	void sub(const std::string& menu, const std::string& sub_menu);
    // setters
	void set_color(int red, int green, int blue, int alpha = 225);
	void set_color(const Vector3& color);
	void set_color(const Vector4& color);
	// outline
	void set_outline(bool outline); static int set_outline(lua_State *L);
	void set_outline_width(double width); static int set_outline_width(lua_State *L);
	void set_outline_color(int red, int green, int blue, int alpha = 255); static int set_outline_color(lua_State *L);
	void set_outline_color(const Vector3& color);// static int set_(lua_State *L);
	void set_outline_color(const Vector4& color);// static int set_(lua_State *L);
	void set_outline_antialiased(bool antialised); static int set_outline_antialiased(lua_State *L);	
	// getters
	Box * get_menu(int index);     static int get_menu(lua_State * L);
	Box * get_submenu(int menu_index, int sub_index);     static int get_submenu(lua_State * L);
	int get_menu_count(); static int get_menu_count(lua_State * L);
	int get_submenu_count(int index); static int get_submenu_count(lua_State * L);
	Vector4 get_color();              static int get_color(lua_State *L);
	// static int (lua_State * L);
	// boolean
	bool has_sub(int menu_index);// static int (lua_State * L);
private:
    // callback
	void on_hover(void);
	void on_menu_press(Box * menu, int index);
    Box * previous;
	std::vector<Box *> menu_list; 
	std::vector< std::vector<Box *> > sub_list;//std::vector<Box *> sub_menu;
	Vector4 color; // applies to all menus
	Vector4 submenu_color; // color for all submenus	
	// outline
	bool outline;
	double outline_width;
	bool outline_antialiased; // removes staircase effect from line and straightens the line (can lower performance)
	Vector4 outline_color;	
	// highlight
	bool highlight;
	Vector4 highlight_color;
};
#endif
#endif
/*
USAGE:
	Menubar * menubar = new Menubar();
	menubar->set_position(600, 20);
	menubar->add(*new Box());
	menubar->add(*new Box());
	menubar->add(*new Box());
	menubar->sub(*new Box(), 0); // add submenu to menu 1
	menubar->sub(*new Box(), 0); // add submenu to menu 1
	menubar->sub(*new Box(), 1); // add submenu to menu 2
	menubar->sub(*new Box(), 2); // add submenu to menu 3

*/
