#ifndef CAT_CORE_SIGNAL_SIGNALEMITTER_H
#define CAT_CORE_SIGNAL_SIGNALEMITTER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *	
 *	@file signalemitter.h
 *	@brief The base class for any class that wants to emit signals.
 *
 * @author Catlin Zilinski
 * @date June 20, 2014
 */

#include "core/util/map.h"
#include "core/util/vector.h"
#include "core/signal/signalhandler.h"

namespace Cat {

	/**
	 * @class SignalEmitter signalemitter.h "core/signal/signalemitter.h"
	 *	@brief The base class for any class that wants to emit signals.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Apr 30, 2014
	 */
	class SignalEmitter {		
	  public:
		/**
		 * @brief Create a new NIL SignalEmitter.
		 */
		SignalEmitter(Size capacity = 8) {
			m_signalMap.initMapWithCapacityAndLoadFactor(capacity, 0.8f, NIL);		
		}

		/**
		 * @brief Destroys the map.
		 */
	   virtual ~SignalEmitter();		

		/**
		 * @brief Connect a signal of this object to the specified handler.
		 * @param name The hashed name of the signal to attach the handler to.
		 * @param handler The SignalHandler to attach to the Signal.
		 * @return True if the SignalHandler was successfully attached.
		 */
		Boolean connect(OID name, const SignalHandler& handler);
		
		/**
		 * @brief Connect a signal of this object to the specified handler.
		 * @param name The hashed name of the signal to attach the handler to.
		 * @param func The static slot method on the object to connect to.
		 * @param obj The object to call the slot on.
		 * @return True if the SignalHandler was successfully attached.
		 */
		inline Boolean connect(OID name,
									  void (*func)(void*, SignalData&),
									  void* obj) {
			return connect(name, SignalHandler(func, obj));
		}		

		/**
		 * @brief Connect a signal of this object to the specified handler.
		 * The Signal names are in the form of 
		 *     signal(ArgType1,ArgType2,...) with NO spaces.
		 * @param name The name of the signal to attach the handler to.
		 * @param handler The SignalHandler to attach to the Signal.
		 * @return True if the SignalHandler was successfully attached.
		 */
		inline Boolean connect(const Char* name,
									  const SignalHandler& handler) {
			return connect(crc32(name), handler);
		}		

		/**
		 * @brief Connect a signal of this object to the specified handler.
		 * The Signal names are in the form of 
		 *     signal(ArgType1,ArgType2,...) with NO spaces.
		 * @param name The name of the signal to attach the handler to.
		 * @param func The static slot method on the object to connect to.
		 * @param obj The object to call the slot on.
		 * @return True if the SignalHandler was successfully attached.
		 */
		inline Boolean connect(const Char* name,
									  void (*func)(void*, SignalData&),
									  void* obj) {
			return connect(crc32(name), SignalHandler(func, obj));
		}	

		/**
		 * @brief disconnect a signal of this object from the specified handler.
		 * @param name The hashed name of the signal to detach the handler from.
		 * @param handler The SignalHandler to detach from the Signal.
		 * @return True if the SignalHandler was successfully detached.
		 */
		Boolean disconnect(OID name, const SignalHandler& handler);

		/**
		 * @brief disconnect a signal of this object from the specified handler.
		 * @see disconnect(const Char*, SignalHandler*)
		 * @param name The hashed name of the signal to detach the handler from.
		 * @param func The static slot method on the object to disconnect.
		 * @param obj The object to disconnect the slot.
		 * @return True if the SignalHandler was successfully detached.
		 */
		inline Boolean disconnect(OID name,
										  void (*func)(void*, SignalData&),
										  void* obj) {
			return disconnect(name, SignalHandler(func, obj));
		}		
		
		/**
		 * @brief disconnect a signal of this object from the specified handler.
		 * @see disconnect(const Char*, SignalHandler*)
		 * @param name The name of the signal to detach the handler from.
		 * @param handler The SignalHandler to detach from the Signal.
		 * @return True if the SignalHandler was successfully detached.
		 */
		inline Boolean disconnect(const Char* name, const SignalHandler& handler) {
			return disconnect(crc32(name), handler);
		}

		/**
		 * @brief disconnect a signal of this object from the specified handler.
		 * @see disconnect(const Char*, SignalHandler*)
		 * @param name The name of the signal to detach the handler from.
		 * @param func The static slot method on the object to disconnect.
		 * @param obj The object to disconnect the slot.
		 * @return True if the SignalHandler was successfully detached.
		 */
		inline Boolean disconnect(const Char* name,
										  void (*func)(void*, SignalData&),
										  void* obj) {
			return disconnect(crc32(name), SignalHandler(func, obj));
		}

		/**
		 * @brief disconnect all signal handlers from the specified signal.
		 * @param signal The hashed name of the signal to detach the handler from.
		 * @return True if there are no handlers attached anymore.
		 */
		Boolean disconnect(OID signal);	

		/**
		 * @brief disconnect all signal handlers from the specified signal.
		 * @param signal The name of the signal to detach the handler from.
		 * @return True if there are no handlers attached anymore.
		 */
		inline Boolean disconnect(const Char* signal) {
			return disconnect(crc32(signal));
		}		

	
	  protected:
		/**
		 * @brief Emit the specified signal.
		 * @param signal The signal to emit.
		 * @param data The SignalData to send with the signal.
		 */
		void emit(OID name, SignalData& data);
		inline void emit(OID name, void* sender) {
			SignalData data(sender);
			emit(name, data);
		}
		inline void emit(OID name, void* sender, void* dataPtr) {
			SignalData data(dataPtr, sender);
			emit(name, data);
		}	

		inline void emit(const Char* name, SignalData& data) {
			emit(crc32(name), data);
		}
		inline void emit(const Char* name, void* sender) {
			SignalData data(sender);
			emit(crc32(name), data);
		}
		inline void emit(const Char* name, void* sender, void* dataPtr) {
			SignalData data(dataPtr, sender);
			emit(crc32(name), data);
		}
		

		Map< Vector<SignalHandler>* > m_signalMap;
		
		
		
	};
	
} // namepsace cc

#endif // CAT_CORE_SIGNAL_SIGNALEMITTER_H
