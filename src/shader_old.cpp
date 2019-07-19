#include "../include/shader.h"

Shader::Shader() : id(0), program(0)
{}
/////////////
Shader::Shader(int type) 
{
    create(type);
}
/////////////
Shader::Shader(int type, const char * const * source)
{
	create(type, source);
}
/////////////
Shader::~Shader()
{
    destroy();
}
/////////////
/////////////
void Shader::create(int type) // glew extention functions will not work without a context (can also crashes glew)
{
#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{	
		if(!is_shader())
	    {
	        switch (type)
	        {
		case DOKUN_SHADER_VERTEX: 
		    id = glCreateShader( GL_VERTEX_SHADER );
		    break;
		case DOKUN_SHADER_FRAGMENT:
		    id = glCreateShader( GL_FRAGMENT_SHADER );
		    break;
		case DOKUN_SHADER_GEOMETRY:
		    id = glCreateShader( GL_GEOMETRY_SHADER );
		    break;
	        }
		}	
	}
#endif
#ifdef DOKUN_VULKAN
#endif
} 
/////////////
void Shader::create(int type, const char * const * source) // glew extention functions will not work without a context (can also crashes glew)
{
#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{	
		if(!is_shader())
	    {
	        switch (type)
	        {
		case DOKUN_SHADER_VERTEX: 
		    id = glCreateShader( GL_VERTEX_SHADER );
		    break;
		case DOKUN_SHADER_FRAGMENT:
		    id = glCreateShader( GL_FRAGMENT_SHADER );
		    break;
		case DOKUN_SHADER_GEOMETRY:
		    id = glCreateShader( GL_GEOMETRY_SHADER );
		    break;
	        }
		}	
	}
	// set the source
	set_source(source);
	// compile the shader
	compile();
