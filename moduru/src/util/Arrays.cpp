#include <util/Arrays.hpp>

//#include <lang/ArrayIndexOutOfBoundsException.hpp>
//#include <lang/Class.hpp>
//#include <lang/ClassCastException.hpp>
#include <lang/Comparable.hpp>
#include <lang/Double.hpp>
#include <lang/Float.hpp>
//#include <lang/IllegalArgumentException.hpp>
#include <lang/Integer.hpp>
//#include <lang/Math.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/Object.hpp>
#include <lang/String.hpp>
#include <lang/StringBuilder.hpp>
//#include <lang/System.hpp>
//#include <lang/reflect/Array_.hpp>
//#include <util/Arrays_ArrayList.hpp>
#include <util/Comparator.hpp>
//#include <util/HashSet.hpp>
//#include <util/Set.hpp>
//#include <Array.hpp>
//#include <ObjectArray.hpp>

#include <math/Math.hpp>

#include <VecUtil.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::util;
using namespace std;

Arrays::Arrays()
{
}

void Arrays::sort(vector<int64_t>* a)
{

    sort1(a, int32_t(0), a->size());
}

void Arrays::sort(vector<int64_t>* a, int32_t fromIndex, int32_t toIndex)
{

	rangeCheck(a->size(), fromIndex, toIndex);
	sort1(a, fromIndex, toIndex - fromIndex);
}

void Arrays::sort(vector<int32_t>* a)
{

    sort1(a, int32_t(0), a->size());
}

void Arrays::sort(vector<int32_t>* a, int32_t fromIndex, int32_t toIndex)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    sort1(a, fromIndex, toIndex - fromIndex);
}

void Arrays::sort(vector<int16_t>* a)
{

    sort1(a, int32_t(0), a->size());
}

void Arrays::sort(vector<int16_t>* a, int32_t fromIndex, int32_t toIndex)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    sort1(a, fromIndex, toIndex - fromIndex);
}

void Arrays::sort(vector<char16_t>* a)
{

    sort1(a, int32_t(0), a->size());
}

void Arrays::sort(vector<char16_t>* a, int32_t fromIndex, int32_t toIndex)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    sort1(a, fromIndex, toIndex - fromIndex);
}

void Arrays::sort(vector<int8_t>* a)
{

    sort1(a, int32_t(0), a->size());
}

void Arrays::sort(vector<int8_t>* a, int32_t fromIndex, int32_t toIndex)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    sort1(a, fromIndex, toIndex - fromIndex);
}

void Arrays::sort(vector<double>* a)
{

    sort2(a, int32_t(0), a->size());
}

void Arrays::sort(vector<double>* a, int32_t fromIndex, int32_t toIndex)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    sort2(a, fromIndex, toIndex);
}

void Arrays::sort(vector<float>* a)
{

    sort2(a, int32_t(0), a->size());
}

void Arrays::sort(vector<float>* a, int32_t fromIndex, int32_t toIndex)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    sort2(a, fromIndex, toIndex);
}

void Arrays::sort2(vector<double>* a, int32_t fromIndex, int32_t toIndex)
{

    auto const NEG_ZERO_BITS = moduru::lang::Double::doubleToLongBits(-0.0);
    auto numNegZeros = int32_t(0);
    int32_t i = fromIndex, n = toIndex;
    while (i < n) {
        if((*a)[i] != (*a)[i]) {
            swap(a, i, --n);
        } else {
            if((*a)[i] == 0 && moduru::lang::Double::doubleToLongBits((*a)[i]) == NEG_ZERO_BITS) {
                (*a)[i] = 0.0;
                numNegZeros++;
            }
            i++;
        }
    }
    sort1(a, fromIndex, n - fromIndex);
    if(numNegZeros != 0) {
        auto j = binarySearch0(a, fromIndex, n, 0.0);
        do {
            j--;
        } while (j >= fromIndex && (*a)[j] == 0.0);
        for (auto k = int32_t(0); k < numNegZeros; k++)
                        (*a)[++j] = -0.0;

    }
}

void Arrays::sort2(vector<float>* a, int32_t fromIndex, int32_t toIndex)
{

    auto const NEG_ZERO_BITS = moduru::lang::Float::floatToIntBits(-0.0f);
    auto numNegZeros = int32_t(0);
    int32_t i = fromIndex, n = toIndex;
    while (i < n) {
        if((*a)[i] != (*a)[i]) {
            swap(a, i, --n);
        } else {
            if((*a)[i] == 0 && moduru::lang::Float::floatToIntBits((*a)[i]) == NEG_ZERO_BITS) {
                (*a)[i] = 0.0f;
                numNegZeros++;
            }
            i++;
        }
    }
    sort1(a, fromIndex, n - fromIndex);
    if(numNegZeros != 0) {
        auto j = binarySearch0(a, fromIndex, n, 0.0f);
        do {
            j--;
        } while (j >= fromIndex && (*a)[j] == 0.0f);
        for (auto k = int32_t(0); k < numNegZeros; k++)
                        (*a)[++j] = -0.0f;

    }
}

void Arrays::sort1(vector<int64_t>* x, int32_t off, int32_t len)
{

    if(len < 7) {
        for (auto i = off; i < len + off; i++)
                        for (auto j = i; j > off && (*x)[j - int32_t(1)] > (*x)[j]; j--)
                                swap(x, j, j - int32_t(1));


        return;
    }
    auto m = off + (len >> int32_t(1));
    if(len > 7) {
        auto l = off;
        auto n = off + len - int32_t(1);
        if(len > 40) {
            auto s = len / int32_t(8);
            l = med3(x, l, l + s, l + int32_t(2) * s);
            m = med3(x, m - s, m, m + s);
            n = med3(x, n - int32_t(2) * s, n - s, n);
        }
        m = med3(x, l, m, n);
    }
    auto v = (*x)[m];
    int32_t a = off, b = a, c = off + len - int32_t(1), d = c;
    while (true) {
        while (b <= c && (*x)[b] <= v) {
            if((*x)[b] == v)
                swap(x, a++, b);

            b++;
        }
        while (c >= b && (*x)[c] >= v) {
            if((*x)[c] == v)
                swap(x, c, d--);

            c--;
        }
        if(b > c)
            break;

        swap(x, b++, c--);
    }
    int32_t s, n = off + len;
    s = moduru::math::Math::min(a - off, b - a);
    vecswap(x, off, b - s, s);
    s = moduru::math::Math::min(d - c, n - d - int32_t(1));
    vecswap(x, b, n - s, s);
    if((s = b - a) > 1)
        sort1(x, off, s);

    if((s = d - c) > 1)
        sort1(x, n - s, s);

}

