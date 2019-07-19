#include "../include/light.h"

////////////////////////
Light::Light() : type("positional"), enabled(false), ambient(0, 0, 0, 1), diffuse(1, 1, 1, 1), specular(1, 1, 1, 1), position(0, 0, 1, 0)
, spot_direction(0, 0, -1), spot_exponent(0), spot_cutoff(180), attenuation(1, 0, 0)
{
    Factory::get_light_factory()->store(this);
}
////////////////////////
Light::Light(const std::string& type) : enabled(false)
{
	if(String::lower(type) == "directional")
	{
		set_type("directional");
		position.w = 0.0; //  If the last value, w, is zero, the corresponding light source is a directional one
	}
	if(String::lower(type) == "positional" || String::lower(type) == "point" || String::lower(type) == "sun" || String::lower(type) == "lamp")
	{
		set_type("positional");
		position.w = 1.0; // If the w value is nonzero, the light is positional
	}	
	if(String::lower(type) == "spot" || String::lower(type) == "spotlight" || String::lower(type) == "flashlight")
	{
		set_type("spot");
		spot_cutoff = 45.0;
		
	}
	Factory::get_light_factory()->store(this);
}
////////////////////////
Light::~Light()
{
	Factory::get_light_factory()->release(this);
}
////////////////////////
void Light::on() 
{
	set_enabled(true);
}
////////////////////////
int Light::on(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->on();
	}
	return 0;
}
////////////////////////
void Light::off()
{
	set_enabled(false);
}
////////////////////////
int Light::off(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->off();
	}    
	return 0;
}
////////////////////////
void Light::draw()
{
	if(is_enabled()) // light is on
	{
		// Generate texture if not yet generated
		texture->generate();
		// draw ..
		//Renderer::draw_light(mode, ambient, diffuse, specular, position, 
		    //spot_direction, spot_exponent, spot_cutoff, attenuation);
	}
}
////////////////////////
int Light::draw(lua_State *L)
{
	return 0;
}
////////////////////////
////////////////////////
// SETTERS
void Light::set_type(const std::string& type)
{
	this->type = type;
}
////////////////////////
int Light::set_type(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TNUMBER)
		{
		    //light->set_type(static_cast<int>(lua_tonumber(L, 2)));
	    }
		if(lua_type(L, 2) == LUA_TSTRING)
		{
			light->set_type(lua_tostring(L, 2));
		}
	}   
    return 0;		
}
////////////////////////
void Light::set_enabled(bool enabled)
{
	this->enabled = enabled;
} 
////////////////////////
int Light::set_enabled(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_enabled(lua_toboolean(L, 2));
	}   
    return 0;	
}
////////////////////////
/* void Light::set_mode(int mode)
{
	this->mode = mode;
} 
////////////////////////
int Light::set_mode(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_mode(static_cast<int>(lua_tonumber(L, 2)));
	}   
    return 0;	
} */
////////////////////////
void Light::set_ambient(double red, double green, double blue, double alpha)
{
    ambient = Vector4(red, green, blue, alpha);
}
////////////////////////
void Light::set_ambient(const Vector3& ambient)
{
	set_ambient(ambient.x, ambient.y, ambient.z);
}
void Light::set_ambient(const Vector4& ambient)
{
	set_ambient(ambient.x, ambient.y, ambient.z, ambient.w);
}
////////////////////////
int Light::set_ambient(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_ambient(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5) );
	}
	return 0;
}
////////////////////////
void Light::set_diffuse(double red, double green, double blue, double alpha)
{
    diffuse = Vector4(red, green, blue, alpha);
}
////////////////////////
void Light::set_diffuse(const Vector3& diffuse)
{
	set_diffuse(diffuse.x, diffuse.y, diffuse.z);
}
void Light::set_diffuse(const Vector4& diffuse)
{
	set_diffuse(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
}
////////////////////////
int Light::set_diffuse(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_diffuse(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5) );
	}
	return 0;
}
////////////////////////
void Light::set_specular(double red, double green, double blue, double alpha)
{
	specular = Vector4(red, green, blue, alpha);
}
////////////////////////
void Light::set_specular(const Vector3& specular)
{
	set_specular(specular.x, specular.y, specular.z);
}
void Light::set_specular(const Vector4& specular)
{
	set_specular(specular.x, specular.y, specular.z, specular.w);
}
////////////////////////
int Light::set_specular(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_specular( lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5) );
	}
	return 0;
}
////////////////////////
void Light::set_position(double x, double y, double z, double w)
{
    position = Vector4(x, y, z, w);
}
////////////////////////
void Light::set_position(const Vector3& position)
{
	set_position(position.x, position.y, position.z);
}
void Light::set_position(const Vector4& position)
{
	set_position(position.x, position.y, position.z, position.w);	
}
////////////////////////
int Light::set_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_position( lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5) );
	}
	return 0;
}
////////////////////////
void Light::set_spot_direction(double x, double y, double z)
{
	spot_direction = Vector3(x, y, z);
}
////////////////////////
int Light::set_spot_direction(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_spot_direction( lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	}
	return 0;
}
////////////////////////
void Light::set_spot_exponent(double exponent)
{
    spot_exponent = exponent;
}
////////////////////////
int Light::set_spot_exponent(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_spot_exponent(lua_tonumber(L, 2));
	}
	return 0;
}
////////////////////////
void Light::set_spot_cutoff(double cutoff)
{
    spot_cutoff = cutoff;
}
////////////////////////
int Light::set_spot_cutoff(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_spot_cutoff(lua_tonumber(L, 2));
	}
	return 0;
}
////////////////////////
void Light::set_constant(double constant)
{
    attenuation.x = constant;
}
////////////////////////
int Light::set_constant(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_constant(lua_tonumber(L, 2));
	}
	return 0;
}
////////////////////////
void Light::set_linear(double linear)
{
    attenuation.y = linear;
}
////////////////////////
int Light::set_linear(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_linear(lua_tonumber(L, 2));
	}
	return 0;
}
////////////////////////
void Light::set_quadratic(double quadratic)
{
    attenuation.z = quadratic;
}
////////////////////////
int Light::set_quadratic(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		light->set_quadratic(lua_tonumber(L, 2));
	}
	return 0;
}		
////////////////////////
void Light::set_attenuation(double constant, double linear, double quadratic)
{
	attenuation = Vector3(constant, linear, quadratic);
}
////////////////////////
int Light::set_attenuation(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); 
	luaL_checktype(L, 3, LUA_TNUMBER); 
	luaL_checktype(L, 4, LUA_TNUMBER); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
        light->set_attenuation( lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	}
	return 0;
}
////////////////////////
void Light::set_texture(const Texture& texture)
{
	this->texture = &const_cast<Texture&>(texture);
}
////////////////////////
int Light::set_texture(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // light
    luaL_checktype(L, 2, LUA_TTABLE); // texture	   
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Texture * texture = *static_cast<Texture **>(lua_touserdata(L, -1));
	    lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
			Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
			light->set_texture(*texture);
			// set in lua as well
			lua_pushvalue(L, 2);
			lua_setfield(L, 1, "texture");
		}		
	}	
	return 0;
}
////////////////////////
// GETTERS
////////////////////////
std::string Light::get_type() const
{
	return type;
}
////////////////////////
int Light::get_type(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushstring(L, light->get_type().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;	
}
////////////////////////
/*
unsigned int Light::get_mode() const
{
    return mode;
}
////////////////////////
int Light::get_mode(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_mode() );
		return 1;
	}
	lua_pushnil(L);
	return 1;
}*/
////////////////////////
Vector4 Light::get_ambient() const
{
    return ambient;
} 
////////////////////////
int Light::get_ambient(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_ambient().x);
		lua_pushnumber(L, light->get_ambient().y);
		lua_pushnumber(L, light->get_ambient().z);
		lua_pushnumber(L, light->get_ambient().w);
		return 4;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;
}
////////////////////////
Vector4 Light::get_diffuse() const
{
    return diffuse;
}
////////////////////////
int Light::get_diffuse(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_diffuse().x);
		lua_pushnumber(L, light->get_diffuse().y);
		lua_pushnumber(L, light->get_diffuse().z);
		lua_pushnumber(L, light->get_diffuse().w);
		return 4;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;
}
////////////////////////
Vector4 Light::get_specular() const
{
    return specular;
}
////////////////////////
int Light::get_specular(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_specular().x);
		lua_pushnumber(L, light->get_specular().y);
		lua_pushnumber(L, light->get_specular().z);
		lua_pushnumber(L, light->get_specular().w);
		return 4;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;
}
////////////////////////
Vector4 Light::get_position() const
{
    return position;
}
////////////////////////
int Light::get_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_position().x);
		lua_pushnumber(L, light->get_position().y);
		lua_pushnumber(L, light->get_position().z);
		lua_pushnumber(L, light->get_position().w);
		return 4;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 4;
}
////////////////////////	
Vector3 Light::get_spot_direction() const
{
    return spot_direction;
}
////////////////////////
int Light::get_spot_direction(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_spot_direction().x);
		lua_pushnumber(L, light->get_spot_direction().y);
		lua_pushnumber(L, light->get_spot_direction().z);
		return 3;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}
