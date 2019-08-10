#include "../include/model.h"

//////////// 
Model::Model(void) : x (0), y (0), z (-1), rotation (0, 0, 0), scale_factor (1, 1, 1), origin(0, 0, 0) // if position.z is 0 the model will be way too close to the user's face so its better take a step foward by 1    
{
    material.ambient   = Vector4(0.2, 0.2, 0.2, 1.0);
	material.diffuse   = Vector4(0.8, 0.8, 0.8, 1.0);
	material.specular  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.emission  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.shininess = 0.0;
	Factory::get_model_factory()->store(this);
#ifdef DOKUN_DEBUG    
    Logger::push(DOKUN_LOGTAG "Model " + String(this).str() + " allocated (index=" + String(Factory::get_model_factory()->get_location(this)).str() + ")" + " (total_model_instances=" + String(Factory::get_model_factory()->get_size()).str() + ")");
#endif
}
////////////
Model::Model(const Model& model) // not complete !
{
	// set geometry
	set_position(model.get_position());
	set_rotation(model.get_rotation().x, 1, 0, 0);
	set_rotation(model.get_rotation().y, 0, 1, 0);
	set_rotation(model.get_rotation().z, 0, 0, 1);
	set_scale(model.get_scale());
	set_origin(model.get_origin());
	// set material color
	set_material("ambient"  , model.get_material("ambient") );
	set_material("diffuse"  , model.get_material("diffuse") );
	set_material("specular" , model.get_material("specular"));
	set_material("emission" , model.get_material("emission"));
	set_material("shininess", model.get_shininess());
    // get vertices, textures, etc.
	vertices.vertex = model.get_vertex_array();
	vertices.uv     = model.get_uv_array();
	vertices.normal = model.get_normal_array();
	elements.vertex = model.get_element_array(0);
	elements.uv     = model.get_element_array(1);
	elements.normal = model.get_element_array(2);
	texture_list    = model.get_texture_array();
	Factory::get_model_factory()->store(this);
#ifdef DOKUN_DEBUG    
    Logger::push(DOKUN_LOGTAG "Model " + String(this).str() + " allocated (index=" + String(Factory::get_model_factory()->get_location(this)).str() + ")" + " (total_model_instances=" + String(Factory::get_model_factory()->get_size()).str() + ")");
#endif	
}
////////////
Model::Model(const std::string& file_name) : x (0), y (0), z (-1), rotation (0, 0, 0), scale_factor (1, 1, 1), origin(0, 0, 0)
{
    material.ambient   = Vector4(0.2, 0.2, 0.2, 1.0);
	material.diffuse   = Vector4(0.8, 0.8, 0.8, 1.0);
	material.specular  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.emission  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.shininess = 0.0;
    // load model (from file)
	if(!load(file_name))
		Logger("Could not open " + file_name); 
	Factory::get_model_factory()->store(this);
#ifdef DOKUN_DEBUG    
    Logger::push(DOKUN_LOGTAG "Model " + String(this).str() + " allocated (index=" + String(Factory::get_model_factory()->get_location(this)).str() + ")" + " (total_model_instances=" + String(Factory::get_model_factory()->get_size()).str() + ")");
#endif	
} 
////////////
Model::Model(const std::string& file_name, const Texture& texture) : x (0), y (0), z (-1), rotation (0, 0, 0), scale_factor (1, 1, 1), origin(0, 0, 0)
{
    material.ambient   = Vector4(0.2, 0.2, 0.2, 1.0);
	material.diffuse   = Vector4(0.8, 0.8, 0.8, 1.0);
	material.specular  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.emission  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.shininess = 0.0;	
	// load model (from file)
	if(!load(file_name))
		Logger("Could not open " + file_name);
	// set texture
    set_texture(texture);	
	Factory::get_model_factory()->store(this);
#ifdef DOKUN_DEBUG    
    Logger::push(DOKUN_LOGTAG "Model " + String(this).str() + " allocated (index=" + String(Factory::get_model_factory()->get_location(this)).str() + ")" + " (total_model_instances=" + String(Factory::get_model_factory()->get_size()).str() + ")");
#endif	
}
////////////
Model::Model(const std::vector<Vector3>& vertex_array, 
	const std::vector<unsigned int>& element_array)
{
	// set material
    material.ambient   = Vector4(0.2, 0.2, 0.2, 1.0);
	material.diffuse   = Vector4(0.8, 0.8, 0.8, 1.0);
	material.specular  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.emission  = Vector4(0.0, 0.0, 0.0, 1.0);
	material.shininess = 0.0;		
	// load model (from std::vector)
	if(!load(vertex_array, element_array))
		Logger("Could not load array");
	Factory::get_model_factory()->store(this);
#ifdef DOKUN_DEBUG    
    Logger::push(DOKUN_LOGTAG "Model " + String(this).str() + " allocated (index=" + String(Factory::get_model_factory()->get_location(this)).str() + ")" + " (total_model_instances=" + String(Factory::get_model_factory()->get_size()).str() + ")");
#endif	
}			
////////////
Model::~Model(void)
{
	Factory::get_model_factory()->release(this);
#ifdef DOKUN_DEBUG 	
	Logger::push(DOKUN_LOGTAG + "Model " + String(this).str() + " deallocated (total_model_instances=" + String(Factory::get_model_factory()->get_size()).str() + ")");
#endif	  
}
////////////
Shader * Model::shader (new Shader());
////////////
int Model::create(lua_State *L) 
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING) 
	{
		file_name = lua_tostring(L, -1);
	}
	lua_settop(L, 0);
	lua_createtable(L, 0, 0);
	lua_getglobal(L, "Model");
	lua_setmetatable(L, 1);
	Model **model = static_cast<Model**>(lua_newuserdata(L, sizeof(Model*)));
	if(!file_name.empty()) {
		*model = new Model(file_name);
	}
	else {
	    *model = new Model;
	}
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, -1))
		return 1;
	lua_pushnil(L);
	return 1;
}
////////////
bool Model::load(const std::string& file_name)
{
	std::string extension = String::lower(file_name.substr(file_name.find_last_of(".") + 1));
	if(extension == "dae")
	{
		return load_dae(file_name);
	}
	return load_obj(file_name); // default
}
////////////
bool Model::load(const std::vector<Vector3>& vertex_array, const std::vector<unsigned int>& element_array)
{
	for(int i = 0; i < vertex_array.size(); i++) 
		vertices.vertex.push_back(vertex_array[i]);
    for(int j = 0; j < element_array.size(); j++) 
		elements.vertex.push_back(element_array[j]);
    return true;	
}
////////////
int Model::load(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checkany(L, 2);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		if(lua_type(L, 2) == LUA_TTABLE) // v
		{
		#ifdef DOKUN_LUA51
			for(int i = 1; i <= lua_objlen(L, 2); i += 3) // for every 3 number (lua tables start at index 1)
		#endif
		#ifndef DOKUN_LUA51
			for(int i = 1; i <= luaL_len(L, 2); i += 3) // for every 3 number (lua tables start at index 1)
		#endif
			{
			    lua_rawgeti (L, 2, i);      
				double x = lua_tonumber(L, -1);
				lua_rawgeti (L, 2, i + 1);  
				double y = lua_tonumber(L, -1);				
				lua_rawgeti (L, 2, i + 2);  
				double z = lua_tonumber(L, -1);
				model->vertices.vertex.push_back(Vector3(x, y, z)); // store value (in C++)
				lua_pop(L, 1); // pop x
				lua_pop(L, 1); // pop y
				lua_pop(L, 1); // pop z
			}
			if(lua_type(L, 3) == LUA_TTABLE) // f 0
			{
			#ifdef DOKUN_LUA51
				for(int i = 1; i <= lua_objlen(L, 3); i++)
			#endif
			#ifndef DOKUN_LUA51
				for(int i = 1; i <= luaL_len(L, 3); i++)
			#endif
				{
					lua_rawgeti (L, 3, i); // get element as it is
					model->elements.vertex.push_back(static_cast<int>(lua_tonumber(L, -1))); // store element (c++)
					lua_pop(L, 1); // pop element
				}
                // calculate normals
                model->vertices.normal.resize(model->vertices.vertex.size(), Vector3(0.0, 0.0, 0.0));
				for(unsigned int i = 0; i < model->elements.vertex.size(); i += 3)
	            {
		            unsigned int a, b, c;

		            a = model->elements.vertex[i];
		            b = model->elements.vertex[i + 1];
		            c = model->elements.vertex[i + 2];
					
					Vector3 a0 = Vector3(model->vertices.vertex[b] - model->vertices.vertex[a]);
					Vector3 b0 = Vector3(model->vertices.vertex[c] - model->vertices.vertex[a]);
					Vector3 c0 = Vector3::cross(a0, b0);
					Vector3 n0 = Vector3::normal(c0);
					model->vertices.normal[a] = model->vertices.normal[b] = model->vertices.normal[c] = n0;
	            }				
			}
			if(lua_type(L, 4) == LUA_TTABLE) // vt
			{}
			lua_pushboolean(L, true);
		    return 1;			
		}
		if(lua_type(L, 2) == LUA_TSTRING)
		{
			lua_pushboolean(L, model->load(lua_tostring(L, 2)));
		    return 1;
		}
	}
	lua_pushboolean(L, false);
	return 1;
}
////////////
void Model::generate()
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
	if(Renderer::get_current_API() != "OpenGL") // but it is not set as the current rendering API
	    return;
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Rendering Failed : No OpenGL Context found");
		return;
	}
