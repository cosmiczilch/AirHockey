//////////////////////////////////////////////////////////////////////////
// Tranformation.h -- Basic transformation classes
// 
// Liming Zhao
// 11/10/2007
// University of Pennsylvania
//////////////////////////////////////////////////////////////////////////


//OpenGL transformation matrix
//	Rx =
//	|1       0        0    Tx|
//	|0  cos(a)  -sin(a)    Ty|
//	|0  sin(a)   cos(a)    Tz|
//	|0       0        0    1 |
//
//	Ry =
//	| cos(a)  0  sin(a)    Tx|
//	|      0  1       0    Ty|
//	|-sin(a)  0  cos(a)    Tz|
//	|      0  0       0    1 |
//
//	Rz = 
//	|cos(a)  -sin(a)  0   Tx|
//	|sin(a)   cos(a)  0   Ty|
//	|     0        0  1   Tz|
//	|     0        0  0   1 |
//
// However, when they are stored in OpenGL matrix, they are stored column major
// OpenGL convention
// m[0] = R[0][0]; m[4] = R[0][1]; m[8]  = R[0][2]; m[12] = Tx;
// m[1] = R[1][0]; m[5] = R[1][1]; m[9]  = R[1][2]; m[13] = Ty;
// m[2] = R[2][0]; m[6] = R[2][1]; m[10] = R[2][2]; m[14] = Tz;
// m[3] = 0.0f;    m[7] = 0.0f;    m[11] = 0.0f;    m[15] = 1.0f;


/****************************************************************
*																*
* C++ Vector and Matrix Algebra routines						*
* Author: Jean-Francois DOUE									*
* Version 3.1 --- October 1993									*
*																*
****************************************************************/
//
//	From "Graphics Gems IV / Edited by Paul S. Heckbert
//	Academic Press, 1994, ISBN 0-12-336156-9
//	"You are free to use and modify this code in any way 
//	you like." (p. xv)
//
//	Modified by J. Nagle, March 1997
//	-	All functions are inline.
//	-	All functions are const-correct.
//	-	All checking is via the standard "assert" macro.
//	-	Stream I/O is disabled for portability, but can be
//		re-enabled by defining ALGEBRA3IOSTREAMS.

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <cmath>

using namespace std;

enum {VX, VY, VZ, VW};		    // axes
enum {PA, PB, PC, PD};		    // planes
enum {RED, GREEN, BLUE};	    // colors
enum {KA, KD, KS, ES};		    // phong coefficients

//////////////////////////////////////////////////////////////////////////
//PI
//
#ifndef M_PI
const float M_PI = 3.14159265358979323846f;		// per CRC handbook, 14th. ed.
#endif
const float M05_PI = (M_PI/2.0f);				// PI/2
const float M2_PI = (M_PI*2.0f);				// PI*2
const float Rad2Deg = (180.0f / M_PI);			// Rad to Degree
const float Deg2Rad = (M_PI / 180.0f);			// Degree to Rad

#ifndef EPSILON
#define EPSILON 0.001f
#endif

// min-max macros
#ifndef MIN
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#endif
#ifndef MAX
#define MAX(A,B) ((A) > (B) ? (A) : (B))
#endif

// error handling macro
#define ALGEBRA_ERROR(E) { assert(false); }

class vec2;
class vec3;
class vec4;
class mat3;
class mat4;
class Quaternion;
class Transform;


/****************************************************************
*																*
*			    2D Vector										*
*																*
****************************************************************/

class vec2
{
protected:

	float n[2];

public:

	// Constructors
	vec2();
	vec2(const float x, const float y);
	vec2(const float d);
	vec2(const vec2& v);				// copy constructor
	vec2(const vec3& v);				// cast v3 to v2
	vec2(const vec3& v, int dropAxis);	// cast v3 to v2

	// Assignment operators
	vec2& operator	= ( const vec2& v );	// assignment of a vec2
	vec2& operator += ( const vec2& v );	// incrementation by a vec2
	vec2& operator -= ( const vec2& v );	// decrementation by a vec2
	vec2& operator *= ( const float d );	// multiplication by a constant
	vec2& operator /= ( const float d );	// division by a constant
	float& operator [] ( int i);			// indexing
        float operator[] ( int i) const;// read-only indexing

