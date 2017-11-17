#pragma once
#include <gui/geom/PathIterator.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class QuadCurve2D;
			class AffineTransform;

			class QuadIterator
				: public virtual PathIterator
			{

			public:
				QuadCurve2D* quad{ nullptr };
				AffineTransform* affine{ nullptr };
				int index{ 0 };

			public:
				int getWindingRule() override;
				bool isDone() override;
				void next() override;
				int currentSegment(std::vector<float>* coords) override;
				int currentSegment(std::vector<double>* coords) override;

			public:
				QuadIterator(QuadCurve2D* q, AffineTransform* at);
				~QuadIterator();

			};

		}
	}
}
