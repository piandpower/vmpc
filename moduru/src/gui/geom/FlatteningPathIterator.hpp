#pragma once

#include <gui/geom/PathIterator.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class FlatteningPathIterator
				: public virtual PathIterator
			{

			public:
				static constexpr int GROW_SIZE{ 24 };
				PathIterator* src{ nullptr };
				double squareflat{};
				int limit{};
				std::vector<double> hold = std::vector<double>(14);
				double curx{}, cury{};
				double movx{}, movy{};
				int holdType{};
				int holdEnd{};
				int holdIndex{};
				std::vector<int> levels{};
				int levelIndex{};
				bool done{};

			public:
				virtual double getFlatness();
				virtual int getRecursionLimit();
				int getWindingRule() override;
				bool isDone() override;

			public:
				virtual void ensureHoldCapacity(int want);

			public:
				void next() override;

			private:
				void next(bool doNext);

			public:
				int currentSegment(std::vector<float>* coords) override;
				int currentSegment(std::vector<double>* coords) override;

				FlatteningPathIterator(PathIterator* src, double flatness);
				FlatteningPathIterator(PathIterator* src, double flatness, int limit);
				~FlatteningPathIterator();

			};

		}
	}
}
