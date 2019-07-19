#include "../include/renderer.h"

Renderer::Renderer() // don't initialize anything in constructor or Renderer will crash
{}
////////////
Renderer::~Renderer()
{}
////////////
////////////
Renderer * Renderer::render_ptr (new Renderer());
Camera   * Renderer::camera     (new Camera  ());
Light    * Renderer::light      (new Light   ());
////////////
Vector4 Renderer::color(32, 32, 32, 255);
////////////
unsigned int Renderer::window_width (DOKUN_DEFAULT_WIDTH );
unsigned int Renderer::window_height(DOKUN_DEFAULT_HEIGHT);	
////////////
bool Renderer::status     (false); 
////////////
std::string Renderer::API ("OpenGL");
////////////
unsigned int Renderer::sprite_vertex_array_obj   (0);
unsigned int Renderer::sprite_vertex_buffer_obj  (0);
unsigned int Renderer::sprite_texcoord_buffer_obj(0);
unsigned int Renderer::sprite_element_buffer_obj (0);

unsigned int Renderer::model_vertex_array_obj    (0);
////////////
////////////
////////////
////////////
////////////
////////////
////////////
void Renderer::start(void){ // initialize default values for renderer
    light->set_enabled(false); // light is off by default (light on means texture will turn black)
}
void Renderer::destroy (void){ // call this function to deallocate attached renderer objects before deleting renderer
}
void Renderer::generate_vertex_array(double x, double y, unsigned int width, unsigned int height) // sprites not showing up when using this :(
{
#ifdef DOKUN_OPENGL
    if(!glIsVertexArray(sprite_vertex_array_obj)) // if vao has not been generated
	{
	// vertex array obj -
    glGenVertexArrays(1, &sprite_vertex_array_obj);
    // vertex buffer obj
	glBindVertexArray(sprite_vertex_array_obj); // bind	
	    GLuint sprite_vertex_buffer_obj;
	    glGenBuffers(1, &sprite_vertex_buffer_obj);
		std::vector<GLfloat> vertex_array; // vertex array (C++) 
		vertex_array.push_back(x);
		vertex_array.push_back(y);
		vertex_array.push_back(x + static_cast<GLfloat>(width));
		vertex_array.push_back(y);
		vertex_array.push_back(x + static_cast<GLfloat>(width));
		vertex_array.push_back(y + static_cast<GLfloat>(height));
		vertex_array.push_back(x);
		vertex_array.push_back(y + static_cast<GLfloat>(height));
		/*GLfloat vertex_array[] ={ // vertex array (C)
			static_cast<float>(x)                                     , static_cast<float>(y),
		    static_cast<float>(x) + static_cast<GLfloat>(width)       , static_cast<float>(y),
	 	    static_cast<float>(x) + static_cast<GLfloat>(width)       , static_cast<float>(y) + static_cast<GLfloat>(height),
		    static_cast<float>(x)                                     , static_cast<float>(y) + static_cast<GLfloat>(height),
		};*/
        glBindBuffer(GL_ARRAY_BUFFER, sprite_vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, vertex_array.size()/*sizeof(vertex_array)*/ * sizeof(GLfloat), /*vertex_array*/vertex_array.data(), GL_DYNAMIC_DRAW); // DYNAMIC cause it will update often
		glEnableVertexAttribArray(0); // vertex attribute ptr
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));	
	glBindVertexArray(0); // unbind
	// texcoord buffer obj
	glBindVertexArray(sprite_vertex_array_obj); // bind again  
	    GLuint sprite_texcoord_buffer_obj;
        glGenBuffers(1, &sprite_texcoord_buffer_obj);
		GLfloat tex_coord[] = {0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0,};
        glBindBuffer(GL_ARRAY_BUFFER, sprite_texcoord_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord) * sizeof(GLfloat), tex_coord, GL_STATIC_DRAW); 
		glEnableVertexAttribArray(1); // vertex attribute ptr
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));			
	glBindVertexArray(0); // unbind	
	// element buffer 
	glBindVertexArray(sprite_vertex_array_obj); // bind
	    GLuint sprite_element_buffer_obj;
		glGenBuffers(1, &sprite_element_buffer_obj);
		GLuint element[] = {0, 1, 3,  1, 2, 3};//{0,1,2, 2,3,0};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_element_buffer_obj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element) * sizeof(GLuint), element, GL_STATIC_DRAW);
	glBindVertexArray(0); // unbind	
	Logger("Sprite vertex array and buffer objects generated");
	}
#endif	
}
////////////
void Renderer::update_vertex_array(const std::vector<float>& vertex_array)
{
#ifdef DOKUN_OPENGL
	// Update vertex_buffer_obj (each frame) NOT the VAO!!
	// This code is equivalent to glNamedBufferSubData
    /*glBindBuffer(GL_ARRAY_BUFFER, sprite_vertex_buffer_obj);
	    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_array.size() * sizeof(GLfloat), vertex_array.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
	glNamedBufferSubData(sprite_vertex_buffer_obj, 0, vertex_array.size() * sizeof(GLfloat), vertex_array.data()); Logger("Sprite vertex data updated");// updates buffer_obj without having to bind it	
#endif	
}
////////////
void Renderer::destroy_vertex_array (void) // do not destroy vertex array until object is no longer in use or the endgine closes
{
#ifdef DOKUN_OPENGL	
	// delete the buffers
	glDeleteBuffers(1, &sprite_vertex_buffer_obj  );
	glDeleteBuffers(1, &sprite_texcoord_buffer_obj);	
	glDeleteBuffers(1, &sprite_element_buffer_obj );
    // delete the arrays
	glDeleteVertexArrays(1, &sprite_vertex_array_obj);	Logger("Sprite vertex data destroyed");
#endif	
}
////////////
////////////
void Renderer::context_check(void)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
    if(get_current_API() != "OpenGL") // but you don't set it as the current API, perhaps Vulkan is the current API?
	{
		Logger("Context check failed : \
		    OpenGL not set as current API");
		return;
	}
#ifdef __windows__
    if(!wglGetCurrentContext())
	{
		Logger("Context check failed : \
		    No OpenGL Context was found.");
		return;
	}
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Context check failed : \
		    No OpenGL Context was found.");
		return;
	}	
#endif
#endif
#endif	
}
////////////
////////////
void Renderer::device_check (void)
{
#ifdef DOKUN_VULKAN
	if(get_current_API() == "Vulkan")
	{/*
	    // Query how many devices are present in the system
	    uint32_t device_count = 0;
        VkResult result = vkEnumeratePhysicalDevices(get_pointer()->instance, &device_count, nullptr);
        if(result != VK_SUCCESS) {
            fprintf(stderr, "Failed to query the number of physical devices present: %d\n", result);
            abort();
        }
        // There has to be at least one device present
        if (device_count == 0) {
            fprintf(stderr, "Couldn't detect any device present with Vulkan support: %d\n", result);
            abort();
        }
        // Get the physical devices
        std::vector<VkPhysicalDevice> physical_devices(device_count);
        result = vkEnumeratePhysicalDevices(instance, &deviceCount, &physical_devices[0]);
        if (result != VK_SUCCESS) {
            fprintf(stderr, "Faied to enumerate physical devices present: %d\n", result);
            abort();
        }*/
	/*
		// look for devices and gpus that support vulkan
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, & extension_count, nullptr);

        std::cout << extension_count << " extensions supported" << std::endl;	
		
		VkDevice device;
		VkPhysicalDevice physical_device;
		uint32_t physical_device_count = 0;
		std::vector<VkPhysicalDevice> physical_device_list(physical_device_count);
		// check how many devices we have
		VkResult result = vkEnumeratePhysicalDevices(get_pointer()->instance, & physical_device_count, & physical_device);
		assert(result == VK_SUCCESS);
		std::cout << physical_device_count << " devices found" << std::endl;
	    // device properties
		//VkPhysicalDeviceProperties device_prop = {};
		vkGetPhysicalDeviceProperties(physical_device, & get_pointer()->device_prop);   // for single device
		*/
	}
#endif	
}
////////////
void Renderer::draw_sprite_test(double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
    const std::vector<float>& vertex_array, const Shader& shader, const Texture& texture)
{
    // Generate vertex array and buffer objects ONE TIME!
    Renderer::generate_vertex_array(x, y, texture.get_width(), texture.get_height());
			// Update vertex array and buffer objects
	Renderer::update_vertex_array(vertex_array);
	// Enable or disable states
	glDisable(GL_DEPTH_TEST); // Disable 3d
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);	
	// Shader
	glUseProgram(shader.get_program()); // push program
	// Transform
#ifdef use_glm
	// model (object)
	glm::mat4 model = glm::mat4(1.0f); // WARNING: ALLOCATING IN LOOP CAUSES SLOW RENDERING!!
	model = glm::translate(model, glm::vec3(glm::vec2(x, y), -1.0f));  // z must be negative(e.g -1) if using the 'view' matrix
	model = glm::rotate(model, static_cast<GLfloat>(angle * 0.0174533), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(glm::vec2(static_cast<GLfloat>(scale_x), static_cast<GLfloat>(scale_y)), 1.0f));	
	model = glm::translate(model, glm::vec3(glm::vec2(-x, -y), 1.0f));
    // view (camera)     // WARNING: ALLOCATING IN LOOP CAUSES SLOW RENDERING!!
    glm::vec3 eye    = glm::vec3(camera->get_position().x, camera->get_position().y, camera->get_position().z);
    glm::vec3 center = glm::vec3(camera->get_position().x + camera->get_view().x, camera->get_position().y + camera->get_view().y, camera->get_position().z + camera->get_view().z);
	glm::vec3 up     = glm::vec3(camera->get_up().x  , camera->get_up().y  , camera->get_up().z);	
	glm::mat4 view   = glm::lookAt(eye, center, up); // glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0));
	// projection (zoom) // WARNING: ALLOCATING IN LOOP CAUSES SLOW RENDERING!!
	glm::mat4 proj = glm::ortho(static_cast<GLfloat>(camera->get_left()), static_cast<GLfloat>(window_width), static_cast<GLfloat>(window_height),
 	    static_cast<GLfloat>(camera->get_top()), static_cast<GLfloat>(camera->get_near()), static_cast<GLfloat>(camera->get_far()));//glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);//glm::ortho(0.0f, static_cast<float>(window_width), 0.0f, static_cast<float>(window_height), -1.0f, 1.0f); /// bottom-left(0, width, 0, height), top-left(0, width, height, 0)=default for dokun	

	if(shader.get_uniform("model") != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, false, glm::value_ptr(model));
	if(shader.get_uniform("proj" ) != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj" ), 1, false, glm::value_ptr(proj) );
	if(shader.get_uniform("view" ) != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view" ), 1, false, glm::value_ptr(view) );		
#endif
    // Light (global)
    if(shader.get_uniform("light.ambient" ) != -1) glUniform4f(shader.get_uniform("light.ambient" ), light->get_ambient ().x, light->get_ambient ().y, light->get_ambient ().z, light->get_ambient ().w);
    if(shader.get_uniform("light.diffuse" ) != -1) glUniform4f(shader.get_uniform("light.diffuse" ), light->get_diffuse ().x, light->get_diffuse ().y, light->get_diffuse ().z, light->get_diffuse ().w); // Let's darken the light a bit to fit the scene
    if(shader.get_uniform("light.specular") != -1) glUniform4f(shader.get_uniform("light.specular"), light->get_specular().x, light->get_specular().y, light->get_specular().z, light->get_specular().w);  
	if(shader.get_uniform("light.position") != -1) glUniform4f(shader.get_uniform("light.position"), light->get_position().x, light->get_position().y, light->get_position().z, light->get_position().w);
	if(shader.get_uniform("light.spot_direction") != -1) glUniform3f(shader.get_uniform("light.spot_direction"), light->get_spot_direction().x, light->get_spot_direction().y, light->get_spot_direction().z);
	if(shader.get_uniform("light.spot_exponent" ) != -1) glUniform1f(shader.get_uniform("light.spot_exponent" ), light->get_spot_exponent ());
	if(shader.get_uniform("light.spot_cutoff"   ) != -1) glUniform1f(shader.get_uniform("light.spot_cutoff"   ), light->get_spot_cutoff   ());
	if(shader.get_uniform("light.attenuation"   ) != -1) glUniform3f(shader.get_uniform("light.attenuation"   ), light->get_attenuation   ().x, light->get_attenuation   ().y, light->get_attenuation   ().z);	 								  
	if(shader.get_uniform("light.is_enabled"    ) != -1) glUniform1i(shader.get_uniform("light.is_enabled"    ), light->is_enabled());	 								  
	// Camera (global)
	if(shader.get_uniform("camera.position") != -1) glUniform3f(shader.get_uniform("camera.position"), camera->get_position().x, camera->get_position().y, camera->get_position().z);
	// Miscellaneous
	if(shader.get_uniform("color") != -1) glUniform4f(glGetUniformLocation(shader.get_program(), "color"), (red / 255), (green / 255), (blue / 255), (alpha / 255));
	if(shader.get_uniform("map.is_texture") != -1) glUniform1i(shader.get_uniform("map.is_texture"), (texture.get_data() != nullptr)); 
	if(shader.get_uniform("resolution") != -1) glUniform2f(shader.get_uniform("resolution"), Renderer::get_display_width(), Renderer::get_display_height());	
	// Texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture.get_buffer()); // bind texture (base)
	const_cast<Shader&>(shader).set_integer("map.diffuse", 0);
	// Draw
	glBindVertexArray(sprite_vertex_array_obj);                // bind vertex array
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);      //glDrawArrays(GL_TRIANGLES, 0, 6); // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, &indices);
    glBindVertexArray(0);                 // unbind vertex array
	glBindTexture(GL_TEXTURE_2D, 0);      // unbind texture (base)
	glActiveTexture(GL_TEXTURE0 + 0);
	// DO NOT Destroy vertex array and buffer objects (until the engine closes)
    // Disable program
	glUseProgram(0);
}
////////////
////////////
////////////
void Renderer::draw_sprite(const Texture& texture, double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
    const std::vector<float>& vertex_array, const Shader& shader, const std::vector<Texture *>& map) // Why is vertex_array in params if not in use???
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
    context_check();
	// vertex array obj -
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    // vertex buffer obj
	glBindVertexArray(vertex_array_obj); // bind	
	    GLuint vertex_buffer_obj;
	    glGenBuffers(1, &vertex_buffer_obj);
		GLfloat vertex[] ={
			static_cast<float>(x)                                            , static_cast<float>(y),  
		    static_cast<float>(x) + static_cast<GLfloat>(texture.get_width()), static_cast<float>(y),  
	 	    static_cast<float>(x) + static_cast<GLfloat>(texture.get_width()), static_cast<float>(y) + static_cast<GLfloat>(texture.get_height()),  
		    static_cast<float>(x)                                            , static_cast<float>(y) + static_cast<GLfloat>(texture.get_height()),
		};
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); // vertex attribute ptr
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));	
	glBindVertexArray(0); // unbind
	// texcoord buffer obj
	glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint texcoord_buffer_obj;
        glGenBuffers(1, &texcoord_buffer_obj);
		GLfloat tex_coord[] = {0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0,};
        glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord) * sizeof(GLfloat), tex_coord, GL_STATIC_DRAW); 
		glEnableVertexAttribArray(1); // vertex attribute ptr
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));			
	glBindVertexArray(0); // unbind	
	// element buffer 
	glBindVertexArray(vertex_array_obj); // bind
	    GLuint element_buffer_obj;
		glGenBuffers(1, &element_buffer_obj);
		GLuint element[] = {0, 1, 3,  1, 2, 3};//{0,1,2, 2,3,0};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element) * sizeof(GLuint), element, GL_STATIC_DRAW);
	glBindVertexArray(0); // unbind// color buffer tex - texture that will be attached to framebuffer; will store color
	//-----------------------
	// Framebuffer-related 
	/*const unsigned int SHADOW_WIDTH = 512, SHADOW_HEIGHT = 512;
    unsigned int shadow;
    glGenTextures(1, &shadow);
        glBindTexture(GL_TEXTURE_2D, shadow);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
             SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  	
    glBindTexture(GL_TEXTURE_2D, 0);
    // color_buffer_texture	
	GLuint color_buffer_texture;
    glGenTextures(1, &color_buffer_texture);
    glBindTexture(GL_TEXTURE_2D, color_buffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Renderer::get_display_width(), Renderer::get_display_height(), 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// render buffer obj - stores depth and stencil
	GLuint render_buffer_obj;
    glGenRenderbuffers(1, &render_buffer_obj);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_obj); // bind
	    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Renderer::get_display_width(), Renderer::get_display_height());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);                 // unbind
	// frame buffer obj - for post-processing effects
	GLuint frame_buffer_obj;
    glGenFramebuffers(1, &frame_buffer_obj);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_obj); //  bind
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer_texture , 0);       // attach color buffer (texture)
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_obj); // attach render buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) // must be attached to a colorbuffer(texture) and a renderbuffer to be complete
		    Logger("Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	             // unbind
	*/
	//////////////////////////
	// Enable or disable state
	glDisable(GL_DEPTH_TEST); // Disable 3d
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);	
	//glEnable(GL_DEBUG_OUTPUT);
	// Shader
	glUseProgram(shader.get_program()); // push program
	// Transform
