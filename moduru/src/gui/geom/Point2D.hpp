#pragma once

namespace moduru {
	namespace gui {
		namespace geom {

			class Point2D
			{

			public:
				virtual double distance(Point2D* pt);
				virtual double distance(double px, double py);
				static double distance(double x1, double y1, double x2, double y2);
				virtual double distanceSq(Point2D* pt);
				virtual double distanceSq(double px, double py);
				static double distanceSq(double x1, double y1, double x2, double y2);
				virtual double getX() = 0;
				virtual double getY() = 0;
				virtual void setLocation(Point2D* p);
				virtual void setLocation(double x, double y) = 0;

			public:
				Point2D();
			};

		}
	}
}
