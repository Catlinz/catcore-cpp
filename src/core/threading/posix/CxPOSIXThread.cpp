#include "core/threading/posix/CxPOSIXThread.h"
#include "core/threading/CxProcessRunner.h"
#include "core/threading/CxTaskRunner.h"
#include "core/common/CxMem.h"

namespace cat {

	class CxThreadFuncData__ {
		CxThread::Entry	 func;
		void					 *data;
		CxThread				 thread;
		CxThreadFuncData__(CxThread::Entry in_func, void *in_data) : func(in_func) data(in_data) {}
	};

	CxThread CxThread::run(CxThread::Entry in_entry, void *in_data) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		CxThreadFuncData__ *tfd = new CxThreadFuncData__(in_entry, in_data);
		tfd->thread.initThreadInfo(CxThread::kNotStarted);
		pthread_t* thread_ptr = &(tfd->thread.m_handle);

		CxI32 error = pthread_create(thread_ptr, &attr, cx_func_entry__, (void *)tfd);
		pthread_attr_destroy(&attr);

		if (error != 0) {
			CxI32 ret_err;
			switch(error) {
			case EAGAIN: ret_err = CxError::kInsufficientResources; break;
			case EINVAL: ret_err = CxError::kInvalidValue; break;
			case EPERM:  ret_err = CxError::kInvalidPermissions; break;
			default: ret_err = CxError::kUnknownError; break;
			}
				
			CXD_ERR("Failed to start thread for function.  Failed with error code %d (%s).",
					  error, CxError::str(ret_err));
			
			CxThread t = tfd.thread;
			delete tfd;
			t.setStatus(CxThread::kFailedToStart);
			return t;
		}
		else { return tfd->thread; }
	}

	
	CxI32 CxThread::runProcessRunner(CxProcessRunner* in_runner) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		CxThread &t = in_runner->thread();
		t->initThreadInfo(CxThread::kNotStarted);
		pthread_t* thread_ptr = &(t.m_handle);

		CxI32 error = pthread_create(thread_ptr, &attr, cx_process_runner_func_entry__, (void *)in_runner);
		pthread_attr_destroy(&attr);
		
		if (error != 0) {
			CxI32 ret_err;
			switch(error) {
			case EAGAIN: ret_err = CxError::kInsufficientResources; break;
			case EINVAL: ret_err = CxError::kInvalidValue; break;
			case EPERM:  ret_err = CxError::kInvalidPermissions; break;
			default: ret_err = CxError::kUnknownError; break;
			}
				
			CXD_ERR("Failed to start thread for process runner '%s'.  Failed with error code %d (%s).",
					  in_runner->name(), error, CxError::str(ret_err));
			return ret_err;
		}
		else { return 0; }
	}

	CxI32 CxThread::runTaskRunner(CxTaskRunner* in_runner) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		CxThread &t = in_runner->thread();
		t->initThreadInfo(CxThread::kNotStarted);
		pthread_t* thread_ptr = &(t.m_handle);
		
		CxI32 error = pthread_create(thread_ptr, &attr, cx_task_runner_func_entry__, (void *)in_runner);
		pthread_attr_destroy(&attr);
		
		if (error != 0) {			
			CxI32 ret_err;
			switch(error) {
			case EAGAIN: ret_err = CxError::kInsufficientResources; break;
			case EINVAL: ret_err = CxError::kInvalidValue; break;
			case EPERM:  ret_err = CxError::kInvalidPermissions; break;
			default: ret_err = CxError::kUnknownError; break;
			}
				
			CXD_ERR("Failed to start thread for task runner '%s'.  Failed with error code %d (%s).",
					  in_runner->name(), error, CxError::str(ret_err));
			return ret_err;
		}
		else { return 0; }
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

	void * cx_func_entry__(void *in_data) {
		CxThreadFuncData__ *d =  reinterpret_cast<CxThreadFuncData__*>(in_data);
		d->thread.setStatus(CxThread::kRunning);
		d->thread.setExitStatus(d->func(d->data));
		d->thread.setStatus(CxThread::kFinished);

		CxI32 exit_status = d->thread.exitStatus();
		delete d;
		pthread_exit((void *)exit_status);
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
