#include "BJDrawing.h"
#include "Vector.h"

void BJDrawing::HitboxESP(CBaseEntity * pPlayer, int r, int g, int b, int a)
{
	/*
	matrix3x4 matrix[128];
	if (!pPlayer->SetupBones(matrix, 128, 0x00000100, g_pEngine->GetLastTimeStamp()))
		return;

	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(pPlayer->GetModel());
	mstudiohitboxset* set = hdr->pHitboxSet(0);

	for (int i = 0; i < set->numhitboxes; i++)
	{
		mstudiobbox_t* hitbox = set->pHitbox(i);
		if (!hitbox)
			continue;
		int bone = hitbox->bone;
		Vector vMaxUntransformed = hitbox->bbmax;
		Vector vMinUntransformed = hitbox->bbmin;

		DrawHitboxes(pPlayer, matrix, bone, vMaxUntransformed, vMinUntransformed, r, g, b, a);
	}
	*/
}
