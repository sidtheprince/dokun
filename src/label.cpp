#include "../include/label.h"

Label::Label() : width(0), height(0), color(255, 255, 255, 255), alignment("left")//, relative_position(0,0) // label size is 0 as no string has been set (empty string)   EVERY LABEL YOU CREATE, SHOULD NOT GET ITS OWN FONT BY DEFAULT, ON CREATION (IT WILL CAUSE MEMORY LEAK!)  // INSTEAD CREATE A DEFAULT FONT THAT ALL INSTANCES OF LABEL CAN USE!!!!!!!!!!!! // ALL LABELS ARE CAPABLE OF USING A SINGLE FONT OBJECT
{
    if(!default_font->get_file().empty()) set_font(*default_font); // if the default font has already been loaded, set the font      //Logger("Default font has been set to label " + String(this).str());                                                  
	set_position(0, 0);
}
/////////////
Label::Label(const Label& label)
{
	set_font(* label.get_font());
	set_string(label.get_string());
	set_color(label.get_color());
	set_style(label.get_style());
	set_background_color(label.get_background_color());
	set_alignment(label.get_alignment());
	//set_relative_position(label.get_relative_position());
	
	set_position(label.get_position());
	set_width(label.get_width());
	set_height(label.get_height());	
}
/////////////
Label::Label(const FONT& font) : width(0), height(0), color(255, 255, 255, 255), alignment("left")
{
	set_font(font);
}
/////////////
Label::Label(const std::string& text) : width(0), height(0), color(255, 255, 255, 255), alignment("left")
{
    if(!default_font->get_file().empty()) set_font(*default_font); // if the default font has already been loaded, set the font
	set_string(text);
	set_position(0, 0);	
}
/////////////
Label::Label(int x, int y) : width(0), height(0), color(255, 255, 255, 255), alignment("left")
{
    if(!default_font->get_file().empty()) set_font(*default_font); // if the default font has already been loaded, set the font 
	set_position (x, y);
}
/////////////
Label::Label(int x, int y, int width, int height) : color(255, 255, 255, 255), alignment("left")
{
    if(!default_font->get_file().empty()) set_font(*default_font);
	set_position (x, y); 
	set_width (width);
	set_height (height);
}
/////////////
Label::Label(const std::string& text, int x, int y, int width, int height) : color(255, 255, 255, 255), alignment("left")
{
    if(!default_font->get_file().empty()) set_font(*default_font); // if the default font has already been loaded, set the font
	set_string (text);
	set_position (x, y);
	set_width (width);
	set_height (height);
}
/////////////
Label::Label(const std::string& text, const FONT& font) : width(0), height(0), color(255, 255, 255, 255), alignment("left")
{
	set_font(font);
	set_string(text);
	set_position(0, 0);
}
/////////////
Label::~Label()
{}
/////////////
FONT * Label::default_font (new FONT());
/////////////
void Label::draw()
{
	if(!font) {Logger("Font is null");return;}
    if(is_visible())
    {
		//if(font->get_file().empty()) Logger("No file in font");
		// STORE ALL CHARACTERS IN ARRAY BEFORE DRAWING
		font->generate();
        // Draw text
		Renderer::draw_text(string, get_x(), get_y(), 
		        get_width(), get_height(),
		        get_angle(), get_scale().x, get_scale().y, 
	            *font, get_color().x, get_color().y, get_color().z, get_color().w);
		// Draw glyph
		/*for(int i = 0; i < string.length(); i++)
		{
			unsigned char character = string[i];
			Renderer::draw_glyph(character, get_x() + i * font->get_width(character), get_y(), get_angle(), get_scale().x, get_scale().y, 
	            *font, get_color().x, get_color().y, get_color().z, get_color().w); // x = get_x + space_between_each_glyph
		#ifdef DOKUN_DEBUG0
			std::cout << "Glyph size: " << character << " " << font->get_size(character) << "\n"; // actual pixel_size of glyph
		#endif
		}*/
    }
	on_draw(); // callback for all gui
}
///////////// 
int Label::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->draw();
	}
	return 0;
}
/////////////
void Label::update(void)
{}
/////////////
/////////////
// SETTERS	
/////////////
void Label::set_string(const std::string& string)
{
	this->string = string;
	// update label size whenever the string is changed
	width = font->get_width (string);
	height= font->get_height(string);
} 
/////////////
int Label::set_string(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label -> set_string (lua_tostring(L, 2));
	}	
	return 0;
} 
/////////////
void Label::set_font(const FONT& font)
{
	this->font = &const_cast<FONT&>(font);
}
/////////////
int Label::set_font(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
		    lua_getfield(L, 1, "udata");
		    if(lua_isuserdata(L, -1))
		    {
			    Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
			    label->set_font (*font);
			    // set font in (Lua)
			    lua_pushvalue(L, 2);
			    lua_setfield(L, 1, "font");
		    }
	    }
	}
	if(lua_isnil(L, 2)) 
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
			label->font = (nullptr); // set font to nullptr instead of deleting it so it can be reused
		}
		lua_pushvalue(L, 2); // push 2nd arg
		lua_setfield(L, 1, "font"); // set font to nil
	}	
	return 0;
}	
/////////////
void Label::set_color(double red, double green, double blue, double alpha)
{
	color = Vector4(red, green, blue, alpha);
}   
/////////////
void Label::set_color(const Vector3& color)
{
	this->color = Vector4(color, 255);
}
/////////////
void Label::set_color(const Vector4& color)
{
	this->color = color;
}
/////////////
int Label::set_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->set_color((unsigned int)lua_tonumber(L, 2), (unsigned int)lua_tonumber(L, 3), (unsigned int)lua_tonumber(L, 4));
	}
	return 0;
}
/////////////
void Label::set_style(const int style)
{
	(this)->style = style;
	if(style & DOKUN_STYLE_REGULAR) // if regular, this function ignores all other styles
	{
		std::cout << "Regular"<<std::endl;
	}
	if(style & DOKUN_STYLE_BOLD)
	{
		std::cout << "Bold"<<std::endl;
	}
	if(style & DOKUN_STYLE_ITALIC)
	{
		std::cout << "Italic"<<std::endl;
	}
	if(style & DOKUN_STYLE_UNDERLINED)
	{
		std::cout << "Underline"<<std::endl;
	}
	if(style & DOKUN_STYLE_STRIKETHROUGH)
	{
		std::cout << "Strikethrough"<<std::endl;
	}
}   
/////////////
int Label::set_style(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->set_style((unsigned int)lua_tonumber(L, 2));
	}	
	return 0;
}
/////////////
void Label::set_background_color(int red, int green, int blue, int alpha)
{
	background_color = Vector4(red, green, blue, alpha);
}
/////////////
void Label::set_background_color(const Vector3& color)
{
	set_background_color(color.x, color.y, color.z);
}
/////////////
void Label::set_background_color(const Vector4& color)
{
	set_background_color(color.x, color.y, color.z, color.w);
}
/////////////
int Label::set_background_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->set_background_color((unsigned int)lua_tonumber(L, 2), (unsigned int)lua_tonumber(L, 3), (unsigned int)lua_tonumber(L, 4));
	}		
	return 0;
}
/////////////
void Label::set_alignment(const std::string& alignment)
{
	this->alignment = alignment;
}
int Label::set_alignment(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->set_alignment(lua_tostring(L, 2));
	}
	return 0;
}
/////////////
/*
void Label::set_relative_position(double x, double y)
{
	relative_position = Vector2(x, y);
}
void Label::set_relative_position(const Vector2& position)
{
	set_relative_position(position.x, position.y);
}
int Label::set_relative_position(lua_State * L)
{}*/
/////////////
/////////////
// GETTERS
/////////////
FONT * Label::get_font()const
{
	return font;
}
/////////////
int Label::get_font(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "font");
	if(lua_istable(L, -1))
	{
		return 1 ;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
std::string Label::get_string()const
{
	return string;
}
/////////////
int Label::get_string(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		lua_pushstring(L, label->get_string ().c_str());
		return 1 ;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector4 Label::get_color()const
{
	return color;
}   
/////////////
int Label::get_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_color().x);
		lua_pushnumber(L, label->get_color().y);
		lua_pushnumber(L, label->get_color().z);
		return 3;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}  
