#ifndef _QUATERNION
#define _QUATERNION

#include "matrix_test.h"

#ifdef __cplusplus
#include <iostream>
#include <cmath>
#include <lua.hpp>

class Quaternion
{
    Quaternion(void) : w(0), x(0), y(0), z(0) {}
	Quaternion(double w, double x, double y, double z) // based on element passed
	{
		(this)-> w = w ;

		(this)-> x = x ;
		(this)-> y = y ;
		(this)-> z = z ;
	}
	Quaternion(const Quaternion & q)
	{
		w = q.w ;

		x = q.x ;
		y = q.y ;
		z = q.z ;
	}
	//Quaternion() {}
	//Quaternion() {}
	Quaternion(const Vector2 & vector, double z, double w)
	{
		x = vector.x ;
		y = vector.y ;
		(this)->z = z ;
		(this)->w = w ;
	}
	Quaternion(const Vector3 & vector, double w)
	{
		x = vector.x ;
		y = vector.y ;
		z = vector.z ;
		(this)->w = w ;
	}
	Quaternion(const Vector4 & vector)
	{
		x = vector.x ;
		y = vector.y ;
		z = vector.z ;
		w = vector.w ;
	}
    Quaternion(const double* array)
	{
		//if(!array)
			//std::cerr << "(ERROR): Invalid array" << std::endl;
        x = array[0];
		y = array[1];
		z = array[2];
		w = array[3];

    }
	~Quaternion(void){}
    //////////
	double real()
	    { return w ; }
	Vector3 complex()
	{ return Vector3(x, y, z);}
	Quaternion conjugate()
	{
        return Quaternion(w, -x, -y, -z);
	}
    double length()
	{
		return sqrt(w * w + x * x + y * y + z * z);
	}
	Quaternion normalize()
	{
		return Quaternion
		(
		    w / length(),
		    x / length(),
			y / length(),
			z / length()
		);
 	}
	Quaternion magnitude()
	{
		return Quaternion
		(
		    sqrt(normalize().w),
			sqrt(normalize().x),
			sqrt(normalize().y),
			sqrt(normalize().z)
		);
	}
    Quaternion scale(double s) // scalar multiplication
	{
        return Quaternion( w * s, x * s, y * s, z * s);
    }
	Quaternion inverse()
	{
		return Quaternion
		(
		    conjugate().scale( 1 / normalize().w ).w,
		    conjugate().scale( 1 / normalize().x ).x,
			conjugate().scale( 1 / normalize().y ).y,
			conjugate().scale( 1 / normalize().z ).z
		);
	}
	//
	Quaternion unit()
	{
		return Quaternion
		(
		    scale( 1 / magnitude().w ).w,
			scale( 1 / magnitude().x ).x,
			scale( 1 / magnitude().y ).y,
			scale( 1 / magnitude().z ).z
		) ;
	}
	// rotate a 3d vector, vector by the quaterion
	Quaternion rotate(Vector3 &vector)
	{
        Quaternion qv(0, vector.x, vector.y, vector.z);
        Quaternion qm = (*this) * qv * (*this).inverse();

        vector.x = qm.x;
        vector.y = qm.y;
        vector.z = qm.z;
	}

