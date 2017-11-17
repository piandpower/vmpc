#pragma once
#include <gui/geom/PathIterator.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Line2D;
			class AffineTransform;

			class LineIterator
				: public virtual PathIterator
			{

			public:
				Line2D* line{ nullptr };
				AffineTransform* affine{ nullptr };
				int index{};

			public:
				int getWindingRule() override;
				bool isDone() override;
				void next() override;
				int currentSegment(std::vector<float>* coords) override;
				int currentSegment(std::vector<double>* coords) override;

			public: 
				LineIterator(Line2D* l, AffineTransform* at);
				~LineIterator();
			};
		}
	}
}
