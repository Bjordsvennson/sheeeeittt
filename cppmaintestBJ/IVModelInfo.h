#pragma once

struct studiohdr;

struct model_t
{
	char name[255];
};

class IVModelInfo
{
public:
	const char* GetModelName(const model_t* model);
	studiohdr* GetStudiomodel(const model_t *mod);
};

extern IVModelInfo* g_pModelInfo;
