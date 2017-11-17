#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

namespace moduru {
	namespace io {

		class CircularBuffer {

		private:
			std::mutex Mutex;
			std::condition_variable cv;
			bool m_bBlockingRead;
			bool m_bBlockingWrite;
			std::vector<char> m_abData;
			int m_nSize;
			long m_lReadPos;
			long m_lWritePos;
			bool m_bOpen;
			
		public:
			void close();
			int availableRead();
			int availableWrite();
			int read(std::vector<char>* abData);
			int read(std::vector<char>* abData, int nOffset, int nLength);
			int write(std::vector<char> abData);
			int write(std::vector<char> abData, int nOffset, int nLength);

		private:
			bool isOpen();
			int getReadPos();
			int getWritePos();

		public:
			CircularBuffer(int nSize, bool bBlockingRead, bool bBlockingWrite);
			~CircularBuffer();
		};

	}
}
