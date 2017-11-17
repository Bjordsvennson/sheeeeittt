#include "IInputSystem.h"
#include "VTable.h"

enum ButtonCode_t;

void IInputSystem::EnableInput(bool bEnable)
{
	typedef void(__thiscall* Fn)(void*, bool);
	return ((Fn)vtablehook_getfunction(this, 7))(this, bEnable);
}

bool IInputSystem::IsButtonDown(ButtonCode_t code)
{
	typedef bool(__thiscall* Fn)(void*, ButtonCode_t);
	return ((Fn)vtablehook_getfunction(this, 11))(this, code);
}

void IInputSystem::ResetInputState()
{
	typedef void(__thiscall* Fn)(void*);
	return ((Fn)vtablehook_getfunction(this, 25))(this);
}

ButtonCode_t IInputSystem::VirtualKeyToButtonCode(int nVirtualKey)
{
	typedef ButtonCode_t(__thiscall* Fn)(void*, int);
	return ((Fn)vtablehook_getfunction(this, 32))(this, nVirtualKey);
}

int IInputSystem::ButtonCodeToVirtualKey(ButtonCode_t code)
{
	typedef int(__thiscall* Fn)(void*, ButtonCode_t);
	return ((Fn)vtablehook_getfunction(this, 33))(this, code);
}

void IInputSystem::SetCursorPosition(int * m_pX, int * m_pY)
{
	typedef void(__thiscall* Fn)(void*, int*, int*);
	return ((Fn)vtablehook_getfunction(this, 36))(this, m_pX, m_pY);
}

IInputSystem* g_pInput;