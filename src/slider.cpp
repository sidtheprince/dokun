#include "../include/slider.h"

Slider::Slider() : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 255), background_color(160, 160, 160, 255),
    ball_width(10), ball_color(32, 32, 32, 255), ball_radius(50),
	// outline
	outline (false),
    outline_color(0, 0, 0, 255),
    outline_width(1.0),
    outline_antialiased(false)
{
	set_position(0, 0);
	set_size(200, 20);
	set_orientation(0);
}
/////////////            
Slider::Slider(int x, int y) : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 255), background_color(160, 160, 160, 255),
    ball_width(10), ball_color(32, 32, 32, 255), ball_radius(50),
	// outline
	outline (false),
    outline_color(0, 0, 0, 255),
    outline_width(1.0),
    outline_antialiased(false)
{
	set_position(x, y);
	set_size(200, 20);
	set_orientation(0);
}
/////////////
Slider::Slider(int x, int y, int width, int height) : value(0), range(0, 100), radius(50), foreground_color(0, 51, 102, 255), background_color(160, 160, 160, 255),
    ball_width(10), ball_color(32, 32, 32, 255), ball_radius(50),
	// outline
	outline (false),
    outline_color(0, 0, 0, 255),
    outline_width(1.0),
    outline_antialiased(false)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);
}
/////////////
Slider::~Slider()
{}
/////////////		
void Slider::draw()
{
	if(is_visible())  // is it visible? 
	{
		if(!is_active()) // not disabled?
		{}		
		if(is_active()) // is it disabled?
		{}
	    double min_val = get_range().x;
	    double max_val = get_range().y;
		double value = get_value();
	    double x = get_position().x;
		double y = get_position().y;
		double angle = get_angle();
		double scale_x = get_scale().x;
		double scale_y = get_scale().y;
		int width  = get_width();
		int height = get_height();
        int red    = get_color().x;
        int green  = get_color().y;
        int blue   = get_color().z;		
		int alpha  = get_color().w;
		Renderer::draw_slider(x, y, width, height, angle, scale_x, scale_y, red, green, blue, alpha,
			// beam properties
			min_val, max_val, value, background_color,
			// ball properties
			ball_width, ball_color
			// outline
			);
		// mouse over slider
        if(Mouse::is_over(get_ball_x(), get_ball_y(), get_ball_width(), get_ball_height()))
		{
			if(Mouse::is_pressed(1))
			    set_value(get_value() + 2 * 1 / 1); // ?? not sure how to increment it with mouse movement
		}			
	}
    on_draw(); // callback for all gui	
}    
///////////// 
int Slider::draw(lua_State *L)
{
	return 0;
}
/////////////
/////////////
// SETTERS
/////////////		
void Slider::set_color(int red, int green, int blue, int alpha)
{
	foreground_color = Vector4(red, green, blue, alpha);
}  
/////////////
int Slider::set_color(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_range(int max, int min)
{
	range = Vector2(min, max);
} 
/////////////
int Slider::set_range(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_minimum_value(int minimum)
{
	range.x = minimum;
} 
/////////////
int Slider::set_minimum_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_maximum_value(int max)
{} 
/////////////
int Slider::set_maximum_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_value(double value)
{
	if(value >= get_maximum_value()) {this->value = get_maximum_value();return;} // an attempt to exceed max_value
	if(value <= get_minimum_value()) {this->value = get_minimum_value();return;} // an attempt to go below min_value	
	this->value = value;
} 
/////////////
int Slider::set_value(lua_State *L)
{
	return 0;
}
/////////////
void Slider::set_ball_color(int layer, int red, int green, int blue)
{
	switch(layer)
	{
		case 0: // inner
		break;
		case 1: // outer
		break;
	}
}
/////////////
int Slider::set_ball_color(lua_State *L)
{}
/////////////
void Slider::set_ball_inner_color(int red, int green, int blue, int alpha)
{}  
/////////////
int Slider::set_ball_inner_color(lua_State *L)
{
	return 0;
}// ball and beam parts of slider
/////////////
void Slider::set_ball_outer_color(int red, int green, int blue, int alpha)
{}  
/////////////
int Slider::set_ball_outer_color(lua_State *L)
{
	return 0;
}// ball and beam parts of slider
/////////////
void Slider::set_ball_width(int ball_width)
{
	this->ball_width = ball_width;
}
/////////////
void Slider::set_radius(double radius)
{
	//(this)->radius = radius;
}  
/////////////
int Slider::set_radius(lua_State *L)
{
	return 0;
}
/////////////
/////////////
// GETTERS
/////////////
double Slider::get_value()
{
	return value;
} 
/////////////
int Slider::get_value(lua_State *L)
{
	return 1;
}
/////////////
Vector2 Slider::get_range()
{
	return range;
} 
/////////////
int Slider::get_range(lua_State *L)
{
	return 2;
}
/////////////
double Slider::get_minimum_value()
{
	return range.x;
} 
/////////////
int Slider::get_minimum_value(lua_State *L)
{
	return 1;
}
/////////////
double Slider::get_maximum_value()
{
	return range.y;
} 
/////////////
int Slider::get_maximum_value(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Slider::get_color() const
{
	return foreground_color;
}
int Slider::get_color(lua_State *L)
{
	return 1;
}
/////////////
/////////////
/////////////
/////////////
/////////////
int Slider::get_ball_x()
{
	return get_x() + (get_value() / get_maximum_value()) * static_cast<float>(get_width());
}
int Slider::get_ball_x(lua_State *L)
{}
/////////////
int Slider::get_ball_y()
{
	return get_y() + 1;
}
int Slider::get_ball_y(lua_State *L)
{}
/////////////
int Slider::get_ball_width()
{
	return ball_width;
}
int Slider::get_ball_width(lua_State *L)
{
	return 1;
}
/////////////
int Slider::get_ball_height()
{
	return get_height() - 2;
}
int Slider::get_ball_height(lua_State *L)
{}
/////////////
/////////////
/////////////
/////////////
/////////////
// BOOLEAN
/////////////		
bool Slider::is_moved()
{
	return false;
} 
int Slider::is_moved(lua_State *L)
{
	lua_pushboolean(L, false);
	return 1;
}
/////////////
/////////////
int Slider::new_(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}