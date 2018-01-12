#include <io/FilterOutputStream.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::io;
using namespace std;

FilterOutputStream::FilterOutputStream(OutputStream* out)
{
	this->out = out;
}

void FilterOutputStream::close()
{
	flush();
	out->close();
}

void FilterOutputStream::flush()
{
}

void FilterOutputStream::write(char b)
{
	out->write(b);
}

void FilterOutputStream::write(vector<char> b)
{
	write(b, 0, b.size());
}

void FilterOutputStream::write(vector<char> b, int off, int len)
{
	if ((off | len | (b.size() - (len + off)) | (off + len)) < 0)
		throw std::invalid_argument("Index out of bounds!");

	for (int i = 0; i < len; i++) {
		write(b[off + i]);
	}
}
