#pragma once
#include <gui/geom/Point2D.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Point2D_Float
				: public Point2D
			{

			public:
				typedef Point2D super;

			public:
				float x{};
				float y{};

			public:
				double getX() override;
				double getY() override;
				void setLocation(double x, double y) override;
				virtual void setLocation(float x, float y);

				Point2D_Float();
				Point2D_Float(float x, float y);
			};

		}
	}
}
