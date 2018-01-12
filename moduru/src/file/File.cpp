#define _CRT_SECURE_NO_WARNINGS
#include <file/File.hpp>

#include <fstream>

#if defined (__APPLE__) || defined(__linux__)
#include <sys/stat.h>
#endif


using namespace moduru::file;
using namespace std;

File::File(string const path, Directory* const parent)
	: FsNode(path, parent)
{
}

int File::getLength() {
	struct stat stat_buf;
	int rc = stat(getPath().c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

bool File::setData(vector<char>* src) {
	int counter = 0;
	//if (handle) {
		int size = (int) src->size();
		ofstream os;
		os.open(getPath().c_str(), ios::binary);
		os.write(reinterpret_cast<char*>(&(*src)[0]), size);
		os.close();
		return true;
	//}
	//else {
//		return false;
//	}
}

bool File::create() {
	auto fp = fopen(getPath().c_str(), "w");
	auto res = fp != nullptr;
	if (res) fclose(fp);
	return res;
}

bool File::isFile() {
	return true;
}

bool File::isDirectory() {
	return false;
}

bool File::close() {
	if (handle) {
		//handle.reset();
	}
	return true;
}

int File::getPosition() {
	if (!handle) return 0;
	return handle->getPosition();
}

void File::seek(int pos) {
	if (!handle) return;
	handle->seek(pos);
}

void File::openRead() {
	if (handle) {
		handle->seek(0);
		return;
	}
	handle = make_unique<FileHandle>(getPath(), true);
}

void File::openWrite() {
	if (handle && !handle->isReadOnly()) return;
	handle = make_unique<FileHandle>(getPath(), false);
}

char File::readByte() {
	return handle->readByte();
}

void File::writeByte(char& c) {
	handle->writeByte(c);
}

long File::readLong() {
	return handle->readLong();
}

short File::readShort() {
	return handle->readShort();
}

void File::writeShort(short& s) {
	handle->writeShort(s);
}

bool File::getData(vector<char>* dest) {
	openRead();
	if (handle) {
		handle->seek(0);
		int size = getLength();
		if (dest->size() != size) dest->resize(size);
		ifstream is;
		is.open(getPath().c_str(), ios::binary);
		is.seekg(0);
		is.read(reinterpret_cast<char*>(&(*dest)[0]), size);
		is.close();
		return true;
	}
	else {
		return false;
	}
}

File::~File() {
}
