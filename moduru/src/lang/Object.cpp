#include <lang/Object.hpp>

//#include <lang/Class.hpp>
//#include <lang/IllegalArgumentException.hpp>
#include <lang/Integer.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/String.hpp>
#include <lang/StringBuilder.hpp>

#ifdef __linux__
#include <typeinfo>
#include <stdexcept>
#endif

using namespace moduru::lang;
using namespace std;

Object::Object()
{
}

bool Object::equals(Object* obj)
{
    return (this == obj);
}

String* Object::toString()
{
	string res = "";
	res += string(typeid(this).name());
	res += "@" + Integer::toHexString(hashCode());
	return StringBuilder().append(res.c_str())->toString();
}

void Object::wait(int64_t timeout, int32_t nanos)
{
	if (timeout < 0) {
		throw std::invalid_argument("timeout value is negative");
	}
	if (nanos < 0 || nanos > 999999) {
		throw std::invalid_argument("nanosecond timeout value out of range");
	}
	if (nanos >= 500000 || (nanos != 0 && timeout == 0)) {
		timeout++;
	}
	//wait(timeout);
}

void Object::wait()
{
    //wait(0);
}

void Object::finalize()
{
}


Object::~Object()
{
}
