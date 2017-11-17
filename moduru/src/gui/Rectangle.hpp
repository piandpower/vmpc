#pragma once

#include <gui/geom/Rectangle2D.hpp>
#include <gui/Shape.hpp>

namespace moduru {
	namespace gui {

		class Point;
		class Dimension;

		class Rectangle
			: public moduru::gui::geom::Rectangle2D
			, public virtual Shape

		{

		public:
			typedef moduru::gui::geom::Rectangle2D super;
			int height{};

		public:
			int width{};
			int x{};
			int y{};

		public:
			virtual void add(Point* pt);
			virtual void add(Rectangle* r);
			virtual void add(int newx, int newy);
			static int clip(double v, bool doceil);
			virtual bool contains(Point* p);
			virtual bool contains(Rectangle* r);
			virtual bool contains(int x, int y);
			virtual bool contains(int X, int Y, int W, int H);
			moduru::gui::geom::Rectangle2D* createIntersection(moduru::gui::geom::Rectangle2D* r) override;
			moduru::gui::geom::Rectangle2D* createUnion(moduru::gui::geom::Rectangle2D* r) override;
			//bool equals(moduru::lang::Object* obj) override;
			Rectangle* getBounds() override;
			moduru::gui::geom::Rectangle2D* getBounds2D() override;
			double getHeight() override;
			virtual Point* getLocation();
			virtual Dimension* getSize();
			double getWidth() override;
			double getX() override;
			double getY() override;
			virtual void grow(int h, int v);
			/*static void initIDs(); (private) */
			virtual bool inside(int X, int Y);
			virtual Rectangle* intersection(Rectangle* r);
			virtual bool intersects(Rectangle* r);
			bool isEmpty() override;
			virtual void move(int x, int y);
			int outcode(double x, double y) override;
			virtual void reshape(int x, int y, int width, int height);
			virtual void resize(int width, int height);
			virtual void setBounds(Rectangle* r);
			virtual void setBounds(int x, int y, int width, int height);
			virtual void setLocation(Point* p);
			virtual void setLocation(int x, int y);
			void setRect(double x, double y, double width, double height) override;
			virtual void setSize(Dimension* d);
			virtual void setSize(int width, int height);
			//::String* toString() override;
			virtual void translate(int dx, int dy);
			virtual Rectangle* union_(Rectangle* r);
			virtual Shape* clone() override { return nullptr; };
			
			Rectangle();
			Rectangle(Rectangle* r);
			Rectangle(Point* p);
			Rectangle(Dimension* d);
			Rectangle(int width, int height);
			Rectangle(Point* p, Dimension* d);
			Rectangle(int x, int y, int width, int height);

		};

	}
}
