#pragma once

#include <io/InputStream.hpp>

#include <vector>

namespace moduru {
	namespace io {

		class FilterInputStream
			: public InputStream
		{

		public:
			typedef InputStream super;

		public:
			InputStream*  in{};

		public:
			virtual int available() override;
			virtual void close() override;
			virtual void mark(int readlimit) override;
			virtual bool markSupported() override;
			virtual char read() override;
			virtual int read(std::vector<char>* b) override;
			virtual int read(std::vector<char>* b, int off, int len) override;
			virtual void reset() override;
			virtual long skip(long n) override;

		public:
			FilterInputStream(InputStream* in);
		};

	}
}
