#pragma once

#include <lang/Object.hpp>
#include <util/Enumeration.hpp>

#include <vector>

namespace moduru {

	namespace lang {
		class String;
	}

	namespace util {

		class StringTokenizer
			: public virtual moduru::lang::Object
			, public virtual Enumeration
		{

		public:
			typedef moduru::lang::Object super;

		private:
			int32_t currentPosition{};
			int32_t newPosition{};
			int32_t maxPosition{};
			moduru::lang::String* str{};
			moduru::lang::String* delimiters{};
			bool retDelims{};
			bool delimsChanged{};
			int32_t maxDelimCodePoint{};
			bool hasSurrogates{ false };
			std::vector<int32_t>* delimiterCodePoints{};
			void setMaxDelimCodePoint();

		private:
			int32_t skipDelimiters(int32_t startPos);
			int32_t scanToken(int32_t startPos);
			bool isDelimiter(int32_t codePoint);

		public:
			virtual bool hasMoreTokens();
			virtual moduru::lang::String* nextToken();
			virtual moduru::lang::String* nextToken(moduru::lang::String* delim);
			bool hasMoreElements() override;
			moduru::lang::Object* nextElement() override;
			virtual int32_t countTokens();

			StringTokenizer(moduru::lang::String* str, moduru::lang::String* delim, bool returnDelims);
			StringTokenizer(moduru::lang::String* str, moduru::lang::String* delim);
			StringTokenizer(moduru::lang::String* str);
		};

	}
}
