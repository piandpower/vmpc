#include "FTControl.hpp"
#include "Constants.hpp"

#include <Logger.hpp>

struct Vec2
{
	Vec2() { }
	Vec2(float a, float b)
		: x(a), y(b) { }

	float x, y;
};


struct Rect
{
	Rect() { }
	Rect(float left, float top, float right, float bottom)
		: xmin(left), xmax(right), ymin(top), ymax(bottom) { }

	void Include(const Vec2 &r)
	{
		xmin = MIN(xmin, r.x);
		ymin = MIN(ymin, r.y);
		xmax = MAX(xmax, r.x);
		ymax = MAX(ymax, r.y);
	}

	float Width() const { return xmax - xmin + 1; }
	float Height() const { return ymax - ymin + 1; }

	float xmin, xmax, ymin, ymax;
};

void RasterCallback(const int y,
	const int count,
	const FT_Span * const spans,
	void * const user)
{
	Spans *sptr = (Spans *)user;
	for (int i = 0; i < count; ++i)
		sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
}

void FTControl::RenderSpans(FT_Outline * const outline, Spans *spans)
{
	FT_Raster_Params params;
	memset(&params, 0, sizeof(params));
	params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans = RasterCallback;
	params.user = spans;

	FT_Outline_Render(library, outline, &params);
}

void add_pixels(Pixel32 src, Pixel32& dst)
{
	dst.r = (int)(dst.r + ((src.r - dst.r) * src.a) / 255.0f);
	dst.g = (int)(dst.g + ((src.g - dst.g) * src.a) / 255.0f);
	dst.b = (int)(dst.b + ((src.b - dst.b) * src.a) / 255.0f);
	dst.a = MIN(255, dst.a + src.a);
}

