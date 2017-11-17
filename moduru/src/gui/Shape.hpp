#pragma once

namespace moduru {
	namespace gui {

		namespace geom {
			class Rectangle2D;
			class Point2D;
			class PathIterator;
			class AffineTransform;
		}

		class Rectangle;

		class Shape
		{
		public:
			virtual bool contains(moduru::gui::geom::Point2D* p) = 0;
			virtual bool contains(moduru::gui::geom::Rectangle2D* r) = 0;
			virtual bool contains(double x, double y) = 0;
			virtual bool contains(double x, double y, double w, double h) = 0;
			virtual Rectangle* getBounds() = 0;
			virtual moduru::gui::geom::Rectangle2D* getBounds2D() = 0;
			virtual moduru::gui::geom::PathIterator* getPathIterator(moduru::gui::geom::AffineTransform* at) = 0;
			virtual moduru::gui::geom::PathIterator* getPathIterator(moduru::gui::geom::AffineTransform* at, double flatness) = 0;
			virtual bool intersects(moduru::gui::geom::Rectangle2D* r) = 0;
			virtual bool intersects(double x, double y, double w, double h) = 0;

			virtual Shape* clone() { return nullptr; };

		};

	}
}
