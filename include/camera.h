#ifndef _CAMERA
#define _CAMERA

#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

#include "vector.h"
//#include "matrix.h"
#include "shader.h"
#include "factory.h"
//#include "renderer.h"
// VS built-in compiler keywords back in the 16-bit days
#ifdef far
#undef far
#endif
#ifdef near
#undef near
#endif

#ifdef __cplusplus // if c++
#include <iostream>
#include <cmath>
#include <lua.hpp> 

class Camera 
{
    public:
	    Camera();      static int new_(lua_State *L);
		~Camera(void);
		void move(double x, double y, double z); static void move(Camera &camera, double x, double y, double z); static void move(Camera &camera, Vector3 position); static int move(lua_State *L);
		void move(const Vector3& dir);
		
		void rotate(double angle, double x, double y, double z); static void rotate(Camera &camera, double angle, double x, double y, double z); static int rotate(lua_State *L);
		void zoom(double z); static void zoom(Camera &camera, double z); static int zoom(lua_State *L);
        void zoom_in(double zoom);                                       static int zoom_in(lua_State *L);
        void zoom_out(double zoom);                                      static int zoom_out(lua_State *L);
		
		void look(double x, double y, double z); static void look(Camera &camera, double x, double y, double z); static int look(lua_State *L);
		void look(const Vector3& target); static void look(Camera &camera, const Vector3& target);
		//void look(Sprite *sprite); static void look(Camera &camera, Sprite *sprite);
		//void look(Model *model);   static void look(Camera &camera, Model *model);
		//void follow(Sprite *sprite); static void follow(Camera &camera, Sprite *sprite); static int follow(lua_State *L);
        //void follow(Model *model); static void follow(Camera &camera, Model *model);
		void draw(); static void draw(Camera &camera); static int draw(lua_State *L);
        
		
		void move_forward(double distance);
		void move_upward(double distance);
		void strafe(double distance); static void strafe(Camera &camera, double distance); static int strafe(lua_State *L);
		// setters   //void set_viewport(int x, int y, int width, int height);//instead call Renderer::set_display_size and Renderer::get_display_size and use ortho2d to zoom
		void set_default(); static void set_default(Camera &camera); static int set_default(lua_State *L);
		void set_position(double x, double y, double z); static void set_position(Camera &camera, double x, double y, double z); static int set_position(lua_State *L);
		void set_position(const Vector3& position); static void set_position(Camera &camera, Vector3 position);
		void set_view(double x, double y, double z); static void set_view(Camera &camera, double x, double y, double z); static int set_view(lua_State *L);
		void set_view(const Vector3& view); static void set_view(Camera &camera, Vector3 view);
		void set_enabled(bool enable); static int set_enabled(lua_State *L);// determines if a camera is applied to the scene or not
		// view (camera)
		void set_eye(double x, double y, double z);                 static int set_eye(lua_State *L);
		void set_eye(const Vector3& eye);
		void set_center(double x, double y, double z);              static int set_center(lua_State *L);
		void set_center(const Vector3& center);
		void set_up(double x, double y, double z);                  static int set_up(lua_State *L);
		void set_up(const Vector3& up);
		void set_right(double x, double y, double z);               static int set_right(lua_State *L);
		void set_right(const Vector3& right);
        // projection (zoom)	
		void set_zoom(double zoom);                                 static int set_zoom(lua_State *L);
        void set_ortho(double left, double right, double bottom, double top, double near, double far);      static int set_ortho(lua_State * L);
		// getters    //Vector4 get_viewport() const;//instead call Renderer::set_display_size and Renderer::get_display_size and use ortho2d to zoom
		static Camera * get_default(); static int get_default(lua_State *L);
		Vector3 get_position() const; static int get_position(lua_State *L);
		Vector3 get_rotation() const; static int get_rotation(lua_State *L);
	    Vector3 get_view    () const; static int get_view(lua_State *L);
		double get_zoom() const; static int get_zoom(lua_State *L);
		// view (camera)
		Vector3 get_eye   () const; static int get_eye(lua_State * L);
		Vector3 get_center() const; static int get_center(lua_State * L);
		Vector3 get_up    () const; static int get_up(lua_State *L);
		Vector3 get_right () const; static int get_right(lua_State *L);
		// projection (zoom) - ortho2d
		double get_left()const;
		double get_right2D()const;
		double get_bottom()const;
		double get_top()const;
		double get_near()const;
		double get_far()const;
		Vector4 get_ortho()const;
		