	// special functions
	float Length() const;			// length of a vec2
	float SqrLength() const;			// squared length of a vec2
	vec2& Normalize() ;				// normalize a vec2 in place
	
	// friends
	friend vec2 operator- (const vec2& v);						// -v1
	friend vec2 operator+ (const vec2& a, const vec2& b);	    // v1 + v2
	friend vec2 operator- (const vec2& a, const vec2& b);	    // v1 - v2
	friend vec2 operator* (const vec2& a, const float d);	    // v1 * 3.0
	friend vec2 operator* (const float d, const vec2& a);	    // 3.0 * v1
	friend vec2 operator* (const mat3& a, const vec2& v);	    // M . v
	friend vec2 operator* (const vec2& v, const mat3& a);		// v . M
	friend float operator* (const vec2& a, const vec2& b);    // dot product
	friend vec2 operator/ (const vec2& a, const float d);	    // v1 / 3.0
	friend vec3 operator^ (const vec2& a, const vec2& b);	    // cross product
	friend int operator== (const vec2& a, const vec2& b);	    // v1 == v2 ?
	friend int operator!= (const vec2& a, const vec2& b);	    // v1 != v2 ?

	friend void Swap(vec2& a, vec2& b);						// swap v1 & v2
	friend vec2 Min(const vec2& a, const vec2& b);		    // min(v1, v2)
	friend vec2 Max(const vec2& a, const vec2& b);		    // max(v1, v2)
	friend vec2 Prod(const vec2& a, const vec2& b);		    // term by term *

	// input output
	friend istream& operator>>(istream& s, vec2& v);
	friend ostream& operator<<(ostream& s, const vec2& v);

	// necessary friend declarations
	friend class vec3;
};

/****************************************************************
*																*
*			    3D Vector										*
*																*
****************************************************************/

class vec3
{
protected:

	float n[3];

public:

	// Constructors
	vec3();
	vec3(const float x, const float y, const float z);
	vec3(const float d);
	vec3(const vec3& v);					// copy constructor
	vec3(const vec2& v);					// cast v2 to v3
	vec3(const vec2& v, float d);		    // cast v2 to v3
	vec3(const vec4& v);					// cast v4 to v3
	vec3(const vec4& v, int dropAxis);	    // cast v4 to v3

	// Assignment operators
	vec3& operator	= ( const vec3& v );	    // assignment of a vec3
	vec3& operator += ( const vec3& v );	    // incrementation by a vec3
	vec3& operator -= ( const vec3& v );	    // decrementation by a vec3
	vec3& operator *= ( const float d );	    // multiplication by a constant
	vec3& operator /= ( const float d );	    // division by a constant
	float& operator [] ( int i);				// indexing
	float operator[] (int i) const;			// read-only indexing

	// special functions
	float Length() const;				// length of a vec3
	float SqrLength() const;				// squared length of a vec3
	vec3& Normalize();					// normalize a vec3 in place
	vec3 Cross(vec3 &v) const;			// cross product

	// friends
	friend vec3 operator - (const vec3& v);						// -v1
	friend vec3 operator + (const vec3& a, const vec3& b);	    // v1 + v2
	friend vec3 operator - (const vec3& a, const vec3& b);	    // v1 - v2
	friend vec3 operator * (const vec3& a, const float d);	    // v1 * 3.0
	friend vec3 operator * (const float d, const vec3& a);	    // 3.0 * v1
	friend vec3 operator * (const mat4& a, const vec3& v);	    // M . v
	friend vec3 operator * (const vec3& v, const mat4& a);		// v . M
	friend float operator * (const vec3& a, const vec3& b);    // dot product
	friend vec3 operator / (const vec3& a, const float d);	    // v1 / 3.0
	friend vec3 operator ^ (const vec3& a, const vec3& b);	    // cross product
	friend int operator == (const vec3& a, const vec3& b);	    // v1 == v2 ?
	friend int operator != (const vec3& a, const vec3& b);	    // v1 != v2 ?

	friend void Swap(vec3& a, vec3& b);						// swap v1 & v2
	friend vec3 Min(const vec3& a, const vec3& b);		    // min(v1, v2)
	friend vec3 Max(const vec3& a, const vec3& b);		    // max(v1, v2)
	friend vec3 Prod(const vec3& a, const vec3& b);		    // term by term *

	// input output
	friend istream& operator>>(istream& s, vec3& v);
	friend ostream& operator<<(ostream& s, const vec3& v);

