#include "Common.h"
#include "VTable.h"
#include "Offsets.h"
#include "Misc.h"
#include "NetVarManager.h"

#include "Vector.h"

#include "CGlobalVarsBase.h"
#include "CBaseEntity.h"
#include "IBaseClientDLL.h"
#include "IClientEntity.h"
#include "IInputSystem.h"
#include "IPanel.h"
#include "ISurface.h"
#include "IVEngineClient.h"
#include "IClientMode.h"
#include "IEngineTrace.h"
#include "IVModelInfo.h"

///	
///	TODO: Studio hitboxes, nospread.
///
///	ALSO: Put functions like worldtoscreen and toscreen in BJDrawing where they belong.
///		  Take jumbles of code in hooks and put them in a class as functions.
///

// interfaces
#define ID_PAINTTRAVERSE 41
#define ID_CREATEMOVE 21
#define ID_OVERRIDEVIEW 16

RECT GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	g_pEngine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

bool WorldToScreen(Vector& in, Vector& out)
{
	const matrix4x4& worldToScreen = g_pEngine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
	out.z = 0;

	if (w > 0.001)
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w;
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f);
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f);
		return true;
	}

	return false;
}

void CalcAngle(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}

void DrawString(int x, int y, int r, int g, int b, int a, const char* pszText, ...)
{
	if (pszText == NULL)
		return;

	//HFont BjordFont = g_pSurface->BjordCreateFont();
	//g_pSurface->SetFontGlyphSet(BjordFont, "Tahoma", 14, 150, 0, 0, FONTFLAG_OUTLINE, NULL, NULL);

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawSetTextFont(5);
	g_pSurface->DrawSetTextColor(r, g, b, a);
	g_pSurface->DrawPrintText(szString, wcslen(szString));
};

void GetTextSizeWH(int& width, int& height, const char* msg)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->GetTextSize(5, wbuf, width, height);
};

void Draw3DBox()
{

}

void DrawPlayerHitboxes(CBaseEntity* player, matrix3x4 matrix, Color c)
{

}

// hooks

bool walls = false;

typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
PaintTraverseFn oPaintTraverse;

void __fastcall PaintTraverse_Hooked(void* _this, void*, unsigned int vpanel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(_this, vpanel, forceRepaint, allowForce);

	static unsigned int OverlayPopupPanel;
	if (OverlayPopupPanel == NULL)
	{
		const char* szName = g_pPanel->GetName(vpanel);
		if (szName[0] == 'O' && szName[7] == 'P')
		{
			OverlayPopupPanel = vpanel;
		}
	}

	if (OverlayPopupPanel != vpanel)
		return;

	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return;

	CBaseEntity* LocalPlayer = (CBaseEntity*)entitylist->GetClientEntity(g_pEngine->GetLocalPlayer());  /// pretty sure I dont need to cast the entity to a
																										/// CBaseEntity becuase it already is a CBaseEntity
	if (!LocalPlayer)
		return;

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		walls = !walls;
		if (walls) Beep(0x367, 200);
		else Beep(0x255, 200);
	}

	if (!walls)
		return;

	for (int i = 0; i < entitylist->GetHighestEntityIndex(); i++)
	{

		CBaseEntity* Entity = (CBaseEntity*)entitylist->GetClientEntity(i);

		if (!Entity || Entity == LocalPlayer)
			continue;

		if (Entity->GetHealth() < 1)
			continue;

		///printf("Model Name: %s", Entity->GetModel()->name);

		player_info_t info;

		if (!g_pEngine->GetPlayerInfo(i, &info))
			continue;

		/*
		mstudiohitboxset* set = Entity->GetHBoxSet();

		mstudiobbox_t* hitbox = set->hitbox(0);

		Vector bbmin = hitbox->bbmin;
		Vector bbmax = hitbox->bbmax;
		*/

		Vector top = Entity->GetOrigin() + Vector(0, 0, Entity->GetCollideable()->OBBMaxs().z);
		Vector bottom = Entity->GetOrigin();

		///
		/// BOUNDING BOX DRAWING
		///

		Vector bottomout, topout, namepos;

		if (!WorldToScreen(bottom, bottomout) || !WorldToScreen(top, topout) || !WorldToScreen(top, namepos))
			continue;

		float height = bottomout.y - topout.y;
		float width = height / 4;

		int iWidth, iHeight;

		GetTextSizeWH(iWidth, iHeight, info.name);

		g_pSurface->DrawSetColor(41, 28, 226, 255);
		g_pSurface->DrawOutlinedRect(bottomout.x - width, topout.y, bottomout.x + width, topout.y + height);

		g_pSurface->DrawSetColor(0, 0, 0, 255);
		g_pSurface->DrawOutlinedRect(bottomout.x - width - 1, topout.y - 1, bottomout.x + width + 1, topout.y + height + 1);
		g_pSurface->DrawOutlinedRect(bottomout.x - width + 1, topout.y + 1, bottomout.x + width - 1, topout.y + height - 1);

		///
		/// HEALTH BAR DRAWING
		///

		int hp = Entity->GetHealth() * height / 100;

		if (hp > height)
			hp = height;

		g_pSurface->DrawSetColor(0, 0, 0, 255);

		g_pSurface->DrawFilledRect(bottomout.x - width - 6, bottomout.y - height - 1, bottomout.x - width - 2, bottomout.y + 1);

		g_pSurface->DrawSetColor((100 - Entity->GetHealth()) * 2.55, Entity->GetHealth() * 2.55, 0, 255);

		g_pSurface->DrawFilledRect(bottomout.x - width - 5, bottomout.y - height, bottomout.x - width - 3, topout.y + hp);
		
		///
		/// NAME DRAWING
		///

		namepos.x -= iWidth / 2; /// positioning
		namepos.y -= iHeight;

		DrawString(namepos.x, namepos.y, 0, 255, 0, 255, info.name);
	};
};

