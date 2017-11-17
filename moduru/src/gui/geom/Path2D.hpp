#pragma once

#include <gui/Shape.hpp>
#include <cstdint>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class AffineTransform;
			class Point2D;

			class Path2D
				: public virtual moduru::gui::Shape
			{

			private:
				static bool $assertionsDisabled;

			public:
				static const int EXPAND_MAX{ 500 };
				static const int EXPAND_MAX_COORDS{ 1000 };
				static const int EXPAND_MIN{ 10 };
				static const int INIT_SIZE{ 20 };

			protected:
				static const int8_t SEG_CLOSE{ int8_t(4) };
				static const int8_t SEG_CUBICTO{ int8_t(3) };
				static const int8_t SEG_LINETO{ int8_t(1) };
				static const int8_t SEG_MOVETO{ int8_t(0) };
				static const int8_t SEG_QUADTO{ int8_t(2) };
				static const int8_t SERIAL_PATH_END{ int8_t(97) };
				static const int8_t SERIAL_SEG_CLOSE{ int8_t(96) };
				static const int8_t SERIAL_SEG_DBL_CUBICTO{ int8_t(83) };
				static const int8_t SERIAL_SEG_DBL_LINETO{ int8_t(81) };
				static const int8_t SERIAL_SEG_DBL_MOVETO{ int8_t(80) };
				static const int8_t SERIAL_SEG_DBL_QUADTO{ int8_t(82) };
				static const int8_t SERIAL_SEG_FLT_CUBICTO{ int8_t(67) };
				static const int8_t SERIAL_SEG_FLT_LINETO{ int8_t(65) };
				static const int8_t SERIAL_SEG_FLT_MOVETO{ int8_t(64) };
				static const int8_t SERIAL_SEG_FLT_QUADTO{ int8_t(66) };
				static const int8_t SERIAL_STORAGE_DBL_ARRAY{ int8_t(49) };
				static const int8_t SERIAL_STORAGE_FLT_ARRAY{ int8_t(48) };

			public:
				static const int WIND_EVEN_ODD{ 0 };
				static const int WIND_NON_ZERO{ 1 };

			public: 
				int numCoords{};
				int numTypes{};
				std::vector<int8_t> pointTypes{};
				int windingRule{};

			public: 
				virtual void append(float x, float y) = 0;
				virtual void append(double x, double y) = 0;

			public:
				void append(moduru::gui::Shape* s, bool connect);
				virtual void append(PathIterator* pi, bool connect) = 0;

			public: 
				virtual std::vector<double> cloneCoordsDouble(AffineTransform* at) = 0;
				virtual std::vector<float> cloneCoordsFloat(AffineTransform* at) = 0;

			public:
				void closePath();
				bool contains(Point2D* p) override;
				bool contains(Rectangle2D* r) override;
				static bool contains(PathIterator* pi, Point2D* p);
				bool contains(double x, double y) override;
				static bool contains(PathIterator* pi, Rectangle2D* r);
				static bool contains(PathIterator* pi, double x, double y);
				bool contains(double x, double y, double w, double h) override;
				static bool contains(PathIterator* pi, double x, double y, double w, double h);
				moduru::gui::Shape* createTransformedShape(AffineTransform* at);
				virtual void curveTo(double x1, double y1, double x2, double y2, double x3, double y3) = 0;

			public: 
				static std::vector<int8_t> expandPointTypes(std::vector<int8_t> oldPointTypes, int needed);

			public:
				moduru::gui::Rectangle* getBounds() override;
				Point2D* getCurrentPoint();
				PathIterator* getPathIterator(AffineTransform* at, double flatness) override;

			public: 
				virtual Point2D* getPoint(int coordindex) = 0;

			public:
				int getWindingRule();
				bool intersects(Rectangle2D* r) override;
				static bool intersects(PathIterator* pi, Rectangle2D* r);
				bool intersects(double x, double y, double w, double h) override;
				static bool intersects(PathIterator* pi, double x, double y, double w, double h);
				virtual void lineTo(double x, double y) = 0;
				virtual void moveTo(double x, double y) = 0;

			public: 
				virtual void needRoom(bool needMove, int newCoords) = 0;
				virtual int pointCrossings(double px, double py) = 0;

			public:
				virtual void quadTo(double x1, double y1, double x2, double y2) = 0;

			public: 
				//void readObject(moduru::io::ObjectInputStream* s, bool storedbl);
				virtual int rectCrossings(double rxmin, double rymin, double rxmax, double rymax) = 0;

			public:
				void reset();
				void setWindingRule(int rule);
				virtual void transform(AffineTransform* at) = 0;

			public: 
				//void writeObject(moduru::io::ObjectOutputStream* s, bool isdbl);

				
				Path2D();
				Path2D(int rule, int initialTypes);

			public:
				virtual PathIterator* getPathIterator(AffineTransform* at) = 0;
			};

		}
	}
}
