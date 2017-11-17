#pragma once

class ClientClass;

class IBaseClientDLL
{
public:
	ClientClass* GetAllClasses();
};

extern IBaseClientDLL* g_pClient;