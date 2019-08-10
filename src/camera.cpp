#include "../include/camera.h"

Camera::Camera() : position(Vector3(0.0, 0.0, 0.0)), view(Vector3(0.0, 0.0,-1.0)), up_vector(Vector3(0.0, 1.0, 0.0)), right_vector(Vector3(1.0, 0.0, 0.0)), 
    ortho(0.0, 800.0 + zoom_factor, 600.0 + zoom_factor, 0.0), near(-1.0), far(1.0), zoom_factor(0.0) // you should not have to change viewport to zoom, instead use ortho2d for zooming purposes
{  
    rotation.x = rotation.y = rotation.z = 0.0; // pitch, yaw, row (x, y, z)
	Factory::get_camera_factory()->store(this);
}
//////////////
Camera::~Camera(void)
{
	Factory::get_camera_factory()->release(this);
}
//////////////
Camera *Camera::c_default = nullptr;
//////////////
void Camera::move(double x, double y, double z)
{
    position = Vector3(position.x+x, position.y+y, position.z+z);
}
//////////////
void Camera::move(const Vector3& direction)
{
    position = position + direction;
}
//////////////
int Camera::move(lua_State *L)
{
   luaL_checktype(L, 1, LUA_TTABLE);
   luaL_checktype(L, 2, LUA_TNUMBER);
   luaL_checktype(L, 3, LUA_TNUMBER);
   luaL_optnumber(L, 4, 0);
   lua_getfield(L, 1, "udata");
   if(lua_isuserdata(L, -1)) 
   {
        Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->move(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
   }
   return 0;
}
//////////////
void Camera::rotate(double angle, double x, double y, double z)
{
	angle = ((int)angle + 360) % 360; // 0-360
	if(x > 0) 
	{
        rotation.x += angle;
        view.set( Vector3::normal(view.mul(cos(angle *(3.14159265/180))) + up_vector.mul(sin(angle *((3.14159265/180))))) );
		up_vector = Vector3::cross(view, right_vector) * -1;
	}
	if(y > 0)
	{
	    rotation.y += angle;
	    view.set( Vector3::normal(view * cos(angle * (3.14159265/180)) - right_vector * sin(angle * (3.14159265/180))) );
	    right_vector = ( Vector3::cross(view, up_vector) );
	}
	if(z > 0)
	{
	    rotation.z += angle;
	    right_vector.set( Vector3::normal(right_vector * cos(angle * (3.14159265/180)) + up_vector * sin(angle * (3.14159265/180))) );
	    up_vector = ( Vector3::cross(view, right_vector) * -1 );
	} 
}
//////////////
void Camera::rotate(Camera &camera, double angle, double x, double y, double z) 
{
    camera.rotate(angle, x, y, z);
}
//////////////
int Camera::rotate(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->rotate(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;
}
//////////////
void Camera::zoom(double zoom) // works! tested! 7-15-2019
{
	set_zoom(zoom);//to zoom in and out, change "right" and "bottom" in Ortho2d (NOTE: This is for 2d)
}
//////////////
int Camera::zoom(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
	    camera->zoom(lua_tonumber(L, 2));	
	}
	return 0;
}
//////////////
void Camera::zoom_in(double zoom) // works! tested! 7-15-2019
{
    set_zoom(get_zoom() - zoom); // zoom_in
}
int Camera::zoom_in(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
	    camera->zoom_in(lua_tonumber(L, 2));	
	}
	return 0;
}
//////////////
void Camera::zoom_out(double zoom) // works! tested! 7-15-2019
{
    set_zoom(get_zoom() + zoom); // zoom_out
}
int Camera::zoom_out(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
	    camera->zoom_out(lua_tonumber(L, 2));	
	}
	return 0;
}
//////////////
void Camera::look(double x, double y, double z) 
{
    view = Vector3(x, y, z);
}
//////////////
void Camera::look(const Vector3& target)
{
    look(target.x, target.y, target.z);
}
//////////////
//////////////
int Camera::look(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	if(! lua_istable(L, 2)) 
	{
	    luaL_checktype(L, 2, LUA_TNUMBER);
	    luaL_checktype(L, 3, LUA_TNUMBER);
	    luaL_checktype(L, 4, LUA_TNUMBER);
	}
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
	    camera->look(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));	
	}
	return 0;
}
//////////////
/*
void Camera::follow(Sprite *sprite) 
{
    position = Vector3(sprite->get_position().x, sprite->get_position().y, -1.0);
}
//////////////
void Camera::follow(Model *model)
{  
    position = model->get_position() + Vector3(0, 0, zoom_factor);                                            //Vector3(20*cos(model->angle), 10, 20*sin(model->angle));//Vector3(zoomf);// + Vector3(20*cos(model->angle), 20, 20*sin(model->angle));// camera, keep a few distance from object                           + Vector3(20*cos(model->angle), 10, 20*sin(model->angle));
}
//////////////
int Camera::follow(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // camera object
	luaL_checktype(L, 2, LUA_TTABLE); // sprite or model object
    lua_getfield(L, 2, "udata");
    if(lua_isuserdata(L, -1))
    {
        Sprite * sprite = *static_cast<Sprite **>(lua_touserdata(L, -1));
        lua_getfield(L, 1, "udata");
        if(lua_isuserdata(L, -1))
        {
            Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
            camera->follow(sprite);
        }
    }
	return 0;
}
*/
//////////////
void Camera::draw() 
{
	if(is_enabled()) // camera is enabled
	{
		//Vector3 view_p = position + view;
	    // draw ..
		/*
		Renderer::draw_camera(position.x, position.y, position.z,
		    view_p.x, view_p.x, view_p.x,
			up_vector.x, up_vector.y, up_vector.z);
		*/
	}
}
//////////////
int Camera::draw(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata") ;
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->draw();
	}
	return 0;
}
//////////////
void Camera::move_forward(double distance) // moves camera foward, towards where camera is facing.
{
	position = get_position() + (view.mul( -distance ));
}
//////////////
void Camera::move_upward(double distance) 
{
	position = get_position() + (up_vector.mul( distance ));
}
//////////////
void Camera::strafe(double distance) // strafe left or right
{
	position = get_position() + (right_vector.mul( distance ));
}
//////////////
int Camera::strafe(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->strafe((double)lua_tonumber(L, 2));
	}
	return 0;
}
//////////////
//////////////
// SETTERS
//////////////
void Camera::set_enabled(bool enable)
{
	(this)->enabled = enable;
}
//////////////
int Camera::set_enabled(lua_State *L)
{
	return 0;
}
//////////////
void Camera::set_default()
{
    Camera::c_default = this;
}
//////////////
void Camera::set_default(Camera &camera) 
{
	camera.set_default();
}
//////////////
int Camera::set_default(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); 
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_default();
		// SET DEFAULT CAMERA IN LUA
		lua_pushvalue(L, 1);
		lua_setglobal(L, "c_default");

	}
	return 0;
}
//////////////
//////////////
void Camera::set_eye(double x, double y, double z)
{
	position = Vector3(x, y, z);
}
//////////////
void Camera::set_eye(const Vector3& eye)
{
	set_eye(eye.x, eye.y, eye.z);
}
//////////////
int Camera::set_eye(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata") ; // 2
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_eye(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	}
	return 0;	
}
//////////////
void Camera::set_center(double x, double y, double z)
{
	view = Vector3(x, y, z);
}
//////////////
void Camera::set_center(const Vector3& center)
{
	set_center(center.x, center.y, center.z);
}
//////////////
int Camera::set_center(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata") ; // 2
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_center(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	}
	return 0;	
}
//////////////
void Camera::set_up(double x, double y, double z)
{
    up_vector = Vector3(x, y, z);
}
//////////////
void Camera::set_up(const Vector3& up)
{
    set_up(up.x, up.y, up.z);
}
//////////////
int Camera::set_up(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata") ; // 2
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_up( lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	}
	return 0;
}
//////////////
void Camera::set_right(double x, double y, double z)
{
    right_vector = Vector3(x, y, z);
}
//////////////
void Camera::set_right(const Vector3& right)
{
    set_right(right.x, right.y, right.z);
}
//////////////
int Camera::set_right(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	
	lua_getfield(L, 1, "udata") ; // 2
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_right(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	}
	return 0;
}
//////////////
//////////////
//////////////
//////////////
//////////////
void Camera::set_position(double x, double y, double z) 
{
    set_eye(x, y, z);
}
//////////////
void Camera::set_position(const Vector3& p)
{
    set_position(p.x, p.y, p.z);
}
//////////////
int Camera::set_position(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata") ; // 2
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_position(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	}
	return 0;
}
//////////////
void Camera::set_view(double x, double y, double z) 
{
    set_center(x, y, z);
}
//////////////
void Camera::set_view(const Vector3& view) 
{
    set_view(view.x, view.y, view.z);
}
//////////////
int Camera::set_view(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	lua_getfield(L, 1, "udata") ; // 2
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_view( lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	}
	return 0;
}
//////////////
// 2D
//////////////
void Camera::set_ortho(double left,
 	double right,
 	double bottom,
 	double top,
 	double near,
 	double far)
{
	ortho = Vector4(left, right, bottom, top);
	(this)->near = near;
	(this)->far  = far ;
}	
int Camera::set_ortho(lua_State * L)
{
	luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TNUMBER);
	luaL_checktype(L, 6, LUA_TNUMBER);
	luaL_checktype(L, 7, LUA_TNUMBER);	
    lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_ortho(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4),lua_tonumber(L, 5),lua_tonumber(L, 6),lua_tonumber(L, 7));
	}		
	return 0;
}
//////////////
//////////////
// GETTERS
//////////////
Camera *Camera::get_default()
{
    return Camera::c_default;
}
//////////////
int Camera::get_default(lua_State *L)
{
    lua_getglobal(L, "c_default"); // never return a userdata, but a table with a userdata since tables are objects
	if(lua_istable(L, -1)) 
	{
	    return 1;
	}
    lua_pushnil(L);
	return 1;
}
//////////////
Vector3 Camera::get_position() const
{
    return Vector3(position.x, position.y, position.z);
}
//////////////
int Camera::get_position(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_position().x);
		lua_pushnumber(L, camera->get_position().y);
		lua_pushnumber(L, camera->get_position().z);
		return 3;
	}
	lua_pushnil(L);
	return 1;
}
//////////////
Vector3 Camera::get_view() const
{
    return Vector3(view.x, view.y, view.z);
}
//////////////
int Camera::get_view(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_view().x);
		lua_pushnumber(L, camera->get_view().y);
		lua_pushnumber(L, camera->get_view().z);
		return 3;
	}
	lua_pushnil(L);
	return 1;
}
//////////////
//////////////
//////////////
//////////////
//////////////
//////////////
Vector3 Camera::get_eye   () const
{
	return position;
}
int Camera::get_eye(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera =*static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_eye().x);
		lua_pushnumber(L, camera->get_eye().y);
		lua_pushnumber(L, camera->get_eye().z);
		return 3;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;	
}
//////////////
Vector3 Camera::get_center() const
{
	return view;
}
int Camera::get_center(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera =*static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_center().x);
		lua_pushnumber(L, camera->get_center().y);
		lua_pushnumber(L, camera->get_center().z);
		return 3;
	}
	lua_pushnil(L);
	lua_pushnil(L);
	lua_pushnil(L);
	return 3;	
}
//////////////
Vector3 Camera::get_up() const
{
    return up_vector;
}
//////////////
int Camera::get_up(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera =*static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_up().x);
		lua_pushnumber(L, camera->get_up().y);
		lua_pushnumber(L, camera->get_up().z);
		return 3;
	}
	lua_pushnil(L);
	return 1;
}
//////////////
Vector3 Camera::get_right() const
{
    return right_vector;
}
//////////////
int Camera::get_right(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_right().x);
		lua_pushnumber(L, camera->get_right().y);
		lua_pushnumber(L, camera->get_right().z);
		return 3;
	}
	lua_pushnil(L);
	return 1;
}
//////////////
//////////////
//////////////
double Camera::get_zoom()const
{
    return zoom_factor;
}
//////////////
int Camera::get_zoom(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_zoom());
		return 1;

    }
    lua_pushnil(L);
 	return 1;	
}
//////////////
Vector3 Camera::get_rotation() const
{
    return rotation;
}
//////////////
int Camera::get_rotation(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		lua_pushnumber(L, camera->get_rotation().x);
		lua_pushnumber(L, camera->get_rotation().y);
		lua_pushnumber(L, camera->get_rotation().z);
		return 3;
	}
	lua_pushnil(L);
	return 1;
}
//////////////
// 2D
//////////////
void Camera::set_zoom(double zoom)
{
    zoom_factor = zoom;
}
int Camera::set_zoom(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE); // 1
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata") ; // 2
	if(lua_isuserdata(L, -1)) 
	{    
	    Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));
		camera->set_zoom(lua_tonumber(L, 2));
	}
	return 0;
}
//////////////
double Camera::get_left()const
{
	return ortho.x;
}
double Camera::get_right2D()const
{
	return ortho.y;
}
double Camera::get_bottom()const
{
	return ortho.z;
}
double Camera::get_top()const
{
	return ortho.w;
}
double Camera::get_near()const
{
	return near;
}
double Camera::get_far()const
{
	return far;
}
Vector4 Camera::get_ortho()const
{
	return ortho;
}
//////////////
//////////////
bool Camera::is_enabled()
{
	return (enabled == true);
}
//////////////
int Camera::is_enabled(lua_State *L)
{
	return 0;
}
//////////////
int Camera::new_(lua_State *L)
{
    lua_settop(L, 0); // clear any value pushed on top
	// create table (object)
	lua_createtable(L, 0, 0); // #1
    // set mt
	lua_getglobal(L, "Camera");
	lua_setmetatable(L, 1);
	// set userdata
	Camera **camera = static_cast<Camera**>(lua_newuserdata(L, sizeof(Camera*)));
	*camera = new Camera();
    lua_setfield(L, 1, "udata");
	// return camera
	if(lua_istable(L, -1))
	    return 1;
	lua_pushnil(L); // if not table
	return 1; // return nil
}
/////////////
// eoc = end of code
