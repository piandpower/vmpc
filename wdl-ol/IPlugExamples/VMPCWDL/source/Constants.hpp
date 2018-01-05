#pragma once
#include <IControl.h>

class Constants {

public:
	static const float TFACTOR;
	static const int KBLABEL_FONT_SIZE = 20;
	static const int KBLABEL_OUTLINE_SIZE = 4;

	static IColor* LCD_ON();
	static IColor* LCD_HALF_ON();
	static IColor* LCD_OFF();
	static IColor* TRANS_BLACK();

	static IRECT* LCD_RECT();
	static IRECT* PLUG_RECT();
	static IRECT* DATAWHEEL_RECT();
	static IRECT* RECKNOB_RECT();
	static IRECT* VOLKNOB_RECT();
	static IRECT* SLIDER_RECT();


private:
	static IColor LCD_ON_;
	static IColor LCD_HALF_ON_;
	static IColor LCD_OFF_;
	static IColor TRANS_BLACK_;


	static IRECT LCD_RECT_;
	static IRECT PLUG_RECT_;
	static IRECT DATAWHEEL_RECT_;
	static IRECT RECKNOB_RECT_;
	static IRECT VOLKNOB_RECT_;
	static IRECT SLIDER_RECT_;
};
