#ifndef _RENDER
#define _RENDER

#include "platform.h"
#include "vector.h"
#include "matrix_test.h"//#include "matrix.h" // will replace glm once completed
#include "shader.h"
#include "texture.h"
#include "light.h"
#include "camera.h"
#include "font.h"
//------------------
#ifdef __cplusplus // if c++
#include <iostream> // March 1, 2016     12:21 PM
#include <string>
#include <vector>

#define use_glm /* remove comment to enable glm*/
#ifdef use_glm
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif


class Renderer // draws object to screen; NOTE: The renderer does not hold any kind of data values or information(except GPU info) but takes information from an object and uses that information to draw the object
{
public:	
	Renderer();
	~Renderer();
	// --------------------------
	static void start   (void); // initializes renderer
	static void destroy (void); // destroy renderer and all objects
	// entity -------------------
	static void draw_sprite(const Texture& texture, double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,// = 255,
	    const std::vector<float>& vertex_array, const Shader& shader, const std::vector<Texture *>& map); // fast (uses 2-7% CPU)
	static void draw_atlas(const Texture& texture, int width, int height, double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, int frame, const Shader& shader);
	static void draw_model (const std::vector<Vector3>& vertex_array, const std::vector<unsigned int>& element_array, const std::vector<Vector2>& uv_array, const std::vector<Vector3>& normal_array, double x, double y, double z, double rx, double ry, double rz, double sx, double sy, double sz, 
		double ox, double oy, double oz, 
		const std::vector<Texture *>& texture_array, const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector4& emission, double shininess, const Shader& shader);	
	static void draw_image(const unsigned int buffer/*Texture& texture*/, int width, int height, int depth, double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha = 255, int channel=4); // complete!
	// light --------------------
	static void draw_light(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, 
	    const Vector4& position, const Vector3& spot_direction, 
		double spot_exponent, double spot_cutoff, const Vector3& attenuation, const Texture * texture = nullptr/*const Shader& shader*/);
	// camera -------------------
	static void draw_camera(double eye_x, double eye_y, double eye_z, double center_x, double center_y,double center_z, double up_x, double up_y,double up_z, const Shader& shader);	
    // ui -----------------------
	static void draw_box(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, 
		double radius, bool iconified,
		// title bar
		bool title_bar,
		int title_bar_height,
		const Vector4& title_bar_color,
		bool title_bar_button_iconify,
        bool title_bar_button_maximize,
        bool title_bar_button_close, const Vector4& title_bar_button_close_color,
		// outline
        bool outline,
        double outline_width,
        const Vector4& outline_color,
        bool outline_antialiased,
        // border
        bool border,
        double border_radius,
        const Vector4& border_color,
		// gradient
		bool gradient,
		const Vector4& gradient_color,
		// shadow
		bool shadow
	);
	//--------------------------
	static void draw_text (const std::string& text, double x, double y, int width, int height, double angle, double scale_x, double scale_y, const FONT& font, double red, double green, double blue, double alpha = 255);
	static void draw_glyph (unsigned char glyph, double x, double y, double angle, double scale_x, double scale_y, const FONT& font, double red, double green, double blue, double alpha = 255);
	static void draw_text2 (const std::string& text, double x, double y, int width, int height, double angle, double scale_x, double scale_y, const FONT& font, double red, double green, double blue, double alpha = 255);
	//--------------------------
	static void draw_button(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, 
		// outline
		bool outline, 
		double outline_width, 
		const Vector4& outline_color,
		bool outline_antialiased
		// border
		//bool border,
		//const Vector4& border_color,		
	);
	//--------------------------
	static void draw_progressbar(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, 
		double min_value, double max_value, double value, const Vector4& background_color,
		// outline
		bool outline, 
		double outline_width, 
		const Vector4& outline_color,
		bool outline_antialiased
		// border
		//bool border,
		//const Vector4& border_color,
		);
	//--------------------------
	static void draw_edit(const std::string& text, int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	    bool multilined,
		// cursor
		bool cursor, double cursor_x, double cursor_y, int cursor_height
	);
	//--------------------------
	static void draw_slider(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,  // good!
        // beam
		double min_value, double max_value, double value, const Vector4& background_color, 
		// ball
		int ball_width, const Vector4& ball_color
	);
	//--------------------------
	static void draw_switch(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, // good!
		int value,
		const Vector4& background_color_on,
		const Vector4& background_color_off,
		// border
		bool outline,
		double outline_width,
		const Vector4& outline_color,
		bool outline_antialiased
	);
	//--------------------------
	static void draw_radio(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	    int value, Vector4& background_color,
		// outline
		bool outline,
		double outline_width,
		const Vector4& outline_color,
		bool outline_antialiased
	);
	//--------------------------
	static void draw_checkbox(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//--------------------------
	static void draw_tooltip(const std::string& text, int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//--------------------------
	static void draw_scrollbar(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	    double value,
		// handle
		int handle_height, const Vector4& handle_color,
		// button
		bool button, int button_height, const Vector4& button_color,
		// arrow
		bool arrow, const Vector4& arrow_color,
		// outline
		bool outline, 
		double outline_width, 
		const Vector4& outline_color,
		bool outline_antialiased,
		// border
		bool border
	);
	//--------------------------
	static void draw_spinner(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	    double value,
		// button
        int button_width, const Vector4& button_color, 		
		// border
		bool outline,
		double outline_width,
		const Vector4& outline_color,
		bool outline_antialiased	
	);
	//--------------------------
	static void draw_combobox(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	    const Vector4& button_color, int button_width);
	//--------------------------
	//use quad_ui for this - static void draw_menubar(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//use quad_ui for this - static void draw_grid(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	//static void draw_(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha);
	// levels and maps ---------
	static void draw_level ();
	static void draw_sky ();
	// 2d primitives -----------
	static void draw_point   ();
	static void draw_line    (double x, double y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha = 255);
	static void draw_circle  ();
	static void draw_triangle(double x, double y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha = 255);
	static void draw_quad    ();
	static void draw_polygon ();
	// 3d primitives -----------
	static void draw_cube    ();
	static void draw_sphere  ();
	static void draw_torus   ();
	static void draw_cylinder();
	static void draw_pyramid ();
	static void draw_teapot  ();
	//--------------------------
	// ...
	void display(); // shows rendering information
	// setters
	static void set_viewport(int x, int y, int width, int height);
	static void set_camera(const Camera& camera); static int set_camera(lua_State *L);// sets the current camera
	static void set_light(const Light& light);    static int set_light (lua_State *L); // sets the current light
	
	static void set_clear(double red, double green, double blue, double alpha = 255, double depth = 1.0, int stencil = 0);
	static void set_vertical_synchronization(bool v_sync);
	static void set_current_API(const std::string& API); static int set_current_API(lua_State *L);
	static void set_background_alpha(bool bg_alpha); // 0 = transparent 1 = visible
	// getters
	void print_vulkan_info();
	void get_GPU_info();	
	static Renderer * get_pointer();
	static bool get_status();
	static int get_display_width ();
	static int get_display_height();
	static Vector2i get_display_size();
	// global objects -----------
	static Camera * get_camera();
	static Light  * get_light ();
    /////////////////////
	// API
	static std::string get_current_API(void);
	static std::string get_current_API_version(void);
	static int get_current_API_version_major(void);
	static int get_current_API_version_minor(void);
	static int get_current_API_version_patch(void);
	// shader_language
	static std::string get_shader_language(void);
	static std::string get_shader_language_version(void);
	static int get_shader_language_version_major(void);
	static int get_shader_language_version_minor(void);
	static int get_shader_language_version_patch(void);
	static bool is_supported_extension(const char *extList, const char *extension);
	// GPU
	static std::string get_gpu(void);
	static std::string get_gpu_vendor(void);
	static std::string get_gpu_version(void);
	/////////////////////
	#ifdef DOKUN_VULKAN
	    static VkInstance get_instance();
	#endif
	// screen dimensions
	static unsigned int window_width ;
	static unsigned int window_height;	
	static void set_display_size(int width, int height);
    // Test
    static void draw_sprite_test(double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
    const std::vector<float>& vertex_array, const Shader& shader, const Texture& texture);
private:
    static void context_check(void);
	static void device_check (void); // NEW! added 9-3-2018
    /////////////////////////////////////////
 	static void generate_vertex_array(double x, double y, unsigned int width, unsigned int height);
	static void update_vertex_array(const std::vector<float>& vertex_array);
	static void destroy_vertex_array (void);
	/////////////////////////////////////////
    static Renderer * render_ptr; // default renderer
	static bool status;
	static std::string API;
	static Camera * camera; // default camera
	static Light  * light ; // default global light
	static Vector4 color;
	///////////////OPENGL OBJECTS///////////////////////////////
	static unsigned int sprite_vertex_array_obj;
	static unsigned int sprite_vertex_buffer_obj;
	static unsigned int sprite_texcoord_buffer_obj;
	static unsigned int sprite_element_buffer_obj;
	/////////////////////////////////////////////
	static unsigned int model_vertex_array_obj;
	//////////// SHADER PROGRAM/////////////////////////
    Shader * sprite_shader;
	Shader * model_shader;
	/// gpu data ////////////////////
	struct gpu
	{
		unsigned int count;
		std::string name;
		std::string vendor;
		std::string API_name;
		std::tuple<int, int, int> API_version;
		std::string API_version_str;
		unsigned int API_version_major;
		unsigned int API_version_minor;
		unsigned int API_version_patch;
		std::string shader_version;
		std::string shader_lang;
		unsigned int ext_count;
		std::vector<std::string> ext_list;
	} gpu;	
public:	
#ifdef DOKUN_VULKAN
	VkApplicationInfo app_info;
	VkInstanceCreateInfo instance_info;
	VkInstance instance;
	VkViewport viewport;
	VkPhysicalDeviceProperties device_prop;
#endif
};
#define DOKUN_CURRENT_API_OPENGL (Renderer::get_current_API() == "OpenGL")
#define DOKUN_CURRENT_API_VULKAN (Renderer::get_current_API() == "Vulkan")
#endif
#endif