void AntiSpread(CUserCmd* cmd, Vector fakeview)
{
	if (cmd->buttons & IN_ATTACK)
	{
		// ang = angle with predicted spread

		NormalizeAngles(fakeview);
		g_pEngine->SetViewAngles(fakeview);
		///cmd->viewangles = fakeview;
	};
};

Vector fakeview(0, 0, 0);

void FakeMouseSamples(CUserCmd* cmd)
{
	if (fakeview.IsZero())
		g_pEngine->GetViewAngles(fakeview);

	fakeview = fakeview + Vector(cmd->mousedy * 0.022, cmd->mousedx * -0.022, 0);
	NormalizeAngles(fakeview);

	/*
	if (fakeview.x > 89)
		fakeview.x = 89;	///
							/// its more fun without clamping so don't uncomment this
	if(fakeview.x < -89)	///
		fakeview.x = -89;
	*/

	if (cmd->command_number == 0)
		return;

	g_pEngine->SetViewAngles(fakeview);
	AntiSpread(cmd, fakeview);
};


void FixMovement(CUserCmd *cmd, QAngle& va)
{
	float yaw, speed;
	Vector& move = *(Vector*)&cmd->move[0];
	Vector move2 = *(Vector*)&cmd->move;

	speed = move2.Length2D();

	QAngle view = cmd->viewangles;
	NormalizeAngles(view);

	yaw = RAD2DEG(atan2(move.y, move.x));
	yaw = DEG2RAD(view.y - va.y + yaw);

	cmd->move[0] = cos(yaw) * speed;
	cmd->move[1] = sin(yaw) * speed;

	if (view.x < -89.f || view.x > 89.f)
	{
		cmd->move.x = -cmd->move.x;
	}
}

/*
void FixMovement(CUserCmd* cmd, Vector& va) // incorporate the methods used here into the nikyuria movement fix to make it work
{
	//Vector vAngs;
	//g_pEngine->GetViewAngles(vAngs);

	Vector vMove(cmd->move.x, cmd->move.y, cmd->move.z);
	float flSpeed = vMove.Length2D();
	Vector vMove2;
	VectorAngles(vMove, vMove2);

	float flYaw = DEG2RAD(cmd->viewangles.y - va.y + vMove2.y);
	cmd->move.x = cos(flYaw) * flSpeed;
	cmd->move.y = sin(flYaw) * flSpeed;

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f)
		cmd->move.x = -cmd->move.x;
};
*/

bool IsVisible(Vector& vecAbsStart, Vector& vecAbsEnd, CBaseEntity* target)
{
	trace_t tr;
	Ray_t pRay;
	CTraceFilter traceFilter;
	pRay.Init(vecAbsStart, vecAbsEnd);
	traceFilter.pSkip = (CBaseEntity*)entitylist->GetClientEntity(g_pEngine->GetLocalPlayer());

	g_pEngineTrace->TraceRay(pRay, 0x46004003, &traceFilter, &tr);

	if (tr.fraction == 1.0f)
	{
		return true;
	}

	if (tr.m_pEnt && target)
	{
		if (tr.m_pEnt == target)
		{
			return true;
		}
	}

	return false;
};

