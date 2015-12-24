#include "core/threading/CxProcessRunner.h"

namespace cat {

	CxProcessRunner::CxProcessRunner()
		: m_pauseCount(0), m_maxProcesses(0), m_procMask(0) {}

	CxProcessRunner::CxProcessRunner(const CxChar *in_id, CxI32 in_maxProcesses)
		: m_pauseCount(0), m_maxProcesses(in_maxProcesses), m_procMask(0xffffffff) {
		m_strID = CxStrID(in_id);
		m_check.initialize();
		
		m_proc = CxVector<CxProcess *>(in_maxProcesses);
		m_proc.zero();
		m_removed = CxVector<CxProcess *>(in_maxProcesses);
		m_removed.zero();
		m_msgs = CxDualQueue<CxMsg1<CxMemAddr> >(in_maxProcesses);
		m_msgs.zero();
	}

	CxProcessRunner::~CxProcessRunner() {
		/* Assume thread has already stopped */
		for (CxI32 i = 0; i < m_proc.count(); ++i) {
			CxDeleteISPtr(m_proc[i]);
		}

		for (CxI32 i = 0; i < m_removed.count(); ++i) {
			CxDeleteISPtr(m_removed[i]);
		}

		/* Make sure if there are any processes queued, we delete them */
		m_msgs.swap();
		while (!m_msgs.isReadEmpty()) {
			Msg1<CxMemAddr> m = msgs.pop();
			if (m.id() == kQueueProcess && m.arg0().ptr != 0) {
				CxProcess * p = reinterpret_cast<CxProcess *>(m.arg0().ptr);
				CxDeleteISPtr(p);
			}
		}
		m_pause_count = m_maxProcesses = m_procMask = 0;
	}

	CxProcess * CxProcessRunner::findProcessByHash(CxU32 in_hash) {
		const CxI32 p_count = m_proc.count();
		for (CxI32 i = 0; i < p_count; ++i) {
			if (m_proc.at(i)->strID() == in_hash) { return m_proc[i]; }
		}
		return 0;
	}
	
	CxProcess * CxProcessRunner::findProcessByPID(CxU64 in_pid) {
		const CxI32 p_count = m_proc.count();
		for (CxI32 i = 0; i < p_count; ++i) {
			if (m_proc.at(i)->pid() == in_pid) { return m_proc[i]; }
		}
		return 0;
	}

	CxProcessPtr CxProcessRunner::getProcess(CxU64 in_pid) {
		CxProcessPtr p;
		m_msgs.lock();
		const CxI32 proc_count = m_proc.count();
	   for (CxI32 i = 0; i < proc_count; ++i) {
			if (m_proc[i]->pid() == in_pid) {
				p = m_proc[i];  break;
			}
		}
		m_msgs.unlock();
		CXD_IF_MSG(p == 0, "Failed to find process (%ll).", in_pid);
		return p;
	}

	CxProcessPtr CxProcessRunner::getProcess(const CxChar *in_name) {
		CxProcessPtr p;
		const CxU32 hash = CxHash(in_name);
		m_msgs.lock();
		const CxI32 proc_count = m_proc.count();
	   for (CxI32 i = 0; i < proc_count; ++i) {
			if (m_proc[i]->strID() == hash) {
				p = m_proc[i];  break;
			}
		}
		m_msgs.unlock();
		CXD_IF_MSG(p == 0, "Failed to find process '%s'.", in_name);
		return p;
	}

	CxBool CxProcessRunner::pause(CxU64 in_pid) {
		CxMemAddr pid;  pid.addr = in_pid;
		CxBool success = priv_post(CxMsg1<CxMemAddr>(kPauseProcessByPID, pid));
		CXD_IF_WARN(!success, "Failed to post pause message for process (%ll).", in_pid);
		return success;
	}

	CxBool CxProcessRunner::pause(const CxChar *in_name) {
		CxMemAddr hash;  pid.addr = CxHash(in_name);
	   CxBool success = priv_post(CxMsg1<CxMemAddr>(kPauseProcessByName, hash));
		CXD_IF_WARN(!success, "Failed to post pause message for process '%s'.", in_name);
		return success;
	}

	void CxProcessRunner::processMessages() {
		m_msgs.swap();

		while (!m_msgs.isReadEmpty()) {
			const CxMsg1<CxMemAddr> m = m_msgs.pop();
			const CxI32 type = m.id();
			CxProcess *p = 0;
			
			switch(type) {
			case kQueueProcess:
				p = reinterpret_cast<CxProcess *>(m.arg0().ptr);
				m_proc.append(p);
				p->setStatus(CxProcess::kRunning);
				p->onStart(this);
				break;
				
			case kPauseProcessByPID:
				setToPaused(findProcessByPID(m.arg0().addr));
				break;
			case kPauseProcessByName:
				setToPaused(findProcessByHash((CxU32)m.arg0().addr));
				break;

			case kResumeProcessByPID:
				resumePaused(findProcessByPID(m.arg0().addr));
				break;
			case kResumeProcessByName:
				resumePaused(findProcessByHash((CxU32)m.arg0().addr));
				break;

			case kTerminateProcessByPID:
				terminateProcess(findProcessByPID(m.arg0().addr));
				break;
			case kTerminateProcessByName:
				terminateProcess(findProcessByHash((CxU32)m.arg0().addr));
				break;
			case kTerminateAllProcesses:
				terminateAllProcesses();
				break;

			case kStopRunner:
				setStatus(CxThread::kWillFinish);
				break;
			}
		}
	}

