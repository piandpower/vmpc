#include <controls/sampler/window/VeloEnvFilterControls.hpp>

#include <ui/sampler/SamplerGui.hpp>
#include <sampler/NoteParameters.hpp>

using namespace mpc::controls::sampler::window;
using namespace std;

VeloEnvFilterControls::VeloEnvFilterControls(mpc::Mpc* mpc) 
	: AbstractSamplerControls(mpc)
{
}

void VeloEnvFilterControls::turnWheel(int i)
{
    init();
    if (param.compare("attack") == 0) {
		lastNp->setFilterAttack(lastNp->getFilterAttack() + i);
	}
	else if (param.compare("decay") == 0) {
		lastNp->setFilterDecay(lastNp->getFilterDecay() + i);
	}
	else if (param.compare("amount") == 0) {
		lastNp->setFilterEnvelopeAmount(lastNp->getFilterEnvelopeAmount() + i);
	}
	else if (param.compare("velofreq") == 0) {
		lastNp->setVelocityToFilterFrequency(lastNp->getVelocityToFilterFrequency() + i);
	}
	else if (param.compare("note") == 0) {
		samplerGui->setPadAndNote(samplerGui->getPad(), samplerGui->getNote() + i);
	}
}
