/*
The MIT License (MIT)

Copyright (c) 2014 Jeremy C Brooks
http://vertexarray.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "VectorMath.h"
#include <limits>

float InvsSqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*)&x;  // store floating-point bits in integer
    i = 0x5f3759d5 - (i >> 1);      // initial guess for Newton's method
    x = *(float*)&i;              // convert new bits into float
    x = x*(1.5f - xhalf*x*x);       // One round of Newton's method
    return x;
}

float vmDistanceSqr(const Vector3D &a, const Vector3D &b)
{
	const Vector3D t(b-a);
	return (t.x*t.x + t.y*t.y + t.z*t.z);
}

float vmDistance(const Vector3D &a, const Vector3D &b)
{
	const Vector3D t(b-a);
	return sqrtf(t.x*t.x + t.y*t.y + t.z*t.z);
}

float vmInvsDistance(const Vector3D &a, const Vector3D &b)
{
	const Vector3D t(b-a);
	return InvsSqrt(t.x*t.x + t.y*t.y + t.z*t.z);
}

float vmMagnitude(const Vector3D &v)
{
	return 1.f / InvsSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector3D vmNormalize(const Vector3D &v)
{
	const float d = InvsSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	const float di = fabs(d)<epsilon?epsilon:d;
	return Vector3D(v*di);
}

Vector3D vmCrossProduct(const Vector3D &a, const Vector3D &b)
{
	return Vector3D(a.y*b.z - a.z*b.y,
					a.z*b.x - a.x*b.z,
					a.x*b.y - a.y*b.x);
}

float vmDotProduct(const Vector3D &a, const Vector3D &b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float vmGetAngle(const Vector3D &a, const Vector3D &b)
{
	float dp = vmDotProduct(a,b);
	if (dp>0.99999999f) return 0.f;
	else if (dp<-0.99999999f) return pi;
	else return acosf(dp);
}

const Vector3D operator-(const Vector3D &v)
{
	return Vector3D(-v.x, -v.y, -v.z);
}

const Vector3D operator-(const Vector3D &a, const Vector3D &b)
{
	return Vector3D(a.x - b.x,
					a.y - b.y,
					a.z - b.z);
}

const Vector3D operator+(const Vector3D &a, const Vector3D &b)
{
	return Vector3D(a.x + b.x,
					a.y + b.y,
					a.z + b.z);
}

const Vector3D operator*(const Vector3D &a, const float &b)
{
	return Vector3D(a.x * b,
					a.y * b,
					a.z * b);
}

const Vector3D operator/(const Vector3D &a, const float &b)
{
	return Vector3D(a.x / b,
					a.y / b,
					a.z / b);
}

void Matrix::LoadIdentity()
{
	memset(lpfMatrix, 0, 16*sizeof(float));
	memset(lpfInverse, 0, 16*sizeof(float));

	lpfMatrix[0] = lpfInverse[0] = 1.0;
	lpfMatrix[5] = lpfInverse[5] = 1.0;
	lpfMatrix[10]= lpfInverse[10]= 1.0;
	lpfMatrix[15]= lpfInverse[15]= 1.0;

	bInverseUpToDate = true;
}

void Matrix::Concatenate(float *m)
{
	float tm[16];
	int j;
	
	memset(tm, 0, sizeof(float)*16);
	tm[0] = tm[5] = tm[10] = tm[15] = 1.0f;

	for (int i=0; i<4; ++i)
	{
		j = 4*i;
		tm[j+0] = m[j]*lpfMatrix[0] + m[j+1]*lpfMatrix[4] + m[j+2]*lpfMatrix[8] + m[j+3]*lpfMatrix[12];
		tm[j+1] = m[j]*lpfMatrix[1] + m[j+1]*lpfMatrix[5] + m[j+2]*lpfMatrix[9] + m[j+3]*lpfMatrix[13];
		tm[j+2] = m[j]*lpfMatrix[2] + m[j+1]*lpfMatrix[6] + m[j+2]*lpfMatrix[10]+ m[j+3]*lpfMatrix[14];
		tm[j+3] = m[j]*lpfMatrix[3] + m[j+1]*lpfMatrix[7] + m[j+2]*lpfMatrix[11]+ m[j+3]*lpfMatrix[15];
	}
	memcpy(lpfMatrix, tm, sizeof(float)*16);
	bInverseUpToDate = false;
}

void Matrix::Concatenate(const Matrix &m)
{
	float tm[16];
	int j;
	
	memset(tm, 0, sizeof(float)*16);
	tm[0] = tm[5] = tm[10] = tm[15] = 1.0f;

	for (int i=0; i<4; ++i)
	{
		j = 4*i;
		tm[j+0] = m.lpfMatrix[j]*lpfMatrix[0] + m.lpfMatrix[j+1]*lpfMatrix[4] + m.lpfMatrix[j+2]*lpfMatrix[8] + m.lpfMatrix[j+3]*lpfMatrix[12];
		tm[j+1] = m.lpfMatrix[j]*lpfMatrix[1] + m.lpfMatrix[j+1]*lpfMatrix[5] + m.lpfMatrix[j+2]*lpfMatrix[9] + m.lpfMatrix[j+3]*lpfMatrix[13];
		tm[j+2] = m.lpfMatrix[j]*lpfMatrix[2] + m.lpfMatrix[j+1]*lpfMatrix[6] + m.lpfMatrix[j+2]*lpfMatrix[10]+ m.lpfMatrix[j+3]*lpfMatrix[14];
		tm[j+3] = m.lpfMatrix[j]*lpfMatrix[3] + m.lpfMatrix[j+1]*lpfMatrix[7] + m.lpfMatrix[j+2]*lpfMatrix[11]+ m.lpfMatrix[j+3]*lpfMatrix[15];
	}
	memcpy(lpfMatrix, tm, sizeof(float)*16);
	bInverseUpToDate = false;
}

void Matrix::Rotate(float angle, const Vector3D& v)
{
	Rotate(angle, v.x, v.y, v.z);
}

void Matrix::Rotate(float angle, float x, float y, float z)
{
	float tm[16];
	memset(tm, 0, sizeof(float)*16);
	tm[0] = tm[5] = tm[10] = tm[15] = 1.0f;

	const float c = cosf(angle);
	const float s = sinf(angle);
	const float v = 1.0f - c;

	const float xv = x*v;
	const float yv = y*v;
	const float xs = x*s;
	const float ys = y*s;
	const float zs = z*s;
	const float xvy = xv*y;
	const float xvz = xv*z;
	const float yvz = yv*z;

	tm[0] = xv*x + c;
	tm[1] = xvy - zs;
	tm[2] = xvz + ys;
	tm[4] = xvy + zs;
	tm[5] = yv*y + c;
	tm[6] = yvz - xs;
	tm[8] = xvz - ys;
	tm[9] = yvz + xs;
	tm[10]= z*z*v + c;

	Concatenate(tm);
}

void Matrix::Scale(float x, float y, float z)
{
	float tm[16];
	memset(tm, 0, sizeof(float)*16);
	tm[0] = tm[5] = tm[10] = tm[15] = 1.0f;

	tm[0] = x;
	tm[5] = y;
	tm[10]= z;
	Concatenate(tm);
}

void Matrix::Translate(const Vector3D& v)
{
	Translate(v.x, v.y, v.z);
}

void Matrix::Translate(float x, float y, float z)
{
	float tm[16];
	memset(tm, 0, sizeof(float)*16);
	tm[0] = tm[5] = tm[10] = tm[15] = 1.0f;

	tm[3] = x;
	tm[7] = y;
	tm[11]= z;
	
	Concatenate(tm);
}

void Matrix::UpdateInverse()
{
	Invert(lpfMatrix, lpfInverse);
	bInverseUpToDate = true;
}

void Matrix::UpdateNormal()
{
	float temp[16];
	Invert(lpfMatrix, temp);
	Transpose(temp, lpfNormal);
	bNormalUpToDate = true;
}

void Matrix::UpdateInverseNormal()
{
	if (!bNormalUpToDate) UpdateNormal();
	Invert(lpfNormal, lpfInverseNormal);
	bInverseNormalUpToDate = true;
}

void Matrix::Transpose(float *mat, float *tmat)
{
	int x, y;
	for(y=0;y<4;y++) {
		for (x=0;x<4;x++) {
			tmat[x*4 + y] = mat[y*4 + x];
		}
	}
}

void Matrix::Invert(float *mat, float *imat)
{
	float tmp[12];
	float src[16];
	float det;

	for (int i=0; i<4; i++)
	{
		src[i] = mat[i*4];
		src[i+4] = mat[i*4+1];
		src[i+8] = mat[i*4+2];
		src[i+12]= mat[i*4+3];
	}

	tmp[0] = src[10]*src[15];
	tmp[1] = src[11]*src[14];
	tmp[2] = src[9]*src[15];
	tmp[3] = src[11]*src[13];
	tmp[4] = src[9]*src[14];
	tmp[5] = src[10]*src[13];
	tmp[6] = src[8]*src[15];
	tmp[7] = src[11]*src[12];
	tmp[8] = src[8]*src[14];
	tmp[9] = src[10]*src[12];
	tmp[10] = src[8]*src[13];
	tmp[11] = src[9]*src[12];

	imat[0] =  tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	imat[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	imat[1] =  tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	imat[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	imat[2] =  tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	imat[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	imat[3] =  tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	imat[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	imat[4] =  tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	imat[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	imat[5] =  tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	imat[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	imat[6] =  tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	imat[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	imat[7] =  tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	imat[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];
	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10]= src[0]*src[5];
	tmp[11]= src[1]*src[4];

	imat[8] =  tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	imat[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	imat[9] =  tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	imat[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	imat[10]=  tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	imat[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	imat[11]=  tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	imat[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	imat[12]=  tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	imat[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	imat[13]=  tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	imat[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	imat[14]=  tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	imat[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	imat[15]=  tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	imat[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

	det = src[0]*lpfInverse[0] + src[1]*lpfInverse[1] + src[2]*lpfInverse[2] + src[3]*lpfInverse[3];

	if (det != 0)	det = 1.0f/det;
	else det = 1.0;
	for (int j=0;j<16;j++)
	{
		imat[j] *= det;
	}
}

void Matrix::Duplicate(const Matrix &m)
{
	bInverseUpToDate = false;
	memcpy(lpfMatrix, m.lpfMatrix, sizeof(float)*16);
}

void Matrix::FromArray(float* m)
{
	bInverseUpToDate = false;
	memcpy(lpfMatrix, m, sizeof(float)*16);
}

void Matrix::GetMatrixArray(float *tMat)
{
	Transpose(lpfMatrix, tMat);
}

void Ortho(float Ortho[16], const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal)
{
	memset(Ortho, 0, sizeof(float)*16);
		
	float temp2=right-left;
	float temp3=top-bottom;
	float temp4=farVal-nearVal;

	Ortho[0] = 2.0f/temp2;
	Ortho[5] = 2.0f/temp3;
	Ortho[10] = -2.0f/temp4;
	Ortho[15] = 1.0f;
		
	Ortho[12] = (-right+-left)/temp2;
	Ortho[13] = (-top-bottom)/temp3;
	Ortho[14] = (-farVal-nearVal)/temp4;
}

void gldLoadIdentity(float* m)
{
	memset(m, 0, sizeof(float)*16);
	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10]= 1.0f;
	m[15]= 1.0f;
}
 
static void __gluMultMatricesd(const float a[16], const float b[16], float r[16])
{
    int i, j;

    for (i = 0; i < 4; i++) {
	for (j = 0; j < 4; j++) {
	    r[i*4+j] = 
		a[i*4+0]*b[0*4+j] +
		a[i*4+1]*b[1*4+j] +
		a[i*4+2]*b[2*4+j] +
		a[i*4+3]*b[3*4+j];
	}
    }
}

static int __gluInvertMatrixd(const float m[16], float invOut[16])
{
    float inv[16], det;
    int i;

    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
             + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
             - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
             + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
             - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
             - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
             + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
             - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
             + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
             + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
             - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
             + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
             - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
             - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
             + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
             - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
             + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0)
        return 0;

    det = 1.f / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return 1;
}

static void __gluMultMatrixVecd(const float matrix[16], const float in[4], float out[4])
{
    int i;

    for (i=0; i<4; i++) {
	out[i] = 
	    in[0] * matrix[0*4+i] +
	    in[1] * matrix[1*4+i] +
	    in[2] * matrix[2*4+i] +
	    in[3] * matrix[3*4+i];
    }
}

int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate)
{
    float finalMatrix[16];
    float in[4];
    float out[4];

    __gluMultMatricesd(modelview, projection, finalMatrix);
    if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(0);

    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=1.0;

    /* Map x and y from window coordinates */
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];

    /* Map to range -1 to 1 */
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;

    __gluMultMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(0);
    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
    objectCoordinate[0] = out[0];
    objectCoordinate[1] = out[1];
    objectCoordinate[2] = out[2];
    return(1);
  }
 

float IntersectRayPlane(const Vector3D& rOrigin, const Vector3D& rVector, const Vector3D& pOrigin, const Vector3D& pNormal) {
  
  float d = -(vmDotProduct(pNormal,pOrigin));
 
  float numer = vmDotProduct(pNormal,rOrigin) + d;
  float denom = vmDotProduct(pNormal,rVector);
  
  if (denom == 0.f)
   return (-1.0f);
   
  return -(numer / denom);	
}
