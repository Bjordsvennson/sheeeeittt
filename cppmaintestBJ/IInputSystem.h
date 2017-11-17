#pragma once

enum ButtonCode_t;

class IInputSystem
{
public:
	void EnableInput(bool bEnable);
	bool IsButtonDown(ButtonCode_t code);
	void ResetInputState();
	ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey);
	int ButtonCodeToVirtualKey(ButtonCode_t code);
	void SetCursorPosition(int* m_pX, int* m_pY);
};

extern IInputSystem* g_pInput;