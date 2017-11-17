#pragma once

#include <control/CompoundControl.hpp>
#include <vector>

namespace ctoot {
	namespace control {
		class CompoundControlChain
			: public CompoundControl
		{

		public:
			typedef CompoundControl super;

		public:
			virtual void add(std::shared_ptr<CompoundControl> control);
			virtual void add(int index, std::shared_ptr<CompoundControl> control);
			void insert(std::string insertName, std::string insertBeforeName);
			void insert(Control* controlToInsert, std::string insertBeforeName);

		public:
			virtual CompoundControl* createControl(std::string name);

		public:
			void delete_(std::string deleteName);
			void delete_(int indexToDelete);
			void move(std::string moveName, std::string moveBeforeName);
			void setMutating(bool mutating);
			bool isPluginParent() override;

		public:
			CompoundControlChain(int id, std::string name);
			CompoundControlChain(int id, int index, std::string name);
			virtual ~CompoundControlChain();

		};

	}
}
