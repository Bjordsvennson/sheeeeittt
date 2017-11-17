#include "IVModelInfo.h"
#include "VTable.h"

const char* IVModelInfo::GetModelName(const model_t* model)
{
	typedef const char*(__thiscall* Fn)(void*, const model_t*);
	return ((Fn)(vtablehook_getfunction(this, 3)))(this, model);
}

void* IVModelInfo::GetStudiomodel(const model_t* model)
{
	typedef void*(__thiscall* Fn)(void*, const model_t*);
	return ((Fn)(vtablehook_getfunction(this, 28)))(this, model);
}

IVModelInfo* g_pModelInfo;