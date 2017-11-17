#pragma once

#include <cstdint>

namespace moduru {
	namespace lang {

		class String;

		class Object
		{

		private:
			//static void registerNatives();

		public:
			virtual int32_t hashCode() { return 0; };
			virtual bool equals(Object* obj);

		public:
			Object* clone() {
				return nullptr;
			}

		public:
			virtual String* toString();
			//void notify();
			//void notifyAll();
			//void wait(int64_t timeout);
			void wait(int64_t timeout, int32_t nanos);
			void wait();

		protected:
			virtual void finalize();

		public:
			Object();

		public:
			virtual ~Object();

		};

	}
}
