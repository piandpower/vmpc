//
//  CoreAudioServer.h
//  vMPC
//
//  Created by Izmael on 9/24/16.
//
//

#ifndef __vMPC__CoreAudioServer__
#define __vMPC__CoreAudioServer__

#include "AudioServer.hpp"

//#include "RtAudio.h"
#include "StereoOutputProcess.hpp"
#include "StereoInputProcess.hpp"
#include "NamedIO.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace ctoot {
    namespace audio {
        namespace server {
            
            class CoreAudioServer
            : public AudioServer {
            
            private:
                //RtAudio* rtAudio{nullptr};
                int outDeviceID;
                int inDeviceID;
                
            public:
				void start() override;
				void stop() override;
				bool isRunning() override;
				void close() override;
				void setClient(std::weak_ptr<AudioClient> client) override;
				std::vector<std::string> getAvailableOutputNames() override;
				std::vector<std::string> getAvailableInputNames() override;
				IOAudioProcess* openAudioOutput(std::string name, std::string label) override;
				IOAudioProcess* openAudioInput(std::string name, std::string glabel) override;
				void closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) override;
				void closeAudioInput(ctoot::audio::server::IOAudioProcess* input) override;
				float getSampleRate() override;
				float getLoad() override;
				int getInputLatencyFrames() override;
				int getOutputLatencyFrames() override;
				int getTotalLatencyFrames() override;
                
            private:
				void detectOutputs();
				void detectInputs();

            private:
                std::vector<NamedIO> availableOutputs;
				std::vector<NamedIO> availableInputs;
            
            private:
				int getDeviceCount();
				std::string getDeviceName(int devIndex);
				int getDeviceIndex(std::string name);
				bool isDeviceProbed(int devIndex);
				int getInputChannelCount(int devIndex);
				int getOutputChannelCount(int devIndex);
				void destroyRtAudio();
				/*static int AudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFrames,
                                         double streamTime, RtAudioStreamStatus status, void *userData);*/
            public:
				void work() override;

			public:
				bool startStream();
				//RtAudio* getRtAudio();

            public:
                CoreAudioServer(int outDeviceID, int inDeviceID);
                
                
            };
            
        }
    }
}


#endif /* defined(__vMPC__CoreAudioServer__) */
