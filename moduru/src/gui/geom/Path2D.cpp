#include <gui/geom/Path2D.hpp>

#include <gui/geom/Point2D.hpp>
#include <gui/Rectangle.hpp>
#include <gui/geom/PathIterator.hpp>
#include <gui/geom/FlatteningPathIterator.hpp>
#include <gui/geom/Curve.hpp>

#include <lang/Double.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::gui::geom;
using namespace std;

Path2D::Path2D()
{
}

bool Path2D::$assertionsDisabled;

const int Path2D::EXPAND_MAX;
const int Path2D::EXPAND_MAX_COORDS;
const int Path2D::EXPAND_MIN;
const int Path2D::INIT_SIZE;

const int8_t Path2D::SEG_CLOSE;
const int8_t Path2D::SEG_CUBICTO;
const int8_t Path2D::SEG_LINETO;
const int8_t Path2D::SEG_MOVETO;
const int8_t Path2D::SEG_QUADTO;
const int8_t Path2D::SERIAL_PATH_END;
const int8_t Path2D::SERIAL_SEG_CLOSE;
const int8_t Path2D::SERIAL_SEG_DBL_CUBICTO;
const int8_t Path2D::SERIAL_SEG_DBL_LINETO;
const int8_t Path2D::SERIAL_SEG_DBL_MOVETO;
const int8_t Path2D::SERIAL_SEG_DBL_QUADTO;
const int8_t Path2D::SERIAL_SEG_FLT_CUBICTO;
const int8_t Path2D::SERIAL_SEG_FLT_LINETO;
const int8_t Path2D::SERIAL_SEG_FLT_MOVETO;
const int8_t Path2D::SERIAL_SEG_FLT_QUADTO;
const int8_t Path2D::SERIAL_STORAGE_DBL_ARRAY;
const int8_t Path2D::SERIAL_STORAGE_FLT_ARRAY;
const int Path2D::WIND_EVEN_ODD;
const int Path2D::WIND_NON_ZERO;

Path2D::Path2D(int rule, int initialTypes)
{
	setWindingRule(rule);
	this->pointTypes = vector<int8_t>(initialTypes);
}

void Path2D::closePath()
{
	if (numTypes == 0 || pointTypes[numTypes - 1] != SEG_CLOSE) {
		needRoom(true, 0);
		pointTypes[numTypes++] = SEG_CLOSE;
	}
}

int Path2D::getWindingRule()
{
	return windingRule;
}

void Path2D::setWindingRule(int rule)
{
	if (rule != WIND_EVEN_ODD && rule != WIND_NON_ZERO) {
		throw std::invalid_argument("winding rule must be WIND_EVEN_ODD or WIND_NON_ZERO");
	}
	windingRule = rule;
}

Point2D* Path2D::getCurrentPoint()
{
	auto index = numCoords;
	if (numTypes < 1 || index < 1) {
		return nullptr;
	}
	if (pointTypes[numTypes - 1] == SEG_CLOSE) {
		for (auto i = numTypes - 2; i > 0; i--) {
			bool loop0_continue = false;
			switch (pointTypes[i]) {
			case SEG_MOVETO:
				goto loop0_break;
			case SEG_LINETO:
				index -= 2;
				break;
			case SEG_QUADTO:
				index -= 4;
				break;
			case SEG_CUBICTO:
				index -= 6;
				break;
			case SEG_CLOSE:
				break;
			}


		}

	loop0_break:;
	}
	return getPoint(index - 2);
}

void Path2D::reset()
{
	numTypes = numCoords = 0;
}

moduru::gui::Shape* Path2D::createTransformedShape(AffineTransform* at)
{
	auto p2d = dynamic_cast<Path2D*>(clone());
	if (at != nullptr) {
		p2d->transform(at);
	}
	return p2d;
}

moduru::gui::Rectangle* Path2D::getBounds()
{
	return dynamic_cast<moduru::gui::Rectangle*>(getBounds2D()->getBounds());
}

bool Path2D::contains(PathIterator* pi, double x, double y)
{
	if (x * 0.0 + y * 0.0 == 0.0) {
		auto mask = (pi->getWindingRule() == WIND_NON_ZERO ? -1 : 1);
		auto cross = Curve::pointCrossingsForPath(pi, x, y);
		return ((cross & mask) != 0);
	}
	else {
		return false;
	}
}

bool Path2D::contains(PathIterator* pi, Point2D* p)
{
	return contains(pi, p->getX(), p->getY());
}

