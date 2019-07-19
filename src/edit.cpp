#include "../include/edit.h"

Edit::Edit() : color(160, 160, 160, 255), character_limit(10),
    cursor(true), cursor_x(0), cursor_y(0), zoom_factor(0), multilined(false)
{
	set_position(0, 0);
	set_size(200, 20); // 150, 20
	set_orientation(0);
	
	label = new Label();
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
		    red, green, blue, alpha, multilined, cursor, cursor_x, cursor_y);
        // Draw text
		if(!label->get_string().empty())
		{
			label->draw();
			label->set_position(x, y + cursor_y);
			label->set_scale(0.5, 0.5);
			label->set_color(text_color);
		}
		// if mouse over edit, change mouse to I-beam	
		// edit is pressed, set cursor at position_pressed	
		on_hover();
		on_mousepress();
        on_keypress();
        on_backspace();
        on_enter();		
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
	    if( !OpenClipboard(nullptr)) // open clipboard
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
	(this)->label = &const_cast<Label&>(label);
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
	if(string0.length() > get_character_limit()) 
	{
		string0 = string0.erase(string0.length() - string0.length() + get_character_limit());
	}	
	get_label()->set_string(string0);
	int char_width = 10; // estimated glyph width
	set_cursor_x(cursor_x + (get_width() / char_width)); // set cursor at end of string
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
	get_label()->set_color(red, green, blue, alpha);
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
		    SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)ibeam); // DWORD = unsigned long
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
		    SetClassLong(window->get_handle(), GCL_HCURSOR, (DWORD)arrow);
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
	if(Mouse::is_over(get_position(), get_size()))
	{
		if(Mouse::is_pressed(1))
		{
		    WINDOW * window = WINDOW::get_active();
		    if(window != nullptr)
		        set_cursor_position(Mouse::get_position(*window).x, cursor_y);
		}
	}
}
/////////////
void Edit::on_keypress()
{
    if(Keyboard::is_down())
	{
		for(unsigned char i = 0; i < 128; i++)
		{
			if(Keyboard::is_pressed(i))
			{
					// number or letter or punctuation or space
				if(isalnum(i) ||  ispunct(i) || isspace(i)) 
				{
					set_text(get_text() + String::to_string(i));
				}
			}
		}
	}			
}
/////////////
void Edit::on_backspace()
{
#ifdef __windows__
    if(Keyboard::is_pressed(0x08))
#endif	
#ifdef __gnu_linux__	
    if(Keyboard::is_pressed(0xff08))
#endif		
	{
		// erase last character in string
		if(get_text().length()!=0) set_text(get_text().erase(get_text().length() - 1));
		// move cursor back
		int char_width = 10;
		set_cursor_x(get_cursor_x() - char_width);
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
			if(get_text().length() >= get_character_limit()) // if text length has reached character_limit
			{
				// double the height of the edit
				set_height(get_height() * 2); // GOOD!
				// add to the character_limit (new character_limit)
				set_character_limit(get_character_limit() + get_character_limit());  // ???
				// newline???
				set_text(get_text() + "\n");
				// reset cursor x position
				set_cursor_x(0);
				// set cursor y position
				int char_height = 12;
				set_cursor_y(get_cursor_y() + char_height);
			}
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