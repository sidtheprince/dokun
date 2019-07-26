#include "../include/font.h"

FONT::FONT() : width(0), height(16)/*(48)*/, data(nullptr), buffer(0)
{
	Factory::get_font_factory()->store(this);
}
/////////////
FONT::FONT(const FONT& font) : width(0), height(16), buffer(0) // copies data from another font
{
    copy(font);
	Factory::get_font_factory()->store(this);
}
/////////////
FONT::FONT(const std::string& file_name) : width(0), height(16), buffer(0)
{
    if(!load(file_name)) {
	    std::cerr << Logger("Could not open font from " + file_name);}
	Factory::get_font_factory()->store(this);
}
/////////////
int FONT::font_new(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		file_name = lua_tostring(L, -1);
	}
    lua_settop(L, 0); // clear any value pushed on top
	// create table (object)
	lua_createtable(L, 0, 0); // #1
    // set mt
	lua_getglobal(L, "Font_mt");
	lua_setmetatable(L, 1);
	// set userdata
	FONT **font = static_cast<FONT**>(lua_newuserdata(L, sizeof(FONT*)));
	if(!file_name.empty())
	{
		*font = new 
		FONT(file_name);
	}
	if(file_name.empty())
	{
	    *font = new 
		FONT();
    }
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, 1)) // return font, if table
	    return 1; 
	lua_pushnil(L); // if not table, return nil
	return 1;
}
/////////////
FONT::~FONT(void)
{
	destroy(); // destroy texture_buffer
	if(data) FT_Done_Face(data); // destroy font_data (texture)
	Factory::get_font_factory()->release(this);
}
/////////////
FT_Library FONT::library (nullptr);
/////////////
FONT * FONT::system_font (new FONT());
/////////////
bool FONT::open()
{
	if(library == nullptr) 
	{
		if(FT_Init_FreeType(&library))
			return false;
	}
#ifdef DOKUN_DEBUG0
    Logger("freetype initialized.");
#endif
	return true;
}
/////////////
void FONT::close()
{
	if(library != nullptr) 
	{
		FT_Done_FreeType(library);
#ifdef DOKUN_DEBUG0
        Logger("freetype closed.");
#endif	
	}
}
/////////////
bool FONT::load(const std::string& file_name) // load a font as a whole
{
	if(FT_New_Face(library, file_name.c_str(), 0, & data)) 
	{
#ifdef DOKUN_DEBUG
		Logger("Could not load font from " + file_name);
#endif
	    return false;
	}
	this->file = file_name; // save filename if load is successful
	FT_Set_Pixel_Sizes(data, width, height); // height x height pixel  // at 8, "Sid" would be 13 x 5 | at 16, "Sid" would be 24 x 11 |at 32, "Sid" would be 47 x 22  | at 48, "Sid" would be 74 x 32
	return true;
}
/////////////
bool FONT::load(void * data, unsigned char size)
{
    if(FT_New_Memory_Face(library, (FT_Byte *)data, size, 0, &this->data))
	{
		Logger("Could not load font from memory");
	    return false; 
	}
	//FT_Set_Pixel_Sizes(data, width, height); // height x height pixel
	return true;
}
/////////////
int FONT::load(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
		lua_pushboolean(L, font->load(lua_tostring(L, 2)));
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
void FONT::generate() // load each character and store it in array for later use
{
	if(is_generated()) return; // return if previously generated
	for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(static_cast<FT_Face>(data), c, FT_LOAD_RENDER))
	    {
	        Logger("Could not load glyph from font");
			continue;
	    }
		// generate texture for each glyph (character)
        unsigned int texture_id = 0;		
    #ifdef DOKUN_OPENGL
    #ifdef __windows__
	    if(!wglGetCurrentContext())
		    return;
    #endif
    #ifdef __gnu_linux__
        if(!glXGetCurrentContext())
		    return;
    #endif	
		glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, data->glyph->bitmap.width, data->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, data->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
		    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		glBindTexture(GL_TEXTURE_2D, 0);
	#endif	
		Character character = {
			texture_id,                      // id
			get_data()->glyph->bitmap.width, // width
			get_data()->glyph->bitmap.rows , // height
			get_data()->glyph->bitmap_left , // bearing_x 
			get_data()->glyph->bitmap_top  , // bearing_y
			get_data()->glyph->advance.x   , // advance_x
		};
	#ifdef DOKUN_DEBUG0 // show all glyphs and their sizes
		std::cout << String::to_string(c) <<" "<< character.width <<", "<< character.height << std::endl;
		//Logger(String::to_string(c) +" "+ std::to_string(character.width) +", "+ std::to_string(character.height) + "\n");
	#endif
		// store each character (for later use)
		character_array.insert(std::pair<char, Character>(c, character));
		// you can store and replace texture_id like this: character_array['a'].id = 10; // WORKS!!  //character_array.find('a')->second.id = 10;    
		//std::cout << "TEXTURE_ID: " << String::to_string(c) << " " << character_array.find(c)->second.id << "\n";
	}
	#ifdef DOKUN_DEBUG0
	    int num_characters = character_array.size();
	    int font_id = Factory::get_font_factory()->get_location(this);
	    Logger("Font(" + String::to_string(font_id) + ") generated with " 
	          + String::to_string(num_characters) + " characters.");
	#endif
}
/////////////
int FONT::generate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
        font->generate();
    }
    return 0;
}
/////////////
void FONT::destroy()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
		return;
