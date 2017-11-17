 #pragma once

#include <gui/geom/PathIterator.hpp>

#include <vector>

using namespace std;

namespace moduru {
	namespace gui {

		namespace geom {
			class AffineTransform;
		}

		class Polygon;

		class PolygonPathIterator
			: public virtual moduru::gui::geom::PathIterator
		{

		public:
			int index{ 0 };
			Polygon* poly{ nullptr };
			moduru::gui::geom::AffineTransform* transform{ nullptr };

		public:
			int currentSegment(vector<float>* coords) override;
			int currentSegment(vector<double>* coords) override;
			int getWindingRule() override;
			bool isDone() override;
			void next() override;


			PolygonPathIterator(Polygon* polygon, moduru::gui::geom::AffineTransform* at);
			~PolygonPathIterator();

		};
	}
}
