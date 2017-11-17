#pragma once
#include <io/OutputStream.hpp>

namespace moduru {
	namespace io {

		class CachedOutputStream
			: public OutputStream
		{

		private:
			std::vector<char> bytes{};

		public:
			void write(char b) override;
			std::vector<char> get();

			CachedOutputStream();

		};

	}
}
