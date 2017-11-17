#pragma once

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class PathIterator
			{
			
			public:
				static const int SEG_CLOSE{ 4 };
				static const int SEG_CUBICTO{ 3 };
				static const int SEG_LINETO{ 1 };
				static const int SEG_MOVETO{ 0 };
				static const int SEG_QUADTO{ 2 };
				static const int WIND_EVEN_ODD{ 0 };
				static const int WIND_NON_ZERO{ 1 };

			public:
				virtual int currentSegment(std::vector<float>* coords) = 0;
				virtual int currentSegment(std::vector<double>* coords) = 0;
				virtual int getWindingRule() = 0;
				virtual bool isDone() = 0;
				virtual void next() = 0;
			};

		}
	}
}
