#pragma once

#include <gui/geom/Point2D.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Point2D_Double
				: public Point2D
			{

			public:
				typedef Point2D super;

			public:
				double x{};
				double y{};

			public:
				double getX() override;
				double getY() override;
				void setLocation(double x, double y) override;

				Point2D_Double();
				Point2D_Double(double x, double y);
			};

		}
	}
}