	// necessary friend declarations
	friend class vec2;
	friend class vec4;
	friend class mat3;
	friend vec2 operator * (const mat3& a, const vec2& v);	// linear transform
	friend vec3 operator * (const mat3& a, const vec3& v);
	friend mat3 operator * (const mat3& a, const mat3& b);	// matrix 3 product
};

const vec3 axisZero(0.0f, 0.0f, 0.0f);
const vec3 axisX(1.0f, 0.0f, 0.0f);
const vec3 axisY(0.0f, 1.0f, 0.0f);
const vec3 axisZ(0.0f, 0.0f, 1.0f);

/****************************************************************
*																*
*			    4D Vector										*
*																*
****************************************************************/

class vec4
{
public:

	float n[4];

public:

	// Constructors
	vec4();
	vec4(const float x, const float y, const float z, const float w);
	vec4(const float d);
	vec4(const vec4& v);			    // copy constructor
	vec4(const vec3& v);			    // cast vec3 to vec4
	vec4(const vec3& v, const float d);	    // cast vec3 to vec4

	// Assignment operators
	vec4& operator	= ( const vec4& v );	    // assignment of a vec4
	vec4& operator += ( const vec4& v );	    // incrementation by a vec4
	vec4& operator -= ( const vec4& v );	    // decrementation by a vec4
	vec4& operator *= ( const float d );	    // multiplication by a constant
	vec4& operator /= ( const float d );	    // division by a constant
	float& operator [] ( int i);				// indexing
	float operator[] (int i) const;			// read-only indexing

	// special functions
	float Length() const;			// length of a vec4
	float SqrLength() const;			// squared length of a vec4
	vec4& Normalize();			    // normalize a vec4 in place
	
	// friends
	friend vec4 operator - (const vec4& v);						// -v1
	friend vec4 operator + (const vec4& a, const vec4& b);	    // v1 + v2
	friend vec4 operator - (const vec4& a, const vec4& b);	    // v1 - v2
	friend vec4 operator * (const vec4& a, const float d);	    // v1 * 3.0
	friend vec4 operator * (const float d, const vec4& a);	    // 3.0 * v1
	friend vec4 operator * (const mat4& a, const vec4& v);	    // M . v
	friend vec4 operator * (const vec4& v, const mat4& a);	    // v . M
	friend float operator * (const vec4& a, const vec4& b);    // dot product
	friend vec4 operator / (const vec4& a, const float d);	    // v1 / 3.0
	friend int operator == (const vec4& a, const vec4& b);	    // v1 == v2 ?
	friend int operator != (const vec4& a, const vec4& b);	    // v1 != v2 ?

	friend void Swap(vec4& a, vec4& b);						// swap v1 & v2
	friend vec4 Min(const vec4& a, const vec4& b);		    // min(v1, v2)
	friend vec4 Max(const vec4& a, const vec4& b);		    // max(v1, v2)
	friend vec4 Prod(const vec4& a, const vec4& b);		    // term by term *

	friend istream& operator >> (istream& s, vec4& v);
	friend ostream& operator << (ostream& s, const vec4& v);

	// necessary friend declarations
	friend class vec3;
	friend class mat4;
	friend vec3 operator * (const mat4& a, const vec3& v);	// linear transform
	friend mat4 operator * (const mat4& a, const mat4& b);	// matrix 4 product
};

/****************************************************************
*																*
*			   3x3 Matrix										*
*																*
****************************************************************/

class mat3
{
protected:

	vec3 v[3];

public:

	// Constructors
	mat3();
	mat3(const vec3& v0, const vec3& v1, const vec3& v2);
	mat3(const float d);
	mat3(const mat3& m);
	mat3(const mat4& m);

	// Static functions
	//static mat3 Identity();
	static mat3 Translation2D(const vec2& v);
	static mat3 Rotation2DDeg(const vec2& center, const float angleDeg);
	static mat3 Rotation2DRad(const vec2& center, const float angleRad);
	static mat3 Scaling2D(const vec2& scaleVector);
	static mat3 Rotation3DDeg(const vec3& axis, const float angleDeg);
	static mat3 Rotation3DRad(const vec3& axis, const float angleRad);
	static mat3 Rotation3DDeg(const int Axis, const float angleDeg);
	static mat3 Rotation3DRad(const int Axis, const float angleRad);
	static mat3 Slerp(const mat3& rot0, const mat3& rot1, const float& fPerc);
	static mat3 Lerp(const mat3& rot0, const mat3& rot1, const float& fPerc);