#ifndef use_glm
    // model (object)
	Matrix4 model = Matrix4(0.0f);
	model = Matrix4::translate(model, x, y, -1.0);
	model = Matrix4::rotate(model, angle * 0.0174533, 0, 0, 1);
	model = Matrix4::scale(model, scale_x, scale_y, 1.0);
	model = Matrix4::translate(model, -x, -y, 1.0);
    // view + proj (camera)
    Matrix4 proj  = Matrix4::ortho(camera->get_left(), window_width, window_height,
	    camera->get_top(), camera->get_near(), camera->get_far()); // left, right, bottom, top, near, far
    Vector3 eye    = Vector3(camera->get_position());
    Vector3 center = Vector3(camera->get_position() + camera->get_view());
    Vector3 up     = Vector3(camera->get_up());
    Matrix4 view   = Matrix4::look_at(eye, center, up);
	
	if(shader.get_uniform("model") != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, false, Matrix4::data(model));//&model.value[0][0]);
	if(shader.get_uniform("proj" ) != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj" ), 1, false, Matrix4::data(proj ));//&proj.value[0][0] );
	if(shader.get_uniform("view" ) != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view" ), 1, false, Matrix4::data(view ));//&view.value[0][0] );
#endif

#ifdef use_glm
	// model (object)
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(glm::vec2(x, y), -1.0f));  // z must be negative(e.g -1) if using the 'view' matrix
	model = glm::rotate(model, static_cast<GLfloat>(angle * 0.0174533), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(glm::vec2(static_cast<GLfloat>(scale_x), static_cast<GLfloat>(scale_y)), 1.0f));	
	model = glm::translate(model, glm::vec3(glm::vec2(-x, -y), 1.0f));
    // view (camera)
    glm::vec3 eye    = glm::vec3(camera->get_position().x, camera->get_position().y, camera->get_position().z);
    glm::vec3 center = glm::vec3(camera->get_position().x + camera->get_view().x, camera->get_position().y + camera->get_view().y, camera->get_position().z + camera->get_view().z);
	glm::vec3 up     = glm::vec3(camera->get_up().x  , camera->get_up().y  , camera->get_up().z);	
	glm::mat4 view   = glm::lookAt(eye, center, up); // glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0));
    // projection (zoom)
    float zoom_factor = camera->get_zoom(); // for 2d ONLY (no matter how much you zoom in and out, viewport:(w_width, w_height) should stay the same, ortho:(right, bottom) on the other hand should change)
	glm::mat4 proj = glm::ortho(static_cast<GLfloat>(camera->get_left()), static_cast<GLfloat>(window_width) + zoom_factor, static_cast<GLfloat>(window_height) + zoom_factor,
 	    static_cast<GLfloat>(camera->get_top()), static_cast<GLfloat>(camera->get_near()), static_cast<GLfloat>(camera->get_far()));//glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);//glm::ortho(0.0f, static_cast<float>(window_width), 0.0f, static_cast<float>(window_height), -1.0f, 1.0f); /// bottom-left(0, width, 0, height), top-left(0, width, height, 0)=default for dokun	

	if(shader.get_uniform("model") != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, false, glm::value_ptr(model));
	if(shader.get_uniform("proj" ) != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj" ), 1, false, glm::value_ptr(proj) );
	if(shader.get_uniform("view" ) != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view" ), 1, false, glm::value_ptr(view) );		
#endif
	// ..............
	//glUniformMatrix4fv(shader.get_uniform("model"), 1, GL_TRUE, (const GLfloat *)&model0.matrix[0]);
	//Shader shader0 = const_cast<Shader&>(shader);
	//const_cast<Shader&>(shader).set_matrix4("model", 1, true, model0); // If transpose is true then row-major
	//const_cast<Shader&>(shader).set_matrix4("view" , 16, view0); // If transpose is true then row-major
	//glUniformMatrix4fv(shader.get_uniform("view" ), 4, true, (const float *)view0.matrix);
	//const_cast<Shader&>(shader).set_matrix4("proj" , 16, (float*)proj0.matrix); // If transpose is true then row-major
	// Light (global)
    if(shader.get_uniform("light.ambient" ) != -1) glUniform4f(shader.get_uniform("light.ambient" ), light->get_ambient ().x, light->get_ambient ().y, light->get_ambient ().z, light->get_ambient ().w);
    if(shader.get_uniform("light.diffuse" ) != -1) glUniform4f(shader.get_uniform("light.diffuse" ), light->get_diffuse ().x, light->get_diffuse ().y, light->get_diffuse ().z, light->get_diffuse ().w); // Let's darken the light a bit to fit the scene
    if(shader.get_uniform("light.specular") != -1) glUniform4f(shader.get_uniform("light.specular"), light->get_specular().x, light->get_specular().y, light->get_specular().z, light->get_specular().w);  
	if(shader.get_uniform("light.position") != -1) glUniform4f(shader.get_uniform("light.position"), light->get_position().x, light->get_position().y, light->get_position().z, light->get_position().w);
	if(shader.get_uniform("light.spot_direction") != -1) glUniform3f(shader.get_uniform("light.spot_direction"), light->get_spot_direction().x, light->get_spot_direction().y, light->get_spot_direction().z);
	if(shader.get_uniform("light.spot_exponent" ) != -1) glUniform1f(shader.get_uniform("light.spot_exponent" ), light->get_spot_exponent ());
	if(shader.get_uniform("light.spot_cutoff"   ) != -1) glUniform1f(shader.get_uniform("light.spot_cutoff"   ), light->get_spot_cutoff   ());
	if(shader.get_uniform("light.attenuation"   ) != -1) glUniform3f(shader.get_uniform("light.attenuation"   ), light->get_attenuation   ().x, light->get_attenuation   ().y, light->get_attenuation   ().z);	 								  
	if(shader.get_uniform("light.is_enabled"    ) != -1) glUniform1i(shader.get_uniform("light.is_enabled"    ), light->is_enabled());	 								  
	// Camera (global)
	if(shader.get_uniform("camera.position") != -1) glUniform3f(shader.get_uniform("camera.position"), camera->get_position().x, camera->get_position().y, camera->get_position().z);
	// etc.
	if(shader.get_uniform("color") != -1) glUniform4f(glGetUniformLocation(shader.get_program(), "color"), (red / 255), (green / 255), (blue / 255), (alpha / 255));
	if(shader.get_uniform("map.is_texture") != -1) glUniform1i(shader.get_uniform("map.is_texture"), (texture.get_data() != nullptr)); 
	if(shader.get_uniform("resolution") != -1) glUniform2f(shader.get_uniform("resolution"), Renderer::get_display_width(), Renderer::get_display_height());	
	// Framebuffer
	// phase0
	/*
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_obj); // bind framebuffer		
	    // do something here ..
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch to default framebuffer
	*/
///////////////////////////////////////*/
	// Texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture.get_buffer()); // bind texture (base)
	const_cast<Shader&>(shader).set_integer("map.diffuse", 0);
	// other textures ...
	/*for(int i = 0; i < map.size(); i++)
	{
	    //std::cout << "Texture map: " << i + 1 << " (" << "map." + map[i]->get_type() << ") added\n";
	    map[i]->generate(); if(map[i]->get_buffer() == 0) Logger("Texture map not generated!");
	    glActiveTexture(GL_TEXTURE0 + i + 1);
	    glBindTexture(GL_TEXTURE_2D, map[i]->get_buffer()); // bind texture (base)
	    const_cast<Shader&>(shader).set_integer("map." + map[i]->get_type(), GL_TEXTURE0 + i + 1);	
	}*/
	// Draw
	glBindVertexArray(vertex_array_obj);                // bind vertex array
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//glDrawArrays(GL_TRIANGLES, 0, 6); // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, &indices);
    glBindVertexArray(0);                 // unbind vertex array
	glBindTexture(GL_TEXTURE_2D, 0);      // unbind texture (base)
	glActiveTexture(GL_TEXTURE0 + 0);
	// Clean textures (to prevent memory leak)
	//glDeleteTextures(1, &shadow);
	//glDeleteTextures(1, &frame_buffer_texture);
	//glDeleteTextures(1, &color_buffer_texture);
	// Clean buffers (to prevent memory leak)
	glDeleteBuffers(1, &vertex_buffer_obj  );
	glDeleteBuffers(1, &texcoord_buffer_obj);	
	glDeleteBuffers(1, &element_buffer_obj );
	//glDeleteRenderbuffers(1, &render_buffer_obj); // Clean render_buffers
	//glDeleteFramebuffers (1, &frame_buffer_obj ); // Clean frame_buffers
	// Clean arrays (to prevent memory leak)
	glDeleteVertexArrays(1, &vertex_array_obj);
    // Disable program
	glUseProgram(0); // pop program
	//DOKUN_OPENGL_ERROR_CHECK(DOKUN_FILE, DOKUN_LINE);
#endif
}
////////////
////////////
void Renderer::draw_atlas(const Texture& texture, int width, int height, double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, int frame, const Shader& shader)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
	// vertex array obj -
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    // vertex buffer data
	glBindVertexArray(vertex_array_obj); // bind	
	    GLuint vertex_buffer;
	    glGenBuffers(1, &vertex_buffer);
		GLfloat vertex[] =
	    {
		    static_cast<GLfloat>(0)                                  , static_cast<GLfloat>(0),
            static_cast<GLfloat>(0) + static_cast<GLfloat>(width)  , static_cast<GLfloat>(0),
            static_cast<GLfloat>(0) + static_cast<GLfloat>(width)  , static_cast<GLfloat>(0) + static_cast<GLfloat>(-height),
            static_cast<GLfloat>(0)                                  , static_cast<GLfloat>(0) + static_cast<GLfloat>(-height), 	
	    };
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); // vertex attribute ptr
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));	
	glBindVertexArray(0); // unbind
	// tex_coord buffer data	
	glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer;
        glGenBuffers(1, &tex_coord_buffer);
		const GLfloat tw = static_cast<GLfloat>(width ) / texture.get_width();
	    const GLfloat th = static_cast<GLfloat>(height) / texture.get_height();
        const int num_per_row = texture.get_width()  / width ; // number of images per row
		const int num_per_col = texture.get_height() / height; // number of images per columns
		const int frame_total = num_per_row * num_per_col;
        const GLfloat tx = (frame % num_per_row    ) * tw;	
        const GLfloat ty = (frame / num_per_row + 1) * th;	
	    GLfloat tex_coord[] =
	    {
		    tx     , ty, 
            tx + tw, ty, 
            tx + tw, ty - th, 
            tx     , ty - th,
	    };
        glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord) * sizeof(GLfloat), tex_coord, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1); // vertex attribute ptr
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));			
	glBindVertexArray(0); // unbind
	// element buffer 
	glBindVertexArray(vertex_array_obj); // bind
	    GLuint element_buffer_obj;
		glGenBuffers(1, &element_buffer_obj);
		GLuint element[] = {0, 1, 3,  1, 2, 3};//{0,1,2, 2,3,0};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element), element, GL_STATIC_DRAW);
	glBindVertexArray(0); // unbind	
	// Draw
    // Enable or disable states
	glDisable(GL_DEPTH_TEST); // Disable 3d
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);		
	// Shader			
	glUseProgram(shader.get_program());
	// Transform
#ifdef use_glm
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(glm::vec2(static_cast<GLfloat>(x), static_cast<GLfloat>(y + height)), -1.0f)); 
	model = glm::rotate(model, static_cast<GLfloat>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(glm::vec2(scale_x, scale_y), 1.0f));
	//model = glm::translate(model, glm::vec3(glm::vec2(-x, -y), 1.0f)); //model = glm::translate(model, glm::vec3(glm::vec2(static_cast<GLfloat>(-x), static_cast<GLfloat>(-y)), 1.0f)); 
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Renderer::get_display_width()), static_cast<float>(Renderer::get_display_height()), 0.0f, -1.0f, 1.0f); // glm::ortho (0.0f, static_cast<float>(Renderer::get_display_width()) / (static_cast<float>(Renderer::get_display_width()) / 2), 0.0f, static_cast<float>(Renderer::get_display_height()) / (static_cast<float>(Renderer::get_display_height()) / 2), -1.0f, 1.0f);  // bottom-left(0, width, 0, height), top-left(0, width, height, 0)=default	

	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, false, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj"),  1, false, glm::value_ptr(proj) );
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view"),  1, false, glm::value_ptr(view) );
#endif	
	// Light (global)
    if(shader.get_uniform("light.ambient" ) != -1) glUniform4f(shader.get_uniform("light.ambient" ), light->get_ambient ().x, light->get_ambient ().y, light->get_ambient ().z, light->get_ambient ().w);
    if(shader.get_uniform("light.diffuse" ) != -1) glUniform4f(shader.get_uniform("light.diffuse" ), light->get_diffuse ().x, light->get_diffuse ().y, light->get_diffuse ().z, light->get_diffuse ().w); // Let's darken the light a bit to fit the scene
    if(shader.get_uniform("light.specular") != -1) glUniform4f(shader.get_uniform("light.specular"), light->get_specular().x, light->get_specular().y, light->get_specular().z, light->get_specular().w);  
	if(shader.get_uniform("light.position") != -1) glUniform4f(shader.get_uniform("light.position"), light->get_position().x, light->get_position().y, light->get_position().z, light->get_position().w);
	if(shader.get_uniform("light.spot_direction") != -1) glUniform3f(shader.get_uniform("light.spot_direction"), light->get_spot_direction().x, light->get_spot_direction().y, light->get_spot_direction().z);
	if(shader.get_uniform("light.spot_exponent" ) != -1) glUniform1f(shader.get_uniform("light.spot_exponent" ), light->get_spot_exponent ());
	if(shader.get_uniform("light.spot_cutoff"   ) != -1) glUniform1f(shader.get_uniform("light.spot_cutoff"   ), light->get_spot_cutoff   ());
	if(shader.get_uniform("light.attenuation"   ) != -1) glUniform3f(shader.get_uniform("light.attenuation"   ), light->get_attenuation   ().x, light->get_attenuation   ().y, light->get_attenuation   ().z);	
	// Camera (global)
	if(shader.get_uniform("camera.position") != -1) glUniform3f(shader.get_uniform("camera.position"), camera->get_position().x, camera->get_position().y, camera->get_position().z);
	// etc.
	if(shader.get_uniform("color") != -1) glUniform4f(glGetUniformLocation(shader.get_program(), "color"), (red / 255), (green / 255), (blue / 255), (alpha / 255));
	if(shader.get_uniform("map.is_texture") != -1) glUniform1i(shader.get_uniform("map.is_texture"), (texture.get_data() != nullptr)); 
	if(shader.get_uniform("resolution") != -1) glUniform2f(shader.get_uniform("resolution"), Renderer::get_display_width(), Renderer::get_display_height());	
	// Texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture.get_buffer());  // bind texture (base)
	// Draw
	glBindVertexArray(vertex_array_obj);  // bind vertex array
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);// glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, &indices);
    glBindVertexArray(0);                 // unbind vertex array
	glBindTexture(GL_TEXTURE_2D, 0);      // unbind texture (base)
    // Clean buffers 
	glDeleteBuffers(1, &vertex_buffer   );
	glDeleteBuffers(1, &tex_coord_buffer);	
	glDeleteBuffers(1, &element_buffer_obj);	
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
    // Disable program
	glUseProgram(0);
