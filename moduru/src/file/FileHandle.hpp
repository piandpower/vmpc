#pragma once

#include <fstream>
#include <string>

namespace moduru {
	namespace file {

		class FileHandle final {

		private:
			const bool readOnly;
			const std::string path;
			
			FILE* file;
			bool open{ false };

		private:
			bool openRead();
			bool openWrite();

		public:
			//bool isOpen();
			bool isReadOnly();
			long getPosition();
			void seek(long pos);
			char readByte();
			long readLong();
			short readShort();
			size_t writeByte(char& c);
			size_t writeShort(short& s);

		public:
			FileHandle(const std::string path, bool readOnly);
			~FileHandle();

		private:
			friend class FsFile;

		};

	}
}
