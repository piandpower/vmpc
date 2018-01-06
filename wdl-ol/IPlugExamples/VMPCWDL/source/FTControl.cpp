#include "FTControl.hpp"
#include "Constants.hpp"

#include "IPlugGUIResize.h"

#include "../resource.h"

std::string FTControl::fontPath = "c:/temp/arial.ttf";
int FTControl::fontSize = Constants::KBLABEL_FONT_SIZE;
int FTControl::outlineSize = Constants::KBLABEL_OUTLINE_SIZE;

void FTControl::Hide(bool b) {
	if (b) {
		unhideDesired = false;
		IPanelControl::Hide(true);
	}
	else {
		unhideDesired = true;
		if (unhideThread.joinable()) unhideThread.join();
		unhideThread = std::thread(&FTControl::static_unhide, this);
	}
}

void FTControl::static_unhide(void * args)
{
	static_cast<FTControl*>(args)->unhide();
}

void FTControl::unhide() {
	int timeWaited = 0;
	while (unhideDesired && timeWaited < 800) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		timeWaited += 5;
	}
	if (unhideDesired) {
		IPanelControl::Hide(false);
	}
}

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
	pen.y = (GetDrawRECT()->H() - 0) * 64;

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
						auto bearingX = face->glyph->metrics.horiBearingX >> 6;
						auto bearingY = face->glyph->metrics.horiBearingY >> 6;
						auto height = face->glyph->metrics.height >> 6;
						int xdist = bearingX;
						int ydist = height - bearingY;
						if (n == 0) xdist = 0;
						Rect rect(spans.front().x,
							(spans.front().y) - (fontSize *0.6) + ydist,
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
								if (x >= GetDrawRECT()->W()) continue;
								if (y >= GetDrawRECT()->H()) continue;
								int pixindex = (int)((GetDrawRECT()->H() - 1 - y) * GetDrawRECT()->W() + x);
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
								if (x >= GetDrawRECT()->W()) continue;
								if (y >= GetDrawRECT()->H()) continue;
								int pixindex = (int)((GetDrawRECT()->H() - 1 - y) * GetDrawRECT()->W() + x);
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

FTControl::FTControl(IPlugBase* pPlug, int x, int y, std::string text)
	: IPanelControl(pPlug, IRECT(0,0,0,0), Constants::LCD_OFF()) {
	this->x = x;
	this->y = y;
	this->text = text;
	FT_Init_FreeType(&library);

	std::ifstream fontFile(fontPath, std::ios::binary);
	fontFile.seekg(0, std::ios::end);
	std::fstream::pos_type fontFileSize = fontFile.tellg();
	fontFile.seekg(0);
	fontBuffer = new unsigned char[fontFileSize];
	fontFile.read((char *)fontBuffer, fontFileSize);
	FT_New_Memory_Face(library, fontBuffer, fontFileSize, 0, &face);
	FT_Set_Char_Size(face, fontSize << 6, fontSize << 6, 90, 90);

	int width = getStringWidth(text) + (2 * outlineSize);
	int height = (fontSize*1.5) + (2 * outlineSize);
	SetDrawRECT(DRECT(x, y, x + width, y + height));
}

int FTControl::getStringWidth(std::string text) {
	int res = 0;
	FT_Library ftlib;
	FT_Init_FreeType(&ftlib);
	std::ifstream fontFile(fontPath, std::ios::binary);
	fontFile.seekg(0, std::ios::end);
	std::fstream::pos_type fontFileSize = fontFile.tellg();
	fontFile.seekg(0);
	auto fb = new unsigned char[fontFileSize];
	FT_Face fc;
	fontFile.read((char *)fb, fontFileSize);
	FT_New_Memory_Face(ftlib, fb, fontFileSize, 0, &fc);
	FT_Set_Char_Size(fc, fontSize << 6, fontSize << 6, 90, 90);

	for (int n = 0; n < text.length(); n++)
	{
		//MLOG("calculating advance value for char " + text.substr(n, 1));
		FT_UInt gindex = FT_Get_Char_Index(fc, text[n]);
		if (FT_Load_Glyph(fc, gindex, FT_LOAD_DEFAULT) == 0) {
			int advance = fc->glyph->advance.x >> 6;
			//MLOG("no error, advance.x == " + std::to_string(advance));
			res += advance;
		}
	}
	if (fb) delete[] fb;
	FT_Done_FreeType(ftlib);
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
	auto ratio = GetGUIResize()->GetGUIScaleRatio();
	fontSize = Constants::KBLABEL_FONT_SIZE * ratio;
	outlineSize = Constants::KBLABEL_OUTLINE_SIZE * ratio;
	FT_Set_Char_Size(face, fontSize << 6, fontSize << 6, 90, 90);
	pixels = std::vector<Pixel32>(GetDrawRECT()->W() * GetDrawRECT()->H());
	Pixel32 oc(0, 0, 0);
	Pixel32 tc(255, 255, 255);
	WriteGlyph(tc, oc);

	const IChannelBlend blend = IChannelBlend::kBlendNone;
	int pixelCounter = 0;
	for (int y = GetDrawRECT()->T; y < GetDrawRECT()->B; y++) {
		for (int x = GetDrawRECT()->L; x < GetDrawRECT()->R; x++) {
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
	//g->DrawRect(Constants::LCD_ON(), &mDrawRECT);
	return false;
}

FTControl::~FTControl() {
	if (fontBuffer) delete[] fontBuffer;
	FT_Done_FreeType(library);
	if (unhideThread.joinable()) unhideThread.join();
}
