#ifndef CAT_CORE_SIGNAL_SIGNALDATA
#define CAT_CORE_SIGNAL_SIGNALDATA
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file signalhandler.h
 *	@brief A handler to handle emitting signals.
 *
 * @author Catlin Zilinski
 * @date Apr 30, 2014
 */

#include "core/corelib.h"
#include "core/time/time.h"

namespace Cat {

	/**
	 * @class SignalData signalhandler.h "core/signal/signaldata.h"
	 * @brief A class to encapsulate the data sent via a Signal.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Apr 30, 2014
	 */
	class SignalData {
	  public:
		/**
		 * @brief Create a new SignalData object.
		 * @param sender A pointer to the object that emitted the signal.
		 */
		SignalData(void* sender, I32 typeId = 0)
			: m_pData(NIL), m_pSender(sender), m_typeId(typeId) {
			m_time = Time::currentTime();
		}

		/**
		 * @brief Create a new SignalData object.
		 * @param data A pointer to data to send with the signal.
		 * @param sender A pointer to the object that emitted the signal.
		 */
		SignalData(void* data, void* sender, I32 typeId = 0)
			: m_pData(data), m_pSender(sender), m_typeId(typeId) {
			m_time = Time::currentTime();
		}
		
		/**
		 * @brief Create a new SignalData object.	
		 * @param data A pointer to a data block.	 
		 * @param sender A pointer to the object that emitted the signal.		 
		 * @param time The time at which the signal was emitted.
		 * @param typeId The type/id of the Signal sent.
		 */
		SignalData(void* data, void* sender, const TimeVal& time,
					  I32 typeId)
			: m_pData(data), m_pSender(sender),
			  m_time(time), m_typeId(typeId) {}

		/**
		 * @brief Get the sender of the Signal.
		 * @return A pointer to the Signal emitter or NIL.
		 */
		inline void* sender() const { return m_pSender; }

		/**
		 * @brief Get the Data block associated with the signal.
		 * @return A pointer to the data or NIL if no data.
		 */
		inline void* data() const { return m_pData; }		

		/**
		 * @brief Get the time at which the signal was emitted.
		 * @return The time at which the signal was emitted.
		 */
		inline const TimeVal& time() const { return m_time; }

		/**
		 * @brief Get the type/id of the Signal.
		 * @return The type/id of the signal.
		 */
		inline I32 typeID() const { return m_typeId; }		

	  private:
		void* m_pData;
		void* m_pSender;
		TimeVal m_time;		
		I32 m_typeId;
	};
	
} // namepsace cc

#endif // CAT_CORE_SIGNAL_SIGNALDATA
