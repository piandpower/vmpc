#pragma once
#include <IControl.h>

#ifdef __APPLE__
#define FONTSIZE 17
#else
#define FONTSIZE 16
#endif

class Constants {

public:
	static const float TFACTOR;
	static const int TEXT_HEIGHT = 7 * 2;
	static const int TEXT_WIDTH = 6 * 2;

	static IColor* LCD_ON();
	static IColor* LCD_OFF();
	static IColor* TRANS_BLACK();

	static IText* FONT_ON();
	static IText* FONT_OFF();

	static IRECT* LCD_RECT();
	static IRECT* PLUG_RECT();
	static IRECT* FB_RECT();
	static IRECT* DATAWHEEL_RECT();
	static IRECT* RECKNOB_RECT();
	static IRECT* VOLKNOB_RECT();
	static IRECT* SLIDER_RECT();
	static IRECT* POPUP_RECT();
	static IRECT* UNDERLINE_RECT();

private:
	static IColor LCD_ON_;
	static IColor LCD_OFF_;
	static IColor TRANS_BLACK_;

	static IText FONT_ON_;
	static IText FONT_OFF_;

	static IRECT LCD_RECT_;
	static IRECT POPUP_RECT_;
	static IRECT PLUG_RECT_;
	static IRECT FB_RECT_;
	static IRECT DATAWHEEL_RECT_;
	static IRECT RECKNOB_RECT_;
	static IRECT VOLKNOB_RECT_;
	static IRECT SLIDER_RECT_;
	static IRECT UNDERLINE_RECT_;
};
