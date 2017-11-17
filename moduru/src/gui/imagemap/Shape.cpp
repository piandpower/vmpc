#include <gui/imagemap/Shape.hpp>

//#include <gui/BasicStroke.hpp>
//#include <gui/Color.hpp>
//#include <gui/Graphics2D.hpp>
#include <gui/Point.hpp>
#include <gui/Polygon.hpp>
#include <gui/Rectangle.hpp>
//#include <gui/Stroke.hpp>
#include <gui/geom/Line2D.hpp>
#include <gui/geom/Point2D.hpp>
#include <lang/String.hpp>
#include <lang/StringBuilder.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <cstdlib>
#endif

using namespace moduru::gui::imagemap;
using namespace std;

Shape::Shape(int x, int y, int x2, int y2)
{
	this->type = TYPE_RECT_;
	this->rect = std::make_shared<moduru::gui::Rectangle>();
	set_rect(x, y, x2, y2);
	href = make_shared<moduru::lang::String>("http://");
}

Shape::Shape(std::shared_ptr<moduru::gui::Point> m, int r) 
{
	this->type = TYPE_CIRCLE_;
	this->circle_center = m;
	this->circle_r = r;
	href = make_shared<moduru::lang::String>("http://");
}

Shape::Shape(std::shared_ptr<moduru::gui::Point> first) 
{
	this->type = TYPE_POLY_;
	poly = std::make_shared<moduru::gui::Polygon>();
	poly->addPoint(static_cast< int >(first->getX()), static_cast< int >(first->getY()));
	href = make_shared<moduru::lang::String>("http://");
}

Shape::Shape(std::shared_ptr<moduru::gui::Polygon> p) 
{
    this->type = TYPE_POLY_;
	poly = std::move(p);
    href = make_shared<moduru::lang::String>("http://");
}

void Shape::set_href(moduru::lang::String* href)
{
//    if(href == nullptr)
        //return;

    this->href = make_shared<moduru::lang::String>(href);
}

void Shape::set_alt(moduru::lang::String* alt)
{
    if(alt == nullptr)
        return;

    this->alt = make_shared<moduru::lang::String>(alt);
}

void Shape::set_onMouseOver(moduru::lang::String* s)
{
    if(s == nullptr)
        return;

    this->onMouseOver = make_shared<moduru::lang::String>(s);
}

void Shape::set_onMouseOut(moduru::lang::String* s)
{
    if(s == nullptr)
        return;

    this->onMouseOut = make_shared<moduru::lang::String>(s);
}

void Shape::set_onClick(moduru::lang::String* s)
{
    if(s == nullptr)
        return;

    this->onClick = make_shared<moduru::lang::String>(s);
}

std::weak_ptr<moduru::lang::String> Shape::get_href()
{
    return href;
}

std::weak_ptr<moduru::lang::String> Shape::get_alt()
{
    return alt;
}

moduru::lang::String Shape::get_typeString()
{
	if (type == TYPE_RECT_)
		return moduru::lang::String("Rectangle");
	else if (type == TYPE_CIRCLE_)
		return moduru::lang::String("Circle");
	else
		return moduru::lang::String("Polygon");
}

