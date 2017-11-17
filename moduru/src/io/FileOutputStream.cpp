#include <io/FileOutputStream.hpp>

using namespace moduru::io;
using namespace std;

FileOutputStream::FileOutputStream(string name) 
	: FileOutputStream(new moduru::file::File(name, nullptr)) {
}

FileOutputStream::FileOutputStream(string name, bool append) 
	: FileOutputStream(new moduru::file::File(name, nullptr), append)
{
}

FileOutputStream::FileOutputStream(moduru::file::File* file) 
	: FileOutputStream(file, false) {
}

FileOutputStream::FileOutputStream(moduru::file::File* file, bool append)
{
	this->file = file;
	this->append = append;
	if (append) {
		outputStream = ofstream(file->getPath().c_str(), ios::out | ios::app | ofstream::binary);
	} else {
		outputStream = ofstream(file->getPath().c_str(), ios::out | ofstream::binary);
	}
}

void FileOutputStream::write(vector<char> b) {
	writeBytes(b, 0, b.size());
}

void FileOutputStream::write(vector<char> b, int off, int len) {
	writeBytes(b, off, len);
}

void FileOutputStream::writeBytes(vector<char> b, int off, int len) {
	outputStream.seekp(off);
	outputStream.write(&b[0], len);
}

void FileOutputStream::close() {

	/*
	synchronized(closeLock) {
		if (closed) {
			return;
		}
		closed = true;
	}

	if (channel != null) {
		fd.decrementAndGetUseCount();
		channel.close();
	}

	int useCount = fd.decrementAndGetUseCount();

	if ((useCount <= 0) || !isRunningFinalize()) {
		close0();
	}
	*/
	outputStream.close();
	file->close();
}

/*
final FileDescriptor getFD()  throws IOException {
	if (fd != null) return fd;
	throw new IOException();
}

FileChannel getChannel() {
	synchronized(this) {
		if (channel == null) {
			channel = FileChannelImpl.open(fd, false, true, this, append);

			fd.incrementAndGetUseCount();
		}
		return channel;
	}
}
*/

void FileOutputStream::finalize() {
	/*
	if (fd != null) {
		if (fd == fd.out || fd == fd.err) {
			flush();
		}
		else {

			runningFinalize.set(Boolean.TRUE);
			try {
				close();
			}
			finally {
				runningFinalize.set(Boolean.FALSE);
			}
		}
	}
	*/
}

void FileOutputStream::open(string name) {

}

FileOutputStream::~FileOutputStream() {
/*
	if (file != nullptr) {
		delete file;
		file = nullptr;
	}
*/
}
