#include <nio/charset/Charset.hpp>

//#include <io/Serializable.hpp>
//#include <lang/ArrayStoreException.hpp>
#include <lang/CharSequence.hpp>
//#include <lang/Class.hpp>
//#include <lang/ClassCastException.hpp>
#include <lang/Comparable.hpp>
//#include <lang/Error.hpp>
//#include <lang/IllegalArgumentException.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/Object.hpp>
#include <lang/String.hpp>
//#include <lang/ThreadLocal.hpp>
//#include <lang/Throwable.hpp>
//#include <nio/ByteBuffer.hpp>
//#include <nio/CharBuffer.hpp>
//#include <nio/charset/CharacterCodingException.hpp>
//#include <nio/charset/Charset_providers_1.hpp>
//#include <nio/charset/Charset_lookupViaProviders_2.hpp>
//#include <nio/charset/Charset_probeExtendedProvider_3.hpp>
//#include <nio/charset/Charset_availableCharsets_4.hpp>
//#include <nio/charset/CharsetDecoder.hpp>
//#include <nio/charset/CharsetEncoder.hpp>
//#include <nio/charset/CodingErrorAction.hpp>
//#include <nio/charset/IllegalCharsetNameException.hpp>
//#include <nio/charset/UnsupportedCharsetException.hpp>
//#include <nio/charset/spi/CharsetProvider.hpp>
//#include <security/AccessController.hpp>
//#include <security/PrivilegedAction.hpp>
//#include <util/Collections.hpp>
//#include <util/HashSet.hpp>
//#include <util/Iterator.hpp>
//#include <util/Map.hpp>
//#include <util/Set.hpp>
//#include <util/SortedMap.hpp>
//#include <sun/misc/VM.hpp>
//#include <sun/nio/cs/StandardCharsets.hpp>
//#include <sun/nio/cs/ThreadLocalCoders.hpp>
//#include <sun/security/action/GetPropertyAction.hpp>
//#include <SubArray.hpp>
//#include <ObjectArray.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::nio::charset;
using namespace std;

Charset::Charset(moduru::lang::String* canonicalName, vector<moduru::lang::String*>* aliases)
{
	checkName(canonicalName);
	auto as = (aliases == nullptr) ? new vector<moduru::lang::String*>(int32_t(0)) : aliases;
	for (auto i = int32_t(0); i < as->size(); i++)
		checkName((*as)[i]);

	this->name_ = canonicalName;
	this->aliases_ = *as;
}

moduru::lang::String*& Charset::bugLevel()
{
    return bugLevel_;
}

bool Charset::atBugLevel(moduru::lang::String* bl)
{
	if (bugLevel_ == nullptr) {
		//if(!::sun::misc::VM::isBooted())
		//            return false;

		//bugLevel_ = dynamic_cast< moduru::lang::String* >(moduru::security::AccessController::doPrivileged(static_cast< ::java::security::PrivilegedAction* >(new ::sun::security::action::GetPropertyAction(u"sun.nio.cs.bugLevel"_j))));
		if (bugLevel_ == nullptr)
			bugLevel_ = new moduru::lang::String("");

	}
	return (bugLevel_ != nullptr) && bugLevel_->equals(static_cast<moduru::lang::Object*>(bl));
}

void Charset::checkName(moduru::lang::String* s)
{

	auto n = s->length();
    auto str = moduru::lang::String("1.4");
	if (!atBugLevel(&str)) {
		if (n == 0)
			//throw new IllegalCharsetNameException(s);
			throw std::invalid_argument("IllegalCharsetNameException");
	}
	for (auto i = int32_t(0); i < n; i++) {
		auto c = s->charAt(i);
		if (c >= u'A' && c <= u'Z')
			continue;

		if (c >= u'a' && c <= u'z')
			continue;

		if (c >= u'0' && c <= u'9')
			continue;

		if (c == u'-' && i != 0)
			continue;

		if (c == u':' && i != 0)
			continue;

		if (c == u'_' && i != 0)
			continue;

		if (c == u'.' && i != 0)
			continue;

		//throw new IllegalCharsetNameException(s);
		throw std::invalid_argument("IllegalCharsetNameException");
	}
}

/*
moduru::nio::charset::spi::CharsetProvider*& Charset::standardProvider()
{

    return standardProvider_;
}
moduru::nio::charset::spi::CharsetProvider* Charset::standardProvider_;
*/


vector<moduru::lang::Object*>& Charset::cache1()
{

    return cache1_;
}
vector<moduru::lang::Object*> Charset::cache1_;

vector<moduru::lang::Object*>& Charset::cache2()
{

    return cache2_;
}
vector<moduru::lang::Object*> Charset::cache2_;


