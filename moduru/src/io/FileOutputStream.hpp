#pragma once
#include <atomic>
#include <io/OutputStream.hpp>

#include <file/File.hpp>

#include <fstream>

#include <string>
#include <vector>

namespace moduru {
	namespace io {

		class FileOutputStream
			: public OutputStream
		{

		public:
			typedef OutputStream super;

		private:
			bool append{ false };
			moduru::file::File* file{ nullptr };
			std::ofstream outputStream;
			//::java::nio::channels::FileChannel* channel{};
			//moduru::lang::Object* closeLock{};
			//std::atomic< bool > closed{};
			//FileDescriptor* fd{};
			//static ::ThreadLocal* runningFinalize_;

		private:
			void open(std::string name);

		public:
			void close() override;
			/*void close0(); (private) */

		public:
			void finalize();

		public:
			//virtual ::java::nio::channels::FileChannel* getChannel();
			//FileDescriptor* getFD();
			/*static void initIDs(); (private) */
			/*static bool isRunningFinalize(); (private) */
			/*void openAppend(::String* name); (private) */
			void write(int b) {};
			void write(std::vector<char> b) override;
			void write(std::vector<char> b, int off, int len) override;
			void writeBytes(std::vector<char> b, int off, int len);

			FileOutputStream(std::string name);
			FileOutputStream(moduru::file::File* file);
			//FileOutputStream(FileDescriptor* fdObj);
			FileOutputStream(std::string name, bool append);
			FileOutputStream(moduru::file::File* file, bool append);

			~FileOutputStream();

		};

	}
}
