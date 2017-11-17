#pragma once

#include <lang/Object.hpp>
#include <util/Comparator.hpp>

namespace moduru {
	namespace lang {

		class String;

		class CaseInsensitiveComparator
			: public virtual Object
			, public virtual moduru::util::Comparator
		{

		public:
			typedef Object super;

		private:
			static constexpr int64_t serialVersionUID{ int64_t(8575799808933029326LL) };

		public:
			virtual int32_t compare(String* s1, String* s2);


			CaseInsensitiveComparator();

		public:
			virtual bool equals(Object* obj);
			virtual int32_t compare(Object* o1, Object* o2) override;

		private:
			friend class String;
		};

	}
}