#endif
#ifdef DOKUN_VULKAN
#endif
} 
/////////////
int Shader::create(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		shader->create((unsigned int)lua_tonumber(L, 2));
	}	
	return 0;
}
/////////////
bool Shader::load(const std::string& file_name)
{
    std::ifstream file(file_name.c_str());
	if(!file.is_open())
	{
	    return false;	
	}
	std::stringstream stream;
	stream << file.rdbuf(); // dump file contents
	file.close(); // close file
	
	fsource = stream.str().c_str(); // store in string
	
	const char * content = fsource.c_str();
	#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{	
	    if(is_shader())
	    {	
	        glShaderSource( (GLuint)get_id(), 1,  &content, nullptr );
		}
	}
    
	#endif
	return true;
}
/////////////
int Shader::load(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		shader->load( lua_tostring(L, 2));
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
void Shader::compile()
{
	#ifdef DOKUN_OPENGL
#ifdef __windows__
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return;
#endif
#endif
	if(is_shader())
	{
        glCompileShader(static_cast<GLuint>(get_id()));
	    // check for error
        GLint status;
	    glGetShaderiv( (GLuint)get_id(), GL_COMPILE_STATUS, & status );
	    if (!status)
	    {
	  	    GLint infoLogLength;
		    glGetShaderiv( (GLuint)get_id(), GL_INFO_LOG_LENGTH, &infoLogLength );
 
		    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		    glGetShaderInfoLog( (GLuint)get_id(), infoLogLength, nullptr, strInfoLog );
 
		    std::cerr << "(ERROR): Shader compilation failed " << strInfoLog << std::endl;
		    delete[] strInfoLog;
	    }
	}
#endif
}
/////////////
int Shader::compile(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		shader->compile();
	}
	return 0;
}
/////////////
void Shader::destroy() // works if not attached to a program
{
#ifdef DOKUN_OPENGL	
#ifdef __windows__
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return;
#endif
#endif
	if(is_shader())
	{
	    glDeleteShader(static_cast<GLuint>(get_id()));
	}
#endif
#ifdef DOKUN_VULKAN
	/*
	void vkDestroyShaderModule(
    VkDevice                                    device,
    VkShaderModule                              shaderModule, // VK_nullptr_HANDLE or a valid shader_module
    const VkAllocationCallbacks*                pAllocator);
	*/
#endif
}
/////////////
int Shader::destroy(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		shader->destroy();
	}	
	return 0;
}
/////////////
/////////////
void Shader::use(unsigned int program)
{
#ifdef DOKUN_OPENGL	// OpenGL is defined
#ifdef __windows__
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return;
#endif
#endif
    if(glIsProgram(program))
	{
	    glUseProgram(program);
	}	
#endif	
}
void Shader::disable(unsigned int program)
{
#ifdef DOKUN_OPENGL		
#ifdef __windows__
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return;
#endif
#endif
    if(glIsProgram(program))
	{
	    glUseProgram(0); // disable program
	}
#endif		
}
void Shader::destroy(unsigned int program)
{
#ifdef DOKUN_OPENGL		
#ifdef __windows__
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return;
#endif
#endif
    if(glIsProgram(program))
	{
	    glDeleteProgram(program); // destroy program
	}
#endif		
}
/////////////
/////////////
void Shader::set_source(const char * const * source )
{
	this->source = source;
#ifdef DOKUN_OPENGL
#ifdef __windows__
    if(!wglGetCurrentContext())
        return;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return;
#endif
#endif
	if(is_shader())
	{	
	    glShaderSource(static_cast<GLuint>(get_id()), sizeof(source)/sizeof(source), source, nullptr);
	}
#endif
}
/////////////
int Shader::set_source(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		const char * source[] =
		{
			lua_tostring(L, 2),
		};
		shader->set_source(source);
	}
	return 0;
}
/////////////
/////////////
void Shader::set_program(unsigned int program)
{
	this->program = program;
}
int set_program(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		shader->set_program(lua_tointeger(L, 2));
	}
    return 0;	
}
/////////////
unsigned int Shader::get_id()
{
    return id;
}
/////////////
int Shader::get_id(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_pushnumber(L, shader->get_id());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
const char * const * Shader::get_source()
{
    return source;
}
/////////////
int Shader::get_source(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_pushstring(L, *shader->get_source());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
std::string Shader::get_type()
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext()) 
	#endif
	{
	    if(is_shader())
		{
	        GLint param;
            glGetShaderiv(	(GLuint)get_id(), GL_SHADER_TYPE, &param);
            //return param;		
			if(param == GL_VERTEX_SHADER)
			{
				return "vertex";
			}
		    if(param == GL_FRAGMENT_SHADER)
			{
				return "fragment";
			}
		    if(param == GL_GEOMETRY_SHADER)
			{
				return "geometry";
			}
		}
	}
	#endif
	return "";
}
/////////////
int Shader::get_type(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_pushstring(L, shader->get_type().c_str());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Shader::get_program() const
{
	return program;
}
int Shader::get_program(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_pushinteger(L, shader->get_program());
		return 1;
	}
	lua_pushnil(L);
    return 1;	
}
/////////////
bool Shader::is_shader()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
    if(!wglGetCurrentContext())
        return 0;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return 0;
#endif
#endif
	return (glIsShader(static_cast<GLuint>(get_id())) != 0);
#endif
	return false;
}
/////////////
int Shader::is_shader(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_touserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_pushboolean(L, (int)shader->is_shader() );
		return 1;
	}
    lua_pushboolean(L, false);
    return 1;	
}
/////////////
int Shader::new_(lua_State *L)
{
    int type = 1;
	if(lua_type(L, -1) == LUA_TNUMBER)
	{		
        type = (unsigned int)lua_tonumber(L, -1);
	}
    // Clear stack.
    lua_settop(L, 0);
	// create a new table (object)
	lua_createtable(L, 0, 0);
	// set mt
	lua_getglobal(L, "Shader_mt");
	lua_setmetatable(L, 1);
	// set userdata
	Shader **shader = static_cast<Shader **>(lua_newuserdata(L, sizeof(Shader *)));
	if(type != 0) // not 0
	{
		*shader = new Shader(type);		
	}
	if(type != 1 || type != 2 || type != 3)
	    *shader = new Shader();
	lua_setfield(L, 1, "udata");
	if(lua_istable(L, -1))
	    return 1;
	lua_pushnil(L);
	return 1;
}
/////////////
/////////////
Program::Program() : id(0)
{}
/////////////
Program::Program(Shader& vertex_shader, Shader& fragment_shader) 
{
	if(!is_program())
	{
		create();
		// attaches shaders, but you'll have to link and use the program yourself
		attach(vertex_shader);
		attach(fragment_shader);
	}
}
/////////////
Program::~Program()
{
	disable();
	destroy();
}
/////////////
Program * Program::p_default (nullptr);
/////////////
void Program::create()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Could not create program : No OpenGL Context was found.");
		return;
	}	
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Could not create program : No OpenGL Context was found.");
		return;
	}	
