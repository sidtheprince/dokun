#include "../include/widget.h"

Widget::Widget() : radius(10.0), color(64, 64, 64, 255), fill(true), image (nullptr), type("box"),
iconified(false),
maximized(false),
restored(true),
max_width(400), 
max_height(300),
old_width(200),
old_height(150),
alignment("left"), // left is default
forbidden_area(0, 0, 0, 0), // forbidden area
// title bar
title_bar(false), 
title_bar_height(16),
title_bar_color(0, 51, 102, 255),
title_bar_button_iconify(false),
title_bar_button_maximize(false),
title_bar_button_close(true),
title_bar_button_width(10),
title_bar_image(nullptr), // if you operate on this and do not assign it to nullptr or any value then the engine will crash (on Linux especially)
title_bar_label(nullptr), // if you operate on this and do not assign it to nullptr or any value then the engine will crash (on Linux especially)
// outline
outline(false),
outline_width(2.0),
outline_color(0, 51, 102, 255),
outline_antialiased(false),
// border
border(false),
border_size(1, 1),
border_thickness(0),
border_style(0),
border_radius(10.0),
border_color(47, 79, 79, 255),
// gradient
gradient(false),
gradient_color(64, 64, 64, 255),
gradient_mix(true),
// shadow
shadow(false),
// highlight - used when hovered over ui
highlight(true),
highlight_color(0, 51, 102, 255),
// other
previous(nullptr),
next(nullptr)
{
	set_position(0, 0);
	set_size(200, 150);
	set_orientation(0);
	set_label(*new Label()); // create a(n)(initialized) label for widget (You will no longer need to check whether label is nullptr)
}
/////////////
Widget::Widget(const Widget& widget)
{
	set_position(widget.get_position());
	set_size(widget.get_size());
	set_orientation(widget.get_orientation());
	//set_type (widget.get_type());
	set_color(widget.get_color());
	set_label(*widget.get_label());
	set_image(*widget.get_image());
	//set_(widget.get_());
	set_title_bar(widget.has_title_bar());
	set_title_bar_color(widget.title_bar_color);
	set_title_bar_size(widget.title_bar_height);
	set_title_bar_icon(*widget.title_bar_image);
	set_title_bar_text(widget.title_bar_label->get_string());
	set_title_bar_text_color(widget.title_bar_text_color);
	set_title_bar_button_iconify(widget.title_bar_button_iconify);
	set_title_bar_button_maximize(widget.title_bar_button_maximize);
	set_title_bar_button_close(widget.title_bar_button_close);
	// outline
	set_outline(widget.has_outline());
	set_outline_width(widget.outline_width);
	set_outline_color(widget.outline_color);
	set_outline_antialiased(widget.outline_antialiased);
	// border
	set_border(widget.has_border());
	// shadow
	set_shadow(widget.has_shadow());
	// gradient
	set_gradient(widget.has_gradient());
	//set_(widget.has_());
}
/////////////
Widget::Widget(const std::string& type)
{
	this->type = type;
    set_position(0, 0);
	set_size(200, 150);
	set_orientation(0);		
	set_label(*new Label());
}
/////////////
Widget::Widget(int x, int y)
{
    set_position(x, y);
	set_size(200, 150);
	set_orientation(0);
	set_label(*new Label());
}
/////////////
Widget::Widget(int x, int y, int width, int height)
{
    set_position(x, y);
	set_width  (width);
	set_height(height);
	set_orientation(0);
	set_label(*new Label());
}
/////////////
Widget::~Widget(void)
{
	//if(label) delete label; // call label destructor
	//if(image) delete image; // call image destructor
}
/////////////
int Widget::widget_new(lua_State *L)
{
    lua_settop(L, 0); // clear any value pushed on top
	// create table (object)
	lua_createtable(L, 0, 0);
    // set mt
	lua_getglobal(L, "Widget");
	lua_setmetatable(L, 1);
	// set userdata
	Widget ** widget = static_cast<Widget**>(lua_newuserdata(L, sizeof(Widget*)));
	* widget = new Widget();
    lua_setfield(L, 1, "udata");
	// return widget
	if(lua_istable(L, -1))
	    return 1;
	lua_pushnil(L); // if not table
	    return 1; // return nil
}
/////////////
void Widget::add(const GUI& gui) // for box widgets only
{
	const_cast<GUI&>(gui).set_parent(*this);
}
/////////////
int Widget::add(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
			Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
			widget->add(*gui);
	    }
	}		
	return 0;
}
/////////////
void Widget::draw(void) 
{
	if(is_box()) // draw box
	{
		if(!is_active()) {} // add shade to color
		if(is_active())  {} // add tint to color
        if(is_visible())  // is it visible? 
        {
			on_titlebar();
            on_drag();
			on_resize();
			// Close button mouse hover
			/*if(Mouse::is_over(get_title_bar_button_close_position(), get_title_bar_button_close_size())) title_bar_button_close_color = Vector4(255, 51, 51, 255); // 227,38,54
			else title_bar_button_close_color = color;*/
            /////////////////////////////////////////////////////////////////////////
            // Draw box	- at (0, 0) the titlebar will not show as it is the main widget at 0, 0 so the title bar would be hidden by the window's titlebar		
			Renderer::draw_box(get_position().x, get_position().y, get_width(), get_height(), get_angle(), 
			get_scale().x, get_scale().y, color.x, color.y, color.z, color.w,
			    radius, is_iconified(),
				// title_bar
				title_bar, title_bar_height, title_bar_color,
				title_bar_button_iconify, 
				title_bar_button_maximize,
				title_bar_button_close, title_bar_button_close_color,
				// outline
				outline, outline_width, outline_color, outline_antialiased,
				// border
				border, border_radius, border_color,
				// gradient
				gradient, gradient_color,
				// shadow
				shadow
			);
            /////////////////////////////////////////////////////////////////////////
			// Draw titlebar image and text (goes on top of titlebar) *****************************
			if(has_title_bar()) // if GUI has a titlebar
			{   // title_bar image or icon (goes on the titlebar)
				if(title_bar_image) // if title_bar_image is not nullptr
				{
					if(title_bar_image->get_alignment() == "left"  ) {title_bar_image->set_relative_position(0, 0);} // relative: child_position = parent_position + relative_position
                    if(title_bar_image->get_alignment() == "center") {title_bar_image->set_relative_position((get_title_bar_size().x - get_title_bar_size().y) / 2, 0);}			
                    if(title_bar_image->get_alignment() == "right" ) {title_bar_image->set_relative_position(get_title_bar_size().x - get_title_bar_size().y, 0);}								
					if(title_bar_image->get_alignment() == "none"  ) {}
                    // set title_bar_image position relative to title_bar
				    title_bar_image->set_position(get_title_bar_position().x + title_bar_image->get_relative_x(), get_title_bar_position().y + title_bar_image->get_relative_y());
				    // scale image to fit the title_bar - ???
                    title_bar_image->scale_to_fit(get_title_bar_size().x, get_title_bar_size().y);
                    // and finally, draw the title_bar_image
				    title_bar_image->draw();// will crash engine if not assigned to nullptr or any value
				}
                // title_bar text (goes on the titlebar)
                if(title_bar_label) // the code inside this "if" statement will crash engine if "title_bar_label" is null
                {
                    if(!title_bar_label->get_string().empty()) {// be sure title_bar_label string is not empty 
			        if(title_bar_label->get_alignment() == "left"  ) {title_bar_label->set_position(0, 0);} // default
                    if(title_bar_label->get_alignment() == "center") {title_bar_label->set_position((get_title_bar_size().x - get_title_bar_size().y) / 2, 0);}			
                    if(title_bar_label->get_alignment() == "right" ) {title_bar_label->set_position(get_title_bar_size().x - get_title_bar_size().y, 0);}
                    if(title_bar_label->get_alignment() == "none"  ) {}
                    // set title_bar_label position to title_bar_position + title_bar_label_position
					title_bar_label->set_position(get_title_bar_position().x + title_bar_label->get_x(), get_title_bar_position().y + title_bar_label->get_y());
				    // scale label to fit titlebar
                    //double aspect = title_bar_label->get_aspect_ratio_correction(get_title_bar_size().x, get_title_bar_size().y);
					//title_bar_label->set_scale(aspect, aspect); // find a way to scale to fit
                    // and finally, draw the title_bar_label
                    title_bar_label->draw(); // title_bar_label's parent is not the Box but the Box's titlebar, so use set_position NOT set_relative_position // will crash engine if not assigned to nullptr or any value
                    }
                }
			} // end of titlebar image *********************************************
            /////////////////////////////////////////////////////////////////////////
			// Draw label (goes inside box) **********************************************************
            if(label) {// make sure Widget has a (initialized) label first (regardless of whether it has a default label or a set label)
		        if(label->get_alignment() == "left"  ) { label->set_relative_position(0                                    , 0); } // default - relative_position will always be (0, 0) unless you change the alignment
				if(label->get_alignment() == "center") { label->set_relative_position((get_width()-label->get_width())/2   , (get_height()-label->get_height())/2); }						
				if(label->get_alignment() == "right" ) { label->set_relative_position(get_width() - label->get_width()     , 0); }	
                if(label->get_alignment() == "none"  ) {} // with this you are free to set the label's relative position to whatever you want         
                label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // set actual position
			#ifdef DOKUN_DEBUG1		
				std::cout << "Label0 position:                 " << label->get_position() << std::endl;
				std::cout << "Label0 position relative to box: " << label->get_relative_position() << std::endl;
			#endif		
				// NO need to draw label since child GUI are automatically drawn //if(!label->get_string().empty()) label->draw(); // child objects are drawn automatically. So DO NOT call draw function!!!
            }
             // Draw multiple labels ... 
            for(int l = 0; l < label_list.size(); l++)//for(int l = 1; l <= label_list.size()-1; l++)
            {
                label_list[0]->set_position(label->get_x(), (label->get_y() + label->get_height()) + 2);//if(label ) label_list[0]->set_position(label->get_x(), (label->get_y() + label->get_height()) + 2);
                //if(!label) label_list[0]->set_position(get_x() + label_list[0]->get_relative_x(), get_y() + label_list[0]->get_relative_y());
                if(l != 0) { // label_list[0] is the default label, so exclude it
                    Label * prev = label_list[l-1];                          
                    label_list[l]->set_position(prev->get_x(), (prev->get_y() + prev->get_height())); // set the sub labels position based on label0's position. Don't worry about alignments and whatnot       
                }
                // NO need to draw label since child GUI are automatically drawn //if(!label_list[l]->get_string().empty()) label_list[l]->draw(); // draw the label (ONLY if the string is not empty!)
            }
            /////////////////////////////////////////////////////////////////////////
			// Draw image (goes inside box) *******************************************
            if(image != nullptr)
			{
				// get image size whether its original or resized or scaled
				int image_width  = image->get_width ();
			    int image_height = image->get_height();
                // set image alignment relative to GUI	
				if(image->get_alignment() == "left"  ) {image->set_relative_position(0, 0);}					
                if(image->get_alignment() == "center") {image->set_relative_position((get_width() - image_width) / 2, (get_height() - image_height) / 2);}
				if(image->get_alignment() == "right" ) {image->set_relative_position(get_width() - image_width, 0);}
                if(image->get_alignment() == "none"  ) {}
                // set image position relative to GUI
				image->set_position(get_x() + image->get_relative_x(), get_y() + image->get_relative_y());
				// make sure image is within widget bounds
				if(image_width > get_width  ()) image->resize(get_width(), image->get_height());// if image is wider than widget, make width equal
				if(image_height > get_height()) image->resize(image->get_width(), get_height());// if image is taller than widget, make height equal
				// and finally, draw the image ...	
				image->draw(); // Image is not a GUI so you cannot set its parent and it must be drawn manually
			}
			/////////////////////////////////////////////////////////////////////////
		}
	}
	//-------------------------------
	if(is_icon()) //If this widget is an icon // draw image (only) // Widgets can be both a box or an icon
	{
		if(!is_active()) // add shade to color
		{}
		if(is_active())  // add tint to color
		{}
		if(is_visible())
		{
			if(image != nullptr)
			{
				image->set_position(get_x() + image->get_relative_x(), get_y() + image->get_relative_y());
                image->resize(get_size());
				image->draw();				
			}
		}
	}	
    // on draw callback
	on_draw();	
}
/////////////
int Widget::draw(lua_State *L)  
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->draw();
	}
	return 0;
}
/////////////
void Widget::maximize(void)
{
    if(!is_maximized() /*&& !is_iconified()*/) // if not maximized yet
	{
	    old_width  = 200;//get_width (); // save old size
        old_height = 150;//get_height();		

		set_size(max_width, max_height); // set new size
	    maximized = true;	             // maximized is true
		iconified = false;
		restored = false;                // not restored to default size
	}
}
int Widget::maximize(lua_State * L)
{
    return 0;
}
/////////////
void Widget::iconify(void) // if maximized, keep maximized while iconified at the same time
{
	if(!is_iconified()) { // if not yet iconified (keep current size, whether maximized or minimized)
	    // hide content
	    if(image) image->set_visible(false);
	    if(label) label->set_visible(false);
	    // set iconify to true
	    iconified = true;
		restored = false; // not restored to default size
	}
}
int Widget::iconify(lua_State * L)
{
    return 0;
}
/////////////
void Widget::restore(void)
{
	//********************************
	if(!restored) { // if maximized
	    if(is_iconified()) { // if iconified just put it down dont mess with sizes
	        // show content
	        if(image) image->set_visible(true);
	        if(label) label->set_visible(true);
	        iconified = false; // set iconified to false in order to restore
	        if(is_maximized()) {return;} // restore but in maximized mode
	    }
		if(is_maximized()) {
	        set_size(old_width, old_height); //  restore original size
		    maximized = false;
	        restored  = true; // set restored to true
		}	
	}
}
int Widget::restore(lua_State * L)
{
    return 0;
}
/////////////
/* // deallocating box
// delete all labels
for(l = 0; l < label_list.size(); l++) {
    Label * label = label_list[l];
    delete label;
}
// delete all images
for(i = 0; i < image_list.size(); i++) {
    Image * image = image_list[i];
    delete image;
}
*/
/////////////
/////////////
// SETTERS
/////////////
/////////////
// setters
void Widget::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
void Widget::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
void Widget::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}		
int Widget::set_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 255);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;
}
void Widget::set_alpha(double alpha)
{
	color.w = alpha;
}                                                  
int Widget::set_alpha(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_alpha(lua_tonumber(L, 2)); // lua_tonumber(L, ), 
	}
	return 0;	
}
void Widget::set_fill(bool fill)
{
	this->fill = fill;
}                                                     
int Widget::set_fill(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_fill(lua_toboolean(L, 2));
	}
	return 0;	
}
// outline - is line drawn around border?
void Widget::set_outline(bool outline)
{
	this->outline = outline;
}                                                
int Widget::set_outline(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_outline(lua_toboolean(L, 2));
	}
	return 0;	
}
void Widget::set_outline_width(double width)
{
	outline_width = width;
}                                                    
int Widget::set_outline_width(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_outline_width(lua_tonumber(L, 2));
	}
	return 0;	
}
void Widget::set_outline_color(int red, int green, int blue, int alpha)
{
	outline_color = Vector4(red, green, blue, alpha);
}                        
void Widget::set_outline_color(const Vector3& color)
{
	set_outline_color(color.x, color.y, color.z);
}
void Widget::set_outline_color(const Vector4& color)
{
	set_outline_color(color.x, color.y, color.z, color.w);
}
int Widget::set_outline_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 255);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_outline_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
void Widget::set_outline_style(int style)
{}                                           
int Widget::set_outline_style(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_outline_style(lua_tonumber(L, 2));
	}
	return 0;	
}
void Widget::set_outline_antialiased(bool antialiased)
{
	outline_antialiased = antialiased;
}		                           
int Widget::set_outline_antialiased(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_outline_antialiased(lua_toboolean(L, 2));
	}
	return 0;	
}
// border - does it have a border(frame)?
void Widget::set_border(bool border)
{
	this->border = border;
}                             
int Widget::set_border(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_border(lua_toboolean(L , 2));
	}
	return 0;	
}
void Widget::set_border_size(int width, int height)
{}                                
int Widget::set_border_size(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_border_size(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;	
}
void Widget::set_border_width(int width, int side)
{}                               
int Widget::set_border_width(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_border_width(lua_tonumber(L, 2));
	}
	return 0;	
}
void Widget::set_border_color(int red, int green, int blue, int alpha)
{}                          
int Widget::set_border_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 255);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_border_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
void Widget::set_border_color(const Vector3& color)
{}
void Widget::set_border_color(const Vector4& color)
{}
void Widget::set_border_style(int style, int side)
{}                               
int Widget::set_border_style(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_border_style(lua_tonumber(L, 2));
	}
	return 0;	
}
void Widget::set_border_radius(int radius, int side)
{}                              
int Widget::set_border_radius(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_border_radius(lua_tonumber(L, 2));
	}
	return 0;	
}
// radius - how round are the edges?
void Widget::set_radius(double radius, int side)
{
	this->radius = radius;
}                                      
int Widget::set_radius(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_radius(lua_tonumber(L, 2));
	}
	return 0;	
}
void Widget::set_trim(double trim, int side)
{}                                   
int Widget::set_trim(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_trim(lua_tonumber(L, 2));
	}
	return 0;	
}
// shadow -  does it have a shadow around it?
void Widget::set_shadow(bool shadow)
{
	this->shadow = shadow;
}                                         
int Widget::set_shadow(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_shadow(lua_toboolean(L, 2));
	}
	return 0;	
}
// gradient - do the colors mix?
void Widget::set_gradient(bool gradient)
{
	this->gradient = gradient;
}                                         
int Widget::set_gradient(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_gradient(lua_toboolean(L, 2));
	}
	return 0;	
}
void Widget::set_gradient_color(int red, int green, int blue, int alpha, bool mix)
{
	gradient_color = Vector4(red, green, blue, alpha);
	gradient_mix   = mix;
}
void Widget::set_gradient_color(const Vector3& color, bool mix)
{
	set_gradient_color(color.x, color.y, color.z);
}
void Widget::set_gradient_color(const Vector4& color, bool mix)
{
	set_gradient_color(color.x, color.y, color.z, color.w);
}
void Widget::set_gradient_color(const Vector3& color0, const Vector3& color1)
{
	color = Vector4(color0, color.w);
	gradient_color = Vector4(color1, gradient_color.w);
}
void Widget::set_gradient_color(const Vector4& color0, const Vector4& color1)
{
	color = color0;
	gradient_color = color1;
}

