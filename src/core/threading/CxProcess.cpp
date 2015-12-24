#include "core/threading/CxProcess.h"

namespace cat {

	CxU64 CxProcess::s_autoPID = 0;

	CxProcess::CxProcess(CxI32 in_cost, CxI32 in_priority)
		: m_priority(in_priority), m_cost(in_cost),
		  m_status(kNotStarted), m_exitStatus(kNone) {
		CX_ISPTR_INIT;
		m_pid = CxProcess::generatePID();
	}

	CxProcess::CxProcess(const CxChar *in_name, CxI32 in_cost, CxI32 in_priority)
		: m_priority(in_priority), m_cost(in_cost),
		  m_status(kNotStarted), m_exitStatus(kNone) {
		CX_ISPTR_INIT;
		m_pid = CxProcess::generatePID();
		m_name = CxStrID(in_name);
	}

	CxProcess::~CxProcess() {}

	void CxProcess::fail() {
		m_status = kDone;
		m_exitStatus = kFailed;
	}

	CxBool CxProcess::onFailure(CxProcessRunner *in_runner) {
		CX_UNUSED(in_runner);  return true;
	}

	void CxProcess::onPause(CxProcessRunner *in_runner) {
		CX_UNUSED(in_runner);
	}

	void CxProcess::onResume(CxProcessRunner *in_runner) {
		CX_UNUSED(in_runner);
	}

	void CxProcess::onStart(CxProcessRunner *in_runner) {
		CX_UNUSED(in_runner);
	}

	CxBool CxProcess::onSuccess(CxProcessRunner *in_runner) {
		CX_UNUSED(in_runner);  return true;
	}

	CxBool CxProcess::onTermination(CxProcessRunner *in_runner) {
		CX_UNUSED(in_runner);  return true;
	}

	void CxProcess::success() {
		m_status = kDone;
		m_exitStatus = kSucceeded;
	}

	void CxProcess::terminate() {
		m_status == kDone;
		m_exitStatus == kTerminated;
	}
	
} // namespace cat