void Arrays::swap(vector<int64_t>* x, int32_t a, int32_t b)
{

    auto t = (*x)[a];
    (*x)[a] = (*x)[b];
    (*x)[b] = t;
}

void Arrays::vecswap(vector<int64_t>* x, int32_t a, int32_t b, int32_t n)
{

    for (auto i = int32_t(0); i < n; i++, a++, b++)
                swap(x, a, b);

}

int32_t Arrays::med3(vector<int64_t>* x, int32_t a, int32_t b, int32_t c)
{

    return ((*x)[a] < (*x)[b] ? ((*x)[b] < (*x)[c] ? b : (*x)[a] < (*x)[c] ? c : a) : ((*x)[b] > (*x)[c] ? b : (*x)[a] > (*x)[c] ? c : a));
}

void Arrays::sort1(vector<int32_t>* x, int32_t off, int32_t len)
{

    if(len < 7) {
        for (auto i = off; i < len + off; i++)
                        for (auto j = i; j > off && (*x)[j - int32_t(1)] > (*x)[j]; j--)
                                swap(x, j, j - int32_t(1));


        return;
    }
    auto m = off + (len >> int32_t(1));
    if(len > 7) {
        auto l = off;
        auto n = off + len - int32_t(1);
        if(len > 40) {
            auto s = len / int32_t(8);
            l = med3(x, l, l + s, l + int32_t(2) * s);
            m = med3(x, m - s, m, m + s);
            n = med3(x, n - int32_t(2) * s, n - s, n);
        }
        m = med3(x, l, m, n);
    }
    auto v = (*x)[m];
    int32_t a = off, b = a, c = off + len - int32_t(1), d = c;
    while (true) {
        while (b <= c && (*x)[b] <= v) {
            if((*x)[b] == v)
                swap(x, a++, b);

            b++;
        }
        while (c >= b && (*x)[c] >= v) {
            if((*x)[c] == v)
                swap(x, c, d--);

            c--;
        }
        if(b > c)
            break;

        swap(x, b++, c--);
    }
    int32_t s, n = off + len;
    s = moduru::math::Math::min(a - off, b - a);
    vecswap(x, off, b - s, s);
    s = moduru::math::Math::min(d - c, n - d - int32_t(1));
    vecswap(x, b, n - s, s);
    if((s = b - a) > 1)
        sort1(x, off, s);

    if((s = d - c) > 1)
        sort1(x, n - s, s);

}

void Arrays::swap(vector<int32_t>* x, int32_t a, int32_t b)
{

    auto t = (*x)[a];
    (*x)[a] = (*x)[b];
    (*x)[b] = t;
}

void Arrays::vecswap(vector<int32_t>* x, int32_t a, int32_t b, int32_t n)
{

    for (auto i = int32_t(0); i < n; i++, a++, b++)
                swap(x, a, b);

}

int32_t Arrays::med3(vector<int32_t>* x, int32_t a, int32_t b, int32_t c)
{

    return ((*x)[a] < (*x)[b] ? ((*x)[b] < (*x)[c] ? b : (*x)[a] < (*x)[c] ? c : a) : ((*x)[b] > (*x)[c] ? b : (*x)[a] > (*x)[c] ? c : a));
}

void Arrays::sort1(vector<int16_t>* x, int32_t off, int32_t len)
{

    if(len < 7) {
        for (auto i = off; i < len + off; i++)
                        for (auto j = i; j > off && (*x)[j - int32_t(1)] > (*x)[j]; j--)
                                swap(x, j, j - int32_t(1));


        return;
    }
    auto m = off + (len >> int32_t(1));
    if(len > 7) {
        auto l = off;
        auto n = off + len - int32_t(1);
        if(len > 40) {
            auto s = len / int32_t(8);
            l = med3(x, l, l + s, l + int32_t(2) * s);
            m = med3(x, m - s, m, m + s);
            n = med3(x, n - int32_t(2) * s, n - s, n);
        }
        m = med3(x, l, m, n);
    }
    auto v = (*x)[m];
    int32_t a = off, b = a, c = off + len - int32_t(1), d = c;
    while (true) {
        while (b <= c && (*x)[b] <= v) {
            if((*x)[b] == v)
                swap(x, a++, b);

            b++;
        }
        while (c >= b && (*x)[c] >= v) {
            if((*x)[c] == v)
                swap(x, c, d--);

            c--;
        }
        if(b > c)
            break;

        swap(x, b++, c--);
    }
    int32_t s, n = off + len;
    s = moduru::math::Math::min(a - off, b - a);
    vecswap(x, off, b - s, s);
    s = moduru::math::Math::min(d - c, n - d - int32_t(1));
    vecswap(x, b, n - s, s);
    if((s = b - a) > 1)
        sort1(x, off, s);

    if((s = d - c) > 1)
        sort1(x, n - s, s);

}

void Arrays::swap(vector<int16_t>* x, int32_t a, int32_t b)
{

    auto t = (*x)[a];
    (*x)[a] = (*x)[b];
    (*x)[b] = t;
}

void Arrays::vecswap(vector<int16_t>* x, int32_t a, int32_t b, int32_t n)
{

    for (auto i = int32_t(0); i < n; i++, a++, b++)
                swap(x, a, b);

}

int32_t Arrays::med3(vector<int16_t>* x, int32_t a, int32_t b, int32_t c)
{

    return ((*x)[a] < (*x)[b] ? ((*x)[b] < (*x)[c] ? b : (*x)[a] < (*x)[c] ? c : a) : ((*x)[b] > (*x)[c] ? b : (*x)[a] > (*x)[c] ? c : a));
}

void Arrays::sort1(vector<char16_t>* x, int32_t off, int32_t len)
{

    if(len < 7) {
        for (auto i = off; i < len + off; i++)
                        for (auto j = i; j > off && (*x)[j - int32_t(1)] > (*x)[j]; j--)
                                swap(x, j, j - int32_t(1));


        return;
    }
    auto m = off + (len >> int32_t(1));
    if(len > 7) {
        auto l = off;
        auto n = off + len - int32_t(1);
        if(len > 40) {
            auto s = len / int32_t(8);
            l = med3(x, l, l + s, l + int32_t(2) * s);
            m = med3(x, m - s, m, m + s);
            n = med3(x, n - int32_t(2) * s, n - s, n);
        }
        m = med3(x, l, m, n);
    }
    auto v = (*x)[m];
    int32_t a = off, b = a, c = off + len - int32_t(1), d = c;
    while (true) {
        while (b <= c && (*x)[b] <= v) {
            if((*x)[b] == v)
                swap(x, a++, b);

            b++;
        }
        while (c >= b && (*x)[c] >= v) {
            if((*x)[c] == v)
                swap(x, c, d--);

            c--;
        }
        if(b > c)
            break;

        swap(x, b++, c--);
    }
    int32_t s, n = off + len;
    s = moduru::math::Math::min(a - off, b - a);
    vecswap(x, off, b - s, s);
    s = moduru::math::Math::min(d - c, n - d - int32_t(1));
    vecswap(x, b, n - s, s);
    if((s = b - a) > 1)
        sort1(x, off, s);

    if((s = d - c) > 1)
        sort1(x, n - s, s);

}

