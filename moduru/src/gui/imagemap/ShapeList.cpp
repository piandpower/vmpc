#include <gui/imagemap/ShapeList.hpp>

#include <gui/imagemap/Shape.hpp>
#include <lang/Object.hpp>
#include <lang/String.hpp>
#include <lang/StringBuilder.hpp>

using namespace moduru::gui::imagemap;
using namespace std;

ShapeList::ShapeList() 
{
	mapName = new moduru::lang::String("MAP");
}

void ShapeList::add_shape(std::shared_ptr<Shape> s)
{
	vec.push_back(s);
}

weak_ptr<Shape> ShapeList::get_shape(int i)
{
	return vec[i];
}

void ShapeList::remove_shape(std::weak_ptr<Shape> s)
{
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == s.lock()) vec.erase(vec.begin() + i);
}

int ShapeList::size()
{
    return vec.size();
}

void ShapeList::clear()
{
    vec.clear();
}

bool ShapeList::isShapePointAt(int x, int y)
{
	for (auto i = 0; i < size(); i++) {
		foundKeyPoint = vec[i]->isKeyPoint(x, y);
		if (foundKeyPoint != 0) {
			foundShape = vec[i];
			return true;
		}
	}
	return false;
}

bool ShapeList::isPointInsideShape(int x, int y)
{
	for (auto i = size() - 1; i >= 0; i--) {
		if (vec[i]->inside(x, y)) {
			foundShape = vec[i];
			foundKeyPoint = 0;
			return true;
		}
	}
	return false;
}

std::weak_ptr<Shape> ShapeList::getFoundShape()
{
    return foundShape;
}

int ShapeList::getFoundKeyPoint()
{
    return foundKeyPoint;
}

int ShapeList::getFoundShapeIndex()
{
	int res;
	for (res = 0; res < vec.size(); res++)
		if (vec[res] == foundShape.lock())
			return res;
	return -1;
}

void ShapeList::removeFoundShape()
{
	if (foundShape.lock())
		vec.erase(vec.begin() + getFoundShapeIndex());
}

void ShapeList::addPoint(int x, int y)
{
    for (auto i = 0; i < size(); i++) {
		auto s = vec[i];
        if(s->tryAddPoint(x, y))
            return;

    }
}

void ShapeList::removePointAt(int x, int y)
{
	for (auto i = 0; i < size(); i++) {
		auto s = vec[i];
		if (s->removePointAt(x, y))
			return;
	}
}

moduru::lang::String* ShapeList::get_html(moduru::lang::String* imgSrc, int w, int h)
{
    auto str = moduru::lang::String(moduru::lang::StringBuilder().append("<img src=\"")->append(imgSrc)
        ->append("\" width=\"")
        ->append(w)
        ->append("\" height=\"")
        ->append(h)
        ->append("\" usemap=\"#")
        ->append(mapName)
        ->append("\" border=\"0\">\n<map name=\"")
        ->append(mapName)
        ->append("\">\n")->toString());
    for (auto i = 0; i < size(); i++) {
        auto s = vec[i];
        str = moduru::lang::StringBuilder(&str).append(s->get_html())->toString();
    }
    str = moduru::lang::StringBuilder(&str).append("</map>")->toString();
	return new moduru::lang::String(str);
}

ShapeList::~ShapeList() {
	delete mapName;
	for (auto& s : vec) {
		s.reset();
	}
}
