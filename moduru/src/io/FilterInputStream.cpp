#include <io/FilterInputStream.hpp>

using namespace moduru::io;
using namespace std;

FilterInputStream::FilterInputStream(InputStream* in)
{
	this->in = in;
}

int FilterInputStream::available()
{ 
    return in->available();
}

void FilterInputStream::close()
{ 
	in->close();
}

void FilterInputStream::mark(int readlimit)
{ 
	in->mark(readlimit);
}

bool FilterInputStream::markSupported()
{ 
    return in->markSupported();
}

char FilterInputStream::read()
{ 
    return in->read();
}

int FilterInputStream::read(vector<char>* b)
{ 
    return read(b, 0, b->size());
}

int FilterInputStream::read(vector<char>* b, int off, int len)
{
	return in->read(b, off, len);
}

void FilterInputStream::reset()
{
	in->reset();
}

long FilterInputStream::skip(long n)
{ 
	return in->skip(n);
}
