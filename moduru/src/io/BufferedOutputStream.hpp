#pragma once
#include <io/FilterOutputStream.hpp>

namespace moduru {
	namespace io {

		class BufferedOutputStream
			: public FilterOutputStream
		{

		private:
			std::vector<char> buf{};
			int count{};

		public:
			void flush() override;
			void flushBuffer();
			void write(char b) override;
			void write(std::vector<char> bytes) override;
			void write(std::vector<char> b, int off, int len) override;
			
			BufferedOutputStream(OutputStream* out);
			BufferedOutputStream(OutputStream* out, int size);

		};

	}
}
