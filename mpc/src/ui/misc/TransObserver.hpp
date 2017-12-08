#pragma once
#include <observer/Observer.hpp>

#include <memory>

namespace mpc {
	class Mpc;
	namespace lcdgui {
		class Field;
		class Label;
	}

	namespace ui {
		namespace misc {

			class TransGui;

			class TransObserver
				: public moduru::observer::Observer
			{

			private:
				mpc::Mpc* mpc;
				TransGui* transGui{ nullptr };
				std::weak_ptr<mpc::lcdgui::Field> trField{};
				std::weak_ptr<mpc::lcdgui::Label> trackNameLabel{};
				std::weak_ptr<mpc::lcdgui::Field> transposeAmountField{};
				std::weak_ptr<mpc::lcdgui::Field> bar0Field{};
				std::weak_ptr<mpc::lcdgui::Field> bar1Field{};

			private:
				void displayTransposeAmount();
				void displayTr();
				void displayBars();

			public:
				void update(moduru::observer::Observable* o, boost::any a) override;

			public:
				TransObserver(mpc::Mpc* mpc);
				~TransObserver();
			};

		}
	}
}
