#include "core/threading/CxTaskRunner.h"

namespace cat {

	CxTaskRunner::CxTaskRunner()
		: m_taskMask(0) {}

	CxTaskRunner::CxTaskRunner(const CxChar *in_id)
		: m_taskMask(0xffffffff) {
		m_strID = CxStrID(in_id);
		m_check.initialize();
		
		m_tasks = CxQueue<CxTask *>(16);
		m_tasks.zero();
		m_msgs = CxDualQueue<CxMsg1<CxMemAddr> >(16);
		m_msgs.zero();
	}

	CxTaskRunner::~CxTaskRunner() {
		/* Assume thread has already stopped */
		while (!m_tasks.isEmpty()) {
			CxTask *t = m_tasks.pop();
			CxDeleteISPtr(t);
		}

		/* Make sure if there are any taskes queued, we delete them */
		m_msgs.swap();
		while (!m_msgs.isReadEmpty()) {
			Msg1<CxMemAddr> m = msgs.pop();
			if (m.id() == kQueueTask && m.arg0().ptr != 0) {
				CxTask * p = reinterpret_cast<CxTask *>(m.arg0().ptr);
				CxDeleteISPtr(p);
			}
		}
	   m_taskMask = 0;
	}

	CxTaskPtr CxTaskRunner::queue(CxTask * in_task) {
		CxBool success = false;  CxMemAddr p;  p.ptr = in_task;
		m_msgs.lock();
		success = m_msgs.push(CxMsg1<CxMemAddr>(kQueueTask, p), kCxNoLock);
		m_check.broadcast();  m_msgs.unlock();
		if (success) { return CxTaskPtr(in_task); }
		else {
#if defined (CX_DEBUG)
			if (in_task->strID().str() != 0) {
				CX_WARN("Failed to queue task '%s'.", in_task->strID().str());
			}
			else {
				CX_WARN("Failed to queue task (%ll).", in_task->pid());
			}
#endif // CX_DEBUG
			return CxTaskPtr();
		}
	}

	void CxTaskRunner::quit() {
		m_msgs.lock();
		m_msgs.push(CxMsg1<CxMemAddr>(kStopRunner), kCxNoLock);
		m_check.broadcast();  m_msgs.unlock();
	}

	CxI32 CxTaskRunner::run() {
		m_msgs.lock();
		setStatus(CxThread::kRunning);  m_check.broadcast();
		m_msgs.unlock();

		/* Enter the tasking loop */
		CxBool loopity = true;
		while (status() == CxThread::kRunning) {

			/* Run a task if we have one */
			if (!m_tasks.isEmpty()) {
				CxTask *t = m_tasks.pop();  t->run();
				if (t->succeeded()) { t->onSuccess(this); }
				else if (t->failed()) { t->onFailed(this); }
				CxDeleteISPtr(t);
			}
			else {
				m_msgs.lock();
				while (m_msgs.isEmpty()) {
					setStatus(CxThread::kWaiting);
					m_check.wait(m_msgs.getLock());
				}
				setStatus(CxThread::kRunning);
				m_msgs.unlock();
			}

			/* Process any messages before running next task */
			if (!m_msgs.isEmpty()) {
				m_msgs.swap();
				while (!m_msgs.isReadEmpty()) {
					const CxMsg1<CxMemAddr> m = m_msgs.pop();
					
					if (m.id() == kQueueTask) {
						CxTask * t = reinterpret_cast<CxTask *>(m.arg0().ptr);
						m_tasks.push(t);
						t->setStatus(CxTask::kRunning);  t->onStart(this);
					}
					else if (m.id() == kStopRunner) { setStatus(CxThread::kWillFinish); }
				}
			}
		}

		CXD_MSG("CxTaskRunner '%s' finishing...", strID().str());
		return status();
	}
	
} // namespace cat
