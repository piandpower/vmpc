#pragma once
#include <file/FsNode.hpp>

#include <vector>
#include <memory>
#include <string>

namespace moduru {
	namespace file {

		class Directory final
			: public FsNode
		{

		public:
			std::vector<std::shared_ptr<FsNode> > listFiles();

		public:
			bool isFile() override;
			bool isDirectory() override;
			bool create() override;
			bool del() override;

		public:
			Directory(std::string const path, Directory* const parent);
			~Directory();

		};

	}
}
