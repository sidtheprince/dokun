#ifndef _MATRIX_TEST
#define _MATRIX_TEST
// VS built-in compiler keywords back in the 16-bit days
#ifdef far
#undef far
#endif
#ifdef near
#undef near
#endif

#include <iostream>
#include <cmath>
#include <algorithm> 
#include <assert.h>
#include "vector.h"
//struct Vector3 {Vector3(){} ~Vector3(){} };
struct Matrix2  // 2 x 2
{};
struct Matrix2x3{};
struct Matrix2x4{};
struct Matrix3  {};
struct Matrix4 // 4 x 4   to use m[r][c] you have to use m.value[r][c] or m(r, c)
{
	Matrix4 () { // default constructor
	    for(int j = 0; j < 4; j++){ // assign all values to 0
			for(int k = 0; k < 4; k++) { value[j][k] = 0;}
		}
		std::clog << "Matrix created (" << (this) << "):\n"<< (*this) << std::endl;
	}
	Matrix4 (const Matrix4& m) { copy(*this, m); }// copy constructor
	Matrix4 (float number) { // default constructor
	    for(int j = 0; j < 4; j++){ // assign all values to 0
			for(int k = 0; k < 4; k++) { value[j][k] = number;}
		}
		std::clog << "Matrix created (" << (this) << "):\n"<< (*this) << std::endl;
	}
	//~Matrix4() {} // destructor (not needed)
	//===========================================================================
	static Matrix4 translate(const Matrix4& m, float x, float y, float z    ) 
	{
		Matrix4 result(m);
		result[0]  = 1.0; // [0][0]
		result[5]  = 1.0; // [1][1]
		result[10] = 1.0; // [2][2]
		result[12] = x;
		result[13] = y;
		result[14] = z;
		result[15] = 1.0; // always 1		
		std::cout << result << " = value on translate\n";
		return result;
		/* // row-major (translates on row rather than column)
[ 1 0 0 0 ]
[ 0 1 0 0 ]
[ 0 0 1 0 ]
[ 2 2 -1 1 ]
 = value on translate
		*/
	/*
		Matrix4 result(m);
		result[0]  = 1.0; // [0][0]
		result[5]  = 1.0; // [1][1]
		result[10] = 1.0; // [2][2]

        result[3] = x;
		result[7] = y;
		result[11] = z;

		result[12] = 0;
		result[13] = 0;
		result[14] = 0;//z;
		result[15] = 1.0; // always 1		
		std::cout << result << " = value on translate\n";
		return result;*/
		/* // column-major (translates on column rather than row)
[ 1 0 0 2 ]
[ 0 1 0 2 ]
[ 0 0 1 -1 ]
[ 0 0 0 1 ]
 = value on translate
		*/     	   
	}
	static Matrix4 translate(const Matrix4& m, const Vector3& vector) 
	{
		return translate(m, vector.x, vector.y, vector.z);
	}
	//---------------------------------------------------------------------------
	static Matrix4 rotate   (const Matrix4& m, float angle, float rx, float ry, float rz) 
	{ // glm version
		float const a = angle;
		float const c = cos(a);
		float const s = sin(a);
		
		Vector3 axis = Vector3::normal(Vector3(rx, ry, rz));
		Vector3 temp = axis * (static_cast<float>(1) - c);
		
		Matrix4 rot; // uninitialized
		rot(0, 0) = c + temp[0] * axis[0];
		rot(0, 1) = temp[0] * axis[1] + s * axis[2];
		rot(0, 2) = temp[0] * axis[2] - s * axis[1];

		rot(1, 0) = temp[1] * axis[0] - s * axis[2];
		rot(1, 1) = c + temp[1] * axis[1];
		rot(1, 2) = temp[1] * axis[2] + s * axis[0];

		rot(2, 0) = temp[2] * axis[0] + s * axis[1];
		rot(2, 1) = temp[2] * axis[1] - s * axis[0];
		rot(2, 2) = c + temp[2] * axis[2];		
		
		Matrix4 result(1); // uninitialized
		result[0] = m[0] * rot(0, 0) + m[1] * rot(0, 1) + m[2] * rot(0, 2);
		result[1] = m[0] * rot(1, 0) + m[1] * rot(1, 1) + m[2] * rot(1, 2);
		result[2] = m[0] * rot(2, 0) + m[1] * rot(2, 1) + m[2] * rot(2, 2);
		result[3] = m[3];
		std::cout << result << " = value on rotate\n";
		return result;
		/*
[ 1 0 0 0 ]
[ 0 1 0 0 ]
[ 0 0 1 0 ]
[ 0 0 -1 1 ]
 = value on rotate
		*/
		
		// my version
		/*Matrix4 rot;
		if(rx) {
			rot[0] = 1.0;
		rot[5] = cos(angle);
		rot[6] = -sin(angle);
		rot[9] = sin(angle);
		rot[10] = cos(angle);
		rot[15] = 1.0;
		}
	    if(ry) {
			rot[0] = cos(angle);
		rot[2] = sin(angle);
		rot[5] = 1.0;
		rot[8] = -sin(angle);
		rot[10] = cos(angle);
		rot[15] = 1.0;	
		}
	    if(rz) {
			 rot[0] = cos(angle);
		rot[1] = -sin(angle);
		rot[4] = sin(angle);
		rot[5] = cos(angle);
		rot[10] = 1.0;
		rot[15] = 1.0;	
		}
		Matrix4 result(m);
		result[0] = m[0] * rot(0, 0) + m[1] * rot(0, 1) + m[2] * rot(0, 2);
		result[1] = m[0] * rot(1, 0) + m[1] * rot(1, 1) + m[2] * rot(1, 2);
		result[2] = m[0] * rot(2, 0) + m[1] * rot(2, 1) + m[2] * rot(2, 2);
		result[3] = m[3];
		std::cout << result << " = value on rotate\n";
		return m;*/
		/*
[ 1 0 0 0 ]
[ 0 1 0 0 ]
[ 0 0 1 0 ]
[ 0 0 -1 1 ]
 = value on rotate

		*/
	}
	static Matrix4 rotate   (const Matrix4& m, float angle, const Vector3& axis         )
	{
		return rotate(m, angle, axis.x, axis.y, axis.z);
	}
	//---------------------------------------------------------------------------
	static Matrix4 scale    (const Matrix4& m, float sx, float sy, float sz ) 
	{ // glm version
		Matrix4 result(m);
		result[0] = m[0] * sx;
		result[1] = m[1] * sy;
		result[2] = m[2] * sz;
		result[3] = m[3];
		std::cout << result << " = value on scale\n";
		return result;
		/*
[ 1 0 0 0 ]
[ 0 1 0 0 ]
[ 0 0 1 0 ]
[ 0 0 -1 1 ]
 = value on scale

		*/
		// my version
		/*Matrix4 result(m);
		result[0]  = sx;
		result[5]  = sy;
		result[10] = sz;
		result[15] = 1.0;
		std::cout << result << " = value on scale\n";
		return result;*/
		/*
[ 1 0 0 0 ]
[ 0 1 0 0 ]
[ 0 0 0 0 ]
[ 0 0 -1 1 ]
 = value on scale
		*/
	}
	static Matrix4 scale    (const Matrix4& m, const Vector3& scale_factor  ) 
	{
		return scale(m, scale_factor.x, scale_factor.y, scale_factor.z);
	}
	//===========================================================================
	static Matrix4 ortho      (float left, float right, float bottom, float top, float near, float far) 
	{ // copied from glm-9.8  glm/gtc/matrix_transform.inl
		/*Matrix4 result(1);
		result(0, 0) = 2 / (right - left);
		result(1, 1) = 2 / (top - bottom);
		result(3, 0) = - (right + left) / (right - left);
		result(3, 1) = - (top + bottom) / (top - bottom);
		
		result(2, 2) = - 2 / (far - near);                         //result(2, 2) = 1 / (far - near);      // if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
		result(3, 2) = - (far + near) / (far - near);                                   //result(3, 2) = - near / (far - near); // if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
		std::cout << result << " = value on ortho\n";
		return result;*/	
        /*
[ 0.0015625 1 1 1 ]
[ 1 -0.00277778 1 1 ]
[ 1 1 1 1 ]
[ -1 1 -0 1 ]
 = value on ortho
	    */
	   ///*
		// my version
        Matrix4 result(1.0);
	    result[0]  = 2.0 / (right - left);
        result[5]  = 2.0 / (top - bottom);
		result[10] = -2.0 / (far - near);
        result[12] = -(right + left) / (right - left);
        result[13] = -(top + bottom) / (top - bottom);
        result[14] = -(far + near) / (far - near);
		std::cout << result << " = value on ortho\n";
		return result; 
		//*/  
		/*
[ 0.0015625 1 1 1 ]
[ 1 -0.00277778 1 1 ]
[ 1 1 -1 1 ]
[ -1 1 -0 1 ]
 = value on ortho
		*/     		
	} ////
	static Matrix4 frustum    (float left, float right, float bottom, float top, float near, float far) 
	{ // copied from glm-9.8  glm/gtc/matrix_transform.inl
		Matrix4 result(0);
		result(0, 0) = (2 * near) / (right - left);
		result(1, 1) = (2 * near) / (top - bottom);
		result(2, 0) = (right + left) / (right - left);
		result(2, 1) = (top + bottom) / (top - bottom);
		result(2, 3) = -1;	// 1	
		
		result(2, 2) = - (far + near) / (far - near);                                   //result(2, 2) = far / (near - far);           // if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
		result(3, 2) = - (2 * far * near) / (far - near);                               //result(3, 2) = -(far * near) / (far - near); // if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
		//result(3, 3) = 0;
		std::cout << result << " = value on frustum\n";
		return result;
	} //template <typename T>         replace 'float' with T to use any data type
	static Matrix4 perspective(float fovy, float aspect, float z_near, float z_far                    ) 
	{ // copied from glm-9.8  glm/gtc/matrix_transform.inl
		assert(std::abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0));
		float tangent = tan(fovy / static_cast<float>(2)); // tangent of half of fovy
		
