#include "../include/image.h"
/////////////
Image::Image() : width(0), height(0), depth(1), channel(4), data (nullptr), x(0), y(0), angle(0), scale(1, 1), color(255, 255, 255, 255), 
    relative_x(0), relative_y(0), alignment("none"), visible(true)
{
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
	format          = GL_RGBA;
#endif			
	Factory::get_image_factory()->store(this);
}
/////////////
Image::Image(const Image& image) : x(0), y(0), angle(0), scale(1, 1), color(255, 255, 255, 255), 
    relative_x(0), relative_y(0), alignment("none"), visible(true)
{
	copy(image);
	Factory::get_image_factory()->store(this);
}
/////////////
Image::Image(const Texture& texture) : x(0), y(0), angle(0), scale(1, 1), color(255, 255, 255, 255), 
    relative_x(0), relative_y(0), alignment("none"), visible(true)
{
	copy(texture);
    Factory::get_image_factory()->store(this);
}
/////////////
Image::Image(const std::string& file_name) : x(0), y(0), angle(0), scale(1, 1), color(255, 255, 255, 255), 
    relative_x(0), relative_y(0), alignment("none"), visible(true)
{
	if(!load(file_name))
	{
		Logger("Could not load " + file_name);
	}
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
	format = (channel == 4) ? GL_RGBA : GL_RGB;
#endif	
	Factory::get_image_factory()->store(this);
}
/////////////
Image::Image(void * data, int width, int height, int depth, int channel) : width(0), height(0), depth(1), data (nullptr), x(0), y(0), angle(0), scale(1, 1), color(255, 255, 255, 255), 
    relative_x(0), relative_y(0), alignment("none"), visible(true)
{
	if(!load(data, width, height, depth, channel))
		Logger("Could not load image from data");
#ifdef DOKUN_OPENGL	
	buffer          = 0;	
    mag_filter      = GL_NEAREST;
    min_filter      = GL_NEAREST;
    wrap_s          = GL_CLAMP_TO_BORDER;
 	wrap_t          = GL_CLAMP_TO_BORDER;
    internal_format = GL_RGBA;
#endif
	Factory::get_image_factory()->store(this);
}
/////////////
Image::~Image(void)
{
	destroy();
	Factory::get_image_factory()->release(this);
}
/////////////
bool Image::load(const std::string& file_name) // from file
{
	std::string extension = String::lower(file_name.substr(file_name.find_last_of(".") + 1));
	if(extension == "png") return load_png(file_name);
	if(extension == "jpeg" || extension == "jpg"  || extension == "jpe" || extension == "jif" || extension == "jfif" || extension == "jfi") 
		return load_jpeg(file_name);
	if(extension == "bmp" || extension == "dib") return load_bmp(file_name);
	if(extension == "gif") return load_gif(file_name);
	if(extension == "tif" || extension == "tiff") return load_tiff(file_name);
	Logger("Unsupported format " + extension);
	Logger("Loading as .PNG by default");
    return load_png(file_name); // default
}
/////////////
bool Image::load(void * data , int width, int height, int depth, int channel) // from data
{
	this->data    = data;
	this->width   = width;
	this->height  = height;
	this->depth   = depth;
	this->channel = channel;
	format = (channel == 4) ? DOKUN_RGBA : DOKUN_RGB;
	return true;
}
/////////////
int Image:: load(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // string or userdata
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
        Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TLIGHTUSERDATA)
		{
			// get width
			luaL_checktype(L, 3, LUA_TNUMBER);
			// get height
			luaL_checktype(L, 4, LUA_TNUMBER);
			// get data
			void * data = static_cast<void *>(lua_touserdata(L, 2));
			// load data
			lua_pushboolean(L, image->load(data, (int)lua_tonumber(L, 3), (int)lua_tonumber(L, 4)));
		    return 1;
		}
		if(lua_type(L, 2) == LUA_TSTRING)
		{
		    lua_pushboolean(L, image->load(lua_tostring(L, 2)));
			return 1 ;
		}
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
void Image:: draw ()
{
	if(visible) {
	generate();
	Renderer::draw_image(get_buffer(), get_width(), get_height(), get_depth(),
	    get_x(), get_y(), get_angle(), get_scale().x, get_scale().y, 
		get_color().x, get_color().y, get_color().z, get_color().w, get_channel());
	}
}
/////////////
void Image::draw(int x, int y)
{
	(this)->x = x;
	(this)->y = y;
	draw();
}
/////////////
int Image::draw (lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));

        if((lua_type(L, 2) == LUA_TNUMBER) &&
	    (lua_type(L, 3) == LUA_TNUMBER))
	    {
	        image->draw (lua_tonumber(L, 2), lua_tonumber(L, 3));
	    }
		else
		{
			image->draw();
		}
	}
	return 0;
}
/////////////
bool Image::save(std::string file_name)
{
	if(file_name.empty()) // to prevent crash
	{
		file_name = "Image" + std::to_string(Factory::get_image_factory()->get_location(this)) + ".png";
	}
	FILE *fp = fopen(file_name.c_str(), "wb"); // create file in write mode
    if (!fp)
	{
		std::cout << "Could not create " << file_name << std::endl;
	}
	return true;
}
/////////////
int Image::save(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->save(lua_tostring(L, 2));
	}
	return 0;
}
/////////////
void Image::copy(const Image& image)
{
    // destroy old texture_buffer so you can be able to generate a new texture_buffer
    destroy();

	data    = image.get_data   ();
	width   = image.get_width  ();
	height  = image.get_height ();
	depth   = image.get_depth  ();
	channel = image.get_channel();
	file    = image.get_file   ();
	set_position(image.get_position());
	set_angle(image.get_angle());
	set_scale(image.get_scale());
	set_color(image.get_color());
	// user interface
	set_relative_position(image.get_relative_position());
	set_alignment(image.get_alignment());
#ifdef DOKUN_OPENGL
    //buffer          = image.get_buffer         (); // buffer must be unique like the Image itself
	min_filter      = image.get_filter         ().x;
	mag_filter      = image.get_filter         ().y;
	wrap_s          = image.get_wrap           ().x;
	wrap_t          = image.get_wrap           ().y;
	internal_format = image.get_internal_format();
	format          = image.get_format         ();
#endif
}
/////////////
void Image::copy(const Texture& texture) // same as Image:copy_texture in Lua
{
    // destroy old texture_buffer so you can be able to generate a new texture_buffer
    destroy();

	width   = texture.get_width  ();
	height  = texture.get_height ();
	depth   = texture.get_depth  ();
	channel = texture.get_channel();
	data    = texture.get_data   ();
	file    = texture.get_file   ();
#ifdef DOKUN_OPENGL
	//buffer     = texture.get_buffer(); // buffer must be unique like the Image itself (so DO NOT copy!!)
	min_filter = texture.get_filter().x;
	mag_filter = texture.get_filter().y;
	wrap_s     = texture.get_wrap  ().x;
	wrap_t     = texture.get_wrap  ().y;
	internal_format = texture.get_internal_format();	
	format = texture.get_format();	
#endif	
}               
/////////////
int Image::copy(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image1 = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
			image->copy(*image1);
		}
	}
	return 0;
}
/////////////
int Image::copy_texture(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
			Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
			image->copy(*texture);
		}
	}
	return 0;	
}
/////////////
void Image::flip(int x, int y)
{
	if(x) scale = Vector2(-x, y);
	if(y) scale = Vector2(x, -y);
    if(x && y) scale = Vector2(-x, -y);
}
/////////////
int Image::flip(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->flip(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
/////////////
void Image::resize(int width, int height)
{
	int old_width  = get_width ();
	int old_height = get_height();
	set_scale(width / (double)old_width, height / (double)old_height);
}
/////////////
void Image::resize(const Vector2& size)
{
	resize(size.x, size.y);
}
/////////////
int Image::resize(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->resize(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
	}
    return 0;	
}
/////////////
void Image::scale_to_fit(int width, int height)
{
	float aspect = get_aspect_ratio_correction(width, height);
	set_scale(aspect, aspect);
}
/////////////
void Image::scale_to_fit(const Vector2& size)
{
	scale_to_fit(size.x, size.y);
}
/////////////
void Image::generate()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return;
#endif		
#ifdef __gnu_linux__
    if(!glXGetCurrentContext())
		return;
#endif
	if(!glIsTexture(buffer)) // no buffer yet (generate a single buffer and no more than 1)
	{
		glGenTextures(1, &buffer); // generate a buffer
		glBindTexture(GL_TEXTURE_2D, buffer); // bind buffer
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(get_width()), static_cast<GLsizei>(get_height()), 0, static_cast<GLenum>(get_format()), GL_UNSIGNED_BYTE, static_cast<GLvoid *>(get_data())); // pass texture width, height, and data to OpenGL
        glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps
		glBindTexture(GL_TEXTURE_2D, 0); // unbind buffer
	#ifdef DOKUN_DEBUG
	    //Logger("Image" + String(Factory::get_image_factory()->get_location(this)).str() + " buffer generated");
    #endif	
	}
#endif	
}
int Image::generate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
        image->generate();
	}
    return 0;	
}
/////////////
void Image::destroy()
{
#ifdef DOKUN_OPENGL	
#ifdef __windows__
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
        return;
#endif		
#ifdef __gnu_linux__
    if(!glXGetCurrentContext())
		return;
#endif
    if(buffer != 0)
	{
        glDeleteTextures(1, static_cast<GLuint *>(&buffer));
        buffer = 0;	// to ensure its deleted
	#ifdef DOKUN_DEBUG0
	    Logger("Image" + String(Factory::get_image_factory()->get_location(this)).str() + " buffer destroyed");
    #endif		
        // delete texture_data as well?
        data = nullptr;        
#endif	
	}		
}
int Image::destroy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
        image->destroy();
	}
    return 0;	
}
/////////////
/////////////
// SETTERS
/////////////
void Image::set_x(double x)
{
	this->x = x;
}
/////////////
int Image::set_x(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // x
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_x(lua_tonumber(L, 2));
	}		
	return 0;
}
/////////////
void Image::set_y(double y)
{
	this->y = y;
}
/////////////
int Image::set_y(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_y(lua_tonumber(L, 2));
	}		
	return 0;
}
/////////////
void Image::set_position(double x, double y)
{
	set_x(x);
	set_y(y);
}
/////////////
void Image::set_position(const Vector2& position)
{
	set_position(position.x, position.y);
}
/////////////
int Image::set_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // x
	luaL_checktype(L, 3, LUA_TNUMBER); // y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_position(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}		
	return 0;
}
/////////////
void Image::set_relative_position(double x, double y)
{
	relative_x = x;
	relative_y = y;
}
/////////////
void Image::set_relative_position(const Vector2& position)
{
	set_relative_position(position.x, position.y);
}
/////////////
int Image::set_relative_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // x
	luaL_checktype(L, 3, LUA_TNUMBER); // y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_relative_position(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}		
	return 0;	
}
/////////////
void Image::set_angle(double angle) // in degrees
{
	this->angle = angle;
}
/////////////
int Image::set_angle(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // angle
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_angle(lua_tonumber(L, 2));
	}		
	return 0;
}
/////////////
void Image::set_scale(double sx, double sy)
{
	scale = Vector2(sx, sy);
	// save new size ? - remove this ASAP!
	new_width  = scale.x * (double)this->width; // scale * original_size = new_size
	new_height = scale.y * (double)this->height;
	resized = true;
#ifdef  DOKUN_DEBUG0
	std::cout << "New size after scale: " << get_new_size() << "" << std::endl;
#endif		
}            
///////////// 
void Image::set_scale(const Vector2& scale)
{
	set_scale(scale.x, scale.y);	
}
/////////////
int Image::set_scale(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // scale_x
	luaL_checktype(L, 3, LUA_TNUMBER); // scale_y
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_scale(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}	
	return 0;
}
/////////////
void Image::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
/////////////
void Image::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}           
/////////////
void Image::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
/////////////
int Image::set_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // red
	luaL_checktype(L, 3, LUA_TNUMBER); // green
	luaL_checktype(L, 4, LUA_TNUMBER); // blue
	luaL_optnumber(L, 5, 255);         // alpha
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}		
	return 0;
}
/////////////
void Image::set_alignment(const std::string& alignment)
{
	this->alignment = alignment;
}
/////////////
int Image::set_alignment(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_alignment(lua_tostring(L, 2));
	}
    return 0;	
}
/////////////
void Image::set_visible(bool visible)
{
	this->visible = visible;
}
int Image::set_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		image->set_visible(lua_toboolean(L, 2));
	}
    return 0;		
}
/////////////
/////////////
void Image::set_filter(int min_filter, int mag_filter)
{
	this->min_filter = min_filter;
	this->mag_filter = mag_filter;
}
int Image::set_filter(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_wrap(int wrap_s, int wrap_t)
{
	this->wrap_s = wrap_s;
	this->wrap_t = wrap_t;
}
int Image::set_wrap(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_internal_format(int internal_format)
{
	this->internal_format = internal_format;
}
int Image::set_internal_format(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_format(unsigned int format)
{
	this->format = format;
}
int Image::set_format(lua_State * L)
{
    return 0;
}
/////////////
void Image::set_param(int min_filter, int mag_filter, 
	int wrap_s, int wrap_t, 
	int internal_format, unsigned int format)
{
	set_filter(min_filter, mag_filter);
	set_wrap(wrap_s, wrap_t);
	set_internal_format(internal_format);
	set_format(format);
}
int Image::set_param(lua_State *L)
{
    return 0;
}
/////////////
/////////////
// GETTERS
/////////////
int Image::get_width () const
{
    return width * get_scale().x;
}
/////////////
int Image::get_width(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_width());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
int Image::get_height () const
{
    return height * get_scale().y;
}
/////////////
int Image::get_height(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_height());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
int Image::get_depth() const
{
	return depth;
}
int Image::get_depth(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_depth());
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
Vector2 Image::get_size() const
{
    return Vector2(get_width(), get_height());
}
/////////////
int Image::get_size(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_size().x);
		lua_pushnumber(L, image->get_size().y);
		return 2;
	}
    lua_pushnil(L);
	lua_pushnil(L);
    return 2;
}
/////////////
void * Image::get_data() const
{
    return data;
}
/////////////
int Image::get_data(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushlightuserdata(L, image->get_data());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
std::string Image::get_file() const
{
	return file;
}
/////////////
int Image::get_file(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushstring(L, image->get_file().c_str());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
int Image::get_channel()        const
{
	return channel;
}
/////////////
int Image::get_channel(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_channel());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Image::get_buffer()const
{
	return buffer;
}
int Image::get_buffer(lua_State *L)
{
    return 1;
}
/////////////
Vector2i Image::get_filter()const
{
	return Vector2i(min_filter, mag_filter);
}
int Image::get_filter(lua_State *L)
{
    return 1;
}
/////////////
Vector2i Image::get_wrap()const
{
	return Vector2i(wrap_s, wrap_t);
}
int Image::get_wrap(lua_State * L)
{
    return 1;
}
/////////////
int Image::get_internal_format() const
{
	return internal_format;
}
int Image::get_internal_format(lua_State *L)
{
    return 1;
}
/////////////
unsigned int Image::get_format()         const
{
	return format;
}
/////////////
int Image::get_format (lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushinteger(L, image->get_format());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
Vector4 Image::get_color()      const
{
	return color;
}
/////////////
int Image::get_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_color().x);
		lua_pushnumber(L, image->get_color().y);
		lua_pushnumber(L, image->get_color().z);
		lua_pushnumber(L, image->get_color().w);
		return 4;
	}			
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;		
}
/////////////
double Image::get_x()const
{
	return x;
}
/////////////
int Image::get_x(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_x());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
double Image::get_y()const
{
	return y;
}
/////////////
int Image::get_y(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_y());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
Vector2 Image::get_position()const
{
	return Vector2(get_x(), get_y());
}
/////////////
int Image::get_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_position().x);
		lua_pushnumber(L, image->get_position().y);
		return 2;
	}	
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
double Image::get_relative_x()         const
{
	return relative_x;
}
/////////////
int Image::get_relative_x(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_relative_x());
		return 1;
	}
    lua_pushnil(L);
    return 1;
}
/////////////
double Image::get_relative_y()         const
{
	return relative_y;
}
/////////////
int Image::get_relative_y(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_relative_y());
		return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
