#pragma once
#include <control/CompoundControlChain.hpp>
#include <audio/core/ChannelFormat.hpp>
#include <audio/core/MetaInfo.hpp>
#include <vector>

using namespace ctoot::control;
namespace ctoot {
	namespace audio {
		namespace core {

			class AudioControlsChain
				: public CompoundControlChain
			{

			private:
				std::string sourceLabel{ "" };
				std::string sourceLocation{ "" };
				audio::core::ChannelFormat* constraintChannelFormat { nullptr };

			public:
				virtual void setMetaInfo(std::weak_ptr<MetaInfo> metaInfo);
				virtual std::string getSourceLabel();
				virtual std::string getSourceLocation();
				virtual ChannelFormat* getConstraintChannelFormat();

			public:
				CompoundControl* createControl(std::string name) override;
				int getMaxInstance() override;
				//virtual bool isCompatibleDescriptor(service::ServiceDescriptor* d);

			public:
				//std::vector<ctoot::service::ServiceDescriptor> descriptors() override;
				std::string getPersistenceDomain() override;

				AudioControlsChain(int id, std::string name);
				AudioControlsChain(int id, int index, std::string name, ChannelFormat* constraintFormat);
				virtual ~AudioControlsChain();

			};

		}
	}
}
