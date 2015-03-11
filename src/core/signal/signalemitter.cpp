#include "core/signal/signalemitter.h"

namespace Cat {

	SignalEmitter::~SignalEmitter() {
		m_signalMap.eraseAll();
	}	

	Boolean SignalEmitter::connect(OID name, const SignalHandler& handler) {
		Vector<SignalHandler>* handlers = m_signalMap.at(name);
		if (!handlers) {
			handlers = new Vector<SignalHandler>(4);			
			DMSG("Creating new signal handler vector for signal " << name);			
			m_signalMap.insert(name, handlers);
		}
		
		for (Size i = 0; i < handlers->size(); ++i) {
			if (handlers->at(i) == handler) {
				DWARN("Cannot add SignalHandler " << name << " twice!");					
				return false;
			}
		}			
		handlers->append(handler);
		return true;			
	}

	Boolean SignalEmitter::disconnect(OID name, const SignalHandler& handler) {
		Vector<SignalHandler>* handlers = m_signalMap.at(name);
		if (handlers) {
			for (Size i = 0; i < handlers->size(); ++i) {
				if (handlers->at(i) == handler) {
					handlers->removeAt(i);
					return true;
				}
			}
			DWARN("SignalHandler for signal "
					<< name
					<< " already disconnected or never connected.");			
			return false;			
		} else {			
			DWARN("Cannot remove Handler from invalid Signal name "
					<< name
					<< "!");			
			return false;
		}
	}

	Boolean SignalEmitter::disconnect(OID name) {
		Vector<SignalHandler>* handlers = m_signalMap.at(name);
		if (handlers) {
			handlers->clear();			
		} else {			
			DWARN("Cannot remove Handler from non existant signal name "
					<< name
					<< "!");			
			return false;
		}
		return true;		
	}

	void SignalEmitter::emit(OID name, SignalData& data) {
		Vector<SignalHandler>* handlers = m_signalMap.at(name);
		if (handlers) {
			for (Size i = 0; i < handlers->size(); ++i) {
				handlers->at(i).call(data);
			}
		}
#if defined (DEBUG)
		else {
			DMSG("No handlers found for signal " << name << ".");
		}
#endif /* DEBUG */
	}	
	

} // namespace Cat