void FTControl::WriteGlyph(const Pixel32 &fontCol,
	const Pixel32 outlineCol)
{
	FT_Vector pen;
	pen.x = 0 * 64;
	pen.y = (mRECT.H() - 0) * 64;

	for (int n = 0; n < text.length(); n++) {
		FT_UInt gindex = FT_Get_Char_Index(face, text[n]);
		if (FT_Load_Glyph(face, gindex, FT_LOAD_DEFAULT) == 0)
		{
			if (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
			{
				Spans spans;
				RenderSpans(&face->glyph->outline, &spans);

				Spans outlineSpans;

				FT_Stroker stroker;
				FT_Stroker_New(library, &stroker);
				FT_Stroker_Set(stroker,
					(int)(outlineSize * 64),
					FT_STROKER_LINECAP_ROUND,
					FT_STROKER_LINEJOIN_ROUND,
					0);
				FT_Glyph glyph;
				if (FT_Get_Glyph(face->glyph, &glyph) == 0)
				{
					FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
					if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
					{
						FT_Outline *o =
							&reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
						RenderSpans(o, &outlineSpans);
					}

					FT_Stroker_Done(stroker);
					FT_Done_Glyph(glyph);

					if (!spans.empty())
					{
						//MLOG("\nchar    : " + text.substr(n, 1));
						auto bearingX = face->glyph->metrics.horiBearingX >> 6;
						auto bearingY = face->glyph->metrics.horiBearingY >> 6;
						auto height = face->glyph->metrics.height >> 6;
						//MLOG("bearingX: " + std::to_string(bearingX));
						//MLOG("width   : " + std::to_string(width));
						int xdist = bearingX;
						int ydist = height - bearingY;
						if (n == 0) xdist = 0;
						Rect rect(spans.front().x,
							(spans.front().y) - (fontSize *0.5) + ydist,
							spans.front().x,
							(spans.front().y));

						for (Spans::iterator s = spans.begin();
							s != spans.end(); ++s)
						{
							rect.Include(Vec2(s->x, s->y));
							rect.Include(Vec2(s->x + s->width - 1, s->y));
						}
						for (Spans::iterator s = outlineSpans.begin();
							s != outlineSpans.end(); ++s)
						{
							rect.Include(Vec2(s->x, s->y));
							rect.Include(Vec2(s->x + s->width - 1, s->y));
						}
						
						for (Spans::iterator s = outlineSpans.begin();
							s != outlineSpans.end(); ++s)
							for (int w = 0; w < s->width; ++w) {
								int x = (s->x + pen.x + xdist) - rect.xmin + w;
								int y = (s->y - rect.ymin);
								if (x >= mRECT.W()) continue;
								if (y >= mRECT.H()) continue;
								int pixindex = (int)((mRECT.H() - 1 - y) * mRECT.W() + x);
								if (pixindex > pixels.size()) continue;
								auto bgpixel = pixels[pixindex];
								auto textpixel = Pixel32(outlineCol.r, outlineCol.g, outlineCol.b, s->coverage);
								add_pixels(bgpixel, textpixel);
								pixels[pixindex] = textpixel;
							}

						for (Spans::iterator s = spans.begin();
							s != spans.end(); ++s) {
							for (int w = 0; w < s->width; ++w)
							{
								int x = (s->x + pen.x + xdist) - rect.xmin + w;
								int y = (s->y - rect.ymin);
								if (x >= mRECT.W()) continue;
								if (y >= mRECT.H()) continue;
								int pixindex = (int)((mRECT.H() - 1 - y) * mRECT.W() + x);
								if (pixindex > pixels.size()) continue;
								Pixel32 &dst =
									pixels[pixindex];
								Pixel32 src = Pixel32(fontCol.r, fontCol.g, fontCol.b,
									s->coverage);
								dst.r = (int)(dst.r + ((src.r - dst.r) * src.a) / 255.0f);
								dst.g = (int)(dst.g + ((src.g - dst.g) * src.a) / 255.0f);
								dst.b = (int)(dst.b + ((src.b - dst.b) * src.a) / 255.0f);
								dst.a = MIN(255, dst.a + src.a);
							}
						}
					}
					pen.x += face->glyph->advance.x >> 6;
				}
			}
		}
	}
}

FTControl::FTControl(IPlugBase* pPlug, int x, int y, std::string text, int fontSize, int outlineSize)
	: IPanelControl(pPlug, IRECT(0,0,0,0), Constants::LCD_OFF()) {
	this->x = x;
	this->y = y;
	this->fontSize = fontSize;
	this->outlineSize = outlineSize;
	this->text = text;
	FT_Init_FreeType(&library);

	std::ifstream fontFile("c:/temp/Vera.ttf", std::ios::binary);
	fontFile.seekg(0, std::ios::end);
	fontFileSize = fontFile.tellg();
	fontFile.seekg(0);
	fontBuffer = new unsigned char[fontFileSize];
	fontFile.read((char *)fontBuffer, fontFileSize);
	FT_New_Memory_Face(library, fontBuffer, fontFileSize, 0, &face);
	FT_Set_Char_Size(face, fontSize << 6, fontSize << 6, 90, 90);

	int width = getStringWidth() + (2 * outlineSize);
	int height = (fontSize*1.25) + (2 * outlineSize);
	mRECT = IRECT(x, y, x + width, y + height);
	pixels = std::vector<Pixel32>(mRECT.W() * mRECT.H());
}

int FTControl::getStringWidth() {
	int res = 0;

	for (int n = 0; n < text.length(); n++)
	{
		//MLOG("calculating advance value for char " + text.substr(n, 1));
		FT_UInt gindex = FT_Get_Char_Index(face, text[n]);
		if (FT_Load_Glyph(face, gindex, FT_LOAD_DEFAULT) == 0) {
			int advance = face->glyph->advance.x >> 6;
			//MLOG("no error, advance.x == " + std::to_string(advance));
			res += advance;
		}
	}
	return res;
}

void blend_alpha(unsigned char result[4], unsigned char fg[4], unsigned char bg[4])
{
	unsigned int alpha = fg[3] + 1;
	unsigned int inv_alpha = 256 - fg[3];
	result[0] = (unsigned char)((alpha * fg[0] + inv_alpha * bg[0]) >> 8);
	result[1] = (unsigned char)((alpha * fg[1] + inv_alpha * bg[1]) >> 8);
	result[2] = (unsigned char)((alpha * fg[2] + inv_alpha * bg[2]) >> 8);
	result[3] = 0xff;
}

bool FTControl::Draw(IGraphics* g) {
	Pixel32 oc(0, 0, 0);
	Pixel32 tc(255, 255, 255);
	WriteGlyph(tc, oc);

	const IChannelBlend blend = IChannelBlend::kBlendNone;
	int pixelCounter = 0;
	for (int y = mRECT.T; y < mRECT.B; y++) {
		for (int x = mRECT.L; x < mRECT.R; x++) {
			Pixel32 pixel = pixels[pixelCounter++];
			IColor bgc = g->GetPoint(x, y);
			unsigned char res[4] = { 0,0,0,0 };
			unsigned char fg[4] = { pixel.r, pixel.g, pixel.b, pixel.a };
			unsigned char bg[4] = { bgc.R, bgc.G, bgc.B, bgc.A };
			blend_alpha(res, fg, bg);
			IColor resc(res[3], res[0], res[1], res[2]);
			g->DrawPoint(&resc, x, y, &blend);
		}
	}

	//g->DrawRect(Constants::LCD_ON(), &mRECT);
	return true;
}

FTControl::~FTControl() {
	if (fontBuffer) delete[] fontBuffer;
	FT_Done_FreeType(library);
}
