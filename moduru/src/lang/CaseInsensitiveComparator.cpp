#include <lang/CaseInsensitiveComparator.hpp>

#include <lang/Character.hpp>
//#include <lang/Math.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/String.hpp>

#include <math/Math.hpp>

#include <file/File.hpp>

using namespace moduru::file;

using namespace moduru::lang;

CaseInsensitiveComparator::CaseInsensitiveComparator()
{
}

constexpr int64_t CaseInsensitiveComparator::serialVersionUID;

int32_t CaseInsensitiveComparator::compare(Object* o1, Object* o2)
{
	return compare(dynamic_cast< String* >(o1), dynamic_cast< String* >(o2));
}

int32_t CaseInsensitiveComparator::compare(String* s1, String* s2)
{
	auto n1 = s1->length();
	auto n2 = s2->length();
	auto min = moduru::math::Math::min(n1, n2);
	for (auto i = int32_t(0); i < min; i++) {
		auto c1 = s1->charAt(i);
		auto c2 = s2->charAt(i);
		if (c1 != c2) {
			c1 = Character::toUpperCase(c1);
			c2 = Character::toUpperCase(c2);
			if (c1 != c2) {
				c1 = Character::toLowerCase(c1);
				c2 = Character::toLowerCase(c2);
				if (c1 != c2) {
					return c1 - c2;
				}
			}
		}
	}
	return n1 - n2;
}

bool CaseInsensitiveComparator::equals(Object* obj)
{
    return Object::equals(obj);
}
