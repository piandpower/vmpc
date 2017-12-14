#pragma once
#include <string>
#include <cstdint>

namespace ctoot {
	namespace audio {
		namespace core {

			class Encoding
			{

			private:
				static Encoding* ALAW_;
				static Encoding* PCM_SIGNED_;
				static Encoding* PCM_UNSIGNED_;
				static Encoding* ULAW_;
				std::string name = "" ;


			public:
				std::string toString();

				Encoding(std::string name);

			public:
				static Encoding*& ALAW();
				static Encoding*& PCM_SIGNED();
				static Encoding*& PCM_UNSIGNED();
				static Encoding*& ULAW();

			};

		}
	}
}
