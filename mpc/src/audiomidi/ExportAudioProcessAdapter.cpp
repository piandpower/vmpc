#include <audiomidi/ExportAudioProcessAdapter.hpp>

//#include <Util.hpp>
//#include <disk/MpcFile.hpp>
//#include <file/wav/WavFile.hpp>
//#include <maingui/StartUp.hpp>
#include <audio/core/AudioFormat.hpp>
#include <audio/core/AudioBuffer.hpp>

#include <file/File.hpp>
#include <file/FileUtil.hpp>
#include <io/FileOutputStream.hpp>
#include <file/ByteUtil.hpp>

#include <chrono>
#include <thread>

using namespace mpc::audiomidi;
using namespace std;
using namespace moduru::io;
using namespace moduru::file;

ExportAudioProcessAdapter::ExportAudioProcessAdapter(ctoot::audio::core::AudioProcess* process, weak_ptr<ctoot::audio::core::AudioFormat> format, string name) : AudioProcessAdapter(process)
{
	reading = false;
	writing = false;
	this->format = format;
	this->name = name;
}

void ExportAudioProcessAdapter::static_eapa(void * args)
{
	static_cast<ExportAudioProcessAdapter*>(args)->run();
}

void ExportAudioProcessAdapter::prepare(moduru::file::File* file, int lengthInFrames)
{
	if (reading || writing) {
		throw std::invalid_argument("Can't setFile() when already exporting");
	}
	circularBuffer.reset();
	circularBuffer = make_unique<CircularBuffer>(100000, true, true);
	lengthInBytes = lengthInFrames * 2 * 2;

	if (lengthInBytes % 2 != 0) {
		lengthInBytes++;
	}

	if (file->exists()) file->del();
	file->create();
	fos1 = new moduru::io::FileOutputStream(file);
	auto written = 0;
	vector<char> buffer(512);

	for (int i = 0; i < 512; i++) {
		buffer[i] = 0;
	}

	while (lengthInBytes - written > 512) {
		fos1->write(buffer);
		written += 512;
	}
	auto remaining = lengthInBytes - written;
	for (int i = 0; i < remaining; i++)
		fos1->write(0);

	raf = fstream(file->getPath().c_str(), ios_base::out | ios_base::in | ios_base::binary);
	this->file = file;
}

void ExportAudioProcessAdapter::start()
{
	if (file == nullptr) {
		throw std::invalid_argument("null export file");
	}
    
	if (reading || writing) return;

    reading = true;
	if (writeThread != nullptr) {
		writeThread->detach();
		delete writeThread;
		writeThread = nullptr;
	}
    writeThread = new thread(&ExportAudioProcessAdapter::static_eapa, this);
}

int ExportAudioProcessAdapter::processAudio(ctoot::audio::core::AudioBuffer* buf, int nFrames)
{
	auto ret = super::processAudio(buf, nFrames);
	if (reading) {
		auto lFormat = format.lock();
		vector<char> ba(buf->getByteArrayBufferSize(lFormat.get(), nFrames));
		buf->convertToByteArray_(0, nFrames, &ba, 0, lFormat.get());
		circularBuffer->write(ba, 0, ba.size());
	}
	return ret;
}

void ExportAudioProcessAdapter::stop()
{
    if (!reading) return;
    reading = false;
}

void ExportAudioProcessAdapter::run()
{
    int written = 0;
    writing = true;
	// how to set thread priority to low?
	while ((reading || circularBuffer->availableRead() > 0) && writing) {

		auto close = false;
		vector<char> ba = vector<char>(circularBuffer->availableRead());
		if (ba.size() + written > lengthInBytes) {
			ba.clear();
			ba.resize(lengthInBytes - written);
			close = true;
		}
		if (ba.size() == 0) {
			this_thread::sleep_for(chrono::milliseconds(1));
			continue;
		}
		circularBuffer->read(&ba);
		raf.seekp(written);
		raf.write(&ba[0], ba.size());
		raf.flush();
		if (close) {
			break;
		}
		written += ba.size();
	}
	writing = false;
    reading = false;
}

void ExportAudioProcessAdapter::writeWav()
{
	auto ints = vector<int>(lengthInBytes / 2);
	int  read = 0;
	int converted = 0;
	auto nonZeroDetected = false;
	while (lengthInBytes - read > 512) {
		raf.seekg(read);
		auto ba = vector<char>(512);
		raf.read(&ba[0], ba.size());
		auto ba2 = vector<char>(2);
		for (int i = 0; i < 512; i += 2) {
			ba2 = vector<char>{ ba[i], ba[i + 1] };
			short value = moduru::file::ByteUtil::bytes2short(ba2);
			ints[converted++] = value;
			if (value != 0) {
				if (!nonZeroDetected)
					nonZeroDetected = true;
			}
		}
		read += 512;
	}
	auto remain = lengthInBytes - read;
	auto remainder = vector<char>(remain);
	raf.seekg(read);
	raf.read(&remainder[0], remain);
	raf.close();
	//file->del();
	for (int i = 0; i < remain; i += 2) {
		auto ba = vector<char>{ remainder[i], remainder[i + 1] };
		auto value = moduru::file::ByteUtil::bytes2short(ba);
		ints[converted++] = value;
		if (value != 0) {
			if (!nonZeroDetected)
				nonZeroDetected = true;
		}
	}

	if (nonZeroDetected) {
		/*
		string sep = FileUtil::getSeparator();
		string wavFileName = maingui::StartUp::home + sep + "vMPC" + sep + "recordings" + sep  + file->getName() + ".WAV";
		auto newFile = new moduru::file::File(wavFileName, nullptr);
		if (newFile->exists()) newFile->del();
		newFile->create();
		auto wavFile = mpc::file::wav::WavFile();
		wavFile.newWavFile(2, ints.size() / 2, 16, 44100);
		wavFile.writeFrames(&ints, ints.size() / 2);
		wavFile.close();
		auto wavBytes = wavFile.getResult();
		fos2 = new moduru::io::FileOutputStream(newFile);
		fos2->write(wavBytes);
		fos2->close();
		newFile->close();
		delete newFile;
		*/
	}
}

ExportAudioProcessAdapter::~ExportAudioProcessAdapter() {
	raf.close();
	if (file != nullptr) {
		file->close();
		delete file;
	}
	if (fos1 != nullptr) delete fos1;
	if (fos2 != nullptr) {
		fos2->close();
		delete fos2;
	}
}
