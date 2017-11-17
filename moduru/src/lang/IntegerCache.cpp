#include <lang/IntegerCache.hpp>

#include <lang/Integer.hpp>
#include <lang/Number.hpp>

using namespace moduru::lang;
using namespace std;

IntegerCache::IntegerCache() 
{
}

vector<Integer*> IntegerCache::cache() {
	cache_ = vector<Integer*>(-(-int32_t(128)) + int32_t(127) + int32_t(1));
	{
		for (auto i = int32_t(0); i < cache_.size(); i++)
			cache_[i] = new Integer(i - int32_t(128));

	}
	return cache_;
}

vector<Integer*> IntegerCache::cache_;
