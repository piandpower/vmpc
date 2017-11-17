#pragma once

#include <vector>

namespace moduru {
	namespace io {

		class OutputStream
		{

		public:
			virtual void close() {};
			virtual void flush() {};
			virtual void write(char b) {};
			virtual void write(std::vector<char> b);
			virtual void write(std::vector<char> b, int off, int len);

		};
	}
}