void Arrays::swap(vector<char16_t>* x, int32_t a, int32_t b)
{

    auto t = (*x)[a];
    (*x)[a] = (*x)[b];
    (*x)[b] = t;
}

void Arrays::vecswap(vector<char16_t>* x, int32_t a, int32_t b, int32_t n)
{

    for (auto i = int32_t(0); i < n; i++, a++, b++)
                swap(x, a, b);

}

int32_t Arrays::med3(vector<char16_t>* x, int32_t a, int32_t b, int32_t c)
{

    return ((*x)[a] < (*x)[b] ? ((*x)[b] < (*x)[c] ? b : (*x)[a] < (*x)[c] ? c : a) : ((*x)[b] > (*x)[c] ? b : (*x)[a] > (*x)[c] ? c : a));
}

void Arrays::sort1(vector<int8_t>* x, int32_t off, int32_t len)
{

    if(len < 7) {
        for (auto i = off; i < len + off; i++)
                        for (auto j = i; j > off && (*x)[j - int32_t(1)] > (*x)[j]; j--)
                                swap(x, j, j - int32_t(1));


        return;
    }
    auto m = off + (len >> int32_t(1));
    if(len > 7) {
        auto l = off;
        auto n = off + len - int32_t(1);
        if(len > 40) {
            auto s = len / int32_t(8);
            l = med3(x, l, l + s, l + int32_t(2) * s);
            m = med3(x, m - s, m, m + s);
            n = med3(x, n - int32_t(2) * s, n - s, n);
        }
        m = med3(x, l, m, n);
    }
    auto v = (*x)[m];
    int32_t a = off, b = a, c = off + len - int32_t(1), d = c;
    while (true) {
        while (b <= c && (*x)[b] <= v) {
            if((*x)[b] == v)
                swap(x, a++, b);

            b++;
        }
        while (c >= b && (*x)[c] >= v) {
            if((*x)[c] == v)
                swap(x, c, d--);

            c--;
        }
        if(b > c)
            break;

        swap(x, b++, c--);
    }
    int32_t s, n = off + len;
    s = moduru::math::Math::min(a - off, b - a);
    vecswap(x, off, b - s, s);
    s = moduru::math::Math::min(d - c, n - d - int32_t(1));
    vecswap(x, b, n - s, s);
    if((s = b - a) > 1)
        sort1(x, off, s);

    if((s = d - c) > 1)
        sort1(x, n - s, s);

}

void Arrays::swap(vector<int8_t>* x, int32_t a, int32_t b)
{

    auto t = (*x)[a];
    (*x)[a] = (*x)[b];
    (*x)[b] = t;
}

void Arrays::vecswap(vector<int8_t>* x, int32_t a, int32_t b, int32_t n)
{

    for (auto i = int32_t(0); i < n; i++, a++, b++)
                swap(x, a, b);

}

int32_t Arrays::med3(vector<int8_t>* x, int32_t a, int32_t b, int32_t c)
{

    return ((*x)[a] < (*x)[b] ? ((*x)[b] < (*x)[c] ? b : (*x)[a] < (*x)[c] ? c : a) : ((*x)[b] > (*x)[c] ? b : (*x)[a] > (*x)[c] ? c : a));
}

void Arrays::sort1(vector<double>* x, int32_t off, int32_t len)
{

    if(len < 7) {
        for (auto i = off; i < len + off; i++)
                        for (auto j = i; j > off && (*x)[j - int32_t(1)] > (*x)[j]; j--)
                                swap(x, j, j - int32_t(1));


        return;
    }
    auto m = off + (len >> int32_t(1));
    if(len > 7) {
        auto l = off;
        auto n = off + len - int32_t(1);
        if(len > 40) {
            auto s = len / int32_t(8);
            l = med3(x, l, l + s, l + int32_t(2) * s);
            m = med3(x, m - s, m, m + s);
            n = med3(x, n - int32_t(2) * s, n - s, n);
        }
        m = med3(x, l, m, n);
    }
    auto v = (*x)[m];
    int32_t a = off, b = a, c = off + len - int32_t(1), d = c;
    while (true) {
        while (b <= c && (*x)[b] <= v) {
            if((*x)[b] == v)
                swap(x, a++, b);

            b++;
        }
        while (c >= b && (*x)[c] >= v) {
            if((*x)[c] == v)
                swap(x, c, d--);

            c--;
        }
        if(b > c)
            break;

        swap(x, b++, c--);
    }
    int32_t s, n = off + len;
    s = moduru::math::Math::min(a - off, b - a);
    vecswap(x, off, b - s, s);
    s = moduru::math::Math::min(d - c, n - d - int32_t(1));
    vecswap(x, b, n - s, s);
    if((s = b - a) > 1)
        sort1(x, off, s);

    if((s = d - c) > 1)
        sort1(x, n - s, s);

}

void Arrays::swap(vector<double>* x, int32_t a, int32_t b)
{

    auto t = (*x)[a];
    (*x)[a] = (*x)[b];
    (*x)[b] = t;
}

void Arrays::vecswap(vector<double>* x, int32_t a, int32_t b, int32_t n)
{
    for (auto i = int32_t(0); i < n; i++, a++, b++)
                swap(x, a, b);

}

int32_t Arrays::med3(vector<double>* x, int32_t a, int32_t b, int32_t c)
{
    return ((*x)[a] < (*x)[b] ? ((*x)[b] < (*x)[c] ? b : (*x)[a] < (*x)[c] ? c : a) : ((*x)[b] > (*x)[c] ? b : (*x)[a] > (*x)[c] ? c : a));
}

