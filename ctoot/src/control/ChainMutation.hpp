#pragma once
#include <cstdint>
#include <string>

namespace ctoot {
	namespace control {

		class ChainMutation
		{

		public:
			static const int DELETE{ 1 };
			static const int INSERT{ 2 };
			static const int MOVE{ 3 };
			static const int COMMENCE{ 4 };
			static const int COMPLETE{ 5 };

		private:
			static ChainMutation* COMMENCE_INSTANCE_;
			static ChainMutation* COMPLETE_INSTANCE_;
			int type{};
			int index0 = -1;
			int index1 = -1;

		public:
			virtual int getType();
			virtual int getIndex0();
			virtual int getIndex1();
			std::string toString();

		private:
			std::string typeName();

		public:
			ChainMutation(int type);
			ChainMutation(int type, int index);
			ChainMutation(int type, int index0, int index1);


		public:
			static ChainMutation*& COMMENCE_INSTANCE();
			static ChainMutation*& COMPLETE_INSTANCE();

		};

	}
}
