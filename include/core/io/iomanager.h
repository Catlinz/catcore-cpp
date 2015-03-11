#ifndef CAT_CORE_IO_IOMANAGER_H
#define CAT_CORE_IO_IOMANAGER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file iomanager.h
 * @brief Manages Async I/O Operations. 
 *
 * @author: Catlin Zilinski
 * @date: Oct 17, 2013
 */

#include "core/corelib.h"

namespace Cat {

	class AsyncTaskRunner;

	/**
	 * @class IOManager iomanager.h "core/io/iomanager.h"
	 * @brief A Singleton global class to manage Async I/O Operations.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 17, 2013
	 */
	class IOManager {
		public:
			/**
			 * @brief Initializes the AsyncTaskRunner for the IOManager instance.
			 */
			IOManager();

			/**
			 * @brief Destroyes the AsyncTaskRunner for the IOManager instance.
			 */
			~IOManager();

			/**
			 * @brief Get the AsyncTaskRunner associated with this IOManager.
			 * @return The AsyncTaskRunner to run Async IO Tasks.
			 */
			inline AsyncTaskRunner* getTaskRunner();

			/**
			 * @brief Initializes the IOManager singleton instance.
			 */
			static void initializeIOManagerInstance();

			/**
			 * @brief Destroyes the IOManager singleton instance.
			 */
			static void destroyIOManagerInstance();

			/**
			 * @brief Gets the singletone IOManager object.
			 * @return The IOManager instance.
			 */
			static inline IOManager* getInstance();

		private:
			AsyncTaskRunner*	runner_;	/**< The AsyncTaskRunner to run the Async I/O tasks. */

			static IOManager*	singleton_instance_;
	};

	inline AsyncTaskRunner* IOManager::getTaskRunner() {
		return runner_;
	}

	inline IOManager* IOManager::getInstance() {
		#if defined (DEBUG)
		if (!singleton_instance_) {
			DWARN("Getting singleton instance of UNINITIALIZED IOManager!");
		}
		#endif

		return singleton_instance_;
	}

} // namespace Cat

#endif // CAT_CORE_IO_IOMANAGER_H
