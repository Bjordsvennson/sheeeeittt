//#include "sdk.h"
#include "IBaseClientDLL.h"
#include "VTable.h"

ClientClass* IBaseClientDLL::GetAllClasses()
{
	typedef ClientClass*(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 8))(this);
}

IBaseClientDLL* g_pClient;