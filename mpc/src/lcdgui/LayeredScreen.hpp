#pragma once
#include <lcdgui/Layer.hpp>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <memory>
#include <vector>
#include <string>

namespace mpc {

	class Mpc;

	namespace lcdgui {
		class SelectedEventBar;
		class Background;
		class EnvGraph;
		class TwoDots;
		class HorizontalBar;
		class Underline;
		class VerticalBar;
		class MixerKnobBackground;
		class MixerFaderBackground;
		class Knob;
		class Popup;
		class Wave;
		class Field;
		class Label;


		class LayeredScreen
			//	: public IPanelControl 
		{

		private:
			std::vector<std::vector<bool> > pixels;

		private:
			static const int LAYER_COUNT{ 4 };

			mpc::Mpc* mpc;

			std::vector<rapidjson::Document> layerJsons = std::vector<rapidjson::Document>(LAYER_COUNT);
			std::vector<std::unique_ptr<Layer>> layers = std::vector<std::unique_ptr<Layer>>(LAYER_COUNT);

			int getCurrentParamIndex();
			std::weak_ptr<mpc::lcdgui::Field> findBelow(std::weak_ptr<mpc::lcdgui::Field> tf);
			std::weak_ptr<mpc::lcdgui::Field> findAbove(std::weak_ptr<mpc::lcdgui::Field> tf);
			//std::unique_ptr<mpc::lcdgui::Popup> popup{};
			//IBitmap gridBmp;
			//std::shared_ptr<IBitmapControl> gridControl{};

		public:
			void attachControls();
			std::string findBelow(std::string tf);
			std::string findAbove(std::string tf);
			void transferFocus(bool backwards);
			Layer& getLayer(int i);
			int getCurrentLayer();
			int openScreen(std::string name); // returns layer number
			std::weak_ptr<Field> lookupField(std::string s);
			std::weak_ptr<Label> lookupLabel(std::string s);

		private:
			std::vector<std::vector<std::string>> lastFocus{};

			int currentLayer{ 0 };
			std::string currentScreenName{ "" };
			std::string previousScreenName{ "" };

		public:
			std::shared_ptr<mpc::lcdgui::EnvGraph> envGraph{};
			std::shared_ptr<mpc::lcdgui::TwoDots> twoDots{};
			std::vector<std::shared_ptr<mpc::lcdgui::HorizontalBar>> horizontalBarsStepEditor{};
			std::vector<std::shared_ptr<mpc::lcdgui::SelectedEventBar>> selectedEventBarsStepEditor{};
			std::vector<std::shared_ptr<mpc::lcdgui::HorizontalBar>> horizontalBarsTempoChangeEditor{};
			std::shared_ptr<mpc::lcdgui::Underline> underline{};
			std::vector<std::shared_ptr<mpc::lcdgui::VerticalBar>> verticalBarsMixer{};
			std::vector<std::shared_ptr<mpc::lcdgui::MixerKnobBackground>> mixerKnobBackgrounds{};
			std::vector<std::shared_ptr<mpc::lcdgui::MixerFaderBackground>> mixerFaderBackgrounds{};
			std::vector<std::shared_ptr<mpc::lcdgui::Knob>> knobs{};
			std::shared_ptr<mpc::lcdgui::Wave> fineWave{};
			std::shared_ptr<mpc::lcdgui::Wave> wave{};
			mpc::lcdgui::Background* currentBackground{ nullptr };

		private:
			int previousLayer{ 0 };
			std::string previousViewModeText{ "" };
			std::string previousFromNoteText{ "" };
			std::string firstField{ "" };


		public:
			void createPopup(std::string text, int textXPos);
			lcdgui::Background* getCurrentBackground();
			void removeCurrentBackground();
			void setCurrentBackground(std::string s);
			void removePopup();
			void setPopupText(std::string text);

		public:
			void returnToLastFocus();
			void redrawEnvGraph(int attack, int decay);
			void setLastFocus(std::string screenName, std::string tfName);
			std::string getLastFocus(std::string screenName);
			void setCurrentScreenName(std::string screenName);
			std::string getCurrentScreenName();
			void setPreviousScreenName(std::string screenName);
			std::string getPreviousScreenName();
			mpc::lcdgui::EnvGraph* getEnvGraph();
			mpc::lcdgui::Popup* getPopup();
			std::string getPreviousFromNoteText();
			void setPreviousFromNoteText(std::string text);
			void setPreviousViewModeText(std::string text);
			std::string getPreviousViewModeText();
			std::vector<std::weak_ptr<mpc::lcdgui::HorizontalBar>> getHorizontalBarsTempoChangeEditor();
			std::vector<std::weak_ptr<mpc::lcdgui::HorizontalBar>>getHorizontalBarsStepEditor();
			std::vector<std::weak_ptr<mpc::lcdgui::VerticalBar>> getVerticalBarsMixer();
			std::vector<std::weak_ptr<mpc::lcdgui::SelectedEventBar>> getSelectedEventBarsStepEditor();
			FunctionKeys* getFunctionKeys();
			std::vector<std::weak_ptr<mpc::lcdgui::Knob>> getKnobs();
			std::vector<std::weak_ptr<mpc::lcdgui::MixerKnobBackground>> getMixerKnobBackgrounds();
			std::vector<std::weak_ptr<mpc::lcdgui::MixerFaderBackground>> getMixerFaderBackgrounds();
			void drawFunctionKeyses(std::string screenName);
			mpc::lcdgui::Underline* getUnderline();
			mpc::lcdgui::TwoDots* getTwoDots();
			mpc::lcdgui::Wave* getFineWave();
			mpc::lcdgui::Wave* getWave();

		public:
			std::vector<std::vector<bool> >* getPixels();

		public:
			LayeredScreen(mpc::Mpc* mpc);
			~LayeredScreen();

		};
	}
}