void Arrays::sort1(vector<float>* x, int32_t off, int32_t len)
{
    if(len < 7) {
        for (auto i = off; i < len + off; i++)
                        for (auto j = i; j > off && (*x)[j - int32_t(1)] > (*x)[j]; j--)
                                swap(x, j, j - int32_t(1));


        return;
    }
    auto m = off + (len >> int32_t(1));
    if(len > 7) {
        auto l = off;
        auto n = off + len - int32_t(1);
        if(len > 40) {
            auto s = len / int32_t(8);
            l = med3(x, l, l + s, l + int32_t(2) * s);
            m = med3(x, m - s, m, m + s);
            n = med3(x, n - int32_t(2) * s, n - s, n);
        }
        m = med3(x, l, m, n);
    }
    auto v = (*x)[m];
    int32_t a = off, b = a, c = off + len - int32_t(1), d = c;
    while (true) {
        while (b <= c && (*x)[b] <= v) {
            if((*x)[b] == v)
                swap(x, a++, b);

            b++;
        }
        while (c >= b && (*x)[c] >= v) {
            if((*x)[c] == v)
                swap(x, c, d--);

            c--;
        }
        if(b > c)
            break;

        swap(x, b++, c--);
    }
    int32_t s, n = off + len;
    s = moduru::math::Math::min(a - off, b - a);
    vecswap(x, off, b - s, s);
    s = moduru::math::Math::min(d - c, n - d - int32_t(1));
    vecswap(x, b, n - s, s);
    if((s = b - a) > 1)
        sort1(x, off, s);

    if((s = d - c) > 1)
        sort1(x, n - s, s);

}

void Arrays::swap(vector<float>* x, int32_t a, int32_t b)
{
    auto t = (*x)[a];
    (*x)[a] = (*x)[b];
    (*x)[b] = t;
}

void Arrays::vecswap(vector<float>* x, int32_t a, int32_t b, int32_t n)
{
    for (auto i = int32_t(0); i < n; i++, a++, b++)
                swap(x, a, b);
}

int32_t Arrays::med3(vector<float>* x, int32_t a, int32_t b, int32_t c)
{
    return ((*x)[a] < (*x)[b] ? ((*x)[b] < (*x)[c] ? b : (*x)[a] < (*x)[c] ? c : a) : ((*x)[b] > (*x)[c] ? b : (*x)[a] > (*x)[c] ? c : a));
}

void Arrays::sort(vector<moduru::lang::Object*>* a)
{
	auto aux = vector<moduru::lang::Object*>();
	for (auto& o : *a)
		aux.push_back(o);
	mergeSort(&aux, a, 0, a->size(), 0);
}

void Arrays::sort(vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    auto aux = dynamic_cast< vector<moduru::lang::Object*>* >(copyOfRange(static_cast< vector<moduru::lang::Object*>* >(a), fromIndex, toIndex));
    mergeSort(aux, a, fromIndex, toIndex, -fromIndex);
}

constexpr int32_t Arrays::INSERTIONSORT_THRESHOLD;

void Arrays::mergeSort(vector<moduru::lang::Object*>* src, vector<moduru::lang::Object*>* dest, int32_t low, int32_t high, int32_t off)
{
	auto length = high - low;
	if (length < INSERTIONSORT_THRESHOLD) {
		for (auto i = low; i < high; i++)
			for (auto j = i; j > low && (dynamic_cast<moduru::lang::Comparable*>((*dest)[j - int32_t(1)]))->compareTo((*dest)[j]) > 0; j--)
				swap(dest, j, j - int32_t(1));
		return;
	}
	auto destLow = low;
	auto destHigh = high;
	low += off;
	high += off;
	auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
	mergeSort(dest, src, low, mid, -off);
	mergeSort(dest, src, mid, high, -off);
	if ((dynamic_cast<moduru::lang::Comparable*>((*src)[mid - int32_t(1)]))->compareTo((*src)[mid]) <= 0) {
		moduru::VecUtil::VecCopy(src, low, dest, destLow, length);
		return;
	}
	for (int32_t i = destLow, p = low, q = mid; i < destHigh; i++) {
		if (q >= high || p < mid && (dynamic_cast<moduru::lang::Comparable*>((*src)[p]))->compareTo((*src)[q]) <= 0)
			(*dest)[i] = (*src)[p++];
		else
			(*dest)[i] = (*src)[q++];
	}
}

void Arrays::swap(vector<moduru::lang::Object*>* x, int32_t a, int32_t b)
{
	auto t = (*x)[a];
	(*x)[a] = (*x)[b];
	(*x)[b] = t;
}

void Arrays::sort(vector<moduru::lang::Object*>* a, Comparator* c)
{
	auto aux = vector<moduru::lang::Object*>();
	for (auto& o : *a)
		aux.push_back(o);
	if (c == nullptr)
		mergeSort(&aux, a, 0, a->size(), 0);
	else
		mergeSort(&aux, a, 0, a->size(), 0, c);
}

void Arrays::sort(vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, Comparator* c)
{
    rangeCheck(a->size(), fromIndex, toIndex);
    auto aux = dynamic_cast< vector<moduru::lang::Object*>* >(dynamic_cast< vector<moduru::lang::Object*>* >(copyOfRange(static_cast< vector<moduru::lang::Object*>* >(a), fromIndex, toIndex)));
    if(c == nullptr)
        mergeSort(aux, a, fromIndex, toIndex, -fromIndex);
    else
        mergeSort(aux, a, fromIndex, toIndex, -fromIndex, c);
}

void Arrays::mergeSort(vector<moduru::lang::Object*>* src, vector<moduru::lang::Object*>* dest, int32_t low, int32_t high, int32_t off, Comparator* c)
{

	auto length = high - low;
	if (length < INSERTIONSORT_THRESHOLD) {
		for (auto i = low; i < high; i++)
			for (auto j = i; j > low && c->compare((*dest)[j - int32_t(1)], (*dest)[j]) > 0; j--)
				swap(dest, j, j - int32_t(1));


		return;
	}
	auto destLow = low;
	auto destHigh = high;
	low += off;
	high += off;
	auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
	mergeSort(dest, src, low, mid, -off, c);
	mergeSort(dest, src, mid, high, -off, c);
	if (c->compare((*src)[mid - int32_t(1)], (*src)[mid]) <= 0) {
		moduru::VecUtil::VecCopy(src, low, dest, destLow, length);
		return;
	}
	for (int32_t i = destLow, p = low, q = mid; i < destHigh; i++) {
		if (q >= high || p < mid && c->compare((*src)[p], (*src)[q]) <= 0)
			(*dest)[i] = (*src)[p++];
		else
			(*dest)[i] = (*src)[q++];
	}
}

