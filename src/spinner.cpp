#include "../include/spinner.h"

Spinner::Spinner() : value(0), range(0, 100), color(236, 238, 241, 255),
// button
button_width(20),
button_color(160, 160, 160, 255),
// outline
outline(true),
outline_width(2.0),
outline_color(0, 0, 0, 255),
outline_antialiased(false)
{
	set_position(0, 0);
	set_size(60, 30);
	set_orientation(0);	
	
	Label * label = new Label();
	label->set_color(0, 0, 0, 255);
	label->set_string(std::to_string(get_value()));
	set_label(*label);
}
Spinner::Spinner(int x, int y)
{
	set_position(x, y);
	set_size(60, 30);
	set_orientation(0);	

	Label * label = new Label();
	label->set_color(0, 0, 0, 255);
	label->set_string(std::to_string(get_value()));
	set_label(*label);	
}
Spinner::Spinner(int x, int y, int width, int height)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);	
	
	Label * label = new Label();
	label->set_color(0, 0, 0, 255);
	label->set_string(std::to_string(get_value()));
	set_label(*label);	
}
int spinner_new(lua_State *L)
{}
Spinner::~Spinner()
{}
////////////////		
void Spinner::draw()
{
	if(is_visible())
	{
		Renderer::draw_spinner(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y, color.x, color.y, color.z, color.w,
		    value, button_width, button_color, outline, outline_width, outline_color, outline_antialiased);
		// Label
		label->set_string(std::to_string(static_cast<int>(get_value())));
		label->set_position(get_x() + 1, get_y() + 10);
		label->set_scale(0.5, 0.5);
        label->draw();	
	}
	on_draw(); // callback for all gui
}
int Spinner::draw(lua_State *L)
{}
////////////////
void Spinner::set_value(double value)
{
	if(value >= get_maximum_value()) {this->value = get_maximum_value();return;} // an attempt to exceed max_value
	if(value <= get_minimum_value()) {this->value = get_minimum_value();return;} // an attempt to go below min_value	
	this->value = value;
}
int Spinner::set_value(lua_State * L)
{}
void Spinner::set_range(double min_val, double max_val)
{
	range = Vector2(min_val, max_val);
}
void Spinner::set_range(const Vector2& range)
{
	set_range(range.x, range.y);
}
int Spinner::set_range(lua_State * L)
{}
void Spinner::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
void Spinner::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
void Spinner::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
int Spinner::set_color(lua_State * L)
{}

void Spinner::set_label(const Label& label)
{
	this->label = &const_cast<Label&>(label);
}
int Spinner::set_label(lua_State * L)
{}
////////////////
double Spinner::get_value() const
{
	return value;
}
int Spinner::get_value(lua_State * L)
{}
double Spinner::get_minimum_value() const
{
	return range.x;
}
int Spinner::get_minimum_value(lua_State * L)
{}
double Spinner::get_maximum_value() const
{
	return range.y;
}
int Spinner::get_maximum_value(lua_State * L)
{}
Vector2 Spinner::get_range() const
{
	return range;
}
int Spinner::get_range(lua_State * L)
{}
Vector4 Spinner::get_color() const
{
	return color;
}
int Spinner::get_color(lua_State * L)
{}
Label * Spinner::get_label() const
{
	return label;
}
int Spinner::get_label(lua_State * L)
{}
int Spinner::get_top_button_x() const
{
	return get_x() + get_width();
}
int Spinner::get_top_button_y() const
{
	return get_y();
}
int Spinner::get_top_button_width() const
{
	return button_width;
}
int Spinner::get_top_button_height() const
{
	return get_height() / 2;
}

int Spinner::get_bottom_button_x() const
{
	get_x() + get_width();
}
int Spinner::get_bottom_button_y() const
{
	return get_y() + (get_height() / 2);
}
int Spinner::get_bottom_button_width() const
{
	return button_width;
}
int Spinner::get_bottom_button_height() const
{
	return get_height() / 2;
}	