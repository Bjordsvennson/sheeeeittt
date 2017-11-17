#include "IPanel.h"
#include "VTable.h"

const char* IPanel::GetName(unsigned int vpanel)
{
	typedef const char*(__thiscall* Fn)(void*, int);
	return ((Fn)vtablehook_getfunction(this, 36))(this, vpanel);
}

IPanel* g_pPanel;