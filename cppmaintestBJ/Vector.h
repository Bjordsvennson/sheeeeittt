#pragma once

#include "Common.h"

#define M_PI 3.1415925f
#define M_PI_F ((float)(M_PI))

#define FLOAT32_NAN_BITS (uint32)0x7FC00000	// not a number!

#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )

inline unsigned long& FloatBits(float& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline unsigned long const& FloatBits(float const& f)
{
	return *reinterpret_cast<unsigned long const*>(&f);
}

inline float BitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

#define FLOAT32_NAN BitsToFloat( FLOAT32_NAN_BITS )

inline bool IsFinite(float f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

inline float FloatNegate(float f)
{
	return BitsToFloat(FloatBits(f) ^ 0x80000000);
}

class Vector
{
public:
	float x, y, z;

	Vector() { x = y = z = 0; };
	Vector(float X, float Y, float Z) { x = X; y = Y; z = Z; };
	explicit Vector(float XYZ) { x = y = z = XYZ; };

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	};

	inline float operator[](int i) const
	{
		return ((float const*)this)[i];
	};

	inline float& operator[](int i)
	{
		return ((float*)this)[i];
	};

	inline Vector& operator+(Vector& v)
	{
		Vector res;
		res.x = x + v.x;
		res.y = y + v.y;
		res.z = z + v.z;

		return res;
	};

	inline Vector& operator*(Vector& v)
	{
		Vector res;
		res.x = x * v.x;
		res.y = y * v.y;
		res.z = z * v.z;
		return res;
	};

	inline Vector& operator *= (const Vector &in)
	{
		x *= in.x;
		y *= in.y;
		z *= in.z;

		return *this;
	};

	inline Vector& operator *= (const float in)
	{
		x *= in;
		y *= in;
		z *= in;

		return *this;
	};

	inline Vector& operator+=(Vector& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	};

	inline Vector& operator-=(Vector& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	};

	inline Vector& operator*(float f) const
	{
		Vector res;
		res.x = x * f;
		res.y = y * f;
		res.z = z * f;
		return res;
	};

	inline Vector& operator-(Vector& v)
	{
		Vector res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	};

	inline float DistTo(const Vector& vOther)
	{
		Vector delta;
		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;
		return delta.Length();
	};

	inline float Length()
	{
		return sqrtf(x * x + y * y + z * z);
	};

	inline float Length2D() const
	{
		return sqrtf(x * x + y * y);
	}

	inline void Zero()
	{
		x = y = z = 0.f;
	}

	inline bool IsZero()
	{
		return x == 0.f && y == 0.f && z == 0.f;
	};

	inline float LengthSqr() const
	{
		return (x * x + y * y + z * z);
	}
};

typedef Vector QAngle;
typedef float vec_t;

struct matrix3x4
{
	matrix3x4() {}
	matrix3x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	matrix3x4(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	float* operator[](int i) { return m_flMatVal[i]; }
	const float* operator[](int i) const { return m_flMatVal[i]; }
	float* Base() { return &m_flMatVal[0][0]; }
	const float* Base() const { return &m_flMatVal[0][0]; }
	float m_flMatVal[3][4];
};

struct matrix4x4
{
	inline float* operator[](int i)
	{
		return m[i];
	};
	inline const float* operator[](int i) const
	{
		return m[i];
	};

	vec_t m[4][4];
};

class VectorAligned : public Vector
{
public:
	VectorAligned()
	{
		x = y = z = 0;
	}

	VectorAligned(const Vector &in)
	{
		x = in.x;
		y = in.y;
		z = in.z;
	}

	float w;
};

inline void SinCos(float flRadians, float *flSine, float *flCosine)
{
	*flSine = sin(flRadians);
	*flCosine = cos(flRadians);
}

inline void NormalizeAngle(float &angle)
{
	while (angle > 180.f)
		angle -= 360.f;

	while (angle < -180.f)
		angle += 360.f;
}

inline void NormalizeAngles(QAngle &vecAngles)
{
	for (int iAxis = 0; iAxis < 3; iAxis++)
	{
		while (vecAngles[iAxis] > 180.f)
			vecAngles[iAxis] -= 360.f;

		while (vecAngles[iAxis] < -180.f)
			vecAngles[iAxis] += 360.f;
	}
}

inline Vector NormalizeVector(Vector vecInput)
{
	float flLength = vecInput.Length();
	float flLengthNormal = 1.f / (FLT_EPSILON + flLength);
	return vecInput * flLengthNormal;
}

inline static void CrossProduct(const Vector &vecOne, const Vector &vecTwo, Vector &vecCross)
{
	vecCross.x = ((vecOne.y * vecTwo.z) - (vecOne.z * vecTwo.y));
	vecCross.y = ((vecOne.z * vecTwo.x) - (vecOne.x * vecTwo.z));
	vecCross.z = ((vecOne.x * vecTwo.y) - (vecOne.y * vecTwo.x));
}

inline static void AngleVectors(const Vector &vecAngles, Vector *vecForward)
{
	float flSinPitch, flSinYaw, flCosPitch, flCosYaw;

	SinCos(DEG2RAD(vecAngles.x), &flSinPitch, &flCosPitch);
	SinCos(DEG2RAD(vecAngles.y), &flSinYaw, &flCosYaw);

	vecForward->x = flCosPitch * flCosYaw;
	vecForward->y = flCosPitch * flSinYaw;
	vecForward->z = -flSinPitch;
}

inline static void AngleVectors(const Vector &vecAngles, Vector *vecForward, Vector *vecRight, Vector *vecUp)
{
	float flSinPitch, flSinYaw, flSinRoll, flCosPitch, flCosYaw, flCosRoll;

	SinCos(RAD2DEG(vecAngles.x), &flSinPitch, &flCosPitch);
	SinCos(RAD2DEG(vecAngles.y), &flSinYaw, &flCosYaw);
	SinCos(RAD2DEG(vecAngles.z), &flSinRoll, &flCosRoll);

	if (vecForward)
	{
		vecForward->x = flCosPitch * flCosYaw;
		vecForward->y = flCosPitch * flSinYaw;
		vecForward->z = -flSinPitch;
	}

	if (vecRight)
	{
		vecRight->x = -1.f * flSinRoll * flSinPitch * flCosYaw + -1.f * flCosRoll * -flSinYaw;
		vecRight->y = -1.f * flSinRoll * flSinPitch * flSinYaw + -1.f * flCosRoll * flCosYaw;
		vecRight->z = -1.f * flSinRoll * flCosPitch;
	}

	if (vecUp)
	{
		vecUp->x = flCosRoll * flSinPitch * flCosYaw + -flSinRoll * -flSinYaw;
		vecUp->y = flCosRoll * flSinPitch * flSinYaw + -flSinRoll * flCosYaw;
		vecUp->z = flCosRoll * flCosPitch;
	}
}

inline static void VectorAngles(const Vector &forward, QAngle &angles)
{
	if (forward.x == 0 && forward.y == 0)
	{
		if (forward.z > 0.f)
			angles.x = 270.f;
		else
			angles.x = 90.f;

		angles.y = 0.f;
	}
	else
	{
		angles.x = DEG2RAD(atan2(-forward.z, sqrtf(forward.x * forward.x + forward.y * forward.y)));
		angles.y = DEG2RAD(atan2(forward.y, forward.x));

		if (angles.x < 0.f)
			angles.x += 360.f;

		if (angles.y < 0.f)
			angles.y += 360.f;
	}

	angles.z = 0.f;
}

inline static void VectorAngles(const Vector &vecDirection, const Vector &vecUp, Vector &vecAngles)
{
	Vector vecLeft;
	CrossProduct(vecUp, vecDirection, vecLeft);

	float flLength2D = sqrtf((vecDirection.x * vecDirection.x) + (vecDirection.y * vecDirection.y));
	vecAngles.x = RAD2DEG(atan2(-vecDirection.z, flLength2D));

	if (flLength2D > .001f)
	{
		vecAngles.y = RAD2DEG(atan2(vecDirection.y, vecDirection.x));

		// todo: if > 84 || < -84, sqrt / dunno if this is needed
		float flUp = ((vecLeft.y * vecDirection.x) - (vecLeft.x * vecDirection.y));
		vecAngles.z = RAD2DEG(atan2(vecLeft.z, flUp));
	}
	else
	{
		vecAngles.y = RAD2DEG(atan2(-vecLeft.x, vecLeft.y));
		vecAngles.z = 0.f;
	}

	NormalizeAngles(vecAngles);
}

inline static void VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}


inline static void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;

}

inline static void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

inline static void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}