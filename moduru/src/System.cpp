#include <System.hpp>

#include <chrono>

using namespace std;

using namespace moduru;

long System::nanoTime() {
	auto nanoTime = chrono::high_resolution_clock::now();
	return  (long) chrono::duration_cast<std::chrono::nanoseconds>(nanoTime.time_since_epoch()).count();
}
