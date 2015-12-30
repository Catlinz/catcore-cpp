#ifndef CX_CORE_THREADING_CXTASKRUNNER_H
#define CX_CORE_THREADING_CXTASKRUNNER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file CxTaskRunner.h: 
 * @brief Contains the definition for the TaskRunner class.
 *
 * @author Catlin Zilinski
 * @date Nov 6, 2015
 */

#include "core/Cx.h"
#include "core/common/CxMsg.h"
#include "core/common/CxStrID.h"
#include "core/common/CxQueue.h"
#include "core/common/CxDualQueue.h"
#include "core/threading/CxTask.h"
#include "core/threading/CxThread.h"
#include "core/threading/CxConditionVariable.h"

namespace cat {

	/**
	 * @class CxTaskRunner CxTaskRunner.h "core/threading/CxTaskRunner.h"
	 * @brief The Class that is responsible for running CxTaskes in threads.
	 *
	 * The CxTaskRunner provides the means to run taskes on
	 * a single, separate thread.
	 *
	 * @since Mar 4, 2014
	 * @version 2
	 * @author Catlin Zilinski
	 */
	class CxTaskRunner : public CxThread {
	  public:
		/** @brief Enum of message types */
		enum Message {
			kQueueTask = 1 << 0,
			kStopRunner = 1 << 1,
		};
		
		/** @brief Null task runner */
		CxTaskRunner();

		/**
		 * @brief Create a new task runner.
		 * @param in_name The name / id of the task runner to create.
		 * @param in_maxTaskes The max # of taskes we can have.
		 */
		CxTaskRunner(const CxChar *in_id);

		/** @brief Destroys all the taskes. */
		~CxTaskRunner();

		/** 
		 * @brief Check whether or not the runner can run a task.
		 * This method will check both the task mask and whether or
		 * not this runner is full.
		 * @return True if the task runner can run the task. 
		 */
		CxBool accepts() const;

		/** @return The number of taskes (queued or running) */
		CX_FORCE_INLINE CxI32 count() const { return m_tasks.count(); }

		/** @return The StrID (name + id) for the task runner. */
	   CX_FORCE_INLINE const CxStrID & id() const { return m_id; }

		/** @return The task mask for this runner. */
		CX_FORCE_INLINE CxI32 mask() const { return m_taskMask; }

		/** @brief Process any recieved messages. */
		void processMessages();

		/**
		 * @brief Send a message to run the specified task on the runner.
		 * This method does not retain the task, it assumes that the 
		 * retain count is already one and that if the caller wants to store
		 * the task it will call retain on it.
		 * @param in_task The task to run on the runner.
		 * @return True if the message was posted.
		 */
		CxTaskPtr queue(const CxTask *in_task);

		/** @brief Post a message telling the runner that it should quit. */
		void quit();

		/** @brief Run loop for the task runner. */
		virtual CxI32 run();

		/**
		 * @brief Set the task mask for this runner.
		 * For a task to be able to run on this task runner, then 
		 * runner->mask() & task->mask must equal task->mask().  
		 * I.e., all the bits that are set in the task->mask() must also
		 * be set in the runner->mask().
		 * @param in_mask The mask to determine which proccesses can run.
		 */
		CX_FORCE_INLINE void setMask(CxI32 in_mask) { m_taskMask = in_mask; }

	  private:
		CxStrID m_id;
		
		CxConditionVariable m_check;

		CxQueue<CxTask *> m_tasks;
		CxDualQueue<CxMsg1<CxMemAddr>, CxMutex> m_msgs;

		CxI32 m_taskMask;

		CX_FORCE_INLINE CxBool priv_post(const CxMsg1<CxMemAddr> &in_msg) {
			CxBool success = false;
			m_msgs.lock();
			success = m_msgs.push(in_msg, kCxNoLock);
			m_check.broadcast();  m_msgs.unlock();
			return success;
		}


	};

	CX_FORCE_INLINE CxBool CxTaskRunner::accepts(const CxTask *in_t) const {
		const CxI32 t_mask = in_t->mask();
		return ((m_taskMask & t_mask) == t_mask);
	}
	
} // namespace cat

#endif // CX_CORE_THREADING_CXTASKRUNNER_H
