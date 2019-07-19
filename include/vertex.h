#ifndef _VERTEX
#define _VERTEX

#include "platform.h"
#include "vector.h"
#include "shader.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <vector>

class Vertex // represents a single point with x, y, z properties (good for particle system)
{
	public:
	    Vertex(void);               static int new_(lua_State *L);
	    Vertex(const Vertex &vertex); // copy
		Vertex(double x, double y);
		Vertex(double x, double y, double z);
		Vertex(double x, double y, double z, double w);
	    Vertex(const Vector2 &vector);
	    Vertex(const Vector3 &vector);
	    Vertex(const Vector4 &vector);
		Vertex(const double (&array)[2]);
		Vertex(const double (&array)[3]);
		Vertex(const double (&array)[4]);
		// multiple vertices
		Vertex(std::vector<Vector2>& vertex_list);
		Vertex(std::vector<Vector3>& vertex_list);
		Vertex(std::vector<Vector4>& vertex_list);
	    ~Vertex(void);
		// normal functions
		void draw();     static int draw(lua_State *L);
		// number
		void draw2(double x, double y); 
		void draw3(double x, double y, double z);
		void draw4(double x, double y, double z, double w);
		// vector
		void draw2v(const Vector2 &vector);
		void draw3v(const Vector3 &vector);
		void draw4v(const Vector4 &vector);
		// array
		void draw2a(const double (&array)[2]); 
		void draw3a(const double (&array)[3]);
		void draw4a(const double (&array)[4]);
        // draw multiple vertices to create an object
		void draw_object(unsigned int mode = 0);  static int draw_object(lua_State *L);
		// Vertex cube; cube.add(0, 0, 1); cube.add(1, 0.9, 0); cube.draw_object();
		void add(const Vector2& vertex);       static int add(lua_State *L);
		void add(const Vector3& vertex);		
		void add(const Vector4& vertex);
		// transforms
		void translate(double x, double y, double z, double w = 1);
		void rotate(double degrees);
		void scale(double sx, double sy, double sz);
		void shear(double shx, double shy, double sz);
		void reflect(int x, int y);
		// setters
		void set_position(double x, double y); static int set_position(lua_State *L);
		void set_position(double x, double y, double z);
		void set_position(double x, double y, double z, double w);
		// getters
		Vector4 get_position(); static int get_position(lua_State *L);
		unsigned int size(); static int size(lua_State *L);
		/////////////
		Vector4 operator [] (unsigned int index) const
		{
			return object[index]; 
		}
		/////////////
		friend std::ostream& operator << (std::ostream& os, const Vertex& vertex)
		{
			os 
			<< vertex.x << " "
			<< vertex.y << " "
			<< vertex.z << " "
			<< vertex.w << "\n";
			return os;
		}		
	    /////////////	
	    // vertex position
	    double x;
	    double y;
	    double z;
	    double w;
		// an object, containing many vertices
	    std::vector<Vector4> object; 
		// Graphics API values
		#ifdef DOKUN_OPENGL
		    GLuint vertex_buffer;
		#endif
};
		/* USAGE:
		Vertex cube; // a cube with 8 points
		cube.add( Vector3(1.000000, -1.000000, -1.000000) );
		cube.add( Vector3(1.000000, -1.000000,  1.000000) );
		cube.add( Vector3(-1.000000, -1.000000,  1.000000) );
		cube.add( Vector3(-1.000000, -1.000000, -1.000000) );
		
		cube.add( Vector3(1.000000,  1.000000, -0.999999) );
		cube.add( Vector3(0.999999,  1.000000,  1.000001) );
		cube.add( Vector3(-1.000000,  1.000000,  1.000000) );
		cube.add( Vector3(-1.000000,  1.000000, -1.000000) );
		
		cube.draw_mul();
		*/
#endif		
#endif