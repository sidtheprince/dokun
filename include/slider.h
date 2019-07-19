#ifndef _SLIDER
#define _SLIDER

#include "ui.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Slider : public GUI // a bit like a ball and beam
{
    public:
	    Slider();           static int new_(lua_State *L);
		Slider(int x, int y);
		Slider(int x, int y, int width, int height);
		~Slider();
		
		void draw();    static int draw(lua_State *L);
		
		void set_color(int red, int green, int blue, int alpha = 255); static int set_color(lua_State *L);// beam color
		void set_range(int max_val, int min_val);static int set_range(lua_State *L);
		void set_minimum_value(int min_val);static int set_minimum_value(lua_State *L);
		void set_maximum_value(int max_val);static int set_maximum_value(lua_State *L);
		void set_value(double value);static int set_value(lua_State *L);
		// ball properties
		void set_ball_color(int layer, int red, int green, int blue); static int set_ball_color(lua_State *L);// ball color; 0 = inner, 1 = outer
		void set_ball_inner_color(int red, int green, int blue, int alpha = 255); static int set_ball_inner_color(lua_State *L);// ball and beam parts of slider
		void set_ball_outer_color(int red, int green, int blue, int alpha = 255); static int set_ball_outer_color(lua_State *L);// ball and beam parts of slider
		void set_ball_width(int ball_width);
		// slider (beam) properties
		void set_radius(double radius); static int set_radius(lua_State *L);// roundness of beam edges [===========]
		// outline properties
		// outline
		void set_outline(bool outline); static int set_outline(lua_State *L);
		void set_outline_width(double width); static int set_outline_width(lua_State *L);
		void set_outline_color(int red, int green, int blue, int alpha = 255); static int set_outline_color(lua_State *L);
		void set_outline_color(const Vector3& color);// static int set_(lua_State *L);
		void set_outline_color(const Vector4& color);// static int set_(lua_State *L);
		void set_outline_antialiased(bool antialised); static int set_outline_antialiased(lua_State *L);
		// border properties
		void set_border(bool border);
		// getters
		double get_value();static int get_value(lua_State *L);
		Vector2 get_range();static int get_range(lua_State *L);
		double get_minimum_value();static int get_minimum_value(lua_State *L);
		double get_maximum_value();static int get_maximum_value(lua_State *L);
		Vector4 get_color() const; static int get_color(lua_State *L);
		// ball
		int get_ball_x();static int get_ball_x(lua_State *L);
		int get_ball_y();static int get_ball_y(lua_State *L);
		int get_ball_width(); static int get_ball_width(lua_State *L);
		int get_ball_height();static int get_ball_height(lua_State *L);
		// boolean
		bool is_moved();static int is_moved(lua_State *L); // ball on slider is moved(value changed)
		//static int (lua_State *L);
	private:
	    // beam
	    double value;
		Vector2 range;
		Vector4 foreground_color;
		Vector4 background_color;
		double radius;
		// ball
		double ball_position;
		Vector4 ball_color;
		Vector4 ball_color_inner;
		Vector4 ball_color_outer;
		int ball_width;
		double ball_radius;
		// outline
		bool outline;
		Vector4 outline_color;
		double outline_width;
		bool outline_antialiased;
};
/*
// slider example:
[----0---]

// Checking if mouse is over pivot (horizontal slider)
double ball_y        = slider->get_y();
double ball_width    = slider->get_ball_width();
double ball_x        = slider->get_x() + slider->get_value() * 2;
double ball_height   = slider->get_height();
if(Mouse::is_over(ball_x, ball_y, ball_width, ball_height))
{
	if(Mouse::is_down()) // drag the pivot
	{
		slider->set_value();
	}
}	
	Slider * slider = new Slider();
	slider->set_position(10, 200);
	
	
*/
#endif
#endif