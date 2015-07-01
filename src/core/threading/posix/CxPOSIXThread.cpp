#include "core/threading/posix/CxPOSIXThread.h"
#include "core/threading/CxProcessRunner.h"
#include "core/threading/CxTaskRunner.h"
#include "core/common/CxMem.h"

namespace cat {

	CxThread::CxThread() {
		mem::set(&m_handle, 0, sizeof(pthread_t));
	}

	CxI32 CxThread::runProcessRunner(CxProcessRunner* in_runner) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		pthread_t* thread_ptr = &(in_runner->thread()->m_handle);
		CxI32 error = pthread_create(thread_ptr, &attr, cx_process_runner_func_entry__, (void *)in_runner);
		
		if (error != 0) {
			CXD_ERR("Failed to start thread for process runner '%s'.  Failed with error code %d.", in_runner->name(), error);
			switch(error) {
			case EAGAIN: return CxError::kInsufficientResources; break;
			case EINVAL: return CxError::kInvalidValue; break;
			case EPERM:  return CxError::kInvalidPermissions; break;
			default: return CxError::kUnknownError; break;
			}
		}
		pthread_attr_destroy(&attr);
		return 0;
	}

	CxI32 CxThread::runTaskRunner(CxTaskRunner* in_runner) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		pthread_t* thread_ptr = &(in_runner->thread()->m_handle);
		CxI32 error = pthread_create(thread_ptr, &attr, cx_task_runner_func_entry__, (void *)in_runner);
		if (error != 0) {			
			CXD_ERR("Failed to start thread for task runner '%s'.  Failed with error code %d.", in_runner->name(), error);
			switch(error) {
			case EAGAIN: return CxError::kInsufficientResources; break;
			case EINVAL: return CxError::kInvalidValue; break;
			case EPERM:  return CxError::kInvalidPermissions; break;
			default: return CxError::kUnknownError; break;
			}
		}
		pthread_attr_destroy(&attr);
		return 0;
	}

	CxBool CxThread::join(CxThread &in_handle) {
		void *status = NIL;
		CxI32 error = pthread_join(in_handle.m_handle, &status);
		
		if (error != 0) {
			CXD_ERR("Failed to join on thread, error code: %d.", error);
			return false;
		}
		else { return true; }
	}

	void * cx_process_runner_func_entry__(void *in_data) {
		CxProcessRunner* runner = reinterpret_cast<CxProcessRunner*>(in_data);
		runner->processingLoop();
		CxProcessRunner::State state = runner->state();
		pthread_exit((void *)state);
	}

	
	void * cx_task_runner_func_entry__(void *in_data) {
		CxTaskRunner* runner = reinterpret_cast<CxTaskRunner*>(in_data);
		runner->taskRunLoop();
		CxTaskRunner::State state = runner->state();
		pthread_exit((void *)state);
	}

} // namespace cat
