#include <control/ControlColumn.hpp>

using namespace ctoot::control;

ControlColumn::ControlColumn() : CompoundControl(0, "ControlColumn")
{
}

bool ControlColumn::isAlwaysVertical()
{
    return true;
}

bool ControlColumn::isNeverBordered()
{
    return true;
}