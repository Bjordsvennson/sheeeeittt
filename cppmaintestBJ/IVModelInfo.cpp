#include "IVModelInfo.h"
#include "VTable.h"
#include "CBaseEntity.h"

const char* IVModelInfo::GetModelName(const model_t* model)
{
	typedef const char*(__thiscall* Fn)(void*, const model_t*);
	return ((Fn)(vtablehook_getfunction(this, 2)))(this, model);
}

studiohdr* IVModelInfo::GetStudiomodel(const model_t* model)
{
	typedef studiohdr*(__thiscall* Fn)(void*, const model_t*);
	return ((Fn)(vtablehook_getfunction(this, 27)))(this, model);
}

IVModelInfo* g_pModelInfo;