	// Rotation operations, matrix must be orthonomal
	bool ToEulerAnglesXYZ(vec3& anglesRad) const;
	bool ToEulerAnglesXZY(vec3& anglesRad) const;
	bool ToEulerAnglesYXZ(vec3& anglesRad) const;
	bool ToEulerAnglesYZX(vec3& anglesRad) const;
	bool ToEulerAnglesZXY(vec3& anglesRad) const;
	bool ToEulerAnglesZYX(vec3& anglesRad) const;
	mat3 FromEulerAnglesXYZ(const vec3& anglesRad);
	mat3 FromEulerAnglesXZY(const vec3& anglesRad);
	mat3 FromEulerAnglesYXZ(const vec3& anglesRad);
	mat3 FromEulerAnglesYZX(const vec3& anglesRad);
	mat3 FromEulerAnglesZXY(const vec3& anglesRad);
	mat3 FromEulerAnglesZYX(const vec3& anglesRad);
	void ToGLMatrix(float* pData);

	// Conversion with Quaternion
	Quaternion ToQuaternion() const;
	void FromQuaternion(const Quaternion& q);
	void ToAxisAngle(vec3& axis, float& angleRad) const;
	void FromAxisAngle(const vec3& axis, const float& angleRad);

	// Assignment operators
	mat3& operator	= ( const mat3& m );	    // assignment of a mat3
	mat3& operator += ( const mat3& m );	    // incrementation by a mat3
	mat3& operator -= ( const mat3& m );	    // decrementation by a mat3
	mat3& operator *= ( const float d );	    // multiplication by a constant
	mat3& operator /= ( const float d );	    // division by a constant
	vec3& operator [] ( int i);					// indexing
	const vec3& operator [] ( int i) const;		// read-only indexing

	// special functions
	mat3 Transpose() const;								// transpose
	mat3 Inverse() const;								// inverse
	void WriteToGLMatrix(float* m);							// turn rotational data into 4x4 opengl matrix with zero translation
	void ReadFromGLMatrix(float* m);						// read rotational data from 4x4 opengl matrix
	bool Reorthogonalize();								// Gram-Schmidt orthogonalization
	//void getRow(unsigned int axis, vec3& rowVec) const;	// get a particular row
	//void getCol(unsigned int axis, vec3& colVec) const;	// get a particular col
	vec3 GetRow(unsigned int axis) const;	// get a particular row
	vec3 GetCol(unsigned int axis) const;	// get a particular col
	void SetRow(unsigned int axis, const vec3& rowVec);	// set a particular row
	void SetCol(unsigned int axis, const vec3& colVec);	// set a particular col
	vec3 GetYawPitchRoll(unsigned int leftAxis, unsigned int upAixs, unsigned int frontAxis) const;

	// friends
	friend mat3 operator - (const mat3& a);						// -m1
	friend mat3 operator + (const mat3& a, const mat3& b);	    // m1 + m2
	friend mat3 operator - (const mat3& a, const mat3& b);	    // m1 - m2
	friend mat3 operator * (const mat3& a, const mat3& b);		// m1 * m2
	friend mat3 operator * (const mat3& a, const float d);	    // m1 * 3.0
	friend mat3 operator * (const float d, const mat3& a);	    // 3.0 * m1
	friend mat3 operator / (const mat3& a, const float d);	    // m1 / 3.0
	friend int operator == (const mat3& a, const mat3& b);	    // m1 == m2 ?
	friend int operator != (const mat3& a, const mat3& b);	    // m1 != m2 ?
	friend void Swap(mat3& a, mat3& b);			    // swap m1 & m2

	friend istream& operator >> (istream& s, mat3& v);
	friend ostream& operator << (ostream& s, const mat3& v);

	// necessary friend declarations
	friend vec3 operator * (const mat3& a, const vec3& v);	    // linear transform
	friend vec2 operator * (const mat3& a, const vec2& v);	    // linear transform
};

const mat3 identity3D(axisX, axisY, axisZ);
const mat3 zero3D(axisZero, axisZero, axisZero);

/****************************************************************
*																*
*			   4x4 Matrix										*
*																*
****************************************************************/

