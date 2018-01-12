#include <io/BufferedInputStream.hpp>
#include <VecUtil.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::io;
using namespace std;

BufferedInputStream::BufferedInputStream(InputStream* in) : BufferedInputStream(in, DEFAULT_BUFFER_SIZE)
{
}

BufferedInputStream::BufferedInputStream(InputStream* in, int size) : FilterInputStream(in)
{
	buf = vector<char>(size);
}

const int BufferedInputStream::DEFAULT_BUFFER_SIZE;
const int BufferedInputStream::MAX_BUFFER_SIZE;

/*
java::util::concurrent::atomic::AtomicReferenceFieldUpdater*& BufferedInputStream::bufUpdater()
{

    return bufUpdater_;
}
java::util::concurrent::atomic::AtomicReferenceFieldUpdater* BufferedInputStream::bufUpdater_;
*/

int BufferedInputStream::available()
{
	return getInIfOpen()->available() + (count - pos);
}

void BufferedInputStream::close()
{
	buf.clear();
	in->close();
}

void BufferedInputStream::mark(int readlimit)
{
	marklimit = readlimit;
	markpos = pos;
}

bool BufferedInputStream::markSupported()
{
    return true;
}

char BufferedInputStream::read()
{
	if (pos >= count) {
		fill();
		if (pos >= count)
			return -1;
	}
	return (*getBufIfOpen())[pos++] & 0xff;
}

int BufferedInputStream::read(vector<char>* b, int off, int len)
{
	if ((off | len | (off + len) | ((int)(b->size()) - (off + len))) < 0) {
		return -1;
	}
	else if (len == 0) {
		return 0;
	}

	int n = 0;
	for (;;) {
		int nread = read1(b, off + n, len - n);
		if (nread <= 0) {
			return (n == 0) ? nread : n;
		}
		n += nread;
		if (n >= len) {
			return n;
		}
		// if not closed but no bytes available, return
		InputStream* input = in;
		if (input != nullptr && input->available() <= 0) {
			return n;
		}
	}
}

vector<char>* BufferedInputStream::getBufIfOpen() {
	if (buf.size() == 0)
		throw std::invalid_argument("Stream closed, buffer size 0.\n");
	return &buf;
}

InputStream* BufferedInputStream::getInIfOpen() {
	InputStream* input = in;
	if (input == nullptr)
		throw std::invalid_argument("Stream closed, in == nullptr.\n");
	return input;
}

int BufferedInputStream::read1(vector<char>* b, int off, int len) {
	int avail = count - pos;
	if (avail <= 0) {
		if (len >= getBufIfOpen()->size() && markpos < 0) {
			return getInIfOpen()->read(b, off, len);
		}
		fill();
		avail = count - pos;
		if (avail <= 0) return -1;
	}
	int cnt = (avail < len) ? avail : len;
	moduru::VecUtil::VecCopy(getBufIfOpen(), pos, b, off, cnt);
	pos += cnt;
	return cnt;
}

void BufferedInputStream::fill() {
	auto buffer = getBufIfOpen();
	if (markpos < 0) { /* no mark: throw away the buffer */
		pos = 0;
	}
	else if (pos >= buffer->size()) { /* no room left in buffer */
		if (markpos > 0) {  /* can throw away early part of the buffer */
			int sz = pos - markpos;
			moduru::VecUtil::VecCopy(buffer, markpos, buffer, 0, sz);
			pos = sz;
			markpos = 0;
		}
		else if (buffer->size() >= marklimit) {
			markpos = -1;   /* buffer got too big, invalidate mark */
			pos = 0;        /* drop buffer contents */
		}
		else {            /* grow buffer */
			int nsz = pos * 2;
			if (nsz > marklimit)
				nsz = marklimit;
			auto nbuf = new vector<char>(nsz);
			moduru::VecUtil::VecCopy(buffer, 0, nbuf, 0, pos);
			//if (!bufUpdater.compareAndSet(this, buffer, nbuf)) {
				// Can't replace buf if there was an async close.
				// Note: This would need to be changed if fill()
				// is ever made accessible to multiple threads.
				// But for now, the only way CAS can fail is via close.
				// assert buf == null;
				//throw new IOException("Stream closed");
			//}
			//buffer->clear();
			buffer = nbuf;
		}
	}
	count = pos;
	int n = getInIfOpen()->read(buffer, pos, (int) (buffer->size()) - pos);
	if (n > 0) {
		count = n + pos;
	}
}

void BufferedInputStream::reset()
{
	getBufIfOpen();
	if (markpos < 0)
		throw std::invalid_argument("Resetting to invalid mark.\n");
	pos = markpos;
}

long BufferedInputStream::skip(long n)
{
	getBufIfOpen(); // Check for closed stream
	if (n <= 0) {
		return 0;
	}
	long avail = count - pos;

	if (avail <= 0) {
		// If no mark position set then don't keep in buffer
		if (markpos <0)
			return getInIfOpen()->skip(n);

		// Fill in buffer to save bytes for reset
		fill();
		avail = count - pos;
		if (avail <= 0)
			return 0;
	}

	long skipped = (avail < n) ? avail : n;
	pos += skipped;
	return skipped;
}

int BufferedInputStream::read(vector<char>* b)
{
	return super::read(b);
}

BufferedInputStream::~BufferedInputStream() {
}