/*
void Shape::draw(moduru::gui::Graphics2D* g2, int zoomFactor)
{
    auto dash1 = (::floatArray({4.0f}));
    auto oldStroke = npc(g2)->getStroke();
    auto bs = moduru::gui::BasicStroke(1.0f, moduru::gui::BasicStroke::CAP_BUTT, moduru::gui::BasicStroke::JOIN_MITER, 3.0f, dash1, 0.0f);
    if(type == TYPE_RECT_) {
        int x = static_cast< int >(rect->getX()) * zoomFactor, y = static_cast< int >(rect->getY()) * zoomFactor;
        int w = static_cast< int >(rect->getWidth()) * zoomFactor, h = static_cast< int >(rect->getHeight()) * zoomFactor;
        npc(g2)->setColor(moduru::gui::Color::black());
        npc(g2)->drawRect(x, y, w, h);
        npc(g2)->setStroke(bs);
        npc(g2)->setColor(moduru::gui::Color::white());
        npc(g2)->drawRect(x, y, w, h);
        npc(g2)->setStroke(oldStroke);
        npc(g2)->setColor(moduru::gui::Color::black());
        npc(g2)->fillRect(x - 2, y - 2, 4, 4);
        npc(g2)->fillRect(x + w - 2, y - 2, 4, 4);
        npc(g2)->fillRect(x + w - 2, y + h - 2, 4, 4);
        npc(g2)->fillRect(x - 2, y + h - 2, 4, 4);
        npc(g2)->setColor(moduru::gui::Color::white());
        npc(g2)->drawRect(x - 2, y - 2, 4, 4);
        npc(g2)->drawRect(x + w - 2, y - 2, 4, 4);
        npc(g2)->drawRect(x + w - 2, y + h - 2, 4, 4);
        npc(g2)->drawRect(x - 2, y + h - 2, 4, 4);
    } else if(type == TYPE_CIRCLE_) {
        auto x = static_cast< int >(circle_center->getX()) * zoomFactor;
        auto y = static_cast< int >(circle_center->getY()) * zoomFactor;
        auto r = circle_r * zoomFactor;
        npc(g2)->setColor(moduru::gui::Color::black());
        npc(g2)->drawOval(x - r, y - r, r * 2, r * 2);
        npc(g2)->setStroke(bs);
        npc(g2)->setColor(moduru::gui::Color::white());
        npc(g2)->drawOval(x - r, y - r, r * 2, r * 2);
        npc(g2)->setStroke(oldStroke);
        npc(g2)->setColor(moduru::gui::Color::black());
        npc(g2)->fillRect(x + r - 2, y - 2, 4, 4);
        npc(g2)->setColor(moduru::gui::Color::white());
        npc(g2)->drawRect(x + r - 2, y - 2, 4, 4);
    } else if(type == TYPE_POLY_) {
        auto pg = poly;
        if(zoomFactor != 1) {
            pg =  moduru::gui::Polygon();
            for (auto i = 0; i < poly->npoints; i++) 
                                npc(pg)->addPoint(poly->xpoints[i] * zoomFactor, poly->ypoints[i] * zoomFactor);

        }
        npc(g2)->setColor(moduru::gui::Color::black());
        npc(g2)->drawPolygon(pg);
        npc(g2)->setStroke(bs);
        npc(g2)->setColor(moduru::gui::Color::white());
        npc(g2)->drawPolygon(pg);
        npc(g2)->setStroke(oldStroke);
        npc(g2)->setColor(moduru::gui::Color::black());
        for (auto i = 0; i < poly->npoints; i++) 
                        npc(g2)->fillRect(poly->xpoints[i] * zoomFactor - 2, poly->ypoints[i] * zoomFactor - 2, 4, 4);

        npc(g2)->setColor(moduru::gui::Color::white());
        for (auto i = 0; i < poly->npoints; i++) 
                        npc(g2)->drawRect(poly->xpoints[i] * zoomFactor - 2, poly->ypoints[i] * zoomFactor - 2, 4, 4);

    }
}
*/

void Shape::set_rect(int x1, int y1, int x2, int y2)
{
    int x, y, w, h;
    if(x2 > x1) {
        x = x1;
        w = x2 - x1;
    } else {
        x = x2;
        w = x1 - x2;
    }
    if(y2 > y1) {
        y = y1;
        h = y2 - y1;
    } else {
        y = y2;
        h = y1 - y2;
    }
    rect = make_shared<moduru::gui::Rectangle>(x, y, w, h);
}

void Shape::set_rect_ul(int mx, int my)
{
    int x = rect->x, w = rect->width, y = rect->y, h = rect->height;
    if(mx < rect->x + rect->width) {
        x = mx;
        w = rect->x + rect->width - mx;
    } else {
        x = rect->x + rect->width - 1;
        w = 1;
    }
    if(my < rect->y + rect->height) {
        y = my;
        h = rect->y + rect->height - my;
    } else {
        y = rect->y + rect->height - 1;
        h = 1;
    }
    rect->setLocation(x, y);
    rect->setSize(w, h);
}

void Shape::set_rect_lr(int mx, int my)
{
    auto w = mx - rect->x;
    if(w < 1)
        w = 1;

    auto h = my - rect->y;
    if(h < 1)
        h = 1;

    rect->setSize(w, h);
}