/////////////
int Label::get_style()const
{
	return 0;//style;
}   
/////////////
int Label::get_style(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_style());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
/////////////
Vector4 Label::get_background_color()const
{
	return background_color;
}
/////////////  
int Label::get_background_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_background_color().x);
		lua_pushnumber(L, label->get_background_color().y);
		lua_pushnumber(L, label->get_background_color().z);
		lua_pushnumber(L, label->get_background_color().w);
		return 4;
	}	
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);	
	return 4;	
}
/////////////
std::string Label::get_alignment() const
{
	return alignment;
}
int Label::get_alignment(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		lua_pushstring(L, label->get_alignment().c_str());
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
/*
double Label::get_relative_x() const
{
	return relative_position.x;
}
int Label::get_relative_x(lua_State * L)
{}
/////////////
double Label::get_relative_y() const
{
	return relative_position.y;
}
int Label::get_relative_y(lua_State * L)
{}
/////////////
Vector2 Label::get_relative_position() const
{
	return relative_position;
}
int Label::get_relative_position(lua_State * L)
{}*/
/////////////
double Label::get_aspect_ratio_correction(int rect_width, int rect_height) const
{
	double label_aspect = get_width() / get_height();
	double rect_aspect  = rect_width  / rect_height;
    double scale_factor = 0.0;
   	if(rect_aspect > label_aspect) {
		scale_factor     = rect_height / (double)get_height();
	} else scale_factor = rect_width  / (double)get_width ();
	return scale_factor;	
}
int Label::get_aspect_ratio_correction(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		lua_pushnumber(L, label->get_aspect_ratio_correction(lua_tonumber(L, 2), lua_tonumber(L, 3)));
        return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
/////////////
std::vector<Vector2> Label::get_character_size_array()
{
	std::vector<Vector2> character_size_list;
    // Iterate through all characters in string
    for (std::string::const_iterator c = get_string().begin(); c != get_string().end(); c++) // scan through all characters in string
    {
        FONT::Character ch = get_font()->character_array[*c];  // will change Character
        double width  = get_font()->character_array[*c].width;
        double height = get_font()->character_array[*c].width;
		character_size_list.push_back(Vector2(width, height));
	}		
	return character_size_list;
}
/////////////
FONT * Label::get_default_font()
{
    return default_font;
}
int Label::get_default_font(lua_State *L)
{
    return 1;
}
/////////////
/////////////
// OVERRIDE	
/////////////
void Label::set_width(int width)
{
	int old_width  = (font ? font->get_width (string) : get_width()); // width of entire string
	set_scale(width / (double)old_width, get_scale().y);
	this->width = width; // save width (in value)
}
int Label::set_width(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->set_width(static_cast<int>(lua_tonumber(L, 2)));
	}		
	return 0;	
}
/////////////
void Label::set_height(int height)
{
	int old_height = (font ? font->get_height(string) : get_height());
	set_scale(get_scale().x, height / (double)old_height);
	this->height = height; // save height (in value)
}
int Label::set_height(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->set_height(static_cast<int>(lua_tonumber(L, 2)));
	}		
	return 0;	
}
/////////////
void Label::set_size(int width, int height)
{
	set_width(width);
	set_height(height);
}
int Label::set_size(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Label * label = *static_cast<Label **>(lua_touserdata(L, -1));
		label->set_size(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
	}		
	return 0;	
}
/////////////
// override (getters)
int Label::get_width()const
{
	if(!width && font) 
		return font->get_width(this->string) * get_scale().x; // if width is not set, return width of all characters combined, whether scaled or not
	return width * get_scale().x; // return width whether scaled or not
}
int Label::get_width(lua_State * L)
{}
/////////////
int Label::get_height()const
{
	if(!height && font)
		return font->get_height(this->string) * get_scale().y; // if height is not set, return largest glyph height, whether scaled or not
	return height * get_scale().y; // return height whether scaled or not
}
int Label::get_height(lua_State * L)
{}
/////////////
Vector2 Label::get_size() const
{
	return Vector2(get_width(), get_height());
}
int Label::get_size(lua_State * L)
{}
/////////////
Vector4 Label::get_rect() const
{
	return Vector4(get_x(), get_y(), get_width(), get_height());
}
int Label::get_rect(lua_State * L)
{}
/////////////
/////////////
/////////////
int Label::new_(lua_State *L)
{
	lua_settop(L, 0);
	// create table
	lua_createtable(L, 0, 0);
	// set mt
	lua_getglobal(L, "Label_mt");
	lua_setmetatable(L, 1);
	// set userdata
	Label **label = static_cast<Label **>(lua_newuserdata(L, sizeof(Label *)));
	*label = new Label();
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
		return 1;
	lua_pushnil(L);
	return 1;
}
///////////////
/*  
std::cout << "Label position: " << label->get_position() << std::endl;
std::cout << "Label size: " << label->get_size() << " (all glyphs' width combined, highest glyph height)" << std::endl;

*/
