#pragma once
/*
#include "Common.h"

typedef float vec_t;

class QAngleByValue;

class QAngle
{
public:

	float x, y, z;

	QAngle(void)
	{
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
		// Initialize to NAN to catch errors
		x = y = z = float_NAN;
#endif
#endif
	};
	QAngle(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	};

	QAngle Normalized()
	{
		if (this->x != this->x)
			this->x = 0;
		if (this->y != this->y)
			this->y = 0;
		if (this->z != this->z)
			this->z = 0;

		if (this->x > 89.f)
			this->x = 89.f;
		if (this->x < -89.f)
			this->x = -89.f;

		while (this->y > 180)
			this->y -= 360;
		while (this->y <= -180)
			this->y += 360;

		if (this->y > 180.f)
			this->y = 180.f;
		if (this->y < -180.f)
			this->y = -180.f;

		this->z = 0;

		return *this;
	};

	operator QAngleByValue &() { return *((QAngleByValue *)(this)); }
	operator const QAngleByValue &() const { return *((const QAngleByValue *)(this)); }

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	};

	//void Random(vec_t minVal, vec_t maxVal);

	float operator[](int i) const
	{
		return ((float*)this)[i];
	};
	float& operator[](int i)
	{
		return ((float*)this)[i];
	};

	float* Base()
	{
		return (float*)this;
	};
	float const* Base() const
	{
		return (float const*)this;
	};

	//inline void Zero();

	bool operator==(const QAngle& v) const
	{
		return (v.x == x) && (v.y == y) && (v.z == z);
	};
	bool operator!=(const QAngle& v) const
	{
		return (v.x != x) || (v.y != y) || (v.z != z);
	};
	QAngle& operator=(const QAngle& vOther)
	{
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;
		return *this;
	};//
	QAngle&	operator+=(const QAngle &v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	};//
	QAngle&	operator-=(const QAngle &v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	};
	QAngle&	operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	};
	QAngle&	operator/=(float s)
	{
		float oofl = 1.0f / s;
		x *= oofl;
		y *= oofl;
		z *= oofl;
		return *this;
	};
	QAngle	operator-(void) const
	{
		QAngle ret(-x, -y, -z);
		return ret;
	};
	QAngle	operator+(const QAngle& v) const
	{
		QAngle res;
		res.x = x + v.x;
		res.y = y + v.y;
		res.z = z + v.z;
		return res;
	};
	QAngle	operator-(const QAngle& v) const
	{
		QAngle res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	};
	QAngle	operator*(float fl) const
	{
		QAngle res;
		res.x = x * fl;
		res.y = y * fl;
		res.z = z * fl;
		return res;
	};
	QAngle	operator/(float fl) const
	{
		QAngle res;
		res.x = x / fl;
		res.y = y / fl;
		res.z = z / fl;
		return res;
	};
};
*/