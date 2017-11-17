#include <io/CachedOutputStream.hpp>

using namespace moduru::io;
using namespace std;

CachedOutputStream::CachedOutputStream()
{
}

void CachedOutputStream::write(char b)
{
    bytes.push_back(b);
}

vector<char> CachedOutputStream::get()
{
	return bytes;
}
