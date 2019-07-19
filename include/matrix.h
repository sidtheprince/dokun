#ifndef _MATRIX
#define _MATRIX

#include "vector.h"
// VS built-in compiler keywords back in the 16-bit days
#ifdef far
#undef far
#endif
#ifdef near
#undef near
#endif

#ifdef __cplusplus // if c++
#include <iostream>
#include <cmath>
#include <algorithm> 

// rows = up + down, columns = left and right
struct Matrix 
{};
///////////// 
struct Matrix2  // 2 x 2
{
	/*
    Matrix2() : a(0), b(0), c(0), d(0) {}
	Matrix2 // create from four constants
	(
	    double a00, double a01,
  	    double a10, double a11
	)
	{
		a = a00; b = a01;
		c = a10; d = a11;
	}
	Matrix2(double number) // create from a single constant
	{
		a = number; b = number;
		c = number; d = number;
	}
	Matrix2(const Matrix2 & matrix) // create from another Matrix
	{
		(*this) = matrix;
	}
	/////////////
	// normal functions
	Matrix2 reciprocal() 
	{
		return Matrix2
		(
		    (1 / a), (1 / b),
			(1 / c), (1 / d)
		);
	}
	/////////////
	Matrix2 inverse()
	{
		// swap the positions of a and d
		double old_a = matrix[0];
		double a = matrix[3];
		double d = old_a;
		// put negatives in front of b and c
		double b = -matrix[1];
		double c = -matrix[2];
		// divide everything by the determinant (ad - bc)
		double det = 1 / ( (a * d) - (b * c) );
		return Matrix2
		(
		    (det * a), (det * b),
			(det * c), (det * d)
		);
	}
	/////////////
	Matrix2 identity()
	{ // gotta be equal to 1 diagonally
		double a = matrix[0] * inverse().a + matrix[1] * inverse().c;
		double b = matrix[0] * inverse().b + matrix[1] * inverse().d;
		double c = matrix[2] * inverse().a + matrix[3] * inverse().c;
		double d = matrix[2] * inverse().b + matrix[3] * inverse().d;
		return Matrix2
		(
		    (a), (b),
			(c), (d)
		);
	}
	/////////////
	Matrix2 transpose() // swaps rows and colums
	{
		double old_b = matrix[1];
		double b = matrix[2];
		double c = old_b;
		return Matrix2
		(
		    (a), (b),
			(c), (d)
		);		
	}
	/////////////
	// transform
	Matrix2 translate(double x, double y)
	{
		return Matrix2
		(
		    a + x, b,
			c, d + y
		);
	}
	/////////////
	Matrix2 rotate(double degrees)
	{
		return Matrix2
		(
		    a * cos(degrees), -b * sin(degrees),
			c * sin(degrees), d * cos(degrees)
		);
	}
	/////////////
	Matrix2 scale(double sx, double sy)
	{
		return Matrix2
		(
		    a * sx, b * sy,
			c,      d 
		);
	}
	/////////////
	Matrix2 shear(double shx, double shy)
	{
		return Matrix2
		(
		    1,  a + (a * shy),
			b + (b * shx),  1
		);
	}
	/////////////
	Matrix2 reflect(int x, int y)
	{
		if (x)
			scale(-x, y);
		if(y)
			scale(x, -y);
	}
	/////////////
	// math operations
	Matrix2 add // add by four constants
	(
	    double a00, double a01, 
		double a10, double a11
	)
	{
		return Matrix2
		(
		    a + a00, b + a01,
		    c + a10, d + a11		
		);
	}
	/////////////
	// arithmetic
	Matrix2 add(double x, double y) // add by two constants
	{
		return add(x, y, x, y);
	}
	/////////////
	Matrix2 add(double number) // add by constant
	{
		return add(number, number);
	}
	/////////////
	Matrix2 add(Vector2 vector)
	{
        return add(vector.x, vector.y);
	}
	/////////////
	Matrix2 add(Matrix2 matrix)
	{
		return add(matrix[0], matrix[1], matrix[2], matrix[3]);
	}	
	/////////////
	Matrix2 sub
	(
	    double a00, double a01, 
		double a10, double a11
	)
	{
		return Matrix2
		(
		    a - a00, b - a01,
		    c - a10, d - a11
		);
	}
	/////////////
	Matrix2 sub(double x, double y)
	{
        return sub(x, y, x, y);
	}
	/////////////
	Matrix2 sub(double number)
	{
        return sub(number, number);
	}
	/////////////
	Matrix2 sub(Vector2 vector)
	{
		return sub(vector.x, vector.y);
	}
	/////////////
	Matrix2 sub(Matrix2 matrix)
	{
        return sub
		(
		    matrix[0], matrix[1], 
			matrix[2], matrix[3]
		);
	}		
	/////////////
	Matrix2 mul // matrix multiplication is not communitive a * b != b * a
	(
	    double a00, double a01, double a10, double a11
	){}
	Matrix2 mul(double x, double y) // multiply with 3 points
	{}
	Matrix2 mul(double number) // multiply by a constant
	{
		return Matrix2
		(
		    number * a, number * b,
			number * c, number * d
		);
	}	
	Vector2 mul(Vector2 vector)
	{
		return Vector2
		(
		    a * vector.x +  b * vector.y,
			c * vector.x +  d * vector.y
		);
	}
	Matrix2 mul(Matrix2 matrix)
	{
        double a = matrix[0] * matrix[0] + matrix[1] * matrix[2];
        double b = matrix[0] * matrix[1] + matrix[1] * matrix[3];
        double c = matrix[2] * matrix[0] + matrix[3] * matrix[2];
        double d = matrix[2] * matrix[1] + matrix[3] * matrix[3];
		return Matrix2
		(
		    (a), (b),
			(c), (d)
		);
	}			
	/////////////
	Matrix2 div // So we don't divide, instead we multiply by an inverse (no such thing as dividing matrix)
	(
	    double a00, double a01, double a10, double a11
	){}
	Matrix2 div(double x, double y)
	{
        
	}
	Matrix2 div(Vector2 vector)
	{
	}
	Matrix2 div(Matrix2 matrix)
	{
        return (*this) * matrix[8]nverse();
	}			
	/////////////	
	// Operator overloading
	friend std::ostream& operator << (std::ostream& os, const Matrix2 & matrix)
	{
			os << "[" << matrix[0] << " " << matrix[1] << "]\n" << 
			"[" << matrix[2] << " " << matrix[3] << "]";
			return os ;		
	}	
	/////////////
	// Operator
	Matrix2 operator + (const Matrix2 &matrix) 
	{
        return add(matrix);
	}
	Matrix2 operator - (const Matrix2 &matrix) 
	{
        return sub(matrix);
	}	
	Matrix2 operator * (const Matrix2 &matrix) 
	{
        return mul(matrix);
	}	
	Matrix2 operator / (const Matrix2 &matrix) 
	{
        return div(matrix);
	}	
	/////////////	
	// vector type
	double a00, a;
	double a01, b;
	double a10, c;
	double a11, d;
	// array type
	double m[2][2];
	*/
};
struct Matrix2x3{};
struct Matrix2x4{};
struct Matrix3  // 3 x 3
{
    Matrix3() {}
	Matrix3(double m) {}
    Matrix3(
	    double a00, double a01, double a02, 
	    double a10, double a11, double a12,
		double a20, double a21, double a22
	){}
	~Matrix3(void){}

