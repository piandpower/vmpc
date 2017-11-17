#pragma once

#include <lang/Object.hpp>
#include <lang/Comparable.hpp>

#include <vector>
#include <cstdint>
#include <set>

namespace moduru {

	namespace lang {
		class String;
	}

	namespace nio {

		class CharBuffer;
		class ByteBuffer;

		namespace charset {

			class CharsetDecoder;
			class CharsetEncoder;

			class Charset
				: public virtual moduru::lang::Object
				, public virtual moduru::lang::Comparable
			{

			public:
				typedef moduru::lang::Object super;

			private:
				static moduru::lang::String* bugLevel_;

			public: /* package */
				static bool atBugLevel(moduru::lang::String* bl);

			private:
				static void checkName(moduru::lang::String* s);
				//static moduru::nio::charset::spi::CharsetProvider* standardProvider_;
				static std::vector<moduru::lang::Object*> cache1_;
				static std::vector<moduru::lang::Object*> cache2_;
				static void cache(moduru::lang::String* charsetName, Charset* cs);
				//static ::java::util::Iterator* providers();
				//static moduru::lang::ThreadLocal* gate_;
				static Charset* lookupViaProviders(moduru::lang::String* charsetName);
				static moduru::lang::Object* extendedProviderLock_;
				static bool extendedProviderProbed_;
				//static moduru::nio::charset::spi::CharsetProvider* extendedProvider_;
				static void probeExtendedProvider();
				static Charset* lookupExtendedCharset(moduru::lang::String* charsetName);
				static Charset* lookup(moduru::lang::String* charsetName);
				static Charset* lookup2(moduru::lang::String* charsetName);

			public:
				static bool isSupported(moduru::lang::String* charsetName);
				static Charset* forName(moduru::lang::String* charsetName);

			private:
				//static void put(::java::util::Iterator* i, ::java::util::Map* m);

			public:
				//static ::java::util::SortedMap* availableCharsets();

			private:
				static Charset* defaultCharset__;

			public:
				static Charset* defaultCharset();

			private:
				moduru::lang::String* name_{};
				std::vector<moduru::lang::String*> aliases_{};
				std::set<moduru::lang::String*> aliasSet{};

			public:
				moduru::lang::String* name();
				std::set<moduru::lang::String*> aliases();
				virtual moduru::lang::String* displayName();
				bool isRegistered();
				//virtual moduru::lang::String* displayName(::java::util::Locale* locale);
				virtual bool contains(Charset* cs) = 0;
				virtual CharsetDecoder* newDecoder() = 0;
				virtual CharsetEncoder* newEncoder() = 0;
				virtual bool canEncode();
				moduru::nio::CharBuffer* decode(moduru::nio::ByteBuffer* bb);
				moduru::nio::ByteBuffer* encode(moduru::nio::CharBuffer* cb);
				moduru::nio::ByteBuffer* encode(moduru::lang::String* str);
				int32_t compareTo(Charset* that);
				int32_t hashCode() override;
				bool equals(moduru::lang::Object* ob) override;
				moduru::lang::String* toString() override;

			public: /* protected */
				Charset(moduru::lang::String* canonicalName, std::vector<moduru::lang::String*>* aliases);

			public:
				virtual int32_t compareTo(Object* o) override;

			private:
				static moduru::lang::String*& bugLevel();
				//static moduru::nio::charset::spi::CharsetProvider*& standardProvider();
				static std::vector<moduru::lang::Object*>& cache1();
				static std::vector<moduru::lang::Object*>& cache2();
				//static moduru::lang::ThreadLocal*& gate();
				//static moduru::lang::Object*& extendedProviderLock();
				//static bool& extendedProviderProbed();
				//static moduru::nio::charset::spi::CharsetProvider*& extendedProvider();
				static Charset*& defaultCharset_();
				//virtual moduru::lang::Class* getClass0();
				//friend class Charset_providers_1;
				//friend class Charset_lookupViaProviders_2;
				//friend class Charset_probeExtendedProvider_3;
				//friend class Charset_availableCharsets_4;
			};

		}
	}
}
