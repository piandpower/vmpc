#pragma once
#include <gui/geom/PathIterator.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class Rectangle2D;
			class AffineTransform;

			class RectIterator
				: public virtual PathIterator
			{

			public:
				double x{}, y{}, w{}, h{};
				AffineTransform* affine{ nullptr };
				int index{};

			public:
				int getWindingRule() override;
				bool isDone() override;
				void next() override;
				int currentSegment(std::vector<float>* coords) override;
				int currentSegment(std::vector<double>* coords) override;

			public:
				RectIterator(Rectangle2D* r, AffineTransform* at);
				~RectIterator();
			};

		}
	}
}
