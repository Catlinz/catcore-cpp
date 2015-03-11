#ifndef CAT_CORE_THREADING_PROCESSMANAGER_H
#define CAT_CORE_THREADING_PROCESSMANAGER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file processmanager.h: 
 * @brief Contains the definition for the ProcessManager class.
 *
 * @author Catlin Zilinski
 * @date Mar 11, 2014
 */

#include "core/corelib.h"
#include "core/threading/process.h"
#include "core/threading/processrunner.h"
#include "core/util/staticmap.h"

namespace Cat {

	/**
	 * @class ProcessManager processmanager.h "core/threading/processmanager.h"
	 * @brief The Class that is responsible for holding multiple ProcessRunners.
	 *
	 * The ProcessManager provides a means of running multiple ProcessRunners 
	 * simultaneously.
	 *
	 * @since Mar 7, 2014
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class ProcessManager {
	  public:		
		/**
		 * @brief Initializes an empty ProcessManager with no process runners.
		 */
		ProcessManager() { }		

		/**
		 * @brief Create a new Process Manager with the specified number of runners.
		 * @param maxProcessRunners The number of ProcessRunners to run.
		 */
		ProcessManager(Size maxProcessRunners);		

		/**
		 * @brief Makes sure to stop all the process runners.
		 */
		~ProcessManager();

		/**
		 * @brief Create a new ProcessRunner.
		 * @param name The name of the process runner.
		 * @param queueSize the number of processes that can be run on the runner.
		 */
		Boolean createProcessRunner(const Char* name, Size queueSize = 32) {
			if (!m_runners.contains(crc32(name)) && m_runners.size() != m_runners.capacity()) {
				m_runners.insert(crc32(name), new ProcessRunner(name, queueSize));
				return true;				
			}
			else {
#if defined (DEBUG)
				if (m_runners.contains(crc32(name))) {
					DWARN("Cannot add Process Runner " << name << ", already added!");
				}
				else {
					DWARN("Cannot add Process Runner " << name << ", Process Manager full!");
				}
#endif /* DEBUG */
				return false;
			}			
		}

		/**
		 * @brief Get a pointer to the specified process runner.
		 * @param id The id of the process runner.
		 * @return A pointer to the specified process runner.
		 */
		inline ProcessRunner* getProcessRunner(OID id) {
			return m_runners.at(id);			
		}		

		/**
		 * @brief Get a pointer to the specified process Runner.
		 * @param name The name of the process Runner.
		 * @return A pointer to the specified process Runner.
		 */
		inline ProcessRunner* getProcessRunner(const Char* name) {
			return getProcessRunner(crc32(name));
		}

		/**
		 * @brief Find the specified process on the specified process runner.
		 * @param pid The pid of the process.
		 * @param runnerID The ID of the process runner the process is on.
		 * @return A pointer ot the runner or NIL if not found.
		 */
		inline ProcessPtr getProcess(OID pid, OID runnerID) {
			ProcessRunner* runner = m_runners.at(runnerID);
			if (runner) {
				return runner->getProcess(pid);
			}
			else {				
				return ProcessPtr::nullPtr();				
			}
		}
		inline ProcessPtr getProcess(const Char* pname, OID runnerID) {
			return getProcess(crc32(pname), runnerID);
		}
		
		/**
		 * @brief Find the specified process on the specified process runner.
		 * @param pid The pid of the process.
		 * @param runnerName The name of the process runner the process is on.
		 * @return A pointer ot the process or NIL if not found.
		 */
		inline ProcessPtr getProcess(OID pid, const Char* runnerName) {
			return getProcess(pid, crc32(runnerName));
		}
		inline ProcessPtr getProcess(const Char* pname, const Char* runnerName) {
			return getProcess(crc32(pname), crc32(runnerName));
		}

		/**
		 * @brief Find the specified process (looks on all process runner).
		 * @param pid The pid of the process.
		 */
		ProcessPtr getProcess(OID pid);
		
		/**
		 * @brief Find the specified process (looks on all process runners).
		 * @param name The name of the process.
		 */
		inline ProcessPtr getProcess(const Char* name) {
			return getProcess(crc32(name));
		}

		/**
		 * @brief Get the max number of process runners we can have.
		 * @return The max number of process runners we can have.
		 */
		inline U32 maxProcessRunners() const { return m_runners.capacity(); }		

		/**
		 * @brief Get the number of process runners we have.
		 * @return The number of process runners we have.
		 */
		inline U32 numProcessRunners() const { return m_runners.size(); }	

		/**
		 * @brief Pause the specified process on the specified process runner.
		 * @param pid The process ID.
		 * @param runnerID The process Runner ID the process is running on.
		 */
		inline void pauseProcess(OID pid, OID runnerID) {
			ProcessRunner* runner = m_runners.at(runnerID);
			if (runner) {				
				runner->pauseProcess(pid);
			}
			else {
				DWARN("No such process runner with id " << runnerID << ".");
			}
		}
		inline void pauseProcess(const Char* pname, OID runnerID) {
			pauseProcess(crc32(pname), runnerID);		
		}

		/**
		 * @brief Pause the specified process on the specified process runner.
		 * @param pid The process ID.
		 * @param runnerName The process Runner name the process is running on.
		 */
		inline void pauseProcess(OID pid, const Char* runnerName) {
			pauseProcess(pid, crc32(runnerName));
		}
		inline void pauseProcess(const Char* pname, const Char* runnerName) {
			pauseProcess(crc32(pname), crc32(runnerName));		
		}

