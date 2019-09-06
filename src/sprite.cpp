#include "../include/sprite.h"

////////////
Sprite::Sprite(void) : x(0), y(0), angle(0), scale_factor(1, 1), texture(nullptr), width(0), height(0), depth(1),
    color(255, 255, 255, 255), origin(0, 0)
{
	Factory::get_sprite_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	Logger::push("dokun: " + String(this).str() + " has been allocated with Sprite::new ()");
	Logger::push("       (index=" + std::to_string(Factory::get_sprite_factory()->get_location(this)) + ", total_sprite_count=" + std::to_string(Factory::get_sprite_factory()->get_size()) + ")");
#endif
}
////////////
Sprite::Sprite(const Sprite& sprite)
{
	x = sprite.get_position().x;
	y = sprite.get_position().y;
	angle = sprite.get_angle();
	scale_factor = sprite.get_scale();
	texture = sprite.get_texture();
	width   = sprite.get_width ();
	height  = sprite.get_height();
	depth   = sprite.get_depth ();
	color   = sprite.get_color ();
	origin  = sprite.get_origin();
	Factory::get_sprite_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	Logger::push(DOKUN_LOGTAG + "Sprite " + String(this).str() + " allocated (index=" + String(Factory::get_sprite_factory()->get_location(this)).str() + ")" + " (total_sprite_instances=" + String(Factory::get_sprite_factory()->get_size()).str() + ")");
#endif	
}
////////////
Sprite::Sprite(const Texture& texture) : x(0), y(0), angle(0), scale_factor(1, 1), width(0), height(0), depth(1),
    color(255, 255, 255, 255), origin(0, 0)
{
	set_texture(texture);
	Factory::get_sprite_factory()->store(this);
#ifdef DOKUN_DEBUG 	
	Logger::push(DOKUN_LOGTAG + "Sprite " + String(this).str() + " allocated (index=" + String(Factory::get_sprite_factory()->get_location(this)).str() + ")" + " (total_sprite_instances=" + String(Factory::get_sprite_factory()->get_size()).str() + ")");
#endif	
}
////////////
Sprite::Sprite(const std::string& file_name) : x(0), y(0), angle(0), scale_factor(1, 1), width(0), height(0), depth(1),
    color(255, 255, 255, 255), origin(0, 0)
{ 
	if(!load(file_name)) 
		Logger("Could not open " + file_name);
	Factory::get_sprite_factory()->store(this); 
#ifdef DOKUN_DEBUG 	
	Logger::push(DOKUN_LOGTAG + "Sprite " + String(this).str() + " allocated (index=" + String(Factory::get_sprite_factory()->get_location(this)).str() + ")" + " (total_sprite_instances=" + String(Factory::get_sprite_factory()->get_size()).str() + ")");
#endif	
}
////////////
Sprite::~Sprite(void)
{
	Factory::get_sprite_factory()->release(this); // dont destroy texture cause it may be reused by another sprite. Deallocate it seperately
#ifdef DOKUN_DEBUG 	
	Logger::push("dokun: " + String(this).str() + " deallocated with Sprite::Sprite~()\n       (total_sprite_count=" + String(Factory::get_sprite_factory()->get_size()).str() + ")");
#endif	
}
////////////
Shader * Sprite::shader (new Shader());
////////////
////////////
int Sprite::sprite_new(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING) 
		file_name = lua_tostring(L, -1); // keep it at -1 just in case base class is pushed into "new" function Sprite:new()
	lua_settop(L, 0); // clear stack	
	lua_createtable(L, 0, 0);      // create table
	lua_getglobal(L, "Sprite"); // set metatable
	lua_setmetatable(L, 1);
	Sprite ** sprite = static_cast<Sprite **>(lua_newuserdata(L, sizeof(Sprite*))); // set userdata
	if(!file_name.empty()) 
	    *sprite = new Sprite(file_name);
    else 
		*sprite = new Sprite;
	lua_setfield(L, 1, "udata");
	// return the newly created table
	if(lua_istable(L, -1)) 
		return 1;
	// return nil if not a table
	lua_pushnil(L);
	return 1;
}
////////////
bool Sprite::load(const std::string& file_name)
{ 
    Texture * texture = new Texture();
	if(!texture->load(file_name))
	{
		return false;
	}
	set_texture(*texture);
	return true;
}
////////////
bool Sprite::load(const std::string& file_name, int x, int y, int left, int top)
{
    Texture * texture = new Texture();
	texture->set_rect(x, y, left, top);
	if(!texture->load(file_name))
	{
		return false;
	}
	set_texture(*texture);
	return true;
}
////////////
int Sprite::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_optinteger(L, 3, 0); // x
	luaL_optinteger(L, 4, 0); // y
	luaL_optinteger(L, 5, 0); // left
	luaL_optinteger(L, 6, 0); // top
	lua_getfield(L, 1, "udata");
	//std::cout << Vector4(lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5), lua_tointeger(L, 6)) << "\n";
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushboolean(L, sprite->load(lua_tostring(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5), lua_tointeger(L, 6)));
		return 1;	
	}
	lua_pushboolean(L, false);
	return 1; 
}
////////////
void Sprite::generate() // will generate shaders (ONLY once)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	if(Renderer::get_current_API() != "OpenGL") // but it is not set as the current rendering API
	    return;
