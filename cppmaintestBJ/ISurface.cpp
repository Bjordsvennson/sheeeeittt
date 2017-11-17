#include "ISurface.h"
#include "VTable.h"

void ISurface::DrawSetColor(int r, int g, int b, int a)
{
	typedef void(__thiscall* Fn)(void*, int, int, int, int);
	return ((Fn)(vtablehook_getfunction(this, 11)))(this, r, g, b, a);
}

void ISurface::DrawFilledRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall* Fn)(void*, int, int, int, int);
	return ((Fn)(vtablehook_getfunction(this, 12)))(this, x0, y0, x1, y1);
}

void ISurface::DrawOutlinedRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall* Fn)(void*, int, int, int, int);
	return ((Fn)(vtablehook_getfunction(this, 14)))(this, x0, y0, x1, y1);
}

void ISurface::DrawLine(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall* Fn)(void*, int, int, int, int);
	return ((Fn)(vtablehook_getfunction(this, 15)))(this, x0, y0, x1, y1);
}

void ISurface::DrawSetTextFont(HFont font)
{
	typedef void(__thiscall* Fn)(void*, HFont);
	return ((Fn)(vtablehook_getfunction(this, 17)))(this, font);
}

void ISurface::DrawSetTextColor(int r, int g, int b, int a)
{
	typedef void(__thiscall* Fn)(void*, int, int, int, int);
	return ((Fn)(vtablehook_getfunction(this, 19)))(this, r, g, b, a);
}

void ISurface::DrawSetTextPos(int x, int y)
{
	typedef void(__thiscall* Fn)(void*, int, int);
	return ((Fn)(vtablehook_getfunction(this, 20)))(this, x, y);
}

void ISurface::DrawPrintText(const wchar_t * text, int len, FontDrawType_t type)
{
	typedef void(__thiscall* Fn)(void*, const wchar_t*, int, FontDrawType_t);
	return ((Fn)(vtablehook_getfunction(this, 22)))(this, text, len, type);
}

HFont ISurface::BjordCreateFont()
{
	typedef HFont(__thiscall* Fn)(void*);
	return ((Fn)(vtablehook_getfunction(this, 66)))(this);
}

bool ISurface::SetFontGlyphSet(HFont font, const char * windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax)
{
	typedef bool(__thiscall* Fn)(void*, HFont, const char*, int, int, int, int, int, int, int);
	return ((Fn)(vtablehook_getfunction(this, 67)))(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
}

void ISurface::GetTextSize(HFont font, const wchar_t * text, int & wide, int & tall)
{
	typedef void(__thiscall* Fn)(void*, HFont, const wchar_t*, int&, int&);
	return ((Fn)(vtablehook_getfunction(this, 76)))(this, font, text, wide, tall);
}

ISurface* g_pSurface;