#endif
#endif
	if(!glIsProgram(static_cast<GLuint>(get_id())))
	{	
        id = glCreateProgram();
	}
#endif
}
/////////////
int Program::create(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		program->create();
	}		
	return 0;
}
/////////////
void Program::attach(Shader& shader)
{  
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Could not attach shader : No OpenGL Context was found.");
		return;
	}	
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Could not attach shader : No OpenGL Context was found.");
		return;
	}	
#endif
#endif
    if(glIsProgram((GLuint)get_id()))
	{
        glAttachShader( (GLuint)get_id(), (GLuint)shader.get_id() );
	    this->shader.push_back(&shader);
    }
#endif
}
/////////////
int Program::attach(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		    program->attach(*shader);
		}
	}
	return 0;
}
/////////////
void Program::detach(Shader& shader)
{ 
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Could not detach shader : No OpenGL Context was found.");
		return;
	}	
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Could not detach shader : No OpenGL Context was found.");
		return;
	}	
#endif
#endif
	if(is_program())
	{
	    glDetachShader((GLuint)get_id(), (GLuint)shader.get_id());
		for(unsigned int i = 0; i < this->shader.size(); i++)
		{
			if(this->shader[i]->get_id() == shader.get_id())
			{
		        this->shader.erase(this->shader.begin() + i);
			}
		}
	}
#endif
}
/////////////
int Program::detach(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Shader * shader = *static_cast<Shader **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		    program->detach(*shader);
		}
	}	
	return 0;
}
/////////////
void Program::link()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Linking failed : No OpenGL Context was found.");
		return;
	}	
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Linking failed : No OpenGL Context was found.");
		return;
	}	
#endif
#endif
	if(is_program()) // is a valid program
	{		
		// link program
		if(shader.size() > 1) // At least two shaders.
            glLinkProgram((GLuint)get_id());
        // check for error (with linking)
		GLint status;
	    char buffer[512];
	    glGetProgramiv( (GLuint)get_id(), GL_LINK_STATUS, &status );
        if(!status) 
	    {
            glGetProgramInfoLog( (GLuint)get_id(), 512, nullptr, buffer );
            std::cerr << "(ERROR) : Program linking failed " << buffer << std::endl;
        }
	}
#endif
}
/////////////
int Program::link(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		program->link();
	}
	return 0;
}
/////////////
void Program::use()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Could not use program : No OpenGL Context was found.");
		return;
	}	
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Could not use program : No OpenGL Context was found.");
		return;
	}	
#endif
#endif
	if(glIsProgram((GLuint)get_id()))
    {
        glUseProgram((GLuint)get_id());
	}
#endif
}
/////////////
int Program::use(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		program->use();
	}
	return 0;
}
/////////////
void Program::disable()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Could not disable program : No OpenGL Context was found.");
		return;
	}	
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Could not disable program : No OpenGL Context was found.");
		return;
	}	
#endif
#endif
	if(glIsProgram((GLuint)get_id()))
	{
        glUseProgram(0);
	}
#endif
}
/////////////
int Program::disable(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		program->disable();
	}
	return 0;
}
/////////////
void Program::destroy()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Destruction of program failed : No OpenGL Context was found.");
		return;
	}	
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
	{
		Logger("Destruction of program failed : No OpenGL Context was found.");
		return;
	}	
#endif
#endif
	if(glIsProgram((GLuint)get_id()))
	{
		glDeleteProgram((GLuint)get_id());
	}
