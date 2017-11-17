#pragma once

#include <gui/Shape.hpp>

#include <vector>

namespace moduru {
	namespace gui {

		namespace geom {
			class PathIterator;
			class AffineTransform;
			class Rectangle2D;
			class Crossings;
		}

		class Rectangle;
		class Point;

		class Polygon
			: public virtual Shape
		{

		private:
			static const int MIN_LENGTH{ 4 };

		public:
			Rectangle* bounds{ nullptr };

		public:
			int npoints{};

		public:
			std::vector<int> xpoints{};
			std::vector<int> ypoints{};

		public:
			virtual void addPoint(int x, int y);

		public:
			virtual void calculateBounds(std::vector<int> xpoints, std::vector<int> ypoints, int npoints);

		public:
			virtual bool contains(Point* p);
			bool contains(moduru::gui::geom::Point2D* p) override;
			bool contains(moduru::gui::geom::Rectangle2D* r) override;
			virtual bool contains(int x, int y);
			bool contains(double x, double y) override;
			bool contains(double x, double y, double w, double h) override;
			virtual Rectangle* getBoundingBox();
			Rectangle* getBounds() override;
			moduru::gui::geom::Rectangle2D* getBounds2D() override;
			moduru::gui::geom::PathIterator* getPathIterator(moduru::gui::geom::AffineTransform* at) override;
			moduru::gui::geom::PathIterator* getPathIterator(moduru::gui::geom::AffineTransform* at, double flatness) override;
			virtual bool inside(int x, int y);
			bool intersects(moduru::gui::geom::Rectangle2D* r) override;
			bool intersects(double x, double y, double w, double h) override;
			virtual void invalidate();
			virtual void reset();
			virtual void translate(int deltaX, int deltaY);

		public:
			virtual void updateBounds(int x, int y);


		private:
			moduru::gui::geom::Crossings* getCrossings(double xlo, double ylo, double xhi, double yhi);

		public:
			Polygon();
			Polygon(std::vector<int> xpoints, std::vector<int> ypoints, int npoints);
			~Polygon();

		};

	}
}
