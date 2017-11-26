//#include "Vector.h"
#include "CBaseEntity.h"
#include "NetVarManager.h"
#include "IVEngineClient.h"
#include "Offsets.h"
#include "IVModelInfo.h"
#include "VTable.h"

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

model_t* CBaseEntity::GetModel()
{
	void* clientRenderable = (void*)(this + 0x4);
	typedef model_t*(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(clientRenderable, 9))(clientRenderable);
}

bool CBaseEntity::SetupBones(matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	void* clientRenderable = (void*)(this + 0x4);
	typedef bool(__thiscall* Fn)(void*, matrix3x4*, int, int, float);
	return ((Fn)vtablehook_getfunction(clientRenderable, 16))(clientRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

Vector CBaseEntity::GetBonePosition(int iBone)
{
	matrix3x4 boneMatrixes[128];
	if (this->SetupBones(boneMatrixes, 128, 0x100, 0))
	{
		matrix3x4 boneMatrix = boneMatrixes[iBone];
		return Vector(boneMatrix.m_flMatVal[0][3], boneMatrix.m_flMatVal[1][3], boneMatrix.m_flMatVal[2][3]);
	}
	else
		return Vector(0, 0, 0);
}

mstudiobbox_t * mstudiohitboxset_t::hitbox(int i)
{
	return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i;
}

mstudiohitboxset_t* hboxsets[0xFFFFFFF];

mstudiohitboxset_t * CBaseEntity::GetHBoxSet()
{
	int index = this->index;
	if (!index)
		return 0;

	return hboxsets[index];
}

Vector CBaseEntity::GetAimPunch()
{
	return *(Vector*)((DWORD)this + offsets.m_Local + offsets.m_vecPunchAngle);
}
