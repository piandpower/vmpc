#include <control/ControlRow.hpp>

using namespace ctoot::control;

ControlRow::ControlRow() : CompoundControl(0, "ControlRow")
{
}

bool ControlRow::isAlwaysHorizontal()
{
    return true;
}

bool ControlRow::isNeverBordered()
{
    return true;
}
