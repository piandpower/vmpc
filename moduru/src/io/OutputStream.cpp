#include <io/OutputStream.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::io;
using namespace std;

void OutputStream::write(vector<char> b) {
	write(b, 0, b.size());
}

void OutputStream::write(vector<char> b, int off, int len) {
	if ((off < 0) || (off > b.size()) || (len < 0) ||
		((off + len) > b.size()) || ((off + len) < 0)) {
		throw std::invalid_argument("index out of bounds");
	}
	else if (len == 0) {
		return;
	}
	for (int i = 0; i < len; i++) {
		write(b[off + i]);
	}
}
