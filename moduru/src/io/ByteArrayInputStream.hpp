#pragma once

#include <io/InputStream.hpp>

namespace moduru {
	namespace io {

		class ByteArrayInputStream
			: public InputStream
		{

		private:
			std::vector<char>* buf{ nullptr };
			int count{ 0 };
			int mark_{ -1 };
			int pos{ 0 };

		public:
			int available() override;
			void close() override;
			void mark(int readAheadLimit) override;
			bool markSupported() override;
			char read() override;
			int read(std::vector<char>* b, int off, int len) override;
			void reset() override;
			long skip(long n) override;

			ByteArrayInputStream(std::vector<char>* buf);
			ByteArrayInputStream(std::vector<char>* buf, int offset, int length);

		};

	}
}