void Widget::set_forbidden_area(int x, int y, int width, int height) // set forbidden_area within the gui (relative to the gui)
{
	forbidden_area = Vector4(x, y, width, height);
}
void Widget::set_forbidden_area(const Vector4& rect)
{
    set_forbidden_area(rect.x, rect.y, rect.z, rect.w);
}
// title_bar - does it have a title_bar
void Widget::set_title_bar(bool title_bar)
{
	this->title_bar = title_bar;
}                                      
int Widget::set_title_bar(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar(lua_toboolean(L, 2));
	}
	return 0;	
}
void Widget::set_title_bar_size(int height)
{
	title_bar_height = height;
}
int Widget::set_title_bar_size(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar_size(lua_tonumber(L, 2)); // lua_tonumber(L, ), 
	}
	return 0;	
}
void Widget::set_title_bar_text(const std::string& text)
{
	if(!title_bar_label) {
#ifdef DOKUN_DEBUG0
		Logger("title_bar_label is null | Widget::set_title_bar_text");
#endif
		return;
	}
	title_bar_label->set_string(text);
}                                    
int Widget::set_title_bar_text(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar_text(lua_tostring(L, 2)); // lua_tostring(L, )
	}
	return 0;	
}
/////////////
void Widget::set_title_bar_text_color(int red, int green, int blue, int alpha)
{
	if(!title_bar_label) {
		Logger("title_bar_label is null | Widget::set_title_bar_text_color");
		return;
	}	
	title_bar_label->set_color(red, green, blue, alpha);
}  
/////////////
void Widget::set_title_bar_text_color(const Vector3& color)
{
	set_title_bar_text_color(color.x, color.y, color.z);
}
/////////////
void Widget::set_title_bar_text_color(const Vector4& color)
{
	set_title_bar_text_color(color.x, color.y, color.z, color.w);
}
/////////////
int Widget::set_title_bar_text_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 255);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar_text_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
//////////////
void Widget::set_title_bar_label(const Label& label)
{
	title_bar_label = &const_cast<Label&>(label);
}
int Widget::set_title_bar_label(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
            widget->set_title_bar_label(* label);
           // set in Lua as well
           lua_pushvalue(L, 2);
           lua_setfield(L, 1, "title_bar_label");
        }
    }
    return 0;
}
//////////////
void Widget::set_title_bar_image(const Image& image)
{
	title_bar_image = &const_cast<Image&>(image);
}                                  
int Widget::set_title_bar_image(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		    widget->set_title_bar_icon(*image);
           // set in Lua as well
           lua_pushvalue(L, 2);
           lua_setfield(L, 1, "title_bar_image");
	    }
	}
	return 0;	
}
void Widget::set_title_bar_icon(const Image& icon)
{
	set_title_bar_image(icon);
}
void Widget::set_title_bar_color(int red, int green, int blue, int alpha)
{
	title_bar_color = Vector4(red, green, blue, alpha);
}
void Widget::set_title_bar_color(const Vector3& color)
{
	set_title_bar_color(color.x, color.y, color.z);
}
void Widget::set_title_bar_color(const Vector4& color)
{
	set_title_bar_color(color.x, color.y, color.z, color.w);
}
int Widget::set_title_bar_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 255);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;	
}
void Widget::set_title_bar_button_iconify(bool button_iconify)
{
	this->title_bar_button_iconify = button_iconify;
}                     
int Widget::set_title_bar_button_iconify(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar_button_iconify(lua_toboolean(L, 2));
	}
	return 0;	
}
void Widget::set_title_bar_button_maximize(bool button_maximize)
{
	this->title_bar_button_maximize = button_maximize;
}                    
int Widget::set_title_bar_button_maximize(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar_button_maximize(lua_toboolean(L, 2));
	}
	return 0;	
}
void Widget::set_title_bar_button_close(bool button_close)
{
	this->title_bar_button_close = button_close;
}                       
int Widget::set_title_bar_button_close(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_title_bar_button_close(lua_toboolean(L, 2));
	}
	return 0;	
}
/////////////
/////////////
// icon
void Widget::set_as_icon(bool icon)
{
	type = (icon == true) ? "icon" : "box";
}
void Widget::set_image(const Image& image) // images are not GUIs so they do not require a parent
{
	this->image = &const_cast<Image&>(image); // add image to box 
}
int Widget::set_image(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		    widget->set_image(*image);
			// set in Lua as well
			lua_pushvalue(L, 2);
			lua_setfield(L, 1, "image");
	    }
	}
	return 0;	
}
void Widget::set_image_list(const Image& image)
{
    image_list.push_back(&const_cast<Image&>(image)); // store image in image_list (first image set will ALWAYS be in index 0)
}
int Widget::set_image_list(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		    widget->set_image(*image);
			// set in Lua as well
            lua_pushvalue(L, 2);
			lua_setfield(L, 1, std::string( std::string("image") + std::to_string((int)widget->image_list.size()) ).c_str());
	    }
	}
	return 0;
}
/////////////
/////////////
// label		
void Widget::set_text(const std::string& text)
{
	get_label()->set_string(text);
}
int Widget::set_text(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		widget->set_text(lua_tostring(L, 2));	
	}
	return 0;
}
/////////////
void Widget::set_label(const Label& label) // Labels are GUI elements so they are drawn automatically once a parent is set
{
	this->label = &const_cast<Label&>(label); // add label to box 
	this->label->set_parent(* this); // set parent to Widget
} 
int Widget::set_label(lua_State * L)	
{
	luaL_checktype(L, 1, LUA_TTABLE); // box
	luaL_checktype(L, 2, LUA_TTABLE); // label
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
        lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		    widget->set_label(*label);
			// set in Lua as well
			lua_pushvalue(L, 2); // label
			lua_setfield(L, 1, "label"); // box.label = label
			// set Box as parent for Label
			lua_pushvalue(L, 1); // box
			lua_setfield(L, 2, "parent"); // label.parent = box
	    }
	}
    return 0;	
}	
void Widget::set_label_list(const Label& label) // Labels are GUI elements so they are drawn automatically once a parent is set
{
    label_list.push_back(&const_cast<Label&>(label)); // store label in label_list (first label set will always be in index 0)
    label_list[label_list.size()-1]->set_parent(* this); // set Box as parent - C++ indexes start from 0
}
int Widget::set_label_list(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // box
	luaL_checktype(L, 2, LUA_TTABLE); // label
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
        lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		    widget->set_label_list(*label);
            // set in Lua
            lua_pushvalue(L, 2);
            lua_setfield(L, 1, std::string( std::string("label") + std::to_string((int)widget->label_list.size()) ).c_str()); // box.label1, box.label2, box.label3, and so on ...        // lua_getfield(L, 1, "label_list");if(!lua_istable(L, -1)){lua_newtable(L); lua_setfield(L, 1, "label_list");} // check is box.label_list exists. If not create it
        }
    }    
    return 0;
}
/////////////
/////////////
void Widget::set_alignment(const std::string& alignment)
{
	this->alignment = alignment;
}
int Widget::set_alignment(lua_State * L)
{
    return 0;
}
/////////////
/////////////
// GETTERS
/////////////
/////////////
Vector4 Widget::get_color() const
{
	return color;
} 
int Widget::get_color(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // widget
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Widget * widget = *static_cast<Widget **>(lua_touserdata(L, -1));
		lua_pushboolean(L, widget->get_color().x);
		lua_pushboolean(L, widget->get_color().y);
		lua_pushboolean(L, widget->get_color().z);
		lua_pushboolean(L, widget->get_color().w);
		return 4;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;
}
//////////////
Image * Widget::get_image() const
{
	return image;
}
int Widget::get_image(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "image");
	return 1;
}
Image * Widget::get_image_list(int index) const
{
	return image_list[index];
}
int Widget::get_image_list(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // box (arg:1)
    luaL_checktype(L, 2, LUA_TNUMBER); // index (arg:2)
    lua_getfield(L, 1, "image_list"); // image_list (arg:3)
    if(lua_istable(L, -1))
    {
	    lua_rawgeti(L, -1, lua_tointeger(L, 2));
        return 1;
    }
    lua_pushnil(L);
	return 1;	
}
/////////////
Label * Widget::get_label() const
{
	return label;
}
int Widget::get_label(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "label");
	return 1;	
}
Label * Widget::get_label_list(int index) const
{
	return label_list[index];
}
int Widget::get_label_list(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // box (arg:1)
    luaL_checktype(L, 2, LUA_TNUMBER); // index (arg:2)
    lua_getfield(L, 1, "label_list"); // label_list (arg:3)
    if(lua_istable(L, -1))
    {
	    lua_rawgeti(L, -1, lua_tointeger(L, 2));
        return 1;
    }
    lua_pushnil(L);
	return 1;	
}
/////////////
std::string Widget::get_text() const
{
	return get_label()->get_string();
}
/////////////
Vector2 Widget::get_title_bar_position() const
{
	double x = get_x();
	double y = get_y() + (-title_bar_height);
	return Vector2(x, y);
}
Vector2 Widget::get_title_bar_size() const
{
	return Vector2(get_width(), title_bar_height);
}
/////////////
Vector4 Widget::get_title_bar_color() const
{
	return title_bar_color;
}
/////////////
std::string Widget::get_title_bar_text() const
{
	if(!title_bar_label) // title_bar_label is null
		return "";       // return empty string
	return title_bar_label->get_string();//title_bar_text;
}
/////////////
Vector4 Widget::get_title_bar_text_color() const
{
	if(!title_bar_label) {// title_bar_label is null
	    return Vector4(255, 255, 255, 255);
	}
	return title_bar_label->get_color();
}
/////////////
Image * Widget::get_title_bar_icon() const
{
	return title_bar_image;
}
/////////////
/////////////
Vector2 Widget::get_title_bar_button_iconify_position()const
{
	double x = get_title_bar_position().x + (get_title_bar_size().x - (title_bar_button_width * 3)) - 3;
	double y = get_title_bar_position().y + 2;
	return Vector2(x, y);	
}
Vector2 Widget::get_title_bar_button_iconify_size()const
{
	return Vector2(title_bar_button_width, title_bar_height - 5);	
}
/////////////
Vector2 Widget::get_title_bar_button_maximize_position()const
{
	double x = get_title_bar_position().x + (get_title_bar_size().x - (title_bar_button_width * 2)) - 2;
	double y = get_title_bar_position().y + 2;
	return Vector2(x, y);	
}
Vector2 Widget::get_title_bar_button_maximize_size()const
{
	return Vector2(title_bar_button_width, title_bar_height - 5);	
}
/////////////
Vector2 Widget::get_title_bar_button_close_position()const
{
	double x = get_title_bar_position().x + (get_title_bar_size().x - title_bar_button_width) - 1;
	double y = get_title_bar_position().y + 2;
	return Vector2(x, y);
}
Vector2 Widget::get_title_bar_button_close_size()const
{
	return Vector2(title_bar_button_width, title_bar_height - 5);
}
Vector4 Widget::get_title_bar_button_close_color()const
{
	return title_bar_button_close_color;
}
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
double Widget::get_radius() const
{
	return radius;
}
int Widget::get_radius(lua_State * L)
{
	return 1;
}
/////////////
/////////////
std::string Widget::get_alignment()const
{
	return alignment;
}
int Widget::get_alignment(lua_State * L)
{
	return 1;
}
/////////////
/////////////
// BOOLEAN
/////////////
bool Widget::is_box  ()const
{
	return (String::lower(type) == "box");
} 
int Widget::is_box(lua_State * L)
{
	return 1;
}
bool Widget::is_icon ()const
{
	return (String::lower(type) == "icon");
}
int Widget::is_icon(lua_State * L)
{
	return 1;
}
bool Widget::has_title_bar()const
{
	return (title_bar == true);
} 
int Widget::has_title_bar(lua_State * L)
{
	return 1;
}
bool Widget::has_outline()const
{
	return (outline == true);
}   
int Widget::has_outline(lua_State * L)
{
	return 1;
}
bool Widget::has_border()const
{
	return (border == true);
} 
int Widget::has_border(lua_State * L)
{
	return 1;
}
bool Widget::has_shadow()const
{
	return (shadow == true);
}
int Widget::has_shadow(lua_State * L)
{
	return 1;
}
bool Widget::has_gradient()const
{
	return (gradient == true);
}  
int Widget::has_gradient(lua_State * L)
{
	return 1;
}
/////////////
bool Widget::is_maximized()const
{
	return (maximized == true);
}
int Widget::is_maximized(lua_State * L)
{
	return 1;
}
bool Widget::is_iconified()const
{
	return (iconified == true);
}
int Widget::is_iconified(lua_State * L)
{
	return 1;
}
bool Widget::is_restored()const
{
	return (restored == true);
}
int Widget::is_restored(lua_State * L)
{
	return 1;
}
/////////////
/////////////
void Widget::on_titlebar(void)
{
	if(!has_title_bar())
	    return;
	if(title_bar_button_iconify)
	{
		if(Mouse::is_over(get_title_bar_button_iconify_position(), get_title_bar_button_iconify_size()))
		{
			if(Mouse::is_pressed(1)) // ??
			{
				iconify();
			}
		}
	}
	if(title_bar_button_maximize)
	{
		if(Mouse::is_over(get_title_bar_button_maximize_position(), get_title_bar_button_maximize_size()))
		{
			if(Mouse::is_pressed(1))
			{
                if(!restored) restore(); else maximize();
			}
		}
	}
	if(title_bar_button_close)
	{
		if(Mouse::is_over(get_title_bar_button_close_position(), get_title_bar_button_close_size()))
		{
			if(Mouse::is_pressed(1)) set_visible(false);
		}
	}
}
/////////////
void Widget::on_drag(void)
{
	if(!is_draggable())
        return;
	WINDOW * window = WINDOW::get_active();
	if(window == nullptr)
		return;
	if(has_title_bar()) // has title_bar (drag by title_bar)
	{
		if(Mouse::is_over(get_title_bar_position(), get_title_bar_size()))
		{
			dragged = true; // can drag
			if(title_bar_button_close)
			{
			    if(Mouse::is_over(get_title_bar_button_close_position   (), get_title_bar_button_close_size   ()))
				    dragged = false;
		    }
		    if(title_bar_button_iconify)
			{
			    if(Mouse::is_over(get_title_bar_button_iconify_position (), get_title_bar_button_iconify_size ()))
				    dragged = false;
			}
            if(title_bar_button_maximize)
			{				
		        if(Mouse::is_over(get_title_bar_button_maximize_position(), get_title_bar_button_maximize_size()))
			        dragged = false;
		    }
		}
		if(Mouse::is_pressed(1) && dragged)
		{
			set_position(Mouse::get_position(*window).x - get_title_bar_size().x / 2, Mouse::get_position(*window).y + get_title_bar_size().y / 2);
		}
		if(Mouse::is_released(1)) // on release
		{
			dragged = false;
			set_position(get_position());
		}
	}
	if(!has_title_bar()) // no title_bar
	{
		if(Mouse::is_over(get_x(), get_y(), 
		    get_width() - forbidden_area.z, get_height() - forbidden_area.w))
		{
			dragged = true;
		}/*
		if(Mouse::is_over(forbidden_area))
		{
			dragged = false;
		}*/
		if(Mouse::is_pressed(1) && dragged)
		{
			set_position(Mouse::get_position(*window).x - get_size().x / 2,//Mouse::get_position(*window).x - (get_size().x + get_x()),
			    Mouse::get_position(*window).y - get_size().y / 2);
		}
		if(Mouse::is_released(1)) // on release
		{
			dragged = false;
			set_position(get_position());
		}
	}
}
void Widget::on_resize(void)
{
	if(!is_resizeable())
        return;
	WINDOW * window = WINDOW::get_active();
	if(window == nullptr)
		return;	
    if(Mouse::is_over(0,0)) // mouse over side edges
	{
		// change mouse cursor
		// WINDOWS CURSORS
		// IDC_SIZENESW , IDC_SIZENS , IDC_SIZENWSE , IDC_SIZEWE , 
		// LINUX CURSORS
		// XC_right_side , XC_bottom_right_corner , XC_top_right_corner
		// XC_left_side , XC_bottom_left_corner , XC_top_left_corner
		// XC_bottom_side
		// XC_top_side
	#ifdef __windows__
		HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
		SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)cursor);
	#endif
	#ifdef __gnu_linux__	
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_top_left_corner); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
		XFreeCursor(window->get_display(), cursor);
	#endif									
	} 
	// restore original cursor
	else {
	#ifdef __windows__
		HCURSOR cursor = LoadCursor(nullptr, IDC_ARROW);
		SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)cursor);
	#endif
	#ifdef __gnu_linux__	
		Cursor cursor = XCreateFontCursor(window->get_display(), XC_left_ptr); 
        XDefineCursor(window->get_display(), window->get_handle(), cursor);
		XFreeCursor(window->get_display(), cursor);
	#endif
	}	
}		
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
