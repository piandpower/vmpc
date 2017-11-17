#pragma once
#include <io/FilterInputStream.hpp>

#include <vector>

namespace moduru {
	namespace io {

		class BufferedInputStream
			: public FilterInputStream
		{

		public:
			typedef FilterInputStream super;

		private:
			static const int DEFAULT_BUFFER_SIZE = 8192;
			static const int MAX_BUFFER_SIZE = 8192;

		public:
			std::vector<char> buf{};
			std::vector<char>* getBufIfOpen();

		private:
			//static ::java::util::concurrent::atomic::AtomicReferenceFieldUpdater* bufUpdater_;
			int read1(std::vector<char>* b, int off, int len);
			InputStream* getInIfOpen();
			void fill();

		public:
			int count{ 0 };
			int marklimit{ 0 };
			int markpos{ -1 };
			int pos{ 0 };

		public:
			int available() override;
			void close() override;
			void mark(int readlimit) override;
			bool markSupported() override;
			char read() override;
			int read(std::vector<char>* b, int off, int len) override;
			void reset() override;
			long skip(long n) override;

			BufferedInputStream(InputStream* in);
			BufferedInputStream(InputStream* in, int size);
			~BufferedInputStream();

		public:
			int read(std::vector<char>* b);

		private:
			//static ::java::util::concurrent::atomic::AtomicReferenceFieldUpdater*& bufUpdater();
		};

	}
}
