#include <lang/CharacterCache.hpp>

//#include <java/io/Serializable.hpp>
//#include <lang/ArrayStoreException.hpp>
#include <lang/Character.hpp>
#include <lang/Comparable.hpp>
//#include <lang/NullPointerException.hpp>
//#include <SubArray.hpp>
//#include <ObjectArray.hpp>

using namespace moduru::lang;
using namespace std;

CharacterCache::CharacterCache() 
{
}

vector<Character*>*& CharacterCache::cache()
{
	for (auto i = int32_t(0); i < cache_->size(); i++)
		(*cache_)[i] = new Character(static_cast<char16_t>(i));
	return cache_;
}
vector<Character*>* CharacterCache::cache_ = new vector<Character*>(int32_t(127) + int32_t(1));

