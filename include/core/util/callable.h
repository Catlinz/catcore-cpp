#ifndef CAT_CORE_UTIL_CALLABLE_H
#define CAT_CORE_UTIL_CALLABLE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file callable.h
 * @brief Contains a simple Callable interface. 
 *
 * @author Catlin Zilinski
 * @date Feb 23, 2014
 */

#include "core/corelib.h"

namespace Cat {

	/**
	 * @interface Callable callable.h "core/util/callable.h"
	 * @brief An interface for classes to implement to be able to be called like functions.
	 *
	 * The Callable interface defines two methods, call() and call(VPtr) which allow implementing 
	 * classes to be used as things like callbacks by having the exec methods implement the functional
	 * behaviour.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 28, 2013
	 */
	class Callable {
	  public:
		/**
		 * @brief Make sure has virtual destructor.
		 */
		virtual ~Callable() {}	  
		
		/**
		 * @brief Execute the functional behaviour with no arguments
		 */
		virtual void call() {};		

		/**
		 * @brief Execute the functional behaviour with arguments.
		 * @param data Some type of data to execute the function on.
		 */
		virtual void call(VPtr data) { call(); }
	};

} // namespace Cat

#endif // CAT_CORE_UTIL_CALLABLE_H

