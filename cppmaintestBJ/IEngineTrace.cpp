#include "IEngineTrace.h"
#include "VTable.h"

void IEngineTrace::TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
{
	typedef void(__thiscall* Fn)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
	return ((Fn)vtablehook_getfunction(this, 4))(this, ray, fMask, pTraceFilter, pTrace);
}

IEngineTrace* g_pEngineTrace;