#pragma once

#include "Common.h"

// interface grabber thanks to biggie

static const char* fmts(const char* fmt, ...)
{
	char buffer[256];
	va_list list;
	va_start(list, fmt);
	vsnprintf(buffer, 256, fmt, list);
	va_end(list);

	return buffer;
};

#define Format fmts

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
static void* GetInterface(const char* pModule, const char* pName)
{
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(pModule), "CreateInterface");
	void* Interface = CreateInterface(pName, 0);
	for (int i = 0; i < 100; i++)
	{
		if (Interface)
		{
			printf("version: %s\n", pName);
			break;
		}

		const char* Possible0 = Format("%s0%i", pName, i);
		Interface = CreateInterface(Possible0, 0);
		if (Interface)
		{
			printf("version: %s\n", Possible0);
			break;
		}

		const char* Possible1 = Format("%s00%i", pName, i);
		Interface = CreateInterface(Possible1, 0);
		if (Interface)
		{
			printf("version: %s\n", Possible1);
			break;
		}
	};

	return Interface;
};

// hooking method from github

static int vtablehook_unprotect(void* region)
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)region, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
	return mbi.Protect;
};

static void vtablehook_protect(void* region, int protection)
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)region, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, protection, &mbi.Protect);
};

static void* vtablehook_hook(void* instance, void* hook, int offset)
{
	intptr_t vtable = *((intptr_t*)instance);
	intptr_t entry = vtable + sizeof(intptr_t) * offset;
	intptr_t original = *((intptr_t*)entry);

	int original_protection = vtablehook_unprotect((void*)entry);
	*((intptr_t*)entry) = (intptr_t)hook;
	vtablehook_protect((void*)entry, original_protection);

	return (void*)original;
};

static void* vtablehook_getfunction(void* instance, int offset)
{
	intptr_t VirtualFunction = (*(intptr_t*)instance) + sizeof(intptr_t) * offset;
	return (void*)*((intptr_t*)VirtualFunction);
};