bool Path2D::contains(double x, double y)
{
	if (x * 0.0 + y * 0.0 == 0.0) {
		if (numTypes < 2) {
			return false;
		}
		auto mask = (windingRule == WIND_NON_ZERO ? -1 : 1);
		return ((pointCrossings(x, y) & mask) != 0);
	}
	else {
		return false;
	}
}

bool Path2D::contains(Point2D* p)
{
	return contains(p->getX(), p->getY());
}

bool Path2D::contains(PathIterator* pi, double x, double y, double w, double h)
{
	if (moduru::lang::Double::isNaN(x + w) || moduru::lang::Double::isNaN(y + h)) {
		return false;
	}
	if (w <= 0 || h <= 0) {
		return false;
	}
	auto mask = (pi->getWindingRule() == WIND_NON_ZERO ? -1 : 2);
	auto crossings = Curve::rectCrossingsForPath(pi, x, y, x + w, y + h);
	return (crossings != Curve::RECT_INTERSECTS && (crossings & mask) != 0);
}

bool Path2D::contains(PathIterator* pi, Rectangle2D* r)
{
	return contains(pi, r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool Path2D::contains(double x, double y, double w, double h)
{
	if (moduru::lang::Double::isNaN(x + w) || moduru::lang::Double::isNaN(y + h)) {
		return false;
	}
	if (w <= 0 || h <= 0) {
		return false;
	}
	auto mask = (windingRule == WIND_NON_ZERO ? -1 : 2);
	auto crossings = rectCrossings(x, y, x + w, y + h);
	return (crossings != Curve::RECT_INTERSECTS && (crossings & mask) != 0);
}

bool Path2D::contains(Rectangle2D* r)
{
	return contains(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool Path2D::intersects(PathIterator* pi, double x, double y, double w, double h)
{
	if (moduru::lang::Double::isNaN(x + w) || moduru::lang::Double::isNaN(y + h)) {
		return false;
	}
	if (w <= 0 || h <= 0) {
		return false;
	}
	auto mask = (pi->getWindingRule() == WIND_NON_ZERO ? -1 : 2);
	auto crossings = Curve::rectCrossingsForPath(pi, x, y, x + w, y + h);
	return (crossings == Curve::RECT_INTERSECTS || (crossings & mask) != 0);
}

bool Path2D::intersects(PathIterator* pi, Rectangle2D* r)
{
	return intersects(pi, r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool Path2D::intersects(double x, double y, double w, double h)
{
	if (moduru::lang::Double::isNaN(x + w) || moduru::lang::Double::isNaN(y + h)) {
		return false;
	}
	if (w <= 0 || h <= 0) {
		return false;
	}
	auto mask = (windingRule == WIND_NON_ZERO ? -1 : 2);
	auto crossings = rectCrossings(x, y, x + w, y + h);
	return (crossings == Curve::RECT_INTERSECTS || (crossings & mask) != 0);
}

bool Path2D::intersects(Rectangle2D* r)
{
	return intersects(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

PathIterator* Path2D::getPathIterator(AffineTransform* at, double flatness)
{
	return new FlatteningPathIterator(getPathIterator(at), flatness);
}

/*
void Path2D::writeObject(::java::io::ObjectOutputStream* s, bool isdbl)
{
	s->defaultWriteObject();
	::floatArray* fCoords;
	vector<double>* dCoords;
	if (isdbl) {
		dCoords = npc((dynamic_cast< Path2D_Double* >(this)))->doubleCoords;
		fCoords = nullptr;
	}
	else {
		fCoords = npc((dynamic_cast< Path2D_Float* >(this)))->floatCoords;
		dCoords = nullptr;
	}
	auto numTypes = this->numTypes;
	s->writeByte(static_cast< int >(isdbl ? SERIAL_STORAGE_DBL_ARRAY : SERIAL_STORAGE_FLT_ARRAY));
	s->writeInt(numTypes);
	s->writeInt(numCoords);
	s->writeByte(static_cast< int >(static_cast< int8_t >(windingRule)));
	auto cindex = 0;
	for (auto i = 0; i < numTypes; i++) {
		int npoints;
		int8_t serialtype;
		switch (pointTypes[i]) {
		case SEG_MOVETO:
			npoints = 1;
			serialtype = (isdbl ? SERIAL_SEG_DBL_MOVETO : SERIAL_SEG_FLT_MOVETO);
			break;
		case SEG_LINETO:
			npoints = 1;
			serialtype = (isdbl ? SERIAL_SEG_DBL_LINETO : SERIAL_SEG_FLT_LINETO);
			break;
		case SEG_QUADTO:
			npoints = 2;
			serialtype = (isdbl ? SERIAL_SEG_DBL_QUADTO : SERIAL_SEG_FLT_QUADTO);
			break;
		case SEG_CUBICTO:
			npoints = 3;
			serialtype = (isdbl ? SERIAL_SEG_DBL_CUBICTO : SERIAL_SEG_FLT_CUBICTO);
			break;
		case SEG_CLOSE:
			npoints = 0;
			serialtype = SERIAL_SEG_CLOSE;
			break;
		default:
			throw new ::InternalError(u"unrecognized path type"_j);
		}

		s->writeByte(static_cast< int >(serialtype));
		while (--npoints >= 0) {
			if (isdbl) {
				s->writeDouble((*dCoords)[cindex++]);
				s->writeDouble((*dCoords)[cindex++]);
			}
			else {
				s->writeFloat((*fCoords)[cindex++]);
				s->writeFloat((*fCoords)[cindex++]);
			}
		}
	}
	s->writeByte(static_cast< int >(static_cast< int8_t >(SERIAL_PATH_END)));
}

void Path2D::readObject(::java::io::ObjectInputStream* s, bool storedbl)
{
	s->defaultReadObject();
	s->readByte();
	auto nT = s->readInt();
	auto nC = s->readInt();
	try {
		setWindingRule(s->readByte());
	}
	catch (::IllegalArgumentException* iae) {
		throw new ::java::io::InvalidObjectException(npc(iae)->getMessage());
	}
	pointTypes = new ::int8_tArray((nT < 0) ? INIT_SIZE : nT);
	if (nC < 0) {
		nC = INIT_SIZE * 2;
	}
	if (storedbl) {
		npc((dynamic_cast< Path2D_Double* >(this)))->doubleCoords = new ::doubleArray(nC);
	}
	else {
		npc((dynamic_cast< Path2D_Float* >(this)))->floatCoords = new ::floatArray(nC);
	}
	for (auto i = 0; nT < 0 || i < nT; i++) {
		bool PATHDONE0_continue = false;
		bool isdbl;
		int npoints;
		int8_t segtype;
		auto serialtype = s->readByte();
		switch (serialtype) {
		case SERIAL_SEG_FLT_MOVETO:
			isdbl = false;
			npoints = 1;
			segtype = SEG_MOVETO;
			break;
		case SERIAL_SEG_FLT_LINETO:
			isdbl = false;
			npoints = 1;
			segtype = SEG_LINETO;
			break;
		case SERIAL_SEG_FLT_QUADTO:
			isdbl = false;
			npoints = 2;
			segtype = SEG_QUADTO;
			break;
		case SERIAL_SEG_FLT_CUBICTO:
			isdbl = false;
			npoints = 3;
			segtype = SEG_CUBICTO;
			break;
		case SERIAL_SEG_DBL_MOVETO:
			isdbl = true;
			npoints = 1;
			segtype = SEG_MOVETO;
			break;
		case SERIAL_SEG_DBL_LINETO:
			isdbl = true;
			npoints = 1;
			segtype = SEG_LINETO;
			break;
		case SERIAL_SEG_DBL_QUADTO:
			isdbl = true;
			npoints = 2;
			segtype = SEG_QUADTO;
			break;
		case SERIAL_SEG_DBL_CUBICTO:
			isdbl = true;
			npoints = 3;
			segtype = SEG_CUBICTO;
			break;
		case SERIAL_SEG_CLOSE:
			isdbl = false;
			npoints = 0;
			segtype = SEG_CLOSE;
			break;
		case SERIAL_PATH_END:
			if (nT < 0) {
				goto PATHDONE0_break;
			}
			throw new ::java::io::StreamCorruptedException(u"unexpected PATH_END"_j);
		default:
			throw new ::java::io::StreamCorruptedException(u"unrecognized path type"_j);
		}

		needRoom(segtype != SEG_MOVETO, npoints * 2);
		if (isdbl) {
			while (--npoints >= 0) {
				append(s->readDouble(), s->readDouble());
			}
		}
		else {
			while (--npoints >= 0) {
				append(s->readFloat(), s->readFloat());
			}
		}
		pointTypes[numTypes++] = segtype;

	}

PATHDONE0_break:;
	if (nT >= 0 && s->readByte() != SERIAL_PATH_END) {
		throw new ::java::io::StreamCorruptedException(u"missing PATH_END"_j);
	}
}
*/
