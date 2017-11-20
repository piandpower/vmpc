#ifndef COMPONENT
#define COMPONENT

//#include <gui/BasicStructs.hpp>

#include <vector>

namespace mpc {
	namespace lcdgui {
		class Component {

		private:
			//IRECT rect;
			bool hidden{ false };

		protected:
			bool dirty{ false };

		public:
			void Hide(bool b) { if (hidden != b) { hidden = b; SetDirty(); } }
			void SetDirty() { dirty = true; }

			bool IsHidden() { return hidden; }
			bool IsDirty() { return dirty; }

		public:
			virtual void Draw(std::vector<std::vector<bool> >* pixels) {}

			//IRECT* GetRECT() { return &rect; }

			virtual ~Component() {}

		};

	}
}


#endif // !COMPONENT
