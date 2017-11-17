#include <lang/Number.hpp>

using namespace moduru::lang;
using namespace std;

Number::Number()
{
}

int8_t Number::byteValue()
{
    return static_cast< int8_t >(intValue());
}

int16_t Number::shortValue()
{
    return static_cast< int16_t >(intValue());
}
