#ifndef VECTORMATH_H_
#define VECTORMATH_H_

#include <stdlib.h>
#include <math.h>

const float epsilon = 0.00001f;                         ///< floating point epsilon for single precision. todo: verify epsilon value and usage
const float epsilonSquared = epsilon * epsilon;         ///< epsilon value squared
const float pi = 3.1415926f;                            ///< pi stored at a reasonable precision for single precision floating point.
const float PI_OVER_180	= 0.0174532925199432f;
const float PI_OVER_360 = 0.0087266462599716f;
const float ONE_OVER_PI = 1.f/pi;
const float ONE_OVER_2PI = 1.f/(2.f*pi);
const float DEG2RAD = (pi/180.f);
const float RAD2DEG = (180.f/pi);

struct Vector3D {
	float x, y, z;

	Vector3D():x(0.f),y(0.f),z(0.f){}
    Vector3D(float _x, float _y, float _z):x(_x),y(_y),z(_z){}
    Vector3D(float _v):x(_v),y(_v),z(_v){}
};

const float FRAND_MAX = (float)RAND_MAX;
inline float frand()
{
	return (float)rand()/FRAND_MAX;
}

Vector3D vmNormalize(const Vector3D &v);
Vector3D vmCrossProduct(const Vector3D &a, const Vector3D &b);
Vector3D vmReflect(const Vector3D &view, const Vector3D &normal);
float vmDistanceSqr(const Vector3D &a, const Vector3D &b);
float vmDistance(const Vector3D &a, const Vector3D &b);
float vmInvsDistance(const Vector3D &a, const Vector3D &b);
float vmMagnitude(const Vector3D &v);
float vmDotProduct(const Vector3D &a, const Vector3D &b);
float vmGetAngle(const Vector3D &a, const Vector3D &b);

float IntersectRayPlane(const Vector3D& rOrigin, const Vector3D& rVector, const Vector3D& pOrigin, const Vector3D& pNormal);

const Vector3D operator-(const Vector3D &v);
const Vector3D operator-(const Vector3D &a, const Vector3D &b);
const Vector3D operator+(const Vector3D &a, const Vector3D &b);
const Vector3D operator*(const Vector3D &a, const float &b);
inline const Vector3D operator*(const float &a, const Vector3D &b) { return b*a; }
const Vector3D operator/(const Vector3D &a, const float &b);
inline float operator*(const Vector3D &a, const Vector3D &b) { return vmDotProduct(a, b); }
inline const Vector3D operator^(const Vector3D &a, const Vector3D &b) { return vmCrossProduct(a, b);}
inline const bool operator==(const Vector3D &a, const Vector3D &b) { return (a.x==b.x)&&(a.y==b.y)&&(a.z==b.z);}
inline const bool operator!=(const Vector3D &a, const Vector3D &b) { return !(a==b);}

// Matrix Stuff
class Matrix {
public:
	Matrix():bInverseUpToDate(false), bNormalUpToDate(false), bInverseNormalUpToDate(false) {LoadIdentity();};
	Matrix(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33){

	}

	void LoadIdentity();
	void Rotate(float angle, float x, float y, float z);
	void Rotate(float angle, const Vector3D& v);
	void Translate(float x, float y, float z);
	void Translate(const Vector3D& v);
	void Scale(float x, float y, float z);
	void Duplicate(const Matrix &m);
	void FromArray(float*);
	void Concatenate(const Matrix&);
	void Concatenate(float *m);

	Matrix Inverse();

	Vector3D Transform(const Vector3D &v);
	void Transform4(float* in, float* out);
	Vector3D InverseTransform(const Vector3D &v);
	Vector3D NormalTransform(const Vector3D &v);
	Vector3D InverseNormalTransform(const Vector3D &v);
	
	void GetMatrixArray(float *tMat);

private:
	float lpfMatrix[16];
	float lpfInverse[16];
	float lpfNormal[16];
	float lpfInverseNormal[16];
	bool bInverseUpToDate;
	bool bNormalUpToDate;
	bool bInverseNormalUpToDate;
	void Invert(float *mat, float *imat);
	void Transpose(float *mat, float *tmat);
	void UpdateInverse();
	void UpdateNormal();
	void UpdateInverseNormal();
};

void Ortho(float Ortho[16], const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal);
void gldLoadIdentity(float*);
int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate);
int glhProjectf(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate);

#endif