#include "IClientEntity.h"
#include "VTable.h"

CBaseEntity* IClientEntityList::GetClientEntity(int iIndex)
{
	typedef CBaseEntity*(__thiscall* Fn)(void*, int);
	return ((Fn)vtablehook_getfunction(this, 3))(this, iIndex);
}

CBaseEntity* IClientEntityList::GetClientEntityFromHandle(DWORD hHandle)
{
	typedef CBaseEntity*(__thiscall* Fn)(void*, DWORD);
	return ((Fn)vtablehook_getfunction(this, 4))(this, hHandle);
}

int IClientEntityList::NumberOfEntities(bool bIncludeNonNetworkable)
{
	typedef int(__thiscall* Fn)(void*, bool);
	return ((Fn)vtablehook_getfunction(this, 4))(this, bIncludeNonNetworkable);
}

int IClientEntityList::GetHighestEntityIndex()
{
	typedef int(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 4))(this);
}

IClientEntityList* entitylist;