#pragma once
#include <control/CompoundControl.hpp>
#include <control/CompoundControlPersistence.hpp>

namespace ctoot {
	namespace control {
		class CompoundControlMidiPersistence
			: public virtual CompoundControlPersistence
		{


		private:
			FILE* root{};

		public:
			//    vector<string*> getPresets(CompoundControl* c) override;
			//    void loadPreset(CompoundControl* c, string* name) override;
			//    void savePreset(CompoundControl* c, string* name) override;

		public:
			virtual std::string* path(control::CompoundControl* c);

		public:
			CompoundControlMidiPersistence(FILE* root);

		};
	}
}