#ifdef __windows__
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return;
#endif
#endif
static const char * vertex_source[] =
        //{"#version 330\n"};
	    {
		    "#version 330\n" // 330, 400,  410, 420, 430, 440, 450
		    "\n"
            "\n"			
		    "layout(location = 0) in vec2 xy;\n"
		    "layout(location = 1) in vec2 tex_coord;\n"
		    "layout(location = 2) in vec2 normal;\n"
		    "\n"
		    "out vec2 Texcoord;\n"
		    "out vec2 Normal;  \n"
		    "\n"
		    "\n"
		    "uniform mat4 proj;\n"
		    "uniform mat4 view;\n"
		    "uniform mat4 model;\n"
		    "uniform mat4 light_space;\n"
		    "\n"
		    "\n"
		    "void main(void)\n"
            "{\n"
		    "\n"
		    "\n"
		        "Normal      = normal;\n"
		        "Texcoord    = vec2(tex_coord.x, 1.0 - tex_coord.y);\n"         // if ortho is top-left corner	
		        "gl_Position = proj * view * model * vec4(xy, 0, 1);\n"
		    "}\n"
	    };
	    static const char * fragment_source[] =
	    //{"#version 330\n"};
	    {
		    "#version 330\n" // 330, 400,  410, 420, 430, 440, 450
		    "out vec4 out_color;\n"
		    "uniform vec4 color;\n"
			"uniform vec2 resolution;\n"
		    "in vec2 Texcoord;  \n"
		    "in vec2 Normal;    \n"
		    "\n"
		    "struct Texture {\n"
		        "sampler2D diffuse;\n" // base map;same as color
		        "sampler2D specular;\n"
				"sampler2D normal;\n"
		        "sampler2D shadow;\n"
				"//sampler2DArray texture_array;\n"
		        "bool is_texture; \n"
		        "bool is_video;   \n"
				"int  count;      \n" // to see if sprite has normal_map
		    "};\n"
		    "\n"
		    "struct Light {\n" // global light
	    	    "vec4  position;      \n"
	    	    "vec4  ambient;       \n"
	    	    "vec4  diffuse;       \n"
	    	    "vec4  specular;      \n"
				"vec3  spot_direction;\n"
				"float spot_exponent; \n"
				"float spot_cutoff;   \n"
				"vec3  attenuation;   \n"
                "bool is_enabled;     \n"				
		    "};\n"
		    "struct Camera {\n"
		        "vec3 position;\n"
		    "};\n"
			"uniform Texture map;\n"
		    "uniform Light light;\n"
		    "uniform Camera camera;\n"
		    "\n"
		    "\n"
		    "\n"
		    "\n"
		    "vec3 rgb_to_yuv(float R, float G, float B)             \n"
		    "{                                                      \n"
		        "float Y = (0.299 * R) + (0.587 * G) + (0.114 * B); \n"
		        "float U = (B - Y) * 0.565;                         \n"
		        "float V = (R - Y) * 0.713;                         \n"
		        "return vec3(Y, U, V);                              \n"
		    "}                                                      \n"
		    "\n"
		    "vec3 rgb_to_yuv(vec3 color)                                              \n"
		    "{                                                                        \n"
		        "float Y = (0.299 * color.x) + (0.587 * color.y) + (0.114 * color.z); \n"
		        "float U = (color.z - Y) * 0.565;                                     \n"
		        "float V = (color.x - Y) * 0.713;                                     \n"
		        "return vec3(Y, U, V);                                                \n"
		    "}                                                                        \n"    
		    "\n"
			"void main(void)\n"
            "{\n"		
		        "\n"
		        // attenuation
				"float distance = length(light.position.xy - gl_FragCoord.xy);\n" // distance to light    lightlocation
		        "float attenuation = 1.0 / (1.0 + 0.0 * pow(distance, 2));\n"
				// diffuse
				"vec2 normal = normalize(vec3(Normal, 0)).xy;"
				"vec4 light_direction = vec4(normalize(light.position.xy - gl_FragCoord.xy), 0.0, 1.0);\n" // light_direction
		        "float diffuse_coefficient = max(dot(vec4(normal, 0, 1), light_direction), 0);"
				// specular
				"vec3 view_direction = normalize(vec4(camera.position, 1.0).xyz - gl_FragCoord.xyz);\n"
				"float mat_shininess = 1.0;" // default sprite_shininess will be 1.0
	            "float specular_coefficient = pow(max(0, dot(vec4(view_direction, 1.0), reflect(-light_direction, vec4(normal, 0, 1) ))), mat_shininess);\n"		
		        // gamma_correction - corrects the lighting
				"float gamma = 2.2;\n"
		        "\n"
		        "\n"
				"//---------------------------\n"
		        "vec4 ambient, diffuse, specular, light_color;\n"
				"//---------------------------\n"
				"if(map.is_texture == true){\n"
				    "ambient   = light.ambient  * texture(map.diffuse , Texcoord);\n"
				    "diffuse   = light.diffuse  * texture(map.diffuse , Texcoord) * diffuse_coefficient; \n"
				    "specular  = light.specular * texture(map.specular, Texcoord) * specular_coefficient;\n"
				    "light_color = ambient + attenuation * (diffuse + specular);\n"				
			        "if(light.is_enabled == false){\n"
					    "out_color = color * texture(map.diffuse, Texcoord);\n" //"out_color = vec4(rgb_to_yuv(color.xyz), color.w) * texture(map.diffuse, Texcoord);\n"
					"}" 
					"if(light.is_enabled == true){\n"
					    "out_color =  light_color * color;//pow(light_color * color, vec4(1.0/gamma, 1.0/gamma, 1.0/gamma, 1.0));\n"
				    "}"
				"}"
				"if(map.is_texture == false)\n" // blank texture (data is null), use color ONLY
				"{"		
				    "ambient   = light.ambient;\n"
				    "diffuse   = light.diffuse  * diffuse_coefficient;\n"
				    "specular  = light.specular * specular_coefficient;\n"
				    "light_color = ambient + attenuation * (diffuse + specular);\n"				
					"if(light.is_enabled == false){\n"
					    "out_color = color;\n"
					"}"
					"if(light.is_enabled == true){\n"
					    "out_color = light_color * color;\n"
					"}"
				"}"
		    "}\n"			
		};		
    if(!shader->has_program())
    {
	    shader->create();
	    shader->set_source(vertex_source  , 0);
	    shader->set_source(fragment_source, 1);
	    shader->prepare();
	#ifdef DOKUN_DEBUG
	    Logger::push(DOKUN_LOGTAG + "Sprite shaders have been generated (count=2)");
    #endif
    }
