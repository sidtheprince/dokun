#ifndef _WIDGET
#define _WIDGET

#include "ui.h"
#include "label.h"
#include "image.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Widget : public GUI { // A box ui - ui element that can act as a container for any kind of ui (button, icon, menu, grid, frame, list, tabs, tool_tip, dialog)
    public:
        Widget();                                                                      static int widget_new(lua_State * L);
        Widget(const Widget& widget);
		Widget(const std::string& type);
		Widget(int x, int y);
        Widget(int x, int y, int width, int height);
		~Widget();
		// normal
		void add(const GUI& gui);                                                      static int add(lua_State * L);
		void draw(void);                                                               static int draw(lua_State * L);
		void maximize(void);                                                           static int maximize(lua_State * L);
		void iconify(void);                                                            static int iconify(lua_State * L);
		void restore(void);                                                            static int restore(lua_State * L);
		// setters
		void set_color(int red, int green, int blue, int alpha = 255);                 static int set_color(lua_State * L);
		void set_color(const Vector3& color);
		void set_color(const Vector4& color);
		void set_alpha(double alpha);                                                  static int set_alpha(lua_State * L);
		void set_fill(bool fill);                                                      static int set_fill(lua_State * L);
		// outline - is line drawn around border?
		void set_outline(bool outline);                                                static int set_outline(lua_State * L);
		void set_outline_width(double width);                                          static int set_outline_width(lua_State * L);
		void set_outline_color(int red, int green, int blue, int alpha = 255);         static int set_outline_color(lua_State * L);
		void set_outline_color(const Vector3& color);
		void set_outline_color(const Vector4& color);
		void set_outline_style(int style);                                             static int set_outline_style(lua_State * L);
		void set_outline_antialiased(bool antialiased);		                           static int set_outline_antialiased(lua_State * L);
		// border - does it have a border(frame)?
		void set_border(bool border);                                                  static int set_border(lua_State * L);
		void set_border_size(int width, int height);                                   static int set_border_size(lua_State * L);
		void set_border_width(int width, int side = 0);                                static int set_border_width(lua_State * L);
		void set_border_color(int red, int green, int blue, int alpha = 255);          static int set_border_color(lua_State * L);
		void set_border_color(const Vector3& color);
		void set_border_color(const Vector4& color);
		void set_border_style(int style, int side = 0);                                static int set_border_style(lua_State * L);
		void set_border_radius(int radius, int side = 0);                              static int set_border_radius(lua_State * L);
		// radius - how round are the edges?
		void set_radius(double radius, int side = 0);                                  static int set_radius(lua_State * L);
		void set_trim(double trim, int side = 0);                                      static int set_trim(lua_State * L);
	    void set_alignment(const std::string& alignment);                              static int set_alignment(lua_State * L);
		// shadow -  does it have a shadow around it?
	    void set_shadow(bool shadow);                                                  static int set_shadow(lua_State * L);
		// gradient - do the colors mix?
		void set_gradient(bool gradient);                                              static int set_gradient(lua_State * L);
		void set_gradient_color(int red, int green, int blue, int alpha = 255, bool mix = true);
		void set_gradient_color(const Vector3& color, bool mix = true);
		void set_gradient_color(const Vector4& color, bool mix = true);
		void set_gradient_color(const Vector3& color0, const Vector3& color1);
		void set_gradient_color(const Vector4& color0, const Vector4& color1);
		// title_bar - does it have a title_bar
		void set_title_bar(bool has_title_bar);                                        static int set_title_bar(lua_State * L);
		void set_title_bar_size(int height);                                           static int set_title_bar_size(lua_State * L);
		void set_title_bar_text(const std::string& text);                              static int set_title_bar_text(lua_State * L);
		void set_title_bar_text_color(int red, int green, int blue, int alpha = 255);  static int set_title_bar_text_color(lua_State * L);
		void set_title_bar_text_color(const Vector3& color);
		void set_title_bar_text_color(const Vector4& color);
		void set_title_bar_label(const Label& label);                                  static int set_title_bar_label(lua_State * L);
		void set_title_bar_image(const Image& image);                                  static int set_title_bar_image(lua_State * L);
		void set_title_bar_icon(const Image& icon);
		void set_title_bar_color(int red, int green, int blue, int alpha = 255);       static int set_title_bar_color(lua_State * L);
        void set_title_bar_color(const Vector3& color);
		void set_title_bar_color(const Vector4& color);		
		void set_title_bar_button_iconify(bool button_iconify);                        static int set_title_bar_button_iconify(lua_State * L);
		void set_title_bar_button_maximize(bool button_maximize);                      static int set_title_bar_button_maximize(lua_State * L);
		void set_title_bar_button_close(bool button_close);                            static int set_title_bar_button_close(lua_State * L);
		void set_forbidden_area(int x, int y, int width, int height);
		void set_forbidden_area(const Vector4& rect);
		// icon
		void set_as_icon(bool icon);
		void set_image(const Image& image);                                            static int set_image(lua_State *L);
        void set_image_list(const Image& image);                                       static int set_image_list(lua_State *L);
		// label
		void set_text(const std::string& text);                                        static int set_text(lua_State * L);
        void set_text_list(const std::string& text, int index);                        static int set_text_list(lua_State * L);
		void set_label(const Label& label);                                            static int set_label(lua_State * L);
        void set_label_list(const Label& label);                                       static int set_label_list(lua_State * L);
		// getters
		Vector4 get_color() const;                                                     static int get_color(lua_State * L);
		Image * get_image () const;                                                    static int get_image(lua_State * L);	
        Image * get_image_list (int index) const;                                      static int get_image_list(lua_State * L);
		Label * get_label() const;                                                     static int get_label(lua_State * L);
        Label * get_label_list(int index) const;                                       static int get_label_list(lua_State * L);
		std::string get_text() const;                                                  static int get_text(lua_State * L);
        std::string get_text_list(int index) const;                                    static int get_text_list(lua_State * L);
		std::string get_alignment()const;                                              static int get_alignment(lua_State * L);
		// title_bar
		Vector2 get_title_bar_position()const;
		Vector2 get_title_bar_size()const;
		Vector4 get_title_bar_color() const;
		std::string get_title_bar_text() const;
		Vector4 get_title_bar_text_color() const;
		Image * get_title_bar_icon() const;
		Vector2 get_title_bar_button_iconify_position()const;
		Vector2 get_title_bar_button_iconify_size()const;
		Vector2 get_title_bar_button_maximize_position()const;
		Vector2 get_title_bar_button_maximize_size()const;
		Vector2 get_title_bar_button_close_position()const;
		Vector2 get_title_bar_button_close_size()const;
		Vector4 get_title_bar_button_close_color()const;
		// outline
		// border
		// radius
		double get_radius() const;                                                     static int get_radius(lua_State * L);
		// boolean
		bool is_box  ()const;                                                               static int is_box(lua_State * L);
		bool is_icon ()const;                                                               static int is_icon(lua_State * L);
		bool has_title_bar()const;                                                          static int has_title_bar(lua_State * L);
		bool has_outline()const;                                                            static int has_outline(lua_State * L);
		bool has_border()const;                                                             static int has_border(lua_State * L);
		bool has_shadow()const;                                                             static int has_shadow(lua_State * L);
		bool has_gradient()const;                                                           static int has_gradient(lua_State * L);
        bool is_maximized()const;                                                           static int is_maximized(lua_State * L);
        bool is_iconified()const;                                                           static int is_iconified(lua_State * L);
        bool is_restored()const;                                                            static int is_restored(lua_State * L);
	private:
	    // callbacks
	    //void on_draw(void); // not needed for now
		void on_titlebar(void);
		void on_drag(void);
		void on_resize(void);
	    // friend
	    friend class Menubar;
		friend class Grid;
		friend class List;
		Widget * previous;
		Widget * next;
		std::vector<Widget *> sub; // subwidget
		// Box : properties
	    double radius; // for entire widget
		Vector4 color;
		bool fill;
		std::string type; // label(1), image(2), box(0)
		int old_width, old_height;
		int max_width, max_height;
		bool iconified;
		bool maximized;
		bool restored;
		bool dragged;
		std::string alignment;
		Vector4 forbidden_area;
		// Box : contents
		Label * label; // default label
		Image * image; // default image
		std::vector<GUI *> box; // can store lists
        // NEW! 2019-07-27. Widgets can now have multiple labels and images. The default label and image will be push_back to 0 index.
        std::vector<Label *> label_list;
        std::vector<Image *> image_list;
		// Box : title_bar
		bool title_bar;
		int title_bar_height;
		Vector4 title_bar_color;// = Vector4(47, 79, 79, 255);
		Label * title_bar_label;//std::string title_bar_text;
		Vector4 title_bar_text_color;
		Image * title_bar_image;// = nullptr;
		//double title_bar_radius;
		bool title_bar_button_iconify;
		bool title_bar_button_maximize;
		bool title_bar_button_close; Vector4 title_bar_button_close_color; Image * close_button_image;
		int title_bar_button_width;
		// Box : outline
		bool outline;
		double outline_width;
		Vector4 outline_color;
		bool outline_antialiased;
		// Box : border
		bool border;
		Vector4 border_color;// = Vector4();
		Vector2 border_size;// = Vector2(1, 1);
		int border_thickness;// = 0;
		int border_style;// = 0;
		double border_radius;//= 0;
		// Box : gradient
		bool gradient;
        bool gradient_mix; // if you want to mix color with gradient_color
        Vector4 gradient_color;		
		// Box : shadow
		bool shadow;
		// Box : highlight
	// highlight
	bool highlight;
	Vector4 highlight_color;	
};
#endif
/*
USAGE:
	Widget * widget = new Widget();
	widget->set_position(500, 500);
	widget->set_title_bar(true);
	widget->set_title_bar_size(20);
    widget->set_title_bar_button_close(true);
	widget->set_outline(true);
/////////////////////////////////////////////////
// The titlebar is a seperate object with its own position and size, but becomes one with the box .

// When drawing a box with a titlebar at position(0, 0), be sure to include the box's titlebar's position as well or the main window's titlebar will overlap (hide) the box's titlebar	
    
    widget->set_position(0, 0);

// So, to show the box's titlebar, you must include the titlebar's height like this:

    widget->set_position(0, 0 + widget->get_title_bar_size().y);

// But then the widget's position will be incorrect (not (0, 0))  even though the titlebar is part of the box. Therefore, is an extension of the box
    
    std::cout << "Box position: (" << widget->get_position() << ")" << std::endl;

// This (code above) will give you a box position of (0, 16), with "16" being the title_bar_height. The box's position is not (0, 0) as the user requested :(
// Hold up! if box has a titlebar, why not set the titlebar's position in place of the box's position .
// IMPOSSIBLE! WHY?: The titlebar position cannot be changed as it is set to correlate with the box's position. Also the titlebar will overlap the content inside of box
// NOTE: The box's width and x_position are ALWAYS equal to the box's width and x_position. So no need to worry about that part.
// The titlebar is a seperate entity so no need to worry about it. This is how it is by design. And there is no need to "correct" it in anyway.
/////////////////////////////////////////////////
// x cross			
				double angle   = 45;
			Renderer::draw_line(250, 250, 2, 10, angle, 1, 1, 
			    255, 255, 255);	
			Renderer::draw_line(250 , 250 + 2, 2, 10, -angle, 1, 1, 
			    255, 255, 255);
         // Minimize button
			Renderer::draw_line(250, 250, 2, 10, 90, 1, 1, 
			    255, 255, 255);						
*/
			//Renderer::draw_line(toolbar->get_x()+5/*left_padding*/, (toolbar->get_y()-2/*top_padding*/), 5, 10, 90, 1, 1, 
			//    32, 32, 32);	
			//Renderer::draw_line(toolbar->get_x()+5/*left_padding*/, (toolbar->get_y()-2/*top_padding*/) + (toolbar->get_height() - 10 * 2), 5, 10, 90, 1, 1, 
			//    32, 32, 32);	
			//Renderer::draw_line(toolbar->get_x()+5/*left_padding*/, (toolbar->get_y()-2/*top_padding*/) + (toolbar->get_height() - 10), 5, 10, 90, 1, 1, 
			//    32, 32, 32);	
#endif
