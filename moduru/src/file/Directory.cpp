#include <file/Directory.hpp>

#include <file/File.hpp>
#include <file/FileUtil.hpp>

#ifdef _WIN32
#include <thirdp/dirent.h>
#include <direct.h>
#else
//OSX
#include <stdio.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#endif

#include <lang/StrUtil.hpp>

using namespace moduru::file;

Directory::Directory(std::string const path, Directory* const parent)
	: FsNode(path, parent)
{
}

std::vector<std::shared_ptr<FsNode>> Directory::listFiles() {
	if (!isDirectory()) return std::vector<std::shared_ptr<FsNode>>(0);
    DIR* dir = opendir(getPath().c_str());
	if (dir == NULL) return std::vector<std::shared_ptr<FsNode>>(0);

	std::vector<std::shared_ptr<FsNode>> res;

	struct dirent *ent;
    
	while ((ent = readdir(dir)) != NULL) {
		switch (ent->d_type) {
		case DT_REG:
			//printf("%s\n", ent->d_name);
			break;

		case DT_DIR:
			//printf("%s/\n", ent->d_name);
			break;

		case DT_LNK:
			//printf("%s@\n", ent->d_name);
			break;

			//		default:
			//printf("%s*\n", ent->d_name);
		}
        if (ent->d_type != DT_DIR && ent->d_type != DT_REG) continue;
        std::string name(ent->d_name);
        if (moduru::lang::StrUtil::hasEnding(name, ".DS_")) continue;
        if (name.compare(".DS_Store") == 0) continue;
        if (name.compare(".") == 0 || name.compare("..") == 0) continue;
        
        std::string childPath = std::string(getPath() + FileUtil::getSeparator() + name);
        bool childIsDir = FileUtil::IsDirectory(childPath);
        std::shared_ptr<FsNode> child;
        if (childIsDir) {
            child = std::make_shared<Directory>(childPath, this);
        }
        else {
            child = std::make_shared<File>(childPath, this);
        }
        res.push_back(std::move(child));
    }
	closedir(dir);
	return res;
}

bool Directory::create() {
#ifdef _WIN32
	return _mkdir(getPath().c_str()) == 0; //Windows
#else
	return mkdir(getPath().c_str(), S_IRWXU) == 0; //OSX
#endif
}

bool Directory::isFile() {
	return false;
}

bool Directory::isDirectory() {
	return true;
}

bool Directory::del() {
#ifdef _WIN32
	return RemoveDirectory(getPath().c_str()) != 0;
#else
	return FsNode::del();
#endif
}

Directory::~Directory() {

}