#endif	
#ifdef __gnu_linux__
#ifdef DOKUN_X11
	if(!glXGetCurrentContext())
	{
		Logger("Rendering failed : No OpenGL Context found");
		return;
	}
#endif
#endif	
const GLchar * vertex_source[] = // uniforms can be changed (cannot be indexed), "out" is declared in vertex shader and passed to fragment shader as "in"
	    {
            "#version 330\n"
		    "layout(location = 0) in vec3 position ;\n"
	    	"layout(location = 1) in vec2 tex_coord;\n"	
	    	"layout(location = 2) in vec3 normal   ;\n"
	    	"                                              \n"
	    	"                                              \n"
	    	"                                              \n"
	    	"uniform mat4 proj;\n"
	    	"uniform mat4 view;\n"
	    	"uniform mat4 model;\n"
	    	"                                              \n"
	    	"                                              \n"
	    	"                                              \n"	
	    	"                                              \n"
	    	"out vec3 Normal  ;\n"
    		"out vec2 Texcoord;\n"
	    	"\n"
	    	"out vec3 frag_position;\n"
	    	"\n"
	    	"\n"
	    	"\n"
	    	"\n"
            "void main(void)                               \n"
            "{                                             \n"
                "\n"
                "Texcoord    = vec2(tex_coord.x, 1.0 - tex_coord.y);\n"
                "Normal      = normal   ;\n"
	    	    "gl_Position = proj * view * model * vec4(position, 1.0);\n"
            "}                                             \n"		
	    };
	    const GLchar * fragment_source[] = // holds all color
	    {
            "#version 330                                  \n"
	    	"                                              \n"
            "out vec4 out_color;                           \n"		
	    	"\n"
	    	"\n"
	    	"in vec3 Normal  ;\n"
	    	"in vec2 Texcoord;\n"
	    	"\n"
	    	"in vec3 frag_position      ;\n"
	    	"uniform vec3 view_position ;\n"		
	    	"\n"
	    	"\n"
	    	"//------------------------------\n"
			"struct Texture {\n"
			    "bool is_texture;        \n"
				"int count;              \n"
			    "sampler2D diffuse;      \n" // base_texture
			    "sampler2D specular;     \n"
			    "//sampler2D emission;   \n"
				"//sampler2D opacity;    \n"
				"//sampler2D bump;       \n"
				"//sampler2D normal;     \n"
				"//sampler2D glow;       \n"
				"//sampler2D reflection; \n"
				//"sampler2D [8];\n"
			"};\n"
	    	"//------------------------------\n"
	    	"struct Material {\n" // replacement for 'color'
	    	    "vec3 ambient;   \n"
	    	    "vec3 diffuse;   \n"
	    	    "vec3 specular;  \n"
	    		"vec3 emission;  \n"
	    	    "float shininess;\n"
				"Texture texture;\n" // from Texture struct
	    	"};\n"
	    	"uniform Material material;\n"
	    	"//-----------------------------\n"
	    	"struct Light {\n" // global light
	    	    "vec3 position;\n"
	    	    "vec3 ambient;\n"
	    	    "vec3 diffuse;\n"
	    	    "vec3 specular;\n"
				"vec3 spot_direction;\n"
				"float spot_exponent;\n"
				"float spot_cutoff;\n"
				"vec3 attenuation;\n"
	    	"};\n"
	    	"uniform Light light;\n"
	    	"//-----------------------------\n"
	    	"\n"
	    	"void main(void)                               \n"
            "{                                             \n"
	    	    "\n"
	    	    "vec3 view_direction  = normalize(view_position  - frag_position);\n"
	    	    "vec3 light_direction = normalize(light.position - frag_position);\n"
	    	    "\n"
	    	    "vec3 ambient, diffuse, specular;\n"
				"if(material.texture.is_texture == false)" // NO texture - light + material_color
				"{"
	    		    "ambient  = light.ambient  * material.ambient;\n"
	    		    "diffuse  = light.diffuse  * material.diffuse  * max(dot(normalize(Normal), light_direction), 0.0);\n" // base_color
	    	        "specular = light.specular * material.specular * pow(max(dot(view_direction, reflect(-light_direction, normalize(Normal))), 0.0), material.shininess);\n"
	    	    "}\n"				
				"if(material.texture.is_texture == true)\n"  // has texture - light + material_texture
				"{" ////////////////////////////////////////////////// version 1 (multiple textures) (delete this)
					/*"for(int i = 0; i < material.texture.count; i++)\n" // material_color will not have an affect, only light
                    "{\n"
					    "ambient  = light.ambient  * vec3(texture(material.texture.diffuse[i], Texcoord));\n"
	    		        "diffuse  = light.diffuse  * vec3(texture(material.texture.diffuse[i], Texcoord)) * max(dot(normalize(Normal), light_direction), 0.0);\n"
	    	            "specular = light.specular * vec3(texture(material.texture.specular[i], Texcoord)) * pow(max(dot(view_direction, reflect(-light_direction, normalize(Normal))), 0.0), material.shininess);\n"
				    "}\n"*/
                    ////////////////////////////////////////////////// version 2 (single texture)
                    "{\n"
					    "ambient  = light.ambient  * vec3(texture(material.texture.diffuse, Texcoord));\n"
	    		        "diffuse  = light.diffuse  * vec3(texture(material.texture.diffuse, Texcoord)) * max(dot(normalize(Normal), light_direction), 0.0);\n"
	    	            "specular = light.specular * vec3(texture(material.texture.specular,Texcoord)) * pow(max(dot(view_direction, reflect(-light_direction, normalize(Normal))), 0.0), material.shininess);\n"
				    "}\n"
				"}"				
				"out_color = vec4((ambient + diffuse + specular), 1.0);\n"
            "}\n"		
	    };		
    if(!shader->has_program())
    {
	    shader->create();
	    shader->set_source(vertex_source  , 0);
	    shader->set_source(fragment_source, 1);
	    shader->prepare();
	#ifdef DOKUN_DEBUG
	    Logger::push(DOKUN_LOGTAG "Model shaders have been generated (count=2)");
    #endif			
    }
