#include "../include/ui.h"

/////////////
GUI::GUI (void) : x (0), y (0), angle(0), scale_x(1), scale_y(1), width (0), height (0), orientation(0), parent(nullptr),    visible(true), active(true),    draggable(false), droppable(false), resizeable(false), sortable(false)
{
	Factory::get_gui_factory()->store(this);
#ifdef DOKUN_DEBUG    
    Logger::push(DOKUN_LOGTAG + "GUI " + String(this).str() + " allocated (index=" + String(Factory::get_gui_factory()->get_location(this)).str() + ")" + " (total_gui_instances=" + String(Factory::get_gui_factory()->get_size()).str() + ")");
#endif		
}
/////////////
GUI::~GUI(void)
{
	Factory::get_gui_factory()->release(this); 
#ifdef DOKUN_DEBUG 	
	Logger::push(DOKUN_LOGTAG + "GUI " + String(this).str() + " deallocated (total_gui_instances=" + String(Factory::get_gui_factory()->get_size()).str() + ")");
#endif	
}
/////////////
GUI * GUI::focused  (nullptr);
/////////////
Shader * GUI::shader(new Shader());
/////////////
void GUI::show()
{
	set_visible(true);
}
/////////////
int GUI::show(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->show (); 
	}
	return 0;
}
/////////////  
void GUI::hide()
{
	set_visible(false);
}
/////////////  
int GUI::hide(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->hide (); 
	}
	return 0;
}
/////////////  
void GUI::show_all()
{
	for (int i = 0; i < Factory::get_gui_factory()->get_size(); i++) 
	{
		GUI * gui = static_cast<GUI *>(Factory::get_gui_factory()->get_object(i));
		gui->show();
	}
}   
/////////////  
int GUI::show_all(lua_State *L)
{
	GUI::show_all ();
	return 0;
}
/////////////  
void GUI::hide_all()
{
	for (int i = 0; i < Factory::get_gui_factory()->get_size(); i++) 
	{
		GUI * gui = static_cast<GUI *>(Factory::get_gui_factory()->get_object(i));
		gui->hide();
	}	
}
/////////////  
int GUI::hide_all(lua_State *L)
{
	GUI::hide_all ();
	return 0;
}
/////////////  
void GUI::connect(const GUI& a, int signal, const GUI& b, std::function<void(void)> slot)
{
	if(signal == 0) // hover
	{
		if(const_cast<GUI&>(a).is_hovered()) slot();
	}
    if(signal == 1) // pressed
	{
		if(const_cast<GUI&>(a).is_pressed()) slot();
	}	
    if(signal == 2) // released
	{
		if(const_cast<GUI&>(a).is_released()) slot();
	}	
}
/////////////  
void GUI::generate(void)
{
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
            "gl_Position = proj * model * vec4(position, 0.0, 1.0);\n" // no need for 'view' as GUIs do not depend on camera
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
	bool is_shader = shader->has_program();
    if(is_shader == false)
	{
		shader->create();
		shader->set_source(vertex_source  , 0);
		shader->set_source(fragment_source, 1);
		shader->compile(0);
		shader->compile(1);
		shader->attach(0);
		shader->attach(1);
		shader->link();
	}		
}
/////////////  
/////////////  
// SETTERS     
/////////////   
void GUI::set_width(int width)
{
	this->width = width;	
}
/////////////
int GUI::set_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_width(static_cast<int>(lua_tonumber(L, 2)));
	}	
	return 0;
}
/////////////   
void GUI::set_height(int height)
{
	this->height = height;
}
/////////////   
int GUI::set_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_height(static_cast<int>(lua_tonumber(L, 2)));
	}		
	return 0;
}
/////////////    
void GUI::set_size(int width, int height)
{
    set_width (width );
	set_height(height);
}
/////////////   
void GUI::set_size(const Vector2& size)
{
	set_size(size.x, size.y);
}
/////////////   
int GUI::set_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_size(static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3)));
	}		
	return 0;
}
/////////////    
void GUI::set_position(double x, double y) // once a parent is set, a gui is locked to that parent unless parent is set to nullptr
{
	if(parent != nullptr) // if object has a parent, set position relative to parent or else this function will be useless :D UPDATED: 9-7-2018
	{
		set_relative_position(x, y);  // now that it is a child, its position should be relative to the parent position
		return;
	}
	this->x = x;
	this->y = y;
}
///////////// 
void GUI::set_position(const Vector2& position)
{
	set_position(position.x, position.y);
}
///////////// 
int GUI::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_position(static_cast<double>(lua_tonumber(L, 2)), static_cast<double>(lua_tonumber(L, 3)));
	}	
	return 0;
} 
///////////// 
void GUI::set_relative_position(double x, double y) // set child position relative to the parent
{
	GUI * parent = get_parent();
	if(parent != nullptr) 
	{
        this->x = parent->get_x() + x;
        this->y = parent->get_y() + y;
	}
}
///////////// 
void GUI::set_relative_position(const Vector2& position)
{
	set_relative_position(position.x, position.y);
}
/////////////
int GUI::set_relative_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);	
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_relative_position(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
/////////////
void GUI::set_scale(double sx, double sy)
{
	this->scale_x = sx;
	this->scale_y = sy;
}
///////////// 
int GUI::set_scale(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
    luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_scale (static_cast<int>(lua_tonumber(L, 2)), static_cast<int>(lua_tonumber(L, 3))) ;
	}
	return 0;
}
///////////// 
void GUI::set_angle(double angle)
{
	this->angle = angle; // in degrees
}
///////////// 
int GUI::set_angle(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_angle (static_cast<double>(lua_tonumber(L, 2)));
	}
	return 0;
}
///////////// 
void GUI::set_orientation(int orientation)
{
	switch(orientation)
	{
		case 0:
		    set_angle(static_cast<double>(0));
		break;
		case 1:
		    set_angle(static_cast<double>(90));
		break;
	}
    this->orientation = orientation;
}
///////////// 
int GUI::set_orientation(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_orientation (static_cast<int>(lua_tonumber(L, 2))) ;
	}
	return 0;
}
///////////// 
void GUI::set_parent(const GUI& parent)
{
	this->parent = &const_cast<GUI&>(parent);
	// set position relative to parent (if parent set)
	set_position(parent.get_x(), parent.get_y());
	//set_visible (get_parent()->is_visible()); // if parent is visible child will also be visible
}                 
///////////// 
int GUI::set_parent(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2); // parent
	if(lua_istable(L, 2))
	{
	    lua_getfield(L, 2, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    GUI * parent = *static_cast<GUI **>(lua_touserdata(L, -1));	
		    lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
		        GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		        gui->set_parent(*parent); // set parent in (C++)
			    // set parent in (Lua)
		        lua_pushvalue(L, 2);
	  	        lua_setfield(L, 1, "parent");
	        }
	    }
	}
	if(lua_isnil(L, 2))
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		    gui->parent = nullptr; // set parent to nullptr (C++)
		}
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "parent"); // set parent to nil (Lua)
	}
	return 0;	
}	 
///////////// 
void GUI:: set_active(bool active) // active or disabled
{
	this->active = active;
}
///////////// 
int GUI:: set_active(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_active((lua_toboolean(L, 2) != 0));
	}
	return 0;
}
///////////// 
void GUI:: set_visible(bool visible) // shown or hidden
{ 
	this->visible = visible;
	for(int i = 0; i < get_children().size(); i++)
	{
		GUI * child = get_children()[i];
		child->set_visible (visible); // set child visibility to the same as parent
	}
}
///////////// 
int GUI::set_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_visible((lua_toboolean(L, 2) != 0));
	}	
	return 0;
}
///////////// 
void GUI::set_focused(bool focused)
{
    GUI::focused = (focused == true) ? this : nullptr; 
}
/////////////
int GUI::set_focused(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		gui->set_focused((lua_toboolean(L, 2) != 0));
	}	
	return 0;	
}
///////////// 
///////////// 
void GUI::set_draggable (bool draggable)
{
	this->draggable = draggable;
}
///////////// 
int GUI::set_draggable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_draggable((lua_toboolean(L, 2) != 0));
	}

	return 0;
}
///////////// 
void GUI::set_droppable (bool droppable)
{
	this->droppable = droppable;
}
///////////// 
int GUI:: set_droppable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_droppable((lua_toboolean(L, 2) != 0));
	}	
	return 0;
}
///////////// 
void GUI::set_resizeable(bool resizeable)
{
	this->resizeable = resizeable;
}
///////////// 
int GUI:: set_resizeable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_resizeable((lua_toboolean(L, 2) != 0));
	}	
	return 0;
}
/////////////  
void GUI::set_sortable(bool sortable)
{
	this->sortable = sortable;
}           
/////////////  
int GUI::set_sortable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    gui->set_sortable((lua_toboolean(L, 2) != 0));
	}	
	return 0;	
}
/////////////  
///////////// 
// GETTERS 
///////////// 
int GUI::get_width() const
{
	return width * get_scale().x; // returns width whether scaled or not
}
///////////// 
int GUI::get_width(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_width());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
///////////// 
int GUI:: get_height() const
{
	return height * get_scale().y; // returns height whether scaled or not
}
///////////// 
int GUI:: get_height(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
	    lua_pushnumber(L, gui->get_height());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
///////////// 
Vector2 GUI::get_size() const
{
	return Vector2(get_width(), get_height());
} 
///////////// 
int GUI::get_size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_size().x);
		lua_pushnumber(L, gui->get_size().y);
		return 2;
	}		
	lua_pushnil(L);
	return 1;
}
///////////// 
double GUI::get_x() const
{
	return x;
} 
///////////// 
int GUI:: get_x(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_x());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
///////////// 
double GUI::get_y() const
{
	return y;
}
///////////// 
int GUI::get_y(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_y());
		return 1;
	}		
	lua_pushnil(L);
	return 1;
}
///////////// 
Vector2 GUI::get_position() const
{
	return Vector2(get_x(), get_y());
}
///////////// 
int GUI::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_position().x);
		lua_pushnumber(L, gui->get_position().y);
		return 2;
	}
	lua_pushnil(L);
	return 1;
}
///////////// 
double GUI::get_relative_x() const
{
	GUI * parent = get_parent();
    if(parent == nullptr) 
	{
		//Logger("GUI has no parent : ui.cpp | line: 638");
		return get_x();
	}
	return get_position().x - parent->get_position().x;	
} 
///////////// 
int GUI::get_relative_x(lua_State *L)
{}
///////////// 
double GUI::get_relative_y() const
{
	GUI * parent = get_parent();
    if(parent == nullptr) 
	{
		//Logger("GUI has no parent : ui.cpp | line: 652");
		return get_y();
	}
	return get_position().y - parent->get_position().y;	
}
///////////// 
int GUI::get_relative_y(lua_State *L)
{}
///////////// 
Vector2 GUI::get_relative_position() const // returns object position relative to the parent coordinates
{
	return Vector2(get_relative_x(), get_relative_y());
}
/////////////
int GUI::get_relative_position(lua_State *L)
{}
/////////////
Vector2 GUI::get_scale()const
{
	return Vector2(scale_x, scale_y);
}
/////////////
int GUI::get_scale(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_scale().x);
        lua_pushnumber(L, gui->get_scale().y);
		return 2;
	}	
	lua_pushnil(L);
    lua_pushnil(L);
	return 2;
}
/////////////
double GUI::get_angle()const
{
	return angle;
}
/////////////
int GUI::get_angle(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_angle());
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
/////////////
int GUI::get_orientation() const
{
	return orientation;
}   
///////////// 
int GUI::get_orientation(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_orientation());
		return 1;
	}	
	lua_pushnil(L);
	return 1;	
}
/////////////
GUI * GUI::get_parent() const
{
	return parent;
}
///////////// 
int GUI::get_parent(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "parent");
		if(lua_istable(L, -1))
		{
			return 1;
		}
	}	
	lua_pushnil(L);
	return 1;
}
/////////////
std::vector<GUI *> GUI::get_children()
{
	std::vector<GUI *> children;
	for(int i = 0; i < Factory::get_gui_factory()->get_size(); i++)
	{
		GUI * gui = static_cast<GUI *>(Factory::get_gui_factory()->get_object(i));
		if(gui->get_parent() == this)
		{
			children.push_back(gui);
		}
	}
	return children;
}
/////////////
int GUI::get_children(lua_State *L)
{
	return 0;
}
/////////////
Vector4 GUI::get_rect()const
{
	return Vector4(get_x(), get_y(), get_width(), get_height());
}
int GUI::get_rect(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushnumber(L, gui->get_rect().x);	
		lua_pushnumber(L, gui->get_rect().y);	
		lua_pushnumber(L, gui->get_rect().z);	
		lua_pushnumber(L, gui->get_rect().w);
        return 4;
	}
    lua_pushnil(L);	
    lua_pushnil(L);	
    lua_pushnil(L);	
    lua_pushnil(L);
    return 4;	
}
/////////////
///////////// 
// BOOLEAN
///////////// 
bool GUI::is_visible() // checks if shown or hidden
{
	return visible;
}
/////////////
int GUI::is_visible(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_visible() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
/////////////
bool GUI::is_active() // checks if active or disabled
{
	return active;
} 
///////////// 
int GUI::is_active(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_active() );
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;
}
///////////// 
bool GUI::is_draggable()
{
	return draggable;
}
///////////// 
int GUI::is_draggable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_draggable() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
///////////// 
bool GUI::is_droppable()
{
	return droppable;
}
///////////// 
int GUI::is_droppable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_droppable() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
/////////////    
bool GUI::is_resizeable()
{
	return resizeable;
}
/////////////   
int GUI::is_resizeable(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_resizeable() );
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;
}
///////////// 
bool GUI::is_sortable()
{
	return sortable;
}
///////////// 
int GUI::is_sortable(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_sortable() );
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;	
}
///////////// 
bool GUI::is_gui()
{
    return ((this != 0) && (dokun::instanceof<GUI>(this) != 0));
}
///////////// 
int GUI::is_gui(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));
		lua_pushboolean(L, dynamic_cast<GUI *>(entity)->is_gui());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool GUI::has_parent()
{
	return (parent != nullptr);
}
/////////////
int GUI::has_parent(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->has_parent());
		return 1;
	}	
	lua_pushboolean(L, false);
	return 1;		
}
/////////////
bool GUI::is_child_of(const GUI& gui) // is this your parent? UPDATED: 9-7-2018
{
	return (parent == &gui);
}
/////////////
int GUI::is_child_of(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * parent = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		    lua_pushboolean(L, gui->is_child_of(*parent));
		    return 1;
	    }
	}	
	lua_pushboolean(L, false);
	return 1;	
}
/////////////
bool GUI::is_parent_of(const GUI& gui) // is this your child? UPDATED: 9-7-2018
{
	return (this == gui.get_parent());
}
/////////////
int GUI::is_parent_of(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * child = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_getfield(L, 1, "udata");
	    if(lua_isuserdata(L, -1))
	    {
		    GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		    lua_pushboolean(L, gui->is_parent_of(*child));
		    return 1;
	    }
	}	
	lua_pushboolean(L, false);
	return 1;		
}
/////////////
///////////// 
bool GUI::is_focused()
{
	return (this == focused);
}
/////////////
/////////////
// CALLBACK
/////////////
void GUI::on_create()
{}
/////////////
void GUI::on_draw()
{
	GUI * parent = get_parent();
	if(parent != nullptr)
	{
		// keep object within parent bounds (keep from moving outside parent)
        if(get_x() <= parent->get_x()) {set_position(parent->get_x(), get_y());}
        if(get_y() <= parent->get_y()) {set_position(get_x(), parent->get_y());}
	    if(get_x() >= parent->get_x() + (parent->get_width() - get_width()))   {set_position(parent->get_x() + (parent->get_width() - get_width()), get_y());}
	    if(get_y() >= parent->get_y() + (parent->get_height() - get_height())) {set_position(get_x(), parent->get_y() + (parent->get_height() - get_height()));}
	
	    // keep object from exceeding parent size (width and height) UPDATED: 9-7-2018
	    if(get_width () > parent->get_width ()) set_size(parent->get_width(), get_height()); // if object is wider than parent, make width equal to parent's width
		if(get_height() > parent->get_height()) set_size(get_width(), parent->get_height()); // if object is taller than parent, make height equal to parent's height
	}	
	if(is_visible()) 
	{
		for(int i = 0; i < Factory::get_gui_factory()->get_size(); i++)
		{
			GUI * gui = static_cast<GUI*>(Factory::get_gui_factory()->get_object(i));
			// draw all children if visible (child ui can also have their own visibility)
			if(is_parent_of(*gui)) { gui->draw(); }
			// set focus to the gui that is pressed
			/*if(gui->is_pressed()) {gui->set_focused(true);}*/
		}
	}
	// if mouse is over ui and mouse is pressed, set as current focus
}
/////////////
/////////////
/////////////
// EVENTS
/////////////
bool  GUI::is_hovered()
{	
	int x = 0;
	int y = 0;
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif	
	return Mouse::is_over(get_rect());
}
///////////// 
int GUI::is_hovered(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_hovered());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
///////////// 
bool GUI::is_pressed() // executes multiple times
{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif
	return (Mouse::is_over(get_rect()) && Mouse::is_pressed(1));
}
///////////// 
int GUI::is_pressed(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_pressed());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
///////////// 
bool GUI::is_selected() // executes once
{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif	
	return false;
}
///////////// 
/*
int GUI::is_(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		GUI * gui = *static_cast<GUI **>(lua_touserdata(L, -1));
		lua_pushboolean(L, gui->is_());
		return 1;
	}		
	lua_pushboolean(L, false);
	return 1;	
}
*/
///////////// 
///////////// 
bool GUI::is_released() // if released (only right after button is pressed)
{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif	
	return false;
}
///////////// 
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
bool GUI::is_dragged()
{	
    if(is_draggable())
	{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif		
	}
	return false;
}
///////////// 
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
bool GUI::is_dropped() // returns true if not dragging
{
	if(is_droppable())
	{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif		
	}
	return false;
}
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
bool GUI::is_resized()
{
	if(is_resizeable())
	{
	#ifdef __windows__
	#endif
	
	#ifdef __gnu_linux__
	#endif		
	}
	return false;
}
///////////// 
///////////// 
//int GUI::(lua_State * L)
//{}
///////////// 
/////////////
