#pragma once

#include "Vector.h"

typedef unsigned int uint32;
typedef unsigned long CRC32_t;
struct model_t;
//class Vector;

struct player_info_t
{
	char			name[32];
	int				userID;
	char			guid[33];
	uint32			friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	CRC32_t			customFiles[4];
	unsigned char	filesDownloaded;
};

class ICollideable
{
public:
	virtual void pad0();
	virtual const Vector& OBBMins() const;
	virtual const Vector& OBBMaxs() const;
};

struct	mstudiobbox_t
{
	int		bone;
	int		group;
	Vector	bbmin;
	Vector	bbmax;
	int		szhitboxnameindex;
	int		pad[8];
};

struct mstudiohitboxset
{
	int	sznameindex;
	int	numhitboxes;
	int	hitboxindex;

	mstudiobbox_t* hitbox(int i);
};

extern mstudiohitboxset* hboxsets[0xFFFFFFF];

class CBaseEntity
{
public:
	char _0x0000[0x50];
	int index;
	model_t* GetModel();
	//player_info_t GetPlayerInfo();
	int GetHealth();
	Vector GetOrigin();
	int GetFlags();
	int GetTeam();
	Vector GetEyePosition();
	ICollideable* GetCollideable();
	mstudiohitboxset* GetHBoxSet();
	Vector GetAimPunch();
};