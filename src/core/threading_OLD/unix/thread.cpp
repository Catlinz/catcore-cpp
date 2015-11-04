#include "core/threading/unix/thread.h"
#include <cassert>
#include "core/threading/processrunner.h"
#include "core/threading/taskrunner.h"

namespace Cat {

	ThreadHandle* Thread::run(Runnable* runnable) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		pthread_t* threadId = runnable->getThread();
		I32 error = pthread_create(threadId, &attr, runnable_func_entry__, (VPtr)runnable);
		if (error != 0) {
			runnable->setError(error);
			DERR("Failed to start thread for running Runnable: " << runnable->getInfo() << " with error: " << error << " (" << getStringErrorCode(error) << ").");
			return NIL;
		}
		pthread_attr_destroy(&attr);
		return threadId;
	}

	ThreadHandle* Thread::runProcessRunner(ProcessRunner* runner) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		pthread_t* threadId = runner->threadPtr();
		I32 error = pthread_create(threadId, &attr, process_runner_func_entry__, (VPtr)runner);
		if (error != 0) {			
			DERR("Failed to start thread for Process Runner!" << runner->name() << " with error: " << error << " (" << getStringErrorCode(error) << ").");
			return NIL;
		}
		pthread_attr_destroy(&attr);
		return threadId;
	}

		ThreadHandle* Thread::runTaskRunner(TaskRunner* runner) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// Explict for portability.
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		pthread_t* threadId = runner->threadPtr();
		I32 error = pthread_create(threadId, &attr, task_runner_func_entry__, (VPtr)runner);
		if (error != 0) {			
			DERR("Failed to start thread for Task Runner!" << runner->name() << " with error: " << error << " (" << getStringErrorCode(error) << ").");
			return NIL;
		}
		pthread_attr_destroy(&attr);
		return threadId;
	}

	/**
	 * Joins on the specified thread.  Returns the VPtr value casted to 
	 * an int.
	 * @param threadHandle The thread to join on.
	 * @return The thread status of the joined thread.
	 */
	ThreadStatus Thread::join(ThreadHandle* threadHandle) {
		VPtr status = NIL;
		I32 error = pthread_join(*threadHandle, &status);
		
		if (error != 0) {
			DERR("Failed to join on thread!  Failed with error: " << error << " (" << getStringErrorCode(error) << ").");
			return -1;
		}
		return (ThreadStatus)status;
	}

	VPtr process_runner_func_entry__(VPtr data) {
		ProcessRunner* runner = reinterpret_cast<ProcessRunner*>(data);
		runner->processingLoop();
		ProcessRunner::ProcessRunnerState state = runner->state();
		pthread_exit((VPtr)state);
	}

	
	VPtr task_runner_func_entry__(VPtr data) {
		TaskRunner* runner = reinterpret_cast<TaskRunner*>(data);
		runner->taskRunLoop();
		TaskRunner::TaskRunnerState state = runner->state();
		pthread_exit((VPtr)state);
	}

} // namespace Cat
