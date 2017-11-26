#pragma once

#include "Vector.h"
#include "Common.h"

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

struct mstudiohitboxset_t
{
	int	sznameindex;
	int	numhitboxes;
	int	hitboxindex;

	mstudiobbox_t* hitbox(int i);
};

struct mstudiobbox
{
	int		bone;
	int		group;
	Vector	bbmin;
	Vector	bbmax;
	int		szhitboxnameindex;
	int		unused[8];
};

struct mstudiobone
{
	int					sznameindex;
	inline char* const pszName(void) const { return ((char *)this) + sznameindex; }
	int		 			parent;
};

struct studiohdr
{
	int					id;
	int					version;
	long				checksum;		// this has to be the same in the phy and vtx files to load!
	inline const char *	pszName(void) const { return name; }
	char				name[64];
	int					length;
	Vector				eyeposition;	// ideal eye position
	Vector				illumposition;	// illumination center
	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;
	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;
	int					flags;
	int					numbones;			// bones
	int					boneindex;
	inline mstudiobone* pBone(int i) const { return (mstudiobone*)(((byte*)this) + boneindex) + i; };
};

class CBaseEntity
{
public:
	char _0x0000[0x50];
	int index;
	//player_info_t GetPlayerInfo();
	int GetHealth();
	Vector GetOrigin();
	int GetFlags();
	int GetTeam();
	Vector GetEyePosition();
	ICollideable* GetCollideable();
	model_t* GetModel();
	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	Vector GetBonePosition(int iBone);
	mstudiohitboxset_t* GetHBoxSet();
	Vector GetAimPunch();
};