void Arrays::rangeCheck(int32_t arrayLen, int32_t fromIndex, int32_t toIndex)
{
	if (fromIndex > toIndex)
		//throw std::invalid_argument(moduru::lang::StringBuilder().append("fromIndex(")->append(fromIndex)->append(") > toIndex(")
		//			->append(toIndex)->append(")")->toString()->g);
		throw std::invalid_argument("arrays range check");

	if (fromIndex < 0)
		throw std::invalid_argument(to_string(fromIndex).c_str());

	if (toIndex > arrayLen)
		throw std::invalid_argument(to_string(toIndex).c_str());
}

int32_t Arrays::binarySearch(vector<int64_t>* a, int64_t key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<int64_t>* a, int32_t fromIndex, int32_t toIndex, int64_t key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<int64_t>* a, int32_t fromIndex, int32_t toIndex, int64_t key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        if(midVal < key)
            low = mid + int32_t(1);
        else if(midVal > key)
            high = mid - int32_t(1);
        else
            return mid;
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<int32_t>* a, int32_t key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<int32_t>* a, int32_t fromIndex, int32_t toIndex, int32_t key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<int32_t>* a, int32_t fromIndex, int32_t toIndex, int32_t key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        if(midVal < key)
            low = mid + int32_t(1);
        else if(midVal > key)
            high = mid - int32_t(1);
        else
            return mid;
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<int16_t>* a, int16_t key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<int16_t>* a, int32_t fromIndex, int32_t toIndex, int16_t key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<int16_t>* a, int32_t fromIndex, int32_t toIndex, int16_t key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        if(midVal < key)
            low = mid + int32_t(1);
        else if(midVal > key)
            high = mid - int32_t(1);
        else
            return mid;
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<char16_t>* a, char16_t key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<char16_t>* a, int32_t fromIndex, int32_t toIndex, char16_t key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<char16_t>* a, int32_t fromIndex, int32_t toIndex, char16_t key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        if(midVal < key)
            low = mid + int32_t(1);
        else if(midVal > key)
            high = mid - int32_t(1);
        else
            return mid;
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<int8_t>* a, int8_t key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<int8_t>* a, int32_t fromIndex, int32_t toIndex, int8_t key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<int8_t>* a, int32_t fromIndex, int32_t toIndex, int8_t key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        if(midVal < key)
            low = mid + int32_t(1);
        else if(midVal > key)
            high = mid - int32_t(1);
        else
            return mid;
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<double>* a, double key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<double>* a, int32_t fromIndex, int32_t toIndex, double key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<double>* a, int32_t fromIndex, int32_t toIndex, double key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        if(midVal < key)
            low = mid + int32_t(1);
        else if(midVal > key)
            high = mid - int32_t(1);
        else {
            auto midBits = moduru::lang::Double::doubleToLongBits(midVal);
            auto keyBits = moduru::lang::Double::doubleToLongBits(key);
            if(midBits == keyBits)
                return mid;
            else if(midBits < keyBits)
                low = mid + int32_t(1);
            else
                high = mid - int32_t(1);
        }
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<float>* a, float key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<float>* a, int32_t fromIndex, int32_t toIndex, float key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<float>* a, int32_t fromIndex, int32_t toIndex, float key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        if(midVal < key)
            low = mid + int32_t(1);
        else if(midVal > key)
            high = mid - int32_t(1);
        else {
            auto midBits = moduru::lang::Float::floatToIntBits(midVal);
            auto keyBits = moduru::lang::Float::floatToIntBits(key);
            if(midBits == keyBits)
                return mid;
            else if(midBits < keyBits)
                low = mid + int32_t(1);
            else
                high = mid - int32_t(1);
        }
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<moduru::lang::Object*>* a, moduru::lang::Object* key)
{

    return binarySearch0(a, int32_t(0), a->size(), key);
}

int32_t Arrays::binarySearch(vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

int32_t Arrays::binarySearch0(vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key)
{

    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = dynamic_cast< moduru::lang::Comparable* >((*a)[mid]);
        auto cmp = midVal->compareTo(key);
        if(cmp < 0)
            low = mid + int32_t(1);
        else if(cmp > 0)
            high = mid - int32_t(1);
        else
            return mid;
    }
    return -(low + int32_t(1));
}

int32_t Arrays::binarySearch(vector<moduru::lang::Object*>* a, moduru::lang::Object* key, Comparator* c)
{

    return binarySearch0(a, int32_t(0), a->size(), key, c);
}

int32_t Arrays::binarySearch(vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key, Comparator* c)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key, c);
}

int32_t Arrays::binarySearch0(vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key, Comparator* c)
{

    if(c == nullptr) {
        return binarySearch0(static_cast< vector<moduru::lang::Object*>* >(a), fromIndex, toIndex, static_cast< moduru::lang::Object* >(key));
    }
    auto low = fromIndex;
    auto high = toIndex - int32_t(1);
    while (low <= high) {
        auto mid = static_cast<int32_t>(static_cast<uint32_t>((low + high)) >> int32_t(1));
        auto midVal = (*a)[mid];
        auto cmp = c->compare(midVal, key);
        if(cmp < 0)
            low = mid + int32_t(1);
        else if(cmp > 0)
            high = mid - int32_t(1);
        else
            return mid;
    }
    return -(low + int32_t(1));
}

bool Arrays::equals(vector<int64_t>* a, vector<int64_t>* a2)
{

    if(a == a2)
        return true;

    if(a == nullptr || a2 == nullptr)
        return false;

    auto length = a->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++)
                if((*a)[i] != (*a2)[i])
            return false;


    return true;
}

bool Arrays::equals(vector<int32_t>* a, vector<int32_t>* a2)
{

    if(a == a2)
        return true;

    if(a == nullptr || a2 == nullptr)
        return false;

    auto length = a->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++)
                if((*a)[i] != (*a2)[i])
            return false;


    return true;
}

bool Arrays::equals(vector<int16_t>* a, vector<int16_t>* a2)
{
	if (a == a2)
		return true;

	if (a == nullptr || a2 == nullptr)
		return false;

	auto length = a->size();
	if (a2->size() != length)
		return false;

	for (auto i = int32_t(0); i < length; i++)
		if ((*a)[i] != (*a2)[i])
			return false;
	return true;
}

bool Arrays::equals(vector<char16_t>* a, vector<char16_t>* a2)
{

    if(a == a2)
        return true;

    if(a == nullptr || a2 == nullptr)
        return false;

    auto length = a->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++)
                if((*a)[i] != (*a2)[i])
            return false;


    return true;
}

