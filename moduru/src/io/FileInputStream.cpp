#include <io/FileInputStream.hpp>

using namespace moduru::io;
using namespace std;

FileInputStream::FileInputStream(string name) : FileInputStream(make_shared<moduru::file::File>(name, nullptr))
{

}

FileInputStream::FileInputStream(weak_ptr<moduru::file::File> file)
{
	this->file = file;
	file.lock()->openRead();
}

/*
FileInputStream::FileInputStream(FileDescriptor* fdObj)
{
}
*/

void FileInputStream::close()
{
	file.lock()->close();
}

void FileInputStream::finalize()
{
	close(); // tmp hack
}

/*
java::nio::channels::FileChannel* FileInputStream::getChannel()
{ 
	return channel;
}

FileDescriptor* FileInputStream::getFD()
{ 
	unimplemented_(u"FileDescriptor* FileInputStream::getFD()");
	return 0;
}
*/

char FileInputStream::read()
{ 
	return file.lock()->readByte();
}

int FileInputStream::read(vector<char>* b)
{ 
	return readBytes(b, 0, b->size());
}

int FileInputStream::readBytes(vector<char>* b, int off, int len) {
	auto lFile = file.lock();
	int length = lFile->getLength();
	int pos = lFile->getPosition();
	int toWrite = len;
	int written = 0;
	//if (lFile->getPosition() + 1 >= length) return -1;
	for (int i = 0; i < toWrite; i++) {
		(*b)[i + off] = lFile->readByte();
		//if (lFile->getPosition() < length) written++;
		written++;
	}
	return written;
}

int FileInputStream::read(vector<char>* b, int off, int len)
{
	return readBytes(b, off, len);
}

int FileInputStream::available() {
	auto lFile = file.lock();
	return lFile->getLength() - lFile->getPosition();
}