/*
bool GetHitboxPosition(int HitBox, Vector& Origin, int Index)
{
	if (HitBox < 0 || HitBox >= 20)
		return false;
	matrix3x4 matrix[128];
	CBaseEntity* Entity = entitylist->GetClientEntity(Index);
	if (!Entity)
		return false;
	//if (Entity->IsDormant()) /// dormancy could be the reason studiohdr crashes
	//	return false;
	const model_t* Model = Entity->GetModel();
	if (!Model)
		return false;
	studiohdr* pStudioHdr = Interfaces.ModelInfo->GetStudiomodel(Model);
	if (!pStudioHdr)
		return false;
	if (!Entity->SetupBones((VMatrix*)pmatrix, 128, BONE_USED_BY_HITBOX, 0))
		return false;
	mstudiohitboxset_t *set = pStudioHdr->pHitboxSet(0);
	if (!set)
		return false;
	mstudiobbox_t* pBox = NULL;
	pBox = pStudioHdr->pHitbox(HitBox, NULL);
	Vector min, max;
	QAngle angles;
	MatrixAngles(pmatrix[pBox->bone], angles, Origin);
	VectorTransform(pBox->bbmin, pmatrix[pBox->bone], min);
	VectorTransform(pBox->bbmax, pmatrix[pBox->bone], max);
	Origin = (min + max) * 0.5f;

	return true;
}
*/

int FindHead(CBaseEntity* pPlayer)
{
	studiohdr *hdr = g_pModelInfo->GetStudiomodel(pPlayer->GetModel());
	if (!hdr)
		return -1;
	mstudiobone* bone;
	for (int k = 0; k < hdr->numbones; k++)
	{
		bone = hdr->pBone(k);
		if (bone && (bone->parent != -1))
		{
			char* boneName = bone->pszName();
			std::string name(boneName);
			size_t found = name.find("Head");
			if (found != std::string::npos)
				return k;
		}
	}
	return -1;
};

typedef bool(__fastcall *CreateMoveFn)(void*, int, float, CUserCmd*);
CreateMoveFn oCreateMove;

bool ups = false;
bool cacProtection = true;
int TickDelay = 0;

bool  __fastcall CreateMove_Hooked(void* _this, int edx, float flInputSampleTime, CUserCmd* cmd)
{
	oCreateMove(_this, edx, flInputSampleTime, cmd);

	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return true;

	CBaseEntity* LocalPlayer = (CBaseEntity*)entitylist->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!LocalPlayer)
		return true;

	///
	/// FAKE VIEW
	///

	FakeMouseSamples(cmd);
	Vector oview;
	g_pEngine->GetViewAngles(oview);

	//printf("movement: Vector(%f, %f, %f)\n", cmd->move.x, cmd->move.y, cmd->move.z);

	if (GetAsyncKeyState(VK_F9) & 1)
	{
		ups = !ups;
		if (ups) Beep(0x367, 200);
		else Beep(0x255, 200);
	}

	if (ups)
	{

		///
		/// BHOP
		///

		/*
		if (!cacProtection)
		{
			if (cmd->buttons & IN_JUMP)
			{
				if (!(LocalPlayer->GetFlags() & FL_ONGROUND))
				{
					//cmd->move.y = cmd->mousedx > 0 ? 400 : -400;

					if (!cacProtection)
					{
						cmd->buttons &= ~IN_JUMP;
						return true;
					}
					else
					{
						//printf("%i\n", cmd->tick_count - TickDelay);

						if (cmd->tick_count - TickDelay < 3)
							return true;

						cmd->buttons &= ~IN_JUMP;
					}
				}
				else
				{
					if (cacProtection)
						TickDelay = cmd->tick_count;
				}
			}
		}
		*/

		if (cmd->command_number != 0)
		{

			///
			/// AntiAim
			///

			Vector viewang(0, 0, 0);

			viewang.x = -180.00971595f;

			viewang.y = (cmd->command_number % 50) * 7.2;

			NormalizeAngle(viewang.y);

			g_pEngine->SetViewAngles(viewang);

			///
			/// Aimbot
			///

			for (int i = 0; i < entitylist->GetHighestEntityIndex(); i++)
			{
				if (!g_pInput->IsButtonDown(KEY_F))
					continue;

				//g_pEngine->ClientCmd_Unrestricted("givecurrentammo"); /// doesnt work? :9

				CBaseEntity* Entity = (CBaseEntity*)entitylist->GetClientEntity(i);

				if (!Entity || Entity == LocalPlayer)
					continue;

				player_info_t info;

				if (!g_pEngine->GetPlayerInfo(i, &info))
					continue;

				if (Entity->GetHealth() <= 0/* || Entity->GetTeam() == LocalPlayer->GetTeam()*/)
					continue;

				if (!IsVisible(LocalPlayer->GetEyePosition(), Entity->GetEyePosition(), Entity))
					continue;

				///Vector fov;

				///int scrw, scrh;
				///g_pEngine->GetScreenSize(scrw, scrh);

				///Vector crosshair((float)scrw / 2.f, (float)scrh / 2.f, 0);

				Vector mid = Entity->GetEyePosition();

				mid.z -= Entity->GetCollideable()->OBBMaxs().z / 3;

				///int head = FindHead(Entity);

				///Vector headpos = Entity->GetBonePosition(head);

				///if (!WorldToScreen(mid, fov))
				///	continue;

				///Vector target(fov.x, fov.y, 0);

				///float distance = crosshair.DistTo(target);

				//if (distance > 180)
				//	continue;

				Vector angles;

				CalcAngle(LocalPlayer->GetEyePosition(), mid, angles);

				angles -= LocalPlayer->GetAimPunch() * 2.0f;

				NormalizeAngles(angles);

				g_pEngine->SetViewAngles(angles);
				///cmd->viewangles = angles;
				///cmd->buttons |= IN_ATTACK;
			}
		}
	}

	FixMovement(cmd, oview);

	return false;
};

