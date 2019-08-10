#include "../include/edit.h"

Edit::Edit() : color(160, 160, 160, 255), character_limit(20),
    cursor(true), cursor_x(0), cursor_y(0), cursor_height(20), zoom_factor(0), multilined(false), num_newlines(0), label(nullptr)
{
	set_position(0, 0);
	set_size(200, 20); // 150, 20
	set_orientation(0);
}
/////////////
Edit::~Edit()
{}
/////////////
void Edit::append(const std::string& text)
{
	set_text(get_text() + text);
}
/////////////
int Edit::append(lua_State *L)
{
	return 0;
}
/////////////
void Edit::draw()
{
	if(is_visible())  // is it visible? 
	{
		if(is_active()) // is it disabled?
		{}
		// callbacks
		on_hover();// if mouse over edit, change mouse to I-beam
		//on_mousepress();// edit is pressed, set cursor at position_pressed
        on_keypress();
        on_backspace();
        on_enter();
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
        void * font = (get_label() ? get_label()->get_font()->get_data() : nullptr);		
		Vector4 text_color = get_text_color();
		// Draw edit
		Renderer::draw_edit(get_text(), x, y, width, height, angle, scale_x, scale_y,
		    red, green, blue, alpha, multilined, cursor, cursor_x, cursor_y, cursor_height);
        // Draw text
		if(label) // ->get_string().empty()
		{
		    label->set_relative_position(label->get_relative_position());
			label->set_position(get_x() + label->get_relative_x(), get_y() + label->get_relative_y()); // label position stays the same - on the edit, but the cursor is the only thing that moves
		}
        on_draw(); // draw children
	}
}
/////////////
int Edit::draw(lua_State *L)
{
	return 0;
}
/////////////
void Edit::cut() 
{}
/////////////
int Edit::cut(lua_State *L)
{
	return 0;
}
/////////////
void Edit::copy() 
{}
/////////////
int Edit::copy(lua_State *L)
{
	return 0;
}
/////////////
void Edit::paste()
{
#ifdef __windows__
	if(!OpenClipboard(nullptr)) // open clipboard
	{
		return;
	}
    append( (const char *)GetClipboardData(CF_TEXT) );
#endif
}
/////////////
int Edit::paste(lua_State *L)
{
	return 0;
}
/////////////
void Edit::undo() 
{}
/////////////
int Edit::undo(lua_State *L)
{
	return 0;
}
/////////////
void Edit::redo() 
{}
/////////////
int Edit::redo(lua_State *L)
{
	return 0;
}
/////////////
void Edit::select() 
{}
/////////////
int Edit::select(lua_State *L)// (highlights specific text) or all
{
	return 0;
}
/////////////
void select_all()
{} 
/////////////
int select_all(lua_State *L)
{
	return 0;
}
/////////////
void Edit::clear() 
{}
/////////////
int Edit::clear(lua_State *L)// (std::string selected_text) or all
{
	return 0;
}
/////////////
void Edit::clear_all() 
{
	get_label()->set_string("");
}
/////////////
int Edit::clear_all(lua_State *L)// (std::string selected_text) or all
{
	return 0;
}
/////////////
void Edit::zoom(int value) 
{}
/////////////
int Edit::zoom(lua_State *L)// -1 (zoom out), +1 (zoom in) 
{
	return 0;
}
/////////////
// SETTERS
/////////////
void Edit::set_color(int red, int green, int blue, int alpha) // base_color
{
	color = Vector4(red, green, blue, alpha);
}
/////////////
void Edit::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
/////////////
void Edit::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
/////////////
int Edit::set_color(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_cursor(bool cursor)
{
	(this)->cursor = cursor;
} 
/////////////
int Edit::set_cursor(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_cursor_x(double x)
{
    // cannot go past the width of the edit
	if(x >= get_width()) {cursor_x = get_width();return;}
	if(x <= 0)           {cursor_x = 0;return;          }
	cursor_x = x;	
}
/////////////
void Edit::set_cursor_y(double y)
{
	cursor_y = y;
}
/////////////
void Edit::set_cursor_position(double x, double y)
{
    set_cursor_x(x);
	set_cursor_y(y);
} 
/////////////
void Edit::set_cursor_position(const Vector2& position)
{
	set_cursor_position(position.x, position.y);
}
/////////////
/////////////
int Edit::set_cursor_position(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_label(const Label& label)
{
	this->label = &const_cast<Label&>(label);
	this->label->set_parent(* this);
}
/////////////
int Edit::set_label(lua_State *L)
{
	return 0;
}
/////////////
// TEXT-RELATED
/////////////
void Edit::set_text(const std::string& text)
{
	std::string string0 (text);
	if(string0.length() > get_character_limit()) // character limit has been reached
	{
		string0 = string0.erase(string0.length() - string0.length() + get_character_limit());
	#ifdef DOKUN_DEBUG
	    std::cout << "Cannot exceed " <<  get_character_limit() << " character limit!" << std::endl;
	#endif	
	    return; // exit function
	}
	int edit_capacity = get_width() / 10; // number of characters the edit can hold
	if(cursor_x >= get_width()) // cursor cannot go past edit
	{
	    // keep label from going past edit. Set character limit to 50
	    if(!is_multilined()) {
	        string0.resize(edit_capacity);
	    #ifdef DOKUN_DEBUG1    
	        std::cout << "Text Edit can only hold " << edit_capacity << " characters based on its size!" << std::endl;
	        std::cout << "Characters in text: " << get_text().size() << std::endl;
	    #endif
	    }
		if(is_multilined())  // if multilined=true, create a new line // a multilined editor
		{
	        char last_char = String::get_last_character(string0);
	        // remove last (51th) chatracter
	        string0 = string0.erase(string0.length() - 1);//std::cout << last_char << " removed\n";		
		
		    int char_height = label->get_font()->get_height();
		    // double the height of the edit
			set_height(get_height() + char_height); // GOOD!
			// add a newline in text (basically replaces last_char with a newline)
			string0.append("\n"); num_newlines = num_newlines + 1;
			// also append the last character
			string0.append(String::to_string(last_char)); // finally append last (51th) character to the newline
			// reset cursor x position - try to get location of newline
			set_cursor_x(0);
			// set cursor y position (bring it down)
			set_cursor_y(cursor_y + char_height);
	    }
	}		
	get_label()->set_string(string0);
}
/////////////
int Edit::set_text(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_font(const FONT& font)
{
	get_label()->set_font(font); // set label font (if already exist)
} 
/////////////
int Edit::set_font(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_color(int red, int green, int blue, int alpha)
{
	label->set_color(red, green, blue, alpha);
} 
/////////////
void Edit::set_text_color(const Vector3& color)
{
	set_text_color(color.x, color.y, color.z);
}
/////////////
void Edit::set_text_color(const Vector4& color)
{
	set_text_color(color.x, color.y, color.z, color.w);
}
/////////////
int Edit::set_text_color(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_size(int width, int height)
{
	get_label()->set_size(width, height);
}
/////////////
int Edit::set_text_size(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_style(int style)
{
	get_label()->set_style(style);
} 
/////////////
int Edit::set_text_style(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_text_background_color(int red, int green, int blue, int alpha)
{
	get_label()->set_background_color(red, green, blue, alpha);
}
/////////////
int Edit::set_text_background_color(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_character_limit(int limit)
{
	this->character_limit = limit;
}
/////////////
int Edit::set_maximum_character(lua_State *L)
{
	return 0;
}
/////////////
void Edit::set_multilined(bool multilined)
{
	this->multilined = multilined;
}
/////////////
/////////////
/////////////
/////////////
// GETTERS
/////////////
Label * Edit::get_label()const
{
	return label;
}    
/////////////
int Edit::get_label(lua_State *L)
{
	return 0;
}
/////////////
std::string Edit::get_text()const
{
	return get_label()->get_string();
}  
/////////////
int Edit::get_text(lua_State *L)
{
	return 0;
} 
/////////////
Vector4 Edit::get_color()const
{
	return color;
}
///////////// 
int Edit::get_color(lua_State *L)
{
	return 0;
}
/////////////
int Edit::get_cursor_x() const
{
	return cursor_x;
}
/////////////
int Edit::get_cursor_y() const
{
	return cursor_y;
}		
/////////////
Vector2 Edit::get_cursor_position()const
{
	return Vector2(cursor_x, cursor_y);
}  
/////////////
int Edit::get_cursor_position(lua_State *L)
{
	return 0;
}
/////////////
FONT * Edit::get_font()const
{
    return get_label()->get_font();
}
/////////////
int Edit::get_font(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Edit::get_text_color()const
{
	return get_label()->get_color();
}
/////////////
int Edit::get_text_color(lua_State *L)
{
	return 4;
}
/////////////
Vector2 Edit::get_text_size()const
{
	return get_label()->get_size();
}
/////////////
int Edit::get_text_size(lua_State *L)
{
	return 2;
}
/////////////
int Edit::get_text_style()const
{
	return get_label()->get_style();
}    
/////////////
int Edit::get_text_style(lua_State *L)
{
	return 1;
}
/////////////
Vector4 Edit::get_text_background_color()const
{	
	return get_label()->get_background_color();
} 
/////////////
int Edit::get_text_background_color(lua_State *L)
{
	return 4;
}
/////////////
int Edit::get_character_limit()const
{
	return character_limit;
}
/////////////
bool Edit::is_multilined() const
{
	return (multilined == true);
}
/////////////
/////////////
/////////////
void Edit::on_hover()
{
	if(Mouse::is_over(get_position(), get_size()))
	{
		// change mouse cursor to I-beam
		WINDOW * window = WINDOW::get_active();
		if(window != nullptr)
		{
		#ifdef __windows__
		    HCURSOR ibeam = LoadCursor(nullptr, IDC_IBEAM);
		    //SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)ibeam); // DWORD = unsigned long
        #endif		
		#ifdef __gnu_linux__		
		    Cursor cursor = XCreateFontCursor(window->get_display(), XC_xterm); 
            XDefineCursor(window->get_display(), window->get_handle(), cursor);
			XFreeCursor(window->get_display(), cursor);
		#endif
		}
	} else {   // revert back to original mouse cursor
		WINDOW * window = WINDOW::get_active();
		if(window != nullptr)
		{
	    #ifdef __windows__
		    HCURSOR arrow = LoadCursor(nullptr, IDC_ARROW);
		    //SetCursor(HCURSOR hCursor);//SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)arrow);
		#endif
		#ifdef __gnu_linux__	
		    Cursor cursor = XCreateFontCursor(window->get_display(), XC_left_ptr); 
            XDefineCursor(window->get_display(), window->get_handle(), cursor);
			XFreeCursor(window->get_display(), cursor);
		#endif	
		}
	}
}
/////////////
void Edit::on_mousepress()
{
    if(!Mouse::is_over(get_rect())) return; // mouse_not_over_edit, return
	if(Mouse::is_over(get_position(), (get_text().size() * 10)+10)) // mouse_over_text
	{
		if(Mouse::is_pressed(1))
		{
		    WINDOW * window = WINDOW::get_active();
		    if(window == nullptr) return;
		    int x = fabs(round((double)Mouse::get_position(*window).x - (double)(get_x() + cursor_x))); // abs() for int, fabs() for decimals//if(x < 0) x = -x; = makes negative numbers into a positive
		    set_cursor_position(x, cursor_y);//round to the nearest tenth//get_y() - Mouse::get_position(*window).y);//cursor_changed_by_mouse = true; // add chars to location of cursor : get_text().size() / cursor_x
		}
	}
}
/////////////
void Edit::on_keypress()
{
    if(Keyboard::is_down())
	{
		char key = static_cast<char>(Keyboard::key);
		if(Keyboard::is_pressed(key))
		{   
		#ifdef DOKUN_DEBUG0
		    std::cout << key << " pressed\n";
		#endif    
		    //if(isascii(key)) {  set_text(get_text() + std::to_string(int(key))); set_cursor_x(cursor_x + 10); return; }
			// number or letter or punctuation or space
			if(isalnum(key) ||  ispunct(key) || isspace(key))
			{
				set_text(get_text() + String::to_string(key)); //if(std::string(get_text()).length() > get_character_limit()) return; // exit if character limit has been reached	
				set_cursor_x(cursor_x + 10);//increment by 10 for each character//10 * get_text().size();// label_width does not give accurate results so DO NOT use!
				return;
			}
		}
	}
}
/////////////
void Edit::on_backspace()
{
    if(get_text().empty()) return; // empty string, return (so it does not crash engine)
#ifdef __windows__
    if(Keyboard::is_pressed(0x08))
#endif
#ifdef __gnu_linux__
    if(Keyboard::is_pressed(0xff08))
#endif
	{
	    char last_char = String::get_last_character(label->get_string()); // get last character in text (before erasing it) // make sure text is not empty so you can operate on it (to prevent crash) if(get_text().empty()) return;
		// erase last character in string
		if(get_text().length() > 0) set_text(get_text().erase(get_text().length() - 1));
		// bring cursor_x back on character deletion //if(!get_text().empty()) 
		set_cursor_x(cursor_x - 10);		
	#ifdef DOKUN_DEBUG0
		std::cout << "\"" << last_char << "\" deleted" << std::endl;
	#endif
		// if a newline is removed, decrease the size of the edit
		if(last_char == '\n' && multilined) {
		    int char_height = label->get_font()->get_height();
		    set_height(get_height() - char_height);
		    // decrease num_newlines
		    num_newlines = num_newlines - 1;
		    // bring cursor_y back up (if erasing newline)
		    set_cursor_y(cursor_y - char_height);
		}
		// reset cursor position if text is empty
		if(get_text ().empty()) set_cursor_position(0, 0);
	}	
}
/////////////
void Edit::on_enter()
{
#ifdef __windows__
    if(Keyboard::is_pressed(0x0D))
#endif	
#ifdef __gnu_linux__	
	if(Keyboard::is_pressed(0xff0d))
#endif		
	{
		if(is_multilined()) // a multilined editor
		{
		    int char_height = label->get_font()->get_height();
		    // double the height of the edit
			set_height(get_height() + char_height); // GOOD!
			// add a newline in text
			set_text(get_text() + "\n");
			num_newlines = num_newlines + 1;
			// reset cursor x position - try to get location of newline
			set_cursor_x(0);
			// set cursor y position (bring it down)
			set_cursor_y(cursor_y + char_height);
		}
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
int Edit::new_(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}
/////////////
