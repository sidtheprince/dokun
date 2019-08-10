
#include "../include/level.h"

Level::Level() : gravity (9.8), mode(0)
// sky properties
, sky_position(0, 0, 0)
, sky_rotation(0, 0, 0)
, sky_scale(1, 1, 1)
// grid properties
, grid_position(0, -1, -1)
, grid_rotation(0, 0, 0)
, grid_scale(1, 1, 1)
, grid_color(47, 79, 79)
// axis properties
, axis_position(0, 0, 0)
, axis_rotation(0, 0, 0)
, axis_scale(1, 1, 1)
// background properties
, background(nullptr)
// camera + viewing properties
, zoom_factor(1.0)
// light properties
, global_ambient(0.2, 0.2, 0.2, 1.0)
// sound properties
, master_volume(100)
{
	// load default sky box textures from Resource
	sky_texture[0] = (nullptr);
	sky_texture[1] = (nullptr);
	sky_texture[2] = (nullptr);
	sky_texture[3] = (nullptr);
	sky_texture[4] = (nullptr);
	sky_texture[5] = (nullptr);
	
	// init world and set gravity
	world = new b2World(b2Vec2(0.0f, 9.8)); // gravity = 9.81 world->SetGravity()
	/*
	//--------------------------------
	// define ground
	b2BodyDef ground_def;
	ground_def.type = b2_staticBody;
	ground_def.position.Set(0.0f/30, 500.0f/30);
	// make the ground
	ground = world->CreateBody(&ground_def);
	// make the ground fixure
	b2PolygonShape ground_box;
	ground_box.SetAsBox(50.0f/30, 20.0f/30);
	ground->CreateFixture(&ground_box, 0.0f);
	//------------------------------
	*/
	Factory::get_scene_factory()->store(this);
}
/////////////
Level * Level::active (nullptr);
/////////////
/////////////
// DESTRUCTOR
Level::~Level(void)
{
    if(this) destroy();
	Factory::get_scene_factory()->release(this);
}
/////////////
void Level::add(Entity * entity)
{
	if(entity == nullptr)
	    return;
	if(dynamic_cast<Sprite *>(entity)->is_sprite()) 
	{
		Sprite * sprite = static_cast<Sprite *>(entity); // convert entity to sprite
	    b2BodyDef body_def;
        body_def.type          = b2_dynamicBody;
		body_def.position.Set(sprite->get_position().x / 30.0f, sprite->get_position().y / 30.0f);
	    body_def.angle         = sprite->get_angle();
		body_def.userData      = nullptr;//sprite;
		body_def.fixedRotation = true;
		sprite->body = world->CreateBody(&body_def);
		// add rectangular shape (or circle)
		b2PolygonShape shape;
		shape.SetAsBox(sprite->get_width() + 30.0f, sprite->get_height() + 30.0f);
		// add fixture
		b2FixtureDef fixture_def;
        fixture_def.shape       = &shape;
	    fixture_def.userData    = nullptr;
	    fixture_def.restitution = 0.1f;
        fixture_def.density     = 1.0f;
	    fixture_def.isSensor    = false;
		fixture_def.friction    = 0.7f;
		fixture_def.filter.categoryBits = 0x02; // solid=0x02 non-solid=0x04 
		fixture_def.filter.maskBits     = 0xffff;
		sprite->fixture = sprite->body->CreateFixture(&fixture_def);
	}
    
	entity_list.push_back(entity);
}
/////////////
void Level::add(Camera * camera)
{
	//if(camera == nullptr)
	//    return;	
	camera_list.push_back(camera);
}
/////////////
void Level::add(Light * light)
{
	//if(light == nullptr)
	//    return;	
	light_list.push_back(light);
}
/////////////
int Level::add(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	luaL_checktype(L, 3, LUA_TSTRING); // entity, light, camera, etc.
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		std::string type = lua_tostring(L, 3);
		if(String::lower(type) == "entity")
	    {
	        Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));	
	        lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
	            Level * scene = *static_cast<Level **>(lua_touserdata(L, -1));
			    scene->add(entity);
	        }
		    luaL_getsubtable(L, 1, "entity_list");
		#ifdef DOKUN_LUA51
			int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		    int size = luaL_len(L, -1);
		#endif
		    lua_pushvalue(L, 2); // push entity
		    lua_rawseti(L, -2,  size + 1);
		}
		if(String::lower(type) == "camera")
	    {
	        Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));	
	        lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
	            Level * scene = *static_cast<Level **>(lua_touserdata(L, -1));
			    scene->add(camera);
	        }
		    luaL_getsubtable(L, 1, "camera_list");
		#ifdef DOKUN_LUA51
			int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		    int size = luaL_len(L, -1);
		#endif
			lua_pushvalue(L, 2); // push entity
		    lua_rawseti(L, -2,  size + 1);
		}
		if(String::lower(type) == "light")
	    {
	        Light * light = *static_cast<Light **>(lua_touserdata(L, -1));	
	        lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
	            Level * scene = *static_cast<Level **>(lua_touserdata(L, -1));
			    scene->add(light);
	        }
		    luaL_getsubtable(L, 1, "light_list");
		#ifdef DOKUN_LUA51
			int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
		    int size = luaL_len(L, -1);
		#endif
		    lua_pushvalue(L, 2); // push entity
		    lua_rawseti(L, -2,  size + 1);
		}		
	}
    return 0;	
}
/////////////
void Level::remove(Entity * entity)
{
	for(int i = 0; i < entity_list.size(); i++)
	{
		if(entity_list[i] == entity) 
		{	
			entity_list.erase(entity_list.begin() + i);
		}
	}	
}
/////////////
void Level::remove(Camera * camera)
{
	for(int i = 0; i < camera_list.size(); i++)
	{
		if(camera_list[i] == camera) 
		{	
			camera_list.erase(camera_list.begin() + i);
		}
	}	
}
/////////////
void Level::remove(Light * light)
{
	for(int i = 0; i < light_list.size(); i++)
	{
		if(light_list[i] == light) 
		{	
			light_list.erase(light_list.begin() + i);
		}
	}	
}
/////////////
int Level::remove(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	luaL_checktype(L, 3, LUA_TSTRING); // entity, light, camera, etc.
	lua_getfield(L, 2, "udata");
	if(lua_isuserdata(L, -1))
	{
		// determine if entity, light or camera
		std::string type = lua_tostring(L, 3);
		if(String::lower(type) == "entity")
	    {
	        Entity * entity = *static_cast<Entity **>(lua_touserdata(L, -1));	
	        lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
	            Level * scene = *static_cast<Level **>(lua_touserdata(L, -1));
			    scene->remove(entity);
	        }
		    lua_getfield(L, 1, "entity_list");
		#ifdef DOKUN_LUA51
			int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
			int size = luaL_len(L, -1);
		#endif
			for(int i = 1; i <= size; i++)
			{
				lua_rawgeti(L, -1, i); // get table value
			#ifdef DOKUN_LUA51
				if(lua_equal (L, -1, 2))
			#endif
			#ifndef DOKUN_LUA51
			    if(lua_compare (L, -1, 2, LUA_OPEQ)) // check if object matches the object pushed into the stack
			#endif
				{
					lua_pushnil(L);
					lua_rawseti(L, -3, i);
			    }
                lua_pop(L, 1); // pop each object until we find the right object
			}
		}
		if(String::lower(type) == "camera")
	    {
	        Camera * camera = *static_cast<Camera **>(lua_touserdata(L, -1));	
	        lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
	            Level * scene = *static_cast<Level **>(lua_touserdata(L, -1));
			    scene->remove(camera);
	        }
		    lua_getfield(L, 1, "camera_list");
		#ifdef DOKUN_LUA51
			int size = lua_objlen(L, -1);
		#endif
		#ifndef DOKUN_LUA51
			int size = luaL_len(L, -1);
		#endif
			for(int i = 1; i <= size; i++)
			{
				lua_rawgeti(L, -1, i); // get table value
			#ifdef DOKUN_LUA51
				if(lua_equal (L, -1, 2))
			#endif
			#ifndef DOKUN_LUA51
			    if(lua_compare (L, -1, 2, LUA_OPEQ)) // check if object matches the object pushed into the stack
			#endif
				{
					lua_pushnil(L);
					lua_rawseti(L, -3, i);
			    }
                lua_pop(L, 1); // pop each object until we find the right object
			}
		}
		if(String::lower(type) == "light")
	    {
	        Light * light = *static_cast<Light **>(lua_touserdata(L, -1));	
	        lua_getfield(L, 1, "udata");
	        if(lua_isuserdata(L, -1))
	        {
	            Level * scene = *static_cast<Level **>(lua_touserdata(L, -1));
			    scene->remove(light);
	        }
		    lua_getfield(L, 1, "light_list");
			#ifdef DOKUN_LUA51
			int size = lua_objlen(L, -1);
			#endif
			#ifndef DOKUN_LUA51
		    int size = luaL_len(L, -1);
		    #endif
			for(int i = 1; i <= size; i++)
			{
				lua_rawgeti(L, -1, i); // get table value
			    #ifdef DOKUN_LUA51
				if(lua_equal (L, -1, 2))
				#endif
				#ifndef DOKUN_LUA51
			    if(lua_compare (L, -1, 2, LUA_OPEQ)) // check if object matches the object pushed into the stack
			    #endif
			    {
					lua_pushnil(L);
					lua_rawseti(L, -3, i);
			    }
                lua_pop(L, 1); // pop each object until we find the right object
			}
		}		
	}
    return 0;	
}
/////////////
void Level::draw() // draw scene and all objects associated with it
{
	//if(is_active()) // scene is active
	{
	    if(get_background() != nullptr) get_background()->draw(); // for 2d game
	    for(int i = 0; i < camera_list.size(); i++) // for cameras, you can enable or disable
	    {
		    Camera * camera = camera_list[i];
			if(camera != nullptr) camera->draw();
	    }
	    for(int i = 0; i < light_list.size(); i++) // for lights, you can set on or off
	    {
		    Light * light = light_list[i];
			if(light != nullptr) light->draw();
	    }
	    for(int i = 0; i < entity_list.size(); i++) // for entities, you can show or hide
	    {
		    Entity * entity = entity_list[i];
			if(entity != nullptr) entity->draw();
	    }		
	}
}
/////////////
int Level::draw(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * scene = *static_cast<Level **>(lua_touserdata(L, -1));
		scene->draw();
	}	
	return 0;
}
/////////////
void Level::zoom(double zoom_factor)
{
	(this)->zoom_factor = zoom_factor;
}
/////////////
int Level::zoom(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE );
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->zoom ((double)lua_tonumber(L, 2) );
	}      	
	return 0;
}
/////////////
void Level::update(const WINDOW& window)
{
	const_cast<WINDOW &>(window).update();
}
/////////////
/////////////
void Level::update()
{
	const float  update_interval    = 1.0f / 30.0f;
	const double second_per_update  = 0.1;	

	for(int i = 0;i < entity_list.size(); i++) 
	{
		if(dynamic_cast<Sprite *>(entity_list[i])->is_sprite())
	    {
			Sprite * sprite = static_cast<Sprite *>(entity_list[i]);
			//if (sprite->body->GetType() == b2_dynamicBody)
			//sprite->set_position(
			//	(sprite->body->GetPosition().x) / 30.0f, 
			//	(sprite->body->GetPosition().y) / 30.0f);
			//sprite->set_angle(sprite->body->GetAngle() * 180.0f/b2_pi);
			//if(Keyboard::is_pressed(KEY_RIGHT))
            //{
				//sprite->body->SetLinearVelocity(b2Vec2(-1, -10)); // move at constant speed
				//sprite->body->ApplyTorque( 20 , true);
				//sprite->body->ApplyAngularImpulse( 20, true );
                //sprite->body->ApplyForce(sprite->body->GetMass() * world->GetGravity(), sprite->body->GetWorldCenter(), true);//(b2Vec2(0.0f, 50.0f), BodyIterator->GetWorldCenter(), true);
			   //sprite->body->ApplyLinearImpulse( b2Vec2(0,50), sprite->body->GetWorldCenter() );
              //sprite->body->SetTransform( b2Vec2(10,20), 0 );
            //}					
		} // is_sprite
		// Model
		if(dynamic_cast<Model *>(entity_list[i])->is_model())
	    {
			Model * model = static_cast<Model *>(entity_list[i]);
		} // is_model
		// GUI
		if(dynamic_cast<GUI *>(entity_list[i])->is_gui())
	    {
			GUI * gui = static_cast<GUI *>(entity_list[i]);
		}
	} // is_entity	
    world->Step(1 / 30.0f, 8, 3);
}
/////////////
int Level::update(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->update ();
	}      
	return 0;
}
/////////////
void Level::destroy(void)
{
	for(int i = 0; i < entity_list.size(); i++)
	{
		Entity * entity = entity_list[i];
		if(dynamic_cast<Sprite *>(entity)->is_sprite()) 
		{
			b2Body * body = static_cast<Sprite *>(entity)->body;
			if(body) world->DestroyBody(body);
		}
		
		entity->destroy(); // destroy entity
		remove(entity);    // remove entity from level
	}	
}
/////////////
bool Level::load(lua_State * L, const std::string& file_name)
{
	Script script(L, file_name);
	
	std::ifstream file(file_name.c_str());
	if(!file.is_open()) 
	{
		Logger("Could not load level");
		return false;
	}
	std::stringstream stream;
	stream << file.rdbuf(); // dump file contents	
	std::vector<std::string> content = String::split(stream.str(), "\n"); // split each line
	//////
	for(int i = 0; i < content.size(); i++)
	{
		//std::cout << content[i] << "\n";
		if(String::contains(content[i], "entity_"))
		{
			std::string entity_table = String::remove_first_of(String::no_spacing(content[i]), "=");
			if(entity_table != "error")
			{
				std::string parent = script.get_string(L, entity_table + ".parent");
                if(parent == "Sprite")
				{
					Entity * sprite = new Sprite();
					//----------------
					script.get_table(L, entity_table + ".position"); // push table
					lua_rawgeti (L, -1, 1);
					double x = lua_tonumber(L, -1);
					lua_rawgeti (L, -2, 2);
					double y = lua_tonumber(L, -1);
					dynamic_cast<Sprite *>(sprite)->set_position(x, y);
					lua_pop(L, lua_gettop(L) - 1); // pop all but table
					//std::cout << "position : " << Vector2(x, y) << "\n";
					//std::cout <<"stack size after pop: "<< lua_gettop(L) << "\n";
					//----------------
					script.get_table(L, entity_table + ".angle");
					double angle = lua_tonumber(L, -1);
					dynamic_cast<Sprite *>(sprite)->set_angle(angle);
					lua_pop(L, lua_gettop(L) - 1);
					//std::cout << "angle: " << angle << "\n";
					//----------------
					script.get_table(L, entity_table + ".scale");
					lua_rawgeti (L, -1, 1);
					double scale_x = lua_tonumber(L, -1);
					lua_rawgeti (L, -2, 2);
					double scale_y = lua_tonumber(L, -1);
					dynamic_cast<Sprite *>(sprite)->set_scale(scale_x, scale_y);
					lua_pop(L, lua_gettop(L) - 1);
					//std::cout << "scale: " << Vector2(scale_x, scale_y) << "\n";
					//----------------
					Texture * texture = new Texture();
					std::string file = script.get_string(L, entity_table + ".texture");
					if(file.empty())  Logger("Texture is null");
					if(!file.empty()) texture->load(file);
					dynamic_cast<Sprite *>(sprite)->set_texture(*texture);
					lua_pop(L, lua_gettop(L) - 1);
					//std::cout << "texture: " << texture->get_file() << "\n";
					//----------------
					script.get_table(L, entity_table + ".size");
					lua_rawgeti (L, -1, 1);
					int width = lua_tonumber(L, -1);
					lua_rawgeti (L, -2, 2);
					int height = lua_tonumber(L, -1);
					dynamic_cast<Sprite *>(sprite)->set_size(width, height);
					lua_pop(L, lua_gettop(L) - 1);		
                    //std::cout << "size: " << Vector2(width, height) << "\n";					
					//----------------
					script.get_table(L, entity_table + ".color");
					lua_rawgeti (L, -1, 1);
					double red = lua_tonumber(L, -1);
					lua_rawgeti (L, -2, 2);
					double green = lua_tonumber(L, -1);	
					lua_rawgeti (L, -3, 3);
					double blue = lua_tonumber(L, -1);	
					lua_rawgeti (L, -4, 4);
					double alpha = lua_tonumber(L, -1);						
					dynamic_cast<Sprite *>(sprite)->set_color(red, green, blue, alpha);
                    lua_pop(L, lua_gettop(L) - 1);					
					//std::cout << "color: " << Vector4(red, green, blue, alpha) << "\n";	
					//----------------
					//script.get_table(L, entity_table + ".");
					//std::cout << ": " << << "\n";
					//lua_pop(L, lua_gettop(L) - 1);
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					add(sprite);
				}
                if(parent == "Model")
				{
					Entity * model = new Model();
					//----------------
					script.get_table(L, entity_table + ".position");
					lua_rawgeti (L, -1, 1);
					double x = lua_tonumber(L, -1);
					lua_rawgeti (L, -2, 2);
					double y = lua_tonumber(L, -1);
					lua_rawgeti (L, -3, 3);
					double z = lua_tonumber(L, -1);		
					dynamic_cast<Model *>(model)->set_position(x, y, z);
					lua_pop(L, lua_gettop(L) - 1);
					//std::cout << "model_position: " << Vector3(x, y, z) << "\n";
					//----------------
					script.get_table(L, entity_table + ".rotation");
					lua_rawgeti (L, -1, 1);
					double rotate_x = lua_tonumber(L, -1);
					lua_rawgeti (L, -2, 2);
					double rotate_y = lua_tonumber(L, -1);
					lua_rawgeti (L, -3, 3);
					double rotate_z = lua_tonumber(L, -1);						
					dynamic_cast<Model *>(model)->set_rotation(rotate_x, 1, 0, 0);
					dynamic_cast<Model *>(model)->set_rotation(rotate_y, 0, 1, 0);
					dynamic_cast<Model *>(model)->set_rotation(rotate_z, 0, 0, 1);
					lua_pop(L, lua_gettop(L) - 1);
					//std::cout << "rotate: " << Vector3(rotate_x, rotate_y, rotate_z) << "\n";
					//----------------
					script.get_table(L, entity_table + ".scale");
					lua_rawgeti (L, -1, 1);
					double scale_x = lua_tonumber(L, -1);
					lua_rawgeti (L, -2, 2);
					double scale_y = lua_tonumber(L, -1);
					lua_rawgeti (L, -3, 3);
					double scale_z = lua_tonumber(L, -1);				
					dynamic_cast<Model *>(model)->set_scale(scale_x, scale_y, scale_z);
					lua_pop(L, lua_gettop(L) - 1);
					//std::cout << "scale: " << Vector3(scale_x, scale_y, scale_z) << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------	
					add(model);
				}
                if(parent == "GUI")
				{
					Entity * gui = new GUI();
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					//script.get_(L, entity_table + );
					//dynamic_cast< *>()->
					//lua_pop(L, lua_gettop(L) - 1);
					//std::cout << ": " << << "\n";
					//----------------
					add(gui);
				}			
			}
		}
	}
	std::cout <<"stack size: "<< lua_gettop(L) << "\n";
	file.close();
	return true;
}
int Level::load(lua_State *L)
{
	return 1;
}
/////////////
bool Level::save(std::string file_name) // same file or seperate files
{
	// generate file for level
	if(file_name.empty())
	    file_name = "Level" + std::to_string(Factory::get_scene_factory()->get_location(this)) + ".level";
	std::ofstream file(file_name.c_str(), std::ofstream::trunc); // override
    if(!file.is_open())
    {
	    return false;
    }
    // ......
	// for each entity
	for(int i = 0; i < entity_list.size(); i++)
	{		
        std::string fn;
		fn = std::string("entity_" + std::to_string(Factory::get_entity_factory()->get_location(entity_list[i]))).c_str(); // std::string("entity_" + std::to_string(Factory::get_entity_factory()->get_location(entity_list[i])) + ".lua").c_str();
        // .......
		file << "-- ";
		file.write(fn.c_str(), fn.length());
		// .......
		file << "\n" << fn.substr(0, fn.find(".")) << " = \n{\n";	
		// .......
		std::vector<std::pair<std::string, std::string>> data = {};	
		// .......
		Entity * entity = entity_list[i];
		// .......
        if(dynamic_cast<Sprite *>(entity)->is_sprite())
	    {
            Sprite * sprite = static_cast<Sprite *>(entity);			
	        data.push_back(std::make_pair("position", String(static_cast<Sprite *>(entity_list[i])->get_position()).str()));
			data.push_back(std::make_pair("angle   ", String(static_cast<Sprite *>(entity_list[i])->get_angle()).str()));
			data.push_back(std::make_pair("scale   ", String(static_cast<Sprite *>(entity_list[i])->get_scale()).str()));
			if(sprite->get_texture()) // has texture
			data.push_back(std::make_pair("texture ", "\"" + sprite->get_texture()->get_file() + "\""));
			if(!sprite->get_texture()) // no texture
			data.push_back(std::make_pair("texture ", "\"\""));
			data.push_back(std::make_pair("size    ", String(static_cast<Sprite *>(entity_list[i])->get_size()).str()));
			data.push_back(std::make_pair("color   ", String(static_cast<Sprite *>(entity_list[i])->get_color()).str()));
			data.push_back(std::make_pair("parent  ", "\"Sprite\""));
		}
        if(dynamic_cast<Model *>(entity_list[i])->is_model())
	    {
            Model * model = static_cast<Model *>(entity);			
	        data.push_back(std::make_pair("position          ", String(static_cast<Model *>(entity_list[i])->get_position()).str()));
			data.push_back(std::make_pair("rotation          ", String(static_cast<Model *>(entity_list[i])->get_rotation()).str()));
			data.push_back(std::make_pair("scale             ", String(static_cast<Model *>(entity_list[i])->get_scale()).str()));
			data.push_back(std::make_pair("texture           ", "\"\""));
			data.push_back(std::make_pair("size              ", String(static_cast<Model *>(entity_list[i])->get_size()).str()));
		    data.push_back(std::make_pair("material_ambient  " , String(static_cast<Model *>(entity_list[i])->get_material("ambient")).str()) );
		    data.push_back(std::make_pair("material_diffuse  " , String(static_cast<Model *>(entity_list[i])->get_material("diffuse")).str()) );
		    data.push_back(std::make_pair("material_specular ", String(static_cast<Model *>(entity_list[i])->get_material("specular")).str()));
		    data.push_back(std::make_pair("material_emission ", String(static_cast<Model *>(entity_list[i])->get_material("emission")).str()));
		    data.push_back(std::make_pair("material_shininess", String(static_cast<Model *>(entity_list[i])->get_shininess()).str()));
			data.push_back(std::make_pair("parent            ", "\"Model\""));
		}
        if(dynamic_cast<GUI *>(entity_list[i])->is_gui())
	    {
            GUI * gui = static_cast<GUI *>(entity);			
	        data.push_back(std::make_pair("size       ", String(static_cast<GUI *>(entity_list[i])->get_size()).str()));
			data.push_back(std::make_pair("position   ", String(static_cast<GUI *>(entity_list[i])->get_position()).str()));
			data.push_back(std::make_pair("orientation", String(static_cast<GUI *>(entity_list[i])->get_orientation()).str()));
			data.push_back(std::make_pair("parent     ", String(static_cast<GUI *>(entity_list[i])->get_parent()).str()));
			data.push_back(std::make_pair("state      ", ""/*String::to_string(static_cast<GUI *>(entity_list[i])->get_())*/)); // visible, active
			data.push_back(std::make_pair("draggable  ", String(static_cast<GUI *>(entity_list[i])->is_draggable()).str()));
			data.push_back(std::make_pair("droppable  ", String(static_cast<GUI *>(entity_list[i])->is_droppable()).str()));
			data.push_back(std::make_pair("sizeable   ", String(static_cast<GUI *>(entity_list[i])->is_resizeable()).str()));
			data.push_back(std::make_pair("sortable   ", String(static_cast<GUI *>(entity_list[i])->is_sortable()).str()));
            data.push_back(std::make_pair("parent     ", "\"GUI\""));			
		}
        for(int i = 0; i < data.size(); i++)
		{
			std::string key   = data[i].first;
			std::string value = data[i].second;
			if(value.find(" ") == std::string::npos) // single value
			{
			    file << "    " + key + " = " + value << ",\n";
			}
            if(value.find(" ") != std::string::npos) // multiple value
			{
				std::vector<std::string> value_II = String::split(value, " ");
				file << "    "; // four space
				file << key + " = "; // key assign
				file << "{"; // first sub_table bracket
				for(int j =0; j < value_II.size(); j++)
				{
					file << value_II[j] << ",";
				}			
			    file << "},\n"; // last sub_table bracket
			}			
		}	
		// ....
		// components
		for(int i = 0; i < entity_list.size(); i++)
		{
			int num_components = entity_list[i]->get_count("component");
		    for(int j = 0; j < num_components; j++)
		    {
			    std::string value;
			    if(entity_list[i]->get_component(j)->get_type() == 0) value = String::to_string(entity_list[i]->get_component(j)->to_integer());				
			    if(entity_list[i]->get_component(j)->get_type() == 1) value = String::to_string(entity_list[i]->get_component(j)->to_double());
			    if(entity_list[i]->get_component(j)->get_type() == 2) value = "\"" + entity_list[i]->get_component(j)->to_string() + "\"";				
			    if(entity_list[i]->get_component(j)->get_type() == 3) value = String::to_string(entity_list[i]->get_component(j)->to_vector().x) + (", ") + String::to_string(entity_list[i]->get_component(j)->to_vector().y) + (", ") + String::to_string(entity_list[i]->get_component(j)->to_vector().z) + (", ") + String::to_string(entity_list[i]->get_component(j)->to_vector().w);
			    if(entity_list[i]->get_component(j)->get_type() == 4)
			    {
				    if(entity_list[i]->get_component(j)->to_pointer() != nullptr) value = "userdata: " + String::to_string(entity_list[i]->get_component(j)->to_pointer());
				    if(entity_list[i]->get_component(j)->to_pointer() == nullptr) value = "nil"; // nullptr
			    }
			    if(entity_list[i]->get_component(j)->get_type() == 5) value = "function: " + String::to_string(&entity_list[i]->get_component(j)->function); // function
                if(entity_list[i]->get_component(j)->get_type() == 10) // boolean
			    {
				    if(entity_list[i]->get_component(j)->to_boolean() != 0) value = "true";
				    if(entity_list[i]->get_component(j)->to_boolean() == 0) value = "false";
			    }
                if(entity_list[i]->has_component(  entity_list[i]->get_component(j)   ))				
			    file << "    " << entity_list[i]->get_component(j)->get_name() << " = " << value << ",\n";
		    }
		}
		// .......
	    //------------
		file << "}\n\n";
	}
	// for each camera
	for(int i = 0; i < camera_list.size(); i++)
	{
		Camera * camera = camera_list[i];
		std::string token = "camera_" + std::to_string(Factory::get_camera_factory()->get_location(camera)) + " =\n{\n";
		file << token.c_str();
		file << "    eye    = {" + std::to_string(camera->get_position().x) + ", " + std::to_string(camera->get_position().y) + ", " + std::to_string(camera->get_position().z) << ",},\n";// eye
		file << "    center = {" + std::to_string(camera->get_view    ().x) + ", " + std::to_string(camera->get_view    ().y) + ", " + std::to_string(camera->get_view    ().z) << ",},\n"; // center
		file << "    up     = {" + std::to_string(camera->get_up      ().x) + ", " + std::to_string(camera->get_up      ().y) + ", " + std::to_string(camera->get_up      ().z) << ",},\n"; // up vector
		
		file << "}" << "\n\n";
	}
    // for each light
	for(int i = 0; i < light_list.size(); i++)
	{
		Light * light = light_list[i];
		std::string token = "light_" + std::to_string(Factory::get_light_factory()->get_location(light_list[i])) + " =\n{\n";
		file << token.c_str();		
		if(light->get_type() == "directional") // direction
		file << "    mode           = \"Directional\"" << ",\n";
		if(light->get_type() == "positional") // positional
		file << "    mode           =  \"Positional\"" << ",\n";
		if(light->get_type() == "spot") // spot
		file << "    mode           = \"Spot\"" << ",\n";			
		file << "    enabled        =  " + std::to_string(static_cast<int>(light->is_enabled())) << ",\n";
		file << "    ambient        = {" + std::to_string(light->get_ambient().x)  + ", " + std::to_string(light->get_ambient().y)  + ", " +  std::to_string(light->get_ambient().z)  + ", " + std::to_string(light->get_ambient().w)              << ",},\n";
		file << "    diffuse        = {" + std::to_string(light->get_diffuse().x)  + ", " + std::to_string(light->get_diffuse().y)  + ", " +  std::to_string(light->get_diffuse().z)  + ", " + std::to_string(light->get_diffuse().w)              << ",},\n";
		file << "    specular       = {" + std::to_string(light->get_specular().x) + ", " + std::to_string(light->get_specular().y) + ", " +  std::to_string(light->get_specular().z) + ", " + std::to_string(light->get_specular().w)             << ",},\n";
		file << "    position       = {" + std::to_string(light->get_position().x) + ", " + std::to_string(light->get_position().y) + ", " + std::to_string(light->get_position().z)  + ", " + std::to_string(light->get_position().w)             << ",},\n";
		file << "    spot_direction = {" + std::to_string(light->get_spot_direction().x) + ", " + std::to_string(light->get_spot_direction().y) + ", " +  std::to_string(light->get_spot_direction().z)      << ",},\n"; // vec3
		file << "    spot_exponent  =  " + std::to_string(light->get_spot_exponent()) << ",\n";
		file << "    spot_cutoff    =  " + std::to_string(light->get_spot_cutoff())   << ",\n";
		file << "    attenuation    = {" + std::to_string(light->get_attenuation().x) + ", " + std::to_string(light->get_attenuation().y) + ", " + std::to_string(light->get_attenuation().z) << ",},\n";
		
		file << "}" << "\n\n";
	}
    // .......	
	file.close();
	return true;
}
/////////////
int Level::save(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_optstring(L, 2, "");
	std::string file_name = String("Scene0.txt").str();
	if(lua_tostring(L, 2) != nullptr)
		file_name = lua_tostring(L, 2);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->save(file_name.c_str());
	}
    return 0;	
}
/////////////
void Level::draw_sky() // draws a sky box
{
    if(is_active())
    {}
}
/////////////
int Level::draw_sky(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->draw_sky ();
	}
	return 0;
}
/////////////
void Level::draw_grid() // draws a grid
{
    if(is_active())
    {}	
}
/////////////
int Level::draw_grid(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->draw_grid ();
	}    
	return 0;
}
/////////////
void Level::draw_axis() // draws a 3d arrow axis
{
    if(is_active())
    {}	
}
/////////////
int Level::draw_axis(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->draw_axis ();
	}  
    return 0;	
}
/////////////
void Level::show_gui()
{
	for(int i = 0; i < entity_list.size(); i++)
	{
		Entity * entity = entity_list[i];
		if(dynamic_cast<GUI *>(entity)->is_gui())
		{
			static_cast<GUI *>(entity)->show();
		}
	}	
}
/////////////
int Level::show_gui(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->show_gui ();
	}  
    return 0;		
}
/////////////
void Level::hide_gui()
{
	for(int i = 0; i < entity_list.size(); i++)
	{
		Entity * entity = entity_list[i];
		if(dynamic_cast<GUI *>(entity)->is_gui())
		{
			static_cast<GUI *>(entity)->hide();
		}
	}
}
/////////////
int Level::hide_gui(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->hide_gui ();
	}  
    return 0;		
}
/////////////
/////////////
void Level::set_active(bool active) 
{
	if(!active)
	{
		Level::active = (nullptr);
	}
	if(active)
	{
        Level::active = (this);
	}
}
/////////////
int Level::set_active(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->set_active(lua_toboolean(L, 2));
	}
	// SET (Lua)
	lua_pushvalue(L, 1);
	lua_setglobal(L, "DOKUN_ACTIVE_SCENE");
	return 0;
}
/////////////
void Level::set_gravity(double gravity) 
{
    (this)->gravity = gravity;
}
/////////////
int Level::set_gravity(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->set_gravity((double)lua_tonumber(L, 2) );
	}
	return 0;
}
/////////////
/////////////
void Level::set_global_ambient(double red, double green, double blue, double alpha)
{
	global_ambient = Vector4(red, green, blue, alpha);
}	
/////////////
int Level::set_global_ambient(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 1.0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
    {		
        Level * level = *static_cast<Level **>(lua_touserdata(L, -1));	
		level->set_global_ambient(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
	return 0;
}
/////////////
void Level::set_master_volume(int volume)
{
	master_volume = volume;
}
/////////////
int Level::set_master_volume(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
    {		
        Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
        level->set_master_volume((unsigned int)lua_tonumber(L, 2)); 			
	}
	return 0;
}
/////////////
void Level::set_background(const Image& image) // can be an image (2d)
{
	(this)->background = &const_cast<Image&>(image);
}
/////////////
int Level::set_background(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany(L, 2);
	if(lua_istable(L, 2))
	{
		lua_getfield(L, 2, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Image * image = *static_cast<Image **>(lua_touserdata(L, -1));
			lua_getfield(L, 1, "udata");
			if(lua_isuserdata(L, -1))
			{
				Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
				level->set_background(*image);
				// set in lua
				lua_pushvalue(L, 2);
				if(lua_istable(L, -1))
				{
					lua_setfield(L, 1, "background");
				}
			}
		}
	}
	if(lua_isnil(L, 2)) 
	{
		lua_getfield(L, 1, "udata");
		if(lua_isuserdata(L, -1))
		{
		    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
			level->background = (nullptr); // set background to nullptr
		}
		lua_pushvalue(L, 2); // push 2nd arg
		lua_setfield(L, 1, "background"); // set background to nil
	}
	return 0;
}
/////////////
void Level::set_graphics_mode(int mode) // 0 = both(default), 1 = 2D,  2 = 3D
{
	(this)->mode = mode;
}         
/////////////           
int Level::set_graphics_mode(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
    {		
        Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
		level->set_graphics_mode(static_cast<int>(lua_tonumber(L, 2)));
	}	
    return 0;	
}
/////////////
/////////////
// GETTERS
/////////////
Level * Level::get_active() 
{
    return Level::active;
}
/////////////
int Level::get_active(lua_State *L)
{
    lua_getglobal(L, "DOKUN_ACTIVE_SCENE"); // never return a userdata, but a table with a userdata since tables are objects in Lua
	if(lua_istable(L, -1)) 
	{
	    return 1;
	}
    lua_pushnil(L);
	return 1;
}
/////////////
double Level::get_gravity() 
{
    return gravity;
}
/////////////
int Level::get_gravity(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
	    lua_pushnumber(L, static_cast<double>(level->get_gravity()));
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Vector4 Level::get_ambient()
{
	return global_ambient;
}
/////////////
int Level::get_volume()
{
	return master_volume;
}
/////////////
Image * Level::get_background()
{
	return background;
}
/////////////
Entity * Level::get_object(int index) // local object(inside scene)
{
	return entity_list[index];
}
/////////////		
Entity * Level::get_object(std::string name) // local object(inside scene)
{
	if(name.find("Entity") != std::string::npos)
	{
		for(unsigned int i = 0; i < entity_list.size(); i++)
		{
			Entity * entity = entity_list[i];
			if("Entity" + std::to_string(Factory::get_model_factory()->get_location(entity)) == name)
			{
				return entity_list[i];
			}
		}
	}
    return (nullptr);
}
/////////////
int Level::get_object(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // index
    lua_getfield(L, 1, "entity_list");
	if(lua_istable(L, -1))
	{
		lua_rawgeti(L, -1, static_cast<int>(lua_tonumber(L, 2)));
		return 1;
	}	
	lua_pushnil(L);
	return 1;	
}
/////////////
Camera *Level::get_camera(int index) 
{
    return camera_list[index];
}
/////////////
int Level::get_camera(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // index
    lua_getfield(L, 1, "camera_list");
	if(lua_istable(L, -1))
	{
		lua_rawgeti(L, -1, static_cast<int>(lua_tonumber(L, 2)));
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
/////////////
Light * Level::get_light(int index)
{
	return light_list[index];
}
/////////////
int Level::get_light(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER); // index
    lua_getfield(L, 1, "light_list");
	if(lua_istable(L, -1))
	{
		lua_rawgeti(L, -1, static_cast<int>(lua_tonumber(L, 2)));
		return 1;
	}	
	lua_pushnil(L);
	return 1;
}
/////////////
void * Level::get_global(int id, int type) // 0 = sprite, 1 = model, 2 = gui; by default returns a sprite
{
    if (type == 0)
	{
		Sprite * sprite = (Sprite *)Factory::get_sprite_factory()->get_object(id);
		if (sprite)
	        return sprite;
	}
	if (type == 1)
	{
		Model * model = (Model *)Factory::get_model_factory()->get_object(id);
        if (model)
		    return model;	
	}
	if(type == 2)
	{
		GUI * gui = (GUI *)Factory::get_gui_factory()->get_object(id);
		if(gui)
			return gui;
	}
	return (nullptr);
}
/////////////
void * Level::get_global(std::string name)
{
	// object name contains "Entity"
	if (name.find("Entity") != std::string::npos) // can be a sprite, model, ui (you name it!)
	{
		for (int i = 0; i < Factory::get_entity_factory()->get_size(); i++)
	    {
			Entity * entity = static_cast<Entity*>(Factory::get_entity_factory()->get_object(i) );
		    if("Entity" + std::to_string(Factory::get_entity_factory()->get_location(entity) ) == name)
			{
				if(entity)
					return (entity);
			}
		}
	}
	if (name.find("Sprite") != std::string::npos) 
	{
	    for (int i = 0; i < Factory::get_sprite_factory()->get_size(); i++)
	    {
		    Sprite * sprite = static_cast<Sprite*>(Factory::get_sprite_factory()->get_object(i) );
			if("Sprite" + std::to_string(Factory::get_sprite_factory()->get_location(sprite) ) == name )
			{
				if(sprite)
			        return sprite;
			}
	    }
	}
	// object name contains "Model"
	if(name.find("Model") != std::string::npos) 
	{
	    for (int i = 0; i < Factory::get_model_factory()->get_size(); i++)
	    {
		    Model * model = static_cast<Model*>(Factory::get_model_factory()->get_object(i) );
		    if("Model" + std::to_string(Factory::get_model_factory()->get_location(model) ) == name )
			{
				if(model)
			        return model;
			}
	    }		
	}
	if(name.find("GUI") != std::string::npos)
	{
		for(int i = 0; i < Factory::get_gui_factory()->get_size(); i++)
		{
			GUI * gui = static_cast<GUI*>(Factory::get_gui_factory()->get_object(i) );
			if("GUI" + std::to_string(Factory::get_gui_factory()->get_location(gui) ) == name)
			{
				if(gui)
					return gui;
			}
		}
	}
	return (nullptr);
}
/////////////
int Level::get_global(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checkany (L, 2);
	luaL_optnumber(L, 3, 0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		// get the scene
		Level * scene = *static_cast<Level **>(lua_touserdata(L, -1)); 
		
	    if(lua_type(L, 2) == LUA_TNUMBER) // id
	    {
			if (lua_tonumber(L, 3) == 0) // type
			{
		        for (int i = 0; i < Factory::get_sprite_factory()->get_size(); i++)
		        {
					Sprite * sprite = (Sprite *)Factory::get_sprite_factory()->get_object(i);
			        if( Factory::get_sprite_factory()->get_location(sprite) == lua_tonumber(L, 2) )
					{
						#ifdef DOKUN_LUA51
						lua_getglobal(L, "_G");
						#endif
						#ifndef DOKUN_LUA51
                        lua_pushglobaltable(L);       // Get global table
                        #endif
						lua_pushnil(L);               // put a nil key on stack
                        while (lua_next(L,-2) != 0) 
						{ 
						    if(lua_istable(L, -1))
						    {
                                lua_getfield(L, -1, "udata");
								if(lua_isuserdata(L, -1))
								{
									Sprite * usprite = *(Sprite **)lua_touserdata(L, -1);
									if(usprite == sprite)
									{
										lua_pushvalue(L, -2);
										return 1 ;
									}
								}
								lua_pop(L, 1); // pop userdata
                            } 
							lua_pop(L, 1); // pop key
						} 
						lua_pop(L, 1); // pop global table						
					}						
		        }
			}
			if (lua_tonumber(L, 3) == 1) // type
			{
				for (int i = 0; i < Factory::get_model_factory()->get_size(); i++)
				{
					Model * model = (Model *)Factory::get_model_factory()->get_object(i);
					if( Factory::get_model_factory()->get_location(model) == lua_tonumber(L, 2) )
					{
                        #ifdef DOKUN_LUA51
						lua_getglobal(L, "_G");
						#endif
						#ifndef DOKUN_LUA51
                        lua_pushglobaltable(L);       // Get global table
                        #endif
                        lua_pushnil(L);               // put a nil key on stack
                        while (lua_next(L,-2) != 0) 
						{ 
						    if(lua_istable(L, -1))
						    {
                                lua_getfield(L, -1, "udata");
								if(lua_isuserdata(L, -1))
								{
									Model * umodel = *(Model **)lua_touserdata(L, -1);
									if(umodel == model)
									{
										lua_pushvalue(L, -2);
										return 1 ;
									}
								}
								lua_pop(L, 1); // pop userdata
                            } 
							lua_pop(L, 1); // pop key
						} 
						lua_pop(L, 1); // pop global table						
					}
				}
			}
			if (lua_tonumber(L, 3) == 2)
			{
				for (int i = 0; i < Factory::get_gui_factory()->get_size(); i++)
				{
					GUI * gui = (GUI *)Factory::get_gui_factory()->get_object(i);
					if( Factory::get_gui_factory()->get_location( gui ) == lua_tonumber(L, 2) )
					{
                        #ifdef DOKUN_LUA51
						lua_getglobal(L, "_G");
						#endif
						#ifndef DOKUN_LUA51
                        lua_pushglobaltable(L);       // Get global table
                        #endif
                        lua_pushnil(L);               // put a nil key on stack
                        while (lua_next(L,-2) != 0) 
						{ 
						    if(lua_istable(L, -1))
						    {
                                lua_getfield(L, -1, "udata");
								if(lua_isuserdata(L, -1))
								{
									GUI * ugui = *(GUI **)lua_touserdata(L, -1);
									if(ugui == gui)
									{
										lua_pushvalue(L, -2);
										return 1 ;
									}
								}
								lua_pop(L, 1); // pop userdata
                            } 
							lua_pop(L, 1); // pop key
						} 
						lua_pop(L, 1); // pop global table						
					}
				}
			}			
	    }
		// arg 2 is a string
        if(lua_type(L, 2) == LUA_TSTRING)
	    {
		    // Convert object name to string
			std::string object_name = lua_tostring(L, 2);
            // Object is a sprite			
			if (object_name.find("Sprite") != std::string::npos)
			{
		        // scan through sprite factory
		        for (int i = 0; i < Factory::get_sprite_factory()->get_size(); i++)
				{
					Sprite * sprite = (Sprite *)Factory::get_sprite_factory()->get_object(i);
			        if( "Sprite" + std::to_string(Factory::get_sprite_factory()->get_location(sprite)) == object_name )
					{   
                        #ifdef DOKUN_LUA51
						lua_getglobal(L, "_G");
						#endif
						#ifndef DOKUN_LUA51
                        lua_pushglobaltable(L);       // Get global table
                        #endif
                        lua_pushnil(L);               // put a nil key on stack
                        while (lua_next(L,-2) != 0) 
						{ 
						    if(lua_istable(L, -1))
						    {
                                lua_getfield(L, -1, "udata");
								if(lua_isuserdata(L, -1))
								{
									Sprite * usprite = *(Sprite **)lua_touserdata(L, -1);
									if(usprite == sprite)
									{
										lua_pushvalue(L, -2);
										return 1 ;
									}
								}
								lua_pop(L, 1); // pop userdata
                            } 
							lua_pop(L, 1); // pop key
						} 
						lua_pop(L, 1); // pop global table
					}
				}
			}
			//////////
		    // Object is a model
			if (object_name.find("Model") != std::string::npos)
			{
		        // Scan through model factory		
				for (int i = 0; i < Factory::get_model_factory()->get_size(); i++)
				{
					Model * model = (Model *)Factory::get_model_factory()->get_object(i);
					if( "Model" + std::to_string(Factory::get_model_factory()->get_location(model)) == object_name )
					{
                        #ifdef DOKUN_LUA51
						lua_getglobal(L, "_G");
						#endif
						#ifndef DOKUN_LUA51
                        lua_pushglobaltable(L);       // Get global table
                        #endif
                        lua_pushnil(L);               // put a nil key on stack
                        while (lua_next(L,-2) != 0) 
						{ 
						    if(lua_istable(L, -1))
						    {
                                lua_getfield(L, -1, "udata");
								if(lua_isuserdata(L, -1))
								{
									Model * umodel = *(Model **)lua_touserdata(L, -1);
									if(umodel == model)
									{
										lua_pushvalue(L, -2);
										return 1 ;
									}
								}
								lua_pop(L, 1); // pop userdata 
                            } 
							lua_pop(L, 1); // pop key
						} 
						lua_pop(L, 1); // pop global table
					}
				}
			}	
	    }
	}
	lua_pushnil(L);
	return 1;
}
/////////////
int Level::get_count(const std::string& what)
{
    if(String::lower(what).find("entity") != std::string::npos ||
	  String::lower(what).find("entities") != std::string::npos )
	{
		return entity_list.size();
	}	
    if(String::lower(what).find("camera") != std::string::npos)
	{
		return camera_list.size();
	}	
    if(String::lower(what).find("light") != std::string::npos)
	{
		return light_list.size();
	}		
	return -1;
}
/////////////
/////////////
// BOOLEAN 
/////////////
bool Level::is_active()
{
    return (get_active() == (this));
}
/////////////
int Level::is_active(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Level * level = *static_cast<Level **>(lua_touserdata(L, -1));
	    lua_pushboolean(L, level->is_active() );
        return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}
/////////////
/////////////
int Level::new_(lua_State *L)
{
    // Clear all unneccary top args. just in case Scene:new() is used ; pops the Scene table
    lua_settop(L, 0);
	// Create table.
	lua_createtable(L, 0, 0);
	// Set metatable.
	lua_getglobal(L, "Level");
	lua_setmetatable(L, 1);
	// Set userdata.
	Level **level = static_cast<Level **>(lua_newuserdata(L, sizeof(Level*)));
	*level = new Level();
	lua_setfield(L, 1, "udata");
	// Return table.
	if(lua_istable(L, -1)) 
	{
	    return 1;
	}
	lua_pushnil(L);
	return 1;
}
Shader * Level::scene_vertex_shader (new Shader());
Shader * Level::scene_fragment_shader (new Shader());