void Charset::cache(moduru::lang::String* charsetName, Charset* cs)
{
	cache2_ = cache1_;
	cache1_ = vector<moduru::lang::Object*>({
		static_cast<moduru::lang::Object*>(charsetName)
		, static_cast<moduru::lang::Object*>(cs)
	});
}

/*
java::util::Iterator* Charset::providers()
{

    return new Charset_providers_1();
}

java::lang::ThreadLocal*& Charset::gate()
{

    return gate_;
}
java::lang::ThreadLocal* Charset::gate_;
*/

Charset* Charset::lookupViaProviders(moduru::lang::String* charsetName)
{
    /*
    if(!::sun::misc::VM::isBooted())
        return nullptr;

    if(dynamic_cast< moduru::lang::Object* >(gate_->get()) != nullptr)
        return nullptr;

    {
        auto finally0 = finally([&] {
            gate_->set(nullptr);
        });
        {
            gate_->set(gate_);
            return dynamic_cast< Charset* >(::java::security::AccessController::doPrivileged(static_cast< ::java::security::PrivilegedAction* >(new Charset_lookupViaProviders_2(charsetName))));
        }
    }
	*/
	return nullptr;
}

/*
java::lang::Object*& Charset::extendedProviderLock()
{

    return extendedProviderLock_;
}
java::lang::Object* Charset::extendedProviderLock_;

bool& Charset::extendedProviderProbed()
{

    return extendedProviderProbed_;
}
bool Charset::extendedProviderProbed_;

moduru::nio::charset::spi::CharsetProvider*& Charset::extendedProvider()
{

    return extendedProvider_;
}
moduru::nio::charset::spi::CharsetProvider* Charset::extendedProvider_;
*/

void Charset::probeExtendedProvider()
{

    //::java::security::AccessController::doPrivileged(static_cast< ::java::security::PrivilegedAction* >(new Charset_probeExtendedProvider_3()));
}

Charset* Charset::lookupExtendedCharset(moduru::lang::String* charsetName)
{
/*
    ::moduru::nio::charset::spi::CharsetProvider* ecp = nullptr;
    {
        synchronized synchronized_0(extendedProviderLock_);
        {
            if(!extendedProviderProbed_) {
                probeExtendedProvider();
                extendedProviderProbed_ = true;
            }
            ecp = extendedProvider_;
        }
    }
    return (ecp != nullptr) ? ecp->charsetForName(charsetName) : static_cast< Charset* >(nullptr);
	*/
	return nullptr;
}

Charset* Charset::lookup(moduru::lang::String* charsetName)
{

	if (charsetName == nullptr)
		//throw new moduru::lang::IllegalArgumentException(u"Null charset name"_j);
		throw std::invalid_argument("Illegal argument");

	vector<moduru::lang::Object*>* a;
	if ((a = &cache1_) != nullptr && charsetName->equals((*a)[int32_t(0)]))
		return dynamic_cast<Charset*>((*a)[int32_t(1)]);

	return lookup2(charsetName);
}

Charset* Charset::lookup2(moduru::lang::String* charsetName)
{

	vector<moduru::lang::Object*>* a;
	if ((a = &cache2_) != nullptr && charsetName->equals((*a)[int32_t(0)])) {
		cache2_ = cache1_;
		cache1_ = *a;
		return dynamic_cast<Charset*>((*a)[int32_t(1)]);
	}
	Charset* cs;
	/*
	if((cs = standardProvider_->charsetForName(charsetName)) != nullptr || (cs = lookupExtendedCharset(charsetName)) != nullptr || (cs = lookupViaProviders(charsetName)) != nullptr) {
		cache(charsetName, cs);
		return cs;
	}
	*/
	checkName(charsetName);
	return nullptr;
}

bool Charset::isSupported(moduru::lang::String* charsetName)
{

    return (lookup(charsetName) != nullptr);
}

Charset* Charset::forName(moduru::lang::String* charsetName)
{

    auto cs = lookup(charsetName);
    if(cs != nullptr)
        return cs;

    //throw new UnsupportedCharsetException(charsetName);
	throw std::invalid_argument("unsupported charset exception");
}

/*
void Charset::put(::java::util::Iterator* i, ::java::util::Map* m)
{

    while (npc(i)->hasNext()) {
        auto cs = dynamic_cast< Charset* >(dynamic_cast< moduru::lang::Object* >(npc(i)->next()));
        if(!npc(m)->containsKey(npc(cs)->name()))
            npc(m)->put(npc(cs)->name(), cs);

    }
}

java::util::SortedMap* Charset::availableCharsets()
{

    return dynamic_cast< ::java::util::SortedMap* >(::java::security::AccessController::doPrivileged(static_cast< ::java::security::PrivilegedAction* >(new Charset_availableCharsets_4())));
}
*/