////////////////////////
double Light::get_spot_exponent() const
{
    return spot_exponent;
}
////////////////////////
int Light::get_spot_exponent(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_spot_exponent());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////////////////
double Light::get_spot_cutoff() const
{
    return spot_cutoff;
}
////////////////////////
int Light::get_spot_cutoff(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_spot_cutoff());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////////////////
double Light::get_constant() const
{
    return attenuation.x;
}
////////////////////////
int Light::get_constant(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_constant());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////////////////
double Light::get_linear() const
{
    return attenuation.y;
}
////////////////////////
int Light::get_linear(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_linear());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////////////////
double Light::get_quadratic() const
{
    return attenuation.z;
}
////////////////////////
int Light::get_quadratic(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_quadratic());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
////////////////////////
Vector3 Light::get_attenuation() const
{
    return attenuation;
}
////////////////////////
int Light::get_attenuation(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Light * light = *static_cast<Light **>(lua_touserdata(L, -1));
		lua_pushnumber(L, light->get_attenuation().x); // constant
		lua_pushnumber(L, light->get_attenuation().y); // linear
		lua_pushnumber(L, light->get_attenuation().z); // quadratic
		return 3;
	}     
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}
////////////////////////
Texture * Light::get_texture()const
{
	return texture;
}
int Light::get_texture(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "texture");
    return 1;	
}
////////////////////////
// BOOLEAN
bool Light::is_enabled()
{
	return (enabled == true);
}
////////////////////////
int Light::is_enabled(lua_State *L)
{
	return 0;
}
////////////////////////
////////////////////////
int Light::new_(lua_State *L)
{
    unsigned int mode = 0;
	if(lua_type(L, -1) == LUA_TNUMBER)
	{
	    mode = (unsigned int)lua_tonumber(L, -1);
	}
    lua_settop(L, 0);
	// Create table.
	lua_createtable(L, 0, 0);
	// Set metatable.
	lua_getglobal(L, "Light_mt");
	lua_setmetatable(L, 1);
	// Set userdata.
	Light ** light = static_cast<Light **>(lua_newuserdata(L, sizeof(Light *)));
	* light = new Light();
	lua_setfield(L, 1, "udata");
	// Return table.
	if(lua_istable(L, -1)) 
	{
	    return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
