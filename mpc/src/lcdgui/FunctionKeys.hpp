#pragma once

//#include <IControl.h>

#include <rapidjson/document.h>

#include <string>
#include <vector>

namespace mpc {
	namespace lcdgui {

		class FunctionKeys
//			: public IPanelControl	
		{

		public:
			std::string name0{ "" };
			std::string name1{ "" };
			std::string name2{ "" };
			std::string name3{ "" };
			std::string name4{ "" };
			std::string name5{ "" };
			int box0{ 0 }, box1{ 0 }, box2{ 0 }, box3{ 0 }, box4{ 0 }, box5{ 0 };
			std::vector<bool> enabled;
			std::vector<std::string> names{};

		public:
			void initialize(rapidjson::Value& fbLabels, rapidjson::Value& fbTypes);
			void disable(int i);
			void enable(int i);

		public:
			//bool Draw(IGraphics* g) override;

		public:
			FunctionKeys();
			~FunctionKeys();

		};

	}
}
