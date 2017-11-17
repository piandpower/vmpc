#pragma once

#include <vector>

namespace moduru {
	namespace io {

		class InputStream
		{

		private:
			static const int SKIP_BUFFER_SIZE{ 2048 };
			std::vector<char> skipBuffer;

		public:
			virtual int available();
			virtual void close() = 0;
			virtual void mark(int readlimit) {};
			virtual bool markSupported();
			virtual char read() = 0;
			virtual int read(std::vector<char>* b);
			virtual int read(std::vector<char>* b, int off, int len);
			virtual void reset();
			virtual long skip(long n);

		};

	}
}
