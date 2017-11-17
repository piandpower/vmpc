#pragma once

namespace moduru {
	namespace gui {
		namespace geom {

			class Dimension2D
			{

			public:
				virtual double getWidth() = 0;
				virtual double getHeight() = 0;
				virtual void setSize(double width, double height) = 0;
				virtual void setSize(Dimension2D* d);

			public:
				Dimension2D();
			};

		}
	}
}
