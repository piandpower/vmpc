#include "Constants.hpp"

#include "../resource.h"


//const float Constants::TFACTOR = .25f;
const float Constants::TFACTOR = .05f;

IRECT* Constants::UNDERLINE_RECT()
{
	return &UNDERLINE_RECT_;
}
IRECT Constants::UNDERLINE_RECT_ = IRECT(212, 54, 212 + 194, 54 + 2);

IRECT* Constants::LCD_RECT()
{
    return &LCD_RECT_;
}
IRECT Constants::LCD_RECT_ = IRECT(116, 102, 116 + 496, 102 + 120);

IRECT* Constants::POPUP_RECT()
{
    return &POPUP_RECT_;
}
IRECT Constants::POPUP_RECT_ = IRECT(182, 136, 182 + (184*2), 136 + 36);

IRECT* Constants::PLUG_RECT()
{
    return &PLUG_RECT_;
}
IRECT Constants::PLUG_RECT_ = IRECT(0, 0, GUI_WIDTH, GUI_HEIGHT);

IRECT* Constants::FB_RECT()
{
    return &FB_RECT_;
}
IRECT Constants::FB_RECT_ = IRECT(120, 204, 608, 222);

IRECT* Constants::DATAWHEEL_RECT()
{
    return &DATAWHEEL_RECT_;
}
IRECT Constants::DATAWHEEL_RECT_ = IRECT(378, 415, 378 + 171, 415 + 171);

IRECT* Constants::RECKNOB_RECT()
{
    return &RECKNOB_RECT_;
}
IRECT Constants::RECKNOB_RECT_ = IRECT(1014, 183, 1014 + 72, 183 + 73);

IRECT* Constants::VOLKNOB_RECT()
{
    return &VOLKNOB_RECT_;
}
IRECT Constants::VOLKNOB_RECT_ = IRECT(1137, 181, 1137 + 74, 181 + 75);

IRECT* Constants::SLIDER_RECT()
{
    return &SLIDER_RECT_;

}
IRECT Constants::SLIDER_RECT_ = IRECT(33, 668, 33 + 128, 668 + 247);

IColor* Constants::TRANS_BLACK() {
    return &TRANS_BLACK_;
}
IColor Constants::TRANS_BLACK_ = IColor(0, 0, 0, 0);

IColor* Constants::LCD_ON()
{
	return &LCD_ON_;
}
IColor Constants::LCD_ON_ = IColor(255, 86, 61, 145);

IColor* Constants::LCD_HALF_ON()
{
	return &LCD_HALF_ON_;
}
IColor Constants::LCD_HALF_ON_ = IColor(255, 128, 154, 181);

IColor* Constants::LCD_OFF()
{
	return &LCD_OFF_;
}
IColor Constants::LCD_OFF_ = IColor(255, 170, 248, 218);

IText* Constants::FONT_ON()
{
	return &FONT_ON_;
}
IText Constants::FONT_ON_ = IText(FONTSIZE, LCD_ON()
                                  , (char*)("mpc2000xl")
                                  , IText::kStyleNormal
                                  , IText::kAlignNear, 0
                                  , IText::kQualityNonAntiAliased
                                  , TRANS_BLACK(), LCD_ON());

IText* Constants::FONT_OFF()
{
	return &FONT_OFF_;
}
IText Constants::FONT_OFF_ = IText(FONTSIZE, LCD_OFF()
                                   , (char*)("mpc2000xl")
                                   , IText::kStyleNormal
                                   , IText::kAlignNear, 0
                                   , IText::kQualityNonAntiAliased
                                   , TRANS_BLACK(), LCD_OFF());