	Matrix3 inverse();
	// transforms
	Matrix3 translate();
	Matrix3 rotate();
	Matrix3 scale();
	/////////////
	/////////////
	/////////////
	Vector3 mul(Vector3 vector) // return vector after multiplaying with matrix
	{
		return Vector3
		(
		    (a * vector.x + b * vector.y + c * vector.z),
		    (d * vector.x + e * vector.y + f * vector.z),
		    (g * vector.x + h * vector.y + i * vector.z)
		);
	}
	/////////////
	Matrix3 mul(Matrix3 matrix)
	{}
    /////////////
	/////////////
	/////////////
	/////////////
	
	
	// translation
	double a00; // tx
	double a01; // ty
	double a02; // tz
	// rotation
	double a10; // rx
	double a11; // ry
	double a12; // rz
	// scale
	double a20; // sx
	double a21; // sy
	double a22; // sz
	
	double a, b, c;
	double d, e, f;
	double g, h, i;
	// array type
	double m[3][3];	
};
/////////////
/////////////
/////////////
struct Matrix4 // 4 x 4
{
	/////////////
	// Constructor(s)
    Matrix4()
	{
 		for(int i = 0; i < 16; i++)
		{
			matrix[i] = 0.0;
		}
	}
	/////////////
	Matrix4(double number) 
	{
		for(int i = 0; i < 16; i++)
		{
			matrix[i] = number;
		}
	}
	/////////////
    Matrix4
	(	
	    double a00, double a01, double a02, double a03, 
	    double a10, double a11, double a12, double a13, 
		double a20, double a21, double a22, double a23,
		double a30, double a31, double a32, double a33
	)
	{
		matrix[0] = a00;
		matrix[1] = a01;
		matrix[2] = a02;
		matrix[3] = a03;
		matrix[4] = a10;
		matrix[5] = a11;
		matrix[6] = a12;
		matrix[7] = a13;
		matrix[8] = a20;
		matrix[9] = a21;
		matrix[10] = a22;
		matrix[11] = a23;
		matrix[12] = a30;
		matrix[13] = a31;
		matrix[14] = a32;
		matrix[15] = a33;
	}
	/////////////
	Matrix4(const double (&array)[16]) // good!
	{
		set(array);
	}	
	/////////////
	Matrix4(const Matrix4& matrix) // good!
	{
		//set(matrix);
	}
	/////////////
	// Destructor
    ~Matrix4() // good!
	{
		//delete [] matrix;
	}
    /////////////
	// setters
	void set(const Matrix4& matrix) // good! 100%!
	{
		//(*this) = matrix;
	}
	/////////////
	void set(const double (&array)[16]) // good! 100%!
	{
		matrix[0] = array[0]; 
		matrix[1] = array[1]; 
		matrix[2] = array[2]; 
		matrix[3] = array[3]; 
		matrix[4] = array[4]; 
		matrix[5] = array[5]; 
		matrix[6] = array[6]; 
		matrix[7] = array[7]; 
		matrix[8] = array[8];
        matrix[9] = array[9]; 
        matrix[10] = array[10]; 
        matrix[11] = array[11]; 
        matrix[12] = array[12]; 
        matrix[13] = array[13]; 
        matrix[14] = array[14]; 
        matrix[15] = array[15];	
	}
	/////////////
	void set(int row, int column, int number) // good! 100%!
	{
/* 		switch(row)
		{
			case 1: // 1st row (left-to-right)
			    switch(column)
			    {
					case 1: matrix[0] = number; break;
					case 2: matrix[1] = number; break;
					case 3: matrix[2] = number; break;
					case 4: matrix[3] = number; break;
				}
				break;
			case 2: // 2nd row (left-to-right)
			    switch(column)
			    {
					case 1: matrix[4] = number; break;
					case 2: matrix[5] = number; break;
					case 3: matrix[6] = number; break;
					case 4: matrix[7] = number; break;
				}
				break;
			case 3: // 3rd row (left-to-right)
			    switch(column)
			    {
					case 1: matrix[8] = number; break;
					case 2: matrix[9] = number; break;
					case 3: matrix[10] = number; break;
					case 4: matrix[11] = number; break;
				}
				break;
			case 4: // 4th row (left-to-right)
			    switch(column)
			    {
					case 1: matrix[12] = number; break;
					case 2: matrix[13] = number; break;
					case 3: matrix[14] = number; break;
					case 4: matrix[15] = number; break;
				}
				break;
        }				 */
	}
	/////////////
	double * get() // returns a pointer to the matrix
	{
		return &matrix[0];
	}
	/////////////
	double get(int index) // returns an index at the matrix
	{
		return matrix[index];
	}
	/////////////
    // normal functions	
	Matrix4 reciprocal()
	{
/* 		return Matrix4
		(
		    (1 / matrix[0]), (1 / matrix[1]), (1 / matrix[2]), (1 / matrix[3]),
			(1 / matrix[4]), (1 / matrix[5]), (1 / matrix[6]), (1 / matrix[7]),
			(1 / matrix[8]), (1 / matrix[9]), (1 / matrix[10]), (1 / matrix[11]),
			(1 / matrix[12]), (1 / matrix[13]), (1 / matrix[14]), (1 / matrix[15])
		); */
	}
	Matrix4 inverse() // returns the inverse of a matrix
	{
		/*
		a0, b1, c2, d3
		e4, f5, g6, h7
		i8, j9, k10, l11
		m12, n13, o14, p15
		*/
		
		/*
		if(m == 0 && n == 0 && o == 0 && p == 1)
        this->invertAffine();
    else
    {
        this->invertGeneral();
        /*@@ invertProjective() is not optimized (slower than generic one)
        if(fabs(m[0]*m[5] - m[1]*m[4]) > 0.00001f)
            this->invertProjective();   // inverse using matrix partition
        else
            this->invertGeneral();      // generalized inverse
        
    }

    return *this;
	*/
		/*
		return Matrix4
		(
		    a, b, c, d,
			e, f, g, h,
			i, j, k, l,
			m, n, o, p
		);		*/
	}
	Matrix4 inverse_general()
	{
		/*
		    // get cofactors of minor matrices
    float cofactor0 = getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]);
    float cofactor1 = getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]);
    float cofactor2 = getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]);
    float cofactor3 = getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);

    // get determinant
    float determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
    if(fabs(determinant) <= EPSILON)
    {
        return identity();
    }

    // get rest of cofactors for adj(M)
    float cofactor4 = getCofactor(m[1],m[2],m[3], m[9],m[10],m[11], m[13],m[14],m[15]);
    float cofactor5 = getCofactor(m[0],m[2],m[3], m[8],m[10],m[11], m[12],m[14],m[15]);
    float cofactor6 = getCofactor(m[0],m[1],m[3], m[8],m[9], m[11], m[12],m[13],m[15]);
    float cofactor7 = getCofactor(m[0],m[1],m[2], m[8],m[9], m[10], m[12],m[13],m[14]);

    float cofactor8 = getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[13],m[14],m[15]);
    float cofactor9 = getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[12],m[14],m[15]);
    float cofactor10= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[12],m[13],m[15]);
    float cofactor11= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[12],m[13],m[14]);

    float cofactor12= getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[9], m[10],m[11]);
    float cofactor13= getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[8], m[10],m[11]);
    float cofactor14= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[8], m[9], m[11]);
    float cofactor15= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[8], m[9], m[10]);

    // build inverse matrix = adj(M) / det(M)
    // adjugate of M is the transpose of the cofactor matrix of M
    float invDeterminant = 1.0f / determinant;
    m[0] =  invDeterminant * cofactor0;
    m[1] = -invDeterminant * cofactor4;
    m[2] =  invDeterminant * cofactor8;
    m[3] = -invDeterminant * cofactor12;

    m[4] = -invDeterminant * cofactor1;
    m[5] =  invDeterminant * cofactor5;
    m[6] = -invDeterminant * cofactor9;
    m[7] =  invDeterminant * cofactor13;

    m[8] =  invDeterminant * cofactor2;
    m[9] = -invDeterminant * cofactor6;
    m[10]=  invDeterminant * cofactor10;
    m[11]= -invDeterminant * cofactor14;

    m[12]= -invDeterminant * cofactor3;
    m[13]=  invDeterminant * cofactor7;
    m[14]= -invDeterminant * cofactor11;
    m[15]=  invDeterminant * cofactor15;

    return *this;
		*/
	}
	Matrix4 inverse_affine()
	{/*
        Matrix3 r(m[0],m[1],m[2], m[4],m[5],m[6], m[8],m[9],m[10]);
        r.invert();
        m[0] = r[0];  m[1] = r[1];  m[2] = r[2];
        m[4] = r[3];  m[5] = r[4];  m[6] = r[5];
        m[8] = r[6];  m[9] = r[7];  m[10]= r[8];

    // -R^-1 * T
        float x = m[12];
        float y = m[13];
        float z = m[14];
        m[12] = -(r[0] * x + r[3] * y + r[6] * z);
        m[13] = -(r[1] * x + r[4] * y + r[7] * z);
        m[14] = -(r[2] * x + r[5] * y + r[8] * z);

    // last row should be unchanged (0,0,0,1)
    //m[3] = m[7] = m[11] = 0.0f;
    //m[15] = 1.0f;

        return * this;*/
	}
	Matrix4 inverse_euclidean()
	{
		double tmp;
        tmp = matrix[1];  matrix[1] = matrix[4];  matrix[4] = tmp;
        tmp = matrix[2];  matrix[2] = matrix[8];  matrix[8] = tmp;
        tmp = matrix[6];  matrix[6] = matrix[9];  matrix[9] = tmp;
        double x = matrix[12];
        double y = matrix[13];
        double z = matrix[14];
        matrix[12] = -(matrix[0] * x + matrix[4] * y + matrix[8] * z);
        matrix[13] = -(matrix[1] * x + matrix[5] * y + matrix[9] * z);
        matrix[14] = -(matrix[2] * x + matrix[6] * y + matrix[10]* z);
        return (*this);
	}
	///////////////
	Matrix4 invert()
	{
		if(matrix[3] == 0 && matrix[7] == 0 && matrix[11] == 0 && matrix[15] == 1)
			;
		 else /*this->invertGeneral()*/;
		return (*this);
	}
	///////////////
	Matrix4 transpose() // good! 100%!         returns the transpose of a matrix
	{
        std::swap(matrix[1],  matrix[4]);
        std::swap(matrix[2],  matrix[8]);
        std::swap(matrix[3],  matrix[12]);
        std::swap(matrix[6],  matrix[9]);
        std::swap(matrix[7],  matrix[13]);
        std::swap(matrix[11], matrix[14]);
        return (*this);
	}	
	/////////////
	// math operations
	Matrix4 add // good!
	(
	    double a00, double a01, double a02, double a03, 
	    double a10, double a11, double a12, double a13, 
		double a20, double a21, double a22, double a23,
		double a30, double a31, double a32, double a33
	){
/* 		return Matrix4
		(
		    a + a00, b + a01, c + a02, d + a03,
			e + a10, f + a11, g + a12, h + a13,
			i + a20, j + a21, k + a22, l + a23,
			m + a30, n + a31, o + a32, p + a33
		);	 */	
	}
	Matrix4 add(double number) // good!
	{
/* 		return Matrix4
		(
		    a + number, b + number, c + number, d + number,
			e + number, f + number, g + number, h + number,
			i + number, j + number, k + number, l + number,
			m + number, n + number, o + number, p + number
		);	 */	
	}		
	Matrix4 add(double x, double y, double z, double w = 1.0) // good!
	{
/* 		return Matrix4
		(
		    a + x, b + y, c + z, d + w,
			e + x, f + y, g + z, h + w,
			i + x, j + y, k + z, l + w,
			m + x, n + y, o + z, p + w
		);	 */	
	}
	Matrix4 add(Vector4 vector) // good!
	{
	    //add( vector.x, vector.y, vector.z, vector.w );
	}	
	Matrix4 add(Matrix4 matrix) // good!
	{/*
		return Matrix4
		(
		    a + matrix[0], b + matrix[1], c + matrix[2], d + matrix[3],
			e + matrix[4], f + matrix[5], g + matrix[6], h + matrix[7],
			i + matrix[8], j + matrix[9], k + matrix[10], l + matrix[11],
			m + matrix[12], n + matrix[13], o + matrix[14], p + matrix[15]
		);		*/
	}		
	/////////////
	Matrix4 sub(double number) // good!
	{
/* 		return Matrix4
		(
		    a - number, b - number, c - number, d - number,
			e - number, f - number, g - number, h - number,
			i - number, j - number, k - number, l - number,
			m - number, n - number, o - number, p - number
		);	 */			
	}		
	Matrix4 sub(double x, double y, double z, double w = 1.0) // good!
	{
/* 		return Matrix4
		(
		    a - x, b - y, c - z, d - w,
			e - x, f - y, g - z, h - w,
			i - x, j - y, k - z, l - w,
			m - x, n - y, o - z, p - w
		);			 */
	}
	Matrix4 sub(Vector4 vector) // good!
	{
       // sub(vector.x, vector.y, vector.z, vector.w);	
	}	
	Matrix4 sub(Matrix4 matrix)
	{/*
		return Matrix4
		(
		    a - matrix[0], b - matrix[1], c - matrix[2], d - matrix[3],
			e - matrix[4], f - matrix[5], g - matrix[6], h - matrix[7],
			i - matrix[8], j - matrix[9], k - matrix[10], l - matrix[11],
			m - matrix[12], n - matrix[13], o - matrix[14], p - matrix[15]
		);		*/
	}			
	/////////////
	Matrix4 mul(double number)
	{/*
		return Matrix4
		(
		    a * number, b * number, c * number, d * number,
			e * number, f * number, g * number, h * number,
			i * number, j * number, k * number, l * number,
			m * number, n * number, o * number, p * number
		);	*/		
	}		
	/////////////
	Matrix4 mul(double x, double y, double z, double w)
	{/*
		return Matrix4
		(
		    a, b, c, d,
			e, f, g, h,
			i, j, k, l,
			m, n, o, p
		);			*/
	}
	/////////////
	Vector4 mul(Vector4 vector)
	{/*
        double x = (a * vector.x) + (b * vector.y) + (c * vector.z) + (d * vector.w); 
		double y = (e * vector.x) + (f * vector.y) + (g * vector.z) + (h * vector.w); 
		double z = (i * vector.x) + (j * vector.y) + (k * vector.z) + (l * vector.w); 
        double w = (m * vector.x) + (n * vector.y) + (o * vector.z) + (p * vector.w); 	
		return Vector4
		( x, y, z, w );	*/
	}	
	/////////////
	Matrix4 mul(const Matrix4& matrix)
	{		// row x column
	    double a = (matrix[0] * matrix[0]) + (matrix[1] * matrix[4]) + (matrix[2] * matrix[8]) + (matrix[3] * matrix[12]);
		double b = (matrix[0] * matrix[1]) + (matrix[1] * matrix[5]) + (matrix[2] * matrix[9]) + (matrix[3] * matrix[13]);
		double c = (matrix[0] * matrix[2]) + (matrix[1] * matrix[6]) + (matrix[2] * matrix[10]) + (matrix[3] * matrix[14]);
		double d = (matrix[0] * matrix[3]) + (matrix[1] * matrix[7]) + (matrix[2] * matrix[11]) + (matrix[3] * matrix[15]);
		 
		double e = (matrix[4] * matrix[0]) + (matrix[5] * matrix[4]) + (matrix[6] * matrix[8]) + (matrix[7] * matrix[12]);
		double f = (matrix[4] * matrix[1]) + (matrix[5] * matrix[5]) + (matrix[6] * matrix[9]) + (matrix[7] * matrix[13]);
		double g = (matrix[4] * matrix[2]) + (matrix[5] * matrix[6]) + (matrix[6] * matrix[10]) + (matrix[7] * matrix[14]);
		double h = (matrix[4] * matrix[3]) + (matrix[5] * matrix[7]) + (matrix[6] * matrix[11]) + (matrix[7] * matrix[15]);
		 
		double i = (matrix[8] * matrix[0]) + (matrix[9] * matrix[4]) + (matrix[10] * matrix[8]) + (matrix[11] * matrix[12]);
		double j = (matrix[8] * matrix[1]) + (matrix[9] * matrix[5]) + (matrix[10] * matrix[9]) + (matrix[11] * matrix[13]);
		double k = (matrix[8] * matrix[2]) + (matrix[9] * matrix[6]) + (matrix[10] * matrix[10]) + (matrix[11] * matrix[14]);
		double l = (matrix[8] * matrix[3]) + (matrix[9] * matrix[7]) + (matrix[10] * matrix[11]) + (matrix[11] * matrix[15]);
		 
		double m = (matrix[12] * matrix[0]) + (matrix[13] * matrix[4]) + (matrix[14] * matrix[8]) + (matrix[15] * matrix[12]);
		double n = (matrix[12] * matrix[1]) + (matrix[13] * matrix[5]) + (matrix[14] * matrix[9]) + (matrix[15] * matrix[13]);
		double o = (matrix[12] * matrix[2]) + (matrix[13] * matrix[6]) + (matrix[14] * matrix[10]) + (matrix[15] * matrix[14]);
		double p = (matrix[12] * matrix[3]) + (matrix[13] * matrix[7]) + (matrix[14] * matrix[11]) + (matrix[15] * matrix[15]);	

		return Matrix4(
		    a, b, c, d,//matrix[0], matrix[1], matrix[2], matrix[3],
			e, f, g, h,//matrix[4], matrix[5], matrix[6], matrix[7],
			i, j, k, l,//matrix[8], matrix[9], matrix[10], matrix[11],
			m, n, o, p//matrix[12], matrix[13], matrix[14], matrix[15]
		);		
	}			
	///////////// 
	static Matrix4 mul(Matrix4& m0, const Matrix4& m1)
	{
		Matrix4 m = m0.mul(m1);
		return m;
	}
	///////////// 
	Matrix4 div
	(
	    double a00, double a01, double a02, double a03, 
	    double a10, double a11, double a12, double a13, 
		double a20, double a21, double a22, double a23,
		double a30, double a31, double a32, double a33
	){/*
		return Matrix4
		(
		    a, b, c, d,
			e, f, g, h,
			i, j, k, l,
			m, n, o, p
		);	*/		
	}
	/////////////
	Matrix4 div(double number)
	{/*
		return Matrix4
		(
		    a, b, c, d,
			e, f, g, h,
			i, j, k, l,
			m, n, o, p
		);		*/	
	}		
	/////////////
	Matrix4 div(double x, double y, double z)
	{/*
		return Matrix4
		(
		    a, b, c, d,
			e, f, g, h,
			i, j, k, l,
			m, n, o, p
		);			*/
	}
	/////////////
	Vector4 div(Vector4 vector)
	{}		
	/////////////
	Matrix4 div(Matrix4 matrix)
	{
		//return (*this) * inverse();
	}			
	/////////////
	Vector4 column(unsigned int index) // good! 100%            return a column as a Vector4
	{
		switch(index)
		{
			case 1: // first column
		        return Vector4(matrix[0], matrix[4], matrix[8], matrix[12]); break;
		    case 2:
			    return Vector4(matrix[1], matrix[5], matrix[9], matrix[13]); break;				
		    case 3:
			    return Vector4(matrix[2], matrix[6], matrix[10], matrix[14]); break;				
			case 4:
		        return Vector4(matrix[3], matrix[7], matrix[11], matrix[15]); break;				
		}
	}
	/////////////
	Vector4 row(unsigned int index) // good! 100%           return a row as a Vector4
	{
		switch(index)
		{
			case 1:
		        return Vector4(matrix[0], matrix[1], matrix[2], matrix[3]); break;
		    case 2:
		        return Vector4(matrix[4], matrix[5], matrix[6], matrix[7]); break;
		    case 3:
		        return Vector4(matrix[8], matrix[9], matrix[10], matrix[11]); break;
		    case 4:
		        return Vector4(matrix[12], matrix[13], matrix[14], matrix[15]); break;
		}
	}
	/////////////
	static Vector4 get_vector_transformed(const Matrix4& m, const Vector4& v) // returns a transformed vector
	{
		double x_ = m.matrix[0]  * v.x + m.matrix[1]  * v.y + m.matrix[2]  * v.z + m.matrix[3]  * v.w;
		double y_ = m.matrix[4]  * v.x + m.matrix[5]  * v.y + m.matrix[6]  * v.z + m.matrix[7]  * v.w;
		double z_ = m.matrix[8]  * v.x + m.matrix[9]  * v.y + m.matrix[10] * v.z + m.matrix[11] * v.w;
		double w_ = m.matrix[12] * v.x + m.matrix[13] * v.y + m.matrix[14] * v.z + m.matrix[15] * v.w;
		
        return Vector4(x_, y_, z_, w_);	
	}
	/////////////
	static void copy(Matrix4& a, const Matrix4& b) // good !
	{
		a[0] = b.matrix[0];
		a.matrix[1] = b.matrix[1];
		a.matrix[2] = b.matrix[2];
		a.matrix[3] = b.matrix[3];
		a.matrix[4] = b.matrix[4];
		a.matrix[5] = b.matrix[5];
		a.matrix[6] = b.matrix[6];
		a.matrix[7] = b.matrix[7];
		a.matrix[8] = b.matrix[8];
		a.matrix[9] = b.matrix[9];
		a.matrix[10] = b.matrix[10];
		a.matrix[11] = b.matrix[11];
		a.matrix[12] = b.matrix[12];
		a.matrix[13] = b.matrix[13];
		a.matrix[14] = b.matrix[14];
		a.matrix[15] = b.matrix[15];
	}
	/////////////
	static Matrix4 identity() // good !             identity = 0, 5, 10, 15 are all 1's and the rest are 0         identity_matrix = a square matrix in which all the elements of the principal diagonal are ones and all other elements are zeros
	{
		Matrix4 result(0.0);
		result[0]  = 1.0;
		result[5]  = 1.0;
		result[10] = 1.0;
		result[15] = 1.0;
		return result;
	}
	/////////////	
	// transform
	/////////////
	static Matrix4 translate(Matrix4& m, double x, double y, double z, double w = 1.0) //  good!              Builds a translation 4 * 4 matrix created from a vector of 3 components. Translate a matrix by a vector
	{
	/// glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
	/// // m[0][0] == 1.0f, m[0][1] == 0.0f, m[0][2] == 0.0f, m[0][3] == 0.0f
	/// // m[1][0] == 0.0f, m[1][1] == 1.0f, m[1][2] == 0.0f, m[1][3] == 0.0f
	/// // m[2][0] == 0.0f, m[2][1] == 0.0f, m[2][2] == 1.0f, m[2][3] == 0.0f
	/// // m[3][0] == 1.0f, m[3][1] == 1.0f, m[3][2] == 1.0f, m[3][3] == 1.0f		
		m[0]   = 1.0; // [0][0]
		m[5]   = 1.0; // [1][1]
		m[10]  = 1.0; // [2][2]
		m[12]  = x;
		m[13]  = y;
		m[14]  = z;
		m[15]  = w; // 1.0
		return m;
	}
	static Matrix4 translate(Matrix4& m, const Vector3& T) // Translate a matrix by a vector
	{
		return Matrix4::translate(m, T.x, T.y, T.z);
	}
	/////////////
	static Matrix4 rotate_x(Matrix4& m, double angle)
	{
        m[0] = 1.0;
		m[5] = cos(angle);
		m[6] = -sin(angle);
		m[9] = sin(angle);
		m[10] = cos(angle);
		m[15] = 1.0;		
	}
	static Matrix4 rotate_y(Matrix4& m, double angle)
	{
        m[0] = cos(angle);
		m[2] = sin(angle);
		m[5] = 1.0;
		m[8] = -sin(angle);
		m[10] = cos(angle);
		m.matrix[15] = 1.0;		
	}
	static Matrix4 rotate_z(Matrix4& m, double angle)
	{
        m.matrix[0] = cos(angle);
		m.matrix[1] = -sin(angle);
		m.matrix[4] = sin(angle);
		m.matrix[5] = cos(angle);
		m.matrix[10] = 1.0;
		m.matrix[15] = 1.0;			
	}	
	/////////////
	static Matrix4 rotate(Matrix4& m, double angle, int x, int y, int z)
	{
		Matrix4 rot_x, rot_y, rot_z;
		if(x) Matrix4::rotate_x(rot_x, angle);
	    if(y) Matrix4::rotate_y(rot_y, angle);
	    if(z) Matrix4::rotate_z(rot_z, angle);
	    m = Matrix4::mul(m, rot_x);
		m = Matrix4::mul(m, rot_y);
		m = Matrix4::mul(m, rot_z);
		return m;
	}
	static Matrix4 rotate(Matrix4& m, double angle, const Vector4& R)
	{
		return Matrix4::rotate(m, angle, static_cast<int>(R.x), static_cast<int>(R.y), static_cast<int>(R.z));
	}
	/////////////
	static Matrix4 scale(Matrix4& m, double sx, double sy, double sz, double sw = 1.0) // good !
	{
		m[0]  = sx;
		m[5]  = sy;
		m[10] = sz;
		m[15] = sw;
		return m;
	}
	static Matrix4 scale(Matrix4& m, const Vector3& S)
	{
		return Matrix4::scale(m, S.x, S.y, S.z);
	}
	/////////////
	// source : http://www.songho.ca/opengl/gl_matrix.html
	// equivalent to glOrtho
	static Matrix4 ortho(double left, double right, double bottom, double top, double near, double far) // return orthagonal matrix
	{  	
        Matrix4 m(0.0);
    
	    m[0]  = 2.0 / (right - left);
        m[5]  = 2.0 / (top - bottom);
		m[10] = -2.0 / (far - near);
        m[12] = -(right + left) / (right - left);
        m[13] = -(top + bottom) / (top - bottom);
        m[14] = -(far + near) / (far - near);
		
		return m;
	} // orthographic: (0, 800, 0, 600, -1, 1) OR (0, 800, 600, 0, -1, 1)	
	/////////////
	// equivalent to glFrustum
	static Matrix4 frustum(double left, double right, double bottom, double top, double near, double far)
	{
		Matrix4 m(0.0);
    
	    m[0]  = 2 * near / (right - left);
        m[5]  = 2 * near / (top - bottom);
        m[8]  = (right + left) / (right - left);
        m[9]  = (top + bottom) / (top - bottom);
        m[10] = -(far + near) / (far - near);
        m[11] = -1;
        m[14] = -(2 * far * near) / (far - near);
        m[15] = 0;
	
    	return m;
	}
	/////////////
	// equivalent to glPerspective
	static Matrix4 perspective(double fovy, double aspect, double z_near, double z_far)
	{
        double tangent = tan(fovy/2 * (3.14/180)); 
        double height  = z_near * tangent;
        double width   = height * aspect;

        return Matrix4::frustum(-width, width, -height, height, z_near, z_far);		
	} // common fovy(field of view) = 45, 90
	/////////////
	// equivalent to gluLookAt
	static Matrix4 look_at(const Vector3& eye, const Vector3& center,const Vector3& up)
	{	
	    Vector3 f = Vector3::normal(Vector3::sub(center, eye));
		Vector3 s = Vector3::normal(Vector3::cross(f, up));
		Vector3 u = Vector3::normal(Vector3::cross(s, f));
		
        Matrix4 m(0.0);
		
		m[0] = s.x;
        m[4] = s.y;
        m[8] = s.z;

        m[1] = u.x;
        m[5] = u.y;
        m[9] = u.z;

        m[2]  = -f.x;
        m[6]  = -f.y;
        m[10] = -f.z;

        m[3]  = 0.0;
        m[7]  = 0.0;
        m[11] = 0.0;

        m[12] = -Vector3::dot(s, eye);
        m[13] = -Vector3::dot(u, eye);
        m[14] = Vector3::dot(f, eye);
        m[15] = 1.0;
		
        return m;
	} 
	/////////////
	double * data()
	{
		return &matrix[0];
		//return const_cast<double *>(&matrix[0]);
	}
	/////////////
	static double * data(const Matrix4& matrix)
	{
		return const_cast<double *>(&matrix[0]);
	}
	/////////////
	// Operator
	Matrix4 operator + (const Matrix4 &matrix) 
	{
        return add(matrix);
	}
	/////////////
	Matrix4 operator - (const Matrix4 &matrix) 
	{
        return sub(matrix);
	}	
	/////////////
	Matrix4 operator * (const Matrix4 &matrix) 
	{
        return mul(matrix);
	}	
	/////////////
	Matrix4 operator / (const Matrix4 &matrix) 
	{
        return div(matrix);
	}
	/////////////
    bool operator == (const Matrix4 &m) const
	{}
    //////////////////	
    bool operator != (const Matrix4 &m) const
	{}		
	/////////////
	double& operator [] (int number)
	{
		switch(number)
		{
			case 0: return matrix[0]; break;
			case 1: return matrix[1]; break;
			case 2: return matrix[2]; break;
			case 3: return matrix[3]; break;
			case 4: return matrix[4]; break;
			case 5: return matrix[5]; break;
			case 6: return matrix[6]; break;
			case 7: return matrix[7]; break;
			case 8: return matrix[8]; break;
			case 9: return matrix[9]; break;
			case 10: return matrix[10]; break;
			case 11: return matrix[11]; break;
            case 12: return matrix[12]; break;
            case 13: return matrix[13]; break;
            case 14: return matrix[14]; break;
            case 15: return matrix[15]; break;		
		}
	}
	/////////////	
	const double& operator [] (int number) const
	{
		switch(number)
		{
			case 0: return matrix[0]; break;
			case 1: return matrix[1]; break;
			case 2: return matrix[2]; break;
			case 3: return matrix[3]; break;
			case 4: return matrix[4]; break;
			case 5: return matrix[5]; break;
			case 6: return matrix[6]; break;
			case 7: return matrix[7]; break;
			case 8: return matrix[8]; break;
			case 9: return matrix[9]; break;
			case 10: return matrix[10]; break;
			case 11: return matrix[11]; break;
            case 12: return matrix[12]; break;
            case 13: return matrix[13]; break;
            case 14: return matrix[14]; break;
            case 15: return matrix[15]; break;		
		}
	}
    //////////////////		
	friend std::ostream& operator << (std::ostream& os, const Matrix4& m)
	{
		os 
		<< "[ " << m[0]  << " " << m[1]  << " " << m[2]  << " " << m[3]  << " ]\n"
		<< "[ " << m[4]  << " " << m[5]  << " " << m[6]  << " " << m[7]  << " ]\n"
		<< "[ " << m[8]  << " " << m[9]  << " " << m[10] << " " << m[11] << " ]\n"
		<< "[ " << m[12] << " " << m[13] << " " << m[14] << " " << m[15] << " ]\n";
		return os;		
	}		
	/////////////
	// Values
	double a, b, c, d;
	double e, f, g, h;
	double i, j, k, l;
	double m, n, o, p;
	// actual stuff
	double matrix[16]; // 4 x 4
	/////////////
};
#endif
#endif
/*
projection * view * model  (OpenGL)

    model * view * projection  (Direct3D)
*/