typedef void(__fastcall *OverrideViewFn)(void*, CViewSetup*);
OverrideViewFn oOverrideView;

void  __fastcall OverrideView_Hooked(void* _this, CViewSetup* view)
{
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return;

	view->angles = fakeview;

	oOverrideView(_this, view);
};

DWORD WINAPI MainThread(LPVOID param)
{
	printf("main thread started\n");

	///////////////////////////// INTERFACES \\\\\\\\\\\\\\\\\\\\\\\\\\\\

	g_pClient = (IBaseClientDLL*)GetInterface("client.dll", "VClient");
	g_pPanel = (IPanel*)GetInterface("vgui2.dll", "VGUI_Panel");
	g_pSurface = (ISurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface");
	g_pEngine = (IVEngineClient*)GetInterface("engine.dll", "VEngineClient");
	entitylist = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList");
	g_pInput = (IInputSystem*)GetInterface("inputsystem.dll", "InputSystemVersion");
	g_pEngineTrace = (IEngineTrace*)GetInterface("engine.dll", "EngineTraceClient");
	g_pModelInfo = (IVModelInfo*)GetInterface("engine.dll", "VModelInfoClient");

	///////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	printf("interfaces declared\n");

	netvars.Init();

	printf("netvars initialized\n");

	GrabOffsets();

	printf("offsets grabbed... Dumping\n");

	netvars.DumpTables();

	//printf("health offset: 0x%x\n", offsets.m_iHealth);

	printf("origin offset: 0x%x\n", offsets.m_vecOrigin);

	CBaseEntity* LocalPlayer = entitylist->GetClientEntity(g_pEngine->GetLocalPlayer());

	printf("CBaseEntity offset: 0x%x\n", LocalPlayer);

	oPaintTraverse = (PaintTraverseFn)vtablehook_hook(g_pPanel, PaintTraverse_Hooked, ID_PAINTTRAVERSE);
	printf("painttraverse hooked\n");

	IClientMode* clientmode = **(IClientMode***)(((DWORD)(*(void***)g_pClient)[10]) + 0x5);

	oCreateMove = (CreateMoveFn)vtablehook_hook(clientmode, CreateMove_Hooked, ID_CREATEMOVE);
	printf("createmove hooked\n");

	oOverrideView = (OverrideViewFn)vtablehook_hook(clientmode, OverrideView_Hooked, ID_OVERRIDEVIEW);
	printf("overrideview hooked\n");

	/// overrideview index 16

	if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
	{
		printf("LocalPlayer Index: %i\n", entitylist->GetClientEntity(g_pEngine->GetLocalPlayer())->index);
		printf("LocalPlayer Flags: %i\n", entitylist->GetClientEntity(g_pEngine->GetLocalPlayer())->GetFlags());
	}

	while (!GetAsyncKeyState(VK_F10))
	{
		if (GetAsyncKeyState(VK_F11) & 1)
		{
			Beep(0x255, 200);
			printf("painttraverse unhooked\n");
			vtablehook_hook(g_pPanel, oPaintTraverse, ID_PAINTTRAVERSE);
			printf("createmove unhooked\n");
			vtablehook_hook(clientmode, oCreateMove, ID_CREATEMOVE);
			printf("overrideview unhooked\n");
			vtablehook_hook(clientmode, oOverrideView, ID_OVERRIDEVIEW);
			printf("console free'd\n");
			FreeConsole();
		}
	}
	FreeLibraryAndExitThread((HMODULE)param, NULL);
	return FALSE;
};

DWORD WINAPI DllMain(HMODULE hDll, DWORD fdwReason, LPVOID lpReserved)
{
	static HANDLE hThread = INVALID_HANDLE_VALUE;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		hThread = CreateThread(NULL, NULL, MainThread, NULL, NULL, NULL);

		AllocConsole();

		SetConsoleTitle("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		printf("dll attached\n");

		return (hThread != INVALID_HANDLE_VALUE);
	case DLL_PROCESS_DETACH:
		return TRUE;
	default:
		break;
	};
	return FALSE;
};