#endif
}
////////////
////////////
////////////
////////////
void Renderer::draw_model(const std::vector<Vector3>& vertex_array, const std::vector<unsigned int>& element_array, const std::vector<Vector2>& uv_array, const std::vector<Vector3>& normal_array, double x, double y, double z, double rx, double ry, double rz, double scale_x, double scale_y, double sz, double ox, double oy, double oz, const std::vector<Texture *>& texture_array, const Vector4& ambient, const Vector4& diffuse, const Vector4& specular,
		const Vector4& emission, double shininess, const Shader& shader)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();		
    if(vertex_array.empty())
	{
		Logger::push("Rendering failed : Vertex array is empty");
		return;
	}
	if(element_array.empty())
	{
		Logger::push("Rendering failed : Element array is empty");
		return;		
	}
    // vertex array obj
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// vertex buffer obj - for drawing
	glBindVertexArray(vertex_array_obj); // bind vertex array obj to vertex attrib ptr
	    GLuint vertex_buffer_obj;
        glGenBuffers(1, & vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, vertex_array.size() * sizeof(Vector3), &vertex_array[0], GL_STATIC_DRAW); // GL_DYNAMIC_DRAW
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vector3), static_cast<GLvoid*>(0));	
	glBindVertexArray(0);               // unbind
	// tex_coord buffer obj - for texture mapping
	GLuint uv_buffer_obj;
	if(!uv_array.empty())
	{
	glBindVertexArray(vertex_array_obj); // bind 	
	    glGenBuffers(1, & uv_buffer_obj);
	    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_obj);
	    glBufferData(GL_ARRAY_BUFFER, uv_array.size() * sizeof(Vector2), &uv_array[0], GL_STATIC_DRAW); 			
	    glEnableVertexAttribArray(1);  
	    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, sizeof(Vector2), static_cast<GLvoid*>(0));		
	glBindVertexArray(0); // unbind 
	}
	// normal buffer - for lighting
	GLuint normal_buffer_obj;
    if(!normal_array.empty())
	{		
    glBindVertexArray(vertex_array_obj); // bind 
	    glGenBuffers(1, & normal_buffer_obj);
	    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_obj);
	    glBufferData(GL_ARRAY_BUFFER, normal_array.size() * sizeof(Vector3), &normal_array[0], GL_STATIC_DRAW); 			
	    glEnableVertexAttribArray(2);  
	    glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, sizeof(Vector3), static_cast<GLvoid*>(0)); 
	glBindVertexArray(0);               // unbind 
	}
	// element buffer - specifies order in which vertices are to be drawn
	glBindVertexArray(vertex_array_obj); // bind
	    GLuint index_buffer_obj;
	    glGenBuffers(1, & index_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_obj);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_array.size() * sizeof(unsigned int), &element_array[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	// Framebuffer
	// frame_buffer texture
	/*GLuint frame_buffer_texture;
    glGenTextures(1, &frame_buffer_texture);
    glBindTexture(GL_TEXTURE_2D, frame_buffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Renderer::get_display_width(), Renderer::get_display_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	std::cout << dokun_opengl_error() << std::endl;*/
    // color_buffer_texture	
	/*
	GLuint color_buffer_texture;
    glGenTextures(1, &color_buffer_texture);
    glBindTexture(GL_TEXTURE_2D, color_buffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Renderer::get_display_width(), Renderer::get_display_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	std::cout << dokun_opengl_error() << std::endl;
	// render buffer obj - stores depth and stencil
	GLuint render_buffer_obj;
    glGenRenderbuffers(1, &render_buffer_obj);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_obj); // bind
	    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Renderer::get_display_width(), Renderer::get_display_height());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);                 // unbind
	std::cout << dokun_opengl_error() << std::endl;
	// frame buffer obj - for post-processing effects
	GLuint frame_buffer_obj;
    glGenFramebuffers(1, &frame_buffer_obj);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_obj); //  bind
	    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_buffer_texture, 0);       // attach frambuffer texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer_texture, 0);       // attach color buffer (texture)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_obj); // attach render buffer
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) // must be attached to a colorbuffer(texture) and a renderbuffer to be complete
		    Logger("Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	             // unbind	
	std::cout << dokun_opengl_error() << std::endl;
	*/
	////////////////////////	
	// Enable and disable states
    glEnable(GL_DEPTH_TEST);  // enable depth
	glDisable(GL_BLEND);
	glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_CULL_FACE );
	//glDepthFunc(GL_LESS); // default: GL_LESS other: GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, and GL_ALWAYS
	//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
	//glFrontFace(GL_CCW);   // default: GL_CCW  other: GL_CW
	//glCullFace(GL_BACK);   // default: GL_BACK other: GL_FRONT, GL_FRONT_AND_BACK
	// Set mode		
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_POINT, GL_LINE, GL_FILL, 		
#ifdef use_glm   
    // model (object)
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(static_cast<float>(x), 
	    static_cast<float>(y), static_cast<float>(z)));
	model = glm::rotate(model, static_cast<float>(rx * 0.0174533), glm::vec3(1, 0, 0));
	model = glm::rotate(model, static_cast<float>(ry * 0.0174533), glm::vec3(0, 1, 0));
	model = glm::rotate(model, static_cast<float>(rz * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(static_cast<float>(scale_x), 
	    static_cast<float>(scale_y), static_cast<float>(sz)));
    // view (camera)
    glm::vec3 eye    = glm::vec3(camera->get_position().x, camera->get_position().y, camera->get_position().z);
    glm::vec3 center = glm::vec3(camera->get_position().x + camera->get_view().x, camera->get_position().y + camera->get_view().y, camera->get_position().z + camera->get_view().z);
	glm::vec3 up     = glm::vec3(camera->get_up().x  , camera->get_up().y  , camera->get_up().z);	
	glm::mat4 view   = glm::lookAt(eye, center, up);
	// proj (zoom)
	glm::mat4 proj = glm::perspective(67.5f, static_cast<float>(Renderer::get_display_width() / Renderer::get_display_height()), 1.0f, 10024.0f); // average fov = 67.5    45 + 90 = 135 / 2 = 67.5 |  znear=1  zfar=1024
	// Shader (push)    glUniform (glGetUniformLocation(shader.get_program(), "" ), );
	glUseProgram(shader.get_program ());
	if(shader.get_uniform("model") != -1) glUniformMatrix4fv(shader.get_uniform("model"), 1, false, glm::value_ptr(model)); // object
    if(shader.get_uniform("view" ) != -1) glUniformMatrix4fv(shader.get_uniform("view" ), 1, false, glm::value_ptr(view) );   // camera
	if(shader.get_uniform("proj" ) != -1) glUniformMatrix4fv(shader.get_uniform("proj" ), 1, false, glm::value_ptr(proj) );   // zoom	
#endif
    // camera
	if(shader.get_uniform("view_position" ) != -1) const_cast<Shader&>(shader).set_float("view_position", camera->get_position().x, camera->get_position().y, camera->get_position().z);		
	// light
    if(shader.get_uniform("light.ambient" ) != -1) glUniform3f(shader.get_uniform("light.ambient" ), light->get_ambient ().x, light->get_ambient ().y, light->get_ambient ().z); //, light->get_().w
    if(shader.get_uniform("light.diffuse" ) != -1) glUniform3f(shader.get_uniform("light.diffuse" ), light->get_diffuse ().x, light->get_diffuse ().y, light->get_diffuse ().z); //, light->get_().w // Let's darken the light a bit to fit the scene
    if(shader.get_uniform("light.specular") != -1) glUniform3f(shader.get_uniform("light.specular"), light->get_specular().x, light->get_specular().y, light->get_specular().z); //, light->get_().w  
	if(shader.get_uniform("light.position") != -1) glUniform3f(shader.get_uniform("light.position"), light->get_position().x, light->get_position().y, light->get_position().z); //, light->get_().w
	if(shader.get_uniform("light.spot_direction") != -1) glUniform3f(shader.get_uniform("light.spot_direction"), light->get_spot_direction().x, light->get_spot_direction().y, light->get_spot_direction().z);
	if(shader.get_uniform("light.spot_exponent" ) != -1) glUniform1f(shader.get_uniform("light.spot_exponent" ), light->get_spot_exponent());
	if(shader.get_uniform("light.spot_cutoff"   ) != -1) glUniform1f(shader.get_uniform("light.spot_cutoff"   ), light->get_spot_cutoff  ());
	if(shader.get_uniform("light.attenuation"   ) != -1) glUniform3f(shader.get_uniform("light.attenuation"   ), light->get_attenuation  ().x , light->get_attenuation   ().y, light->get_attenuation   ().z);
	// material     if(shader.get_uniform("") != -1) glUniform3f(glGetUniformLocation(shader.get_program(), ""), );
    if(shader.get_uniform("material.ambient"  ) != -1) glUniform3f(shader.get_uniform("material.ambient"  ), ambient.x , ambient.y , ambient.z );
    if(shader.get_uniform("material.diffuse"  ) != -1) glUniform3f(shader.get_uniform("material.diffuse"  ), diffuse.x , diffuse.y , diffuse.z );
    if(shader.get_uniform("material.specular" ) != -1) glUniform3f(shader.get_uniform("material.specular" ), specular.x, specular.y, specular.z);
	if(shader.get_uniform("material.emission" ) != -1) glUniform3f(shader.get_uniform("material.emission" ), emission.x, emission.y, emission.z);
	if(shader.get_uniform("material.shininess") != -1) glUniform1f(shader.get_uniform("material.shininess"), shininess                         );	
	// texture
	if(shader.get_uniform("material.texture.is_texture") != -1) glUniform1i(shader.get_uniform("material.texture.is_texture"), (texture_array.size() > 0)); 
	if(shader.get_uniform("material.texture.count"     ) != -1) glUniform1i(shader.get_uniform("material.texture.count"     ), texture_array.size()      ); 
	//////////////////////////////////////
	// texture data
	for(int i = 0; i < texture_array.size(); i++) // for each texture
	{
	//-----------------------
	std::string texture_name = "material.texture." + texture_array[i]->get_type(); //+ "[" + String(i).str() + "]"; // multiple textures	ex. material.texture.diffuse	
	if(shader.get_uniform(texture_name) != -1) glUniform1i(glGetUniformLocation(shader.get_program(), texture_name.c_str()), i); // bind texture to unit	
	//-----------------------
	// start binding texture
	glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding	
	glBindTexture(GL_TEXTURE_2D, texture_array[i]->get_buffer());   // bind texture
	//glPixelStorei(GL_PACK_ALIGNMENT  , 4); // default 4; optional 1, 2, 4, or 8
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // default 4; optional 1, 2, 4, or 8
	#ifdef DOKUN_DEBUG0
	std::cout << "Texture: " << texture_name << "     | unit : " << i << "\n";
	#endif
	}	
	// Draw the model !
	glBindVertexArray(vertex_array_obj); // bind vertex_array_obj
		glDrawElements(GL_TRIANGLES, element_array.size(), GL_UNSIGNED_INT, 0);//glDrawElements(GL_TRIANGLES, element_array.size(), GL_UNSIGNED_INT, &element_array[0]);	 // elements - order in which vertices are to be drawn
	glBindVertexArray(0);                // unbind vertex_array_obj
	glBindTexture(GL_TEXTURE_2D, 0);     // unbind texture
    glActiveTexture(GL_TEXTURE0); // restore default texture
	////////////////////////
	// Clean textures
	//glDeleteTextures(1, &frame_buffer_texture);
	//glDeleteTextures(1, &color_buffer_texture);
	// Clean buffers
	glDeleteBuffers(1, & vertex_buffer_obj);
	glDeleteBuffers(1, & uv_buffer_obj    );	
	glDeleteBuffers(1, & normal_buffer_obj);
	glDeleteBuffers(1, & index_buffer_obj );
	//glDeleteRenderbuffers(1, &render_buffer_obj); // Clean render_buffers
	//glDeleteFramebuffers (1, &frame_buffer_obj ); // Clean frame_buffers		
	// Clean arrays
	glDeleteVertexArrays(1, & vertex_array_obj);
	// disable and enable here
	//glDisable(GL_CULL_FACE );
	//glDisable(GL_DEPTH_TEST); // REQUIRED!
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Shader (pop)
	glUseProgram(0);
#endif
}
////////////
void Renderer::draw_image(const unsigned int buffer, int width, int height, int depth, double x, double y, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, int channel) // good !
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"uniform mat4 view ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = vec2(tex_coord.x, 1.0 - tex_coord.y);\n"
            "gl_Position = proj * view * model * vec4(position, 0.0, 1.0);\n" // 2:image uses camera (via "view")
        "}\n"
	};
	// fragment shader
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"uniform bool has_texture;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "if(has_texture == false)\n"
		    "{\n"
                "out_color = color;\n"
            "}"	
		    "if(has_texture == true)\n"
		    "{\n"
                "out_color = color * texture(base, Texcoord);\n"
            "}"
		"}\n"
	};
	// texture
	// vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // State
	glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);	
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform	
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x, y, -1));//model = glm::translate(model, glm::vec3(x + width/2, y + height/2, -1));//
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x, -y, 1));//model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 1));
    // projection and view (camera)
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glm::mat4 view  = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0)); // uses its own camera (not subject to the Renderer's camera)
	// pass matrix data to the shader	
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view") , 1, GL_FALSE, glm::value_ptr(view) );
#endif	
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));	
	shader.set_integer("has_texture", (buffer != 0));	
	// Draw
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, buffer);  // bind texture
	shader.set_integer("base", 0);
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	// Restore defaults
	// Destroy shader
	shader.disable();
	shader.destroy();
#endif
}
////////////
////////////
////////////
void Renderer::draw_camera(double eye_x, double eye_y, double eye_z, double center_x, double center_y,double center_z, double up_x, double up_y,double up_z, const Shader& shader)
{
#ifdef DOKUN_OPENGL
    context_check();
    center_x = eye_x + center_x;
	center_y = eye_y + center_y;
	center_z = eye_z + center_z;	
#ifdef use_glm
	glm::mat4 view = glm::lookAt(glm::vec3(eye_x, eye_y, eye_z), glm::vec3(center_x, center_y, center_z), glm::vec3(up_x, up_y, up_z));		
#endif
#ifndef use_glm
    Matrix4 view;//;
#endif
    glUseProgram(shader.get_program());
	    //glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view"),  1, false, glm::value_ptr(view));
	glUseProgram(0);
#endif
}
//////////////////////////////
void Renderer::draw_light(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, 
	const Vector4& position, const Vector3& spot_direction, 
	double spot_exponent, double spot_cutoff, const Vector3& attenuation, const Texture * texture/*const Shader& shader*/)
{
#ifdef DOKUN_OPENGL
    context_check();
    const char * vertex_source[] =
	{
        "#version 330\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec2 texcoord;\n"
        "out vec2 Texcoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 proj;\n"
        "\n"
        "out vec3 frag_position;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "\n"
		    "Texcoord    = vec2(texcoord.x, 1.0 - texcoord.y);\n"
			"gl_Position = proj * view * model * vec4(position, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
        "#version 330\n"
        "out vec4 out_color;\n"
        "in vec2 Texcoord;\n"
		"uniform vec2 resolution;\n"
		"in vec3 frag_position;\n"
	    "uniform vec3 view_position;\n"	
        "struct Light {\n"
            "vec4 position;\n"
            "vec4 ambient; \n"
            "vec4 diffuse; \n"
            "vec4 specular;\n"
            "vec3 spot_direction;\n"
            "float spot_exponent;\n"
            "float spot_cutoff;\n"
			"float constant;\n"
			"float linear;\n"
			"float quadratic;\n"
            "vec3 attenuation;\n"
            "float outer_cutoff;\n"
			"sampler2D texture;\n"
        "};\n"
		"uniform Light light;\n"
		"\n"
        "void main()\n"
        "{\n"
		/*
		    "vec3 direction;\n"
			// check if this is a directional light
		    "if(light.position.w == 0.0) { // directional light\n"
		        "direction = normalize(-light.position.xyz);\n"
		    "}\n"
			// point light (sun, lamp)
		    "float distance = length(light.position.xyz - frag_position);\n"
		    "float attenuation = 1.0 / (light.constant + light.linear * distance +\n" 
    		    "light.quadratic * (distance * distance));  \n"
		    "\n"
		    // spotlight
			"float theta = dot(lightDir, normalize(-light.direction));\n"
			"if(theta > light.spot_cutoff){\n"
			"}\n"
			// flashlight
			//"float epsilon   = light.spot_cutoff - light.outer_cutoff;\n"
			"\n"
			*/
			"\n"
			"vec2 light_coord = (gl_FragCoord.xy / resolution.xy);\n"
			"vec4 light_map   = texture2D(light.texture, light_coord);"
			"vec4 ambient, diffuse, specular, emission;\n"
			"ambient  = light.ambient; \n"
			"diffuse  = light.diffuse; \n"
			"specular = light.specular;\n"
			"vec4 light_color = ambient + diffuse;//+ specular;\n"
		    "out_color = light_color * light_map;\n"
        "}\n"
	};	
	// vertex array obj  - stores vertices
    GLuint vertex_array_obj;
	if(texture != nullptr)
	glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
	GLuint tex_coord_buffer_obj;
	if(texture != nullptr)
	{
    glBindVertexArray(vertex_array_obj); // bind again  
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	}
	// vertex buffer obj
	GLuint vertex_buffer_obj;
	if(texture != nullptr)
	{
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(position.x)                                               , static_cast<GLfloat>(position.y),
            static_cast<GLfloat>(position.x) + static_cast<GLfloat>(texture->get_width()), static_cast<GLfloat>(position.y),
            static_cast<GLfloat>(position.x) + static_cast<GLfloat>(texture->get_width()), static_cast<GLfloat>(position.y) + static_cast<GLfloat>(texture->get_height()),
            static_cast<GLfloat>(position.x)                                               , static_cast<GLfloat>(position.y) + static_cast<GLfloat>(texture->get_height()),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	}
	// element buffer obj
	GLuint element_buffer_obj;
	if(texture != nullptr)
	{	
	glBindVertexArray(vertex_array_obj); // (vao start 1)
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	}
	// State
	glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	//glEnable(GL_BLEND);	
    // Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
    // Program
	glUseProgram(shader.get_program()); // use program
    // Transform
#ifdef use_glm   
    // model (object)
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(position.x, position.y, -1));
	model = glm::rotate(model, static_cast<float>(0.0), glm::vec3(0, 0, 1)); //model = glm::rotate(model, static_cast<float>(rx), glm::vec3(1, 0, 0)); //model = glm::rotate(model, static_cast<float>(ry), glm::vec3(0, 1, 0)); //model = glm::rotate(model, static_cast<float>(rz), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3((32.0f / texture->get_width()), (32.0f / texture->get_height()), 1));
    // view (camera)
    glm::vec3 eye    = glm::vec3(camera->get_position().x, camera->get_position().y, camera->get_position().z);
    glm::vec3 center = glm::vec3(camera->get_position().x + camera->get_view().x, camera->get_position().y + camera->get_view().y, camera->get_position().z + camera->get_view().z);
	glm::vec3 up     = glm::vec3(camera->get_up().x  , camera->get_up().y  , camera->get_up().z);	
	glm::mat4 view   = glm::lookAt(eye, center, up);
	// proj (zoom)
	//float window_width  = Renderer::get_display_width ();
	//float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
    
	if(shader.get_uniform("model") != -1) glUniformMatrix4fv(shader.get_uniform("model"), 1, false, glm::value_ptr(model)); // object
    if(shader.get_uniform("view" ) != -1) glUniformMatrix4fv(shader.get_uniform("view" ), 1, false, glm::value_ptr(view) );   // camera
	if(shader.get_uniform("proj" ) != -1) glUniformMatrix4fv(shader.get_uniform("proj" ), 1, false, glm::value_ptr(proj) );   // zoom		
