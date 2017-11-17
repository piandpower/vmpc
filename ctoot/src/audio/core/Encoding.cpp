#include <audio/core/Encoding.hpp>

using namespace ctoot::audio::core;
using namespace std;

Encoding::Encoding(string name)
{
	this->name = name;
}

Encoding*& Encoding::ALAW()
{
	return ALAW_;
}
Encoding* Encoding::ALAW_ = new Encoding("alaw");

Encoding*& Encoding::PCM_SIGNED()
{
	return PCM_SIGNED_;
}
Encoding* Encoding::PCM_SIGNED_ = new Encoding("pcm_signed");

Encoding*& Encoding::PCM_UNSIGNED()
{
	return PCM_UNSIGNED_;
}
Encoding* Encoding::PCM_UNSIGNED_ = new Encoding("pcm_unsigned");

Encoding*& Encoding::ULAW()
{
	return ULAW_;
}
Encoding* Encoding::ULAW_ = new Encoding("ulaw");

string Encoding::toString()
{
		return name;
}