void Shape::set_rect_ur(int mx, int my)
{
    int y = rect->y, h = rect->height;
    auto w = mx - rect->x;
    if(w < 1)
        w = 1;

    if(my < rect->y + rect->height) {
        y = my;
        h = rect->y + rect->height - my;
    } else {
        y = rect->y + rect->height - 1;
        h = 1;
    }
    rect->setLocation(rect->x, y);
    rect->setSize(w, h);
}

void Shape::set_rect_ll(int mx, int my)
{
    int x = rect->x, w = rect->width;
    if(mx < rect->x + rect->width) {
        x = mx;
        w = rect->x + rect->width - mx;
    } else {
        x = rect->x + rect->width - 1;
        w = 1;
    }
    auto h = my - rect->y;
    if(h < 1)
        h = 1;

    rect->setLocation(x, rect->y);
    rect->setSize(w, h);
}

void Shape::set_circle(moduru::gui::Point* m, int r)
{
    circle_center = make_shared<moduru::gui::Point>(m);
    circle_r = r;
}

void Shape::set_polypoint(int p, int x, int y)
{
	poly->xpoints[p - 1] = x;
	poly->ypoints[p - 1] = y;
}

int Shape::add_polypoint(int x, int y)
{
    poly->addPoint(x, y);
    return poly->npoints;
}

void Shape::remove_polypoint(int p)
{
    if(type != TYPE_POLY_ || poly->npoints <= 3)
        return;

	auto pnew = make_shared<moduru::gui::Polygon>();
    for (auto i = 0; i < poly->npoints; i++) 
                if(i != p - 1)
            pnew->addPoint(poly->xpoints[i], poly->ypoints[i]);

	poly.swap(pnew);
    //poly = pnew;
}

void Shape::remove_lastPolyPoint()
{
    if(type != TYPE_POLY_)
        return;

    remove_polypoint(poly->npoints);
}

int Shape::get_type()
{
    return type;
}

weak_ptr<moduru::gui::Rectangle> Shape::getRect()
{
    return rect;
}

int Shape::isKeyPoint(int mx, int my)
{
    if(type == TYPE_RECT_) {
        int x = static_cast< int >(rect->getX()), y = static_cast< int >(rect->getY());
        int w = static_cast< int >(rect->getWidth()), h = static_cast< int >(rect->getHeight());
        if(mx >= x - 2 && mx <= x + 2 && my >= y - 2 && my <= y + 2)
            return POINT_RECT_UPPERLEFT_;

        if(mx >= x + w - 2 && mx <= x + w + 2 && my >= y - 2 && my <= y + 2)
            return POINT_RECT_UPPERRIGHT_;

        if(mx >= x + w - 2 && mx <= x + w + 2 && my >= y + h - 2 && my <= y + h + 2)
            return POINT_RECT_LOWERRIGHT_;

        if(mx >= x - 2 && mx <= x + 2 && my >= y + h - 2 && my <= y + h + 2)
            return POINT_RECT_LOWERLEFT_;

        if(inside(mx, my)) {
            mouseDragPosition = std::make_shared<moduru::gui::Point>(mx - x, my - y);
            return POINT_RECT_INSIDE_;
        }
    } else if(type == TYPE_CIRCLE_) {
        auto x = static_cast< int >(circle_center->x);
        auto y = static_cast< int >(circle_center->y);
        auto r = circle_r;
        if(mx >= x + r - 2 && mx <= x + r + 2 && my >= y - 2 && my <= y + 2)
            return POINT_CIRCLE_RADIUS_;

        if(inside(mx, my)) {
            mouseDragPosition = std::make_shared<moduru::gui::Point>(mx - x, my - y);
            return POINT_CIRCLE_INSIDE_;
        }
    } else if(type == TYPE_POLY_) {
        for (auto i = 0; i < poly->npoints; i++) {
            if(mx >= poly->xpoints[i] - 3 && mx <= poly->xpoints[i] + 3 && my >= poly->ypoints[i] - 3 && my <= poly->ypoints[i] + 3)
                return i + 1;

        }
        if(inside(mx, my)) {
            mouseDragPosition = std::make_shared<moduru::gui::Point>(mx - poly->xpoints[0], my - poly->ypoints[0]);
            return POINT_POLY_INSIDE_;
        }
    }
    return 0;
}

