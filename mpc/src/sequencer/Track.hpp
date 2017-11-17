#pragma once

#include <Mpc.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Event.hpp>
#include <sequencer/NoteEvent.hpp>
#include <observer/Observable.hpp>

#include <thirdp/concurrentqueue.h>
#include <memory>

namespace mpc {
	namespace sequencer {

		class Track
			: public moduru::observer::Observable
		{

		private:
			static const int MAX_TICK{ 2147483647 };

			std::vector<std::shared_ptr<Event>> events{};

			moodycamel::ConcurrentQueue<std::shared_ptr<NoteEvent>> queuedNoteOnEvents{};
			moodycamel::ConcurrentQueue<std::weak_ptr<NoteEvent>> noteOffs{};

			mpc::sequencer::Sequence* parent{ nullptr };
			Mpc* mpc{ nullptr };
			std::weak_ptr<Sequencer> sequencer{};
			int busNumber{ 0 };
			std::string name{ "" };
			bool on{ false };
			int velocityRatio{ 0 };
			int programChange{ 0 };
			int device{ 0 };
			int trackIndex{ 0 };
			bool used{ false };
			int relativeTick{ 0 };
			int eventIndex{ 0 };
			std::weak_ptr<NoteEvent> lastAdded{};
			bool eventAvailable{ false };
			std::weak_ptr<Event> event {};
			bool multi{ false };
			bool delete_{ false };
			int tcValue{ 0 };
			std::vector<std::weak_ptr<Event>> tempEvents{};

		protected:
			static bool tickCmp(std::weak_ptr<Event> a, std::weak_ptr<Event> b);
			static bool noteCmp(std::weak_ptr<NoteEvent> a, std::weak_ptr<NoteEvent> b);

		public:
			void move(int tick, int oldTick);
			void setTrackIndex(int i);
			int getTrackIndex();
			std::weak_ptr<mpc::sequencer::NoteEvent> recordNoteOn();
			void flushNoteCache();
			void recordNoteOff(NoteEvent* n);
			void setUsed(bool b);
			void setOn(bool b);

		private:
			void addEventRealTime(std::shared_ptr<mpc::sequencer::Event> event);

		public:
			std::weak_ptr<Event> addEvent(int tick, std::string type);
			std::weak_ptr<Event> cloneEvent(std::weak_ptr<Event> src);
			void adjustDurLastEvent(int newDur);
			void removeEvent(int i);
			void removeEvent(std::weak_ptr<Event> event);
			void removeEvents();
			void setVelocityRatio(int i);
			int getVelocityRatio();
			void setProgramChange(int i);
			int getProgramChange();
			void setBusNumber(int i);
			int getBusNumber();
			void setDeviceNumber(int i);
			int getDevice();
			std::weak_ptr<Event> getEvent(int i);
			void setName(std::string s);
			std::string getName();
			std::vector<std::weak_ptr<Event>> getEvents();

			int getNextTick();
			void playNext();
			void off(bool stop);
			bool isOn();
			bool isUsed();

		public:
			void setEventIndex(int i);

		public:
			std::vector<std::weak_ptr<Event>> getEventRange(int startTick, int endTick);
			void correctTimeRange(int startPos, int endPos, int stepLength);

		private:
			void timingCorrect(int fromBar, int toBar, NoteEvent* noteEvent, int stepLength);

		public:
			void removeDoubles();
			void sortEvents();
			std::vector<std::weak_ptr<NoteEvent>> getNoteEvents();

		private:
			std::vector<std::weak_ptr<NoteEvent>> getNoteEventsAtTick(int tick);

		public:
			void sortEventsByNotePerTick();

		private:
			void sortEventsOfTickByNote(std::vector<std::weak_ptr<NoteEvent>> noteEvents);

		public:
			void swing(int noteValue, int percentage, std::vector<int> noteRange);
			void swing(std::vector<std::weak_ptr<Event>> eventsToSwing, int noteValue, int percentage, std::vector<int> noteRange);
			void shiftTiming(bool later, int amount, int lastTick);
			void shiftTiming(std::vector<std::weak_ptr<Event>> eventsToShift, bool later, int amount, int lastTick);

		public:
			int getEventIndex();

		public:
			std::string getActualName();

			Track(mpc::sequencer::Sequence* parent, Mpc* mpc, int i);
			Track(Mpc* mpc, int i);
			~Track();

		private:
			friend class Sequence;

		};
	}
}
