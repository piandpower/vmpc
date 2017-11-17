#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

namespace moduru {
	namespace io {

		class CircularIntBuffer {

		private:
			std::mutex Mutex;
			std::condition_variable cv;
			bool m_bBlockingRead;
			bool m_bBlockingWrite;
			std::vector<int> m_abData;
			int m_nSize;
			long m_lReadPos;
			long m_lWritePos;
			bool m_bOpen;
			
		public:
			void close();
			int availableRead();
			int availableWrite();
			int read();
			int read(std::vector<int>* abData);
			int read(std::vector<int>* abData, int nOffset, int nLength);
			int write(std::vector<int> abData);
			int write(std::vector<int> abData, int nOffset, int nLength);

		private:
			bool isOpen();
			int getReadPos();
			int getWritePos();

		public:
			CircularIntBuffer(int nSize, bool bBlockingRead, bool bBlockingWrite);

		};

	}
}