void Shape::moveKeyPoint(int p, int mx, int my)
{
    if(p == POINT_RECT_UPPERLEFT_)
        set_rect_ul(mx, my);
    else if(p == POINT_RECT_LOWERRIGHT_)
        set_rect_lr(mx, my);
    else if(p == POINT_RECT_UPPERRIGHT_)
        set_rect_ur(mx, my);
    else if(p == POINT_RECT_LOWERLEFT_)
        set_rect_ll(mx, my);
    else if(p == POINT_RECT_INSIDE_)
        set_rect(mx - mouseDragPosition->x, my - mouseDragPosition->y, mx - mouseDragPosition->x + rect->width, my - mouseDragPosition->y + rect->height);
    else if(p == POINT_CIRCLE_RADIUS_) {
        if(mx > static_cast< int >(circle_center->getX())) {
            auto point = moduru::gui::Point(circle_center->x, circle_center->y);
            set_circle(&point, mx - circle_center->x);
        }

    } else if(p == POINT_CIRCLE_INSIDE_) {
        auto point = moduru::gui::Point(mx - mouseDragPosition->x, my - mouseDragPosition->y);
        set_circle(&point, circle_r);
    }
    else if(p == POINT_POLY_INSIDE_) {
        auto dx = mx - mouseDragPosition->x - poly->xpoints[0];
        auto dy = my - mouseDragPosition->y - poly->ypoints[0];
        for (auto i = 0; i < poly->npoints; i++) 
                        set_polypoint(i + 1, poly->xpoints[i] + dx, poly->ypoints[i] + dy);

    } else if(p > 0)
        set_polypoint(p, mx, my);

}

moduru::lang::String* Shape::get_html()
{
	moduru::lang::String str;
    if(type == TYPE_RECT_) {
        str = moduru::lang::String(moduru::lang::StringBuilder().append("<area shape=rect coords=\"")->append(rect->x)
            ->append(",")
            ->append(rect->y)
            ->append(",")
            ->append(static_cast< int >((rect->x + rect->width)))
            ->append(",")
            ->append(static_cast< int >((rect->y + rect->height)))->toString());
    } else if(type == TYPE_CIRCLE_) {
        str = moduru::lang::String(moduru::lang::StringBuilder().append("<area shape=circle coords=\"")->append(circle_center->x)
            ->append(",")
            ->append(circle_center->y)
            ->append(",")
            ->append(circle_r)->toString());
    } else if(type == TYPE_POLY_) {
        str = moduru::lang::String("<area shape=poly coords=\"");
        for (auto i = 0; i < poly->npoints; i++) {
            if(i > 0)
                str = moduru::lang::StringBuilder(&str).append(",")->toString();

            str = moduru::lang::StringBuilder(&str).append(moduru::lang::StringBuilder().append("")->append(poly->xpoints[i])
                ->append(",")
                ->append(poly->ypoints[i])->toString())->toString();
        }
    }
    str = moduru::lang::StringBuilder(&str).append(moduru::lang::StringBuilder().append("\" href=\"")->append(href.get())
        ->append("\" alt=\"")
        ->append(alt.get())
        ->append("\"")->toString())->toString();
    if(onClick->length() > 0)
        str = moduru::lang::StringBuilder(&str).append(moduru::lang::StringBuilder().append(" onClick=\"")->append(onClick.get())
            ->append("\"")->toString())->toString();

    if(onMouseOver->length() > 0)
        str = moduru::lang::StringBuilder(&str).append(moduru::lang::StringBuilder().append(" onMouseOver=\"")->append(onMouseOver.get())
            ->append("\"")->toString())->toString();

    if(onMouseOut->length() > 0)
        str = moduru::lang::StringBuilder(&str).append(moduru::lang::StringBuilder().append(" onMouseOut=\"")->append(onMouseOut.get())
            ->append("\"")->toString())->toString();

    str = moduru::lang::StringBuilder(&str).append(">\n")->toString();
	return new moduru::lang::String(str);
}

bool Shape::inside(int x, int y)
{
    if(type == TYPE_RECT_) {
        if(rect->contains(x, y))
            return true;

    } else if(type == TYPE_CIRCLE_) {
        auto mr = static_cast< int >(sqrt((x - circle_center->x) * (x - circle_center->x) + (y - circle_center->y) * (y - circle_center->y)));
        if(mr < circle_r)
            return true;

    } else if(type == TYPE_POLY_) {
        if(poly->contains(x, y))
            return true;

    }
    return false;
}

