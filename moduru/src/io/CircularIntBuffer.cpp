#include <io/CircularIntBuffer.hpp>

#include <math/Math.hpp> 

#include <VecUtil.hpp>

using namespace moduru::io;
using namespace moduru::math;
using namespace std;

CircularIntBuffer::CircularIntBuffer(int nSize, bool bBlockingRead, bool bBlockingWrite) {
	m_bBlockingRead = bBlockingRead;
	m_bBlockingWrite = bBlockingWrite;
	m_nSize = nSize;
	m_abData = vector<int>(m_nSize);
	m_lReadPos = 0;
	m_lWritePos = 0;
	m_bOpen = true;
}

void CircularIntBuffer::close()
{
	m_bOpen = false;
}

bool CircularIntBuffer::isOpen()
{
	return m_bOpen;
}


int CircularIntBuffer::availableRead()
{
	return (int)(m_lWritePos - m_lReadPos);
}



int CircularIntBuffer::availableWrite()
{
	return m_nSize - availableRead();
}



int CircularIntBuffer::getReadPos()
{
	return (int)(m_lReadPos % m_nSize);
}



int CircularIntBuffer::getWritePos()
{
	return (int)(m_lWritePos % m_nSize);
}



int CircularIntBuffer::read(vector<int>* abData)
{
	return read(abData, 0, abData->size());
}



int CircularIntBuffer::read(vector<int>* abData, int nOffset, int nLength)
{
	if (!isOpen())
	{
		if (availableRead() > 0)
		{
			nLength = Math::min(nLength, availableRead());
		}
		else
		{
			return -1;
		}
	}
	std::lock_guard<std::mutex> guard(this->Mutex);
	{
		if (!m_bBlockingRead)
		{
			nLength = Math::min(availableRead(), nLength);
		}
		int nRemainingBytes = nLength;
		while (nRemainingBytes > 0)
		{
			while (availableRead() == 0)
			{
				try
				{
					std::unique_lock<std::mutex> lck(Mutex);
					cv.wait(lck);
				}
				catch (exception* e)
				{
					//string msg = e->what();
				}
			}
			int	nAvailable = Math::min(availableRead(), nRemainingBytes);
			while (nAvailable > 0)
			{
				int	nToRead = Math::min(nAvailable, m_nSize - getReadPos());
				moduru::VecUtil::VecCopy(&m_abData, getReadPos(), abData, nOffset, nToRead);
				m_lReadPos += nToRead;
				nOffset += nToRead;
				nAvailable -= nToRead;
				nRemainingBytes -= nToRead;
			}
			cv.notify_all();
		}
		return nLength;
	}
}


int CircularIntBuffer::write(vector<int> abData)
{
	return write(abData, 0, abData.size());
}



int CircularIntBuffer::write(vector<int> abData, int nOffset, int nLength)
{
	std::lock_guard<std::mutex> guard(this->Mutex);
	{
		if (!m_bBlockingWrite)
		{
			nLength = Math::min(availableWrite(), nLength);
		}
		int nRemainingBytes = nLength;
		while (nRemainingBytes > 0)
		{
			while (availableWrite() == 0)
			{
				try
				{
					std::unique_lock<std::mutex> lck(Mutex);
					cv.wait(lck);
				}
				catch (exception* e)
				{
					//string msg = e->what();
				}
			}
			int	nAvailable = Math::min(availableWrite(), nRemainingBytes);
			while (nAvailable > 0)
			{
				int	nToWrite = Math::min(nAvailable, m_nSize - getWritePos());
				moduru::VecUtil::VecCopy(&abData, nOffset, &m_abData, getWritePos(), nToWrite);
				m_lWritePos += nToWrite;
				nOffset += nToWrite;
				nAvailable -= nToWrite;
				nRemainingBytes -= nToWrite;
			}
			cv.notify_all();
		}
		return nLength;
	}
}

int CircularIntBuffer::read() {
	vector<int> tmp(1);
	read(&tmp);
	return tmp[0];
}
