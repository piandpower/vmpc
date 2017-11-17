#include <gui/Dimension.hpp>

//#include <gui/GraphicsEnvironment.hpp>
//#include <gui/Toolkit.hpp>

#include <cmath>

using namespace moduru::gui;

Dimension::Dimension() : Dimension(0, 0)
{
}

Dimension::Dimension(Dimension* d) 
	: Dimension((unsigned int) d->getWidth(), (unsigned int) d->getHeight())
{
}

Dimension::Dimension(unsigned int width, unsigned int height) 
{
	this->width = width;
	this->height = height;
}

double Dimension::getWidth()
{
    return width;
}

double Dimension::getHeight()
{
    return height;
}

void Dimension::setSize(double width, double height)
{
    this->width = static_cast< unsigned int >(ceil(width));
    this->height = static_cast< unsigned int >(ceil(height));
}

Dimension* Dimension::getSize()
{
    return new Dimension(width, height);
}

void Dimension::setSize(Dimension* d)
{
    setSize(d->width, d->height);
}

void Dimension::setSize(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
}

/*
bool Dimension::equals(moduru::lang::Object* obj)
{
    if(dynamic_cast< Dimension* >(obj) != nullptr) {
        auto d = dynamic_cast< Dimension* >(obj);
        return (width == d->width) && (height == d->height);
    }
    return false;
}

int Dimension::hashCode()
{
    auto sum = width + height;
    return sum * (sum + 1) / 2) + width;
}
*/

/*
String* Dimension::toString()
{
    return ::StringBuilder().append(npc(getClass())->getName())->append(u"[width="_j)
        ->append(width)
        ->append(u",height="_j)
        ->append(height)
        ->append(u"]"_j)->toString();
}
*/