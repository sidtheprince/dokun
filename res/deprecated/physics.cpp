#include "../include/physics.h"

////////////
b2World * p_Engine::world (nullptr); // creates and deletes obj; The boss of everything
b2Body  * p_Engine::ground(nullptr);
////////////	
void p_Engine::start()
{
	world = new b2World(b2Vec2(0.0, 9.81)); // gravity = 9.81
}
////////////
////////////	
////////////	
//ghjghj
void p_Engine::set_rect(double x, double y, int width, int height, double angle)
{
	// Body def
	b2BodyDef body_def;
    body_def.type               = b2_dynamicBody;	
	body_def.position.Set(x * (1/60), y * (1/60));
	body_def.angle              = 0;
	// Polygon shape
	b2PolygonShape box_shape;
    box_shape.SetAsBox(width / 2, height / 2);
	// Fixure def
	b2FixtureDef fixture_def;
    fixture_def.shape       = &box_shape;
	fixture_def.userData    = nullptr;
	fixture_def.restitution = 0.0f;
    fixture_def.density     = 1.0f; // default: 0.0f;
	fixture_def.isSensor    = false;
	// Dynamic body
	b2Body* dynamic_body = world->CreateBody(&body_def);
    dynamic_body->CreateFixture(&fixture_def);	
}
////////////	
void p_Engine::do_something()
{
	b2BodyDef myBodyDef;              // properties of an object (mass, velocity, angle, etc.)
    myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
    myBodyDef.position.Set(0, 20);   //set the starting position
    myBodyDef.angle = 0;             //set the starting angle
	// Dynamic body
	b2Body* dynamicBody = world->CreateBody(&myBodyDef);
	
    b2PolygonShape boxShape;
    boxShape.SetAsBox(1,1);
  
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    dynamicBody->CreateFixture(&boxFixtureDef);	
	// Set dynamic body properties
	dynamicBody->SetTransform( b2Vec2( 10, 20 ), 1 );
	dynamicBody->SetTransform( b2Vec2( 10, 20 ), 45 * DEGTORAD );
	dynamicBody->SetAngularVelocity( -90 * DEGTORAD );
	// Static body
	myBodyDef.type = b2_staticBody; //this will be a static body
    myBodyDef.position.Set(0, 10); //slightly lower position
    b2Body* staticBody = world->CreateBody(&myBodyDef); //add body to world
    staticBody->CreateFixture(&boxFixtureDef); //add fixture to body
	// Kinematic body
    myBodyDef.type = b2_kinematicBody; //this will be a kinematic body
    myBodyDef.position.Set(-18, 11); // start from left side, slightly above the static body
    b2Body* kinematicBody = world->CreateBody(&myBodyDef); //add body to world
    kinematicBody->CreateFixture(&boxFixtureDef); //add fixture to body
    kinematicBody->SetLinearVelocity( b2Vec2( 1, 0 ) ); //move right 1 unit per second
    kinematicBody->SetAngularVelocity( 360 * DEGTORAD ); //1 turn per second counter-clockwise

for (b2Body* BodyIterator = world->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
        if (BodyIterator->GetType() == b2_dynamicBody)
        {}	
	}		
	
   // Clean up
   world->DestroyBody(dynamicBody);   
}


















































Ray::Ray()
{}
////////
Ray::Ray(Vector3 origin, Vector3 direction)
{}
////////
Ray::~Ray()
{}
////////
Ray * Ray::d_ray (nullptr);
////////
void Ray::mouse_click_callback ()
{

}

Ray * Ray::get_default()
{
	return (d_ray);
}






Physics::Physics()
{


}
/////////////
Physics::~Physics(void)
{}
// NORMAL
/////////////
bool Physics::aabb_aabb_collide(AABB a, AABB b) // no rotation
{
    if(a.max.x < b.min.x || a.min.x > b.max.x) return false; // max = position, min = size
    if(a.max.y < b.min.y || a.min.y > b.max.y) return false;
    return true;
}
/////////////
bool Physics::circle_circle_collide_optimized(Circle a, Circle b)
{
    double r = a.radius + b.radius;
    r = r * r;
    return r < pow(a.position.x + b.position.x, 2) + pow(a.position.y + b.position.y, 2);
}
/////////////
bool Physics::circle_circle_collide_unoptimized(Circle a, Circle b)
{
    double r = a.radius + b.radius;
    return r < distance(a.position, b.position);
}
/////////////
void Physics::resolve_collision(Object a, Object b)
{
    // Calculate relative velocity
	Vector2 a_vel = Vector2(a.velocity.x, a.velocity.y);
	Vector2 b_vel = Vector2(b.velocity.x, b.velocity.y);
	
    Vector2 rv = b_vel - a_vel;
	// Calculate relative velocity in terms of the normal direction
    float velAlongNormal = rv.dot(rv.normal());
    // Do not resolve if velocities are separating
    if(velAlongNormal > 0)
        return;	
    // Calculate restitution
    float e ;//= min( a.restitution, b.restitution);
    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= 1 / a.mass + 1 / b.mass;
  
    /*
    // Apply impulse
    Vector2 impulse = j * normal;
    a.velocity -= 1 / a.mass * impulse
    b.velocity += 1 / b.mass * impulse
	*/
}
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////



// SETTERS
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
 

// GETTERS
/////////////
//double Physics::() {}
double Physics::distance(Vector2 a, Vector2 b) 
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
/////////////
double Physics::distance(Vector3 a, Vector3 b) 
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)); 
}
/*

bool Sprite::collide(Sprite * sprite) // collision works in local coordinates
{  
	if((x < sprite->x + sprite->get_size().x) && (x + get_size().x > sprite->x) &&
        (y < sprite->y + sprite->get_size().y) && (get_size().y + y > sprite->y))
	{
    	return true;
	}
	return false;
}











*/
/*
/////////////
double Physics::force(double m, double a) // without object
{
    double F = m * a;
	return F;
} 
/////////////
double Physics::force(Model &model, double a) // get force of an object
{
    double F;
	double m = model.get_mass();
	//double a = a;//model.get_velocity(); 
	F =  m * a;
	return F;
}
/////////////
double Physics::force(Sprite &b, double a) // get force of an object
{
    double F;
	double m = b.get_mass();
	F =  m * a;
	return F;
}
/////////////
int Physics::force(lua_State *L) 
{
    
    luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1)) 
	{
	    Model **model = (Model **)lua_touserdata(L, -1);
	    lua_pushnumber(L, Physics::force(**model, lua_tonumber(L, 2)));
	    return 1;
	}
}
/////////////
double Physics::force_between_objects(Vector2 &a, Vector2 &b) 
{
   
}
double Physics::force_between_objects(Vector3 &a, Vector3 &b) {}
double Physics::force_between_objects(Model &a, Model &b) 
{
    double m1 = a.get_mass();
	double m2 = b.get_mass();
	double d = distance(a.get_position(), b.get_position());
}
*/
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////

// BOOLEAN
/*
/////////////
// bool Physics::(){}
bool Physics::collision(Sprite &a, Sprite &b){}
/////////////
bool Physics::collision(Model &a, Model &b){}
/////////////
int Physics::collision(lua_State *L)
{
    return 1;
}
/////////////
*/
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
/////////////
