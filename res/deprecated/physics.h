#ifndef _PHYSICS
#define _PHYSICS

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#include "vector.h" // vector math
#include "matrix.h"
#include "logger.h"
#ifdef __cplusplus  // if c++
//extern "C" {      // run as c code
#endif
    #include <Box2D/Box2D.h>            // 2d physics (box2D)
    //#include <btBulletDynamicsCommon.h> // 3d physics (bullet3)
#ifdef __cplusplus
//}
#endif

#ifdef __cplusplus
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <lua.hpp>



class p_Engine // static (no need for constructors)
{
public:
    static void start();
    static void do_something();
	
	static double meter_to_pixel(); // 60
	static double pixel_to_meter(); // 1 / 60
	
	static void set_rect(double x, double y, int width, int height, double angle = 0.0);
protected:
	static b2World * world; // creates and deletes obj; The boss of everything
	static b2Body  * ground;
};








/*
static void b2_hello_world()
{
    //B2_NOT_USED(argc);
	//B2_NOT_USED(argv);

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}

	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.
	
	
}

static void b3_hello_world()
{
	///-----includes_end-----

	int i;
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0,-10,0));

	///-----initialization_end-----

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;


	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,-56,0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}


	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

			startTransform.setOrigin(btVector3(2,10,0));
		
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(body);
	}



/// Do some simulation


	///-----stepsimulation_start-----
	for (i=0;i<150;i++)
	{
		dynamicsWorld->stepSimulation(1.f/60.f,10);
		
		//print positions of all objects
		for (int j=dynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);

			} else
			{
				trans = obj->getWorldTransform();
			}
			printf("world pos object %d = %f,%f,%f\n",j,float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
		}
	}

	///-----stepsimulation_end-----

	//cleanup in the reverse order of creation/initialization
	
	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<collisionShapes.size();j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();	
}
*/




struct AABB;
struct Circle;
struct Object;
/////////////
class Physics 
{
    public:
	    Physics();
		~Physics();
		
		static bool aabb_aabb_collide(AABB a, AABB b);
		static bool circle_circle_collide_optimized(Circle a, Circle b);
		static bool circle_circle_collide_unoptimized(Circle a, Circle b);
		static void resolve_collision(Object a, Object b);
        // tools and utilities
		static double distance(Vector2 a, Vector2 b);
		static double distance(Vector3 a, Vector3 b);

	private:
};
/////////////
struct AABB
{
	Vector3 min;
    Vector3 max;
};
/////////////
struct Circle
{
    double radius;
    Vector3 position;
};
/////////////
struct Object
{
	double restitution; // elasticity, bounciness
	Vector3 velocity; // speed
	double mass; /* in kg*/ // heaviness
};
/////////////
/////////////
class Ray // Ray casting - part of physics
{ 
    public:
        Ray();
	    Ray(Vector3 origin, Vector3 direction);
	    ~Ray();
	
	// normal
	// setters
	// getters
	    Vector3 get_origin(); // 
	//Vector3 get_position(); // get the position of the ray
	    Vector3 get_direction(); // direction ray is pointing at
	    Vector3 get_RGB(); // returns color the ray is pointing to
	
	    double get_distance();
		
		static void mouse_click_callback ();
		void mouse_click_callback (int b, int s, int mouse_x, int mouse_y);
		
		static Ray * get_default();
	//private:
	    void * hit; // anything hit by ray
		
		Vector3 ray_nds; // eye point
		Vector4 ray_clip; // view dir
		Vector4 ray_eye;
		Vector3 ray_wor; // x, y
		
		static Ray *d_ray;
};
/*
// Normalised Device Coordinates
float x = (2.0f * mouse_x) / width - 1.0f;
float y = 1.0f - (2.0f * mouse_y) / height;
float z = 1.0f;
vec3 ray_nds = vec3 (x, y, z);
// homogeneous Clip Coordinates
vec4 ray_clip = vec4 (ray_nds.xy, -1.0, 1.0);
// camera
vec4 ray_eye = inverse (projection_matrix) * ray_clip;
ray_eye = vec4 (ray_eye.xy, -1.0, 0.0);
// 4d world
vec3 ray_wor = (inverse (view_matrix) * ray_eye).xyz;
ray_wor = normalise (ray_wor); // don't forget to normalise the vector at some point

*/
/////////////
#endif
#endif