#endif
}
/////////////
int Program::destroy(lua_State *L)
{
	return 0;
}
/////////////
void Program::set_default()
{
	p_default = (this);
}
/////////////
int Program::set_default(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		program->set_default();
	}	
	return 0;
}
/////////////
unsigned int Program::get_id()const
{
	return id;
}
/////////////
unsigned int Program::get_attribute(const std::string& attribute) const// attribute location
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
        return glGetAttribLocation( (GLuint)get_id(), attribute.c_str());
	}
	#endif
	return -1;
}
/////////////
int Program::get_attribute(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 1, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		lua_pushnumber(L, program->get_attribute(lua_tostring(L, 2)));
		return 1;
    }    
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Program::get_uniform(const std::string& uniform) const// uniform location
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
        return glGetUniformLocation((GLuint)get_id(), uniform.c_str());
	}
	#endif
	return -1;
}
/////////////
int Program::get_uniform(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 1, LUA_TSTRING);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		lua_pushnumber(L, program->get_uniform( lua_tostring(L, 2)));
		return 1;
    }    
	lua_pushnil(L);
	return 1;
}
/////////////
void Program::bind(unsigned int index, const std::string& attribute_name)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    glBindAttribLocation( (GLuint)get_id(), index, attribute_name.c_str() );
	}
	#endif
} 
/////////////
int Program::bind(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
        program->bind((unsigned int)lua_tonumber(L, 2), lua_tostring(L, 3));		
	}
	return 0;
}
/////////////
void Program::set_attribute(const std::string& attribute_name, int value)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLuint attribute_location = glGetAttribLocation( (GLuint)get_id(), attribute_name.c_str() );
        if(attribute_location == -1)
	    {
		    std::cerr << "(ERROR) : Vertex Attribute '" << attribute_name << "' not valid" << std::endl;
		    return;		
	    }
	    glVertexAttrib1f( attribute_location, (GLfloat)value);
	}
	#endif	
}
/////////////
void Program::set_attribute(const std::string& attribute_name, double x, double y)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLuint attribute_location = glGetAttribLocation( (GLuint)get_id(), attribute_name.c_str() );
        if(attribute_location == -1)
	    {
		    std::cerr << "(ERROR) : Vertex Attribute '" << attribute_name << "' not valid" << std::endl;
		    return;		
	    }
	    glVertexAttrib2f( attribute_location, (GLfloat)x, (GLfloat)y);
	}
	#endif	
}
/////////////
void Program::set_attribute(const std::string& attribute_name, double x, double y, double z)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLuint attribute_location = glGetAttribLocation( (GLuint)get_id(), attribute_name.c_str() );
        if(attribute_location == -1)
	    {
		    std::cerr << "(ERROR) : Vertex Attribute '" << attribute_name << "' not valid" << std::endl;
		    return;		
	    }
	    glVertexAttrib3f( attribute_location, (GLfloat)x, (GLfloat)y, (GLfloat)z);
	}
	#endif	
}
/////////////
void Program::set_attribute(const std::string& attribute_name, double x, double y, double z, double w)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLuint attribute_location = glGetAttribLocation( (GLuint)get_id(), attribute_name.c_str() );
        if(attribute_location == -1)
	    {
		    std::cerr << "(ERROR) : Vertex Attribute '" << attribute_name << "' not valid" << std::endl;
		    return;		
	    }
	    glVertexAttrib4f( attribute_location, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
	}
	#endif
}
/////////////
int Program::set_attribute(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_optnumber(L, 3, 0); // x
	luaL_optnumber(L, 4, 0); // y 
	luaL_optnumber(L, 5, 0); // z
	luaL_optnumber(L, 6, 0); // w
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		program->set_attribute(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
	}	
	return 0;
}
/////////////
void Program::set_uniformi(const std::string& uniform_name, int value)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
		glUniform1i(uniform_location, (GLint)value);
	}
	#endif		
}
/////////////
void Program::set_uniformi(const std::string& uniform_name, int x, int y)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
		glUniform2i(uniform_location, (GLint)x, (GLint)y);
	}
	#endif		
}
/////////////
void Program::set_uniformi(const std::string& uniform_name, int x, int y, int z)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
		glUniform3i(uniform_location, (GLint)x, (GLint)y, (GLint)z);
	}
	#endif			
}	
/////////////
void Program::set_uniformi(const std::string& uniform_name, int x, int y, int z, int w)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
		glUniform4i(uniform_location, x, y, (GLint)z, (GLint)w);
	}
	#endif		
}
/////////////
/////////////
void Program::set_uniform(const std::string& uniform_name, double value)
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(wglGetCurrentContext())
#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
	    glUniform1f( uniform_location, (GLfloat)value);
	}
