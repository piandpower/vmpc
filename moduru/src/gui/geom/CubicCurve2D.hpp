#pragma once
#include <gui/Shape.hpp>

#include <vector>

namespace moduru {
	namespace gui {

		class Rectangle;

		namespace geom {

			class CubicCurve2D
				: public virtual moduru::gui::Shape
			{

			public:
				virtual double getX1() = 0;
				virtual double getY1() = 0;
				virtual Point2D* getP1() = 0;
				virtual double getCtrlX1() = 0;
				virtual double getCtrlY1() = 0;
				virtual Point2D* getCtrlP1() = 0;
				virtual double getCtrlX2() = 0;
				virtual double getCtrlY2() = 0;
				virtual Point2D* getCtrlP2() = 0;
				virtual double getX2() = 0;
				virtual double getY2() = 0;
				virtual Point2D* getP2() = 0;
				virtual void setCurve(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2) = 0;
				virtual void setCurve(std::vector<double>* coords, int offset);
				virtual void setCurve(Point2D* p1, Point2D* cp1, Point2D* cp2, Point2D* p2);
				virtual void setCurve(std::vector<Point2D*>* pts, int offset);
				virtual void setCurve(CubicCurve2D* c);
				static double getFlatnessSq(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2);
				static double getFlatness(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2);
				static double getFlatnessSq(std::vector<double>* coords, int offset);
				static double getFlatness(std::vector<double>* coords, int offset);
				virtual double getFlatnessSq();
				virtual double getFlatness();
				virtual void subdivide(CubicCurve2D* left, CubicCurve2D* right);
				static void subdivide(CubicCurve2D* src, CubicCurve2D* left, CubicCurve2D* right);
				static void subdivide(std::vector<double>* src, int srcoff, std::vector<double>* left, int leftoff, std::vector<double>* right, int rightoff);
				static int solveCubic(std::vector<double>* eqn);
				static int solveCubic(std::vector<double>* eqn, std::vector<double>* res);

			private:
				static void fixRoots(std::vector<double>* res, std::vector<double>* eqn);
				static double solveEqn(std::vector<double>* eqn, int order, double t);
				static double findZero(double t, double target, std::vector<double>* eqn);

			public:
				bool contains(double x, double y) override;
				bool contains(Point2D* p) override;

			private:
				static void fillEqn(std::vector<double>* eqn, double val, double c1, double cp1, double cp2, double c2);
				static int evalCubic(std::vector<double>* vals, int num, bool include0, bool include1, std::vector<double>* inflect, double c1, double cp1, double cp2, double c2);
				static constexpr int BELOW{ -2 };
				static constexpr int LOWEDGE{ -1 };
				static constexpr int INSIDE{ 0 };
				static constexpr int HIGHEDGE{ 1 };
				static constexpr int ABOVE{ 2 };
				static int getTag(double coord, double low, double high);
				static bool inwards(int pttag, int opt1tag, int opt2tag);

			public:
				bool intersects(double x, double y, double w, double h) override;
				bool intersects(Rectangle2D* r) override;
				bool contains(double x, double y, double w, double h) override;
				bool contains(Rectangle2D* r) override;
				moduru::gui::Rectangle* getBounds() override;
				PathIterator* getPathIterator(AffineTransform* at) override;
				PathIterator* getPathIterator(AffineTransform* at, double flatness) override;
				//moduru::lang::Object* clone() override;

			public:
				CubicCurve2D();

			private:
				//friend class CubicCurve2D_Float;
				//friend class CubicCurve2D_Double;
			};

		}
	}
}