bool Arrays::equals(vector<int8_t>* a, vector<int8_t>* a2)
{

    if(a == a2)
        return true;

    if(a == nullptr || a2 == nullptr)
        return false;

    auto length = a->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++)
                if((*a)[i] != (*a2)[i])
            return false;


    return true;
}

bool Arrays::equals(vector<bool>* a, vector<bool>* a2)
{

    if(a == a2)
        return true;

    if(a == nullptr || a2 == nullptr)
        return false;

    auto length = a->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++)
                if((*a)[i] != (*a2)[i])
            return false;


    return true;
}

bool Arrays::equals(vector<double>* a, vector<double>* a2)
{

    if(a == a2)
        return true;

    if(a == nullptr || a2 == nullptr)
        return false;

    auto length = a->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++)
                if(moduru::lang::Double::doubleToLongBits((*a)[i]) != moduru::lang::Double::doubleToLongBits((*a2)[i]))
            return false;


    return true;
}

bool Arrays::equals(vector<float>* a, vector<float>* a2)
{

    if(a == a2)
        return true;

    if(a == nullptr || a2 == nullptr)
        return false;

    auto length = a->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++)
                if(moduru::lang::Float::floatToIntBits((*a)[i]) != moduru::lang::Float::floatToIntBits((*a2)[i]))
            return false;


    return true;
}

bool Arrays::equals(vector<moduru::lang::Object*>* a, vector<moduru::lang::Object*>* a2)
{

	if (a == a2)
		return true;

	if (a == nullptr || a2 == nullptr)
		return false;

	auto length = a->size();
	if (a2->size() != length)
		return false;

	for (auto i = int32_t(0); i < length; i++) {
		auto o1 = (*a)[i];
		auto o2 = (*a2)[i];
		if (!(o1 == nullptr ? o2 == nullptr : o1->equals(o2)))
			return false;

	}
	return true;
}

void Arrays::fill(vector<int64_t>* a, int64_t val)
{

    for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<int64_t>* a, int32_t fromIndex, int32_t toIndex, int64_t val)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    for (auto i = fromIndex; i < toIndex; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<int32_t>* a, int32_t val)
{

    for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<int32_t>* a, int32_t fromIndex, int32_t toIndex, int32_t val)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    for (auto i = fromIndex; i < toIndex; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<int16_t>* a, int16_t val)
{

    for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<int16_t>* a, int32_t fromIndex, int32_t toIndex, int16_t val)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    for (auto i = fromIndex; i < toIndex; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<char16_t>* a, char16_t val)
{

    for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<char16_t>* a, int32_t fromIndex, int32_t toIndex, char16_t val)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    for (auto i = fromIndex; i < toIndex; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<int8_t>* a, int8_t val)
{
	for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
		(*a)[i] = val;
}

void Arrays::fill(vector<int8_t>* a, int32_t fromIndex, int32_t toIndex, int8_t val)
{
	rangeCheck(a->size(), fromIndex, toIndex);
	for (auto i = fromIndex; i < toIndex; i++)
		(*a)[i] = val;
}

void Arrays::fill(vector<bool>* a, bool val)
{
	for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
		(*a)[i] = val;
}

void Arrays::fill(vector<bool>* a, int32_t fromIndex, int32_t toIndex, bool val)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    for (auto i = fromIndex; i < toIndex; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<double>* a, double val)
{

    for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<double>* a, int32_t fromIndex, int32_t toIndex, double val)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    for (auto i = fromIndex; i < toIndex; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<float>* a, float val)
{

    for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<float>* a, int32_t fromIndex, int32_t toIndex, float val)
{

    rangeCheck(a->size(), fromIndex, toIndex);
    for (auto i = fromIndex; i < toIndex; i++)
                (*a)[i] = val;

}

void Arrays::fill(vector<moduru::lang::Object*>* a, moduru::lang::Object* val)
{
	for (int32_t i = int32_t(0), len = a->size(); i < len; i++)
		(*a)[i] = val;
}

void Arrays::fill(vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* val)
{
	rangeCheck(a->size(), fromIndex, toIndex);
	for (auto i = fromIndex; i < toIndex; i++)
		(*a)[i] = val;
}

vector<moduru::lang::Object*>* Arrays::copyOf(vector<moduru::lang::Object*>* original, int32_t newLength)
{

	//return dynamic_cast< vector<moduru::lang::Object*>* >(dynamic_cast< vector<moduru::lang::Object*>* >(copyOf(original, newLength, original->getClass())));
	auto copyLength = moduru::math::Math::min(original->size(), newLength);
	vector<moduru::lang::Object*>* copy = new vector<moduru::lang::Object*>(copyLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, copyLength);
	return copy;
}

/*
vector<moduru::lang::Object*>* Arrays::copyOf(vector<moduru::lang::Object*>* original, int32_t newLength, moduru::lang::Class* newType)
{

    auto copy = (dynamic_cast< moduru::lang::Object* >(newType) == dynamic_cast< moduru::lang::Object* >(moduru::lang::ObjectArray::class_())) ? dynamic_cast< vector<moduru::lang::Object*>* >(new moduru::lang::ObjectArray(newLength)) : dynamic_cast< vector<moduru::lang::Object*>* >(moduru::lang::reflect::Array_::newInstance(static_cast< moduru::lang::Class* >(npc(newType)->getComponentType()), newLength));
    moduru::lang::System::arraycopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
    return copy;
}
*/

