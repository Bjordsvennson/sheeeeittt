#pragma once

typedef unsigned int HFont;

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a);
	void DrawFilledRect(int x0, int y0, int x1, int y1);
	void DrawOutlinedRect(int x0, int y0, int x1, int y1);
	void DrawLine(int x0, int y0, int x1, int y1);
	void DrawSetTextFont(HFont font);
	void DrawSetTextColor(int r, int g, int b, int a);
	void DrawSetTextPos(int x, int y);
	void DrawPrintText(const wchar_t* text, int len, FontDrawType_t type = FONT_DRAW_DEFAULT);
	HFont BjordCreateFont();
	bool SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax);
	void GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall);
};

extern ISurface* g_pSurface;