void Shape::convert()
{
    if(type != TYPE_RECT_)
        return;

    poly = make_shared<moduru::gui::Polygon>();
    poly->addPoint(rect->x, rect->y);
    poly->addPoint(rect->x + rect->width, rect->y);
    poly->addPoint(rect->x + rect->width, rect->y + rect->height);
    poly->addPoint(rect->x, rect->y + rect->height);
    type = TYPE_POLY_;
	rect.reset();
}

void Shape::convert(int n)
{
    if(type != TYPE_CIRCLE_)
        return;

    poly = make_shared<moduru::gui::Polygon>();
    for (auto i = 0; i < n; i++) 
                poly->addPoint(circle_center->x + static_cast< int >((cos(i * M_PI * 2 / n) * circle_r)), circle_center->y + static_cast< int >((sin(i * M_PI * 2 / n) * circle_r)));

    type = TYPE_POLY_;
    circle_center = nullptr;
}

moduru::gui::Point* Shape::minDistance(int x, int y, Shape* activeShape, int movingPoint)
{
	if (type == TYPE_POLY_) {
		auto p = moduru::gui::Point(int32_t(10000), int32_t(10000));
		for (auto i = 0; i < poly->npoints; i++) {
			auto np = moduru::gui::Point(poly->xpoints[i], poly->ypoints[i]);
			if (np.distance(static_cast<double>(x), static_cast<double>(y)) < p.distance(static_cast<double>(x), static_cast<double>(y)))
				if (!(activeShape == this && i + 1 == movingPoint))
					p = np;
		}
		return new moduru::gui::Point(&p);
	}
	else if (type == TYPE_RECT_) {
		if (activeShape == this)
			return nullptr;

		auto points = std::vector<moduru::gui::Point>({
			rect->getLocation()
			, moduru::gui::Point(rect->x + rect->width, rect->y)
			, moduru::gui::Point(rect->x + rect->width, rect->y + rect->height)
			, moduru::gui::Point(rect->x, rect->y + rect->height)
		});
		auto p = points[0];
		for (auto i = 1; i < 4; i++)
			if (points[i].distance(x, y) < p.distance(x, y))
				p = points[i];
		return new moduru::gui::Point(&p);
	}
	else {

		if (activeShape == this)
			return nullptr;

		auto points = std::vector<moduru::gui::Point>({
			*circle_center.get()
			, moduru::gui::Point(circle_center->x + circle_r, circle_center->y)
			, moduru::gui::Point(circle_center->x - circle_r, circle_center->y)
			, moduru::gui::Point(circle_center->x, circle_center->y - circle_r)
			, moduru::gui::Point(circle_center->x, circle_center->y + circle_r)
		});
		auto p = points[0];
		for (auto i = 1; i < 5; i++)
			if (points[i].distance(x, y) < p.distance(x, y))
				p = points[i];
		return new moduru::gui::Point(&p);
	}
}

int Shape::minXDistance(int x)
{
	if (type == TYPE_POLY_) {
		auto p = int32_t(10000);
		for (auto i = 0; i < poly->npoints; i++) {
			auto np = poly->xpoints[i];
			if (abs(x - np) < abs(x - p))
				p = np;

		}
		return p;
	}
	else if (type == TYPE_RECT_) {
		auto points = vector<int32_t>({
			rect->x
			, rect->x + rect->height
		});
		auto p = int32_t(10000);
		for (auto i = 0; i < points.size(); i++)
			if (abs(x - points[i]) < abs(x - p))
				p = points[i];
		return p;
	}
	else {
		auto points = vector<int32_t>({
			circle_center->x
			, circle_center->x + circle_r
			, circle_center->x - circle_r
		});
		auto p = int32_t(10000);
		for (auto i = 0; i < points.size(); i++)
			if (abs(x - points[i]) < abs(x - p))
				p = points[i];
		return p;
	}
}

