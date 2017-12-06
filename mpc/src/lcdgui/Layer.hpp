#pragma once
#include "Parameter.hpp"
#include "Component.hpp"
#include "Info.hpp"
#include "Background.hpp"
#include "FunctionKeys.hpp"
//#include <lcdgui/BlinkLabel.hpp>

#include <rapidjson/document.h>

#include <string>
#include <memory>

namespace mpc {

	namespace lcdgui {
		class LayeredScreen;
		class Layer
		{

		private:
			std::string focus{ "" };
			std::vector<std::shared_ptr<mpc::lcdgui::Field>> unusedFields{};
			std::vector<std::shared_ptr<mpc::lcdgui::Field>> usedFields{};
			std::vector<std::shared_ptr<mpc::lcdgui::Label>> unusedLabels{};
			std::vector<std::shared_ptr<mpc::lcdgui::Label>> usedLabels{};
			std::vector<std::unique_ptr<lcdgui::Parameter>> params{};
			std::vector<std::unique_ptr<lcdgui::Info>> infos{};
			mpc::lcdgui::Background* bg{ nullptr };
			FunctionKeys* fk{ nullptr };
			//std::shared_ptr<mpc::lcdgui::BlinkLabel> blinkLabel{};

		private:
			std::weak_ptr<mpc::lcdgui::Field> getUnusedField();
			void unuseField(std::weak_ptr<lcdgui::Field> tf);
			std::weak_ptr<mpc::lcdgui::Label> getUnusedLabel();
			void unuseLabel(std::weak_ptr<lcdgui::Label> label);

		protected:
			std::string openScreen(rapidjson::Value& screenJson, std::string screenName);

		public:
			void clear();
			mpc::lcdgui::Background* getBackground();
			mpc::lcdgui::FunctionKeys* getFunctionKeys();
			std::vector<mpc::lcdgui::Parameter*> getParameters();
			std::vector<mpc::lcdgui::Info*> getInfos();
			std::vector<std::weak_ptr<mpc::lcdgui::Component>> getAllFields();
			std::vector<std::weak_ptr<mpc::lcdgui::Component>> getAllLabels();
			std::vector<std::weak_ptr<mpc::lcdgui::Component>> getComponentsThatNeedClearing();
			std::vector<std::weak_ptr<mpc::lcdgui::Component>> getAllLabelsAndFields();
			void setFocus(std::string textFieldName);
			std::string getFocus();
			
		public:
			Layer(mpc::lcdgui::LayeredScreen* layeredScreen);
			~Layer();

		private:
			friend class LayeredScreen;

		};

	}
}
