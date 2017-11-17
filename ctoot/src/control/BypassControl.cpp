#include <control/BypassControl.hpp>
#include <control/BooleanControl.hpp>

using namespace ctoot::control;

BypassControl::BypassControl(int id) : BooleanControl(id, "Bypass",false)
{
	setHidden(true);
}
