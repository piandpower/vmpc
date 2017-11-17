#pragma once
#include <gui/geom/PathIterator.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class CubicCurve2D;
			class AffineTransform;

			class CubicIterator
				: public virtual PathIterator
			{

			public:
				CubicCurve2D* cubic{ nullptr };
				AffineTransform* affine{ nullptr };
				int index{};

			public:
				int getWindingRule() override;
				bool isDone() override;
				void next() override;
				int currentSegment(std::vector<float>* coords) override;
				int currentSegment(std::vector<double>* coords) override;

			public:
				CubicIterator(CubicCurve2D* q, AffineTransform* at);
				~CubicIterator();

			};

		}
	}
}
