#pragma once

#include <gui/geom/Path2D.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class AffineTransform;

			class Path2D_Double
				: public Path2D
			{

			public:
				typedef Path2D super;

			public:
				std::vector<double> doubleCoords{};

			public:
				std::vector<float> cloneCoordsFloat(AffineTransform* at) override;
				std::vector<double> cloneCoordsDouble(AffineTransform* at) override;
				void append(float x, float y) override;
				void append(double x, double y) override;
				Point2D* getPoint(int coordindex) override;
				void needRoom(bool needMove, int newCoords) override;

			public:
				void moveTo(double x, double y) override;
				void lineTo(double x, double y) override;
				void quadTo(double x1, double y1, double x2, double y2) override;
				void curveTo(double x1, double y1, double x2, double y2, double x3, double y3) override;

			public:
				int pointCrossings(double px, double py) override;
				int rectCrossings(double rxmin, double rymin, double rxmax, double rymax) override;

			public:
				void append(PathIterator* pi, bool connect) override;
				void transform(AffineTransform* at) override;
				Rectangle2D* getBounds2D() override;
				PathIterator* getPathIterator(AffineTransform* at) override;
				Path2D* clone() override;

			private:
				//void writeObject(::java::io::ObjectOutputStream* s);
				//void readObject(::java::io::ObjectInputStream* s);

			public:
				Path2D_Double();
				Path2D_Double(int rule);
				Path2D_Double(int rule, int initialCapacity);
				Path2D_Double(moduru::gui::Shape* s);
				Path2D_Double(moduru::gui::Shape* s, AffineTransform* at);

			public:
				//void append(moduru::gui::Shape* s, bool connect);
				//PathIterator* getPathIterator(AffineTransform* at, double flatness);

			public:
				//void readObject(::java::io::ObjectInputStream* s, bool storedbl);
				//void writeObject(::java::io::ObjectOutputStream* s, bool isdbl);

			private:
				//friend class Path2D;
				//friend class Path2D_Float;
				//friend class Path2D_Float_CopyIterator;
				//friend class Path2D_Float_TxIterator;
				//friend class Path2D_Double_CopyIterator;
				//friend class Path2D_Double_TxIterator;
				//friend class Path2D_Iterator;
			};

		}
	}
}
