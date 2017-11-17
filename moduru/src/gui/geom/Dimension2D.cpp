#include <gui/geom/Dimension2D.hpp>

using namespace moduru::gui::geom;

Dimension2D::Dimension2D()
{
}

void Dimension2D::setSize(Dimension2D* d)
{
    setSize(d->getWidth(), d->getHeight());
}
