#include "core/util/datablob.h"
#include "core/string/stringutils.h"
#include "core/io/serialisable.h"

namespace Cat {

	DataBlob::DataBlob(Size bufferSize)
		: m_pBuffer(NIL), m_pData(NIL), m_pName(NIL),
		  m_bufferSize(bufferSize), m_dataSize(0), m_oid(0),
		  m_readIdx(0), m_writeIdx(0), m_bufferIdx(0),
		  m_storedBytes(0),
		  m_canRead(true), m_canWrite(true) {
		
		if (m_bufferSize > 0) {
			m_pBuffer = new Byte[m_bufferSize];
		}		
	}

	DataBlob::DataBlob(const Char* name, Size bufferSize)
		: m_pBuffer(NIL), m_pData(NIL), m_pName(NIL),
		  m_bufferSize(bufferSize), m_dataSize(0), m_oid(0),
		  m_readIdx(0), m_writeIdx(0), m_bufferIdx(0),
		  m_storedBytes(0),
		  m_canRead(true), m_canWrite(true) {

		if (m_bufferSize > 0) {
			m_pBuffer = new Byte[m_bufferSize];
		}	
		m_pName = StringUtils::copy(name);
		m_oid = crc32(name);
	}

	DataBlob::~DataBlob() {
		if (m_pBuffer) {
			delete[] m_pBuffer;
			m_pBuffer = NIL;
		}

		if (m_pData) {
			delete[] m_pData;
			m_pData = NIL;
		}

		m_pName = StringUtils::free(m_pName);		
	}

	void DataBlob::flush() {
		if (m_bufferIdx != 0) {
			copyBuffer(m_pBuffer, m_bufferIdx);			
			m_bufferIdx = 0;			
		}		
	}

	Size DataBlob::read(VPtr buffer, Size toRead) {
		if (m_readIdx + toRead > m_dataSize) {
			DWARN("Trying to read "
					<< toRead
					<< " from DataBlob, but only "
					<< (m_readIdx - toRead)
					<< " left to read.");			
			toRead = m_dataSize - m_readIdx;
		}		
		memcpy(buffer, &(m_pData[toRead]), toRead);
		m_readIdx += toRead;
		return toRead;		
	}

	Size DataBlob::readObject(Serialisable* object) {
		return object->read(this);		
	}

	Size DataBlob::write(const void* buffer, Size toWrite) {
		if (toWrite >= m_bufferSize) {
			flush();			
			/* Just copy into the data */
			return copyBuffer(buffer, toWrite);			
		}
		/* else, copy into buffer, flushing if need room */		
		else {
			if (m_bufferIdx + toWrite >= m_bufferSize) {
				flush();
			}
			memcpy(&(m_pBuffer[m_bufferIdx]), buffer, toWrite);			
			m_bufferIdx += toWrite;
			return toWrite;
		}
		
	}

	Size DataBlob::writeObject(Serialisable* object) {
		return object->write(this);
	}

	void DataBlob::reserve(Size bytes) {
		if (bytes + m_storedBytes > m_dataSize) {
			if (!m_pData) {
				m_pData = new Byte[bytes];
				m_dataSize = bytes;				
			}
			else {
				Byte* tmp = new Byte[bytes + m_storedBytes];
				memcpy(tmp, m_pData, m_dataSize);
				Byte* tmpDel = m_pData;
				m_pData = tmp;	
				delete[] tmpDel;
				m_dataSize = bytes + m_storedBytes;				
			}
		}
	}
	
	
	Size DataBlob::rewind(Size bytes) {
	   if (m_readIdx < bytes) {
			m_readIdx = bytes;
		}
		else {
			m_readIdx -= bytes;
		}
		
		if (m_writeIdx < bytes) {
			m_writeIdx = bytes;
		}
		else {
			m_writeIdx -= bytes;
		}
		return bytes;		
	}

	Size DataBlob::skip(Size bytes) {
		flush();			
		if (m_readIdx + bytes <= m_storedBytes) {
			m_readIdx += bytes;			
		}
		else {
			m_readIdx = m_storedBytes;
		}

		if (m_writeIdx + bytes <= m_storedBytes) {
			m_writeIdx += bytes;			
		}
		else {
			m_writeIdx = m_storedBytes;
		}
		return bytes;		
	}

	Size DataBlob::copyBuffer(const void* buffer, Size bytes) {
		if (m_writeIdx + bytes > m_dataSize) {
			reserve(bytes);				
		}
		memcpy(&(m_pData[m_writeIdx]), buffer, bytes);
		/** @todo If insert, make sure to move readIdx if needed */
		m_writeIdx += bytes;
		if (m_writeIdx > m_storedBytes) {
			m_storedBytes = m_writeIdx;
		}		
		return bytes;		
	}
	
	
	

} // namespace Cat