#endif    
    // Light
	if(shader.get_uniform("light.ambient" ) != -1) glUniform4f(shader.get_uniform("light.ambient" ), ambient.x , ambient.y , ambient.z , ambient.w );
    if(shader.get_uniform("light.diffuse" ) != -1) glUniform4f(shader.get_uniform("light.diffuse" ), diffuse.x , diffuse.y , diffuse.z , diffuse.w ); // Let's darken the light a bit to fit the scene
    if(shader.get_uniform("light.specular") != -1) glUniform4f(shader.get_uniform("light.specular"), specular.x, specular.y, specular.z, specular.w);  
	if(shader.get_uniform("light.position") != -1) glUniform4f(shader.get_uniform("light.position"), position.x, position.y, position.z, position.w);  // glUniform3f(shader.get_uniform("light.position"), Mouse::get_position().x, Mouse::get_position().y, position.z)
	if(shader.get_uniform("light.spot_direction") != -1) glUniform3f(shader.get_uniform("light.spot_direction"), spot_direction.x, spot_direction.y, spot_direction.z);
	if(shader.get_uniform("light.spot_exponent" ) != -1) glUniform1f(shader.get_uniform("light.spot_exponent" ), spot_exponent);
	if(shader.get_uniform("light.spot_cutoff"   ) != -1) glUniform1f(shader.get_uniform("light.spot_cutoff"   ), spot_cutoff  );
	if(shader.get_uniform("light.constant"   ) != -1) glUniform1f(shader.get_uniform("light.constant"   ), attenuation.x);
	if(shader.get_uniform("light.linear"     ) != -1) glUniform1f(shader.get_uniform("light.linear"     ), attenuation.y);
	if(shader.get_uniform("light.quadratic"  ) != -1) glUniform1f(shader.get_uniform("light.quadratic"  ), attenuation.z);
	if(shader.get_uniform("light.attenuation") != -1) glUniform3f(shader.get_uniform("light.attenuation"), attenuation.x , attenuation.y, attenuation.z);
	/*if(shader.get_uniform("resolution") != -1)*/ glUniform2f(shader.get_uniform("resolution"), window_width, window_height);
	// Texture
	if(texture != nullptr)
	{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture->get_buffer());  // bind texture
	glUniform1i(shader.get_uniform("light.texture"), 0);
	// Draw
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	}    
	// Disable shaders
	glUseProgram(0); // disable program	
	// Clean shaders - TEMP
	shader.destroy();
#endif	
    // Texture

}		
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/*
    const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "void main()\n"
        "{\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
        "#version 330\n"
        "\n"
        "void main()\n"
        "{\n"
        "}\n"
	}	
*/
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////

void Renderer::draw_box(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
    double radius, bool iconify,
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
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();

	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"float round_corner(vec2 p, vec2 b, float r) {\n"
		    "return length(max(abs(p)-b+r, 0.0));\n"
		"}\n"
		"\n"
		"uniform vec2 resolution;\n"
		"vec2 position;" // Texcoord.x = from_left_to_right, Texcoord.y = from_up_to_down
		"struct Gradient {\n"
		    "vec4 color0;\n" // top
		    "vec4 color1;\n" // bottom
			"float value;\n"
		    "bool enabled;\n"
		"\n"
		"};\n"
		"uniform Gradient gradient;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		    "\n"
		    "\n"
			"\n"
		    "out_color = color;\n"
            "if(gradient.enabled == true)\n" 
			"{"
			    "position  = Texcoord; //out_color.xy / resolution;\n"
			    "out_color = vec4(mix(vec4(gradient.color0 + (1.0 - gradient.color0) * 0.25), vec4(gradient.color1 + (0.0 - gradient.color1) * 0.25), position.y));\n"
			    "\n"
			"}\n"
        "}\n"
	};
	// Shader - creates and prepares a shader (WARNING: Not good to create things in a loop unless you delete it after you are done using it or at the end of a function)
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare(); //shader.set_float("", );
	//--------------------------
	// tex_coord_array
    GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	// element_array
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	//--------------------------
	// TITLE_BAR
    // vertex array obj  - stores vertices
    GLuint title_bar_vertex_array_obj;
    glGenVertexArrays(1, &title_bar_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(title_bar_vertex_array_obj); // bind again  
	    GLuint title_bar_tex_coord_buffer_obj;
        glGenBuffers( 1, &title_bar_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, title_bar_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(title_bar_vertex_array_obj); // bind vertex array obj
        GLuint title_bar_vertex_buffer_obj;
        glGenBuffers(1, &title_bar_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, title_bar_vertex_buffer_obj);
		// set title_bar position relative to widget
		float title_bar_x = 0;
		float title_bar_y = -title_bar_height;
		int title_bar_width  = width; // is set automatically
		//std::cout << y + title_bar_y << " title_bar_y\n"; 
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x)                                          , static_cast<GLfloat>(y + title_bar_y),
            static_cast<GLfloat>(x + title_bar_x) + static_cast<GLfloat>(title_bar_width), static_cast<GLfloat>(y + title_bar_y),
            static_cast<GLfloat>(x + title_bar_x) + static_cast<GLfloat>(title_bar_width), static_cast<GLfloat>(y + title_bar_y) + static_cast<GLfloat>(title_bar_height),
            static_cast<GLfloat>(x + title_bar_x)                                          , static_cast<GLfloat>(y + title_bar_y) + static_cast<GLfloat>(title_bar_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(title_bar_vertex_array_obj); // (vao start 1)
	    GLuint title_bar_element_buffer_obj;
        glGenBuffers(1, &title_bar_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, title_bar_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// State
	glDisable(GL_DEPTH_TEST);                         // Disable 3d for User interdata
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);
	// Shader       - This is the part where the shader is going to start being used (for the purpose of keeping the code organized and in place) ******
	shader.use ();    
    // set shader properties ************** Always check to see if a uniform exists (or not) before applying values to it, using "if(shader.get_uniform("") != -1)"
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0)); //model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
    // projection (ortho2d) (no 'view' because GUI should not be affected by camera)          //float window_width  = get_display_width ();//float window_height = get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f); // make sure window_width and window_height are always updated and equal to the window client's size
	// pass data to the shader	
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif
    // set gradient    if(shader.get_uniform("") != -1)
	if(shader.get_uniform("resolution") != -1) shader.set_float("resolution", static_cast<double>(window_width), static_cast<double>(window_height));
	if(shader.get_uniform("gradient.enabled") != -1) shader.set_integer("gradient.enabled", static_cast<int>(gradient)); // set gradient on
	if(shader.get_uniform("gradient.color0" ) != -1) shader.set_float("gradient.color0", (red/255), (green/255), (blue/255), (alpha/255)); // color0 will be a tint  (top)
	if(shader.get_uniform("gradient.color1" ) != -1) shader.set_float("gradient.color1", (gradient_color.x/255), (gradient_color.y/255), (gradient_color.z/255), (gradient_color.w/255)); // color1 will be a shade (bottom)	
	// title_bar_outline
	if(title_bar)
	{
	//glEnable(GL_DEPTH_TEST);           // for removing hidden line
	//glEnable(GL_POLYGON_OFFSET_FILL);  // for removing hidden line	
	if(outline) {
	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	glLineWidth(outline_width);
	glBindVertexArray(title_bar_vertex_array_obj); // use same vao data as title_bar but this time in a line loop
	    glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}	
	// Draw title_bar
	shader.set_float("color", (title_bar_color.x / 255), (title_bar_color.y / 255), (title_bar_color.z / 255), (title_bar_color.w / 255));	
	glBindVertexArray(title_bar_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDisable(GL_POLYGON_OFFSET_FILL); // for removing hidden line
	//glDisable(GL_DEPTH_TEST);          // for removing hidden line
	}
	//--------------------------
	// CLOSE_BUTTON
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// vertex array obj  - stores vertices
	GLuint close_button_vertex_array_obj;
    glGenVertexArrays(1, &close_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(close_button_vertex_array_obj); // bind again  
	    GLuint close_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &close_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, close_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(close_button_vertex_array_obj); // bind vertex array obj
        GLuint close_button_vertex_buffer_obj;
        glGenBuffers(1, &close_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, close_button_vertex_buffer_obj); 
		float close_button_x = (title_bar_width - 10) - 1; // 1 is the right_padding
		float close_button_y = 2; // top_padding
		int close_button_width =  10;//title_bar_button_width;
		int close_button_height = title_bar_height - 5; // 5 is bottom_padding
		GLfloat vertices2[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x + close_button_x)                                             , static_cast<GLfloat>(y + title_bar_y + close_button_y),
            static_cast<GLfloat>(x + title_bar_x + close_button_x) + static_cast<GLfloat>(close_button_width), static_cast<GLfloat>(y + title_bar_y + close_button_y),
            static_cast<GLfloat>(x + title_bar_x + close_button_x) + static_cast<GLfloat>(close_button_width), static_cast<GLfloat>(y + title_bar_y + close_button_y) + static_cast<GLfloat>(close_button_height),
            static_cast<GLfloat>(x + title_bar_x + close_button_x)                                             , static_cast<GLfloat>(y + title_bar_y + close_button_y) + static_cast<GLfloat>(close_button_height),   
        };      		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(close_button_vertex_array_obj); // (vao start 1)
	    GLuint close_button_element_buffer_obj;
        glGenBuffers(1, &close_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, close_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw close_button
	if(title_bar) {
	if(title_bar_button_close) {
	shader.set_float("color", (title_bar_button_close_color.x / 255), (title_bar_button_close_color.y / 255), (title_bar_button_close_color.z / 255), (title_bar_button_close_color.w / 255));
    glBindVertexArray(close_button_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//--------------------------
	// MAXIMIZE_BUTTON
	// maximize_button_color
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    // vertex array obj  - stores vertices
	GLuint maximize_button_vertex_array_obj;
    glGenVertexArrays(1, &maximize_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(maximize_button_vertex_array_obj); // bind again  
	    GLuint maximize_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &maximize_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, maximize_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(maximize_button_vertex_array_obj); // bind vertex array obj
        GLuint maximize_button_vertex_buffer_obj;
        glGenBuffers(1, &maximize_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, maximize_button_vertex_buffer_obj); 
		float maximize_button_x = (title_bar_width - (10*2)) - 2; // 2 is the right_padding
		float maximize_button_y = 2; // top_padding
		int maximize_button_width =  10;//title_bar_width;
		int maximize_button_height = title_bar_height - 5;	// 5 is bottom_padding	
		GLfloat vertices3[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x + maximize_button_x)                                                , static_cast<GLfloat>(y + title_bar_y + maximize_button_y),
            static_cast<GLfloat>(x + title_bar_x + maximize_button_x) + static_cast<GLfloat>(maximize_button_width), static_cast<GLfloat>(y + title_bar_y + maximize_button_y),
            static_cast<GLfloat>(x + title_bar_x + maximize_button_x) + static_cast<GLfloat>(maximize_button_width), static_cast<GLfloat>(y + title_bar_y + maximize_button_y) + static_cast<GLfloat>(maximize_button_height),
            static_cast<GLfloat>(x + title_bar_x + maximize_button_x)                                                , static_cast<GLfloat>(y + title_bar_y + maximize_button_y) + static_cast<GLfloat>(maximize_button_height),   
        };      		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3)* sizeof(GLfloat), vertices3, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(maximize_button_vertex_array_obj); // (vao start 1)
	    GLuint maximize_button_element_buffer_obj;
        glGenBuffers(1, &maximize_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, maximize_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw maximize_button
	if(title_bar) {
	if(title_bar_button_maximize) {
    glBindVertexArray(maximize_button_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
	}
    //--------------------------	
	// MINIMIZE_BUTTON
	// iconify_button_color
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    // vertex array obj  - stores vertices
	GLuint iconify_button_vertex_array_obj;
    glGenVertexArrays(1, &iconify_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(iconify_button_vertex_array_obj); // bind again  
	    GLuint iconify_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &iconify_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, iconify_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(iconify_button_vertex_array_obj); // bind vertex array obj
        GLuint iconify_button_vertex_buffer_obj;
        glGenBuffers(1, &iconify_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, iconify_button_vertex_buffer_obj);
		float iconify_button_x = (title_bar_width - (10*3)) - 3; // 3 is the right_padding
		float iconify_button_y = 2; // top_padding
		int iconify_button_width =  10;//title_bar_width;
		int iconify_button_height = title_bar_height - 5; // 5 is bottom_padding
		GLfloat vertices4[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + title_bar_x + iconify_button_x)                                               , static_cast<GLfloat>(y + title_bar_y + iconify_button_y),
            static_cast<GLfloat>(x + title_bar_x + iconify_button_x) + static_cast<GLfloat>(iconify_button_width), static_cast<GLfloat>(y + title_bar_y + iconify_button_y),
            static_cast<GLfloat>(x + title_bar_x + iconify_button_x) + static_cast<GLfloat>(iconify_button_width), static_cast<GLfloat>(y + title_bar_y + iconify_button_y) + static_cast<GLfloat>(iconify_button_height),
            static_cast<GLfloat>(x + title_bar_x + iconify_button_x)                                               , static_cast<GLfloat>(y + title_bar_y + iconify_button_y) + static_cast<GLfloat>(iconify_button_height),   
        };      		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4)* sizeof(GLfloat), vertices4, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(iconify_button_vertex_array_obj); // (vao start 1)
	    GLuint iconify_button_element_buffer_obj;
        glGenBuffers(1, &iconify_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iconify_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw iconify_button
	if(title_bar) {
	if(title_bar_button_iconify) {	
    glBindVertexArray(iconify_button_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
    }
	//--------------------------
	// BOX (widget)
    // vertex array obj  - stores vertices
    GLuint box_vertex_array_obj;
    glGenVertexArrays(1, &box_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(box_vertex_array_obj); // bind again  
	    GLuint box_tex_coord_buffer_obj;
        glGenBuffers(1, &box_tex_coord_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, box_tex_coord_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(box_vertex_array_obj); // bind vertex array obj
        GLuint box_vertex_buffer_obj;
        glGenBuffers(1, &box_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, box_vertex_buffer_obj);
        GLfloat vertices[] = {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };   		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(box_vertex_array_obj); // (vao start 1)
	    GLuint box_element_buffer_obj;
        glGenBuffers(1, &box_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw box outline
	//glEnable(GL_DEPTH_TEST); // for removing hidden line
	//glEnable(GL_POLYGON_OFFSET_FILL);  // for removing hidden line
	//glPolygonOffset(1.0, 1.0);
	if(!iconify)
	{
	if(outline)
	{
    shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH);
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(box_vertex_array_obj); // use same vao data as box but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);	
	}
	// Draw box
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(box_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//glDisable(GL_POLYGON_OFFSET_FILL); // for removing hidden line
	//glDisable(GL_DEPTH_TEST); // for removing hidden line
	}
	//--------------------------
	// Clean textures
	// Clean buffers
	// : box (widget main body)
	glDeleteBuffers(1, &box_tex_coord_buffer_obj);
	glDeleteBuffers(1, &box_element_buffer_obj  );
	glDeleteBuffers(1, &box_vertex_buffer_obj   );
	// :title_bar
	glDeleteBuffers(1, &title_bar_tex_coord_buffer_obj);
	glDeleteBuffers(1, &title_bar_element_buffer_obj  );
	glDeleteBuffers(1, &title_bar_vertex_buffer_obj   );	
	// : close_button
	glDeleteBuffers(1, &close_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &close_button_element_buffer_obj  );
	glDeleteBuffers(1, &close_button_vertex_buffer_obj   );	
	// : maximize_button
	glDeleteBuffers(1, &maximize_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &maximize_button_element_buffer_obj  );
	glDeleteBuffers(1, &maximize_button_vertex_buffer_obj   );
    // : iconify_button	
	glDeleteBuffers(1, &iconify_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &iconify_button_element_buffer_obj  );
	glDeleteBuffers(1, &iconify_button_vertex_buffer_obj   );
	// :
	//glDeleteBuffers(1, &_tex_coord_buffer_obj);
	//glDeleteBuffers(1, &_element_buffer_obj  );
	//glDeleteBuffers(1, &_vertex_buffer_obj   );	
	// Clean arrays
	glDeleteVertexArrays(1, &box_vertex_array_obj      );
	glDeleteVertexArrays(1, &title_bar_vertex_array_obj);
	glDeleteVertexArrays(1, &close_button_vertex_array_obj);
	glDeleteVertexArrays(1, &maximize_button_vertex_array_obj);
	glDeleteVertexArrays(1, &iconify_button_vertex_array_obj);
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader.disable();
	shader.destroy();
#endif
}
////////////////////////////
////////////
void Renderer::draw_text (const std::string& text, double x, double y, int width, int height, double angle, double scale_x, double scale_y, const FONT& font, double red, double green, double blue, double alpha)
{
#ifdef DOKUN_OPENGL	
    context_check();
	if(font.get_data() == nullptr) // check font
	{
		Logger("Font contains no data");
		return;
	}	
    const char * vertex_source[] =
    {
		"#version 330\n"
        "layout (location = 0) in vec4 vertex;\n"
        "\n"
		"out vec2 Texcoord;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 view;\n"
		"\n"
        "void main(void)\n"
		"{\n"
			"Texcoord    = vec2(vertex.z, 1-vertex.w);\n"
            "gl_Position = proj * view * model * vec4(vertex.xy, 0.0, 1.0);\n"
		"}\n"
	};
	const char * fragment_source[] =
    {
	    "#version 330                                                          \n"
        "\n"
        "out vec4 out_color;\n"
        "in vec2 Texcoord  ;\n"
		"\n"
        "uniform sampler2D font;\n"
        "uniform vec3 color    ;\n"
        "\n"
        "void main(void) {\n"
            "\n"
			"out_color = vec4(color, 1.0) * vec4(1.0, 1.0, 1.0, texture(font, Texcoord).r);\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
    /////////////////////////////////////
	/////////////////////////////////////
	// vertex array obj
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj);
	    GLuint vertex_buffer_obj;
		glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
	// background rect - NEW! ---------------------------
	GLuint background_vertex_array_obj;
	glGenVertexArrays(1, &background_vertex_array_obj);
	// bg vertex_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_vertex_buffer_obj;
		glGenBuffers(1, &background_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_vertex_buffer_obj);
        GLfloat vertices[] = {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };   		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
	// bg texcoord_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_tex_coord_buffer_obj;
        glGenBuffers(1, &background_tex_coord_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1);		
    glBindVertexArray(0);	
	// bg element_buffer
    glBindVertexArray(background_vertex_array_obj);
	    GLuint background_element_buffer_obj;
        glGenBuffers(1, &background_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, background_element_buffer_obj);
        GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 		
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 		
    glBindVertexArray(0);	
    //---------------------------------------	
    // enable
	glEnable(GL_BLEND); // transparent background
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	
	// Shader
	shader.use ();
#ifdef use_glm	
	// model (object)
	glm::mat4 model = glm::mat4(1.0);
 	model = glm::translate(model, glm::vec3(-x, -y, 1.0));//model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, -1));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(x, y, -1.0));//model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	// projection and view (camera)            //float window_width = get_display_width ();//float window_height= get_display_height();
	glm::mat4 proj = glm::ortho(0.0f,  static_cast<float>(window_width), static_cast<float>(window_height), 0.0f);// glm::ortho(0.0f,  window_width, 0.0f, window_height); // flip y when changed ortho
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0)); // label uses a camera of its own
	// pass matrix data to the shader	
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"),  1, false, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj" ),  1, false, glm::value_ptr(proj) );
	if(shader.get_uniform("view") != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view" ),  1, false, glm::value_ptr(view) );
#endif	
	// Draw background ------------ NEW!!
	shader.set_float("color", (0.0f / 255), (51.0f / 255), (102.0f / 255)); // alpha
	glBindVertexArray(background_vertex_array_obj);
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Draw text
	glUniform3f(glGetUniformLocation(shader.get_program(), "color"), (red / 255), (green / 255), (blue / 255)); // alpha
	glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertex_array_obj);
    for (std::string::const_iterator c = text.begin(); c != text.end(); c++) // scan through all characters in string  // Iterate through all characters
	    {
		    FONT::Character character = const_cast<FONT&>(font).character_array[*c];
            // how high or low character is
            GLfloat xpos = x + character.bearing_x * scale_x;
            GLfloat ypos = y + (/*character*/const_cast<FONT&>(font).character_array['H'].bearing_y - character.bearing_y) * scale_y;
            GLfloat glyph_width  = character.width  * scale_x;
            GLfloat glyph_height = character.height * scale_y;
            // Update vertex_buffer_obj for each character
            GLfloat vertices[6][4] = {
            { xpos              , ypos + glyph_height,   0.0, 0.0 },            
            { xpos              , ypos,       0.0, 1.0 },
            { xpos + glyph_width, ypos,       1.0, 1.0 },

            { xpos              , ypos + glyph_height,   0.0, 0.0 },
            { xpos + glyph_width, ypos,       1.0, 1.0 },
            { xpos + glyph_width, ypos + glyph_height,   1.0, 0.0 }     // use custom height      
            };
        // Update content
		GLuint texture = character.id;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Draw text
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (character.advance >> 6) * scale_x; // Bitshift by 6 to get value in pixels (2^6 = 64)
	    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	// Clean textures
	// Clean buffers
	// : text
	glDeleteBuffers(1, & vertex_buffer_obj);
	// : background
	glDeleteBuffers(1, & background_vertex_buffer_obj);
	glDeleteBuffers(1, & background_tex_coord_buffer_obj);
	glDeleteBuffers(1, & background_element_buffer_obj);
	// Clean arrays
	glDeleteVertexArrays(1, & vertex_array_obj);
	glDeleteVertexArrays(1, & background_vertex_array_obj);
	// disable
	glDisable(GL_BLEND);
	// program
	shader.disable(); 
    shader.destroy();
#endif
} 
////////////////////////////
void Renderer::draw_glyph (unsigned char glyph, double x, double y, double angle, double scale_x, double scale_y, const FONT& font, double red, double green, double blue, double alpha)
{
#ifdef DOKUN_OPENGL	
    context_check();
	if(font.get_data() == nullptr) // check font
	{
		Logger("Font contains no data");
		return;
	}	
    const char * vertex_source[] =
    {
		"#version 330\n"
        "layout (location = 0) in vec4 vertex;\n"
        "\n"
		"out vec2 Texcoord;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 view;\n"
		"\n"
        "void main(void)\n"
		"{\n"
			"Texcoord    = vec2(vertex.z, 1 - vertex.w);\n"
            "gl_Position = proj  * model * vec4(vertex.xy, 0.0, 1.0);\n"
		"}\n"
	};
	const char * fragment_source[] =
    {
	    "#version 330                                                          \n"
        "\n"
        "out vec4 out_color;\n"
        "in vec2 Texcoord  ;\n"
		"\n"
        "uniform sampler2D font;\n"
        "uniform vec3 color    ;\n"
        "\n"
        "void main(void) {\n"
            "\n"
			"out_color = vec4(color, 1.0) * vec4(1.0, 1.0, 1.0, texture(font, Texcoord).r);\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
    /////////////////////////////////////
	/////////////////////////////////////
	// vertex array obj
	GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj);
	    GLuint vertex_buffer_obj;
		glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);      
    // enable
	glEnable(GL_BLEND); // transparent background
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	
	// Shader
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);; 
	int width  = const_cast<FONT&>(font).character_array[glyph].width;
	int height = const_cast<FONT&>(font).character_array[glyph].height;
 	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, -1));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(static_cast<float>(scale_x), static_cast<float>(scale_y), 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
    // projection and view (camera)
	glm::mat4 proj = glm::ortho(0.0f,  static_cast<float>(window_width), static_cast<float>(window_height), 0.0f); //glm::ortho(0.0f,  window_width, 0.0f, window_height); // flip y when changed ortho
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0,-1), glm::vec3(0, 1, 0));
		
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"),  1, false, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj" ),  1, false, glm::value_ptr(proj) );
	if(shader.get_uniform("view") != -1) glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "view" ),  1, false, glm::value_ptr(view) );
