#ifndef _LIgreenHT
#define _LIgreenHT
#include "platform.h"
#include "types.h"
#include "vector.h"
#include "factory.h"
#include "string.h"
#include "texture.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <cmath>
#include <lua.hpp>

class Light 
{
public:
	Light();                                                           static int new_(lua_State *L);
    Light(const std::string& type);
	~Light();
	// normal
	void on();                                                         static int on(lua_State *L);
	void off();                                                        static int off(lua_State *L);
	void draw();         static int draw(lua_State *L);
	// setters
	void set_type(const std::string& type);                                           static int set_type(lua_State *L);
	void set_enabled(bool enabled);   static int set_enabled(lua_State *L);
	void set_ambient(double red, double green, double blue, double alpha = 1.0);          static int set_ambient(lua_State *L);
	void set_ambient(const Vector3& ambient);
	void set_ambient(const Vector4& ambient);
	void set_diffuse(double red, double green, double blue, double alpha = 1.0);	       static int set_diffuse(lua_State *L);
	void set_diffuse(const Vector3& diffuse);
	void set_diffuse(const Vector4& diffuse);	
	void set_specular(double red, double green, double blue, double alpha = 1.0);		   static int set_specular(lua_State *L);
	void set_specular(const Vector3& specular);
	void set_specular(const Vector4& specular);	
	void set_position(double x, double y, double z, double w = 1.0);         static int set_position(lua_State *L);
	void set_position(const Vector3& position);
	void set_position(const Vector4& position);	
	void set_spot_direction(double x, double y, double z);	           static int set_spot_direction(lua_State *L);
	void set_spot_exponent(double exponent);                           static int set_spot_exponent(lua_State *L);
	void set_spot_cutoff(double cutoff);                               static int set_spot_cutoff(lua_State *L);
	void set_constant(double constant);                                static int set_constant(lua_State *L);
	void set_linear(double linear);                                    static int set_linear(lua_State *L);
	void set_quadratic(double quadratic);                              static int set_quadratic(lua_State *L);
	void set_attenuation(double x, double y, double z);                static int set_attenuation(lua_State *L);
	void set_texture(const Texture& texture);                          static int set_texture(lua_State *L);
	// getters
	std::string get_type() const;                                            static int get_type(lua_State *L);
	Vector4 get_ambient() const;                                             static int get_ambient(lua_State *L);
	Vector4 get_diffuse() const;                                             static int get_diffuse(lua_State *L);
	Vector4 get_specular() const;                                            static int get_specular(lua_State *L);
	Vector4 get_position() const;                                            static int get_position(lua_State *L);
	Vector3 get_spot_direction() const;                                      static int get_spot_direction(lua_State *L);
	double get_spot_exponent() const;                                        static int get_spot_exponent(lua_State *L);
	double get_spot_cutoff() const;                                          static int get_spot_cutoff(lua_State *L);
	double get_constant() const;                                             static int get_constant(lua_State *L);
	double get_linear() const;                                               static int get_linear(lua_State *L);
	double get_quadratic() const;                                            static int get_quadratic(lua_State *L);
	Vector3 get_attenuation() const;                                         static int get_attenuation(lua_State *L);
    Texture * get_texture()const;                          static int get_texture(lua_State *L);	
	// boolean
	bool is_enabled();                                                      static int is_enabled(lua_State *L);
private:
	bool enabled;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 position;
	Vector3 spot_direction;
	double spot_exponent;
	double spot_cutoff;
	Vector3 attenuation;
	std::string type;
	Texture * texture;
};
#endif
#endif