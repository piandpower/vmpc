#include <lang/Subset.hpp>

#include <lang/Object.hpp>
#include <lang/String.hpp>

using namespace moduru::lang;
using namespace std;

Subset::Subset(String* name)
{
	if (name == nullptr) {
		throw std::invalid_argument("nullptr name");
	}
	this->name = name;
}

bool Subset::equals(Object* obj)
{
    return (static_cast< Object* >(this) == obj);
}

int32_t Subset::hashCode()
{
    return super::hashCode();
}

String* Subset::toString()
{
    return name;
}
