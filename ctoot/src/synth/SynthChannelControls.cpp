#include <synth/SynthChannelControls.hpp>
#include <control/BooleanControl.hpp>
#include <control/Control.hpp>
#include <control/ControlLaw.hpp>
#include <control/EnumControl.hpp>
#include <control/FloatControl.hpp>
#include <synth/ControlChange.hpp>

//#include <observer/Observable.hpp>
//#include <concurrentqueue.h>


using namespace ctoot::synth;
using namespace ctoot::control;
using namespace std;

SynthChannelControls::SynthChannelControls(int id, string name)
	: CompoundControl(id, name)
{
}

void SynthChannelControls::close()
{
	if (observable != nullptr) {
		observable->deleteObserver(this);
	}
	/*
	if (changeThread != nullptr) {
		changeThread = nullptr;
		{
			//synchronized synchronized_0(this);
			//notify();
		}
	}
	*/
}

void SynthChannelControls::notifyParent(Control* obj)
{
    if(learn) {
        learnControl = obj;
    }
    super::notifyParent(obj);
}

void SynthChannelControls::ensureMapExists()
{
	if (map.size() != 128) map = vector<Control*>(128);
}

void SynthChannelControls::update(moduru::observer::Observable* o, boost::any a)
{

	if (dynamic_cast<ControlChange*>(o) == nullptr) return;
	if (observable == nullptr) observable = o;
	auto change = dynamic_cast<ControlChange*>(o);
	auto controller = change->getController();
	if (controller >= 32 && controller < 64) return;

	if (learn && learnControl != nullptr) {
		ensureMapExists();
		map.insert(map.begin() + controller, learnControl);
		learn = false;
		learnControl = nullptr;
	}
	if (map.size() == 128 && map[controller] != nullptr) {
		//if (changeQueue == nullptr) {
			//changeQueue = new concurrent_queue<ControlChange*>;
			//changeThread = new std::thread(run(), getName() + " CC");
			//changeThread->start();
		//}
		changeQueue.try_enqueue(change);
		//            synchronized synchronized_1(this);
			//notify();
	}
}

/*
void SynthChannelControls::run()
{
    auto thisThread = ::java::lang::Thread::currentThread();
    while (thisThread == changeThread) {
        while (!changeQueue->isEmpty()) {
            relay(dynamic_cast< ControlChange* >(changeQueue->poll()));
        }
        {
//            synchronized synchronized_2(this);
            {
                try {
                    wait();
                } catch (::java::lang::InterruptedException* ie) {
                }
            }
        }
    }
}
*/

void SynthChannelControls::relay(ControlChange* change)
{
	auto controller = change->getController();
	auto value = change->getValue();
	auto c = map[controller];
	if (c == nullptr) return;

	if (dynamic_cast<FloatControl*>(c) != nullptr) {
		auto f = dynamic_cast<FloatControl*>(c);
		f->setIntValue(static_cast<int>((static_cast<float>(value) * f->getLaw()->getResolution() / 128)));
	}
	else if (dynamic_cast<EnumControl*>(c) != nullptr) {
		auto e = dynamic_cast<EnumControl*>(c);
		e->setIntValue(static_cast<int>((static_cast<float>(value) * e->getValues().size() / 128)));
	}
	else if (dynamic_cast<BooleanControl*>(c) != nullptr) {
		auto b = dynamic_cast<BooleanControl*>(c);
		b->setValue(value > 63);
	}
}

int SynthChannelControls::getMappedControlId(int n)
{
	if (map.size() != 128) return -1;
	if (map[n] == nullptr) return -1;
	return map[n]->getId();
}

void SynthChannelControls::setMappedControlId(int n, int cid)
{
	if (n < 0) return;
	if (n > 127) return;
	ensureMapExists();
	map.insert(map.begin() + n, deepFind(cid));
}

bool SynthChannelControls::canLearn()
{
    return true;
}

bool SynthChannelControls::getLearn()
{
    return learn;
}

void SynthChannelControls::setLearn(bool learn)
{
    this->learn = learn;
}

SynthChannelControls::~SynthChannelControls() {
	for (auto& c : map) {
		if (c != nullptr) {
			delete c;
		}
	}
}