vector<int8_t>* Arrays::copyOf(vector<int8_t>* original, int32_t newLength)
{

	auto copy = new vector<int8_t>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<int16_t>* Arrays::copyOf(vector<int16_t>* original, int32_t newLength)
{

	auto copy = new vector<int16_t>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<int32_t>* Arrays::copyOf(vector<int32_t>* original, int32_t newLength)
{

	auto copy = new vector<int32_t>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<int64_t>* Arrays::copyOf(vector<int64_t>* original, int32_t newLength)
{

	auto copy = new vector<int64_t>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<char16_t>* Arrays::copyOf(vector<char16_t>* original, int32_t newLength)
{

	auto copy = new vector<char16_t>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<float>* Arrays::copyOf(vector<float>* original, int32_t newLength)
{

	auto copy = new vector<float>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<double>* Arrays::copyOf(vector<double>* original, int32_t newLength)
{

	auto copy = new vector<double>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<bool>* Arrays::copyOf(vector<bool>* original, int32_t newLength)
{

	auto copy = new vector<bool>(newLength);
	moduru::VecUtil::VecCopy(original, 0, copy, 0, moduru::math::Math::min(original->size(), newLength));
	return copy;
}

vector<moduru::lang::Object*>* Arrays::copyOfRange(vector<moduru::lang::Object*>* original, int32_t from, int32_t to)
{

    //return dynamic_cast< vector<moduru::lang::Object*>* >(copyOfRange(original, from, to, dynamic_cast< moduru::lang::Class* >(original->getClass())));
	return nullptr;
}

/*
vector<moduru::lang::Object*>* Arrays::copyOfRange(vector<moduru::lang::Object*>* original, int32_t from, int32_t to, moduru::lang::Class* newType)
{

    auto newLength = to - from;
    if(newLength < 0)
        throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
            ->append(to)->toString());

    auto copy = (dynamic_cast< moduru::lang::Object* >(newType) == dynamic_cast< moduru::lang::Object* >(moduru::lang::ObjectArray::class_())) ? dynamic_cast< vector<moduru::lang::Object*>* >(new moduru::lang::ObjectArray(newLength)) : dynamic_cast< vector<moduru::lang::Object*>* >(moduru::lang::reflect::Array_::newInstance(static_cast< moduru::lang::Class* >(npc(newType)->getComponentType()), newLength));
    moduru::lang::System::arraycopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
    return copy;
}
*/

vector<int8_t>* Arrays::copyOfRange(vector<int8_t>* original, int32_t from, int32_t to)
{

	auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
		//            ->append(to)->toString());
		throw std::invalid_argument("illegal argument");

	auto copy = new vector<int8_t>(newLength);
	moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
	return copy;
}

vector<int16_t>* Arrays::copyOfRange(vector<int16_t>* original, int32_t from, int32_t to)
{

	auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
		//            ->append(to)->toString());
		throw std::invalid_argument("illegal argument");

	auto copy = new vector<int16_t>(newLength);
	moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
	return copy;
}

vector<int32_t>* Arrays::copyOfRange(vector<int32_t>* original, int32_t from, int32_t to)
{

	auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
		//    ->append(to)->toString());
		throw std::invalid_argument("illegal argument");

	auto copy = new vector<int32_t>(newLength);
	moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
	return copy;
}

vector<int64_t>* Arrays::copyOfRange(vector<int64_t>* original, int32_t from, int32_t to)
{

    auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
		 //   ->append(to)->toString());
		throw std::invalid_argument("illegal argument");

    auto copy = new vector<int64_t>(newLength);
    moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
    return copy;
}

vector<char16_t>* Arrays::copyOfRange(vector<char16_t>* original, int32_t from, int32_t to)
{

	auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
		 //   ->append(to)->toString());
		throw std::invalid_argument("illegal argument");

	auto copy = new vector<char16_t>(newLength);
	moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
	return copy;
}

vector<float>* Arrays::copyOfRange(vector<float>* original, int32_t from, int32_t to)
{

	auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
			//->append(to)->toString());
		throw std::invalid_argument("illegal argument");

	auto copy = new vector<float>(newLength);
	moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
	return copy;
}

vector<double>* Arrays::copyOfRange(vector<double>* original, int32_t from, int32_t to)
{

	auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
		//->append(to)->toString());
		throw std::invalid_argument("illegal argument");

	auto copy = new vector<double>(newLength);
	moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
	return copy;
}

vector<bool>* Arrays::copyOfRange(vector<bool>* original, int32_t from, int32_t to)
{

	auto newLength = to - from;
	if (newLength < 0)
		//throw new moduru::lang::IllegalArgumentException(moduru::lang::StringBuilder().append(from)->append(u" > "_j)
		//->append(to)->toString());
		throw std::invalid_argument("illegal argument");

	auto copy = new vector<bool>(newLength);
	moduru::VecUtil::VecCopy(original, from, copy, 0, moduru::math::Math::min(original->size() - from, newLength));
	return copy;
}

/*
java::util::List* Arrays::asList(vector<moduru::lang::Object*>* a)
{

    return new Arrays_ArrayList(a);
}
*/

int32_t Arrays::hashCode(vector<int64_t>* a)
{
	if (a == nullptr)
		return 0;

	auto result = int32_t(1);
	for (auto element : *a) {
		auto elementHash = static_cast<int32_t>((element ^ (static_cast<int64_t>(static_cast<uint64_t>(element) >> int32_t(32)))));
		result = int32_t(31) * result + elementHash;
	}
	return result;
}

int32_t Arrays::hashCode(vector<int32_t>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a)
                result = int32_t(31) * result + element;

    return result;
}

int32_t Arrays::hashCode(vector<int16_t>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a)
                result = int32_t(31) * result + element;

    return result;
}

int32_t Arrays::hashCode(vector<char16_t>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a)
                result = int32_t(31) * result + element;

    return result;
}

int32_t Arrays::hashCode(vector<int8_t>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a)
                result = int32_t(31) * result + element;

    return result;
}

int32_t Arrays::hashCode(vector<bool>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a)
                result = int32_t(31) * result + (element ? int32_t(1231) : int32_t(1237));

    return result;
}

int32_t Arrays::hashCode(vector<float>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a)
                result = int32_t(31) * result + moduru::lang::Float::floatToIntBits(element);

    return result;
}

int32_t Arrays::hashCode(vector<double>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a) {
        auto bits = moduru::lang::Double::doubleToLongBits(element);
        result = int32_t(31) * result + static_cast< int32_t >((bits ^ (static_cast<int64_t>(static_cast<uint64_t>(bits) >> int32_t(32)))));
    }
    return result;
}

int32_t Arrays::hashCode(vector<moduru::lang::Object*>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a)
                result = int32_t(31) * result + (element == nullptr ? int32_t(0) : element->hashCode());

    return result;
}

int32_t Arrays::deepHashCode(vector<moduru::lang::Object*>* a)
{

    if(a == nullptr)
        return 0;

    auto result = int32_t(1);
    for(auto element : *a) {
        auto elementHash = int32_t(0);
        if(dynamic_cast< vector<moduru::lang::Object*>* >(element) != nullptr)
            elementHash = deepHashCode(dynamic_cast< vector<moduru::lang::Object*>* >(element));
        else if(dynamic_cast< vector<int8_t>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<int8_t>* >(element));
        else if(dynamic_cast< vector<int16_t>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<int16_t>* >(element));
        else if(dynamic_cast< vector<int32_t>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<int32_t>* >(element));
        else if(dynamic_cast< vector<int64_t>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<int64_t>* >(element));
        else if(dynamic_cast< vector<char16_t>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<char16_t>* >(element));
        else if(dynamic_cast< vector<float>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<float>* >(element));
        else if(dynamic_cast< vector<double>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<double>* >(element));
        else if(dynamic_cast< vector<bool>* >(element) != nullptr)
            elementHash = hashCode(dynamic_cast< vector<bool>* >(element));
        else if(element != nullptr)
            elementHash = element->hashCode();

        result = int32_t(31) * result + elementHash;
    }
    return result;
}