#endif	
	glUniform3f(glGetUniformLocation(shader.get_program(), "color"), (red / 255), (green / 255), (blue / 255)); // alpha
	glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertex_array_obj);
    // character (glpyh)
	FONT::Character character = const_cast<FONT&>(font).character_array[glyph];
            // how high or low character is
            GLfloat xpos = x +  const_cast<FONT&>(font).character_array[glyph].bearing_x * scale_x;
            GLfloat ypos = y - (const_cast<FONT&>(font).character_array[glyph].height - const_cast<FONT&>(font).character_array[glyph].bearing_y) * scale_y;
            GLfloat w = const_cast<FONT&>(font).get_width(glyph);//const_cast<FONT&>(font).character_array[glyph].width  * scale_x;
            GLfloat h = const_cast<FONT&>(font).get_height(glyph);//const_cast<FONT&>(font).character_array[glyph].height * scale_y;
            // Update vertex_buffer_obj for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },            
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }           
            };
        // Update content
		GLuint texture = const_cast<FONT&>(font).character_array[glyph].id;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Draw text
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (const_cast<FONT&>(font).character_array[glyph].advance >> 6) * scale_x; // Bitshift by 6 to get value in pixels (2^6 = 64)
    
	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	// Clean textures
	// Clean buffers
	glDeleteBuffers(1, & vertex_buffer_obj);
	// Clean arrays
	glDeleteVertexArrays(1, & vertex_array_obj);
	// disable
	glDisable(GL_BLEND);
	// program
	shader.disable(); 
    shader.destroy();
