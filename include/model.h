#ifndef _MODEL
#define _MODEL

#include "entity.h"
#ifdef __cplusplus // if c++ // if c++
//extern "C" {       // run as c code
#endif
    // include c_header here
	//#include <btBulletDynamicsCommon.h> // 3d physics (bullet3)
#ifdef __cplusplus
//}
#endif

#ifdef __cplusplus
#include <iostream> 
#include <string>
#include <vector>
#include <cctype> // std::to_lower
#include <fstream>
#include <sstream>
#include <lua.hpp> // lua

class Model : public Entity 
{
	public: // C++                                               // Lua
	    Model(void);                                                 static int create(lua_State *L);
		Model(const Model& model);
		Model(const std::string& file_name);
		Model(const std::string& file_name, const Texture& texture);
		Model(const std::vector<Vector3>& vertex_array, 
		    const std::vector<unsigned int>& element_array/*, const std::vector<Texture&>& texture_array*/);
		~Model();                            
		bool load(const std::string& file_name);                 static int load(lua_State *L);
		bool load(const std::vector<Vector3>& vertex_array, const std::vector<unsigned int>& element_array);
	    void draw();                                             static int draw(lua_State *L);
	    void update(void);
		void translate(double x, double y, double z);            static int translate(lua_State *L);
		void translate(const Vector3& position);    
		void rotate(double degrees, int x, int y, int z);        static int rotate(lua_State *L); // specify which axis to rotate by a certain degree // degrees = number * pi / 180
		void rotate(double degrees, const Vector3& r);
		void scale(double sx, double sy, double sz);             static int scale(lua_State *L);  // scale(1, 1) - stays the same,  scale(2, 2) - doubles, scale(0.5, 0.5) - half size
		void scale(const Vector3& s);                 
		void shear(double shx, double shy, double shz);          static int shear(lua_State *L); // bonus function
		void shear(const Vector3& sh);  
		void reflect(int x, int y, int z);                       static int reflect(lua_State *L); // bonus function
		// setters
		void set_position(double x, double y, double z);         static int set_position(lua_State *L);
		void set_position(const Vector3 & position);  
		void set_rotation(double degrees, int x, int y, int z);  static int set_rotation(lua_State *L);
		void set_rotation(double degrees, const Vector3 & R);
		void set_scale(double sx, double sy, double sz);         static int set_scale(lua_State *L);
		void set_scale(const Vector3 & S);
		void set_origin(double x, double y, double z);           static int set_origin(lua_State *L);
		void set_origin(const Vector3 & origin);
		void set_texture(const Texture& texture);                static int set_texture(lua_State *L);
		void set_texture(const Texture& texture, const std::string& type);
		void set_material(const std::string& name, double x, double y = 0.0, double z = 0.0, double w = 1.0);  static int set_material(lua_State *L); // material = ambient + diffuse + specular + emission + shininess = final_color
		void set_material(const std::string& name, const Vector3& color);
		void set_material(const std::string& name, const Vector4& color);
		// getters
		Vector3 get_position()const;  	                         static int get_position(lua_State *L); // in units
		Vector3 get_rotation()const;                                  static int get_rotation(lua_State *L); // in degrees
        Vector3 get_scale()const;                                     static int get_scale(lua_State *L);
		Vector3 get_origin()const;                                    static int get_origin(lua_State *L);
		Texture * get_texture(int index = 0)const;                    static int get_texture(lua_State *L);	
		Vector3 get_size()const;                                      static int get_size(lua_State *L); // in pixels		
        //////////
		Vector3 get_center()const;
		double  get_radius()const;
        ///////////		
		Vector4 get_material(const std::string& name)const;                                  static int get_material(lua_State *L);
		double get_shininess()const;
		// boolean
		bool is_model()const;                                         static int is_model(lua_State *L);  // a valid model is created through Model:new() or tomodel()
		// conversion
		static Model * to_model(Entity * entity);                static int to_model(lua_State *L); // convert entity to model	
	    // data
		std::vector<Vector3> get_vertex_array() const;                 static int get_vertex_array(lua_State *L);// get_vertex()[0], get_vertex().size()
		std::vector<Vector2> get_uv_array() const;                     static int get_uv_array(lua_State *L);
		std::vector<Vector3> get_normal_array() const;                 static int get_normal_array(lua_State *L);
		std::vector<unsigned int> get_element_array(int index)const;  static int get_element_array(lua_State *L);// 0=v, 1=vt, 2=vn
	    std::vector<Texture *> get_texture_array()const;                    static int get_texture_array(lua_State *L);
	private:
	    friend class Data; // class Data can now access Model's private members
	    void generate();
		void map();  static int map(lua_State *L); 
	    // model format(s)
		// Open-source
		bool load_obj(const std::string& file_name); // wavefront
		void load_obj(const std::string& file_name, dokun_Error * error);
		bool load_dae(const std::string& file_name); // collada
		void load_dae(const std::string& file_name, dokun_Error * error);
		// Proprietary
		// material format(s)		
	    bool load_mtl(const std::string& file_name);  static int load_mtl(lua_State *L);
	   // Geometry
	    double x;
		double y;
		double z;
		Vector3 rotation;
		Vector3 scale_factor;	
        Vector3 origin;		
		// Texture
		std::vector<Texture *> texture_list;  // supports multiple textures	
        // Render
		struct {
			std::vector<Vector3> vertex;
			std::vector<Vector2> uv;
			std::vector<Vector3> normal;
		} vertices;
		struct {
			std::vector<unsigned int> vertex;
			std::vector<unsigned int> uv;
			std::vector<unsigned int> normal;
		} elements; 
		struct {
			Vector4 ambient;
			Vector4 diffuse;
			Vector4 specular;
			Vector4 emission;
			double shininess;
			Vector3 color_indexes; // default: 0, 1, 1
		} material;
		// Opengl
		static Shader * shader;
};
#endif
#endif