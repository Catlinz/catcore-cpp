#ifndef CX_CORE_THREADING_CXPROCESSRUNNER_H
#define CX_CORE_THREADING_CXPROCESSRUNNER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file CxProcessRunner.h: 
 * @brief Contains the definition for the ProcessRunner class.
 *
 * @author Catlin Zilinski
 * @date Nov 6, 2015
 */

#include "core/Cx.h"
#include "core/common/CxMsg.h"
#include "core/common/CxStrID.h"
#include "core/common/CxVector.h"
#include "core/common/CxDualQueue.h"
#include "core/threading/CxProcess.h"
#include "core/threading/CxThread.h"
#include "core/threading/CxConditionVariable.h"

namespace cat {

	/**
	 * @class CxProcessRunner CxProcessRunner.h "core/threading/CxProcessRunner.h"
	 * @brief The Class that is responsible for running CxProcesses in threads.
	 *
	 * The CxProcessRunner provides the means to run multiple processes on
	 * a single, separate thread.
	 *
	 * @since Mar 4, 2014
	 * @version 2
	 * @author Catlin Zilinski
	 */
	class CxProcessRunner : public CxThread {
	  public:
		/** @brief Enum of message types */
		enum Message {
			kQueueProcess = 1 << 0,

			kPauseProcessByPID = 1 << 1,
			kPauseProcessByName = 1 << 2,

			kResumeProcessByPID = 1 << 3,
			kResumeProcessByName = 1 << 4,

			kTerminateProcessByPID = 1 << 5,
			kTerminateProcessByName = 1 << 6,
			kTerminateAllProcesses = 1 << 7,

			kStopRunner = 1 << 8,
		};
		
		/** @brief Null process runner */
		CxProcessRunner();

		/**
		 * @brief Create a new process runner.
		 * @param in_name The name / id of the process runner to create.
		 * @param in_maxProcesses The max # of processes we can have.
		 */
		CxProcessRunner(const CxChar *in_id, CxI32 in_maxProcesses);

		/** @brief Destroys all the processes. */
		~CxProcessRunner();

		/** 
		 * @brief Check whether or not the runner can run a process.
		 * This method will check both the process mask and whether or
		 * not this runner is full.
		 * @return True if the process runner can run the process. 
		 */
		CxBool accepts() const;

		/** @return The number of processes (paused or running) */
		CX_FORCE_INLINE CxI32 count() const { return m_proc.count(); }

		/** @return The process with the specified pid or null. */
		CxProcessPtr getProcess(CxU64 in_pid);

		/** @return The first process with the specified name or null. */
		CxProcessPtr getProcess(const CxChar *in_name);

		/** @return True if there are any paused processes */
		CX_FORCE_INLINE CxBool hasPaused() const { return m_pauseCount > 0; }

		/** @return True if there are any removed processes */
		CX_FORCE_INLINE CxBool hasRemoved() const { return m_removed.count() > 0; }

		/** @return The StrID (name + id) for the process runner. */
	   CX_FORCE_INLINE const CxStrID & id() const { return m_id; }

		/** @return True if the runner is runner its max number of processes. */
		CX_FORCE_INLINE CxBool isFull() const {
			return m_proc.count() == m_maxProcesses;
		}

		/** @return The process mask for this runner. */
		CX_FORCE_INLINE CxI32 mask() const { return m_procMask; }

		/** @return The max number of processes that this runner can run */
		CX_FORCE_INLINE CxI32 maxProcesses() const { return m_maxProcesses; }

		/**
		 * @brief Send a message to pause the specified process.
		 * @param in_pid The pid of the process to pause.
		 * @return True if the message was posted.
		 */
		CxBool pause(CxU64 in_pid);

		/**
		 * @brief Send a message to pause the specified process.
		 * @param in_name The name of the process to pause.
		 * @return True if the message was posted.
		 */
		CxBool pause(const CxChar *in_name);

		/** @return The number of paused processes. */
		CX_FORCE_INLINE CxI32 pauseCount() const { return m_pauseCount; }

		/** @brief Process any recieved messages. */
		void processMessages();

		/**
		 * @brief Send a message to run the specified process on the runner.
		 * This method does not retain the process, it assumes that the 
		 * retain count is already one and that if the caller wants to store
		 * the process it will call retain on it.
		 * @param in_process The process to run on the runner.
		 * @return True if the message was posted.
		 */
		CxBool queue(const CxProcess *in_process);

		/** @brief Post a message telling the runner that it should quit. */
		void quit();

		/**
		 * @brief Send a message to resume the specified process.
		 * @param in_pid The pid of the process to resume.
		 * @return True if the message was posted.
		 */
		CxBool resume(CxU64 in_pid);

		/**
		 * @brief Send a message to resume the specified process.
		 * @param in_name The name of the process to resume.
		 * @return True if the message was posted.
		 */
		CxBool resume(const CxChar *in_name);

		/**
		 * @brief Run loop for the process runner.
		 */
		virtual CxI32 run();

		/**
		 * @brief Set the process mask for this runner.
		 * For a process to be able to run on this process runner, then 
		 * runner->mask() & process->mask must equal process->mask().  
		 * I.e., all the bits that are set in the process->mask() must also
		 * be set in the runner->mask().
		 * @param in_mask The mask to determine which proccesses can run.
		 */
		CX_FORCE_INLINE void setMask(CxI32 in_mask) { m_procMask = in_mask; }

		/**
		 * @brief Send a message to terminate the specified process.
		 * @param in_pid The pid of the process to terminate.
		 * @return True if the message was posted.
		 */
		CxBool terminate(CxU64 in_pid);

		/**
		 * @brief Send a message to terminate the specified process.
		 * @param in_name The name of the process to terminate.
		 * @return True if the message was posted.
		 */
		CxBool terminate(const CxChar *in_name);

		/** @brief Marks all processes for termination, will not stop the runner. */
		CxBool terminateAll();

	  protected:
		CxProcess * findProcessByHash(CxU32 in_hash);
		CxProcess * findProcessByPID(CxU64 in_pid);
		void resumePaused(CxProcess *in_proc);
		void runProcesses(CxI32 in_timeForEachProcess);
		void setToPaused(CxProcess *in_proc);
		void terminateAllProcesses();
		void terminateProcess(CxProcess *in_proc);

		

	  private:
		CxStrID m_id;
		
		CxConditionVariable m_check;

		CxVector<CxProcess *> m_proc;
		CxVector<CxProcess *> m_removed;
		CxDualQueue<CxMsg1<CxMemAddr>, CxMutex> m_msgs;

		CxI32 m_pauseCount;
		CxI32 m_maxProcesses;
		CxI32 m_procMask;

		CX_FORCE_INLINE CxBool priv_post(const CxMsg1<CxMemAddr> &in_msg) {
			CxBool success = false;
			m_msgs.lock();
			success = m_msgs.push(in_msg, kCxNoLock);
			m_check.broadcast();  m_msgs.unlock();
			return success;
		}


	};

	CX_FORCE_INLINE CxBool CxProcessRunner::accepts(const CxProcess *in_p) const {
		const CxI32 p_mask = in_p->mask();
		return (!isFull() && (m_procMask & p_mask) == p_mask);
	}
	
} // namespace cat

#endif // CX_CORE_THREADING_CXPROCESSRUNNER_H