// generate vertex array objects outside of loop (outside draw_function)
#endif	
}
////////////
void Model::draw()
{
	if(is_visible())
	{
		// check if shaders are generated
		Model::generate();
		// Draw model
		Renderer::draw_model (get_vertex_array(), get_element_array(0), get_uv_array(), get_normal_array(), x, y, z, rotation.x, rotation.y, rotation.z, scale_factor.x, scale_factor.y, scale_factor.z, origin.x, origin.y, origin.z, get_texture_array(),
			get_material("ambient"), get_material("diffuse"), get_material("specular"), get_material("emission"), get_shininess(), *shader);
	}
}        
////////////
int Model::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));		
	    model->draw();
	}
	return 0;
}
////////////
void Model::update(void)
{/*
	// try to manipulate vertices
	for(int i = 0; i < vertices.vertex.size(); i++)
	{
		//vertices.vertex[i].set(Mouse::get_position()); // *WINDOW::get_active()
		vertices.vertex[i].x = vertices.vertex[i].x + Mouse::get_position().x;
		vertices.vertex[i].y = vertices.vertex[i].y + Mouse::get_position().y;
	}*/
}  
////////////  
void Model::translate(double x, double y, double z)
{
	(this)->x = (this)->x + x;
    (this)->y = (this)->y + y;	
	(this)->z = (this)->z + z;
}
////////////
void Model::translate(const Vector3 & p)
{
    translate (p.x, p.y, p.z);
}
////////////
int Model::translate(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    luaL_checktype(L, 4, LUA_TNUMBER);
	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->translate(static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)),static_cast<double>(lua_tonumber(L, 4)));
	}
	return 0;
}
////////////
void Model::rotate(double angle, int x, int y, int z) 
{ 	
    set_rotation(angle, x, y, z);
}
////////////
void Model::rotate(double angle, const Vector3& r)
{
	rotate (angle, static_cast<int>(r.x), static_cast<int>(r.y), static_cast<int>(r.z));
}
////////////
int Model::rotate(lua_State *L)
{
    return set_rotation(L);
}
////////////
void Model::scale(double sx, double sy, double sz) 
{
    set_scale(sx, sy, sz);
}
////////////
void Model::scale(const Vector3 & s)
{
    scale (s.x, s.y, s.z);	
}
////////////
int Model::scale(lua_State *L)
{
    return set_scale(L);
}
////////////
void Model::shear(double shx, double shy, double shz)
{}
////////////
void Model::shear(const Vector3 & sh)
{
	shear (sh.x, sh.y, sh.z);
}
////////////
int Model::shear(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->shear(static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)), static_cast<double>(lua_tonumber(L, 4)));
	}		
	return 0;
}
////////////
void Model::reflect(int x, int y, int z)
{
	if(x != 0) {
		scale(-1, 1, 1);
	}
    if(y != 0) {
		scale(1, -1, 1);	   
    }
    if(z != 0) {
		scale(1, 1, -1);	   
    }
    if(x && y && z) {
		scale(-1, -1, -1);
	}	
}
////////////
int Model::reflect(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->reflect(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)));
	}		
	return 0;	
}
////////////
////////////
void Model::set_position(double x, double y, double z)
{
	(this)->x = x;
	(this)->y = y;
	(this)->z = z;
}
////////////
void Model::set_position(const Vector3 & p)
{
	set_position(p.x, p.y, p.z);
}
////////////
int Model::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checknumber(L, 2);
    luaL_checknumber(L, 3);
    luaL_checknumber(L, 4);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->set_position(static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)), static_cast<double>(lua_tonumber(L, 4)));
	}		
	return 0;
}
////////////
void Model::set_rotation(double angle, int x, int y, int z) // in degrees
{
    //angle = ((int)angle + 360) % 360; // set limit from 0-360
	if(x) rotation.x = angle;
    if(y) rotation.y = angle;
    if(z) rotation.z = angle;
}
////////////
void Model::set_rotation(double angle, const Vector3 & R)
{
	set_rotation(angle, static_cast<int>(R.x), static_cast<int>(R.y), static_cast<int>(R.z));
}
////////////
int Model::set_rotation(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    luaL_checktype(L, 4, LUA_TNUMBER);
    luaL_checktype(L, 5, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->set_rotation(static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)), static_cast<double>(lua_tonumber(L, 4)), static_cast<double>(lua_tonumber(L, 5)));
	}
	return 0;	
}
////////////
void Model::set_scale(double sx, double sy, double sz)
{	
	scale_factor.x = sx;
	scale_factor.z = sz;
	scale_factor.y = sy;
}
////////////
void Model::set_scale(const Vector3 & S)
{
	set_scale(S.x, S.y, S.z);
}
////////////
int Model::set_scale(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->set_scale( static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)), static_cast<double>(lua_tonumber(L, 4)));
	}
	return 0;	
}
////////////
void Model::set_origin(double x, double y, double z)
{
	origin = Vector3(x, y, z);
}
////////////
void Model::set_origin(const Vector3 & origin)
{
	set_origin(origin.x, origin.y, origin.z);
}
////////////
int Model::set_origin(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
    luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->set_origin( static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)), static_cast<double>(lua_tonumber(L, 4)));
	}
	return 0;		
}
////////////
void Model::set_texture(const Texture& texture)
{
	//double s = uv_array[0].x;
	//double t = uv_array[0].y;
	//Vector2 st = uv_array[0];
	// clamping
	//if(st > 1) // use 1
	//if(st < 0) // use 0
	// wrapping - use st modulo 1
	//st % 1
	const_cast<Texture&>(texture).generate();// generate texture if not generated (ONLY once)
	texture_list.push_back(&const_cast<Texture&>(texture)); // store texture in texture_list
}
////////////
void Model::set_texture(const Texture& texture, const std::string& type)
{
    const_cast<Texture&>(texture).generate(); // generate texture if not yet generated (ONLY once)
	const_cast<Texture&>(texture).set_type(type); // set texture type: diffuse, specular, etc.
	texture_list.push_back(&const_cast<Texture&>(texture)); // store texture in texture_list
}
////////////
int Model::set_texture(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	luaL_optnumber(L, 3, 0);
	if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1)) 
	    {
		    Texture * texture = *static_cast<Texture**>(lua_touserdata(L, -1));
		    lua_getfield(L, 1, "udata");
		    if(lua_isuserdata(L, -1)) 
		    {
	            Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
	            model->set_texture(*texture);
			    // store texture in Lua
			    lua_getfield (L, 1, "texture_list"); // lua_getfield
			    if(lua_istable(L, -1))
			    {
			        #ifdef DOKUN_LUA51
			        int size = lua_objlen(L, -1);
			        #endif
			        #ifndef DOKUN_LUA51
			        int size = luaL_len(L, -1); // size of table
			        #endif
			        lua_pushvalue(L, 2);
			        lua_rawseti(L, -2, size + 1);
			    }
		    }
	    }
	}
	if(lua_isnil(L, 2))
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		    model->texture_list.erase(model->texture_list.begin() + static_cast<int>(lua_tonumber(L, 3) - 1));
		}
		lua_getfield (L, 1, "texture_list"); // lua_getfield
		if(lua_istable(L, -1))
		{
			#ifdef DOKUN_LUA51
			int size = lua_objlen(L, -1);
			#endif
			#ifndef DOKUN_LUA51
			int size = luaL_len(L, -1); // size of table
			#endif
			lua_pushnil(L); // push nil
			lua_rawseti(L, -2, static_cast<int>(lua_tonumber(L, 3))); // set last texture to nil (remove last texture)
		}
	}	
	return 0;
}
////////////
void Model::set_material(const std::string& name, double x, double y, double z, double w)
{
	if(String::lower(name) == "ambient")
	{
		material.ambient.x = x;
		material.ambient.y = y;
		material.ambient.z = z;
		material.ambient.w = w;
	}
	if(String::lower(name) == "diffuse")
	{
		material.diffuse.x = x;
		material.diffuse.y = y;
		material.diffuse.z = z;
		material.diffuse.w = w;
	}
	if(String::lower(name) == "specular")
	{
		material.specular.x = x;
		material.specular.y = y;
		material.specular.z = z;
		material.specular.w = w;
	}
	if(String::lower(name) == "emission")
	{
		material.emission.x = x;
		material.emission.y = y;
		material.emission.z = z;
		material.emission.w = w;
	}
	if(String::lower(name) == "shininess")
	{
		material.shininess = x;
	}
}
////////////
void Model::set_material(const std::string& name, const Vector3& color)
{
	set_material(name, color.x, color.y, color.z);	
}
////////////
void Model::set_material(const std::string& name, const Vector4& color)
{
	set_material(name, color.x, color.y, color.z, color.w);
}
////////////
int Model::set_material(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TNUMBER); // x
	luaL_optnumber(L, 4, 0.0);         // y
	luaL_optnumber(L, 5, 0.0);         // z
	luaL_optnumber(L, 6, 1.0);         // w (optional)
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		model->set_material(lua_tostring(L, 2), static_cast<int>(lua_tonumber(L, 3)), static_cast<int>(lua_tonumber(L, 4)), static_cast<int>(lua_tonumber(L, 5)), static_cast<int>(lua_tonumber(L,  6)));
	}	
	return 0;	
}
////////////
////////////
Vector3 Model::get_position() const /* in units */
{
	return Vector3(x, y, z);
}
////////////
int Model::get_position(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
	    Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
	    lua_pushnumber(L, model->get_position().x);
	    lua_pushnumber(L, model->get_position().y);
	    lua_pushnumber(L, model->get_position().z);
		return 3;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
    return 3;
}
////////////
Vector3 Model::get_rotation() const/* in degrees */
{
	return rotation; //* 57.2958;
}
////////////
int Model::get_rotation(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
	    Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_pushnumber(L, model->get_rotation().x);
		lua_pushnumber(L, model->get_rotation().y);
		lua_pushnumber(L, model->get_rotation().z);
		return 3;
	}
    lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}