Vector2 Image::get_relative_position() const
{
	return Vector2(get_relative_x(), get_relative_y());
}
/////////////
int Image::get_relative_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_relative_position().x);
		lua_pushnumber(L, image->get_relative_position().y);
		return 2;
	}	
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
double Image::get_angle()const
{
	return angle;
}
/////////////
int Image::get_angle(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_angle());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
/////////////
Vector2 Image::get_scale()const
{
	return scale;
}
/////////////
int Image::get_scale(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_scale().x);
		lua_pushnumber(L, image->get_scale().y);
		return 2;
	}			
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
/////////////
double Image::get_aspect_ratio_correction(int rect_width, int rect_height) const // scale to fit inside a rect
{
	double image_aspect = get_width() / get_height();
	double rect_aspect  = rect_width  / rect_height;
    double scale_factor = 0.0;
   	if(rect_aspect > image_aspect) {
		scale_factor     = rect_height / (double)get_height();
	} else scale_factor = rect_width  / (double)get_width ();
	return scale_factor;
}
/////////////
int Image::get_aspect_ratio_correction(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushnumber(L, image->get_aspect_ratio_correction(lua_tonumber(L, 2), lua_tonumber(L, 3)));
        return 1;
	}
    lua_pushnil(L);
    return 1;	
}
/////////////
int Image::get_new_width()        const // deprecated (delete soon)
{
	return new_width;
}
/////////////
int Image::get_new_height()       const // deprecated (delete soon)
{
	return new_height;
}
/////////////
Vector2 Image::get_new_size()     const // deprecated (delete soon)
{
	return Vector2(get_new_width(), get_new_height());
}
/////////////
std::string Image::get_alignment() const
{
	return alignment;
}
int Image::get_alignment(lua_State *L)
{
    return 1;
}
/////////////
Vector4 Image::get_rect() const
{
	int image_width  = get_width ();
	int image_height = get_height();
	return Vector4(x, y, image_width, image_height);
}
int Image::get_rect(lua_State * L)
{
    return 4;
}
/////////////
/////////////
// BOOLEAN
/////////////
bool Image::is_png()
{
    png_byte header[8];
	std::FILE * fp;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&fp, get_file().c_str(), "rb");
	#else
        fp = fopen(get_file().c_str(), "rb");
    #endif
    if (!fp) // file not found !
    {
		return false;
    }
    fread(header, 1, 8, fp);
    bool png = !png_sig_cmp(header, 0, 8);
    if (!png)
    {
	    fclose(fp);
		return false;
    }
	fclose(fp);
    return true;
}
/////////////
int Image::is_png(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, image->is_png());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool Image::is_image() // if image is loaded, it returns true
{
	return (is_png()); // only format we can load for now
}
/////////////
int Image::is_image(lua_State *L)
{
	/*
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, image->is_image());
        return 1;
	}
    lua_pushboolean(L, false);
    return 1;
	*/
	luaL_checktype(L, 1, LUA_TTABLE); // 1
	lua_getmetatable(L, 1); // 2
	lua_getglobal(L, "Image_mt"); // 3
#ifdef DOKUN_LUA51
	lua_pushboolean(L, lua_equal (L, 2, 3));
#endif
#ifndef DOKUN_LUA51
	lua_pushboolean(L, lua_compare(L, 2, 3, LUA_OPEQ));
#endif
	return 1;
}
/////////////
bool Image::is_resized() const
{
	return (resized == true);
}
int Image::is_resized(lua_State *L)
{
    return 1;
}
/////////////
bool Image::is_generated()const
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext()) // no context (to make function more safer to use and to prevent crash)
		return false;
