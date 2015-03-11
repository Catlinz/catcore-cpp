#include "core/defer/message.h"
#include <cstring>

namespace Cat {
	Message::Message(I32 p_type, Byte* p_data)
		: m_type(p_type), m_pSender(NIL) {
		m_time = Time::currentTimeNano();
		if (p_data) {
			memcpy(m_pData, p_data, MESSAGE_DATA_SIZE);
		}
		else {
			memset(m_pData, 0, MESSAGE_DATA_SIZE);
		}		
	}

	Message::Message(I32 p_type, void* p_sender, Byte* p_data)
		: m_type(p_type), m_pSender(p_sender) {
		m_time = Time::currentTimeNano();
		if (p_data) {
			memcpy(m_pData, p_data, MESSAGE_DATA_SIZE);
		}
		else {
			memset(m_pData, 0, MESSAGE_DATA_SIZE);
		}			
	}

	void Message::setData(Byte* p_data) {
		if (p_data) {
			memcpy(m_pData, p_data, MESSAGE_DATA_SIZE);
		}
		else {
			memset(m_pData, 0, MESSAGE_DATA_SIZE);
		}
	}	
	
	

} // namespace Cat