#endif
#ifdef __gnu_linux__
    if(!glXGetCurrentContext())
		return;
#endif
    int character_count = character_array.size();	
    for(int i = 0; i < character_count; i++)
	{
		GLint texture = character_array[i].id;
		if(glIsTexture(texture))
	        glDeleteTextures(1, &character_array[i].id); // delete each character's texture
        character_array[i].id = 0; // set to nullptr
	#ifdef DOKUN_DEBUG
	    String string0 = "Font";
	    Logger(string0 + static_cast<int>(Factory::get_font_factory()->get_location(this)) + "'s texture deleted.");
    #endif	
	}
    character_array.clear(); // clear character_array so you can push_back new data
#endif	
}
/////////////
int FONT::destroy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
        font->destroy();		
	}
	return 0;
}
/////////////
/////////////
void FONT::copy(const FONT& font)
{
    // destroy old texture so you can generate a new one
    if(is_generated()) destroy();
	
    width  = font.get_width (); // in pixels
	height = font.get_height(); // in pixels
	data   = font.get_data  ();
	file   = font.get_file  ();
}
/////////////
int FONT::copy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    FONT * font2 = *static_cast<FONT **>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
 	    {
	        FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
            font->copy(*font2);		
	    }
    }
    return 0;
}
/////////////
/////////////
/////////////
/////////////
/////////////
//setters
void FONT::set_width(unsigned int width)
{
    this->width = width;
}
/////////////
int FONT::set_width(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
        font->set_width(lua_tonumber(L, 2));
    }
    return 0;
}
/////////////
void FONT::set_height(unsigned int height)
{
    this->height = height;
}
/////////////
int FONT::set_height(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
        font->set_height(lua_tonumber(L, 2));
    }
    return 0;
}
/////////////
void FONT::set_size(unsigned int width, unsigned int height)
{
    set_width(width);
    set_height(height);
}
/////////////
int FONT::set_size(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
        font->set_size(lua_tonumber(L, 2), lua_tonumber(L, 3));
    }
    return 0;
}
/////////////
/////////////
/////////////
/////////////
/////////////
int FONT::get_width() const  // returns FT_PIXEL_SIZES width
{
    return width;
}
/////////////
int FONT::get_width(char glyph) const // returns width of single character (glyph)
{
    const_cast<FONT*>(this)->generate();
    if(!is_generated()) return 0; //const_cast<FONT*>(this)->generate(); // generate font texture to get the glyph size// NOTE: IT is impossible to get the size of a character unless character_array has already been generated beforehand
    return character_array.find(glyph)->second.width;//data->glyph->bitmap.width;//character_array['A'].width;
}
/////////////
int FONT::get_width(const std::string& text) const // returns width of string (width of all string characters combined)
{
    if(text.empty()) return 0; // if the string is empty then return 0 for width
	int width_ = 0;
	for(int i = 0; i < text.size(); i++)
	{
		char character = text[i];	
		// add width of all characters
		int character_width = get_width(character); // this line crashes in Lua
		width_ = width_ + character_width;
	}
#ifdef DOKUN_DEBUG0
	Logger("Width of string \"" + text + "\" is: " + String(static_cast<int>(width_)).str());
#endif	
	return width_;
}
/////////////
int FONT::get_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_width());
		return 1;		
	}
	lua_pushnil(L);
	return 1;		
}
/////////////
int FONT::get_height() const // returns FT_PIXEL_SIZES height
{
    return height;
}
/////////////
int FONT::get_height(char glyph) const // returns height of single character (glyph)
{
    const_cast<FONT*>(this)->generate();
    if(!is_generated()) return 0;
    return character_array.find(glyph)->second.height;//return data->glyph->bitmap.rows;	 // NOTE: IT is impossible to get the size of a character unless character_array has already been generated beforehand
}
/////////////
int FONT::get_height(const std::string& text) const // returns the largest height of a single character (glyph) found in the text
{
	int height_ = 0;
	for(int i = 0; i < text.size(); i++) // loop through all glyphs
	{
		char character = text[i];
		// get height from character with biggest height
		int character_height = get_height(character); // this line crashes in Lua
		if (character_height > height_) {height_ = character_height;}
	#ifdef DOKUN_DEBUG0
		std::cout << character_height << " character_height [" << text[i] << "]" << std::endl;//Logger("Glyph heights " + String(static_cast<int>(character_height)).str());
	#endif
	}
#ifdef DOKUN_DEBUG0
	Logger("Height of string \"" + text + "\" is: " + String(static_cast<int>(height_)).str());
#endif		
	return height_;	
}
/////////////
int FONT::get_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_height());
		return 1;		
	}
	lua_pushnil(L);
	return 1;	
}
/////////////
Vector2 FONT::get_size() const
{
	return Vector2(get_width(), get_height());
}
/////////////
Vector2 FONT::get_size(char glyph) const
{
	return Vector2(get_width(glyph), get_height(glyph));
}
/////////////
int FONT::get_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_size().x);
		lua_pushnumber(L, font->get_size().y);
		return 2;		
	}
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
Vector4 FONT::get_rect()const
{
	int x = data->bbox.xMax;      // right 
	int y = data->bbox.yMin;      // bottom
	int width  = data->bbox.xMin; // left
	int height = data->bbox.yMax; // top
	return Vector4(x, y, width, height);
}
/////////////
int FONT::get_rect(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
		lua_pushnumber(L, font->get_rect().x);
		lua_pushnumber(L, font->get_rect().y);
		lua_pushnumber(L, font->get_rect().z);
		lua_pushnumber(L, font->get_rect().w);
		return 4;		
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;		
}
/////////////
FT_Face FONT::get_data() const// FT_Byte *
{
	return data;
}
int FONT::get_data(lua_State *L)
{
    return 1;
}
/////////////
FT_Face FONT::get_face() const// FT_Byte *
{
	return data;
}
int FONT::get_face(lua_State *L)
{
    return 1;
}
/////////////
FT_Byte * FONT::get_data_ptr()const
{
    return nullptr;
}
int FONT::get_data_ptr(lua_State *L)
{
    return 1;
}
/////////////
std::string FONT::get_file()const
{
	return file;
}
int FONT::get_file(lua_State *L)
{
    return 1;
}
/////////////
/*FONT * FONT::get_system_font()
{
	return system_font;
}*/
/////////////
/////////////
/////////////
/////////////
bool FONT::is_generated() const // checks if a texture has been generated for each glyph
{
	return !character_array.empty();
}
int FONT::is_generated(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		FONT * font = *static_cast<FONT **>(lua_touserdata(L, -1));
        lua_pushboolean(L, font->is_generated());
        return 1;
    }
    lua_pushnil(L);
    return 1;
}
/////////////
/////////////
/////////////
/////////////
