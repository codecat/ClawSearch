/*  libscratch - Multipurpose objective C++ library.
    
    Copyright (c) 2015 Angelo Geels <spansjh@gmail.com>
    
    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:
    
    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "Common.h"

SCRATCH_NAMESPACE_BEGIN;

class SCRATCH_EXPORT Vector3f
{
public:
  FLOAT x, y, z;

public:
  Vector3f(void);
  Vector3f(FLOAT f);
  Vector3f(FLOAT fX, FLOAT fY, FLOAT fZ);
  ~Vector3f(void);

  void ClampX(const FLOAT min, const FLOAT max);
  void ClampY(const FLOAT min, const FLOAT max);
  void ClampZ(const FLOAT min, const FLOAT max);

  FLOAT Angle(const Vector3f &vOrigin);
  FLOAT Length(void);
  FLOAT Dot(const Vector3f &v2);

  void Normalize(void);

  Vector3f operator +(const Vector3f &v);
  Vector3f operator -(const Vector3f &v);
  Vector3f operator *(const Vector3f &v);
  Vector3f operator *(const FLOAT f);
  Vector3f operator /(const Vector3f &v);
  Vector3f operator /(const FLOAT f);

  BOOL operator ==(const Vector3f &v);
  BOOL operator !=(const Vector3f &v);

  Vector3f& operator +=(const Vector3f &v);
  Vector3f& operator -=(const Vector3f &v);
  Vector3f& operator *=(const Vector3f &v);
  Vector3f& operator *=(const FLOAT f);
  Vector3f& operator /=(const Vector3f &v);
  Vector3f& operator /=(const FLOAT f);
};

#ifdef SCRATCH_IMPL

Vector3f::Vector3f(void)
{
	x = y = z = 0.0f;
}

Vector3f::Vector3f(FLOAT f)
{
	x = y = z = f;
}

Vector3f::Vector3f(FLOAT fX, FLOAT fY, FLOAT fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

Vector3f::~Vector3f(void)
{
}

void Vector3f::ClampX(const FLOAT min, const FLOAT max)
{
	if (x < min) {
		x = min;
	} else if (x > max) {
		x = max;
	}
}

void Vector3f::ClampY(const FLOAT min, const FLOAT max)
{
	if (y < min) {
		y = min;
	} else if (y > max) {
		y = max;
	}
}

void Vector3f::ClampZ(const FLOAT min, const FLOAT max)
{
	if (z < min) {
		z = min;
	} else if (z > max) {
		z = max;
	}
}

FLOAT Vector3f::Angle(const Vector3f &vOrigin)
{
	return atan2(y - vOrigin.y, x - vOrigin.x);
}

FLOAT Vector3f::Length(void)
{
	return sqrt(x*x + y*y + z*z);
}

FLOAT Vector3f::Dot(const Vector3f &v2)
{
	return x*v2.x + y*v2.y + z*v2.z;
}

void Vector3f::Normalize(void)
{
	FLOAT fLength = Length();
	x /= fLength;
	y /= fLength;
	z /= fLength;
}

Vector3f Vector3f::operator +(const Vector3f &v)
{
	return Vector3f(x + v.x, y + v.y, z + v.z);
}

Vector3f Vector3f::operator -(const Vector3f &v)
{
	return Vector3f(x - v.x, y - v.y, z - v.z);
}

Vector3f Vector3f::operator *(const Vector3f &v)
{
	return Vector3f(x*v.x, y*v.y, z*v.z);
}

Vector3f Vector3f::operator *(const FLOAT f)
{
	return Vector3f(x*f, y*f, z*f);
}

Vector3f Vector3f::operator /(const Vector3f &v)
{
	return Vector3f(x / v.x, y / v.y, z / v.z);
}

Vector3f Vector3f::operator /(const FLOAT f)
{
	return Vector3f(x / f, y / f, z / f);
}

BOOL Vector3f::operator ==(const Vector3f &v)
{
	return x == v.x && y == v.y && z == v.z;
}

BOOL Vector3f::operator !=(const Vector3f &v)
{
	return !operator==(v);
}

Vector3f& Vector3f::operator +=(const Vector3f &v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3f& Vector3f::operator -=(const Vector3f &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3f& Vector3f::operator *=(const Vector3f &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

Vector3f& Vector3f::operator *=(const FLOAT f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

Vector3f& Vector3f::operator /=(const Vector3f &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

Vector3f& Vector3f::operator /=(const FLOAT f)
{
	x /= f;
	y /= f;
	z /= f;

	return *this;
}

#endif

SCRATCH_NAMESPACE_END;