class mat4
{
protected:

	vec4 v[4];

public:

	// Constructors
	mat4();
	mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3);
	mat4(const float d);
	mat4(const mat4& m);
	mat4(const mat3& m);
	mat4(const mat3& m, const vec3& t);
	mat4(const float* d);

	// Static functions
	//static mat4 Identity();	
	static mat4 Translation3D(const vec3& v);
	static mat4 Rotation3DDeg(const vec3& axis, const float angleDeg);
	static mat4 Rotation3DRad(const vec3& axis, const float angleRad);
	static mat4 Scaling3D(const vec3& scaleVector);
	static mat4 Perspective3D(const float d);

	// Assignment operators
	mat4& operator	= ( const mat4& m );	    // assignment of a mat4
	mat4& operator += ( const mat4& m );	    // incrementation by a mat4
	mat4& operator -= ( const mat4& m );	    // decrementation by a mat4
	mat4& operator *= ( const float d );	    // multiplication by a constant
	mat4& operator /= ( const float d );	    // division by a constant
	vec4& operator [] ( int i);					// indexing
	const vec4& operator [] ( int i) const;		// read-only indexing

	// special functions
	mat4 Transpose() const;						// transpose
	mat4 Inverse() const;						// inverse
	void WriteToGLMatrix(float* m);
	void ReadFromGLMatrix(float* m);

	// friends
	friend mat4 operator - (const mat4& a);						// -m1
	friend mat4 operator + (const mat4& a, const mat4& b);	    // m1 + m2
	friend mat4 operator - (const mat4& a, const mat4& b);	    // m1 - m2
	friend mat4 operator * (const mat4& a, const mat4& b);		// m1 * m2
	friend mat4 operator * (const mat4& a, const float d);	    // m1 * 4.0
	friend mat4 operator * (const float d, const mat4& a);	    // 4.0 * m1
	friend mat4 operator / (const mat4& a, const float d);	    // m1 / 3.0
	friend int operator == (const mat4& a, const mat4& b);	    // m1 == m2 ?
	friend int operator != (const mat4& a, const mat4& b);	    // m1 != m2 ?
	friend void Swap(mat4& a, mat4& b);							// swap m1 & m2

	friend istream& operator >> (istream& s, mat4& v);
	friend ostream& operator << (ostream& s, const mat4& v);

	// necessary friend declarations
	friend vec4 operator * (const mat4& a, const vec4& v);	    // linear transform
	friend vec3 operator * (const mat4& a, const vec3& v);	    // linear transform
};

const mat4 identity4D(identity3D);
const mat4 zero4D(zero3D);

/****************************************************************
*																*
*		    Quaternion          								*
*																*
****************************************************************/


class Quaternion
{
protected:

	float n[4];

	// Used by Slerp
	static float CounterWarp(float t, float fCos);
	static float ISqrt_approx_in_neighborhood(float s);

	// Internal indexing
	float& operator[](int i);
	float operator[](int i) const;
public:

	// Constructors
	Quaternion();
	Quaternion(const float w, const float x, const float y, const float z);
	Quaternion(const Quaternion& q);
	Quaternion(const vec4& v);

	// Static functions
	static float Dot(const Quaternion& q0, const Quaternion& q1);
	static Quaternion Exp(const Quaternion& q);
	static Quaternion Log(const Quaternion& q);
	static Quaternion UnitInverse(const Quaternion& q);
	static Quaternion Slerp(float t, const Quaternion& q0, const Quaternion& q1);
	static Quaternion Intermediate (const Quaternion& q0, const Quaternion& q1, const Quaternion& q2);
	static Quaternion Squad(float t, const Quaternion& q0, const Quaternion& a, const Quaternion& b, const Quaternion& q1);
	static Quaternion ProjectToAxis(const Quaternion& q, vec3 axis);

	// Conversion functions
	void ToAxisAngle (vec3& axis, float& angleRad) const;
	void FromAxisAngle (const vec3& axis, float angleRad);

	void FromAxisXAngle(float angleRad);
	void FromAxisYAngle(float angleRad);
	void FromAxisZAngle(float angleRad);

	mat3 ToRotation () const;
	void FromRotation (const mat3& rot);

