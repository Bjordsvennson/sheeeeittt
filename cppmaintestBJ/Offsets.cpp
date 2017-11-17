#include "offsets.h"
#include "NetVarManager.h"

COffsets offsets;

void GrabOffsets()
{
	offsets.m_iHealth = netvars.FindOffset("DT_BasePlayer", "m_iHealth");
	offsets.m_vecOrigin = netvars.FindOffset("DT_BaseEntity", "m_vecOrigin");
	offsets.m_vecViewOffset = netvars.FindOffset("DT_BaseFlex", "m_vecViewOffset[0]");
	offsets.m_Collision = netvars.FindOffset("DT_BaseEntity", "m_Collision");
	offsets.m_fFlags = netvars.FindOffset("DT_BasePlayer", "m_fFlags");
	offsets.m_iTeamNum = netvars.FindOffset("DT_BaseEntity", "m_iTeamNum");
	offsets.m_vecPunchAngle = netvars.FindOffset("DT_BasePlayer", "m_vecPunchAngle"); /// if you are recieving crashes it's from here. m_vecPunchAngle isn't a direct child
																					  /// of DT_BasePlayer but I think that it goes DT_BasePlayer>localdata>m_vecPunchAngle
}