		// boolean
		bool is_enabled(); static int is_enabled(lua_State *L);// returns whether the camera is enabled or not
    friend std::ostream& operator << (std::ostream& os, const Camera& camera) // String s; std::cout << s ;
	{   
		os 
		<< "eye   : " << camera.get_position().x << " " << camera.get_position().y << " " << camera.get_position().z << "\n"
		<< "center: " << camera.get_view().x     << " " << camera.get_view().y     << " " << camera.get_view().z     << "\n"
		<< "up    : " << camera.get_up().x       << " " << camera.get_up().y       << " " << camera.get_up().z       << "\n"
		<< "right : " << camera.get_right().x    << " " << camera.get_right().y    << " " << camera.get_right().z    << "\n"
		//<< ": " << camera.get_() << "\n"
		<< "left  : " << camera.get_ortho().x << "\n"
		<< "right : " << camera.get_ortho().y << "\n"
		<< "bottom: " << camera.get_ortho().z << "\n"
		<< "top   : " << camera.get_ortho().w << "\n"
		<< "near  : " << camera.get_near ()   << "\n"
		<< "far   : " << camera.get_far  ()   << "\n";
		//<< ": " << camera.get_() << "\n"
		//<< ": " << camera.get_() << "\n"
		//<< ": " << camera.get_() << "\n"
		//<< ": " << camera.get_() << "\n"
		//<< ": " << camera.get_() << "\n"
		return os;
	}    
private:
		static Camera *c_default; // default camera
        //double angle;
		Vector3 view; // target
		Vector3 right_vector;
		Vector3 up_vector; // up
		Vector3 position; // position
		Vector3 rotation;
        Vector3 view_point;		
		bool enabled;
		// 2d camera - new!
        Vector4 ortho; // bottom-left(0, width, 0, height), top-left(0, width, height, 0)=default
		double  near ;
		double  far  ;
        double zoom_factor; // zoom factor // lower zoom_factor = closer, higher zoom_factor = further // + Vector3(zoom);
		
};
#endif
#endif
/*
            // Ortho2d camera (left, top) manipulation (does not affect 3d objects)
			if(Keyboard::is_pressed(DOKUN_KEY_1)) Renderer::get_camera()->set_ortho( camera->get_left() - 1, camera->get_right2D(), camera->get_bottom(), camera->get_top(), camera->get_near(), camera->get_far() );;// moves 2d camera left and right
			if(Keyboard::is_pressed(DOKUN_KEY_2)) Renderer::get_camera()->set_ortho( camera->get_left() + 1, camera->get_right2D(), camera->get_bottom(), camera->get_top(), camera->get_near(), camera->get_far() );;// moves 2d camera up and down
            // rotate
            if(Keyboard::is_pressed(DOKUN_KEY_Z)) camera->rotate(2, 0, 0, 1); // rotate z-coord     - normal rotation for 2d graphics
            if(Keyboard::is_pressed(DOKUN_KEY_X)) camera->rotate(2, 0, 1, 0); // rotate horzontally - spin-turn(not so great for 2d; good for 3d though)
            if(Keyboard::is_pressed(DOKUN_KEY_C)) camera->rotate(2, 1, 0, 0); // rotate vertically  - roll(not so great for 2d; good for 3d though)
            // zooming without changing the viewport (renderable_area) (for 2d ONLY)
            if(Keyboard::is_pressed(DOKUN_KEY_3)) camera->set_zoom(camera->get_zoom() + 1); // zoom_out
            if(Keyboard::is_pressed(DOKUN_KEY_4)) camera->set_zoom(camera->get_zoom() - 1); // zoom_in
            // moving camera
            if(Keyboard::is_pressed(DOKUN_KEY_W)) camera->set_position( camera->get_position().x, camera->get_position().y - 1, camera->get_position().z );
            if(Keyboard::is_pressed(DOKUN_KEY_A)) camera->set_position( camera->get_position().x - 1, camera->get_position().y, camera->get_position().z );
            if(Keyboard::is_pressed(DOKUN_KEY_S)) camera->set_position( camera->get_position().x, camera->get_position().y + 1, camera->get_position().z );
            if(Keyboard::is_pressed(DOKUN_KEY_D)) camera->set_position( camera->get_position().x + 1, camera->get_position().y, camera->get_position().z );
            
            
        // camera follow sprite test (will affect Mouse_position)
        int camera_height = -(window.get_client_height() / 2);// keep camera at center height of screen;  negative = up   positive = down
        camera->set_position(Vector3(sprite->get_position(), 0.0) + Vector3(-(window.get_client_width() - window.get_client_height() / 2), camera_height, 0) );//Vector3(0, camera_height, 0)//Vector3(cam_x_position_away_from_sprite, camera_height, 0)                        
*/