	// axis-angle to quaterion
	Quaternion to_axis_angle(Quaternion & q)
	{
		Quaternion q0 ;
		double angle = 2 * acos(q0.w) ;
        q0.x = q.x / sqrt(1 - q.w * q.w) ;
        q0.y = q.y / sqrt(1 - q.w * q.w) ;
        q0.z = q.z / sqrt(1 - q.w * q.w) ;
		return q0 ;
	}
	Matrix3 to_matrix(Quaternion & q)
	{
		/*
	    Matrix3 m1;
	    q.w = sqrt(1.0 + m1.a00 + m1.a11 + m1.a22) / 2.0;
	    double w4 = (4.0 * w);
	    q.x = (m1.a21 - m1.a12) / w4 ;
	    q.y = (m1.a02 - m1.a20) / w4 ;
	    q.z = (m1.a10 - m1.a01) / w4 ;
	    return m1 ;
		*/
	}
	// quaterion to axis-angle
	// euler to quaterion
	Vector3 to_euler(Quaternion & q)
	{
		/*
        Vector3 euler;
        const static double PI_OVER_2 = 3.14 * 0.5;
        const static double EPSILON = 1e-10;
        double sqw, sqx, sqy, sqz;

      // quick conversion to Euler angles to give tilt to user
      sqw = q.w*q.w;
      sqx = q.x*q.x;
      sqy = q.y*q.y;
      sqz = q.z*q.z;

      q.y = asin(2.0 * (q.w*q.y - q.x*q.z));
      if (PI_OVER_2 - fabs(q.y) > EPSILON) {
        q.z = atan2(2.0 * (q.x*q.y + q.w*q.z),
                         sqx - sqy - sqz + sqw);
        q.x = atan2(2.0 * (q.w*q.x + q.y*q.z),
                         sqw - sqx - sqy + sqz);
      } else {
        // compute heading from local 'down' vector
        q.z = atan2(2*q.y*q.z - 2*q.x*q.w,
                         2*q.x*q.z + 2*q.y*q.w);
        q.x = 0.0;

        // If facing down, reverse yaw
        if (q.y < 0)
          q.z = 3.14 - q.z;
      }
      return euler;
     */
	}
	// quaterion to euler
	// slerp
	Quaternion slerp(Quaternion q, double t) // t is fraction that the quat is slerped by
	{
			/*
		double omega ;
    double omega = acos(HeliMath::saturate(q0.q.x*q1.q.x +
                                           q0.q.y*q1.q.y +
                                           q0.q.z*q1.q.z +
                                           q0.q.w*q1.q.w, -1,1));
    if (fabs(omega) < 1e-10)
	{
      omega = 1e-10;
    }

    double som = sin(omega);
    double st0 = sin((1-t) * omega) / som;
    double st1 = sin(t * omega) / som;

    return Quaternion(q0.q.x*st0 + q1.q.x*st1,
                      q0.q.y*st0 + q1.q.y*st1,
                      q0.q.z*st0 + q1.q.z*st1,
                      q0.q.w*st0 + q1.q.w*st1);
		*/
	}
	//////////
	Quaternion operator + (const Quaternion & q) const
	{
		return Quaternion
		(
		    (x + q.x),
			(y + q.y),
			(z + q.z),
			(w + q.w)
		);
	}
	Quaternion operator - (const Quaternion & q) const
	{
		return Quaternion
		(
		    (x - q.x),
			(y - q.y),
			(z - q.z),
			(w - q.w)
		);
	}
	Quaternion operator * (const Quaternion & q) const
	{
		return Quaternion
		(
            ( w*q.x + x*q.w + y*q.z - z*q.y ),
            ( w*q.y + y*q.w + z*q.x - x*q.z ),
            ( w*q.z + z*q.w + x*q.y - y*q.x ),

		    ( w*q.w - x*q.x - y*q.y - z*q.z )
		);
	}
	Quaternion operator / (Quaternion & q) const
	{

		return Quaternion
		(
           ( x * q.inverse().x ),
		   ( y * q.inverse().y ),
		   ( z * q.inverse().z ),
		   ( w * q.inverse().w )
		);
	}
    Quaternion operator += (const Quaternion& q)
    {
        x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
		return (*this);
    }
    Quaternion operator -= (const Quaternion& q)
    {
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
		return (*this);
    }
    Quaternion operator *= (const Quaternion& q)
    {
	    double x_val = (w * q.x + x * q.w + y * q.z - z * q.y);
	    double y_val = (w * q.y + y * q.w + z * q.x - x * q.z);
	    double z_val = (w * q.z + z * q.w + x * q.y - y * q.x);
	    double w_val = (w * q.w - x * q.x - y * q.y - z * q.z);

	    w = w_val ;
		x = x_val ;
		y = y_val ;
		z = z_val ;
		return (*this);
    }
    Quaternion operator /= (Quaternion& q)
    {
	    (*this) = (*this) * q.inverse();
		return (*this) ;
	}
    bool operator != (const Quaternion& q)
    {
        return (w != q.w || x != q.x || y != q.y || z != q.z) ? true : false;
    }
    bool operator == (const Quaternion& q)
    {
        return (w == q.w && x == q.x && y == q.y && z == q.z) ? true : false;
    }
    double x;
	double y;
	double z;
	double w;
};
#endif
#endif