	// Assignment operators
	Quaternion& operator = (const Quaternion& q);	// assignment of a quaternion
	Quaternion& operator += (const Quaternion& q);	// summation with a quaternion
	Quaternion& operator -= (const Quaternion& q);	// subtraction with a quaternion
	Quaternion& operator *= (const Quaternion& q);	// multiplication by a quaternion
	Quaternion& operator *= (const float d);		// multiplication by a scalar
	Quaternion& operator /= (const float d);		// division by a scalar

	// Indexing
	float& W();
	float W() const;
	float& X();
	float X() const;
	float& Y();
	float Y() const;
	float& Z();
	float Z() const;

	// Friends
	friend Quaternion operator - (const Quaternion& q);							// -q
        friend Quaternion operator + (const Quaternion& q0, const Quaternion& q1);	    // q0 + q1
	friend Quaternion operator - (const Quaternion& q0, const Quaternion& q1);	// q0 - q1
	friend Quaternion operator * (const Quaternion& q, const float d);			// q * 3.0
	friend Quaternion operator * (const float d, const Quaternion& q);			// 3.0 * v
	friend Quaternion operator * (const Quaternion& q0, const Quaternion& q1);  // q0 * q1
	friend Quaternion operator / (const Quaternion& q, const float d);			// q / 3.0
	friend bool operator == (const Quaternion& q0, const Quaternion& q1);		// q0 == q1 ?
	friend bool operator != (const Quaternion& q0, const Quaternion& q1);		// q0 != q1 ?

	// Special functions
	float Length() const;
	float SqrLength() const;
	Quaternion& Normalize();
	Quaternion& FastNormalize();
	Quaternion Conjugate() const;
	Quaternion Inverse() const;
	void Zero();

	friend istream& operator >> (istream& s, Quaternion& v);
	friend ostream& operator << (ostream& s, const Quaternion& v);

        friend class mat3;
};

/****************************************************************
*																*
*		    Transform	          								*
*																*
****************************************************************/
class Transform
{
public:
	Transform();
	Transform(const vec3& translation, const mat3& rotation);
	Transform(const vec3& translation);
	Transform(const mat3& rotation);
	Transform(const Transform& transform);

	Transform Inverse() const;
	Transform& operator = (const Transform& source); // assignment
	void ToGLMatrix(float* pData);
	void FromCoord(const vec3& coord);
	vec3 ToCoord() const;

	friend Transform operator * (const Transform& t1, const Transform& t2);
	friend vec3 operator * (const Transform& t, const vec3& v);
	static Transform Lerp(const float fPerc, const Transform& t0, const Transform& t1);

public:
	vec3 m_translation;
	mat3 m_rotation;
};

const Transform identityTransform(axisZero, identity3D);

inline Transform operator * (const Transform& t1, const Transform& t2)
{
	Transform tmp;
	tmp.m_rotation = t1.m_rotation * t2.m_rotation;
	tmp.m_translation = t1.m_translation + t1.m_rotation * t2.m_translation;
	return tmp;
}

inline vec3 operator * (const Transform& t, const vec3& v)
{
	return t.m_rotation * v + t.m_translation;
}
////////////////////////////////////////////////////////////////////////

// Some functions for handling computation on angles
inline void ClampAngle(float& angle)
{
	while (angle > M_PI)
		angle -= M2_PI;
	while (angle < -M_PI)
		angle += M2_PI;
}

inline void ClampAngleDeg(float& angle)
{
	while (angle > 180.0f)
		angle -= 360.0f;
	while (angle < -180.0f)
		angle += 360.0f;
}

inline void ClampAngleVectorDeg(vec3& angle)
{
	ClampAngleDeg(angle[VX]);
	ClampAngleDeg(angle[VY]);
	ClampAngleDeg(angle[VZ]);
}

inline float AngleDiff(float angleTo, float angleFrom)
{
	float value = angleTo - angleFrom;
	ClampAngle(value);
	return value;
}

