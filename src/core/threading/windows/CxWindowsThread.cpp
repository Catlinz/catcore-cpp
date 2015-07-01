#include "core/threading/windows/CxWindowsThread.h"
#include "core/threading/CxProcessRunner.h"
#include "core/threading/CxTaskRunner.h"

namespace cat {
	
	CxI32 CxThread::runProcessRunner(CxProcessRunner* in_runner) {
		DWORD id_ptr = &(in_runner->thread()->m_id);
		HANDLE thread_handle = CreateThread(0, 0, cx_process_runner_func_entry__, (void *)in_runner, 0, id_ptr);
		if (thread_handle == 0) {
			DWORD error = GetLastError();
			
			CXD_ERR("Failed to start thread for process runner '%s'.  Failed with error code %d.", in_runner->name(), error);
			switch(error) {
			case ERROR_TOO_MANY_TCBS:
			case ERROR_MAX_THRDS_REACHED:
			case ERROR_TOO_MANY_THREADS: return CxError::kThreadLimitEncountered; break;
			default: return CxError::kUnknownError; break;
			}
		}
		
		CloseHandle(thread_handle);
		return CxError::kNoError;
	}

	CxI32 CxThread::runTaskRunner(CxTaskRunner* in_runner) {
		DWORD id_ptr = &(in_runner->thread()->m_id);
		HANDLE thread_handle = CreateThread(0, 0, cx_task_runner_func_entry__, (void *)in_runner, 0, id_ptr);
		if (thread_handle == 0) {
			DWORD error = GetLastError();
			CXD_ERR("Failed to start thread for task runner '%s'.  Failed with error code %d.", in_runner->name(), error);
			switch(error) {
			case ERROR_TOO_MANY_TCBS:
			case ERROR_MAX_THRDS_REACHED:
			case ERROR_TOO_MANY_THREADS: return CxError::kInsufficientResources; break;
			default:                     return CxError::kUnknownError; break;
			}
		}
		CloseHandle(thread_handle);
		return 0;
	}

	CxBool CxThread::join(CxThread &in_handle) {
		HANDLE thread_handle = OpenThread(SYNCHRONIZE, 0, in_handle.m_id);
		if (thread_handle != 0) {
			DWORD reason = 0;
			do {
				reason = WaitForSingleObject(thread_handle, INFINITE);
				if (reason == WAIT_OBJECT_0) { return true; }
			} while (reason != WAIT_TIMEOUT);
			
			/* If get here, error happened while trying to join */
			DWORD error = GetLastError();
			CXD_ERR("Failed to join on thread, error code: %d.", error);
			return false;
		}
		else {
			CXD_ERR("Failed to get handle for thread to join!");
			return false;
		}
	}

	DWORD cx_process_runner_func_entry__(void *in_data) {
		CxProcessRunner* runner = reinterpret_cast<CxProcessRunner*>(in_data);
		runner->processingLoop();
		CxProcessRunner::State state = runner->state();
		return (DWORD)state;
	}

	
	DWORD cx_task_runner_func_entry__(void *in_data) {
		CxTaskRunner* runner = reinterpret_cast<CxTaskRunner*>(in_data);
		runner->taskRunLoop();
		CxTaskRunner::State state = runner->state();
		return (DWORD)state;
	}

} // namespace cat