	CxBool CxProcessRunner::queue(CxProcess * in_process) {
		CxBool success = false;  CxMemAddr p;  p.ptr = in_process;
		m_msgs.lock();
		success = m_msgs.push(CxMsg1<CxMemAddr>(kQueueProcess, p), kCxNoLock);
		m_check.broadcast();  m_msgs.unlock();
#if defined (CX_DEBUG)
		if (!success) {
			if (in_process->strID().str() != 0) {
				CX_WARN("Failed to queue process '%s'.", in_process->strID().str());
			}
			else {
				CX_WARN("Failed to queue process (%ll).", in_process->pid());
			}
		}
#endif // CX_DEBUG
		return success;
	}

	void CxProcessRunner::quit() {
		m_msgs.lock();
		m_msgs.push(CxMsg1<CxMemAddr>(kStopRunner), kCxNoLock);
		m_check.broadcast();  m_msgs.unlock();
	}

	CxBool CxProcessRunner::resume(CxU64 in_pid) {
		CxMemAddr pid;  pid.addr = in_pid;
		CxBool success = priv_post(CxMsg1<CxMemAddr>(kResumeProcessByPID, pid));
		CXD_IF_WARN(!success, "Failed to post resume message for process (%ll).", in_pid);
		return success;
	}

	CxBool CxProcessRunner::resume(const CxChar *in_name) {
		CxMemAddr hash;  pid.addr = CxHash(in_name);
	   CxBool success = priv_post(CxMsg1<CxMemAddr>(kResumeProcessByName, hash));
		CXD_IF_WARN(!success, "Failed to post resume message for process '%s'.", in_name);
		return success;
	}

	void CxProcessRunner::resumePaused(CxProcess *in_proc) {
		if (in_proc != 0 && in_proc->status() == CxProcess::kPaused) {
			in_proc->setStatus(CxProcess::kRunning);
			in_proc->onResume(this);
			--m_pauseCount;
		}
	}

	void CxProcessRunner::setToPaused(CxProcess *in_proc) {
		if (in_proc != 0 && in_proc->status() != CxProcess::kPaused) {
			in_proc->setStatus(CxProcess::kPaused);
			in_proc->onPause(this);
			++m_pauseCount;
		}
	}
	
	void CxProcessRunner::terminateAllProcesses() {
		const CxI32 p_count = m_proc.count();
		for (CxI32 i = 0; i < p_count; ++i) {
			CxProcess *p = m_proc.at(i);
			p->setTerminated();
			p->onTermination(this);
			m_removed->append(p);
		}
		m_proc.clear();
		m_pauseCount = 0;
	}
	
	void CxProcessRunner::terminateProcess(CxProcess *in_proc) {
		if (in_proc != 0) {
			if (in_proc->status() == CxProcess::kPaused) { --m_pauseCount; }
			in_proc->terminate();
			//			in_proc->setStatus(CxProcess::kDone);
			//in_proc->setExitStatus(CxProcess::kTerminated);
			//in_proc->onTermination();
		}
	}

	CxI32 CxProcessRunner::run() {
		m_msgs.lock();
		setStatus(CxThread::kRunning);  m_check.broadcast();
		m_msgs.unlock();

		/* Enter the processing loop */
		CxBool loopity = true;
		while (status() == CxThread::kRunning) {
			if (m_proc.count() > m_pauseCount) {
				runProcesses(1);
			}
			else {
				m_msgs.lock();
				while (m_msgs.isEmpty() && m_proc.count() <= m_pauseCount) {
					setStatus(CxThread::kWaiting);
					m_check.wait(m_msgs.getLock());
				}
				setStatus(CxThread::kRunning);
				m_msgs.unlock();
			}
			processMessages();
		}

		CXD_MSG("CxProcessRunner '%s' finishing...", strID().str());
		return status();
	}

	void CxProcessRunner::runProcesses(CxI32 in_timeForEachProcess) {
		/* Check for and handle terminated processes. */
		if (m_removed.count() > 0) {
			for (CxI32 i = 0; i < m_removed.count(); ++i) {
				CxProcess * p = m_removed.at(i);
				m_removed.removeAt(i);  CxDeleteISPtr(p);  --i;
			}
		}

		/* Run the processes */
		const CxI32 p_count = m_proc.count();
		for (CxI32 i = 0; i < p_count; ++i) {
			CxProcess *p = m_proc.at(i);
			if (p->status() == CxProcess::kRunning) {
				p->process(p->getRequestedRunTime(in_timeForEachProcess));
			}

			if (p->isDone()) {
				CxBool remove_p = true;
				if (p->succeeded()) { remove_p = p->onSuccess(this); }
				else if (p->terminated()) { remove_p = p->onTermination(this); }
				else if (p->failed()) { remove_p = p->onFailure(this); }

				if (remove_p) {
					m_removed.append(p);  m_proc.removeAt(i);
				}
			}
		}
	}

	CxBool CxProcessRunner::terminate(CxU64 in_pid) {
		CxMemAddr pid;  pid.addr = in_pid;
		CxBool success = priv_post(CxMsg1<CxMemAddr>(kTerminateProcessByPID, pid));
		CXD_IF_WARN(!success, "Failed to post terminate message for process (%ll).", in_pid);
		return success;
	}

	CxBool CxProcessRunner::terminate(const CxChar *in_name) {
		CxMemAddr hash;  pid.addr = CxHash(in_name);
	   CxBool success = priv_post(CxMsg1<CxMemAddr>(kTerminateProcessByName, hash));
		CXD_IF_WARN(!success, "Failed to post terminate message for process '%s'.", in_name);
		return success;
	}

	CxBool CxProcessRunner::terminateAll() {
	   CxBool success = priv_post(CxMsg1<CxMemAddr>(kTerminateAllProcesses));
		CXD_IF_WARN(!success, "Failed to post terminateAll message to '%s'.", m_id.str());
		return success;
	}
	
} // namespace cat