Charset*& Charset::defaultCharset_()
{

    return defaultCharset__;
}
Charset* Charset::defaultCharset__;

Charset* Charset::defaultCharset()
{

	if (defaultCharset__ == nullptr) {
		{
			// synchronized synchronized_1(Charset::class_());
			{
				//auto csn = dynamic_cast<moduru::lang::String*>(::java::security::AccessController::doPrivileged(static_cast<::java::security::PrivilegedAction*>(new ::sun::security::action::GetPropertyAction(u"file.encoding"_j))));
				//auto cs = lookup(csn);

				//if (cs != nullptr) defaultCharset__ = cs;
				//else
                auto str = moduru::lang::String("UTF-8");
				defaultCharset__ = forName(&str);
			}
		}
	}
	return defaultCharset__;
}

moduru::lang::String* Charset::name()
{
	return name_;
}

set<moduru::lang::String*> Charset::aliases()
{
	if (aliasSet.size() != 0)
		return aliasSet;

	auto n = aliases_.size();
	auto hs = set<moduru::lang::String*>();
	for (auto i = int32_t(0); i < n; i++)
		hs.emplace(aliases_[i]);

	//aliasSet = ::java::util::Collections::unmodifiableSet(hs);
	aliasSet = hs;
	return aliasSet;
}

moduru::lang::String* Charset::displayName()
{
    return name_;
}

bool Charset::isRegistered()
{
    auto str1 = moduru::lang::String("X-");
    auto str2 = moduru::lang::String("x-");
    return !name_->startsWith(&str1) && !name_->startsWith(&str2);
}

/*
moduru::lang::String* Charset::displayName(::java::util::Locale* locale)
{
    return name_;
}
*/

bool Charset::canEncode()
{
    return true;
}

moduru::nio::CharBuffer* Charset::decode(::moduru::nio::ByteBuffer* bb)
{
	/*
	try {
        return npc(npc(npc(::sun::nio::cs::ThreadLocalCoders::decoderFor(this))->onMalformedInput(CodingErrorAction::REPLACE()))->onUnmappableCharacter(CodingErrorAction::REPLACE()))->decode(bb);
    } catch (CharacterCodingException* x) {
        throw new moduru::lang::Error(static_cast< moduru::lang::Throwable* >(x));
    }
	*/
	return nullptr;
}

moduru::nio::ByteBuffer* Charset::encode(::moduru::nio::CharBuffer* cb)
{
	/*
    try {
        return npc(npc(npc(::sun::nio::cs::ThreadLocalCoders::encoderFor(this))->onMalformedInput(CodingErrorAction::REPLACE()))->onUnmappableCharacter(CodingErrorAction::REPLACE()))->encode(cb);
    } catch (CharacterCodingException* x) {
        throw new moduru::lang::Error(static_cast< moduru::lang::Throwable* >(x));
    }
	*/
	return nullptr;
}

moduru::nio::ByteBuffer* Charset::encode(moduru::lang::String* str)
{
    //return encode(moduru::nio::CharBuffer::wrap(static_cast< moduru::lang::CharSequence* >(str)));
	return nullptr;
}

int32_t Charset::compareTo(Charset* that)
{
	return (name()->compareToIgnoreCase(that->name()));
}

int32_t Charset::compareTo(Object* o)
{
	if (dynamic_cast<Charset*>(o) == nullptr) return -1;
	return compareTo(dynamic_cast<Charset*>(o));
}

int32_t Charset::hashCode()
{
	return name()->hashCode();
}

bool Charset::equals(moduru::lang::Object* ob)
{
	if (!(dynamic_cast<Charset*>(ob) != nullptr))
		return false;

	if (static_cast<moduru::lang::Object*>(this) == ob)
		return true;

	return name_->equals(static_cast<moduru::lang::Object*>((dynamic_cast<Charset*>(ob))->name()));
}

moduru::lang::String* Charset::toString()
{
    return name();
}

moduru::lang::String* Charset::bugLevel_ = nullptr;
//standardProvider_ = new ::sun::nio::cs::StandardCharsets();
//Charset::cache1_ = nullptr;
//Charset::cache2_ = nullptr;
//gate_ = new moduru::lang::ThreadLocal();
//extendedProviderLock_ = new moduru::lang::Object();
bool Charset::extendedProviderProbed_ = false;
//Charset::extendedProvider_ = nullptr;