inline float AngleDiffDeg(float angleTo, float angleFrom)
{
	ClampAngleDeg(angleTo);
	ClampAngleDeg(angleFrom);
	float result = angleTo - angleFrom;
	if (result > 180.0f)
		result = result - 360.0f;
	if (result < -180.0f)
		result = result + 360.0f;

	return result;
	
	//float value = angleTo - angleFrom;
	//ClampAngleDeg(value);
	//return value;

	//theta1 = normalize_theta(theta1);
	//theta2 = normalize_theta(theta2);
	//if (fabs(theta1-theta2) > 180)
	//{
	//	if (theta1 < 0)
	//		theta1+=360.0;
	//	else
	//		theta2+=360.0;
	//}
	//return fabs(theta1 - theta2);

//	ClampAngleDeg(angleTo);
//	ClampAngleDeg(angleFrom);
//	if (fabs(angleTo-angleFrom) > 180)
//	{
//		if (angleTo < 0)
//			angleTo+=360.0f;
//		else
//			angleFrom+=360.0f;
//	}
//	return fabs(angleTo - angleFrom);
}

inline vec3 AngleDiffDegVec3(const vec3& angleTo, const vec3& angleFrom)
{
	vec3 result;
	for (unsigned int i = 0; i < 3; i++)
	{
		result[i] = AngleDiffDeg(angleTo[i], angleFrom[i]);
	}
	return result;
}

// Some helper functions on interpolation
inline float Lerp(float v0, float v1, float fPerc)
{
	return v0 + fPerc * (v1 - v0);
}

// Some helper functions on transformation

//////////////////////////////////////////////////////////////////////////
// resultCoord = startCoord_Inv * endCoord
inline vec3 CoordDiff(const vec3& startCoord, const vec3& endCoord)
{
	float angleRad = startCoord[VY] * Deg2Rad;
	float c = cos(angleRad);
	float s = sin(angleRad);
	vec3 temp = endCoord - startCoord;
	ClampAngleDeg(temp[VY]);
	return vec3( c * temp[VX] - s * temp[VZ], temp[VY], s * temp[VX] + c * temp[VZ]);
}

//////////////////////////////////////////////////////////////////////////
// |R | T| = |R_tranform | T_transform| * |R_original | T_original|
// |0 | 1|   |     0     |      1     |   |     0     |      1    |
inline vec3 AggregateTransformDeg(const vec3& transform, const vec3& original)
{
	float c = cos(transform[VY] * Deg2Rad);
	float s = sin(transform[VY] * Deg2Rad);
	vec3 result(c * original[VX] + s * original[VZ] + transform[VX], 
		        original[VY] + transform[VY], 
				-s * original[VX] + c * original[VZ] + transform[VZ]);
	ClampAngleDeg(result[VY]);
	return result;
}

inline vec3 AggregatePositionDeg(const vec3& startCoord, const vec3& nominalPosition)
{
	float c = cos(startCoord[VY] * Deg2Rad);
	float s = sin(startCoord[VY] * Deg2Rad);
	vec3 pos;
	pos[VX] =  c * nominalPosition[VX] + s * nominalPosition[VX];
	pos[VZ] = -s * nominalPosition[VZ] + c * nominalPosition[VZ];
	return vec3(pos[VX] + startCoord[VX], nominalPosition[VY], pos[VZ] + startCoord[VZ]);
}

inline vec3 TransformPositionDeg(const vec3& originalCoord, const vec3& originalPosition, const vec3& newCoord)
{
	Transform originalTransform;
	originalTransform.m_rotation = mat3::Rotation3DDeg(axisY, originalCoord[VY]);
	originalTransform.m_translation = vec3(originalCoord[VX], 0.0f, originalCoord[VZ]);
	Transform newTransform;
	newTransform.m_rotation = mat3::Rotation3DDeg(axisY, newCoord[VY]);
	newTransform.m_translation = vec3(newCoord[VX], 0.0f, newCoord[VZ]);
	vec3 newPosition = newTransform * originalTransform.Inverse() * originalPosition;
        return newPosition;
}

inline void TransformPositionVectorDeg(const vec3& originalCoord, vector<vec3>& vPosition, const vec3& newCoord)
{
	Transform originalTransform;
	originalTransform.m_rotation = mat3::Rotation3DDeg(axisY, originalCoord[VY]);
	originalTransform.m_translation = vec3(originalCoord[VX], 0.0f, originalCoord[VZ]);
	Transform newTransform;
	newTransform.m_rotation = mat3::Rotation3DDeg(axisY, newCoord[VY]);
	newTransform.m_translation = vec3(newCoord[VX], 0.0f, newCoord[VZ]);
	for (unsigned int i = 0; i < vPosition.size(); i++)
		vPosition[i] = newTransform * originalTransform.Inverse() * vPosition[i];
}
