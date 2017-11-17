#include "IVEngineClient.h"
#include "Vector.h"
#include "VTable.h"

void IVEngineClient::GetScreenSize(int & width, int & height)
{
	typedef void(__thiscall* Fn)(void*, int&, int&);
	return ((Fn)vtablehook_getfunction(this, 5))(this, width, height);
}

bool IVEngineClient::GetPlayerInfo(int ent_num, player_info_t * pinfo)
{
	typedef bool(__thiscall* Fn)(void*, int, player_info_t*);
	return ((Fn)vtablehook_getfunction(this, 8))(this, ent_num, pinfo);
}

bool IVEngineClient::Con_IsVisible(void)
{
	typedef bool(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 11))(this);
}

int IVEngineClient::GetLocalPlayer(void)
{
	typedef int(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 12))(this);
}

void IVEngineClient::GetViewAngles(QAngle & va)
{
	typedef void(__thiscall* Fn)(void*, QAngle&);
	return ((Fn)vtablehook_getfunction(this, 19))(this, va);
}

void IVEngineClient::SetViewAngles(QAngle & va)
{
	typedef void(__thiscall* Fn)(void*, QAngle&);
	return ((Fn)vtablehook_getfunction(this, 20))(this, va);
}

bool IVEngineClient::IsInGame(void)
{
	typedef bool(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 26))(this);
}

bool IVEngineClient::IsConnected(void)
{
	typedef bool(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 27))(this);
}

const matrix4x4 & IVEngineClient::WorldToScreenMatrix()
{
	typedef const matrix4x4&(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 36))(this); // was 37
}

void IVEngineClient::ClientCmd_Unrestricted(const char* szCmdString)
{
	typedef void(__thiscall* Fn)(void*, const char*);
	return ((Fn)vtablehook_getfunction(this, 106))(this, szCmdString);
}

/// biggie says ClientCmd_Unrestricted is on index 106

IVEngineClient* g_pEngine;