#endif	
}
////////////////////////////
////////////
void Renderer::draw_button(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, 
	// outline
	bool outline, 
	double outline_width, 
	const Vector4& outline_color,
	bool outline_antialiased
	// border
	//bool border,
	//const Vector4& border_color,	
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//if(!fill) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"uniform bool is_image;\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
	// uniform
#ifdef use_glm	
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
    // --------------------
	// BUTTON   
    // vertex array obj  - stores vertices
    GLuint button_vertex_array_obj;
    glGenVertexArrays(1, &button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind again  
	    GLuint button_tex_coord_buffer_obj;
        glGenBuffers( 1, &button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, button_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind vertex array obj
        GLuint button_vertex_buffer_obj;
        glGenBuffers(1, &button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, button_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(button_vertex_array_obj); // (vao start 1)
	    GLuint button_element_buffer_obj;
        glGenBuffers(1, &button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button_element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw button_outline
	if(outline)
	{
    shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH);
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(button_vertex_array_obj); // use same vao data as box but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
	// Draw button
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// Clean textures
	// Clean buffers
	glDeleteBuffers(1, &button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &button_element_buffer_obj);
	glDeleteBuffers(1, &button_vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &button_vertex_array_obj);
	// Restore defaults
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader.disable();
	shader.destroy();
#endif
}
////////////
void Renderer::draw_progressbar(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha, 
    double min_value, double max_value, double value, const Vector4& background_color,
	// outline
	bool outline, 
	double outline_width, 
	const Vector4& outline_color,
	bool outline_antialiased
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	//float window_width  = Renderer::get_display_width ();
	//float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	// texcoord array
	GLfloat tex_coords[] = 
	{ 
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	// element array
	GLuint indices[] = {0, 1, 3,  1, 2, 3,};
	//------------------------
    // BORDER - at the far back of all other elements; can also add border from image
/*
    // vertex array obj  - stores vertices
    GLuint border_vertex_array_obj;
    glGenVertexArrays(1, &border_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(border_vertex_array_obj); // bind again  
	    GLuint border_tex_coord_buffer_obj;
        glGenBuffers( 1, &border_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, border_tex_coord_buffer_obj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(border_vertex_array_obj); // bind vertex array obj
        GLuint border_vertex_buffer_obj;
        glGenBuffers(1, &border_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, border_vertex_buffer_obj);
		float border_x = 0;
		float border_y = 0;
		float border_width  = width  * 2;
		float border_height = height * 2;
		GLfloat vertices2[] = {
		    static_cast<GLfloat>(x + border_x)                                     , static_cast<GLfloat>(y + border_y),
            static_cast<GLfloat>(x + border_x) + static_cast<GLfloat>(border_width), static_cast<GLfloat>(y + border_y),
            static_cast<GLfloat>(x + border_x) + static_cast<GLfloat>(border_width), static_cast<GLfloat>(y + border_y) + static_cast<GLfloat>(border_height),
            static_cast<GLfloat>(x + border_x)                                     , static_cast<GLfloat>(y + border_y) + static_cast<GLfloat>(border_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(border_vertex_array_obj); // (vao start 1)
	    GLuint border_element_buffer_obj;
        glGenBuffers(1, &border_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_element_buffer_obj);
		GLuint indices2[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2)* sizeof(GLuint), indices2, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw border
	int border_red = 128; int border_green = 128; int border_blue = 128; int border_alpha = 255;
    shader.set_float("color", (border_red / 255), (border_green / 255), (border_blue / 255), (border_alpha / 255));
    glBindVertexArray(border_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	std::cout << dokun_opengl_error() << "\n";
	*/
	//------------------------
    // STATIC BAR (background) -  drawn underneath moving bar
    // vertex array obj  - stores vertices
    GLuint background_vertex_array_obj;
    glGenVertexArrays(1, &background_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(background_vertex_array_obj); 
	    GLuint background_tex_coord_buffer_obj;
        glGenBuffers( 1, &background_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, background_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0);
	// vertex buffer obj
    glBindVertexArray(background_vertex_array_obj);
        GLuint background_vertex_buffer_obj;
        glGenBuffers(1, &background_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, background_vertex_buffer_obj); 
        GLfloat vertices[] = 
	    {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };  		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); 
	// element buffer obj
	glBindVertexArray(background_vertex_array_obj);
	    GLuint background_element_buffer_obj;
        glGenBuffers(1, &background_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, background_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);
    // Draw static_bar (background) outline
	if(outline == true)
	{
    shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH);
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(background_vertex_array_obj); // use same vao data as background but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);		
	}
	// Draw static_bar (background)
	shader.set_float("color", (background_color.x / 255), (background_color.y / 255), (background_color.z / 255), (background_color.w / 255));
    glBindVertexArray(background_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//std::cout << dokun_opengl_error() << "\n";
	// ----------------------
	// MOVING BAR (foreground) - drawn on top of static bar (background)
    // vertex array obj  - stores vertices
    GLuint foreground_vertex_array_obj;
    glGenVertexArrays(1, &foreground_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(foreground_vertex_array_obj); // bind again  
	    GLuint foreground_tex_coord_buffer_obj;
        glGenBuffers( 1, &foreground_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, foreground_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(foreground_vertex_array_obj); // bind vertex array obj
        GLuint foreground_vertex_buffer_obj;
        glGenBuffers(1, &foreground_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, foreground_vertex_buffer_obj);
		GLfloat progress = value / max_value; // 1.0 = max_value reached !
		GLfloat vertices1[] = {
		    static_cast<GLfloat>(x)                                           , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress * static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress * static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                           , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<GLvoid *>(0));
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(foreground_vertex_array_obj); // (vao start 1)
	    GLuint foreground_element_buffer_obj;
        glGenBuffers(1, &foreground_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, foreground_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);
	// Draw moving_bar(foreground) outline
	if(outline == true)
	{
	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(foreground_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
    }
	// Draw moving_bar (foreground)
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255)); //  foreground_color
    glBindVertexArray(foreground_vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//std::cout << dokun_opengl_error() << "\n";
	// Clean buffers
	// :static bar (background)
	glDeleteBuffers(1, &background_tex_coord_buffer_obj);
	glDeleteBuffers(1, &background_element_buffer_obj  );
	glDeleteBuffers(1, &background_vertex_buffer_obj   );
	// :moving bar (foreground)
	glDeleteBuffers(1, &foreground_tex_coord_buffer_obj);
	glDeleteBuffers(1, &foreground_element_buffer_obj  );
	glDeleteBuffers(1, &foreground_vertex_buffer_obj   );
	// : border
	//glDeleteBuffers(1, &border_tex_coord_buffer_obj);
	//glDeleteBuffers(1, &border_element_buffer_obj  );
	//glDeleteBuffers(1, &border_vertex_buffer_obj   );		
	// Clean arrays
	//glDeleteVertexArrays(1, &border_vertex_array_obj    );
	glDeleteVertexArrays(1, &foreground_vertex_array_obj);
	glDeleteVertexArrays(1, &background_vertex_array_obj);
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth (1.0);
	// Disable program
	shader.disable();
    shader.destroy();
#endif
}
////////////
void Renderer::draw_edit(const std::string& text, int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	bool multilined,
	// cursor
	bool cursor,
	double cursor_x,
	double cursor_y
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
    //if(!text.empty()) draw_label(text,);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width (); // maybe remove?
	float window_height = Renderer::get_display_height(); // maybe remove?
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//-------------------------------
	//-------------------------------
	// TEXT EDIT
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw edit outline
	/*shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/
	// Draw edit
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
    //-----------------------------------
    // CURSOR
    // vertex array obj  - stores vertices
    GLuint cursor_vertex_array_obj;
    glGenVertexArrays(1, &cursor_vertex_array_obj);	
    // vertex buffer obj
    glBindVertexArray(cursor_vertex_array_obj); // bind vertex array obj
        GLuint cursor_vertex_buffer_obj;
        glGenBuffers(1, &cursor_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, cursor_vertex_buffer_obj);
		int top_padding    = 1;
		int bottom_padding = 2;
		cursor_y = cursor_y + top_padding;
		int cursor_width  = 0;
		int cursor_height = height - bottom_padding; // bottom_padding
        if(multilined)
		    cursor_height = (height/2);
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + cursor_x) + static_cast<float>(cursor_width), static_cast<GLfloat>(y + cursor_y),
            static_cast<GLfloat>(x + cursor_x) + static_cast<float>(cursor_width), static_cast<GLfloat>(y + cursor_y) + static_cast<float>(cursor_height),
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)	
	// Draw cursor
	shader.set_float("color", (0 / 255), (0 / 255), (0 / 255), (255 / 255));
    //glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	glBindVertexArray(cursor_vertex_array_obj);
        glDrawArrays(GL_LINES, 0,  2);
	glBindVertexArray(0);         
	//-----------------------------------
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	// : edit
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// : cursor
	glDeleteBuffers(1, &cursor_vertex_buffer_obj);
	// Clean arrays
	// :edit
	glDeleteVertexArrays(1, &vertex_array_obj);
	// : cursor
	glDeleteVertexArrays(1, &cursor_vertex_array_obj);
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	// Disable program
	shader.disable();
	shader.destroy();
#endif
}
////////////
void Renderer::draw_slider(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
    // beam
	double min_value, double max_value, double value, const Vector4& background_color,
	// ball
	int ball_width, const Vector4& ball_color)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//----------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };	
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	//----------------------------
	// BEAM (BACKGROUND)
    // vertex array obj  - stores vertices
    GLuint slider_vertex_array_obj;
    glGenVertexArrays(1, &slider_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(slider_vertex_array_obj); // bind again  
	    GLuint slider_tex_coord_buffer_obj;
        glGenBuffers( 1, &slider_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, slider_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(slider_vertex_array_obj); // bind vertex array obj
        GLuint slider_vertex_buffer_obj;
        glGenBuffers(1, &slider_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, slider_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(slider_vertex_array_obj); // (vao start 1)
	    GLuint slider_element_buffer_obj;
        glGenBuffers(1, &slider_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, slider_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw slider(beam) outline
	/*shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(slider_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/
	// Draw slider (beam)
	shader.set_float("color", (background_color.x / 255), (background_color.y / 255), (background_color.z / 255), (background_color.w / 255));
    glBindVertexArray(slider_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //----------------------------
    // BEAM_BAR (foreground)
    // vertex array obj  - stores vertices
    GLuint bar_vertex_array_obj;
    glGenVertexArrays(1, &bar_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(bar_vertex_array_obj); // bind again  
	    GLuint bar_tex_coord_buffer_obj;
        glGenBuffers( 1, &bar_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, bar_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(bar_vertex_array_obj); // bind vertex array obj
        GLuint bar_vertex_buffer_obj;
        glGenBuffers(1, &bar_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bar_vertex_buffer_obj);
		// controls where the bar goes
		// to get x position of pivot = x + (value * 2). If value is 100 then pivot_x is 200
		// to get y position of pivot = y
		// position relative to the slider
		float progress = value / max_value; // value changes when pivotx is moved
		GLfloat vertices1[] = {
		    static_cast<GLfloat>(x)                                           , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress*static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + progress*static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                           , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bar_vertex_array_obj); // (vao start 1)
	    GLuint bar_element_buffer_obj;
        glGenBuffers(1, &bar_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bar_element_buffer_obj);  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	/* // Draw beam_bar outline
	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/
	// Draw beam_bar (foreground)
	shader.set_float("color", (red/ 255), (green / 255), (blue / 255), (alpha / 255)); //  foreground_color
    glBindVertexArray(bar_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //-------------------------------------------
    //---------BALL / HANDLE
    // vertex array obj  - stores vertices
    GLuint ball_vertex_array_obj;
    glGenVertexArrays(1, &ball_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(ball_vertex_array_obj); // bind again  
	    GLuint ball_tex_coord_buffer_obj;
        glGenBuffers( 1, &ball_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, ball_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(ball_vertex_array_obj); // bind vertex array obj
        GLuint ball_vertex_buffer_obj;
        glGenBuffers(1, &ball_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, ball_vertex_buffer_obj);
		// pivot size = 10 x height = ball_width x ball_height
		// position relative to slider
		int ball_top_padding    = 1;     // space between handle and top part of slider
		int ball_bottom_padding = 2;     // space between handle and bottom of slider
		int ball_height = height - ball_bottom_padding; // adjust the height a bit                                                                // height (slider_height)
		float ball_y    = 0 + ball_top_padding;             // move down 1 unit away from slider (bottom padding???)               // 0      (slider_y + ball_y) 
		float ball_x    = (value / max_value) * static_cast<GLfloat>(width); // value starts at 0		
		GLfloat vertices2[] = {
		    static_cast<GLfloat>(x + ball_x)                                     , static_cast<GLfloat>(y + ball_y),
            static_cast<GLfloat>(x + ball_x) + static_cast<GLfloat>(ball_width), static_cast<GLfloat>(y + ball_y),
            static_cast<GLfloat>(x + ball_x) + static_cast<GLfloat>(ball_width), static_cast<GLfloat>(y + ball_y) + static_cast<GLfloat>(ball_height),
            static_cast<GLfloat>(x + ball_x)                                     , static_cast<GLfloat>(y + ball_y) + static_cast<GLfloat>(ball_height),   
        };      //std::cout << x + ball_x << " pivot_x\n";
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(ball_vertex_array_obj); // (vao start 1)
	    GLuint ball_element_buffer_obj;
        glGenBuffers(1, &ball_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ball_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw ball / handle
	shader.set_float("color", (ball_color.x/ 255), (ball_color.y / 255), (ball_color.z / 255), (ball_color.w / 255));
    glBindVertexArray(ball_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	// Clean textures
	// Clean buffers
	// :slider
	glDeleteBuffers(1, &slider_tex_coord_buffer_obj);
	glDeleteBuffers(1, &slider_element_buffer_obj  );
	glDeleteBuffers(1, &slider_vertex_buffer_obj   );
	// :bar
	glDeleteBuffers(1, &bar_tex_coord_buffer_obj);
	glDeleteBuffers(1, &bar_element_buffer_obj  );
	glDeleteBuffers(1, &bar_vertex_buffer_obj   );
	// :ball
	glDeleteBuffers(1, &ball_tex_coord_buffer_obj);
	glDeleteBuffers(1, &ball_element_buffer_obj  );
	glDeleteBuffers(1, &ball_vertex_buffer_obj   );
	// Clean arrays
	glDeleteVertexArrays(1, &slider_vertex_array_obj);
	glDeleteVertexArrays(1, &bar_vertex_array_obj   );
	glDeleteVertexArrays(1, &ball_vertex_array_obj  );
	// Restore defaults
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	// Disable program
	shader.disable();
	shader.destroy();
	//std::cout << dokun_opengl_error() << std::endl;
#endif
}
////////////
void Renderer::draw_switch(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	int value,
	const Vector4& background_color_on,
	const Vector4& background_color_off,
	// border
	bool outline,
	double outline_width,
	const Vector4& outline_color,
	bool outline_antialiased
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	// SWITCH (background / body)
    // vertex array obj  - stores vertices 
    GLuint switch_vertex_array_obj;
    glGenVertexArrays(1, &switch_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(switch_vertex_array_obj); // bind again  
	    GLuint switch_tex_coord_buffer_obj;
        glGenBuffers( 1, &switch_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, switch_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(switch_vertex_array_obj); // bind vertex array obj
        GLuint switch_vertex_buffer_obj;
        glGenBuffers(1, &switch_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, switch_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(switch_vertex_array_obj); // (vao start 1)
	    GLuint switch_element_buffer_obj;
        glGenBuffers(1, &switch_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, switch_element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw switch(background) outline
	if(outline)
	{
	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(switch_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
	// Draw switch (background)
	if(value == 0) shader.set_float("color", (background_color_off.x / 255), (background_color_off.y / 255), (background_color_off.z / 255), (background_color_off.w / 255));
	if(value == 1) shader.set_float("color", (background_color_on.x  / 255), (background_color_on.y  / 255), (background_color_on.z  / 255), (background_color_on.w  / 255));
    glBindVertexArray(switch_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//------------------------------
	// HANDLE (foreground)
    // vertex array obj  - stores vertices
    GLuint handle_vertex_array_obj;
    glGenVertexArrays(1, &handle_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind again  
	    GLuint handle_tex_coord_buffer_obj;
        glGenBuffers( 1, &handle_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, handle_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind vertex array obj
        GLuint handle_vertex_buffer_obj;
        glGenBuffers(1, &handle_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, handle_vertex_buffer_obj);
		// set position relative to switch
		float top_padding    = 1;    // space between the top_part of switch and the handle 
		float bottom_padding = 2; // space between bottom_part of switch and the handle
		float handle_x = 0;
		float handle_y = 0 + top_padding;  // move handle down a bit by 1 unit (for padding purposes)
        int handle_width  = (width / 2); // handle is half of the switch's width
        int handle_height =	height - bottom_padding;  // reduce the height by 2 pixels (for padding purposes)
		if(value == 1) // if turned on
			handle_x = handle_x + (width / 2); // move handle to the other half of the switch
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + handle_x)                                , static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),
            static_cast<GLfloat>(x + handle_x)                                , static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),   
        };		
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(handle_vertex_array_obj); // (vao start 1)
	    GLuint handle_element_buffer_obj;
        glGenBuffers(1, &handle_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_element_buffer_obj);  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw handle outline
	if(outline)
	{
	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(handle_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);		
	}
	// Draw handle switch
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(handle_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);  
	// Clean textures
	// Clean buffers
	// :toggle
	glDeleteBuffers(1, &switch_tex_coord_buffer_obj);
	glDeleteBuffers(1, &switch_element_buffer_obj);
	glDeleteBuffers(1, &switch_vertex_buffer_obj );
	// :switch
	glDeleteBuffers(1, &handle_tex_coord_buffer_obj);
	glDeleteBuffers(1, &handle_element_buffer_obj);
	glDeleteBuffers(1, &handle_vertex_buffer_obj );	
	// Clean arrays
	glDeleteVertexArrays(1, &handle_vertex_array_obj);
	glDeleteVertexArrays(1, &switch_vertex_array_obj);
	// Restore defaults
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader.disable();
	shader.destroy();
#endif
}
////////////
void Renderer::draw_tooltip(const std::string& text, int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha)
{/*
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// set outline
	//glLineWidth(10.0);
		float radius = 50.0f;
	float angle_rad = 3.14159265359f * angle / 180.f;
	float x_rad = sin(angle) * radius;
	float y_rad = cos(angle) * radius;
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + x_rad)                                , static_cast<GLfloat>(y + y_rad),
            static_cast<GLfloat>(x + x_rad) + static_cast<GLfloat>(width), static_cast<GLfloat>(y + y_rad),
            static_cast<GLfloat>(x + x_rad) + static_cast<GLfloat>(width), static_cast<GLfloat>(y + y_rad) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x + x_rad)                                , static_cast<GLfloat>(y + y_rad) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	/
	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);	/
	// Draw
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
//---------------------------------------------
//--------- TRIANGLE POINTER --------------------------
/
shader.disable();
program.use();
shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj1;
    glGenVertexArrays(1, &vertex_array_obj1);
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj1); // bind vertex array obj
        GLuint vertex_buffer_obj1;
        glGenBuffers(1, &vertex_buffer_obj1);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj1);
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
		};      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)
	// Draw
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(vertex_array_obj1); // (vao start 2)
        glDrawArrays(GL_TRIANGLES, 0, 3);//glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	/
//---------------------------------------------	
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// triangle
	//glDeleteBuffers(1, &tex_coord_buffer_obj1);
	//glDeleteBuffers(1, &element_buffer_obj1);
	//glDeleteBuffers(1, &vertex_buffer_obj1 );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	//glDeleteVertexArrays(1, &vertex_array_obj1);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable program
	shader.disable();
	shader.destroy();
	if(!text.empty()) {
		//draw_label(text, x, y);
	}
#endif		*/	
}
////////////
void Renderer::draw_radio(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
	    int value, Vector4& background_color,
		// outline
		bool outline,
		double outline_width,
		const Vector4& outline_color,
		bool outline_antialiased
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//----------------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 	
	// RADIO (background)
    // vertex array obj  - stores vertices
    GLuint radio_vertex_array_obj;
    glGenVertexArrays(1, &radio_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(radio_vertex_array_obj); // bind again  
	    GLuint radio_tex_coord_buffer_obj;
        glGenBuffers( 1, &radio_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, radio_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(radio_vertex_array_obj); // bind vertex array obj
        GLuint radio_vertex_buffer_obj;
        glGenBuffers(1, &radio_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, radio_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(radio_vertex_array_obj); // (vao start 1)
	    GLuint radio_element_buffer_obj;
        glGenBuffers(1, &radio_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, radio_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw radio outline
	if(outline)
	{
	if(value == 0) shader.set_float("color", (64.0f / 255), (64.0f / 255), (64.0f / 255), (255.0f / 255)); 
	if(value == 1) shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	if(outline_antialiased) glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(radio_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);		
	}
	// Draw radio
	shader.set_float("color", (background_color.x / 255), (background_color.y / 255), (background_color.z / 255), (background_color.w / 255));
    glBindVertexArray(radio_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//----------------------------
	// INNER QUAD (foreground)
	 // vertex array obj  - stores vertices
    GLuint inner_vertex_array_obj;
    glGenVertexArrays(1, &inner_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(inner_vertex_array_obj); // bind again  
	    GLuint inner_tex_coord_buffer_obj;
        glGenBuffers( 1, &inner_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, inner_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(inner_vertex_array_obj); // bind vertex array obj
        GLuint inner_vertex_buffer_obj;
        glGenBuffers(1, &inner_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, inner_vertex_buffer_obj);
		// set position relative to radio 
		// set size to half of width and height
		int fill_x = width  / 4;         
		int fill_y = height / 4;
		int fill_width  = width  / 2;
		int fill_height = height / 2;
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + fill_x)                                     , static_cast<GLfloat>(y + fill_y),
            static_cast<GLfloat>(x + fill_x) + static_cast<GLfloat>(fill_width), static_cast<GLfloat>(y + fill_y),
            static_cast<GLfloat>(x + fill_x) + static_cast<GLfloat>(fill_width), static_cast<GLfloat>(y + fill_y) + static_cast<GLfloat>(fill_height),
            static_cast<GLfloat>(x + fill_x)                                     , static_cast<GLfloat>(y + fill_y) + static_cast<GLfloat>(fill_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(inner_vertex_array_obj); // (vao start 1)
	    GLuint inner_element_buffer_obj;
        glGenBuffers(1, &inner_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inner_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw inner outline
	/*if(outline)
	{
	if(value == 0) shader.set_float("color", (64.0f / 255), (64.0f / 255), (64.0f / 255), (255.0f / 255)); 
	if(value == 1) shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	if(outline_antialiased) glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // 
	glBindVertexArray(inner_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);		
	}*/
	// Draw inner quad
	if(value == 0) shader.set_float("color", (background_color.x / 255), (background_color.y / 255), (background_color.z / 255), (background_color.w / 255));
	if(value == 1) shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(inner_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//----------------------------
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &radio_tex_coord_buffer_obj);
	glDeleteBuffers(1, &radio_element_buffer_obj);
	glDeleteBuffers(1, &radio_vertex_buffer_obj );
	// inner
	glDeleteBuffers(1, &inner_tex_coord_buffer_obj);
	glDeleteBuffers(1, &inner_element_buffer_obj);
	glDeleteBuffers(1, &inner_vertex_buffer_obj );	
	// Clean arrays
	glDeleteVertexArrays(1, &radio_vertex_array_obj);
	glDeleteVertexArrays(1, &inner_vertex_array_obj);
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader.disable();
	shader.destroy();
#endif
}
////////////
////////////
void Renderer::draw_scrollbar(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
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
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
    //------------------------
	// tex_coord array
	GLfloat tex_coords[] =  // texture coordinates range from (0,0) to (1, 1)
	{
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };
	// vertex array - all unique
	// element array
	GLuint indices[] = {0, 1, 3,  1, 2, 3,};
	//--------------------
	// BORDER - not sure If scrollbars need a border
	//--------------------
    // SCROLLBAR
    // vertex array obj  - stores vertices
    GLuint scrollbar_vertex_array_obj;
    glGenVertexArrays(1, &scrollbar_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(scrollbar_vertex_array_obj); // bind again  
	    GLuint scrollbar_tex_coord_buffer_obj;
        glGenBuffers( 1, &scrollbar_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, scrollbar_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(scrollbar_vertex_array_obj); // bind vertex array obj
        GLuint scrollbar_vertex_buffer_obj;
        glGenBuffers(1, &scrollbar_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, scrollbar_vertex_buffer_obj);  
		GLfloat vertices1[] = {
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),    
        }; 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(scrollbar_vertex_array_obj); // (vao start 1)
	    GLuint scrollbar_element_buffer_obj;
        glGenBuffers(1, &scrollbar_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scrollbar_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	if(outline) {
	// Draw scrollbar outline
    shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(scrollbar_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}	
	// Draw scrollbar
    shader.set_float("color", (red/ 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(scrollbar_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //------------------------
	// TOP BUTTON - optional
    // vertex array obj  - stores vertices
    GLuint top_button_vertex_array_obj;
    glGenVertexArrays(1, &top_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind again  
	    GLuint top_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &top_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, top_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind vertex array obj
        GLuint top_button_vertex_buffer_obj;
        glGenBuffers(1, &top_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, top_button_vertex_buffer_obj);
		int top_x      = 0;
		int top_y      = -button_height; // move up 20 units away from scrollbar
		int top_width  = width;
		int top_height = button_height;
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + top_x)                                    , static_cast<GLfloat>(y + top_y),
            static_cast<GLfloat>(x + top_x) + static_cast<GLfloat>(top_width), static_cast<GLfloat>(y + top_y),
            static_cast<GLfloat>(x + top_x) + static_cast<GLfloat>(top_width), static_cast<GLfloat>(y + top_y) + static_cast<GLfloat>(top_height),
            static_cast<GLfloat>(x + top_x)                                    , static_cast<GLfloat>(y + top_y) + static_cast<GLfloat>(top_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(top_button_vertex_array_obj); // (vao start 1)
	    GLuint top_button_element_buffer_obj;
        glGenBuffers(1, &top_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, top_button_element_buffer_obj);  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	if(button)
	{
	if(outline) {
	// Draw top_button outline
    shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(top_button_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}	
	// Draw top_button
	shader.set_float("color", (button_color.x / 255), (button_color.y / 255), (button_color.z / 255), (button_color.w / 255));
    glBindVertexArray(top_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	}
	//-------------------------
	// UP ARROW
	// up_arrow_color
	//shader.set_float("color", (32.0f / 255), (32.0f / 255), (32.0f / 255), (255 / 255));
    //-------------------------
	// BOTTOM BUTTON - optional
    // vertex array obj  - stores vertices
    GLuint bottom_button_vertex_array_obj;
    glGenVertexArrays(1, &bottom_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind again  
	    GLuint bottom_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &bottom_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, bottom_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind vertex array obj
        GLuint bottom_button_vertex_buffer_obj;
        glGenBuffers(1, &bottom_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bottom_button_vertex_buffer_obj);
		// set position relative to scrollbar
		int bottom_x      = 0;
		int bottom_y      = height; // move 'height' units down past the scrollbar
		int bottom_width  = width; // dont worry about this
		int bottom_height = button_height;
		GLfloat vertices3[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + bottom_x)                                       , static_cast<GLfloat>(y + bottom_y),
            static_cast<GLfloat>(x + bottom_x) + static_cast<GLfloat>(bottom_width), static_cast<GLfloat>(y + bottom_y),
            static_cast<GLfloat>(x + bottom_x) + static_cast<GLfloat>(bottom_width), static_cast<GLfloat>(y + bottom_y) + static_cast<GLfloat>(bottom_height),
            static_cast<GLfloat>(x + bottom_x)                                       , static_cast<GLfloat>(y + bottom_y) + static_cast<GLfloat>(bottom_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3)* sizeof(GLfloat), vertices3, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 1)
	    GLuint bottom_button_element_buffer_obj;
        glGenBuffers(1, &bottom_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottom_button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw bottom_button outline
	if(button)
	{
	if(outline) {
	// Draw scrollbar outline
    shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(bottom_button_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}	
	// Draw bottom_button
	shader.set_float("color", (button_color.x / 255), (button_color.y / 255), (button_color.z / 255), (button_color.w / 255));
    glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	}
	// -------------------------------
	// DOWN ARROW 
	// down_arrow_color
	//shader.set_float("color", (32.0f / 255), (32.0f / 255), (32.0f / 255), (255 / 255));
    // -------------------------------
	// HANDLE 
    // vertex array obj  - stores vertices
    GLuint handle_vertex_array_obj;
    glGenVertexArrays(1, &handle_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind again  
	    GLuint handle_tex_coord_buffer_obj;
        glGenBuffers( 1, &handle_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, handle_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(handle_vertex_array_obj); // bind vertex array obj
        GLuint handle_vertex_buffer_obj;
        glGenBuffers(1, &handle_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, handle_vertex_buffer_obj);
		// controls where the bar goes
		// to get x position of pivot = x + (value * 2). If value is 100 then pivot is 200
		// to get y position of pivot = y
		// position relative to the scrollbar
		//numScrollPositions = (numItemsInList - numItemsVisible) + 1
		float num_of_list_item = 10; // TEMPORARY
		float handle_y    = 0;//(value / step) * static_cast<GLfloat>(width); // value starts at 0
		float handle_x    = 0;//top_x - top_height ;
		int handle_width  = width; // same width as bar
		// calculate here - INCOMPLETE 
		float max_value = 100;
		float step = value / max_value; // value changes when pivotx is moved  		
		GLfloat vertices2[] = {
		    static_cast<GLfloat>(x + handle_x)                                       , static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y),
            static_cast<GLfloat>(x + handle_x) + static_cast<GLfloat>(handle_width), static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),
            static_cast<GLfloat>(x + handle_x)                                       , static_cast<GLfloat>(y + handle_y) + static_cast<GLfloat>(handle_height),   
        };      //std::cout << x + ball_x << " pivot_x\n";
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(handle_vertex_array_obj); // (vao start 1)
	    GLuint handle_element_buffer_obj;
        glGenBuffers(1, &handle_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw handle_outline
	/*if(outline) {
	// Draw scrollbar outline
    shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(handle_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}*/	
	// Draw handle
    shader.set_float("color", (handle_color.x/ 255), (handle_color.y/ 255), (handle_color.z / 255), (handle_color.w / 255)); //  foreground_color	
    glBindVertexArray(handle_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// Clean textures
	// Clean buffers
	// :scrollbar
	glDeleteBuffers(1, &scrollbar_tex_coord_buffer_obj);
	glDeleteBuffers(1, &scrollbar_element_buffer_obj  );
	glDeleteBuffers(1, &scrollbar_vertex_buffer_obj   );
	// :handle
	glDeleteBuffers(1, &handle_tex_coord_buffer_obj);
	glDeleteBuffers(1, &handle_element_buffer_obj  );
	glDeleteBuffers(1, &handle_vertex_buffer_obj   );
	// : top_button
	glDeleteBuffers(1, &top_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &top_button_element_buffer_obj  );
	glDeleteBuffers(1, &top_button_vertex_buffer_obj   );	
	// : bottom_button
	glDeleteBuffers(1, &bottom_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &bottom_button_element_buffer_obj  );
	glDeleteBuffers(1, &bottom_button_vertex_buffer_obj   );
    // : top_arrow
    // : bottom_arrow	
	// Clean arrays
	// :scrollbar
	glDeleteVertexArrays(1, &scrollbar_vertex_array_obj);
	// :handle
	glDeleteVertexArrays(1, &handle_vertex_array_obj   );
	// : top_button [<]
	glDeleteVertexArrays(1, &top_button_vertex_array_obj);
	// : bottom_button [>]
	glDeleteVertexArrays(1, &bottom_button_vertex_array_obj);
    // : top_arrow
    // : bottom_arrow	
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader.disable();
	shader.destroy();
#endif
}
////////////
////////////
////////////
////////////
/*
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	// _
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
	//shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);		
	// Draw _
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable program
	shader.disable();
	shader.destroy();
#endif		*/
////////////
////////////
////////////
////////////
////////////
void Renderer::draw_spinner(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
    double value,		
	// button
    int button_width, const Vector4& button_color, 
	// border
	bool outline,
	double outline_width,
	const Vector4& outline_color,
	bool outline_antialiased	
)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	// vertex shader	
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif	
	//---------------------------------
	GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0, 
    };	
	GLuint indices[] = {0, 1, 3,  1, 2, 3,}; 
	// -------------------------------
	// SPINNER
    // vertex array obj  - stores vertices
    GLuint spinner_vertex_array_obj;
    glGenVertexArrays(1, &spinner_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(spinner_vertex_array_obj); // bind again  
	    GLuint spinner_tex_coord_buffer_obj;
        glGenBuffers( 1, &spinner_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, spinner_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(spinner_vertex_array_obj); // bind vertex array obj
        GLuint spinner_vertex_buffer_obj;
        glGenBuffers(1, &spinner_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, spinner_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(spinner_vertex_array_obj); // (vao start 1)
	    GLuint spinner_element_buffer_obj;
        glGenBuffers(1, &spinner_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spinner_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(spinner_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Draw _
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(spinner_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// ------------------------------
	// TOP_BUTTON
    // vertex array obj  - stores vertices
    GLuint top_button_vertex_array_obj;
    glGenVertexArrays(1, &top_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind again  
	    GLuint top_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &top_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, top_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(top_button_vertex_array_obj); // bind vertex array obj
        GLuint top_button_vertex_buffer_obj;
        glGenBuffers(1, &top_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, top_button_vertex_buffer_obj);
		float top_button_x = width;
		float top_button_y = 0;
		float top_button_width  = button_width;
		float top_button_height = (height / 2);
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + top_button_x)                                           , static_cast<GLfloat>(y + top_button_y),
            static_cast<GLfloat>(x + top_button_x) + static_cast<GLfloat>(top_button_width), static_cast<GLfloat>(y + top_button_y),
            static_cast<GLfloat>(x + top_button_x) + static_cast<GLfloat>(top_button_width), static_cast<GLfloat>(y + top_button_y) + static_cast<GLfloat>(top_button_height),
            static_cast<GLfloat>(x + top_button_x)                                           , static_cast<GLfloat>(y + top_button_y) + static_cast<GLfloat>(top_button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(top_button_vertex_array_obj); // (vao start 1)
	    GLuint top_button_element_buffer_obj;
        glGenBuffers(1, &top_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, top_button_element_buffer_obj); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
 	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(top_button_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); 		
	// Draw _
	shader.set_float("color", (button_color.x / 255), (button_color.y / 255), (button_color.z / 255), (button_color.w / 255));
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(top_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	// ------------------------------
	// BOTTOM_BUTTON
    // vertex array obj  - stores vertices
    GLuint bottom_button_vertex_array_obj;
    glGenVertexArrays(1, &bottom_button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind again  
	    GLuint bottom_button_tex_coord_buffer_obj;
        glGenBuffers( 1, &bottom_button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, bottom_button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(bottom_button_vertex_array_obj); // bind vertex array obj
        GLuint bottom_button_vertex_buffer_obj;
        glGenBuffers(1, &bottom_button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, bottom_button_vertex_buffer_obj);
		float bottom_button_x = width;
		float bottom_button_y = (height / 2);
		float bottom_button_width  = button_width;
		float bottom_button_height = (height / 2);
		//std::cout << Vector4(x+bottom_button_x, y+bottom_button_y, bottom_button_width, bottom_button_height) << " in Renderer button_rect\n";
		GLfloat vertices2[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + bottom_button_x)                                              , static_cast<GLfloat>(y + bottom_button_y),
            static_cast<GLfloat>(x + bottom_button_x) + static_cast<GLfloat>(bottom_button_width), static_cast<GLfloat>(y + bottom_button_y),
            static_cast<GLfloat>(x + bottom_button_x) + static_cast<GLfloat>(bottom_button_width), static_cast<GLfloat>(y + bottom_button_y) + static_cast<GLfloat>(bottom_button_height),
            static_cast<GLfloat>(x + bottom_button_x)                                              , static_cast<GLfloat>(y + bottom_button_y) + static_cast<GLfloat>(bottom_button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 1)
	    GLuint bottom_button_element_buffer_obj;
        glGenBuffers(1, &bottom_button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottom_button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
 	shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(bottom_button_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);	
	// Draw _
	shader.set_float("color", (button_color.x / 255), (button_color.y / 255), (button_color.z / 255), (button_color.w / 255));
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(bottom_button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    // TOP_TRIANGLE
	// BUTTOM_TRIANGLE
	
	// Clean buffers
	glDeleteBuffers(1, &spinner_tex_coord_buffer_obj);
	glDeleteBuffers(1, &spinner_element_buffer_obj);
	glDeleteBuffers(1, &spinner_vertex_buffer_obj );
	// : top_
	glDeleteBuffers(1, &top_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &top_button_element_buffer_obj);
	glDeleteBuffers(1, &top_button_vertex_buffer_obj );
    // bottom_
	glDeleteBuffers(1, &bottom_button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &bottom_button_element_buffer_obj);
	glDeleteBuffers(1, &bottom_button_vertex_buffer_obj );	
	// Clean arrays
	glDeleteVertexArrays(1, &spinner_vertex_array_obj);
	glDeleteVertexArrays(1, &top_button_vertex_array_obj);
	glDeleteVertexArrays(1, &bottom_button_vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader.disable();
	shader.destroy();
#endif
}
////////////
////////////
////////////
////////////
////////////
void Renderer::draw_combobox(int x, int y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha,
    const Vector4& button_color, int button_width)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// vertex shader
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "uniform sampler2D arrow;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
		
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif		
	///////////////////////
	///////////////////////
	// COMBOBOX
    // vertex array obj  - stores vertices
    GLuint combo_vertex_array_obj;
    glGenVertexArrays(1, &combo_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(combo_vertex_array_obj); // bind again  
	    GLuint combo_tex_coord_buffer_obj;
        glGenBuffers( 1, &combo_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, combo_tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(combo_vertex_array_obj); // bind vertex array obj
        GLuint combo_vertex_buffer_obj;
        glGenBuffers(1, &combo_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, combo_vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(combo_vertex_array_obj); // (vao start 1)
	    GLuint combo_element_buffer_obj;
        glGenBuffers(1, &combo_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, combo_element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
	/*shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(combo_vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/		
	// Draw combobox
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(combo_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//----------------------------------
	// BUTTON
    // vertex array obj  - stores vertices
    GLuint button_vertex_array_obj;
    glGenVertexArrays(1, &button_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind again  
	    GLuint button_tex_coord_buffer_obj;
        glGenBuffers( 1, &button_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, button_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(button_vertex_array_obj); // bind vertex array obj
        GLuint button_vertex_buffer_obj;
        glGenBuffers(1, &button_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, button_vertex_buffer_obj);
		// set position relative to combobox
		float button_x = width;
		float button_y = 0;
		int button_height = height;
		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + button_x)                                       , static_cast<GLfloat>(y + button_y),
            static_cast<GLfloat>(x + button_x) + static_cast<GLfloat>(button_width), static_cast<GLfloat>(y + button_y),
            static_cast<GLfloat>(x + button_x) + static_cast<GLfloat>(button_width), static_cast<GLfloat>(y + button_y) + static_cast<GLfloat>(button_height),
            static_cast<GLfloat>(x + button_x)                                       , static_cast<GLfloat>(y + button_y) + static_cast<GLfloat>(button_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(button_vertex_array_obj); // (vao start 1)
	    GLuint button_element_buffer_obj;
        glGenBuffers(1, &button_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
	/*shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/		
	// Draw button
	shader.set_float("color", (button_color.x / 255), (button_color.y / 255), (button_color.z / 255), (button_color.w / 255));//shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(button_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
	//////////////////////
    // TRIANGLE
	/*
    // vertex array obj  - stores vertices
    GLuint arrow_vertex_array_obj;
    glGenVertexArrays(1, &arrow_vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(arrow_vertex_array_obj); // bind again  
	    GLuint arrow_tex_coord_buffer_obj;
        glGenBuffers( 1, &arrow_tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, arrow_tex_coord_buffer_obj);
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(arrow_vertex_array_obj); // bind vertex array obj
        GLuint arrow_vertex_buffer_obj;
        glGenBuffers(1, &arrow_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, arrow_vertex_buffer_obj);
		// set position relative to combobox
		float arrow_x = -width + 100;
		float arrow_y = 0;
		int arrow_width  = 16;
		int arrow_height = 16;
		GLfloat vertices2[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + arrow_x)                                       , static_cast<GLfloat>(y + arrow_y),
            static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(arrow_width), static_cast<GLfloat>(y + arrow_y),
            static_cast<GLfloat>(x + arrow_x) + static_cast<GLfloat>(arrow_width), static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height),
            static_cast<GLfloat>(x + arrow_x)                                       , static_cast<GLfloat>(y + arrow_y) + static_cast<GLfloat>(arrow_height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2)* sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(arrow_vertex_array_obj); // (vao start 1)
	    GLuint arrow_element_buffer_obj;
        glGenBuffers(1, &arrow_element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrow_element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0); */               // (vao end 1  )
    // Draw outline
	/*shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
shader.disable();
program.use();	
	glm::mat4 arrow_model = glm::mat4(1.0);;
	arrow_model = glm::translate(arrow_model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	arrow_model = glm::rotate(arrow_model, static_cast<float>(45.0f * 0.0174533), glm::vec3(0, 0, 1));
	arrow_model = glm::scale(arrow_model, glm::vec3(1, 1, 1));
	arrow_model = glm::translate(arrow_model, glm::vec3(-x - width/2, -y - height/2, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(arrow_model));	
	// Draw arrow
	shader.set_float("color", (255.0 / 255), (255.0 / 255), (255.0 / 255), (255.0 / 255));
	//shader.set_float("color", (button_color.x / 255), (button_color.y / 255), (button_color.z / 255), (button_color.w / 255));//shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    glBindVertexArray(arrow_vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )	
	*/
	//////////////////////
	// Clean buffers
	glDeleteBuffers(1, &combo_tex_coord_buffer_obj);
	glDeleteBuffers(1, &combo_element_buffer_obj  );
	glDeleteBuffers(1, &combo_vertex_buffer_obj   );
	// : button
	glDeleteBuffers(1, &button_tex_coord_buffer_obj);
	glDeleteBuffers(1, &button_element_buffer_obj  );
	glDeleteBuffers(1, &button_vertex_buffer_obj   );	
	// : triangle
	//glDeleteBuffers(1, &arrow_tex_coord_buffer_obj);
	//glDeleteBuffers(1, &arrow_element_buffer_obj  );
	//glDeleteBuffers(1, &arrow_vertex_buffer_obj   );
	// Clean arrays
	glDeleteVertexArrays(1, &combo_vertex_array_obj );
	glDeleteVertexArrays(1, &button_vertex_array_obj);
	//glDeleteVertexArrays(1, &arrow_vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0);
	glDisable(GL_LINE_SMOOTH);
	// Disable program
	shader.disable();
	shader.destroy();
	//////////////////
	//draw_triangle(x, y, 32, 32, -45.0f * 0.0174533, 1, 1, 255, 255, 255);
#endif
}
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
void Renderer::draw_point()
{}
void Renderer::draw_line(double x, double y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
    //if(!text.empty()) draw_label(text,);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
    //-----------------------------------
    // CURSOR
    // vertex array obj  - stores vertices
    GLuint cursor_vertex_array_obj;
    glGenVertexArrays(1, &cursor_vertex_array_obj);	
    // vertex buffer obj
    glBindVertexArray(cursor_vertex_array_obj); // bind vertex array obj
        GLuint cursor_vertex_buffer_obj;
        glGenBuffers(1, &cursor_vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, cursor_vertex_buffer_obj);

		GLfloat vertices1[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x) + static_cast<float>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<float>(width), static_cast<GLfloat>(y) + static_cast<float>(height),
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1)* sizeof(GLfloat), vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)	
	// State
	glDisable(GL_DEPTH_TEST);                         // Disable 3d for User interdata
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Enable transparent background
	glEnable(GL_BLEND);
	// Shader
	Shader shader;
	shader.create();
	shader.set_source(vertex_source  , 0);
	shader.set_source(fragment_source, 1);
	shader.prepare();
	shader.use ();
#ifdef use_glm	
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle * 0.0174533), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
		
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
#endif		
	// Draw cursor
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    //glEnable(GL_LINE_SMOOTH);
	glLineWidth(width);
	glBindVertexArray(cursor_vertex_array_obj);
        glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);  
    shader.disable();
    // Clean
	glDeleteVertexArrays(1, &cursor_vertex_array_obj);
#endif	
}
void Renderer::draw_circle()
{/*
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Enable point size mode
	glEnable(GL_PROGRAM_POINT_SIZE);
	// Set polygon mode 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe 
	//=glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  // points (that you can barely see)
	int width = 100; int height = 100; double x = 0.0; double y = 0.0; double angle = 15*(double)(clock()/CLOCKS_PER_SEC);double scale_x = 1.0; double scale_y = 1.0;
	int red = 255; int green = 255; int blue = 100; int alpha = 255;
	// vertex shader	
	Shader vertex_shader;
	vertex_shader.create(DOKUN_SHADER_VERTEX);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	vertex_shader.set_source(vertex_source);
	vertex_shader.compile();
	// fragment shader
	Shader fragment_shader;
	fragment_shader.create(DOKUN_SHADER_FRAGMENT);
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
    const char * circle_fragment_source[] =
	{
		"\n"
		"out vec4 out_color;\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
			"vec2 pos = mod(out_color.xy, vec2(50.0)) - vec2(25.0);"
			"float dist_squared = dot(pos, pos);"
			"out_color = (dist_squared < 400.0)\n"
			    "? vec4(.90, .90, .90, 1.0)\n"
			    ": vec4(.20, .20, .40, 1.0);"
        "}\n"		
	};	
	fragment_shader.set_source(fragment_source);
	fragment_shader.compile();
	// program
	Shader program;
	program.create();
    program.attach(vertex_shader  );
	program.attach(fragment_shader);
	program.link();
	// destroy shaders after linking
	program.detach(vertex_shader  );
	program.detach(fragment_shader);
	vertex_shader.destroy  ();
	fragment_shader.destroy();
	program.use ();
	// uniform
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// tex_coord buffer obj
    glBindVertexArray(vertex_array_obj); // bind again  
	    GLuint tex_coord_buffer_obj;
        glGenBuffers( 1, &tex_coord_buffer_obj);
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_buffer_obj);
		GLfloat tex_coords[] = { // texture coordinates range from (0,0) to (1, 1)
		    0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0, 
        };
        glBufferData( GL_ARRAY_BUFFER, sizeof(tex_coords)* sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);    
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), static_cast<void *>(0));			
	    glEnableVertexAttribArray(1); 	
	glBindVertexArray(0); // unbind
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		
	    float radius = 50.0;
		float circle_points = 100.0;
	    float angle_rad = 2.0 * 3.14159265359 * angle / 180.f;
	    float x_rad = x + cos(angle) * radius;
	    float y_rad = y + sin(angle) * radius;
		for(int i = 0; i < circle_points; i++)
		{
			float theta = 2 * 3.14159265359 * (float)(i) / 100;
			
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x + cos(theta) * radius)                                , static_cast<GLfloat>(y + sin(theta) * radius),
            static_cast<GLfloat>(x + cos(theta) * radius) + static_cast<GLfloat>(width), static_cast<GLfloat>(y + sin(theta) * radius),
            static_cast<GLfloat>(x + cos(theta) * radius) + static_cast<GLfloat>(width), static_cast<GLfloat>(y + sin(theta) * radius) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x + cos(theta) * radius)                                , static_cast<GLfloat>(y + sin(theta) * radius) + static_cast<GLfloat>(height),   
        };
		/
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
		    static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };/      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 2, 2 * circle_points * sizeof(float), vertices);
		}
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {0, 1, 3,  1, 2, 3,};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
	// Draw
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLE_FAN, 100, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &tex_coord_buffer_obj);
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	// Restore defaults
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable program
	shader.disable();
	shader.destroy();
#endif			*/
}
////////////
void Renderer::draw_triangle(double x, double y, int width, int height, double angle, double scale_x, double scale_y, double red, double green, double blue, double alpha)
{/*
#ifdef DOKUN_OPENGL	// OpenGL is defined
	context_check();
    // Disable 3d for User interdata
	glDisable(GL_DEPTH_TEST);
	// Enable transparent background
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// vertex shader	
	Shader vertex_shader;
	vertex_shader.create(DOKUN_SHADER_VERTEX);
	const char * vertex_source[] =
	{
        "#version 330\n"
        "\n"
        "layout (location = 0) in vec2 position ;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
		"out vec2 Texcoord;\n"
		"\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 proj ;\n"
		"\n"
        "void main()\n"
        "{\n"
		    "Texcoord    = tex_coord;\n"
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n"
        "}\n"
	};
	vertex_shader.set_source(vertex_source);
	vertex_shader.compile();
	// fragment shader
	Shader fragment_shader;
	fragment_shader.create(DOKUN_SHADER_FRAGMENT);
	const char * fragment_source[] =
	{
	    "#version 330\n"
        "\n"
		"out vec4 out_color;\n"
        "uniform vec4 color;\n"
        "//uniform sampler2D base;\n"
		"in vec2 Texcoord;\n"
		"\n"
		"\n"
        "void main()\n"
        "{\n"
		"\n"
		"\n"
		"\n"
            "out_color = color;\n"
        "}\n"
	};
	fragment_shader.set_source(fragment_source);
	fragment_shader.compile();
	// program
	Shader program;
	program.create();
    program.attach(vertex_shader  );
	program.attach(fragment_shader);
	program.link();
	// destroy shaders after linking
	program.detach(vertex_shader  );
	program.detach(fragment_shader);
	vertex_shader.destroy  ();
	fragment_shader.destroy();
	program.use ();
	// uniform
	glm::mat4 model = glm::mat4(1.0);;
	model = glm::translate(model, glm::vec3(x + width/2, y + height/2, 0));//model = glm::translate(model, glm::vec3(x, y, 0));
	model = glm::rotate(model, static_cast<float>(angle), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
	model = glm::translate(model, glm::vec3(-x - width/2, -y - height/2, 0));
	float window_width  = Renderer::get_display_width ();
	float window_height = Renderer::get_display_height();
	glm::mat4 proj  = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.get_program(), "proj") , 1, GL_FALSE, glm::value_ptr(proj) );
	// _
    // vertex array obj  - stores vertices
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
	// vertex buffer obj
    glBindVertexArray(vertex_array_obj); // bind vertex array obj
        GLuint vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
		GLfloat vertices[] = { // when x and y are 0 then from wwidth-wheight
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y),
            static_cast<GLfloat>(x) + static_cast<GLfloat>(width), static_cast<GLfloat>(y) + static_cast<GLfloat>(height),
            static_cast<GLfloat>(x)                                , static_cast<GLfloat>(y) + static_cast<GLfloat>(height),   
        };      
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)* sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0); // vertex array obj (end 0)   
	// element buffer obj
	glBindVertexArray(vertex_array_obj); // (vao start 1)
	    GLuint element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
		GLuint indices[] = {1, 2, 3,  0};  
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)* sizeof(GLuint), indices, GL_STATIC_DRAW); 
	glBindVertexArray(0);                // (vao end 1  )
    // Draw outline
	//shader.set_float("color", (outline_color.x / 255), (outline_color.y / 255), (outline_color.z / 255), (outline_color.w / 255));
	//glEnable(GL_LINE_SMOOTH); // may slow down performance
	/
	glLineWidth(outline_width); // outline_width
	glBindVertexArray(vertex_array_obj); // use same vao data as _ but this time in a line loop
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);/		
	// Draw _
	shader.set_float("color", (red / 255), (green / 255), (blue / 255), (alpha / 255));
	//glBindTexture(GL_TEXTURE_2D, base);  // bind texture
    glBindVertexArray(vertex_array_obj); // (vao start 2)
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);                // (vao end 2  )
    //glBindTexture(GL_TEXTURE_2D, base);  // bind texture
	// Clean textures
	//glDeleteTextures(1, &base);
	// Clean buffers
	glDeleteBuffers(1, &element_buffer_obj);
	glDeleteBuffers(1, &vertex_buffer_obj );
	// Clean arrays
	glDeleteVertexArrays(1, &vertex_array_obj);
	// Restore defaults
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable program
	shader.disable();
	shader.destroy();
#endif	*/
}
////////////
void Renderer::draw_quad()
{}
////////////	
void Renderer::draw_cube()
{}
void Renderer::draw_sphere()
{}
void Renderer::draw_torus()
{}
void Renderer::draw_pyramid()
{}
void Renderer::draw_teapot()
{}
////////////
////////////
void Renderer::set_viewport(int x, int y, int width, int height)
{
#ifdef DOKUN_OPENGL
    if(height == 0) 
		height = 1;
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
#endif
#ifdef DOKUN_VULKAN
	VkViewport viewport; // up to 10 viewports
	memset(&viewport, 0, sizeof(viewport));
	viewport.x        =	0;
	viewport.y        =	0;
	viewport.width    = static_cast<float>(width);
	viewport.height   = static_cast<float>(height);
	viewport.minDepth = static_cast<float>(0.0);
	viewport.maxDepth = static_cast<float>(1.0);
	//vkCmdSetViewport(cmd_buf, 0, 1, &viewport);
#endif	
}
////////////
void Renderer::set_camera(const Camera& camera)
{
	Renderer::camera = &const_cast<Camera&>(camera);
}
////////////
void Renderer::set_light(const Light& light)
{
	Renderer::light = &const_cast<Light&>(light);
}
////////////
////////////
void Renderer::set_clear(double red, double green, double blue, double alpha, double depth, int stencil)
{
	color = Vector4(red, green, blue, alpha);
#ifdef DOKUN_OPENGL
	context_check();
	glClearColor(static_cast<GLclampf>(color.x) / 255, static_cast<GLclampf>(color.y) / 255, static_cast<GLclampf>(color.z) / 255, static_cast<GLclampf>(color.w) / 255); 
    glClearDepth(depth); 
    glClearStencil(stencil);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif
#ifdef DOKUN_VULKAN
	if(Renderer::get_current_API() == "Vulkan")
	{
        /*
		VkClearValue clear_value;
        clear_value.color.float32[0] = static_cast<float>(red);
        clear_value.color.float32[1] = static_cast<float>(green);
        clear_value.color.float32[2] = static_cast<float>(blue);
        clear_value.color.float32[3] = static_cast<float>(alpha); // 1.0f;
		*/
	}
#endif	
}
////////////
void Renderer::set_vertical_synchronization(bool v_sync)
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	wglSwapIntervalEXT(v_sync);
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
	//glXSwapIntervalEXT(display, window, v_sync); // 
#endif
#endif
#endif	
}
////////////
void Renderer::set_display_size(int width, int height) // renderable area (use window_client_size rather than the usual window_size)
{
	Renderer::window_width  = width;
	Renderer::window_height = height;
}
////////////
void Renderer::set_current_API(const std::string& API)
{
	if(dokun::lower(API) == "opengl")
	{
		Renderer::API = "OpenGL";
	}
    if(dokun::lower(API) == "vulkan")
    {
		Renderer::API = "Vulkan";
	}
}
////////////
int Renderer::set_current_API(lua_State *L)
{
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		Renderer::set_current_API(lua_tostring(L, -1));
	}
}
////////////
int Renderer::get_display_width () // returns the width of display area (client area), excluding the titlebar
{
	int window_width;
#ifdef __windows__
	RECT rect;
    ::GetClientRect(::GetActiveWindow(), &rect);
    window_width  = rect.right - rect.left;
#endif
// NOTE: for Linux, the display size is automatically updated in the "Window::update" function
	return window_width;
}
int Renderer::get_display_height() // returns the height of display area (client area), excluding the titlebar
{
	int window_height;
#ifdef __windows__
		RECT rect;
        ::GetClientRect(::GetActiveWindow(), &rect);
        window_height = rect.bottom - rect.top;
#endif
// NOTE: for Linux, the display size is automatically updated in the "Window::update" function
	return window_height;
}
Vector2i Renderer::get_display_size() // returns the size of display area (client area), excluding the titlebar
{
	return Vector2i(get_display_width(), get_display_height());
}
////////////
Camera * Renderer::get_camera()
{
	return camera;
}
////////////
Light  * Renderer::get_light ()
{
	return light;
}
////////////
Renderer * Renderer::get_pointer()
{
	return render_ptr;
}

bool Renderer::get_status()
{
	return status;
}

#ifdef DOKUN_VULKAN
    VkInstance Renderer::get_instance()
    {
	    return get_pointer()->instance;
    }
#endif
//////////////////////
////////////
	// api
std::string Renderer::get_current_API(void)
{
#ifdef DOKUN_OPENGL
    if(String::lower(API) == "opengl") {
	    return "OpenGL";
	}
#endif
#ifdef DOKUN_VULKAN
    if(String::lower(API) == "vulkan") {
	    return "Vulkan";
	}    
#endif
}
////////////
std::string Renderer::get_current_API_version(void)
{
    int major, minor, patch;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    glGetIntegerv(GL_MAJOR_VERSION, & major);         
        glGetIntegerv(GL_MINOR_VERSION, & minor);
	    std::pair<int, int> version = std::make_pair (major, minor);		
	    return std::to_string(version.first) + "." + std::to_string(version.second) + ".0";
    }	
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    major = VK_VERSION_MAJOR(Renderer::get_pointer()->device_prop.apiVersion);
	    minor = VK_VERSION_MINOR(Renderer::get_pointer()->device_prop.apiVersion);
	    patch = VK_VERSION_PATCH(Renderer::get_pointer()->device_prop.apiVersion);
	    return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    }	
#endif	
}
////////////
int Renderer::get_current_API_version_major(void)
{
    int major;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    glGetIntegerv(GL_MAJOR_VERSION, &major);
        return major;	
    }    
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    major = VK_VERSION_MAJOR(Renderer::get_pointer()->device_prop.apiVersion);
	    return major;
    }	
#endif
    return 1;
}
////////////
int Renderer::get_current_API_version_minor(void)
{
    int minor;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    glGetIntegerv(GL_MINOR_VERSION, &minor);
	    return minor;
    }
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    minor = VK_VERSION_MINOR(Renderer::get_pointer()->device_prop.apiVersion);
	    return minor;
    }
#endif
    return 0;
}
////////////
int Renderer::get_current_API_version_patch(void)
{
    int patch;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check ();
	    patch = VK_VERSION_PATCH(Renderer::get_pointer()->device_prop.apiVersion);
	    return patch;
	}
#endif
    return 0;
}
////////////
	// shader
std::string Renderer::get_shader_language(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") return "GLSL";
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") return "SPIR-V";
#endif
}
////////////
std::string Renderer::get_shader_language_version(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
    }
#endif
}
////////////
int Renderer::get_shader_language_version_major(void)
{
    int major;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
        context_check();
	    std::string shader_version = reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	    std::vector<std::string> major_minor = String::split(shader_version, ".");
	    std::string major_ = major_minor[0];    
	    major = static_cast<int>(String::extract(major_));
	    return major;
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {}
#endif
}
////////////
int Renderer::get_shader_language_version_minor(void)
{
    int minor;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    std::string shader_version = reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	    std::vector<std::string> major_minor = String::split(shader_version, ".");
	    std::string minor_ = major_minor[1];
	    if(minor_.length() > 1) 
		    minor_ = minor_.at( minor_.find_first_of(minor_));
	    minor = static_cast<int>(String::extract(minor_));
	    return minor;
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {}
#endif
}
////////////
int Renderer::get_shader_language_version_patch(void)
{
    int patch;
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    std::string shader_version = reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	    std::vector<std::string> major_minor = String::split(shader_version, ".");
	    std::string patch_ = major_minor[1];
	    patch_ = patch_.at(patch_.find_last_of(patch_));
	    patch = static_cast<int>(String::extract(patch_));
	    return patch;
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {}
#endif
}
////////////
	// gpu
std::string Renderer::get_gpu(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_RENDERER)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check();
	    return Renderer::get_pointer()->device_prop.deviceName;
	}
#endif
}
////////////
std::string Renderer::get_gpu_vendor(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_VENDOR)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check();
        //return Renderer::get_pointer()->device_prop.vendorID; // uint32_t??
    }
#endif
}
////////////
std::string Renderer::get_gpu_version(void)
{
#ifdef DOKUN_OPENGL
    if(get_current_API() == "OpenGL") {
	    context_check();
	    return reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(GL_VERSION)));
	}
#endif
#ifdef DOKUN_VULKAN
    if(get_current_API() == "Vulkan") {
        device_check();
    }
#endif
}
////////////
void Renderer::print_vulkan_info()
{
#ifdef DOKUN_VULKAN
	if(get_current_API() == "Vulkan")
	{
		// look for devices and gpus that support vulkan
        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, & extension_count, nullptr);

        std::cout << extension_count << " extensions supported" << std::endl;	
		
		VkDevice device;
		VkPhysicalDevice physical_device;
		uint32_t physical_device_count = 0;
		std::vector<VkPhysicalDevice> physical_device_list( physical_device_count);
		// check how many devices we have
		VkResult result = vkEnumeratePhysicalDevices(get_pointer()->instance, & physical_device_count, & physical_device);
		assert(result == VK_SUCCESS);
		std::cout << physical_device_count << " devices found" << std::endl;
	    // device properties
		VkPhysicalDeviceProperties device_prop = {};
		vkGetPhysicalDeviceProperties(physical_device, & device_prop);   // for single device

		// API information
		std::string API_name = "Vulkan";
		int major = VK_VERSION_MAJOR(device_prop.apiVersion);
		int minor = VK_VERSION_MINOR(device_prop.apiVersion);
		int patch = VK_VERSION_PATCH(device_prop.apiVersion);
		
		std::string shader_lang = "GLSL";
		std::string shader_version = "??";
		// device information
			std::cout << "GPU: " << device_prop.deviceName << std::endl; 
			std::cout << "Vendor: " << device_prop.vendorID << std::endl; 
			std::cout << "Device : " << device_prop.deviceID << std::endl;
            std::cout << "Type : " << device_prop.deviceType << std::endl;
			std::cout << "Driver version: " << device_prop.driverVersion << std::endl; 
			std::cout << "API: " << API_name << std::endl; 
			std::cout << "API version ID : " << device_prop.apiVersion << std::endl;
			std::cout << "API Version: " << major << "." << minor << "." << patch << std::endl; 
			std::cout << "Shader: " << shader_lang << std::endl;
            std::cout << "Shader Version: " << shader_version << std::endl;				
	}
#endif
}	
////////////
////////////