#endif	
}
////////////
int Sprite::generate(lua_State *L)
{
	Sprite::generate();
	return 0;
}
////////////
void Sprite::draw(int frame)
{
	if(!texture) return; // Draw nothing if no texture
	if(is_visible()) // sprite is visible
	{
		double frame_x = (texture ? texture->get_rect().x : 0); 
		double frame_y = (texture ? texture->get_rect().y : 0); 
	    int width_of_individual_sprite  = (texture ? texture->get_rect().z : 32);
	    int height_of_individual_sprite = (texture ? texture->get_rect().w : 32);
		// update vertices
        //update();
		// check if textures have been generated (pre-generates textures ONCE!) (instead of generating textures in a loop which WILL result in a slow down in rendering and memory leak)
		texture->generate();
		// check if shaders have been generated (pre-generates sprite shaders ONCE!)
		Sprite::generate();	
		// Sprite: "Hey renderer, take my information and render me please"
		// Renderer: "Sure wait one moment while I take a look at your data"
		if(texture->get_rect().empty())
		{
		    // Draw sprite
		    Renderer::draw_sprite(*texture, x, y, angle, scale_factor.x, scale_factor.y, color.x, color.y, color.z, color.w, get_vertex_array(), *shader, map);
            //this->update();Renderer::draw_sprite_test(x, y, angle, scale_factor.x, scale_factor.y, color.x, color.y, color.z, color.w, get_vertex_array(), *shader, *texture);
		}			
		if(!texture->get_rect().empty())
		{
	    // Draw atlas
		Renderer::draw_atlas(*texture, width_of_individual_sprite, height_of_individual_sprite, x, y, angle, scale_factor.x, scale_factor.y,
	        color.x, color.y, color.z, color.w, frame, *shader);
		// Update sprite size
        width  = width_of_individual_sprite;
        height = height_of_individual_sprite;		
		}
	}
}
////////////
int Sprite::draw(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_optnumber(L, 2, 0);//luaL_optinteger(L, 2, 0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
        sprite->draw(lua_tonumber(L, 2));//(lua_tointeger(L, 2));
	}
	return 0;
}
////////////
void Sprite::update() // update vertices
{
	vertex_array.clear();
	vertex_array.push_back(x);
	vertex_array.push_back(y);
	vertex_array.push_back(x + get_texture()->get_width());
	vertex_array.push_back(y);
	vertex_array.push_back(x + get_texture()->get_width());
	vertex_array.push_back(y + get_texture()->get_height());
	vertex_array.push_back(x);
	vertex_array.push_back(y + get_texture()->get_height());
}
////////////
int Sprite::update(lua_State *L)
{
    return 0;
}
////////////
void Sprite::translate(double x, double y) /* in units */
{
	this->x = this->x + x;
	this->y = this->y + y;
}
////////////
void Sprite::translate(const Vector2& p)
{
    translate(p.x, p.y);
}
////////////
int Sprite::translate(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		sprite->translate (static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)));		
	}
	return 0;
}
////////////
void Sprite::rotate(double angle) /* in degrees */
{ 
    set_angle(angle);
}
////////////
int Sprite::rotate(lua_State *L)
{
    return set_angle(L);
}
////////////
void Sprite::scale(double sx, double sy)
{
    set_scale(sx, sy);
}
////////////
void Sprite::scale(const Vector2& s)
{
    scale(s.x, s.y);
}
////////////
int Sprite::scale(lua_State * L)
{
    return set_scale(L);
}
////////////
void Sprite::shear(double shx, double shy) 
{}
////////////
void Sprite::shear(const Vector2 & sh) 
{
    shear(sh.x, sh.y);		
}
////////////
int Sprite::shear(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
	    sprite->shear (static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)));
	}
	return 0;
}
////////////
void Sprite::reflect(bool x, bool y) // flip
{
    if(x != 0) {
        scale(-1, 1);
	}
	if(y != 0) {
	    scale(1, -1);
	}
	if(x && y) {
		scale(-1, -1);
	}
}
int Sprite::reflect(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
        sprite->reflect (static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));		
	}
	return 0;
}
////////////
void Sprite::resize(int width, int height)
{
    set_size(width, height);
}
int Sprite::resize(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));	
        sprite->resize(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
////////////
void Sprite::lock(int window_width, int window_height) // keeps sprite within window bounds (works only in a loop)
{
	int sprite_width  = get_width ();
	int sprite_height = get_height();
	if(x > window_width  - sprite_width ) x = window_width  - sprite_width ;
	if(y > window_height - sprite_height) y = window_height - sprite_height;
	if(x < 0) x = 0;
	if(y < 0) y = 0;	
}
////////////
void Sprite::lock(const Vector2& window_size)
{
	lock(window_size.x, window_size.y);
}
////////////
int Sprite::lock(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));	
        sprite->lock(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;	
}
////////////
void Sprite::lock_all(int window_width, int window_height)
{
	for(int i = 0; i < Factory::get_sprite_factory()->get_size(); i++)
	{
		Sprite * sprite = static_cast<Sprite *>(Factory::get_sprite_factory()->get_object(i));
		sprite->lock(window_width, window_height);
	}
}
////////////
int Sprite::lock_all(lua_State *L)
{
	Sprite::lock_all(lua_tonumber(L, 2), lua_tonumber(L, 3));
	return 0;
}
////////////
void Sprite::add(const Texture& texture)
{
	map.push_back(&const_cast<Texture&>(texture));
}
////////////
int Sprite::add(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1)) 
	{	
        Texture * texture = *static_cast<Texture**>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1)) 
	    {
		    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));	
            sprite->add(*texture);
			// set in lua as well
			lua_pushvalue(L, 2);
			lua_setfield(L, 1, ("texture" + std::to_string(sprite->map.size() + 1)).c_str());
	    }
	}
	return 0;	
}
////////////
////////////
void Sprite::set_position(double x, double y) /* in units */
{
	this->x = x;
	this->y = y;
}
void Sprite::set_position(const Vector2& p)
{
    set_position(p.x, p.y);
}
int Sprite::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
	    sprite->set_position (static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "y");			    
	}
	return 0;
}
////////////
void Sprite::set_angle(double angle) /* in degrees */
{
    //angle = (static_cast<double>(degrees) + 360.0) % 360.0; // keep in range from 0-360 degrees
	this->angle = angle;
}
////////////
int Sprite::set_angle(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		sprite->set_angle (static_cast<double>(lua_tonumber(L, 2)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "angle");			
	}
	return 0;
}
////////////
void Sprite::set_scale(double sx, double sy)
{
	scale_factor.x = sx;
	scale_factor.y = sy;
}
////////////
int Sprite::set_scale(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		sprite->set_scale (static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "scale_x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "scale_y");					
	}
	return 0;	
}
//////////// 
void Sprite::set_texture(const Texture& texture, int index)
{
	this->texture = &const_cast<Texture&>(texture);
}
////////////
int Sprite::set_texture(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	if(lua_istable(L, 2))
	{
		lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1)) 
	    {
		    Texture * texture = *static_cast<Texture**>(lua_touserdata(L, -1));
		    lua_getfield(L, 1, "udata");
		    if(lua_isuserdata(L, -1)) 
		    {
				// set texture in C++
			    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
			    sprite->set_texture(*texture);
			    // set texture in Lua
	            lua_pushvalue(L, 2);
			    if(lua_istable(L, -1))
		  	    {
			        lua_setfield(L, 1, "texture");
			    }
		    }
	    }
	}
	if(lua_isnil(L, 2))
	{
		// set texture to nullptr in C++
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		    sprite->texture = nullptr; // set texture to nullptr instead of deleting it so it can be reused
		}
		// set texture to nil in Lua
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "texture");
	}
	return 0;
}
////////////
void Sprite::set_size(int width, int height) /* in pixels */
{
	int old_width  = (texture ? texture->get_width () : get_width ());
	int old_height = (texture ? texture->get_height() : get_height());
	set_scale(width / static_cast<double>(old_width), height / static_cast<double>(old_height));
	// change to new width and height (in value)
	this->width  = width ;
    this->height = height;
} // resizes the sprite ONLY after texture is set; if called before setting a texture, texture will not appear on screen (unless called twice >:D)
////////////
int Sprite::set_size(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		sprite->set_size(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "width");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "height");				
	}
	return 0;
}
////////////
void Sprite::set_color(int red, int green, int blue, int alpha) /* in RBGA */
{
	color = Vector4(static_cast<double>(red), static_cast<double>(green), static_cast<double>(blue), static_cast<double>(alpha)); // final_color = ambient + diffuse + specular
}
////////////
void Sprite::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
////////////
void Sprite::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
} 
//////////// 
int Sprite::set_color(lua_State *L)
{   
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		sprite->set_color(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "red");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "green");
		lua_pushvalue(L, 4);
		lua_setfield(L, 1, "blue");
		lua_pushvalue(L, 5);
		lua_setfield(L, 1, "alpha");								
	}
	return 0;
}
////////////
void Sprite::set_origin(double x, double y)
{
	origin = Vector2(x, y);
}
////////////
void Sprite::set_origin(const Vector2& origin)
{
	set_origin(origin.x, origin.y);
}
////////////
int Sprite::set_origin(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		sprite->set_origin(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
		// set in lua as well ...
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "origin_x");
		lua_pushvalue(L, 3);
		lua_setfield(L, 1, "origin_y");	
	}	
	return 0;
}
////////////
////////////
Vector2 Sprite::get_position()  const/* in units */
{
	return Vector2(x, y);
}
////////////
int Sprite::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushnumber(L, sprite->get_position().x);
		lua_pushnumber(L, sprite->get_position().y);
		return 2;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;
}
////////////
double Sprite::get_angle()  const/* in degrees */
{
    return angle; //* (57.2958);
}
////////////
int Sprite::get_angle(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushnumber(L, sprite->get_angle ());
		return 1;
	}    
	lua_pushnil(L);
	return 1;	
}
////////////
Vector2 Sprite::get_scale() const
{
	return scale_factor;
}
////////////
int Sprite::get_scale(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushnumber(L, sprite->get_scale().x);
		lua_pushnumber(L, sprite->get_scale().y);
		return 2;
	}
    lua_pushnil(L);
    lua_pushnil(L);	
	return 2;
}
////////////
Texture * Sprite::get_texture(int index) const
{
	return texture;
}
////////////
int Sprite::get_texture(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "texture");
	if(lua_istable(L, -1)) 
	{
		return 1;
	}
	lua_pushnil(L);
	return 1;	
}
////////////
int Sprite::get_width() const
{
	if(/*!width &&*/ texture) return texture->get_width() * get_scale().x; // if the width of a sprite is not set, return the width of the texture whether scaled or not
	return width * get_scale().x; // returns width whether scaled or not
}
////////////
int Sprite::get_width(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushinteger(L, sprite->get_width());
		return 1;
	}	
    lua_pushnil(L);	
	return 1;
}
////////////
int Sprite::get_height() const
{
	if(/*!height &&*/ texture) return texture->get_height() * get_scale().y; // if the height of a sprite is not set, return the height of the texture whether scaled or not
	return height * get_scale().y; // returns height whether scaled or not
}
////////////
int Sprite::get_height(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushinteger(L, sprite->get_height());
		return 1;
	}	
    lua_pushnil(L);		
	return 1;
}
////////////
int Sprite::get_depth() const
{
	return depth;
}
////////////
int Sprite::get_depth(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushinteger(L, sprite->get_depth());
		return 1;
	}	
    lua_pushnil(L);		
	return 1;	
}
////////////
Vector2 Sprite::get_size() const /* in pixels */
{
    return Vector2(get_width(), get_height());
}
////////////
int Sprite::get_size(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
	    lua_pushinteger(L, static_cast<int>(sprite->get_size ().x));
		lua_pushinteger(L, static_cast<int>(sprite->get_size ().y));
		return 2;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	return 2;	
}
////////////
Vector4 Sprite::get_color() const /* in RBGA */
{
    return color;
}
////////////
int Sprite::get_color(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushinteger(L, static_cast<int>(sprite->get_color().x));
		lua_pushinteger(L, static_cast<int>(sprite->get_color().y));
		lua_pushinteger(L, static_cast<int>(sprite->get_color().z));
		lua_pushinteger(L, static_cast<int>(sprite->get_color().w));
		return 4;
	}		
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;
}
////////////
Vector2 Sprite::get_origin() const
{
	return origin;
}
////////////
int Sprite::get_origin(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushinteger(L, static_cast<int>(sprite->get_origin().x));
		lua_pushinteger(L, static_cast<int>(sprite->get_origin().y));
		return 2;
	}		
	lua_pushnil(L);
	lua_pushnil(L);
    return 2;	
}
////////////
Vector2 Sprite::get_center() const /* in pixel_coordinates */
{
	double center_x = x + (get_width()  / 2); 
	double center_y = y + (get_height() / 2);
	return Vector2(center_x, center_y);
}
////////////
int Sprite::get_center(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_pushinteger(L, static_cast<int>(sprite->get_center().x));
		lua_pushinteger(L, static_cast<int>(sprite->get_center().y));
		return 2;
	}		
	lua_pushnil(L);
	lua_pushnil(L);
    return 2;		
}
////////////
////////////
std::vector<float> Sprite::get_vertex_array() const
{
	return vertex_array;
}
////////////
////////////
bool Sprite::is_sprite()
{
    return ((this != 0) && (dokun::instanceof<Sprite>(this) != 0));
}
////////////
int Sprite::is_sprite(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushboolean(L, dynamic_cast<Sprite *>(entity)->is_sprite());
		return 1;
	}
    lua_pushboolean(L, false);
    return 1;	
}
////////////
bool Sprite::is_generated()
{
#ifdef DOKUN_OPENGL
	return (shader->has_program() == true);
#endif	
}
////////////
int Sprite::is_generated(lua_State *L)
{
	lua_pushboolean(L, Sprite::is_generated());
	return 1;
}
////////////
bool Sprite::collide(const Sprite& a, const Sprite& b)
{
/*a - bottom right co-ordinates*/	
	int ax = a.x;
	int ay = a.y;
	int aw = a.get_width ();//(a.get_scale().x == 1) ? a.get_width () : a.get_size_scaled().x;
	int ah = a.get_height();//(a.get_scale().y == 1) ? a.get_height() : a.get_size_scaled().y;
	int ax1 = ax + aw - 1; // a.get_position().x   a.get_width ()
	int ay1 = ay + ah - 1; // a.get_position().y   a.get_height()
	/*b - bottom right co-ordinates*/
	int bx = b.x;
	int by = b.y;
	int bw = b.get_width ();//(b.get_scale().x == 1) ? b.get_width () : b.get_size_scaled().x;
	int bh = b.get_height();//(b.get_scale().y == 1) ? b.get_height() : b.get_size_scaled().y;
	int bx1 = bx + bw - 1; // b.get_position().x   b.get_width ()
	int by1 = by + bh - 1; // b.get_position().y   b.get_height()
	/*check if bounding boxes intersect*/
	if((bx1 < ax) || (ax1 < bx))
		return false;
	if((by1 < ay) || (ay1 < by))
		return false;
	return true;
}
////////////
int Sprite::collide(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Sprite * sprite1 = *static_cast<Sprite **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
			Sprite * sprite0 = *static_cast<Sprite **>(lua_touserdata(L, -1));
			lua_pushboolean(L, Sprite::collide(*sprite0, *sprite1));//sprite->collide(*sprite1);
		    return 1;
		}	
	}	
	lua_pushboolean(L, false);
	return 1;
}
////////////
////////////
Sprite * Sprite::to_sprite(Entity * entity) 
{
	return static_cast<Sprite *>(entity);
}
////////////
int Sprite::to_sprite(lua_State *L) 
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getglobal(L, "Sprite");
	lua_setmetatable(L, 1);
	Sprite **sprite = static_cast<Sprite **>(lua_newuserdata(L, sizeof(Sprite*)));
	*sprite = new Sprite;
	lua_setfield(L, 1, "udata");
	return 0;
}
////////////
////////////
