#include "Constants.hpp"

#include "../resource.h"


//const float Constants::TFACTOR = .25f;
const float Constants::TFACTOR = .05f;

IRECT* Constants::LCD_RECT()
{
    return &LCD_RECT_;
}
IRECT Constants::LCD_RECT_ = IRECT(116 * gui_scale, 102 * gui_scale, (116 + 496) * gui_scale, (102 + 120) * gui_scale);

IRECT* Constants::PLUG_RECT()
{
    return &PLUG_RECT_;
}
IRECT Constants::PLUG_RECT_ = IRECT(0, 0, GUI_WIDTH * gui_scale, GUI_HEIGHT * gui_scale);

IRECT* Constants::DATAWHEEL_RECT()
{
    return &DATAWHEEL_RECT_;
}
IRECT Constants::DATAWHEEL_RECT_ = IRECT(378 * gui_scale, 415 * gui_scale, (378 + 171) * gui_scale, (415 + 171) * gui_scale);

IRECT* Constants::RECKNOB_RECT()
{
    return &RECKNOB_RECT_;
}
IRECT Constants::RECKNOB_RECT_ = IRECT(1014 * gui_scale, 183 * gui_scale, (1014 + 72) * gui_scale, (183 + 73) * gui_scale);

IRECT* Constants::VOLKNOB_RECT()
{
    return &VOLKNOB_RECT_;
}
IRECT Constants::VOLKNOB_RECT_ = IRECT(1137 * gui_scale, 181 * gui_scale, (1137 + 74) * gui_scale, (181 + 75) * gui_scale);

IRECT* Constants::SLIDER_RECT()
{
    return &SLIDER_RECT_;

}
IRECT Constants::SLIDER_RECT_ = IRECT(33 * gui_scale, 668 * gui_scale, (33 + 128) * gui_scale, (668 + 247) * gui_scale);

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