		/**
		 * @brief Pause the specified process.  
		 * Looks for the process on all runners.
		 * @param pid The process ID of the process to terminate.
		 */
		void pauseProcess(OID pid);
		inline void pauseProcess(const Char* name) {
			return pauseProcess(crc32(name));
		}
		
		/**
		 * @brief Add a new process to the process runner.
		 * @param runnerID The ID of the process runner to run the process on.		 
		 * @param process The process to add to be run.
		 */
		inline Boolean queueProcess(OID runnerID, const ProcessPtr& process) {
			ProcessRunner* runner = m_runners.at(runnerID);
			if (runner) {
				return runner->queueProcess(process);
			} else {
				DWARN("No such Runner with ID " << runnerID << "!");				
				return false;
			}		
		}

		/**
		 * @brief Add a new process to the process runner.
		 * @param runnerName The name of the process runner to run the process on.		 
		 * @param process The process to add to be run.
		 */
		inline Boolean queueProcess(const Char* runnerName, const ProcessPtr& process) {
		   return queueProcess(crc32(runnerName), process);				
		}

		/**
		 * @brief Resume the specified process on the specified process runner.
		 * @param pid The process ID.
		 * @param runnerID The process Runner ID the process is running on.
		 */
		inline void resumeProcess(OID pid, OID runnerID) {
			ProcessRunner* runner = m_runners.at(runnerID);
			if (runner) {				
				runner->resumeProcess(pid);
			}
			else {
				DWARN("No such process runner with id " << runnerID << ".");
			}
		}
		inline void resumeProcess(const Char* pname, OID runnerID) {
			resumeProcess(crc32(pname), runnerID);		
		}

		/**
		 * @brief Resume the specified process on the specified process runner.
		 * @param pid The process ID.
		 * @param runnerName The process Runner name the process is running on.
		 */
		inline void resumeProcess(OID pid, const Char* runnerName) {
			resumeProcess(pid, crc32(runnerName));
		}
		inline void resumeProcess(const Char* pname, const Char* runnerName) {
			resumeProcess(crc32(pname), crc32(runnerName));		
		}

		/**
		 * @brief Resume the specified process.  
		 * Looks for the process on all runners.
		 * @param pid The process ID of the process to terminate.
		 */
		void resumeProcess(OID pid);
		inline void resumeProcess(const Char* name) {
			return resumeProcess(crc32(name));
		}

		/**
		 * @brief Start all the process runners.
		 */
		void startProcessRunners();		

		/**
		 * @brief Terminate all processes on the specified ProcessRunner.
		 * @param runnerID The ID of the process runner to terminate processes on.
		 */
		inline void terminateAllProcesses(OID runnerID) {
			ProcessRunner* runner = m_runners.at(runnerID);
			if (runner) {				
				runner->terminateAllProcesses();
			}
			else {
				DWARN("No such process runner with id " << runnerID << ".");
			}					
		}
		/**
		 * @brief Terminate all processes on the specified ProcessRunner.
		 * @param runnerName The name of the process runner to terminate processes on.
		 */
		inline void terminateAllProcesses(const Char* runnerName) {
		   terminateAllProcesses(crc32(runnerName));			
		}

		/**
		 * @brief Terminate all processes on all process runners.
		 */
		void terminateAllProcesses();

		/**
		 * @brief Terminate all process runners
		 */
		void terminateAllProcessRunners();		

		/**
		 * @brief Terminate the specified ProcessRunner.
		 * @param runnerID The ID of the process runner to terminate.
		 */
		inline void terminateProcessRunner(OID runnerID) {
			ProcessRunner* runner = m_runners.at(runnerID);
			if (runner) {				
				runner->terminateProcessRunner();
			}
			else {
				DWARN("No such process runner with id " << runnerID << ".");
			}			
		}
		
		/**
		 * @brief Terminate the specified ProcessRunner.
		 * @param runnerName The name of the process runner to terminate.
		 */
		inline void terminateProcessRunner(const Char* runnerName) {
			terminateProcessRunner(crc32(runnerName));
		}

		/**
		 * @brief Terminate the specified process on the specified process runner.
		 * @param pid The process ID.
		 * @param runnerID The process Runner ID the process is running on.
		 */
		inline void terminateProcess(OID pid, OID runnerID) {
			ProcessRunner* runner = m_runners.at(runnerID);
			if (runner) {				
				runner->terminateProcess(pid);
			}
			else {
				DWARN("No such process runner with id " << runnerID << ".");
			}
		}
		inline void terminateProcess(const Char* pname, OID runnerID) {
			terminateProcess(crc32(pname), runnerID);		
		}

		/**
		 * @brief Terminate the specified process on the specified process runner.
		 * @param pid The process ID.
		 * @param runnerName The process Runner name the process is running on.
		 */
		inline void terminateProcess(OID pid, const Char* runnerName) {
			terminateProcess(pid, crc32(runnerName));
		}
		inline void terminateProcess(const Char* pname, const Char* runnerName) {
			terminateProcess(crc32(pname), crc32(runnerName));		
		}

		/**
		 * @brief Terminate the specified process.  
		 * Looks for the process on all runners.
		 * @param pid The process ID of the process to terminate.
		 */
		void terminateProcess(OID pid);
		inline void terminateProcess(const Char* name) {
			return terminateProcess(crc32(name));
		}

		/**
		 * @brief Wait for all the process runners to terminate.
		 * @return True if all ProcessRunners have terminated.
		 */
		Boolean waitForAllProcessRunnersToTerminate();		

	  private:
		StaticMap<ProcessRunner*> m_runners;		
				

	};
	
} // namespace Cat

#endif // CAT_CORE_THREADING_PROCESSMANAGER_H


