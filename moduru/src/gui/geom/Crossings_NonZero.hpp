#pragma once

#include <gui/geom/Crossings.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class Crossings_NonZero final
				: public Crossings
			{

			public:
				typedef Crossings super;

			private:
				std::vector<int> crosscounts{};

			public:
				bool covers(double ystart, double yend) override;
				void remove(int cur);
				void insert(int cur, double lo, double hi, int dir);
				void record(double ystart, double yend, int direction) override;

				Crossings_NonZero(double xlo, double ylo, double xhi, double yhi);

			private:
				//friend class Crossings;
				//friend class Crossings_EvenOdd;
			};

		}
	}
}
