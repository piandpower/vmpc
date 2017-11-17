#pragma once

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class Curve;
			class PathIterator;

			class Crossings
			{

			public:
				int limit{ 0 };
				std::vector<double> yranges{};
				double xlo{}, ylo{}, xhi{}, yhi{};

			public:
				double getXLo();
				double getYLo();
				double getXHi();
				double getYHi();
				virtual void record(double ystart, double yend, int direction) = 0;
				bool isEmpty();
				virtual bool covers(double ystart, double yend) = 0;
				static Crossings* findCrossings(std::vector<Curve*> curves, double xlo, double ylo, double xhi, double yhi);
				static Crossings* findCrossings(PathIterator* pi, double xlo, double ylo, double xhi, double yhi);
				virtual bool accumulateLine(double x0, double y0, double x1, double y1);
				virtual bool accumulateLine(double x0, double y0, double x1, double y1, int direction);

			private:
				std::vector<Curve*> tmp{ nullptr };

			public:
				virtual bool accumulateQuad(double x0, double y0, std::vector<double> coords);
				virtual bool accumulateCubic(double x0, double y0, std::vector<double> coords);

				Crossings(double xlo, double ylo, double xhi, double yhi);
				~Crossings();

			private:
				//friend class Crossings_EvenOdd;
				//friend class Crossings_NonZero;
			};
		}
	}
}