bool Arrays::deepEquals(vector<moduru::lang::Object*>* a1, vector<moduru::lang::Object*>* a2)
{

    if(a1 == a2)
        return true;

    if(a1 == nullptr || a2 == nullptr)
        return false;

    auto length = a1->size();
    if(a2->size() != length)
        return false;

    for (auto i = int32_t(0); i < length; i++) {
        auto e1 = (*a1)[i];
        auto e2 = (*a2)[i];
        if(e1 == e2)
            continue;

        if(e1 == nullptr)
            return false;

        bool eq;
        if(dynamic_cast< vector<moduru::lang::Object*>* >(e1) != nullptr && dynamic_cast< vector<moduru::lang::Object*>* >(e2) != nullptr)
            eq = deepEquals(dynamic_cast< vector<moduru::lang::Object*>* >(e1), dynamic_cast< vector<moduru::lang::Object*>* >(e2));
        else if(dynamic_cast< vector<int8_t>* >(e1) != nullptr && dynamic_cast< vector<int8_t>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<int8_t>* >(e1), dynamic_cast< vector<int8_t>* >(e2));
        else if(dynamic_cast< vector<int16_t>* >(e1) != nullptr && dynamic_cast< vector<int16_t>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<int16_t>* >(e1), dynamic_cast< vector<int16_t>* >(e2));
        else if(dynamic_cast< vector<int32_t>* >(e1) != nullptr && dynamic_cast< vector<int32_t>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<int32_t>* >(e1), dynamic_cast< vector<int32_t>* >(e2));
        else if(dynamic_cast< vector<int64_t>* >(e1) != nullptr && dynamic_cast< vector<int64_t>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<int64_t>* >(e1), dynamic_cast< vector<int64_t>* >(e2));
        else if(dynamic_cast< vector<char16_t>* >(e1) != nullptr && dynamic_cast< vector<char16_t>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<char16_t>* >(e1), dynamic_cast< vector<char16_t>* >(e2));
        else if(dynamic_cast< vector<float>* >(e1) != nullptr && dynamic_cast< vector<float>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<float>* >(e1), dynamic_cast< vector<float>* >(e2));
        else if(dynamic_cast< vector<double>* >(e1) != nullptr && dynamic_cast< vector<double>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<double>* >(e1), dynamic_cast< vector<double>* >(e2));
        else if(dynamic_cast< vector<bool>* >(e1) != nullptr && dynamic_cast< vector<bool>* >(e2) != nullptr)
            eq = equals(dynamic_cast< vector<bool>* >(e1), dynamic_cast< vector<bool>* >(e2));
        else
            eq = e1->equals(e2);
        if(!eq)
            return false;

    }
    return true;
}

/*
moduru::lang::String* Arrays::toString(vector<int64_t>* a)
{

	if (a == nullptr)
		return new moduru::lang::String("null");

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append((*a)[i]);
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<int32_t>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append((*a)[i]);
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<int16_t>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append(static_cast< int32_t >((*a)[i]));
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<char16_t>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append((*a)[i]);
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<int8_t>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append(static_cast< int32_t >((*a)[i]));
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<bool>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append((*a)[i]);
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<float>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append((*a)[i]);
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<double>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append((*a)[i]);
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::toString(vector<moduru::lang::Object*>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1))
        return u"[]"_j;

    auto b = new moduru::lang::StringBuilder();
    b->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        b->append(moduru::lang::String::valueOf((*a)[i]));
        if(i == iMax)
            return npc(b->append(u']'))->toString();

        b->append(u", "_j);
    }
}

java::lang::String* Arrays::deepToString(vector<moduru::lang::Object*>* a)
{

    if(a == nullptr)
        return u"null"_j;

    auto bufLen = int32_t(20) * a->size();
    if(a->size() != 0 && bufLen <= 0)
        bufLen = moduru::lang::Integer::MAX_VALUE;

    auto buf = new moduru::lang::StringBuilder(bufLen);
    deepToString(a, buf, new HashSet());
    return buf->toString();
}

void Arrays::deepToString(vector<moduru::lang::Object*>* a, moduru::lang::StringBuilder* buf, Set* dejaVu)
{

    if(a == nullptr) {
        buf->append(u"null"_j);
        return;
    }
    auto iMax = a->size() - int32_t(1);
    if(iMax == -int32_t(1)) {
        buf->append(u"[]"_j);
        return;
    }
    dejaVu->add(static_cast< moduru::lang::Object* >(a));
    buf->append(u'[');
    for (auto i = int32_t(0); ; i++) {
        auto element = (*a)[i];
        if(element == nullptr) {
            buf->append(u"null"_j);
        } else {
            auto eClass = element->getClass();
            if(npc(eClass)->isArray_()) {
                if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(int8_tArray::class_()))
                    buf->append(toString(dynamic_cast< vector<int8_t>* >(element)));
                else if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(int16_tArray::class_()))
                    buf->append(toString(dynamic_cast< vector<int16_t>* >(element)));
                else if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(int32_tArray::class_()))
                    buf->append(toString(dynamic_cast< vector<int32_t>* >(element)));
                else if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(int64_tArray::class_()))
                    buf->append(toString(dynamic_cast< vector<int64_t>* >(element)));
                else if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(char16_tArray::class_()))
                    buf->append(toString(dynamic_cast< vector<char16_t>* >(element)));
                else if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(floatArray::class_()))
                    buf->append(toString(dynamic_cast< vector<float>* >(element)));
                else if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(doubleArray::class_()))
                    buf->append(toString(dynamic_cast< vector<double>* >(element)));
                else if(static_cast< moduru::lang::Object* >(eClass) == static_cast< moduru::lang::Object* >(boolArray::class_()))
                    buf->append(toString(dynamic_cast< vector<bool>* >(element)));
                else {
                    if(dejaVu->contains(element))
                        buf->append(u"[...]"_j);
                    else
                        deepToString(dynamic_cast< vector<moduru::lang::Object*>* >(element), buf, dejaVu);
                }
            } else {
                buf->append(element->toString());
            }
        }
        if(i == iMax)
            break;

        buf->append(u", "_j);
    }
    buf->append(u']');
    dejaVu->remove(static_cast< moduru::lang::Object* >(a));
}
*/
