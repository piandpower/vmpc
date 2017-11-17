#pragma once
#include <io/OutputStream.hpp>

namespace moduru {
	namespace io {

		class FilterOutputStream
			: public virtual OutputStream
		{

		protected:
			OutputStream* out{};

		public:
			virtual void close() override;
			virtual void flush() override;
			virtual void write(char b) override;
			virtual void write(std::vector<char> b) override;
			virtual void write(std::vector<char> b, int off, int len) override;

			FilterOutputStream(OutputStream* out);
		};

	}
}