////////////
Vector3 Model::get_scale()const
{
	return scale_factor;
}
////////////
int Model::get_scale(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
	    Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_pushnumber(L, model->get_scale().x);
		lua_pushnumber(L, model->get_scale().y);
		lua_pushnumber(L, model->get_scale().z);
		return 3;
	}		
	lua_pushnil(L);
	lua_pushnil(L);	
	lua_pushnil(L);	
	return 3;
}
////////////
Vector3 Model::get_origin()const
{
	return origin;
}
////////////
int Model::get_origin(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);	
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
	    Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_pushnumber(L, model->get_origin().x);
		lua_pushnumber(L, model->get_origin().y);
		lua_pushnumber(L, model->get_origin().z);
		return 3;
	}		
	lua_pushnil(L);
	lua_pushnil(L);	
	lua_pushnil(L);	
	return 3;	
}
////////////
Texture * Model::get_texture(int index)const
{
	if((texture_list.empty()))
		return nullptr;
	return texture_list[index];
}
////////////
int Model::get_texture(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);	// index
	lua_getfield(L, 1, "texture_list");
	if(lua_istable(L, -1)) 
	{
		lua_rawgeti(L, -1, static_cast<int>(lua_tonumber(L, 2))); // get textures from table, texture_list
		return 1; // return texture		
	}
	lua_pushnil(L);
	return 1;
}
////////////
Vector3 Model::get_size()const
{
	if(vertices.vertex.size() == 0)
	{
		return Vector3(0, 0, 0);
	}
	double min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	min_x = max_x = vertices.vertex[0].x;
	min_y = max_y = vertices.vertex[0].y;
	min_z = max_z = vertices.vertex[0].z;
    for (int i = 0; i < vertices.vertex.size(); i++)  // scan through vertices
	{
        if (vertices.vertex[i].x < min_x) min_x = vertices.vertex[i].x;
        if (vertices.vertex[i].x > max_x) max_x = vertices.vertex[i].x;
        if (vertices.vertex[i].y < min_y) min_y = vertices.vertex[i].y;
        if (vertices.vertex[i].y > max_y) max_y = vertices.vertex[i].y;
        if (vertices.vertex[i].z < min_z) min_z = vertices.vertex[i].z;
        if (vertices.vertex[i].z > max_z) max_z = vertices.vertex[i].z;
    }
    return Vector3(max_x - min_x, max_y - min_y, max_z - min_z); // width, height, depth
}
////////////
int Model::get_size(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
	    lua_pushnumber(L, model->get_size().x);
		lua_pushnumber(L, model->get_size().y);
		lua_pushnumber(L, model->get_size().z);
        return 3;		
	}    
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;
}
////////////
Vector3 Model::get_center()const
{
	if(vertices.vertex.size() == 0)
	{
		return Vector3(0, 0, 0);
	}
	double min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	min_x = max_x = vertices.vertex[0].x;
	min_y = max_y = vertices.vertex[0].y;
	min_z = max_z = vertices.vertex[0].z;
    for (int i = 0; i < vertices.vertex.size(); i++)  // scan through vertices
	{
        if (vertices.vertex[i].x < min_x) min_x = vertices.vertex[i].x;
        if (vertices.vertex[i].x > max_x) max_x = vertices.vertex[i].x;
        if (vertices.vertex[i].y < min_y) min_y = vertices.vertex[i].y;
        if (vertices.vertex[i].y > max_y) max_y = vertices.vertex[i].y;
        if (vertices.vertex[i].z < min_z) min_z = vertices.vertex[i].z;
        if (vertices.vertex[i].z > max_z) max_z = vertices.vertex[i].z;
    }
    return Vector3((min_x + max_x)/2, (min_y + max_y)/2, (min_z + max_z)/2);
}
////////////
double  Model::get_radius()const
{
	if(vertices.vertex.size() == 0)
	{
		return 0.0;
	}
	double min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	min_x = max_x = vertices.vertex[0].x;
	min_y = max_y = vertices.vertex[0].y;
	min_z = max_z = vertices.vertex[0].z;
    for (int i = 0; i < vertices.vertex.size(); i++)  // scan through vertices
	{
        if (vertices.vertex[i].x < min_x) min_x = vertices.vertex[i].x;
        if (vertices.vertex[i].x > max_x) max_x = vertices.vertex[i].x;
        if (vertices.vertex[i].y < min_y) min_y = vertices.vertex[i].y;
        if (vertices.vertex[i].y > max_y) max_y = vertices.vertex[i].y;
        if (vertices.vertex[i].z < min_z) min_z = vertices.vertex[i].z;
        if (vertices.vertex[i].z > max_z) max_z = vertices.vertex[i].z;
    }
	return sqrt(std::pow((max_x - (min_x + max_x) / 2), 2) + std::pow((max_y - (min_y + max_y) / 2), 2) + std::pow((max_z - (min_z + max_z) / 2), 2));
}
////////////
////////////
/*
Vector3 Model::get_()const
{
	if(vertices.vertex.size() == 0)
	{
		return Vector3(0, 0, 0);
	}
	double min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	min_x = max_x = vertices.vertex[0].x;
	min_y = max_y = vertices.vertex[0].y;
	min_z = max_z = vertices.vertex[0].z;
    for (int i = 0; i < vertices.vertex.size(); i++)  // scan through vertices
	{
        if (vertices.vertex[i].x < min_x) min_x = vertices.vertex[i].x;
        if (vertices.vertex[i].x > max_x) max_x = vertices.vertex[i].x;
        if (vertices.vertex[i].y < min_y) min_y = vertices.vertex[i].y;
        if (vertices.vertex[i].y > max_y) max_y = vertices.vertex[i].y;
        if (vertices.vertex[i].z < min_z) min_z = vertices.vertex[i].z;
        if (vertices.vertex[i].z > max_z) max_z = vertices.vertex[i].z;
    }
    
}*/
////////////
////////////
////////////
Vector4 Model::get_material(const std::string& name)const
{
	if(String::lower(name) == "ambient")
		return material.ambient;
	if(String::lower(name) == "diffuse")
		return material.diffuse;
	if(String::lower(name) == "specular")
		return material.specular;
	if(String::lower(name) == "emission")
		return material.emission;
    return Vector4(0, 0, 0, 0);
}
////////////
double Model::get_shininess()const
{
	return material.shininess;
}
////////////
int Model::get_material(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_pushinteger(L, static_cast<int>(model->get_material(lua_tostring(L, 2)).x));
		lua_pushinteger(L, static_cast<int>(model->get_material(lua_tostring(L, 2)).y));
		lua_pushinteger(L, static_cast<int>(model->get_material(lua_tostring(L, 2)).z));
		lua_pushinteger(L, static_cast<int>(model->get_material(lua_tostring(L, 2)).w));
		return 4;
	}		
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
    lua_pushnil(L);	
	return 4;
}
////////////
std::vector<Vector3> Model::get_vertex_array() const     
{
	return vertices.vertex;
}
////////////
int Model::get_vertex_array(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_createtable(L, 0, 0); // # 3
		/*
		for(int i = 0; i < model->vertices.vertex.size(); i++)
		{
			lua_pushnumber(L, model->vertices.vertex[i].x);
			lua_rawseti(L, 3, i + 1);
			lua_pushnumber(L, model->vertices.vertex[i].x);
			lua_rawseti(L, 3, i + 2);
			lua_pushnumber(L, model->vertices.vertex[i].x);
			lua_rawseti(L, 3, i + 3);		
			
			lua_pop(L, 1); // pop x
			lua_pop(L, 1); // pop y
			lua_pop(L, 1); // pop z
		}
		*/
		return 1; // return table
	}
	lua_pushnil(L);
    return 1;	
}
////////////		
std::vector<Vector2> Model::get_uv_array() const
{
	return vertices.uv;
}
////////////
int Model::get_uv_array(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_createtable(L, 0, 0); // # 3
		/*
		for(int i = 0; i < model->vertices.uv.size(); i++)
		{
			//lua_pushnumber(L, model->vertices.uv[i]);
			lua_rawseti(L, 3, i);
			lua_pop(L, 1); // pop number
		}*/
		return 1; // return table
	}
	lua_pushnil(L);
    return 1;		
}
////////////		
std::vector<Vector3> Model::get_normal_array() const
{
	return vertices.normal;
}
////////////
int Model::get_normal_array(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_createtable(L, 0, 0); // # 3
		for(int i = 0; i < model->vertices.normal.size(); i++)
		{
			//lua_pushnumber(L, model->vertices.normal[i]);
			lua_rawseti(L, 3, i);
			lua_pop(L, 1); // pop number
		}
		return 1; // return table
	}
	lua_pushnil(L);
    return 1;		
}
////////////
std::vector<unsigned int> Model::get_element_array(int index) const
{
	switch(index)
	{
		case 0:
		    return elements.vertex;
		    break;
		case 1:
		    return elements.uv;
		    break;
		case 2:
		    return elements.normal;
		    break;
		default:
		    return elements.vertex;
		    break;
	}
}
////////////
int Model::get_element_array(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
		lua_createtable(L, 0, 0); // # 4
		for(int i = 0; i < model->elements.vertex.size(); i++)
		{
			//lua_pushnumber(L, model->elements.vertex[i]);
			//lua_rawseti(L, 4, i);
			//lua_pop(L, 1); // pop number
		}
		return 1; // return table
	}
	lua_pushnil(L);
    return 1;	
}
////////////
std::vector<Texture *> Model::get_texture_array()const
{
	return texture_list;
}
////////////
int Model::get_texture_array(lua_State *L)
{
	return 1;
}
////////////
////////////
bool Model::is_model() const
{   
    return ((this != 0) && (dokun::instanceof<Model>(this) != 0));
}
////////////
int Model::is_model(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushboolean(L, dynamic_cast<Model *>(entity)->is_model());
		return 1;
	}    	
    lua_pushboolean(L, false);
    return 1;
}
////////////
Model * Model::to_model(Entity * entity)
{
	return static_cast<Model *>(entity);
}
////////////
int Model::to_model(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getglobal(L, "Model");
	lua_setmetatable(L, 1);	
	Model ** model = static_cast<Model**>(lua_newuserdata(L, sizeof(Model *)));
	* model = new Model();
	lua_setfield(L, 1, "udata");
	return 1;
}
////////////
////////////
// LOADER
////////////
bool Model::load_obj(const std::string& file_name)
{
	// open file
	std::ifstream file(file_name.c_str());
	if(!file.is_open()) {
		return false;
	}	
	std::stringstream stream;
	stream << file.rdbuf(); // dump file contents	
	std::vector<std::string> content = String::split(stream.str(), "\n"); // split each line
	
	if(!vertices.vertex.empty())
	{
		vertices.vertex.clear();
		vertices.uv.clear();
		vertices.normal.clear();
		elements.vertex.clear();
		elements.uv.clear();
		elements.normal.clear();
	}
	
	for(int i = 0; i < content.size(); i++)
	{
		if(String::contains(content[i], "#")) // ignore comments
			continue; 
		
		if(String::contains(content[i], "mtllib "))
		{
			std::string mtllib     = String::swap_first_of(content[i], "mtllib ", "");
			std::string model_dir  = file_name.substr(0, file_name.find_last_of("\\/")); // load material from current model's directory
			std::string mtllib_dir = (model_dir + "/" + mtllib);
			bool loaded = false;
			if(load_mtl(mtllib))     loaded = true;// material not loading (from current dir)
			if(load_mtl(mtllib_dir)) loaded = true;
			if(loaded == false) Logger("Could not load '" + mtllib + "'");
		}
		if(String::contains(content[i], "v "))
		{
			content[i] = String::swap_first_of(content[i], "v ", ""); // remove 'v' token
			std::vector<std::string> v = String::split(content[i], " ");
			// store vertices
			vertices.vertex.push_back(Vector3(String::extract(v[0]), String::extract(v[1]), String::extract(v[2])));
			#ifdef DOKUN_DEBUG0
			    std::cout << String::extract(v[0]) << " " << String::extract(v[1])<< " " << String::extract(v[2]) << std::endl;
		    #endif
		}	
		if(String::contains(content[i], "vt "))
		{
			content[i] = String::swap_first_of(content[i], "vt ", ""); // remove 'vt' token
			std::vector<std::string> vt = String::split(content[i], " "); // split after each space
			// store uv coordinates
			vertices.uv.push_back(Vector2(String::extract(vt[0]), String::extract(vt[1])));
			#ifdef DOKUN_DEBUG0
			    std::cout << String::extract(vt[0]) << " " << String::extract(vt[1])<< " " << String::extract(vt[2]) << std::endl;
		    #endif
		}		
		if(String::contains(content[i], "f "))
		{
			content[i] = String::swap_first_of(content[i], "f ", ""); // remove 'f' token
			// ---
			if(String::occurrences(content[i], "/") == 0) // no dash (all vertices)
			{
				std::vector<std::string> f = String::split(content[i], " "); // split after space
				// store elements (assume triangle)
				elements.vertex.push_back(String::extract(f[0]) - 1);
				elements.vertex.push_back(String::extract(f[1]) - 1);
				elements.vertex.push_back(String::extract(f[2]) - 1);
				if(f.size() == 4) // forth element (quad)
				{
				    elements.vertex.push_back(String::extract(f[2]) - 1);
				    elements.vertex.push_back(String::extract(f[3]) - 1);
				    elements.vertex.push_back(String::extract(f[0]) - 1);				
				}
				#ifdef DOKUN_DEBUG0
				    std::cout << String::extract(f[0]) << " " << String::extract(f[1]) << " " << String::extract(f[2]) << std::endl;
			    #endif
			}
			// v/vt
			if(String::occurrences(content[i], "/") == 3) // single dash (3 occurances) triangle
			{
				content[i] = String::swap_first_of(content[i], "/", " ");
				content[i] = String::swap_first_of(content[i], "/", " ");
				content[i] = String::swap_first_of(content[i], "/", " ");
				// split elements
				std::vector<std::string> f = String::split(content[i], " "); // split after space
				// store elements
                elements.vertex.push_back(String::extract(f[0]) - 1);
				elements.vertex.push_back(String::extract(f[2]) - 1);
				elements.vertex.push_back(String::extract(f[4]) - 1);
                #ifdef DOKUN_DEBUG0			
				    std::cout << String::extract(f[0]) << " " << String::extract(f[2]) << " " << String::extract(f[4]) << std::endl;
				#endif
			}
			if(String::occurrences(content[i], "/") == 4) // single dash (4 occurances) quad
			{
				content[i] = String::swap_first_of(content[i], "/", " ");
				content[i] = String::swap_first_of(content[i], "/", " ");
				content[i] = String::swap_first_of(content[i], "/", " ");
				content[i] = String::swap_first_of(content[i], "/", " ");	
				// split elements
				std::vector<std::string> f = String::split(content[i], " "); // split after space
                // store elements
                elements.vertex.push_back(String::extract(f[0]) - 1);
				elements.vertex.push_back(String::extract(f[2]) - 1);
				elements.vertex.push_back(String::extract(f[4]) - 1);
				elements.vertex.push_back(String::extract(f[4]) - 1);
				elements.vertex.push_back(String::extract(f[6]) - 1);
				elements.vertex.push_back(String::extract(f[0]) - 1);
				#ifdef DOKUN_DEBUG0
				    std::cout << String::extract(f[0]) << " " << String::extract(f[2]) << " " << String::extract(f[4]) << " " << String::extract(f[6]) << std::endl;	                				
				#endif	
			}
			if(String::occurrences(content[i], "/") == 6) 
			{
				if(String::contains(content[i], "//") == false) // single dash (6 occurances) (triangle)(v/vt/vn)
				{
				    content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");	
				    content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");						
				    // split elements
				    std::vector<std::string> f = String::split(content[i], " "); // split after space	
					// store elements
					elements.vertex.push_back(String::extract(f[0]) - 1); // v
					elements.vertex.push_back(String::extract(f[3]) - 1); // v
					elements.vertex.push_back(String::extract(f[6]) - 1); // v
				#ifdef DOKUN_DEBUG0
					std::cout << String::extract(f[0]) << " " << String::extract(f[3]) << " " << String::extract(f[6]) << std::endl;                    				
				#endif
				}				
				if(String::contains(content[i], "//") == true) // double dash ( 6 occurances) triangle
				{				
			        content[i] = String::swap_first_of(content[i], "//", " ");
			        content[i] = String::swap_first_of(content[i], "//", " ");
			        content[i] = String::swap_first_of(content[i], "//", " ");
					// split elements
					std::vector<std::string> f = String::split(content[i], " ");
					// store elements
					elements.vertex.push_back(String::extract(f[0]) - 1); // v
					elements.vertex.push_back(String::extract(f[2]) - 1); // v
					elements.vertex.push_back(String::extract(f[4]) - 1); // v
				#ifdef DOKUN_DEBUG0
					std::cout << String::extract(f[0]) << " " << String::extract(f[2]) << " " << String::extract(f[4]) << std::endl;
				#endif
				}
			}			
			if(String::occurrences(content[i], "/") == 8) 
			{
                if(String::contains(content[i], "//") == false) // single dash (8 occurances) (quad)(v/vt/vn)
				{
				    content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");
				    content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");
			        content[i] = String::swap_first_of(content[i], "/", " ");					
				    // split elements
				    std::vector<std::string> f = String::split(content[i], " "); // split after space	
					// store elements
					elements.vertex.push_back(String::extract(f[0]) - 1); // v
					elements.vertex.push_back(String::extract(f[3]) - 1); // v
					elements.vertex.push_back(String::extract(f[6]) - 1); // v
					elements.vertex.push_back(String::extract(f[9]) - 1); // v					
					// display elements
				#ifdef DOKUN_DEBUG0
					std::cout << String::extract(f[0]) << " " << String::extract(f[3]) << " " << String::extract(f[6]) << " " << String::extract(f[9]) << std::endl;
                #endif					
				}				
				if(String::contains(content[i], "//") == true) // double dash ( 8 occurances) quad
				{
				    content[i] = String::swap_first_of(content[i], "//", " ");
			        content[i] = String::swap_first_of(content[i], "//", " ");
			        content[i] = String::swap_first_of(content[i], "//", " ");
			        content[i] = String::swap_first_of(content[i], "//", " ");		
					// split elements
					std::vector<std::string> f = String::split(content[i], " ");
					// store elements
					elements.vertex.push_back(String::extract(f[0]) - 1); // v
					elements.vertex.push_back(String::extract(f[2]) - 1); // v
					elements.vertex.push_back(String::extract(f[4]) - 1); // v
					elements.vertex.push_back(String::extract(f[6]) - 1); // v					
					// display elements
				#ifdef DOKUN_DEBUG0
					std::cout << String::extract(f[0]) << " " << String::extract(f[2]) << " " << String::extract(f[4]) << " " << String::extract(f[6]) << std::endl;
                #endif					
				}
			}
		}
	}
	// normalize
	vertices.normal.resize(vertices.vertex.size(), Vector3(0.0, 0.0, 0.0));
	for(int i = 0; i < elements.vertex.size(); i += 3)
	{
		unsigned int a = elements.vertex[i];
		unsigned int b = elements.vertex[i + 1];
		unsigned int c = elements.vertex[i + 2];
		
		Vector3 a0 = Vector3(vertices.vertex[b] - vertices.vertex[a]);
		Vector3 b0 = Vector3(vertices.vertex[c] - vertices.vertex[a]);
		Vector3 c0 = Vector3::cross(a0, b0);
		Vector3 n0 = Vector3::normal(c0);
		vertices.normal[a] = vertices.normal[b] = vertices.normal[c] = n0;	
	}
	// close file
	file.close();
	return true;
}
////////////
bool Model::load_dae(const std::string& file_name)
{   /*
	xmlDocPtr document = xmlParseFile(file_name.c_str());
    if (document == nullptr) 
    {
        std::cerr << "Could not parse document \n";
        return 0;
    }
    xmlNodePtr root = xmlDocGetRootElement(document); // get root (one root at a time)
    if (root == nullptr) 
	{
        std::cerr << "Document is empty\n";
        xmlFreeDoc(document);
        return false;
    }
    if (xmlStrcmp(root->name, (const xmlChar *) "COLLADA")) // root
	{
        std::cerr<<"No 'COLLADA' root found" << std::endl;
        xmlFreeDoc(document);
        return false;
    }
	xmlNodePtr node = root; 
	///////////////////////

	
    ///////////////////////
	xmlFreeDoc(document);
	*/
	return true;
}
////////////
////////////
bool Model::load_mtl(const std::string& file_name) 
{
    std::string line;
	std::ifstream file(file_name.c_str());
	if(!file.is_open()) 
	{
		return false;
	}
	while(std::getline(file, line))
	{  
	    std::stringstream 
			stream(line);
	    stream >> line;
		if(line == "newmtl") 
		{
            std::string newmtl;
			stream >> newmtl;
			//material.push_back(newmtl);
		}
		if(line == "Ka") // ambient
		{
			double red, green, blue;
			stream >> red >> green >> blue; 
            material.ambient = Vector4(red, green, blue, 1.0);
		#ifdef DOKUN_DEBUG0	
			std::cout << "Ambient " << material.ambient << std::endl;
		#endif	
		}
		if(line == "Kd") // diffuse
		{
			double red, green, blue;
			stream >> red >> green >> blue; 
            material.diffuse = Vector4(red, green, blue, 1.0);
		#ifdef DOKUN_DEBUG0	
			std::cout << "Diffuse " << material.diffuse << std::endl;
		#endif					
		}
		if(line == "Ks") // specular
		{
			double red, green, blue;
			stream >> red >> green >> blue; 
            material.specular = Vector4(red, green, blue, 1.0);		
		#ifdef DOKUN_DEBUG0	
			std::cout << "Specular " << material.specular << std::endl;
		#endif					
		}
		if(line == "Ke") // emision
		{
			double red, green, blue;
			stream >> red >> green >> blue; 
            material.emission = Vector4(red, green, blue, 1.0);
		#ifdef DOKUN_DEBUG0	
			std::cout << "Emission " << material.emission << std::endl;
		#endif					
		}
		if(line == "Ns") // shininess
		{
			double Ns;
			stream >> Ns;
			material.shininess = Ns;
		#ifdef DOKUN_DEBUG0	
			std::cout << "Shininess " << material.shininess << std::endl;
		#endif					
		}
		if(line == "d") // transparency
		{
			double d;
			stream >> d;	
		#ifdef DOKUN_DEBUG0	
			//std::cout << " " << material. << std::endl;
		#endif					
		}				
	}
    file.close();
	return true;
}
////////////
int Model::load_mtl(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1)) 
	{
        Model * model = *static_cast<Model**>(lua_touserdata(L, -1));
	    model->load_mtl(lua_tostring(L, 2));
	}
	lua_pushboolean(L, false);
	return 1;
}
////////////
