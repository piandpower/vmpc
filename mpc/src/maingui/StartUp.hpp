#pragma once

//////#include <maingui/Gui.hpp>
//#include <lcdgui/LayeredScreen.hpp>
#include <ui/UserDefaults.hpp>

#include <thread>
#include <memory>

namespace mpc {
	namespace maingui {

		class StartUp
		{

		private:
			//std::weak_ptr<Gui> gui{};
			//std::weak_ptr<MainFrame> mainFrame{};

			
			std::thread startupThread;

		private:
			std::thread spawn();
			static std::shared_ptr<ui::UserDefaults> userDefaults;

		public:
			static std::string home;
			static std::string resPath;
			static std::string tempPath;
			static std::string storesPath;

		public:
			void startUp();

		public:
			static std::weak_ptr<ui::UserDefaults> getUserDefaults();
			static void initUserDefaults();

			//void runBootSequence(std::weak_ptr<Gui> gui);

		public:
			StartUp();
			~StartUp();

		};

	}
}
