#pragma once

namespace ctoot {
	namespace synth {

		struct MidiChannel
		{

			virtual void allNotesOff() = 0;
			virtual void allSoundOff() = 0;
			virtual void controlChange(int controller, int value) = 0;
			virtual int getChannelPressure() = 0;
			virtual int getController(int controller) = 0;
			virtual bool getMono() = 0;
			virtual bool getMute() = 0;
			virtual bool getOmni() = 0;
			virtual int getPitchBend() = 0;
			virtual int getPolyPressure(int noteNumber) = 0;
			virtual int getProgram() = 0;
			virtual bool getSolo() = 0;
			virtual bool localControl(bool on) = 0;
			virtual void noteOff(int noteNumber) = 0;
			virtual void noteOff(int noteNumber, int velocity) = 0;
			virtual void noteOn(int noteNumber, int velocity) = 0;
			virtual void programChange(int program) = 0;
			virtual void programChange(int bank, int program) = 0;
			virtual void resetAllControllers() = 0;
			virtual void setChannelPressure(int pressure) = 0;
			virtual void setMono(bool on) = 0;
			virtual void setMute(bool mute) = 0;
			virtual void setOmni(bool on) = 0;
			virtual void setPitchBend(int bend) = 0;
			virtual void setPolyPressure(int noteNumber, int pressure) = 0;
			virtual void setSolo(bool soloState) = 0;

		};
	}
}
