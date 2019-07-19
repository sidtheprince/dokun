
#include "../include/vertex.h"
Vertex::Vertex(void) : x(0.0), y(0.0), z(0.0), w(1.0)
{}
/////////////
Vertex::Vertex(const Vertex &vertex) // copy
{
	set_position(vertex.x, vertex.y, vertex.z, vertex.w);
	(this)->object = vertex.object;
}
/////////////
Vertex::Vertex(double x, double y)
{
    set_position(x, y);
}
/////////////
Vertex::Vertex(double x, double y, double z)
{
	set_position(x, y, z);
}
/////////////
Vertex::Vertex(double x, double y, double z, double w)
{
	set_position(x, y, z, w);
}
/////////////
Vertex::Vertex(const Vector2 &vector)
{
    set_position(vector.x, vector.y);
}
/////////////
Vertex::Vertex(const Vector3 &vector)
{
	set_position(vector.x, vector.y, vector.z);
}
/////////////
Vertex::Vertex(const Vector4 &vector)
{
	set_position(vector.x, vector.y, vector.z, vector.w);
}
/////////////
Vertex::Vertex(const double (&array)[2])
{
	set_position(array[0], array[1]);
}
/////////////
Vertex::Vertex(const double (&array)[3])
{
	set_position(array[0], array[1], array[2]);
}
/////////////
Vertex::Vertex(const double (&array)[4])
{
	set_position(array[0], array[1], array[2], array[3]);
}
/////////////
// create an object from multiple points
Vertex::Vertex(std::vector<Vector2>& vertex) // container or points
{
	for(unsigned int i=0;i< vertex.size();i++)
	{
		object.push_back
		(Vector4(vertex[i].x, vertex[i].y, 0.0, 1.0));
	}
}
/////////////
Vertex::Vertex(std::vector<Vector3>& vertex) // container or points
{
	for(unsigned int i=0;i< vertex.size();i++)
	{
		object.push_back
		( Vector4(vertex[i].x, vertex[i].y, vertex[i].z, 1.0) );
	}
}
/////////////
Vertex::Vertex(std::vector<Vector4>& vertex) // container or points
{
	for(unsigned int i=0;i< vertex.size();i++)
	{
		object.push_back( vertex[i] );
	}
}
/////////////
Vertex::~Vertex(void)
{}
/////////////
void Vertex::draw() // draw single vertex
{}
/////////////
void Vertex::draw2(double x, double y)
{
	// set new coordinates
	(this)->x = x;
	(this)->y = y;
	// draw vertex
	draw();
}
/////////////
void Vertex::draw3(double x, double y, double z)
{
	// set new coordinates
	(this)->x = x;
	(this)->y = y;
	(this)->z = z;
	// draw vertex
	draw();
}
/////////////
void Vertex::draw4(double x, double y, double z, double w)
{
	// set new coordinates
	(this)->x = x;
	(this)->y = y;
	(this)->z = z;
	(this)->w = w;
	// draw vertex
	draw();
}
/////////////
// vector
void Vertex::draw2v(const Vector2 &vector)
{
	x = vector.x;
	y = vector.y;
	draw();
}
/////////////
void Vertex::draw3v(const Vector3 &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	draw();
}
/////////////
void Vertex::draw4v(const Vector4 &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;
	draw();
}
/////////////
// array
void Vertex::draw2a(const double (&array)[2])
{
	x = array[0];
	y = array[1];
	draw();
}
/////////////
void Vertex::draw3a(const double (&array)[3])
{
	x = array[0];
	y = array[1];
	z = array[2];
	draw();
}
/////////////
void Vertex::draw4a(const double (&array)[4])
{
	x = array[0];
	y = array[1];
	z = array[2];
	w = array[3];
	draw();
}
/////////////
void Vertex::draw_object(unsigned int mode)
{
	if(size() != 0)
	{
		#ifdef DOKUN_OPENGL
		///*
        glGenBuffers( 1, & vertex_buffer );
 	    glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
 	    glBufferData
            ( GL_ARRAY_BUFFER, size(), &object[0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
        glVertexAttribPointer
	        ( 0, 4, GL_DOUBLE, GL_FALSE, 0, (void*)0 );
        // draw vertices
		switch(mode) // how do you want to draw / connect the vertices?
		{
		    case 0 : // points
		        glDrawArrays( GL_POINTS, 0, size() ); // tiny individual points
				break;
			case 1 : // lines
			    glDrawArrays( GL_LINES, 0, size() );
				break;
			case 2: // triangle
			    glDrawArrays( GL_TRIANGLES, 0, size() );
			    break;
		}
		#endif
		//*/
	}
}
/////////////
int Vertex::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
    if(lua_gettop(L) > 3) // at least 2 numbers(x, y) in params
	{
		// x, y, z, and w
	    if(lua_type(L, 2) && lua_type(L, 3) && lua_type(L, 4) && lua_type(L, 5))
		{
		    Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
			vertex->draw4(lua_tonumber(L, 2), lua_tonumber(L, 3),lua_tonumber(L, 4), lua_tonumber(L, 5));
		}
		// x, y, and z
		else if(lua_type(L, 2) && lua_type(L, 3) && lua_type(L, 4))
		{
		    Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
			vertex->draw3(lua_tonumber(L, 2), lua_tonumber(L, 3),lua_tonumber(L, 4));
		}
		// x and y
		else if(lua_type(L, 2) && lua_type(L, 3))
		{
	        Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
			vertex->draw2( lua_tonumber(L, 2), lua_tonumber(L, 3) );
		}
	}
	else
	{
	    Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
		vertex->draw();
	}
	return 0;
}
/////////////
int Vertex::draw_object(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
	    vertex->draw_object();
	}
	return 0;
}
/////////////
void Vertex::add(const Vector2& vertex)
{
	object.push_back( Vector4(vertex.x, vertex.y, 0.0, 1.0) );
}
/////////////
void Vertex::add(const Vector3& vertex)
{
	object.push_back( Vector4(vertex.x, vertex.y, vertex.z, 1.0) );
}
/////////////
void Vertex::add(const Vector4& vertex)
{
	object.push_back( vertex );
}
/////////////
int Vertex::add(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER); // x (required)
	luaL_checktype(L, 3, LUA_TNUMBER); // y (required)
	luaL_optnumber(L, 4, 0.0); // z (optional)
	luaL_optnumber(L, 5, 1.0); // w (optional)
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
	    vertex->add(Vector4( lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5) ));
	}
	return 0;
}
/////////////
void Vertex::set_position(double x, double y)
{
	(this)->x = x;
	(this)->y = y;
}
/////////////
void Vertex::set_position(double x, double y, double z)
{
	(this)->x = x;
	(this)->y = y;
	(this)->z = z;
}
/////////////
void Vertex::set_position(double x, double y, double z, double w)
{
	(this)->x = x;
	(this)->y = y;
	(this)->z = z;
	(this->w) = w;
}
/////////////
int Vertex::set_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TNUMBER); // x (required)
	luaL_checktype(L, 3, LUA_TNUMBER); // y (required)
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
	    if( lua_type(L, 4) == LUA_TNUMBER && lua_type(L, 5) != LUA_TNUMBER ) // z
		{
			vertex->set_position( lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4) );
		}
		else if( lua_type(L, 5) == LUA_TNUMBER ) // w
		{
			vertex->set_position( lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4),lua_tonumber(L, 5) );
		}
		else
		{
			vertex->set_position( lua_tonumber(L, 2),lua_tonumber(L, 3) );
		}
	}
	return 0;
}
/////////////
Vector4 Vertex::get_position()
{
	return Vector4(x, y, z, w);
}
/////////////
int Vertex::get_position(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
	    lua_pushnumber(L, vertex->get_position().x );
		lua_pushnumber(L, vertex->get_position().y );
		lua_pushnumber(L, vertex->get_position().z );
		lua_pushnumber(L, vertex->get_position().w );
        return 4;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
unsigned int Vertex::size()
{
	return object.size();
}
/////////////
int Vertex::size(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Vertex * vertex = *static_cast<Vertex **>(lua_touserdata(L, -1));
	    lua_pushnumber(L, vertex->size() );
		return 1;
	}
	return 0;
}
/////////////
int Vertex::new_(lua_State *L)
{
	// clear stack
	lua_settop(L, 0);
	// create table
	lua_createtable(L, 0, 0);
	// set metatable
	lua_getglobal(L, "Vertex_mt");
	lua_setmetatable(L, 1);
	// set userdata
	Vertex ** vertex = static_cast<Vertex**>(lua_newuserdata(L, sizeof(Vertex*)));
	* vertex = new Vertex;
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
	{
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
