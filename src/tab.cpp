#include "../include/tab.h"

Tab::Tab()
{
	label = new Label();
}
Tab::~Tab()
{}
void Tab::draw()
{
	if(is_visible())
	{}	
}                                
int Tab::draw(lua_State * L){}
void Tab::add(const Tab& tab){}                   int Tab::add(lua_State * L){}
	
void Tab::set_text(const std::string& text){}     int Tab::set_text(lua_State *L){}
void Tab::set_label(const Label& label){}         int Tab::set_label(lua_State *L){}
void Tab::set_image(const Image& image){}         int Tab::set_image(lua_State *L){}
void Tab::set_color(int red, int green, int blue, int alpha){}
void Tab::set_color(const Vector3& color){}       int Tab::set_color(lua_State *L){}
void Tab::set_color(const Vector4& color){}
	
std::string Tab::get_text()const{}                int Tab::get_text(lua_State *L){}
Label * Tab::get_label()const{}                   int Tab::get_label(lua_State *L){}
Image * Tab::get_image()const{}                   int Tab::get_image(lua_State *L){}
Vector4 Tab::get_color()const{}                   int Tab::get_color(lua_State *L){}