		Matrix4 result(static_cast<float>(0));
		result(0, 0) = static_cast<float>(1) / (aspect * tangent);
		result(1, 1) = static_cast<float>(1) / (tangent);
		result(2, 3) = static_cast<float>(1);
		
		result(2, 2) = (z_far + z_near) / (z_far - z_near);                             //result(2, 2) = z_far / (z_far - z_near);
	    result(3, 2) = - (static_cast<float>(2) * z_far * z_near) / (z_far - z_near);   //result(3, 2) = -(z_far * z_near) / (z_far - z_near);		
		std::cout << result << " = value on perspective\n";
        return result;	

		
	}
	static Matrix4 look_at    (float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, 
		float up_x, float up_y, float up_z) 
	{
		Vector3 eye(eye_x, eye_y, eye_z);
		Vector3 center(center_x, center_y, center_z);
		Vector3 up(up_x, up_y, up_z);
        look_at(eye, center, up);
	}
	static Matrix4 look_at    (const Vector3& eye, const Vector3& center,const Vector3& up) 
	{ // copied from glm-9.8  glm/gtc/matrix_transform.inl
	    Vector3 f = Vector3::normal(Vector3::sub(center, eye));
		Vector3 s = Vector3::normal(Vector3::cross(f, up));
		Vector3 u = Vector3::normal(Vector3::cross(s, f));
		
		Matrix4 result(1);
		result(0, 0) = s.x;
		result(1, 0) = s.y;
		result(2, 0) = s.z;
		result(0, 1) = u.x;
		result(1, 1) = u.y;
		result(2, 1) = u.z;
		result(0, 2) =-f.x;
		result(1, 2) =-f.y;
		result(2, 2) =-f.z;
		result(3, 0) =-Vector3::dot(s, eye);
		result(3, 1) =-Vector3::dot(u, eye);
		result(3, 2) = Vector3::dot(f, eye);
		result(3, 3) = 0;
		std::cout << result << " = value on look_at\n";
		return result;	
		/*
[ 1 0 -0 1 ]
[ -0 1 -0 1 ]
[ 0 0 1 1 ]
[ -0 -0 0 1 ]
 = value on look_at

        */
	/*
	    Vector3 f = Vector3::normal(Vector3::sub(center, eye));
		Vector3 s = Vector3::normal(Vector3::cross(f, up));
		Vector3 u = Vector3::normal(Vector3::cross(s, f));
		
        Matrix4 result(1.0);
		result[0] = s.x;
        result[4] = s.y;
        result[8] = s.z;
        result[1] = u.x;
        result[5] = u.y;
        result[9] = u.z;
        result[2]  = -f.x;
        result[6]  = -f.y;
        result[10] = -f.z;
        //result[3]  = 0.0;
        //result[7]  = 0.0;
        //result[11] = 0.0;
        result[12] =-Vector3::dot(s, eye);
        result[13] =-Vector3::dot(u, eye);
        result[14] = Vector3::dot(f, eye);
        result[15] = 0.0;//1.0;
		std::cout << result << " = value on look_at\n";
        return result;*/
		/*
[ 1 0 -0 1 ]
[ -0 1 -0 1 ]
[ 0 0 1 1 ]
[ -0 -0 0 0 ]
 = value on look_at
		*/
	}
	//===========================================================================
	static float * data(const Matrix4& m) 
	{
		return const_cast<float *>(&m.value[0][0]);
	}
	//============================================================================
	static void copy(Matrix4& a, const Matrix4& b) {
		a.value[0][0] = b.value[0][0];
		a.value[0][1] = b.value[0][1];
		a.value[0][2] = b.value[0][2];
		a.value[0][3] = b.value[0][3];
		
		a.value[1][0] = b.value[1][0];
		a.value[1][1] = b.value[1][1];
		a.value[1][2] = b.value[1][2];
		a.value[1][3] = b.value[1][3];
		
		a.value[2][0] = b.value[2][0];
		a.value[2][1] = b.value[2][1];
		a.value[2][2] = b.value[2][2];
		a.value[2][3] = b.value[2][3];
		
		a.value[3][0] = b.value[3][0];
		a.value[3][1] = b.value[3][1];
		a.value[3][2] = b.value[3][2];
		a.value[3][3] = b.value[3][3];
	}
	//============================================================================
	// operator
	float& operator () (unsigned int row, unsigned int column)
	{
		return value[row][column];
	}
	float operator () (unsigned int row, unsigned int column) const
	{
		return value[row][column];
	}
	/////////////
	float& operator [] (int number)
	{
		switch(number)
		{
			case 0: return value[0][0]; break;
			case 1: return value[0][1]; break;
			case 2: return value[0][2]; break;
			case 3: return value[0][3]; break;
			
			case 4: return value[1][0]; break;
			case 5: return value[1][1]; break;
			case 6: return value[1][2]; break;
			case 7: return value[1][3]; break;
			
			case 8: return value[2][0]; break;
			case 9: return value[2][1]; break;
			case 10: return value[2][2]; break;
			case 11: return value[2][3]; break;
			
            case 12: return value[3][0]; break;
            case 13: return value[3][1]; break;
            case 14: return value[3][2]; break;
            case 15: return value[3][3]; break;		
		}
	}
	/////////////	
	const float& operator [] (int number) const
	{
		switch(number)
		{
			case 0: return value[0][0]; break;
			case 1: return value[0][1]; break;
			case 2: return value[0][2]; break;
			case 3: return value[0][3]; break;
			
			case 4: return value[1][0]; break;
			case 5: return value[1][1]; break;
			case 6: return value[1][2]; break;
			case 7: return value[1][3]; break;
			
			case 8: return value[2][0]; break;
			case 9: return value[2][1]; break;
			case 10: return value[2][2]; break;
			case 11: return value[2][3]; break;
			
            case 12: return value[3][0]; break;
            case 13: return value[3][1]; break;
            case 14: return value[3][2]; break;
            case 15: return value[3][3]; break;			
		}
	}	
	/////////////
	friend std::ostream& operator << (std::ostream& os, const Matrix4& m)
	{
		os 
		<< "[ " << m.value[0][0]  << " " << m.value[0][1]  << " " << m.value[0][2]  << " " << m.value[0][3]  << " ]\n"
		<< "[ " << m.value[1][0]  << " " << m.value[1][1]  << " " << m.value[1][2]  << " " << m.value[1][3]  << " ]\n"
		<< "[ " << m.value[2][0]  << " " << m.value[2][1]  << " " << m.value[2][2]  << " " << m.value[2][3]  << " ]\n"
		<< "[ " << m.value[3][0]  << " " << m.value[3][1]  << " " << m.value[3][2]  << " " << m.value[3][3]  << " ]\n";
		return os;	
	}	
	// property
	float value[4][4];
};


#endif
