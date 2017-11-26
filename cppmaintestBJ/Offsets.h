#pragma once

typedef unsigned long DWORD;

struct COffsets
{
	DWORD m_iHealth;
	DWORD m_vecOrigin;
	DWORD m_vecViewOffset;
	DWORD m_Collision;
	DWORD m_fFlags;
	DWORD m_iTeamNum;
	DWORD m_Local;
	DWORD m_vecPunchAngle;
};

extern COffsets offsets;

void GrabOffsets();