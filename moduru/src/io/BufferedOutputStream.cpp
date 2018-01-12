#include <io/BufferedOutputStream.hpp>

#include <VecUtil.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::io;
using namespace std;

BufferedOutputStream::BufferedOutputStream(OutputStream* out)
	: BufferedOutputStream(out, 8192)
{
}

BufferedOutputStream::BufferedOutputStream(OutputStream* out, int size)
	: FilterOutputStream(out)
{
	if (size <= 0) {
		throw std::invalid_argument("Buffer size <= 0");
	}
	buf = vector<char>(size);
}

void BufferedOutputStream::flush()
{
	flushBuffer();
	out->flush();
}

void BufferedOutputStream::write(char b)
{
	if (count >= buf.size()) {
		flushBuffer();
	}
	buf[count++] = b;
}

void BufferedOutputStream::write(vector<char> b, int off, int len)
{
	if (len >= buf.size()) {
		flushBuffer();
		out->write(b, off, len);
		return;
	}
	if (len > (int)(buf.size()) - count) {
		flushBuffer();
	}
	VecUtil::VecCopy(&b, off, &buf, count, len);
	count += len;
}

void BufferedOutputStream::flushBuffer() {
	if (count > 0) {
		out->write(buf, 0, count);
		count = 0;
	}
}

void BufferedOutputStream::write(vector<char> bytes) {
	FilterOutputStream::write(bytes);
}
