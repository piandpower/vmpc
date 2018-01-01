#pragma once

#include <file/aps/ApsMixer.hpp>

#include <vector>
#include <string>

namespace mpc {

	namespace sampler {
		class Program;
		class StereoMixerChannel;
		class IndivFxMixerChannel;
	}

	namespace file {
		namespace aps {

			class ApsNoteParameters;
			class ApsAssignTable;
			class ApsSlider;

			class ApsProgram
			{

			public:
				//chunks.push_back(vector<char>{ 1, 0, 7, 4, 30, 0 }); 
				static const int INDEX_OFFSET{ 0 };
				static const int UNKNOWN_OFFSET{ 1 };
				const std::vector<char> UNKNOWN{ 0, 7, 4, 30, 0 };
				static const int NAME_OFFSET{ 6 };
				static const int NAME_LENGTH{ 16 };
				static const int PADDING0_LENGTH{ 1 };
				static const int SLIDER_OFFSET{ 23 };
				static const int SLIDER_LENGTH{ 10 };
				static const int PADDING1_LENGTH{ 5 };
				static const int NOTE_PARAMETERS_OFFSET{ 38 };
				static const int NOTE_PARAMETERS_LENGTH{ 26 };
				static const int PADDING2_LENGTH{ 7 };
				static const int MIXER_OFFSET{ 1703 };
				//static const int MIXER_END{ 2081 };
				static const int MIXER_END{ 2087 };
				static const int PADDING3_LENGTH{ 3 };
				static const int ASSIGN_TABLE_OFFSET{ 2090 };
				static const int ASSIGN_TABLE_LENGTH{ 64 };

				std::string name{};
				int index{};
				ApsSlider* slider{};
				std::vector<ApsNoteParameters*> noteParameters = std::vector<ApsNoteParameters*>(64);
				ApsMixer* mixer{};
				ApsAssignTable* assignTable{};
				std::vector<char> saveBytes{};

			public:
				ApsNoteParameters* getNoteParameters(int note);
				ApsMixer* getMixer();
				ApsAssignTable* getAssignTable();
				ApsSlider* getSlider();
				std::string getName();

			public:
				std::vector<char> getBytes();
				mpc::sampler::StereoMixerChannel* getStereoMixerChannel(int note);
				mpc::sampler::IndivFxMixerChannel* getIndivFxMixerChannel(int note);

			public:
				ApsProgram(std::vector<char> loadBytes);
				ApsProgram(mpc::sampler::Program* program, int index);
			};

		}
	}
}
