#pragma once
#include <cstdint>
#include <vector>
#include <thirdp/wrpkey/key.hxx>

namespace mpc {
	namespace controls {

		class KbMapping
		{

		private:
			const static WonderRabbitProject::key::key_helper_t* kh;

		public:
			KbMapping();

		public:
			static int left();
			static int right();
			static int up();
			static int down();

			static int dataWheelBack();
			static int dataWheelForward();
			static std::vector<int> bankKeys();
			static std::vector<int> padKeys();
			static int rec();
			static int overdub();
			static int stop();
			static int play();
			static int playstart();
			static int mainscreen();
			static int prevStepEvent();
			static int nextStepEvent();
			static int goTo();
			static int prevBarStart();
			static int nextBarEnd();
			static int tap();
			static int nextSeq();
			static int trackMute();
			static int openWindow();
			static int fullLevel();
			static int sixteenLevels();
			static int f1();
			static int f2();
			static int f3();
			static int f4();
			static int f5();
			static int f6();
			static std::vector<int> numPad();
			static std::vector<int> altNumPad();
			static int numPadShift();
			static int numPadEnter();
			static int undoSeq();
			static int erase();
			static int after();

			static int ctrl();
			static int alt();
			static int leftShift();
			static int rightShift();

		};

	}
}