int Shape::minYDistance(int y)
{
	if (type == TYPE_POLY_) {
		auto p = int32_t(10000);
		for (auto i = 0; i < poly->npoints; i++) {
			auto np = poly->ypoints[i];
			if (abs(y - np) < abs(y - p))
				p = np;

		}
		return p;
	}
	else if (type == TYPE_RECT_) {
		auto points = vector<int32_t>({
			rect->y
			, rect->y + rect->height
		});
		auto p = int32_t(10000);
		for (auto i = 0; i < points.size(); i++)
			if (abs(y - points[i]) < abs(y - p))
				p = points[i];


		return p;
	}
	else {
		auto points = vector<int32_t>({
			circle_center->y
			, circle_center->y + circle_r
			, circle_center->y - circle_r
		});
		auto p = int32_t(10000);
		for (auto i = 0; i < points.size(); i++)
			if (abs(y - points[i]) < abs(y - p))
				p = points[i];
		return p;
	}
}

void Shape::insertPoint(int n, int x, int y)
{
	if (type != TYPE_POLY_)
		return;

	auto pnew = make_shared<moduru::gui::Polygon>();
	for (auto i = 0; i < poly->npoints; i++) {
		pnew->addPoint(poly->xpoints[i], poly->ypoints[i]);
		if (i == n)
			pnew->addPoint(x, y);
	}
	poly.swap(pnew);
}

bool Shape::tryAddPoint(int x, int y)
{
	if (type != TYPE_POLY_)
		return false;

	auto minDist = moduru::gui::geom::Line2D::ptSegDist(static_cast<double>(poly->xpoints[poly->npoints - 1]), static_cast<double>(poly->ypoints[poly->npoints - 1]), static_cast<double>(poly->xpoints[0]), static_cast<double>(poly->ypoints[0]), static_cast<double>(x), static_cast<double>(y));
	auto minPoint = poly->npoints - 1;
	for (auto i = 1; i < poly->npoints; i++) {
		auto l = moduru::gui::geom::Line2D::ptSegDist(static_cast<double>(poly->xpoints[i - 1]), static_cast<double>(poly->ypoints[i - 1]), static_cast<double>(poly->xpoints[i]), static_cast<double>(poly->ypoints[i]), static_cast<double>(x), static_cast<double>(y));
		if (l < minDist) {
			minDist = l;
			minPoint = i - 1;
		}
	}
	if (minDist < 10.0) {
		insertPoint(minPoint, x, y);
		return true;
	}
	return false;
}

bool Shape::removePointAt(int x, int y)
{
	if (type != TYPE_POLY_)
		return false;

	for (auto i = 0; i < poly->npoints; i++) {
		if (x >= poly->xpoints[i] - 2 && x <= poly->xpoints[i] + 2 && y >= poly->ypoints[i] - 2 && y <= poly->ypoints[i] + 2) {
			remove_polypoint(i + 1);
			return true;
		}
	}
	return false;
}

moduru::lang::String* Shape::getCoordString(int mouseX, int mouseY)
{
	if (type == TYPE_POLY_) {
		return moduru::lang::StringBuilder().append("x=")->append(mouseX)
			->append("  y=")
			->append(mouseY)->toString();
	}
	else if (type == TYPE_RECT_) {
		return moduru::lang::StringBuilder().append("x=")->append(rect->x)
			->append("  y=")
			->append(rect->y)
			->append("  w=")
			->append(rect->width)
			->append("  h=")
			->append(rect->height)->toString();
	}
	else {
		return moduru::lang::StringBuilder().append("x=")->append(circle_center->x)
			->append("  y=")
			->append(circle_center->y)
			->append("  r=")
			->append(circle_r)->toString();
	}
}

Shape::~Shape() {
/*
	if (rect != nullptr) {
		delete rect;
	}
	if (circle_center != nullptr) {
		delete circle_center;
	}
	if (poly != nullptr) {
		delete poly;
	}
	if (poly_center != nullptr) {
		delete poly_center;
	}
	if (href != nullptr) {
		delete href;
	}
	if (alt != nullptr) {
		delete alt;
	}
	if (onMouseOver != nullptr) {
		delete onMouseOver;
	}
	if (onClick != nullptr) {
		delete onClick;
	}
	if (onMouseOut != nullptr) {
		delete onMouseOut;
	}
	if (mouseDragPosition != nullptr) {
		delete mouseDragPosition;
	}
	*/
}
