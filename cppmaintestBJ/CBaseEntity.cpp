//#include "Vector.h"
#include "CBaseEntity.h"
#include "NetVarManager.h"
#include "IVEngineClient.h"
#include "Offsets.h"
#include "IVModelInfo.h"
#include "VTable.h"

model_t* CBaseEntity::GetModel()
{
	//void* clientRenderable = (void*)(this + 4);
	typedef model_t*(__thiscall* Fn)(void*);
	return ((Fn)(vtablehook_getfunction(this, 9)))(this);
}

/*
player_info_t CBaseEntity::GetPlayerInfo()
{
	player_info_t pinfo;
	g_pEngine->GetPlayerInfo(this->index, &pinfo);
	return pinfo;
}
*/
int CBaseEntity::GetHealth()
{
	return *(int*)((DWORD)this + offsets.m_iHealth);
}

Vector CBaseEntity::GetOrigin()
{
	return *(Vector*)((DWORD)this + offsets.m_vecOrigin);
}

int CBaseEntity::GetFlags()
{
	return *(int*)((DWORD)this + offsets.m_fFlags);
}

int CBaseEntity::GetTeam()
{
	return *(int*)((DWORD)this + offsets.m_iTeamNum);
}

Vector CBaseEntity::GetEyePosition()
{
	Vector origin = this->GetOrigin();
	Vector offset = *(Vector*)((DWORD)this + offsets.m_vecViewOffset);

	return (origin + offset);
}

ICollideable * CBaseEntity::GetCollideable()
{
	return (ICollideable*)((DWORD)this + offsets.m_Collision);
}

mstudiobbox_t * mstudiohitboxset::hitbox(int i)
{
	return (mstudiobbox_t*)(((DWORD)this) + hitboxindex) + i;
}

mstudiohitboxset* hboxsets[0xFFFFFFF];

mstudiohitboxset * CBaseEntity::GetHBoxSet()
{
	int index = this->index;
	if (!index)
		return 0;

	return hboxsets[index];
}

Vector CBaseEntity::GetAimPunch()
{
	return *(Vector*)((DWORD)this + offsets.m_vecPunchAngle);
}
