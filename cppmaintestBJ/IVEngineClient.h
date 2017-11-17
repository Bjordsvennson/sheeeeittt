#pragma once

struct player_info_t;
struct matrix4x4;
class Vector;
typedef Vector QAngle;

class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height);
	bool GetPlayerInfo(int ent_num, player_info_t* pinfo);
	bool Con_IsVisible(void);
	int	GetLocalPlayer(void);
	void GetViewAngles(QAngle& va);
	void SetViewAngles(QAngle& va);
	bool IsInGame(void);
	bool IsConnected(void);
	const matrix4x4& WorldToScreenMatrix();
	void ClientCmd_Unrestricted(const char* szCmdString);
};

extern IVEngineClient* g_pEngine;
