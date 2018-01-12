#include <io/InputStream.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::io;
using namespace std;

int InputStream::read(vector<char>* b) {
	return read(b, 0, b->size());
}

int InputStream::read(vector<char>* b, int off, int len) {
	if (off < 0 || len < 0 || len >(int) (b->size()) - off) {
		throw std::invalid_argument("InputStream::read index out of bounds!\n");
	}
	else if (len == 0) {
		return 0;
	}

	int c = read();
	if (c == -1) {
		return -1;
	}
	(*b)[off] = c;

	int i = 1;
	for (; i < len; i++) {
		c = read();
		if (c == -1) {
			break;
		}
		(*b)[off + i] = c;
	}
	return i;
}

long InputStream::skip(long n) {
	long remaining = n;
	int nr;
	if (skipBuffer.size() != SKIP_BUFFER_SIZE)
		skipBuffer = vector<char>(SKIP_BUFFER_SIZE);

	vector<char> localSkipBuffer = skipBuffer;

	if (n <= 0) {
		return 0;
	}

	while (remaining > 0) {
		int len = SKIP_BUFFER_SIZE < remaining ? SKIP_BUFFER_SIZE : remaining;
		nr = read(&localSkipBuffer, 0, len);
		if (nr < 0) {
			break;
		}
		remaining -= nr;
	}
	return n - remaining;
}

int InputStream::available() {
	return 0;
}

//synchronized
void InputStream::reset() {
	throw std::invalid_argument("mark/reset not supported");
}

bool InputStream::markSupported() {
	return false;
}
