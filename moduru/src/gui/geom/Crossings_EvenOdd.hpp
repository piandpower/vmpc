#pragma once
#include <gui/geom/Crossings.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Crossings_EvenOdd final
				: public Crossings
			{

			public:
				typedef Crossings super;

			public:
				bool covers(double ystart, double yend) override;
				void record(double ystart, double yend, int direction) override;

				Crossings_EvenOdd(double xlo, double ylo, double xhi, double yhi);

			private:
				//friend class Crossings;
				//friend class Crossings_NonZero;
			};

		}
	}
}
