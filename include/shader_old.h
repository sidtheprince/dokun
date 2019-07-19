#ifndef _SHADER
#define _SHADER

#include "platform.h"
#include "types.h"
#include "logger.h"

#ifdef __cplusplus // if c++ // if c++
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <lua.hpp>

class Shader 
{
    public:
		// Constructor
		Shader();                                          static int new_(lua_State *L);
		Shader(int type); 
		Shader(int type, const char * const * source); 
		// Destructor
        ~Shader();
       // Normal.
	    void create(int type);                             static int create(lua_State *L);
		void create(int type, const char * const * source);
		bool load(const std::string& file_name);                  static int load(lua_State *L);
		void compile();  static int compile(lua_State *L);
		void destroy(); static int destroy(lua_State *L);
		// setters
		void set_source(const char * const * source );     static int set_source(lua_State *L);
		// getters
		unsigned int get_id(); static int get_id(lua_State *L);
		const char * const * get_source();                 static int get_source(lua_State *L);
		std::string get_type();                            static int get_type(lua_State *L);
		/////////////////////////////////////////////////////////////
        // Newer functions ! - june 8 2017
		static void use    (unsigned int program);	
		static void disable(unsigned int program);
        static void destroy(unsigned int program);		
		// set - june 28 2017
		void set_program(unsigned int program); static int set_program(lua_State *L);
		// get - june 28 2017
		unsigned int get_program() const;    static int get_program(lua_State *L);
		// boolean
		bool is_shader();                                  static int is_shader(lua_State *L);
private:
		unsigned int id;         // handle for the shader
		const char * const * source;     // The shader source code (i.e. the GLSL code itself)
        std::string fsource; // file source
		unsigned int program; // will be Shader handle
		static bool once;
};
class Program 
{
    public:
		// Constructor
		Program();                                                   static int new_(lua_State *L);
		Program(Shader& vertex_shader, Shader& fragment_shader);
        // Destructor
		~Program();
		void create();                                               static int create(lua_State *L);
		// Method to attach a shader to the shader program
		void attach(Shader& shader);                                 static int attach(lua_State *L);
        void detach(Shader& shader);                                 static int detach(lua_State *L);
		// Method to link the shader program and display the link status
		void link();                                                 static int link(lua_State *L);
			// Method to enable the shader program
		void use();                                                  static int use(lua_State *L);
		void bind(unsigned int index, const std::string& attribute_name);   static int bind(lua_State *L);
		// Method to disable the shader program (will still exist)
		void disable();                                              static int disable(lua_State *L);
		void destroy();                                              static int destroy(lua_State *L);
		// default program
		void set_default();                                          static int set_default(lua_State *L);
		// attribute and uniform locations
		unsigned int get_attribute(const std::string& attribute)const;           static int get_attribute(lua_State *L);
		unsigned int get_uniform(const std::string& uniform)const;               static int get_uniform(lua_State *L);
		//Changing the value of a attribute
		void set_attribute(const std::string& attribute_name, int value);   static int set_attribute(lua_State *L);
		void set_attribute(const std::string& attribute_name, double x, double y);
		void set_attribute(const std::string& attribute_name, double x, double y, double z);
		void set_attribute(const std::string& attribute_name, double x, double y, double z, double w);  
		//Changing the value of a uniform
		void set_uniformi(const std::string& uniform_name, int value);       static int set_uniform(lua_State *L);
        void set_uniformi(const std::string& uniform_name, int x, int y);
		void set_uniformi(const std::string& uniform_name, int x, int y, int z);		
		void set_uniformi(const std::string& uniform_name, int x, int y, int z, int w); 
        		
		void set_uniform(const std::string& uniform_name, double value);
        void set_uniform(const std::string& uniform_name, double x, double y);
		void set_uniform(const std::string& uniform_name, double x, double y, double z);		
		void set_uniform(const std::string& uniform_name, double x, double y, double z, double w);  
		// uniform arrays
		void set_uniform(const std::string& uniform_name, int count, const double * value); // takes in an array of 4 values
		// uniform matrices
		void set_uniform_matrix(const std::string& uniform_name, int count, bool transpose, const double * value); // takes in a matrix
		//void set_uniform(const std::string& uniform_name, );
		unsigned int get_id()const;                                       static int get_id(lua_State *L);
		Shader * get_shader(int index)const;
		static Program * get_default();                              static int get_default(lua_State *L);
		unsigned int get_size();                                     static int get_size(lua_State *L);
		// boolean
        bool is_program();                                           static int is_program(lua_State *L);		
    private:
        unsigned int id;   // handle for program	
		static Program *p_default;
		std::vector<Shader*> shader; // attached shaders
};
#endif
#endif