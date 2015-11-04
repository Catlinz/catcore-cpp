#include "core/threading/asynctaskrunner.h"
#include "core/threading/asynctask.h"
#include "core/threading/thread.h"

namespace Cat {

	AsyncTaskRunner::AsyncTaskRunner() {
		initAsyncTaskRunner(8);
	}

	AsyncTaskRunner::AsyncTaskRunner(U32 number_of_threads) {
		initAsyncTaskRunner(number_of_threads);
	}

	AsyncTaskRunner::~AsyncTaskRunner() {
		stop(); // Make sure the threads are all finished first.
		if (runners_) {
			// Delete all the threads runners.
			for(U32 i = 0; i < number_of_threads_; i++) {
				delete runners_[i];
			}
			delete[] runners_;
			runners_ = NIL;
		}
		
		if (sync_controller_) {
			delete sync_controller_;
			sync_controller_ = NIL;
		}

		// Delete any remaining tasks in the queue. (Shouldn't be any)
		AsyncTaskQueuedItem* task = first_;
		while(task) {
			first_ = first_->next;
			task->task->onCompletion();
			if (task->task->isDestroyable()) {
				task->task->destroy();
				delete task->task;
			}
			delete task;
			task = first_;
		}
		first_ = last_ = NIL;
	}

	/**
	 * The method to stop the runner.
	 * After it's stopped, it can no longer accept any new tasks, and the 
	 * remaining tasks in the queue are run.  The Runner waits until all the 
	 * tasks are finished.
	 */
	void AsyncTaskRunner::stop() {
		if (state_ == RUNNER_STARTED && runners_) {
			sync_controller_->lock();
			state_ = RUNNER_STOPPING;
			sync_controller_->broadcast();
			sync_controller_->unlock();

			// Wait on all the threads to finish.
			for(U32 i = 0; i < number_of_threads_; i++) {
				//sync_controller_->broadcast();
				Thread::join(runners_[i]->getThread());
			}
			state_ = RUNNER_STOPPED;
		}
		
	}

	/**
	 * The method to try and run a task with this AsyncTaskRunner.
	 * The Task gets put onto the queue of tasks to run, then the ConditionVariable 
	 * is signalled.  If there is a thread waiting for a task, it will then wake up and 
	 * pull the next task off the queue.  If there are no waiting threads, then each thread, once 
	 * finished, checks to see if there is something yummy in the queue, and pulls it off to run it.
	 * @param task The AsyncTask to run on this AsyncTaskRunner.
	 * @return The AsyncResult associated with the AsyncTask we're running.
	 */
	AsyncResult* AsyncTaskRunner::run(AsyncTask* task) {
		sync_controller_->lock();
		if (state_ == RUNNER_STARTED) {
			if (!last_) {
				first_ = last_ = new AsyncTaskQueuedItem(task);
			} else {
				last_->next = new AsyncTaskQueuedItem(task, last_);
				last_ = last_->next;
			}
			// Signal a thread to wakeup if there is one to wakeup.
			sync_controller_->signal();
		}
		sync_controller_->unlock();
		return task->getResult();
	}

	/**
	 * The initAsyncTaskRunner method initializes the AsyncTaskRunner with the 
	 * specified number of threads.
	 * @param number_of_threads The number of threads to have running.
	 */
	void AsyncTaskRunner::initAsyncTaskRunner(U32 number_of_threads) {
		last_ = first_ = NIL;
		runners_ = NIL;
		sync_controller_ = new ConditionVariable();
		sync_controller_->lock();

		number_of_threads_ = number_of_threads;
		runners_ = new AsyncTaskRunnerThread*[number_of_threads_];
		for(U32 i = 0; i < number_of_threads_; i++) {
			runners_[i] = new AsyncTaskRunnerThread(this, i);
			Thread::run(runners_[i]);
		}

		state_ = RUNNER_STARTED;
		sync_controller_->unlock();
		
	}

	AsyncTaskRunnerThread::~AsyncTaskRunnerThread() {
		runner_ = NIL;
	}

	AsyncTask* AsyncTaskRunnerThread::getTaskToRun() {
		if (!runner_->first_) {
			return NIL;
		}
		AsyncTaskQueuedItem* item = runner_->first_;
		if (!item->next) {
			runner_->first_ = runner_->last_ = NIL;

		} else {
			runner_->first_ = item->next;
			//runner_->sync_controller_->signal();
		}
		AsyncTask* task = item->task;
		delete item;
		return task;
	}

	I32 AsyncTaskRunnerThread::run() {
		AsyncTask* task = NIL;
		I32 retVal = 0;

		D(std::cout << "AsyncTaskRunner[" << id_ << "] STARTED..." << std::endl << std::flush);

		while(true) {
			runner_->sync_controller_->lock();
			while(!(task = getTaskToRun())) {
				// If the runner is no longer running, break out of the thread.
				if (runner_->state_ != RUNNER_STARTED) {break; }
				runner_->sync_controller_->wait();
	
			}
			// If the runner is no longer running, break out of the thread.
			if (runner_->state_ != RUNNER_STARTED && !task) { runner_->sync_controller_->unlock(); break; }
			D(std::cout << "AsyncTask[" << id_ << "] now RUNNING task...." << std::endl << std::flush);
			runner_->sync_controller_->unlock();

			task->onStart();
			retVal = task->run();
#if defined (DEBUG)
			DMSG("Async Task finished with return value: " << retVal << ".");
#endif
			task->onCompletion();
			if (task->isDestroyable()) {
				task->destroy();
				delete task;
			}
			task = NIL;
			D(std::cout << "AsyncTask[" << id_ << "] now FINISHED task." << std::endl << std::flush);
		}
		if (task) {
			task->onCompletion();
			if (task->isDestroyable()) {
				task->destroy();
				delete task;
			}
			task = NIL;

		}
		D(std::cout << "AsyncTaskRunner[" << id_ << "] FINISHED..." << std::endl << std::flush);
		return 0;
	}
	
	AsyncTaskQueuedItem::AsyncTaskQueuedItem(AsyncTask* ptask, AsyncTaskQueuedItem* pprev) {
		task = ptask;
		next = NIL;
		prev = pprev;
	}
	AsyncTaskQueuedItem::~AsyncTaskQueuedItem() {
		task = NIL;
		next = NIL;
		prev = NIL;
	}

} // namespace Cat
