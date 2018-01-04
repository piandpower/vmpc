#pragma once
#include <IControl.h>

#include <vector>
#include <fstream>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#ifdef _MSC_VER
#define MIN __min
#define MAX __max
#else
#define MIN std::min
#define MAX std::max
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;

#if (('1234' >> 24) == '1')
#elif (('4321' >> 24) == '1')
#define BIG_ENDIAN
#else
#error "Couldn't determine the endianness!"
#endif

union Pixel32
{
	Pixel32()
		: integer(0) { }
	Pixel32(uint8 bi, uint8 gi, uint8 ri, uint8 ai = 255)
	{
		b = bi;
		g = gi;
		r = ri;
		a = ai;
	}

	uint32_t integer;

	struct
	{
#ifdef BIG_ENDIAN
		uint8 a, r, g, b;
#else // BIG_ENDIAN
		uint8 b, g, r, a;
#endif // BIG_ENDIAN
	};
};

struct Span
{
	Span() { }
	Span(int _x, int _y, int _width, int _coverage)
		: x(_x), y(_y), width(_width), coverage(_coverage) { }

	int x, y, width, coverage;
};

typedef std::vector<Span> Spans;

class FTControl
	: public IPanelControl	
	{
	private:
		int fontSize = 0;
		int outlineSize = 0;
		std::string text;
		std::vector<Pixel32> pixels;
		unsigned char *fontBuffer;
		FT_Library library;
		std::fstream::pos_type fontFileSize;
		FT_Face face;
		int x = 0;
		int y = 0;

	private:
		void WriteGlyph(const Pixel32 &fontCol, const Pixel32 outlineCol);
		int getStringWidth();

		void RenderSpans(FT_Outline * const outline, Spans *spans);

public:
	bool Draw(IGraphics* pGraphics) override;

public:
	FTControl(IPlugBase* pPlug, int x, int y, std::string text, int fontSize, int outlineSize);
	~FTControl();

};
