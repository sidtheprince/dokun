#include "../include/tooltip.h"

Tooltip::Tooltip() : label(nullptr), color(96, 96, 96, 255), text_color(255, 255, 255, 255)
{
	set_position(0, 0);
	set_size(100, 50);
	set_position(0, 0);
	set_orientation(0);		
	label = new Label();
}
///////////////////////
Tooltip::Tooltip(int x, int y) : label(nullptr), color(96, 96, 96, 255), text_color(255, 255, 255, 255)
{
	set_position(x, y);
	set_size(100, 50);
	set_position(0, 0);
	set_orientation(0);
	label = new Label();
}
///////////////////////
Tooltip::Tooltip(int x, int y, int width, int height) : label(nullptr), color(96, 96, 96, 255), text_color(255, 255, 255, 255)
{
	set_position(x, y);
	set_size(width, height);
	set_position(0, 0);
	set_orientation(0);
    label = new Label();	
}
///////////////////////
Tooltip::Tooltip(const std::string& text) : label(nullptr), color(96, 96, 96, 255), text_color(255, 255, 255, 255)
{
	label = new Label();
	set_text(text);
	set_position(0, 0);
	set_size(100, 100);
	set_position(0, 0);
	set_orientation(0);	
}
Tooltip::~Tooltip()
{}
///////////////////////
// normal
void Tooltip::draw()
{
	if(is_visible())
	{
		std::string text = get_text();
	    int x = get_x();
	    int y = get_y();
	    int width  = get_width ();
	    int height = get_height();
		double angle = get_angle();
		double scale_x = get_scale().x;
		double scale_y = get_scale().y;
	    double red = get_color().x;
	    double green = get_color().y;
	    double blue  = get_color().z;
	    double alpha = get_color().w;
		Renderer::draw_tooltip(text, x, y, width, height, angle, scale_x, scale_y,
		    red, green, blue, alpha);
		if(label != nullptr)
		{
			label->draw();
			//Renderer::draw_label(text, x, y, angle, get_label()->get_scale().x, get_label()->get_scale().y,
			//   get_label()->get_font()->get_data(), 
			//   get_text_color().x,get_text_color().y,get_text_color().z,get_text_color().w);
		}
	}
	on_draw(); // callback for all gui
}
int Tooltip::draw(lua_State *L)
{
    return 0;
}
///////////////////////
// setters
void Tooltip::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
///////////////////////
int Tooltip::set_color(lua_State *L)
{
    return 0;
}
///////////////////////
void Tooltip::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
///////////////////////
void Tooltip::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
///////////////////////
void Tooltip::set_text(const std::string& text)
{
	get_label()->set_string(text);
}
///////////////////////
int Tooltip::set_text(lua_State *L)
{
    return 0;
}
///////////////////////
void Tooltip::set_label(const Label& label)
{
	this->label = &const_cast<Label&>(label);
}
///////////////////////
int Tooltip::set_label(lua_State *L)
{
    return 0;
}
///////////////////////
void Tooltip::set_text_color(int red, int green, int blue, int alpha)
{
	if(get_label()) get_label()->set_color(red, green, blue, alpha);
	text_color = Vector4(red, green, blue, alpha);
}
///////////////////////
void Tooltip::set_text_color(const Vector3& color)
{
	set_text_color(color.x, color.y, color.z);
}
///////////////////////
void Tooltip::set_text_color(const Vector4& color)
{
	set_text_color(color.x, color.y, color.z, color.w);
}
///////////////////////
int Tooltip::set_text_color(lua_State *L)
{
    return 0;
}
///////////////////////
// getters
std::string Tooltip::get_text()const
{
	if(!get_label())
	return text;
    return get_label()->get_string();
}
///////////////////////
int Tooltip::get_text(lua_State *L)
{
    return 1;
}
///////////////////////
Label * Tooltip::get_label()const
{
	return label;
}
int Tooltip::get_label(lua_State *L)
{
    return 1;
}
///////////////////////
Vector4 Tooltip::get_color()const
{
	return color;
}
///////////////////////
int Tooltip::get_color(lua_State *L)
{
    return 4;
}
///////////////////////
Vector4 Tooltip::get_text_color()const
{
	if(get_label()) return get_label()->get_color();
	return text_color;
}
///////////////////////
int Tooltip::get_text_color(lua_State *L)
{
    return 4;
}
