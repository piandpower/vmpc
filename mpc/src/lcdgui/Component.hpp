#ifndef COMPONENT
#define COMPONENT

#include <gui/BasicStructs.hpp>

#include <vector>

namespace mpc {
	namespace lcdgui {
		class Component {

		protected:
			MRECT rect;
			std::vector<MRECT> clearRects;
			bool hidden{ false };

		protected:
			bool dirty{ false };

		public:
			virtual void Hide(bool b) { if (hidden != b) { hidden = b; SetDirty(); if (!rect.Empty()) clearRects.push_back(rect); } }
			void SetDirty() { dirty = true; }

			bool IsHidden() { return hidden; }
			bool IsDirty() { return dirty; }
			bool NeedsClearing() { return clearRects.size() != 0; }

		public:
			virtual void Draw(std::vector<std::vector<bool> >* pixels) {}
			virtual void Clear(std::vector<std::vector<bool> >* pixels) {
				for (auto& r : clearRects) {
					for (int i = r.L; i < r.R; i++) {
						for (int j = r.T; j < r.B; j++) {
							if (i < 0 || i > 247 || j < 0 || j > 59) continue;
							pixels->at(i).at(j) = false;
						}
					}
				}
				clearRects.clear();
			}

			MRECT* GetRECT() { return &rect; }
		public:
			virtual ~Component() {}

		};

	}
}


#endif // !COMPONENT