#endif		
#ifdef __gnu_linux__
    if(!glXGetCurrentContext())
		return false;
#endif
    return (glIsTexture(buffer) == true);
#endif	
    return false;	
}
int Image::is_generated(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, image->is_generated());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;			
}
/////////////
bool Image::is_visible()const
{
	return (visible == true);
}
int Image::is_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
		lua_pushboolean(L, image->is_visible());
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
/////////////
bool Image::load_png(std::string file_name)
{
// header for testing if it is a png
    int number=8;

    png_byte header[8];
    // open the file
	std::FILE * fp;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&fp, file_name.c_str(), "rb");
	#else
        fp = fopen(file_name.c_str(), "rb");
    #endif
    if (!fp)
    {
        return false;
    }
	else
	{
		file =(file_name);
	}
    // read the header
    fread(header, 1, number, fp);
    int is_png = !png_sig_cmp(header, 0, number);
    if (!is_png)
	{
        fclose(fp);
        return false;
    }
   // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
        nullptr, nullptr);
    if (!png_ptr)
	{
        fclose(fp);
        return false;
    }
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
     fclose(fp);
     return false;
   }
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
     fclose(fp);
     return false;
   }
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return false;
   }
   //init png reading
   png_init_io(png_ptr, fp);
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, number);
   // change zlib compression buffer size when reading compressed data
   //int buffer_size = 8192;
   //png_set_compression_buffer_size(png_ptr, buffer_size);
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
   //variables to pass to get info
   int bit_depth, color_type, interlace_type, compression_type, filter_method;

   //update width and height based on png info
   width =  png_get_image_width(png_ptr, info_ptr);
   height = png_get_image_height(png_ptr, info_ptr);
   // get bit_depth and color_type based on png info
   bit_depth   = png_get_bit_depth(png_ptr, info_ptr);
   color_type = png_get_color_type(png_ptr, info_ptr);
   interlace_type=png_get_interlace_type(png_ptr,
                         info_ptr);
   compression_type=png_get_compression_type(png_ptr,
                         info_ptr);
   filter_method    = png_get_filter_type(png_ptr,
                         info_ptr);

    channel = png_get_channels(png_ptr, info_ptr);
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}

