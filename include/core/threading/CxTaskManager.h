#ifndef CX_CORE_THREADING_CXTASKMANAGER_H
#define CX_CORE_THREADING_CXTASKMANAGER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file CxTaskManager.h: 
 * @brief Contains the definition for the TaskManager class.
 *
 * @author Catlin Zilinski
 * @date Nov 12, 2015
 */

#include "core/Cx.h"
#include "core/common/CxHashMap.h"
#include "core/threading/CxTask.h"

namespace cat {

	class CxTaskRunner;

	/**
	 * @class CxTaskManager CxTaskManager.h "core/threading/CxTaskManager.h"
	 * @brief The Class that is responsible for holding multiple TaskRunners.
	 *
	 * The CxTaskManager provides a means of running multiple CxTaskRunners 
	 * simultaneously.
	 *
	 * @since Mar 7, 2014
	 * @version 3
	 * @author Catlin Zilinski
	 */
	class CxTaskManager {
	  public:
		/**
		 * @brief Create a new Task Manager with max # of task runners.
		 * @param in_maxRunners The max number of task runners to create.
		 */
		CxTaskManager(CxI32 in_maxRunners);

		/** @brief Stops and destroys all task runners. */
		~CxTaskManager();

		/**
		 * @brief Create a new TaskRunner with the specified name.
		 * Method will fail if max # of task runners has been reached, 
		 * or task runner with the same name already exists.
		 * @param in_name The name of the task runner.
		 * @return A pointer to the new CxTaskRunner or null if failed.
		 */
		CxTaskRunner * createRunner(const CxChar *in_name);

		/** @return The max number of task runners we can have. */
		CX_FORCE_INLINE CxI32 maxRunners() const { return m_runners.capacity(); }

		/** @return The number of task runners we have. */
		CX_FORCE_INLINE CxI32 numRunners() const { return m_runners.size(); }

		/**
		 * @brief Add a new task to any runner that will accept it.
		 * If you know which runner you want it to run on, then use
		 * runner(...)->queue() instead.
		 * @param in_task The task to run.
		 * @return A pointer to the queued CxTask or null.
		 */
		CxTaskPtr queue(const CxTask *in_task);
		

		/** @return The specified CxTaskRunner or null if not found. */
		CX_FORCE_INLINE CxTaskRunner * runner(CxU32 in_hash) {
			return m_runners.value(in_hash, 0);
		}

		/** @return The specified CxTaskRunner or null if not found. */
		CX_FORCE_INLINE CxTaskRunner * runner(const CxChar *in_name) {
			return m_runners.value(in_name, 0);
		}

		/** @brief Start all the task runner threads. */
		void startRunners();

		/** 
		 * @brief Stop all the task runners.
		 * @see stopRunner(CxU64)
		 */
		void stopRunners();

		/**
		 * @brief Stop all the task runners and wait for them to stop.
		 * @see stopRunners()
		 * @return True if all the runners are now stopped.
		 */
		CxBool stopRunnersAndWait();

	  private:
		CxHashMap<CxChar *, CxTaskRunner *> m_runners;
	};
	
} // namespace cat

#endif // CX_CORE_THREADING_CXTASKMANAGER_H
