#pragma once
#include <controls/other/AbstractOtherControls.hpp>

namespace mpc {
	namespace controls {
		namespace other {
			namespace dialog {

				class NameControls
					: public mpc::controls::other::AbstractOtherControls
				{

				public:
					void left() override;
					void right() override;
					void turnWheel(int j) override;
					void function(int i) override;
					//void keyEvent(unsigned char c) override;

				private:
					void drawUnderline();
					void initEditColors();
					void resetNameGui();

				public:
					NameControls(mpc::Mpc* mpc) ;

				private:
//					friend class NameControls_keyEvent_1;
//					friend class NameControls_keyEvent_2;
//					friend class NameControls_drawUnderline_3;
				};

			}
		}
	}
}
