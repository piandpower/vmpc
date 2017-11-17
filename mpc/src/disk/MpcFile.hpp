#pragma once

#include <boost/any.hpp>

#include <vector>
#include <string>
#include <memory>

namespace moduru {
	namespace file {
		class FsNode;
		class File;
	}
}

namespace mpc {
	namespace disk {

		class MpcFile
		{

		private:
			bool raw{ false };
			bool std{ true };
			//::de::waldheinz::fs::fat::AkaiFatLfnDirectoryEntry* rawEntry{};
			std::shared_ptr<moduru::file::FsNode> stdEntry{};

		public:
			bool isStd();
			bool isDirectory();
			std::string getName();
			bool setName(std::string s);
			int length();
			void setFileData(std::vector<char>* data);
			bool del();
			std::weak_ptr<moduru::file::FsNode> getFsNode();
			std::weak_ptr<moduru::file::File> getFile();
			//::de::waldheinz::fs::fat::AkaiFatLfnDirectoryEntry* getEntry();
			std::vector<char> getBytes();

			MpcFile(boost::any a);
			~MpcFile();

		};

	}
}
