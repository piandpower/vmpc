#pragma once
#include <lcdgui/Label.hpp>

#include <thread>
#include <string>

namespace mpc {
	namespace lcdgui {
		class BlinkLabel
			: public Label
		{

		private:
			int BLINK_INTERVAL{ 500 };
			bool blinkingOn{ false };
			std::thread blinkThread;

		private:
			static void static_blink(void* args);
			void runBlinkThread();

		public:
			void setBlinking(bool flag);
			bool getBlinking();

		public:
			BlinkLabel(std::string text);
			~BlinkLabel();

		};

	}
}
