#pragma once

#include "basehandle.h"
#include "CBaseEntity.h"
#include "Vector.h"

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(CBaseEntity *pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(CBaseEntity* pEntity, int contentsMask)
	{
		return !(pEntity == pSkip);
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

struct trace_t
{
	Vector			startpos;
	Vector			endpos;
	Vector			normal;
	float			dist;
	BYTE			type;
	BYTE			signbits;
	BYTE			pad[2];
	float			fraction;
	int				contents;
	unsigned short	dispFlags;
	bool			allsolid;
	bool			startsolid;
	float			fractionleftsolid;
	const char		*name;
	short			surfaceProps;
	unsigned short	flags;
	int				hitgroup;
	short			physicsbone;
	CBaseEntity*	m_pEnt;
	int				hitbox;

};

struct Ray_t
{
	VectorAligned  m_Start;
	VectorAligned  m_Delta;
	VectorAligned  m_StartOffset;
	VectorAligned  m_Extents;

	bool m_IsRay;
	bool m_IsSwept;

	void Init(Vector const& start, Vector const& end)
	{
		VectorSubtract(end, start, m_Delta);

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Zero();

		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset.Zero();
		VectorCopy(start, m_Start);

	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		VectorSubtract(end, start, m_Delta);
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorSubtract(maxs, mins, m_Extents);
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd(mins, maxs, m_StartOffset);
		m_StartOffset *= 0.5f;
		VectorAdd(start, m_StartOffset, m_Start);
		m_StartOffset *= -1.0f;
	}

};

class IEngineTrace
{
public:
	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace);
};

extern IEngineTrace* g_pEngineTrace;