// .. must be called before using png_read_update_info, as well as other setters
   int number_of_passes;
   number_of_passes = png_set_interlace_handling(png_ptr);
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);


   // Allocate the image_data as a big block, to be given to opengl
   data = new png_byte[rowbytes * height];
   if (!data)
   {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return false;
   }

   //row_pointers is for pointing to data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers)
   {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     data = nullptr;
     fclose(fp);
     return false;
   }
   // set the individual row_pointers to point at the correct offsets of data
   for (unsigned int i = 0; i < height; ++i)
   {
     row_pointers[height - 1 - i] = (png_byte*)data + i * rowbytes;
   }
    // save row pointers
   data_p = row_pointers;
   // the actual reading of the png file
   //read the png into data through row_pointers
   png_read_image(png_ptr, row_pointers);

   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   fclose(fp);
   return true;
}
/////////////
bool Image::load_jpeg(std::string file_name)
{
	FILE * file = fopen(file_name.c_str(), "rb");
	if(!file) { return false;}

	struct jpeg_decompress_struct info;  //for our jpeg info
	struct jpeg_error_mgr        error;  //the error handler
	//jmp_buf jmp_buffer;
	unsigned char * rowptr;    // pointer to an array   // JSAMPARRAY buffer;
    int row_stride;		// physical row width in output buffer

    info.err = jpeg_std_error(&error);

    jpeg_create_decompress(&info); // create decompression object
	jpeg_stdio_src  (&info, file); // specify data source (e.g a file)
	jpeg_read_header(&info, TRUE); // read header
	jpeg_start_decompress (&info); // decompress the file

	width  = info.output_width ;
	height = info.output_height;

	row_stride = width * info.output_components;


	channel = info.num_components; //  3 =>RGB   4 =>RGBA
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}
	int size = width * height * channel;
	data = new unsigned char [size];

	while (info.output_scanline < info.output_height)
	{
		rowptr = static_cast<unsigned char *>(data) + info.output_scanline * info.output_width * channel;
		jpeg_read_scanlines(&info, &rowptr, 1);
	}

	jpeg_finish_decompress (&info);   //finish decompressing
	jpeg_destroy_decompress(&info);
    fclose(file);                    //close the file
	return true;
}
/////////////
bool Image::load_bmp(std::string file_name)
{
    FILE * file = fopen(file_name.c_str(), "rb");
	if(file == nullptr) {
		return false;
	}
	unsigned char header[54]; // 54 byte header (contains information about a bitmap)
	fread(header, sizeof(unsigned char), 54, file); // read header

	width  = *(unsigned int *)&header[18]; // 18 = width (offset)
	height = *(unsigned int *)&header[22]; // 22 = height (offset)
	channel = 4;
	unsigned int size = channel * width * height;
	unsigned char * image = new unsigned char[size];

	fread(image, sizeof(unsigned char), size, file); // read the rest of the data at once

	for(int i = 0; i < size; i += 3)
	{
		unsigned char t = image[i];
		image[i]    = image[i + 2];
		image[i + 2] =           t;
	}
	data = image;
	format = DOKUN_RGB;
	fclose(file);
	return true;
}
/////////////
bool Image::load_tga(std::string file_name)
{
	return false;
}
/////////////
bool Image::load_gif(std::string file_name)
{
	int error         = D_GIF_SUCCEEDED;
	GifFileType * gif = DGifOpenFileName(file_name.c_str(), &error); // open gif
	if(gif == nullptr)          {
		std::cerr << GifErrorString(D_GIF_ERR_OPEN_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
		return false;
	}
	if(DGifSlurp(gif) != GIF_OK) {                             // read gif
		std::cerr << GifErrorString(D_GIF_ERR_READ_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
		return false;
	}
	if(gif->ImageCount <= 0)     {
		Logger("Gif does not contain any image.");
		return false;
	}
    //-------------------
    SavedImage* first_image = &gif->SavedImages[0];
    ColorMapObject* color_map = first_image->ImageDesc.ColorMap ?
       first_image->ImageDesc.ColorMap : gif->SColorMap;
    width   = first_image->ImageDesc.Width;
    height  = first_image->ImageDesc.Height;
    channel = 3;
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}
	data = new unsigned char[width * height * channel];

	for (int i = 0; i < height; ++i)
	{
        unsigned char * p_dst = static_cast<unsigned char *>(data) + i * width * channel; // row pointer
        for (int j = 0; j < width; ++j)
		{
            GifByteType color_index = first_image->RasterBits[i * width + j];
            const GifColorType& gif_color = color_map->Colors[color_index];
            p_dst[j * channel + 0] = gif_color.Red;
            p_dst[j * channel + 1] = gif_color.Green;
            p_dst[j * channel + 2] = gif_color.Blue;
        }
    }
    //-------------------
    if(DGifCloseFile (gif, &error) != GIF_OK) {
		std::cerr << GifErrorString(D_GIF_ERR_CLOSE_FAILED)
		<< " Reason: " << GifErrorString(error)
		<< std::endl;
	}
	return true;
}
/////////////
bool Image::load_tiff(std::string file_name)
{
    TIFF* tiff = TIFFOpen(file_name.c_str(), "r");
    if(tiff == nullptr) {
		return false;
	}
	// --------------------
	TIFFRGBAImage img;
	uint16 bit_per_sample; // 8 for grayscale
	uint16 BitsPerSample;
	tsize_t scanline;
	uint32 row;
	uint32 imagelength;
	char emsg[1024];
	uint32 npixels;
	uint16 SamplesPerPixel;
	uint32 * raster;
	//unsigned char * buf;
	uint32 length;
	// --------------------

	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH ,     &width  );
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH,     &height ); // length
	TIFFGetField(tiff, TIFFTAG_IMAGEDEPTH ,     &depth  );
	TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &channel);
	TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE,   &bit_per_sample);
	if(channel == 3) { format = DOKUN_RGB; }
	if(channel == 4) { format = DOKUN_RGBA;}
	data = new unsigned char[width * height * channel];

	npixels = width * height;
    raster=(uint32 *) _TIFFmalloc(npixels *sizeof(uint32));
	if(raster == nullptr){
		Logger("Could not allocate memory");
		return false;
	}

    if(TIFFReadRGBAImage(tiff, width, height, raster, 0) == 0)
	{
	    Logger("Could not read image");
		_TIFFfree(raster);
		return false;
	}
    unsigned char * image = new unsigned char[width * height * channel];
	// process raster data
	for (int i = 0; i < height; ++i) {
	//for (int i = height - 1; i != -1; i--) {
        for (int j = 0; j < width; j++)
        {
            int red   = TIFFGetR(raster[i * width + j]);
            int green = TIFFGetG(raster[i * width + j]);
            int blue  = TIFFGetB(raster[i * width + j]);

			image[j * channel + 0] = TIFFGetR(raster[i * width + j]);
			image[j * channel + 1] = TIFFGetG(raster[i * width + j]);
			image[j * channel + 2] = TIFFGetB(raster[i * width + j]);
        }
    }
	//data = new unsigned char[width * height * channel];
    // --------------------
	//
	//data = image;
	_TIFFfree(raster);
    TIFFClose(tiff);
	return true;
}
/////////////
/////////////
/////////////
int Image::new_(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		file_name = lua_tostring(L, -1);
	}
	// clear stack
    lua_settop(L, 0);
	// create table
	lua_createtable(L, 0, 0);
	// set metatable
	lua_getglobal(L, "Image");
	lua_setmetatable(L, 1);
	// set userdata
	Image **image = static_cast<Image **>(lua_newuserdata(L, sizeof(Image *)));
	if(!file_name.empty())
	{
	    *image = new Image(file_name);
    }
	else
	{
	    *image = new Image();
	}
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
	{
	    return 1;
	}
	lua_pushnil(L);
	return 1;
}
