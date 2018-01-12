#include <io/ByteArrayInputStream.hpp>
#include <VecUtil.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::io;
using namespace std;

ByteArrayInputStream::ByteArrayInputStream(vector<char>* buf)
{
	this->buf = buf;
	this->pos = 0;
	this->count = buf->size();
}

ByteArrayInputStream::ByteArrayInputStream(vector<char>* buf, int offset, int length)
{
	this->buf = buf;
	this->pos = offset;
	int v1 = offset + length;
	int v2 = buf->size();
	this->count = v1 < v2 ? v1 : v2;
	this->mark_ = offset;
}

int ByteArrayInputStream::available()
{
	return count - pos;
}

void ByteArrayInputStream::close()
{
}

void ByteArrayInputStream::mark(int readAheadLimit)
{
	mark_ = pos;
}

bool ByteArrayInputStream::markSupported()
{
    return true;
}

char ByteArrayInputStream::read()
{
	return (pos < count) ? ((*buf)[pos++] & 0xff) : -1;
}

int ByteArrayInputStream::read(vector<char>* b, int off, int len)
{
	if (b == nullptr) {
		throw std::invalid_argument("ByteArrayInputStream::read(b, off, len) b == null pointer!!\n");
	}
	else if (off < 0 || len < 0 || len > (int) (b->size()) - off) {
		throw std::invalid_argument("Index out of bounds!");
	}
	if (pos >= count) {
		return -1;
	}
	if (pos + len > count) {
		len = count - pos;
	}
	if (len <= 0) {
		return 0;
	}
	moduru::VecUtil::VecCopy(buf, pos, b, off, len);
	pos += len;
	return len;
}

void ByteArrayInputStream::reset()
{
	pos = mark_;
}

long ByteArrayInputStream::skip(long n)
{
	if (pos + n > count) {
		n = count - pos;
	}
	if (n < 0) {
		return 0;
	}
	pos += n;
	return n;
}
