#pragma once

class CBaseEntity;

typedef unsigned long DWORD;

class IClientEntityList
{
public:
	virtual void __pad000() = 0;
	virtual void __pad001() = 0;
	virtual void __pad002() = 0;
	virtual CBaseEntity* GetClientEntity(int iIndex) = 0; // 3
	virtual CBaseEntity* GetClientEntityFromHandle(DWORD hHandle) = 0; // 4
	virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0; // 5
	virtual int GetHighestEntityIndex() = 0; // 6
};

extern IClientEntityList* entitylist;