#endif		
}
/////////////
void Program::set_uniform(const std::string& uniform_name, double x, double y)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
	    glUniform2f( uniform_location, (GLfloat)x, (GLfloat)y );
	}
	#endif	
}
/////////////
void Program::set_uniform(const std::string& uniform_name, double x, double y, double z)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
	    glUniform3f( uniform_location, (GLfloat)x, (GLfloat)y, (GLfloat)z );
	}
	#endif	
}
/////////////
void Program::set_uniform(const std::string& uniform_name, double x, double y, double z, double w)
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
	    glUniform4f( uniform_location, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w );
	}
	#endif
}
/////////////
void Program::set_uniform(const std::string& uniform_name, int count, const double * value) // an array of four values [4]
{
	#ifdef DOKUN_OPENGL
	#ifdef __windows__
	if(wglGetCurrentContext())
	#endif
	{
	    GLint uniform_location = glGetUniformLocation(	(GLuint)get_id(), uniform_name.c_str() );
        if(uniform_location == -1)
	    {
		    std::cerr << "(ERROR) : Uniform '" << uniform_name << "' is not valid" << std::endl;
		    return;
	    }
	    glUniform4fv(uniform_location, count, (const GLfloat *)value); // //  the number of vec4 in the array       2nd arg = number of matrices to set (only 1); 3rd arg = transpose?
	}
	#endif	
}
/////////////
void Program::set_uniform_matrix(const std::string& uniform_name, int count, bool transpose, const double * value) // 4 x 4 matrix
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
	if(!wglGetCurrentContext())
	{
		Logger("Uniform setting failed : No context found");
		return;
	}
#endif
#ifdef __gnu_linux__
    if(!glXGetCurrentContext())
	{
		Logger("Uniform setting failed : No context found");
		return;
	}
#endif
	GLint uniform_location = glGetUniformLocation(static_cast<GLuint>(get_id()), uniform_name.c_str());
    if(uniform_location == -1)
	{
		Logger(String("(ERROR) : Uniform '") + String(uniform_name) + "' is not valid");
		return;
	}
	glUniformMatrix4fv(uniform_location, 1, false, (const GLfloat *)value); // // count = the number of matrices to use, for arrays its the number of vec4 in the array       2nd arg = number of matrices to set (only 1); 3rd arg = transpose?
#endif	
}
/////////////
/////////////
int Program::set_uniform(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_optnumber(L, 3, 0); // x
	luaL_optnumber(L, 4, 0); // y 
	luaL_optnumber(L, 5, 0); // z
	luaL_optnumber(L, 6, 0); // w	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Program * program = *static_cast<Program **>(lua_touserdata(L, -1));
		program->set_uniform(lua_tostring(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
	}
	return 0;
}
/////////////
/////////////
Program * Program::get_default()
{
	return Program::p_default;
}
/////////////
unsigned int Program::get_size() // number of shaders in this program
{
	return shader.size();
}
/////////////
int Program::get_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        Program * program = *static_cast<Program **>(lua_touserdata(L, -1));	
		lua_pushnumber(L, program->get_size());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Shader * Program::get_shader(int index)const
{
	return shader[index];
}
/////////////
/////////////
bool Program::is_program()
{
#ifdef DOKUN_OPENGL
#ifdef __windows__
    if(!wglGetCurrentContext())
        return 0;
#endif
#ifdef __gnu_linux__
#ifdef DOKUN_X11
    if(!glXGetCurrentContext())
        return 0;
#endif
#endif
	return (glIsProgram(static_cast<GLuint>(get_id())) != 0);
#endif
	return false;
} 
int Program::is_program(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "udata");
    if(lua_isuserdata(L, -1))
    {
        Program * program = *static_cast<Program **>(lua_touserdata(L, -1));	
		lua_pushboolean(L, (int)program->is_program() );
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;
}
/////////////