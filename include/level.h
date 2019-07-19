#ifndef _LEVEL
#define _LEVEL
// sky_box textures
#define SKY_FRONT 0						// Give Front ID = 0
#define SKY_BACK  1						// Give Back  ID = 1
#define SKY_LEFT  2						// Give Left  ID = 2
#define SKY_RIGHT 3						// Give Right ID = 3
#define SKY_UP    4						// Give Up    ID = 4
#define SKY_DOWN  5						// Give Down  ID = 5

#include "window.h"
#include "sprite.h"
#include "model.h"
#include "image.h"
#include "ui.h"
#include "camera.h"
#include "light.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Level // level, scene, planet or whatever  (controls physics, the game environment + has access to every instance, can render all entities, and do alot of cool stuff)
{
    public:
	    // constructor
	    Level();                                                static int new_(lua_State *L);
		Level(int mode);
		// destructor
		~Level();
	    // normal functions
		void add(Entity * entity);                              static int add(lua_State *L);
		void add(Camera * camera);
		void add(Light * light); //  Adds light source; directional (sun), spot (spotlight), point (lamp)
		void remove(Entity * entity);                           static int remove(lua_State *L);
		void remove(Camera * camera);
		void remove(Light * light);
		void draw();                                            static int draw(lua_State *L);// draws the entire scene and all entities
		void zoom(double zoom_factor);                          static int zoom(lua_State *L); // orthogonal or perspective zoom
        void update(const WINDOW& window);  	
		void update();
		static int update(lua_State *L); // update rendering		
		void destroy(void);
		// loading and saving scenes
		bool load(lua_State * L, const std::string& file_name);                static int load(lua_State *L);
		bool save(std::string file_name);                       static int save(lua_State *L); // new! 3-13-2016   12:57AM
		// drawing
		void draw_sky ();                                       static int draw_sky(lua_State *L); // draws a sky box
		void draw_grid();                                       static int draw_grid(lua_State *L); // draws a grid
		void draw_axis();                                       static int draw_axis(lua_State *L); // draws an arrow axis representing x, y, z
        // user interface
		void show_gui();		                                static int show_gui(lua_State *L);
		void hide_gui();                                        static int hide_gui(lua_State *L);
		// setters
		void set_active(bool active);                           static int set_active(lua_State *L);
		void set_gravity(double gravity);                       static int set_gravity(lua_State *L);
		// Lighting and Shadows -  (ambient + diffuse + specular = final illumination)
		void set_global_ambient(double r, double g, double b, double a);static int set_global_ambient(lua_State *L);	
		// Audio settings
		void set_master_volume(int volume);                     static int set_master_volume(lua_State *L);
		// Background (2d) - NEW! 12-21-2015
		void set_background(const Image& image); /* can be an image(2d only)*/ static int set_background(lua_State *L);  		
		// Sky box settings
		void set_sky_position(double x, double y, double z);    static int set_sky_position(lua_State *L);
		void set_sky_rotation(double rx, double ry, double rz); static int set_sky_rotation(lua_State *L);
		void set_sky_scale(double sx, double sy, double sz);    static int set_sky_scale(lua_State *L);
		void set_sky_texture(Texture& texture, int side) ;      static int set_sky_texture(lua_State *L);// side can be SKY_FRONT, SKY_BACK, etc.
	    // Grid properties
		void set_grid_color(double r, double g, double b);      static int set_grid_color(lua_State *L);
		void set_grid_position(double x, double y, double z);   static int set_grid_position(lua_State *L);
		// Axis properties
		void set_axis_position(double x, double y, double z);   static int set_axis_position(lua_State *L);
		void set_axis_rotation(double rx, double ry, double rz);static int set_axis_rotation(lua_State *L);
		// Graphics properties
		void set_graphics_mode(int mode);                       static int set_graphics_mode(lua_State *L);		
		// Other setters
		// getters
		static Level * get_active();                            static int get_active(lua_State *L);
		double get_gravity();                                   static int get_gravity(lua_State *L);
		Vector4 get_ambient();
		int get_volume();
		Image * get_background(); // 2d-only
		// get local objects(stored in scene)
		Entity * get_object(int id);                            static int get_object(lua_State *L);
		Entity * get_object(std::string name);	    
		Camera * get_camera(int index);                         static int get_camera(lua_State *L);
	    Light  * get_light(int index);                          static int get_light(lua_State *L);
		// get global objects (all created objects)
		static void * get_global(int id, int type = 0);         static int get_global(lua_State *L);
	    static void * get_global(std::string name);
		// count
		int get_count(const std::string& what);
		// getters (properties)
		
		// boolean
		bool is_active();                                       static int is_active(lua_State *L);
		/* 
		template<typename T>
		static inline bool is_sprite(const T& object) // works well
		{
			if (!dokun::instanceof<Sprite>(object))
	        {
	            return (false);
	        }
	        return (true);	
		}*/	
	private:
	    // access to private members
		//friend class Window;
	    friend class Entity;
		friend class Camera;
		friend class Light;
		friend class Sprite;
		friend class Model;
	    // scene properties
        double gravity; // 9.8 by default (same as Earth)
		static Level * active;
		// graphics properties
		unsigned int mode;
		// object storage
		std::vector<Entity *> entity_list;
		// camera storage
		std::vector<Camera *> camera_list;
		// light storage
		std::vector<Light *> light_list;
		///////////////////////////
		// box2d properties
		b2World * world; // creates and deletes obj; The boss of everything
	    b2Body  * ground;

		static Vector2 pixel_to_world(double x, double y);
		
        void begin_contact(b2Contact* contact);
        void end_contact  (b2Contact* contact);
		///////////////////////////
		// sky box properties
		Vector3 sky_position;
		Vector3 sky_rotation;
		Vector3 sky_scale;
		Texture * sky_texture[6];
		unsigned int sky_buffer[6];
		// grid properties
		Vector3 grid_position;
		Vector3 grid_rotation;
		Vector3 grid_scale;
		Vector3 grid_color;
		// arrow axis properties
		Vector3 axis_position;
		Vector3 axis_rotation;
		Vector3 axis_scale;
		// background properties
		Image * background;
		Vector3 background_color;
		///////////////////////////
        // zoom
		double zoom_factor;
		///////////////////////////
		// audio properties
		unsigned int master_volume;
        ///////////////////////////
		// lighting properties
		Vector4 global_ambient;
		///////////////////////////
		static Shader * scene_vertex_shader;
		static Shader * scene_fragment_shader;
		/* C++ Usage
		    Level * scene = new Scene();
			
			Sprite * sprite = (Sprite *)scene->get_global(DOKUN_SPRITE, 0);
			sprite->draw() ;

	scene->set_active(true);
    scene->add(new Camera());
	scene->add(new Light());	
	scene->add(sprite);			
			
			scene->save("scene0.txt");
		*/		
};
#endif
#endif