#pragma once

//#include <IControl.h>

#include <memory>
#include <set>
#include <vector>

namespace moduru {
	namespace gui {
		namespace imagemap {
			class Shape;
		}
	}
}

namespace mpc {
	class Mpc;
	namespace ui {
		namespace sampler {
			class SamplerGui;
		}
	}

	namespace controls {

		class GlobalReleaseControls;
		class AbstractControls;

		class KbMouseController
		{
		private:
			int dataWheelIndex = 0;

		public:
			int getDataWheelIndex(){ return dataWheelIndex; }
			void turnDataWheel(int increment);

		private:
			mpc::Mpc* mpc;
			AbstractControls* controls{ nullptr };
			//std::weak_ptr<mpc::maingui::MainFrame> mainFrame{};
			mpc::ui::sampler::SamplerGui* samplerGui{};
			int jump;
			std::unique_ptr<GlobalReleaseControls> releaseControls{};
			bool goToPressed;
			std::set<int> pressedPads;
			bool f6Pressed;
			bool altPressed;
			bool shiftPressed;
			bool recPressed;
			bool overdubPressed;
			bool tapPressed;
			bool erasePressed;
			int lastKnobValue;
			bool ctrlPressed;

		private:
			void init();

		public:
			void press(unsigned char c);
			void release(unsigned char c);
			int getPressedPad(char c);
			void mousePressed(int x, int y);
			void releaseShift();

		private:
			std::weak_ptr<moduru::gui::imagemap::Shape> getShape(int x, int y);

		public:
			AbstractControls* getControls();

		public:
			int getJump();

		public:
			bool isGoToPressed();
			bool isF6Pressed();
			bool isAltPressed();
			bool isShiftPressed();
			bool isRecPressed();
			bool isOverdubPressed();
			bool isTapPressed();
			bool isErasePressed();

			void setGoToPressed(bool b);
			void setF6Pressed(bool b);
			void setAltPressed(bool b);
			void setShiftPressed(bool b);
			void setRecPressed(bool b);
			void setOverdubPressed(bool b);
			void setTapPressed(bool b);
			void setErasePressed(bool b);

		public:
			std::set<int>* getPressedPads();
			std::vector<int>* getPressedPadVelos();

		private:
			std::vector<int> pressedPadVelos = std::vector<int>(16);

		public:
			KbMouseController(mpc::Mpc* mpc);
			~KbMouseController();

		};

	}
}
