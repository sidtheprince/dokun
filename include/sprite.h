#ifndef _SPRITE
#define _SPRITE

#include "entity.h" // abstact base class
#ifdef __cplusplus  // if c++
//extern "C" {      // run as c code
#endif
    #include <Box2D/Box2D.h>            // 2d physics (box2D)
#ifdef __cplusplus
//}
#endif

#ifdef __cplusplus // if c++ // if c++
#include <iostream>
#include <string>
#include <vector>
#include <lua.hpp> // lua

class Sprite : public Entity 
{
    public:  // C++                                      // Lua
	    Sprite(void);                                         static int sprite_new(lua_State *L);      
		Sprite(const Sprite& sprite);
		Sprite(const Texture& texture);
		Sprite(const std::string& file_name);
		~Sprite();                   
	    bool load(const std::string& file_name);          static int load(lua_State *L); // loads a single-imaged sprite
		bool load(const std::string& file_name, int x, int y, int left, int top);
		void draw(int frame = 0);                                      static int draw(lua_State *L);
		void translate(double x, double y);               static int translate(lua_State *L);
		void translate(const Vector2& position);  
		void rotate(double degrees);                      static int rotate(lua_State *L); 
		void scale(double sx, double sy);                 static int scale(lua_State *L);
		void scale(const Vector2& s);      
		void shear(double shx, double shy);               static int shear(lua_State *L); // bonus function
		void shear(const Vector2& sh);  
		void reflect(bool x, bool y);                     static int reflect(lua_State *L);	// flips the sprite
		void resize(int width, int height);               static int resize(lua_State *L);		// resizes the sprite
        void lock(int window_width, int window_height);   static int lock(lua_State *L);
		void lock(const Vector2& window_size);
		static void lock_all(int window_width, int window_height); static int lock_all(lua_State *L);
		void add(const Texture& texture);                 static int add(lua_State * L);
		// setters
		void set_position(double x, double y);            static int set_position(lua_State *L);
        void set_position(const Vector2& position);             
		void set_angle(double angle);                     static int set_angle(lua_State *L); // returns angle (degrees)
		void set_scale(double sx, double sy);             static int set_scale(lua_State *L);
		void set_texture(const Texture& texture, int index=0);         static int set_texture(lua_State *L);               
		void set_size(int width, int height);             static int set_size(lua_State *L); // resizes the texture in (pixels)
		void set_color(int red, int green, int blue, int alpha = 255); static int set_color(lua_State *L); // main material color (RGB)
		void set_color(const Vector3& color);
		void set_color(const Vector4& color);
		void set_origin(double x, double y);
		void set_origin(const Vector2& origin);           static int set_origin(lua_State * L);            // Sets the origin in relation to the sprite's position for scaling and rotation.
		// getters	
		Vector2 get_position() const;                           static int get_position(lua_State *L); // in units
		double get_angle() const;                               static int get_angle(lua_State *L); // in degrees
        Vector2 get_scale() const;                              static int get_scale(lua_State *L);
		Texture * get_texture(int index=0) const;               static int get_texture(lua_State *L);
		int get_width() const;                                  static int get_width(lua_State *L);
		int get_height() const;                                 static int get_height(lua_State *L);
		int get_depth() const;                                  static int get_depth(lua_State *L);
		Vector2 get_size() const;                               static int get_size(lua_State *L); // in pixels
		Vector4 get_color() const;                              static int get_color(lua_State *L); // in RBG			
		Vector2 get_origin() const;                             static int get_origin(lua_State *L); // gets the origin in relation to the sprite's position for scaling and rotation.
		Vector2 get_center() const;                             static int get_center(lua_State *L);
		std::vector<float> get_vertex_array();// new ! june 9 2017
		// boolean												 
		bool is_sprite();                                 static int is_sprite(lua_State *L);
		static bool is_generated();                       static int is_generated(lua_State *L);
		static bool collide(const Sprite& a, const Sprite& b);static int collide(lua_State *L);
		// conversion
		static Sprite * to_sprite(Entity * entity);       static int to_sprite(lua_State *L);  // converts an entity to a sprite     // coverts a table to a sprite object                                         
	private:
	    static void generate();                                  static int generate(lua_State *L); // generates shaders
	    void update();                                           static int update(lua_State *L);
		friend class Level; // now level can access private members
		// Geometry
        double x;
		double y;
	    double angle;
		Vector2 scale_factor;
		// Texture
        Texture * texture;
		std::vector<Texture *> map;
		int width;
		int height;
		int depth;
		Vector4 color;
		Vector2 origin; // The point a sprite rotates and dilates around
		// Physics
		b2Body * body;
		b2Fixture * fixture;
		std::vector<Sprite> sprites; // temporary
		// OpenGL
		std::vector<float> vertex_array;
		static Shader * shader;
}; 
/*
// To load an individual sprite --------------
  Sprite * sprite = new Sprite();
  sprite->load("robot.png");
  
  sprite->draw();
  
// TO load a texture atlas (A.K.A sprite sheet) -----
  Sprite * atlas = new Sprite();
  atlas->load("robot_atlas.png", 0, 0, 32, 32); // or atlas->set_texture(*new Texture("robot_atlas.png", 0, 0, 32, 32))
  int frame = 0;
  atlas->draw(frame);

// To animate a frame in main loop
    // make a vector(or array) of frames
	std::vector<int> walk_right = {8, 9, 10, 11};
	frame = frame + 1; //  change frames
	// reset frame once its reached the end
	if(frame >= 4) // or walk_right.size() which is 4
		frame = 0;
	// draw the frames
	sprite->draw(walk_right[frame]);

// Keeping mouse at center of sprite
    Mouse::set_position(sprite->get_center(), window);	
  
  // to get individual sprite_size in atlas go here: http://getspritexy.com/
*/
#endif
#endif
