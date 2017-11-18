#ifndef COMPONENT
#define COMPONENT

//#include <gui/BasicStructs.hpp>

#include <vector>

namespace mpc {
	namespace lcdgui {
		class Component {

		private:
			//IRECT rect;

		public:
			void Hide(bool hide) {}
			bool IsHidden() { return false; }

			virtual void Draw(std::vector<std::vector<bool> >* pixels) {}
			void SetDirty(bool dirty) {}

			//IRECT* GetRECT() { return &rect; }

			virtual ~Component() {}

		};

	}
}


#endif // !COMPONENT
