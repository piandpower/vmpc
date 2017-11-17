#pragma once
#include <io/InputStream.hpp>

#include <file/File.hpp>

#include <memory>
#include <vector>
#include <string>

namespace moduru {
	namespace io {

		class FileInputStream
			: public InputStream
		{

		private:
			std::weak_ptr<moduru::file::File> file{};
			/*
			::java::nio::channels::FileChannel* channel{};
			moduru::lang::Object* closeLock{};
			std::atomic< bool > closed{};
			FileDescriptor* fd{};
			::String* path{};
			*/

		public:
			int available() override;
			void close() override;
			/*void close0(); (private) */

		public: /* protected */
			void finalize();

		private:
			int readBytes(std::vector<char>* b, int off, int len);

		public:
			//virtual ::java::nio::channels::FileChannel* getChannel();
			//FileDescriptor* getFD();
			/*static void initIDs(); (private) */
			/*void open(::String* name); (private) */
			/*void open0(::String* name); (private) */
			char read() override;
			int read(std::vector<char>* b) override;
			int read(std::vector<char>* b, int off, int len) override;
			/*int read0(); (private) */

			FileInputStream(std::string name);
			FileInputStream(std::weak_ptr<moduru::file::File> file);
			//	FileInputStream(FileDescriptor* fdObj);

		};
	}
}
