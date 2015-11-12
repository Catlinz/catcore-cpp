#ifndef CX_CORE_THREADING_CXPROCESSMANAGER_H
#define CX_CORE_THREADING_CXPROCESSMANAGER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file CxProcessManager.h: 
 * @brief Contains the definition for the ProcessManager class.
 *
 * @author Catlin Zilinski
 * @date Nov 5, 2015
 */

#include "core/Cx.h"
#include "core/common/CxHashMap.h"
#include "core/threading/CxProcess.h"

namespace cat {

	class CxProcessRunner;

	/**
	 * @class CxProcessManager CxProcessManager.h "core/threading/CxProcessManager.h"
	 * @brief The Class that is responsible for holding multiple ProcessRunners.
	 *
	 * The CxProcessManager provides a means of running multiple CxProcessRunners 
	 * simultaneously.
	 *
	 * @since Mar 7, 2014
	 * @version 3
	 * @author Catlin Zilinski
	 */
	class CxProcessManager {
	  public:
		/**
		 * @brief Create a new Process Manager with max # of process runners.
		 * @param in_maxRunners The max number of process runners to create.
		 */
		CxProcessManager(CxI32 in_maxRunners);

		/** @brief Stops and destroys all process runners. */
		~CxProcessManager();

		/**
		 * @brief Create a new ProcessRunner with the specified name.
		 * Method will fail if max # of process runners has been reached, 
		 * or process runner with the same name already exists.
		 * @param in_name The name of the process runner.
		 * @param inopt_maxProcesses The max # of processes that can be run.
		 * @return A pointer to the new CxProcessRunner or null if failed.
		 */
		CxProcessRunner * createRunner(const CxChar *in_name, CxI32 inopt_maxProcesses = 32);

		/**
		 * @brief Try and find the specified process by process ID.
		 * This methods searches all the process runners for the specified process.
		 * If it is know which runner the process is running on, you should use
		 * runner(...)->getProcess() instead.
		 * @param in_process The process name/ID.
		 * @return A pointer to the process or null if it is not found.
		 */
		CxProcessPtr getProcess(CxU64 in_process);
		/** @see getProcess(CxU64) */
		CxProcessPtr getProcess(const CxChar *in_process);

		/** @return The max number of process runners we can have. */
		CX_FORCE_INLINE CxI32 maxRunners() const { return m_runners.capacity(); }

		/** @return The number of process runners we have. */
		CX_FORCE_INLINE CxI32 numRunners() const { return m_runners.size(); }

		/**
		 * @brief Try and pause the specified process by its process ID.
		 * This methods searches all the process runners for the specified process.
		 * If it is know which runner the process is running on, you should use
		 * runner(...)->pause() instead.
		 * @param in_process The name/ID of the process to pause.
		 * @return True if the process was found.
		 */
		CxBool pause(CxU64 in_process);
		/** @see pause(CxU64) */
		CxBool pause(const CxChar *in_process);

		/**
		 * @brief Add a new process to any runner that will accept it.
		 * If you know which runner you want it to run on, then use
		 * runner(...)->queue() instead.
		 * @param in_process The process to run.
		 * @return True if there was a runner that will accept the process.
		 */
		CxBool queue(const CxProcess *in_process);

		/**
		 * @brief Try and resume the specified process by its process ID.
		 * This methods searches all the process runners for the specified process.
		 * If it is know which runner the process is running on, you should use
		 * runner(...)->resume() instead.
		 * @param in_process The name/ID of the process to resume.
		 * @return True if the process was found.
		 */
		CxBool resume(CxU64 in_process);
		/** @see resume(CxU64) */
		CxBool resume(const CxChar *in_process);
		

		/** @return The specified CxProcessRunner or null if not found. */
		CX_FORCE_INLINE CxProcessRunner * runner(CxU32 in_hash) {
			return m_runners.value(in_hash, 0);
		}

		/** @return The specified CxProcessRunner or null if not found. */
		CX_FORCE_INLINE CxProcessRunner * runner(const CxChar *in_name) {
			return m_runners.value(in_name, 0);
		}

		/** @brief Start all the process runner threads. */
		void startRunners();

		/** 
		 * @brief Stop all the process runners.
		 * @see stopRunner(CxU64)
		 */
		void stopRunners();

		/**
		 * @brief Stop all the process runners and wait for them to stop.
		 * @see stopRunners()
		 * @return True if all the runners are now stopped.
		 */
		CxBool stopRunnersAndWait();

		/**
		 * @brief Try and terminate the specified process by its process ID.
		 * This methods searches all the process runners for the specified process.
		 * If it is know which runner the process is running on, you should use
		 * runner(...)->terminate() instead.
		 * @param in_process The name/ID of the process to terminate.
		 * @return True if the process was found.
		 */
		CxBool terminate(CxU64 in_process);
		/** @see terminate(CxU64) */
		CxBool terminate(const CxChar *in_process);

		/** @brief Terminate all processes on all process runners. */
		void terminateAll();
		
	  private:
		CxHashMap<CxChar *, CxProcessRunner *> m_runners;
	};
	
} // namespace cat

#endif // CX_CORE_THREADING_CXPROCESSMANAGER_H
