#ifndef CAT_CORE_THREADING_TASKMANAGER_H
#define CAT_CORE_THREADING_TASKMANAGER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file taskmanager.h: 
 * @brief Contains the definition for the TaskManager class.
 *
 * @author Catlin Zilinski
 * @date Mar 13, 2014
 */

#include "core/corelib.h"
#include "core/threading/task.h"
#include "core/threading/taskrunner.h"
#include "core/util/staticmap.h"

namespace Cat {

	/**
	 * @class TaskManager taskmanager.h "core/threading/taskmanager.h"
	 * @brief The Class that is responsible for holding multiple TaskRunners.
	 *
	 * The TaskManager provides a means of running multiple TaskRunners 
	 * simultaneously.
	 *
	 * @since Mar 13, 2014
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class TaskManager {
	  public:		
		/**
		 * @brief Initializes an empty TaskManager with no task runners.
		 */
		TaskManager() { }		

		/**
		 * @brief Create a new Task Manager with the specified number of runners.
		 * @param maxTaskRunners The number of TaskRunners to run.
		 */
		TaskManager(Size maxTaskRunners);		

		/**
		 * @brief Makes sure to stop all the task runners.
		 */
		~TaskManager();

		/**
		 * @brief Create a new TaskRunner.
		 * @param name The name of the task runner.
		 * @param queueSize the number of taskes that can be run on the runner.
		 */
		Boolean createTaskRunner(const Char* name, Size queueSize = 32) {
			if (!m_runners.contains(crc32(name)) && m_runners.size() != m_runners.capacity()) {
				m_runners.insert(crc32(name), new TaskRunner(name, queueSize));
				return true;				
			}
			else {
#if defined (DEBUG)
				if (m_runners.contains(crc32(name))) {
					DWARN("Cannot add Task Runner " << name << ", already added!");
				}
				else {
					DWARN("Cannot add Task Runner " << name << ", Task Manager full!");
				}
#endif /* DEBUG */
				return false;
			}			
		}

		/**
		 * @brief Get a pointer to the specified task runner.
		 * @param id The id of the task runner.
		 * @return A pointer to the specified task runner.
		 */
		inline TaskRunner* getTaskRunner(OID id) {
			return m_runners.at(id);			
		}		

		/**
		 * @brief Get a pointer to the specified task Runner.
		 * @param name The name of the task Runner.
		 * @return A pointer to the specified task Runner.
		 */
		inline TaskRunner* getTaskRunner(const Char* name) {
			return getTaskRunner(crc32(name));
		}

		/**
		 * @brief Get the max number of task runners we can have.
		 * @return The max number of task runners we can have.
		 */
		inline U32 maxTaskRunners() const { return m_runners.capacity(); }		

		/**
		 * @brief Get the number of task runners we have.
		 * @return The number of task runners we have.
		 */
		inline U32 numTaskRunners() const { return m_runners.size(); }	

		/**
		 * @brief Add a new task to the task runner.
		 * @param runnerID The ID of the task runner to run the task on.		 
		 * @param task The task to add to be run.
		 * @return A pointer to the task if succeeded, or false otherwise.
		 */
		inline TaskPtr queueTask(OID runnerID, const TaskPtr& task) {
			TaskRunner* runner = m_runners.at(runnerID);
			if (runner) {
				return runner->queueTask(task);
			} else {
				DWARN("No such Runner with ID " << runnerID << "!");				
				return TaskPtr::nullPtr();
			}		
		}
		
		/**
		 * @brief Add a new task to the task runner.
		 * @param runnerName The name of the task runner to run the task on.		 
		 * @param task The task to add to be run.
		 * @return The queued Task, or NIL if failed to queue task.
		 */
		inline TaskPtr queueTask(const Char* runnerName, const TaskPtr& task) {
		   return queueTask(crc32(runnerName), task);				
		}

		/**
		 * @brief Add a new task to a task runner.
		 * This method chooses a task runner to run the task on.	 
		 * @param task The task to add to be run.
		 * @return A pointer to the task if succeeded, or false otherwise.
		 */
		TaskPtr queueTask(const TaskPtr& task);

		/**
		 * @brief Start all the task runners.
		 */
		void startTaskRunners();		

		/**
		 * @brief Clears all queued tasks on a specific task runner.
		 * @param runnerID The ID of the task runner to clear waiting tasks on.
		 */
		inline void clearAllWaitingTasks(OID runnerID) {
			TaskRunner* runner = m_runners.at(runnerID);
			if (runner) {				
				runner->clearAllWaitingTasks();
			}
			else {
				DWARN("No such task runner with id " << runnerID << ".");
			}					
		}
		/**
		 * @brief Clears all queued tasks on a specific task runner.
		 * @param runnerName The name of the task runner to clear waiting taskes on.
		 */
		inline void clearAllWaitingTasks(const Char* runnerName) {
		   clearAllWaitingTasks(crc32(runnerName));			
		}

		/**
		 * @brief Clear all waiting tasks on all task runners.
		 */
		void clearAllWaitingTasks();

		/**
		 * @brief Terminate all task runners
		 */
		void terminateAllTaskRunners();		

		/**
		 * @brief Terminate the specified TaskRunner.
		 * @param runnerID The ID of the task runner to terminate.
		 */
		inline void terminateTaskRunner(OID runnerID) {
			TaskRunner* runner = m_runners.at(runnerID);
			if (runner) {				
				runner->terminateTaskRunner();
			}
			else {
				DWARN("No such task runner with id " << runnerID << ".");
			}			
		}
		
		/**
		 * @brief Terminate the specified TaskRunner.
		 * @param runnerName The name of the task runner to terminate.
		 */
		inline void terminateTaskRunner(const Char* runnerName) {
			terminateTaskRunner(crc32(runnerName));
		}

		/**
		 * @brief Wait for all the task runners to terminate.
		 * @return True if all TaskRunners have terminated.
		 */
		Boolean waitForAllTaskRunnersToTerminate();		

	  private:
		StaticMap<TaskRunner*> m_runners;		
				

	};
	
} // namespace Cat

#endif // CAT_CORE_THREADING_TASKMANAGER_H


