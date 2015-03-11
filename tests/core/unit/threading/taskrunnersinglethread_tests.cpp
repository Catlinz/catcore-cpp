#include "core/testcore.h"
#include "core/threading/taskrunner.h"
#include "core/threading/spinlock.h"


namespace cc {
	I32 destroyed_count = 0;	
	I32 failure_count = 0;
	I32 success_count = 0;
	I32 terminated_count = 0;

	Spinlock locky;

	void reset_counts() {
		locky.lock();		
	   destroyed_count = 0;
		failure_count = 0;
		success_count = 0;
		terminated_count = 0;		
		locky.unlock();
	}
	
	inline void test_counts(I32 success, I32 failed, I32 terminated, I32 destroyed) {
		locky.lock();		
		ass_eq(success_count, success);
		ass_eq(failure_count, failed);
		ass_eq(terminated_count, terminated);
		ass_eq(destroyed_count, destroyed);
		locky.unlock();		
	}
	
	
	class TestTask : public Task {		
	  public:
		ConditionVariable lock;
		
		TestTask()
			: Task(),m_val(0), m_timeToRun(0) {}

		TestTask(OID pid, I32 val, U32 timeToRun = 1, Task::TaskState finishState = Task::kTSSucceeded)
			: Task(pid), m_val(val), m_timeToRun(timeToRun), m_finishState(finishState) {}

		TestTask(const Char* name, I32 val, U32 timeToRun = 1, Task::TaskState finishState = Task::kTSSucceeded)
			: Task(name), m_val(val), m_timeToRun(timeToRun), m_finishState(finishState) {}

		~TestTask() {
			locky.lock();			
			DMSG("Test Task " << m_val << " destroyed.");
			destroyed_count++;
			locky.unlock();			
		}		

		void onFailure() {
			locky.lock();			
			DMSG("Test task (" << m_val << ") failed!");
			failure_count++;
			locky.unlock();
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onInitialize() {
			locky.lock();
			DMSG("Test task (" << m_val << ") initialized!");			
			locky.unlock();	
		}
		void onSuccess() {
			locky.lock();
			DMSG("Test task (" << m_val << ") succeeded!");
			success_count++;			
			locky.unlock();
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onTermination() {
			locky.lock();
			DMSG("Test task (" << m_val << ") terminated!");
			terminated_count++;			
			locky.unlock();
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}

		void run() {
			DMSG("Task " << m_val << " ran for " << m_timeToRun << " time.");
			usleep(100000*m_timeToRun);
			
			switch(m_finishState) {
				
			case Task::kTSSucceeded:
				succeeded();
				break;

			case Task::kTSTerminated:
				terminate();
				break;

			case Task::kTSFailed:
				failed();
				break;

			default:
				break;
			}	
		}

		void waitForFinished() {
			lock.lock();
			while (state() == Task::kTSNotStarted || isAlive()) {
				lock.wait();
			}
			lock.unlock();
		}		

		inline I32 val() const { return m_val; }
		inline U32 timeToRun() const { return m_timeToRun; }		

		inline static TaskPtr create(const Char* name, I32 val, U32 timeToRun = 1, Task::TaskState finishState = Task::kTSSucceeded) {
			return TaskPtr(new TestTask(name, val, timeToRun, finishState));
		}		

	  private:
		I32 m_val;
		U32 m_timeToRun;
		Task::TaskState m_finishState;		
	};

	void testCreateAndDestroyTaskRunner() {
		BEGIN_TEST;

		TaskRunner* pm1 = new TaskRunner();
		ass_false(pm1->isRunning());
		ass_true(pm1->isStopped());
		ass_eq(pm1->name(), NIL);
		ass_eq(pm1->oID(), 0);
		ass_eq(pm1->state(), TaskRunner::kTRSNotStarted);
		ass_eq(pm1->inputQueue()->capacity(), 0);
		ass_eq(pm1->messageQueue()->capacity(), 0);
		ass_eq(pm1->numFree(), 0);
		ass_eq(pm1->numUsed(), 0);

		TaskRunner* pm2 = new TaskRunner("PM2");
		ass_false(pm2->isRunning());
		ass_true(pm2->isStopped());
		ass_eq(strcmp(pm2->name(), "PM2"), 0);
		ass_eq(pm2->oID(), crc32("PM2"));
		ass_eq(pm2->state(), TaskRunner::kTRSNotStarted);
		ass_eq(pm2->inputQueue()->capacity(), 32);
		ass_true(pm2->inputQueue()->isEmpty());
		ass_eq(pm2->messageQueue()->capacity(), 32);
		ass_true(pm2->messageQueue()->isEmpty());
		ass_false(pm2->hasQueued());
		ass_false(pm2->hasFullQueue());
		ass_false(pm2->hasRunningTask());
		ass_eq(pm2->numFree(), 32);
		ass_eq(pm2->numUsed(), 0);
		
		TaskRunner* pm3 = new TaskRunner("PM3", 8);
		ass_false(pm3->isRunning());
		ass_true(pm3->isStopped());
		ass_eq(strcmp(pm3->name(), "PM3"), 0);
		ass_eq(pm3->oID(), crc32("PM3"));
		ass_eq(pm3->state(), TaskRunner::kTRSNotStarted);
		ass_eq(pm3->inputQueue()->capacity(), 8);
		ass_true(pm3->inputQueue()->isEmpty());   
		ass_eq(pm3->messageQueue()->capacity(), 8);
		ass_true(pm3->messageQueue()->isEmpty());
		ass_false(pm3->hasQueued());
		ass_false(pm3->hasFullQueue());
		ass_false(pm3->hasRunningTask());
		ass_eq(pm3->numFree(), 8);
		ass_eq(pm3->numUsed(), 0);
		
		
		delete pm1;
		delete pm2;
		delete pm3;		
		
		FINISH_TEST;
	}

	void testTaskRunnerQueueTask() {
		BEGIN_TEST;

		destroyed_count = 0;
		
		TaskRunner* m = new TaskRunner("PM", 4);
		ass_false(m->isRunning());
		ass_true(m->isStopped());
		ass_eq(strcmp(m->name(), "PM"), 0);
		ass_eq(m->oID(), crc32("PM"));
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		
		TaskPtr retVal;

		retVal = m->queueTask(TestTask::create("Task 1", 1));
		ass_true(retVal.notNull());
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());

		retVal = m->queueTask(TestTask::create("Task 2", 2));
		ass_true(retVal.notNull());
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		
		retVal = m->queueTask(TestTask::create("Task 3", 3));
		ass_true(retVal.notNull());
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		
		retVal = m->queueTask(TestTask::create("Task 4", 4));
		ass_true(retVal.notNull());
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_true(m->hasFullQueue());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		
		ass_eq(destroyed_count, 0);
		
		retVal = m->queueTask(TestTask::create("Task 5", 5));
		ass_false(retVal.notNull());

		ass_eq(destroyed_count, 1);

		delete m;
		ass_eq(destroyed_count, 5);		
		
		FINISH_TEST;
	}

	void testTaskRunnerPostMessages() {
		BEGIN_TEST;

		TaskRunner* m = new TaskRunner("PM", 4);
		ass_false(m->isRunning());
		ass_true(m->isStopped());
		ass_eq(strcmp(m->name(), "PM"), 0);
		ass_eq(m->oID(), crc32("PM"));
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);

		Boolean retVal = false;
	
		retVal = m->clearAllWaitingTasks();
		ass_true(retVal);
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_false(m->messageQueue()->isEmpty());
		ass_false(m->messageQueue()->isFull());

		retVal = m->terminateTaskRunner();
		ass_true(retVal);
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_false(m->messageQueue()->isEmpty());
		ass_false(m->messageQueue()->isFull());

		SimpleQueue<TaskRunner::TRMessage>::Iterator itr = m->messageQueue()->begin();				
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, TaskRunner::kTRMClearAllWaitingTasks);

		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());		
		ass_eq(itr.val().type, TaskRunner::kTRMTerminateTaskRunner);

		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());

		retVal = m->terminateTaskRunner();
		ass_true(retVal);		
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_false(m->messageQueue()->isEmpty());
		ass_false(m->messageQueue()->isFull());

		retVal = m->clearAllWaitingTasks();
		ass_true(retVal);		
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_false(m->messageQueue()->isEmpty());
		ass_true(m->messageQueue()->isFull());

		retVal = m->terminateTaskRunner();
		ass_false(retVal);

		itr = m->messageQueue()->begin();
		
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, TaskRunner::kTRMClearAllWaitingTasks);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, TaskRunner::kTRMTerminateTaskRunner);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, TaskRunner::kTRMTerminateTaskRunner);

		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());		
		ass_eq(itr.val().type, TaskRunner::kTRMClearAllWaitingTasks);

		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());		
		ass_eq(itr.val().type, TaskRunner::kTRMClearAllWaitingTasks);
		
		delete m;		
		
		FINISH_TEST;
	}

	void testTaskRunnerRunTasksSimple() {
		BEGIN_TEST;

		destroyed_count = 0;		

		TaskRunner* m = new TaskRunner("PM", 4);

		m->queueTask(TestTask::create("Task 1", 1, 1, Task::kTSSucceeded));
		m->queueTask(TestTask::create("Task 2", 2, 2, Task::kTSFailed));
		m->queueTask(TestTask::create("Task 4", 4, 5, Task::kTSTerminated));
		m->queueTask(TestTask::create("Task 3", 3, 3, Task::kTSSucceeded));
		
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_true(m->hasFullQueue());
		ass_true(m->inputQueue()->isFull());		
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);

		m->runNextTask();	/* Task 1 succeeds */	
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_false(m->inputQueue()->isFull());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		ass_eq(destroyed_count, 1);
		

		m->runNextTask();		/* Task 2 fails */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_false(m->inputQueue()->isFull());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 2);
		ass_eq(m->numUsed(), 2);
		ass_eq(destroyed_count, 2);		

		m->runNextTask();		/* Task 4 Terminated */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_false(m->inputQueue()->isFull());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 3);
		ass_eq(m->numUsed(), 1);
		ass_eq(destroyed_count, 3);

		m->runNextTask();		/* Task 3 succeeded */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_false(m->inputQueue()->isFull());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		ass_eq(destroyed_count, 4);

		delete m;

		ass_eq(destroyed_count, 4);

		FINISH_TEST;
	}

	void testTaskRunnerRunTasksComplex() {
		BEGIN_TEST;

	   reset_counts();		

		TaskRunner* m = new TaskRunner("PM", 4);

		m->queueTask(TestTask::create("Task 1", 1, 1, Task::kTSSucceeded));
		m->queueTask(TestTask::create("Task 2", 2, 2, Task::kTSFailed));
		m->queueTask(TestTask::create("Task 4", 4, 5, Task::kTSTerminated));
		m->queueTask(TestTask::create("Task 3", 3, 3, Task::kTSSucceeded));
		
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_true(m->hasFullQueue());
		ass_true(m->inputQueue()->isFull());
		test_counts(0, 0, 0, 0);		
		
		DMSG(" ---- Run #1");		
		m->runNextTask();	/* Task 1 succeeded */
		/* iqueue: 2f, 4t, 3s
		 * destroyed: 1s
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_false(m->inputQueue()->isFull());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(1, 0, 0, 1);

		m->queueTask(TestTask::create("Task 5", 5, 2));
		m->queueTask(TestTask::create("Task 6", 6, 1));		

		DMSG(" ---- Run #2");
		m->runNextTask(); /* Task 2 failed  */
	   /* iqueue: 4t, 3s, 5s
		 * destroyed: 1s, 2f
		 * queued: 6s
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
	   ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(1, 1, 0, 2);
		
		DMSG(" ---- Run #3");		
		m->runNextTask();		/* Task 4 terminated */
	   /* iqueue: 3s, 5s, 6s
		 * destroyed: 1s, 2f, 4t
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(1, 1, 1, 3);

		DMSG(" ---- Run #4");
		m->runNextTask();  	/* Task 3 suceeded */
		/* iqueue: 5s, 6s
		 * destroyed: 1s, 2f, 4t, 3s
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 2);
		ass_eq(m->numUsed(), 2);
		test_counts(2, 1, 1, 4);

		DMSG(" ---- Run #5");
		m->runNextTask();		/* Task 5 suceeded */
		/* iqueue: 6s
		 * destroyed: 1s, 2f, 4t, 3s, 5s
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 3);
		ass_eq(m->numUsed(), 1);
		test_counts(3, 1, 1, 5);

		DMSG(" ---- Run #6");
		m->runNextTask();		/* Task 6 suceeded */
		/* iqueue:
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(4, 1, 1, 6);

		m->queueTask(TestTask::create("Task 7", 7, 2, Task::kTSTerminated));
		m->queueTask(TestTask::create("Task 8", 8, 2, Task::kTSFailed));
		m->queueTask(TestTask::create("Task 9", 9, 2, Task::kTSFailed));

		DMSG(" ---- Run #7"); /* Task 7 terminated */
		m->runNextTask();  
		/* iqueue: 8f, 9f
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 2);
		ass_eq(m->numUsed(), 2);
		test_counts(4, 1, 2, 7);

		DMSG(" ---- Run #8");
		m->runNextTask();  /* Task 8 failed */
		/* iqueue: 9f
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 3);
		ass_eq(m->numUsed(), 1);
		test_counts(4, 2, 2, 8);
		
		m->queueTask(TestTask::create("Task 10", 10, 1, Task::kTSSucceeded));
		
		DMSG(" ---- Run #9");		
		m->runNextTask();  /* Task 9 failed */
	   /* iqueue: 10s
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 3);
		ass_eq(m->numUsed(), 1);
		test_counts(4, 3, 2, 9);

		m->queueTask(TestTask::create("Task 11", 11, 1, Task::kTSTerminated));
		m->queueTask(TestTask::create("Task 12", 12, 1, Task::kTSSucceeded));
		m->queueTask(TestTask::create("Task 13", 13, 1, Task::kTSFailed));
		m->queueTask(TestTask::create("Task 14", 14, 1, Task::kTSSucceeded));
		
		DMSG(" ---- Run #10");		
		m->runNextTask();  /* Task 10 suceeded */
	   /* iqueue: 11t, 12s, 13f
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s
		 * queued: 14s
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(5, 3, 2, 10);

		DMSG(" ---- Run #11");		
		m->runNextTask();  /* Task 11 terminated */
	   /* iqueue: 12s, 13f, 14s
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(5, 3, 3, 11);

		DMSG(" ---- Run #12");		
		m->runNextTask();  /* Task 12 succeeded */
		/* iqueue: 13f, 14s
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 2);
		ass_eq(m->numUsed(), 2);
		test_counts(6, 3, 3, 12);

		m->queueTask(TestTask::create("Task 15", 15, 1, Task::kTSSucceeded));
		m->queueTask(TestTask::create("Task 16", 16, 2, Task::kTSTerminated));
		m->queueTask(TestTask::create("Task 17", 17, 1, Task::kTSFailed));
		m->queueTask(TestTask::create("Task 18", 18, 1, Task::kTSFailed));

		DMSG(" ---- Run #13");		
		m->runNextTask();  /* Task 13 failed */
		/* iqueue: 14s, 15s, 16t 
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s, 13f
		 * queued: 17f, 18f
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
	   ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(6, 4, 3, 13);
		
		m->queueTask(TestTask::create("Task 19", 19, 2));
		m->queueTask(TestTask::create("Task 20", 20, 3));
		ass_true(m->hasFullQueue());		

		DMSG(" ---- Run #14");		
		m->runNextTask();  /* Task 14 succeeded*/
		/* iqueue: 15s, 16t, 17f
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s, 13f, 14s
		 * queued: 18f, 19s, 20s
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());	
	   ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(7, 4, 3, 14);
		
		m->queueTask(TestTask::create("Task 21", 21, 1, Task::kTSFailed));
		ass_true(m->hasFullQueue());
		
		DMSG(" ---- Run #15");		
		m->runNextTask();  /* Task 15 succeeded*/
	   /* iqueue: 16t, 17f, 18f
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s, 13f, 14s, 15s
		 * queued: 19s, 20s, 21f
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());	
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(8, 4, 3, 15);
		
		m->queueTask(TestTask::create("Task 22", 22, 1, Task::kTSTerminated));
		ass_true(m->hasFullQueue());
		
		DMSG(" ---- Run #16");		
		m->runNextTask();  /* Task 16 terminated */
		/* iqueue: 17f, 18f, 19s
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s, 13f, 14s, 15s, 16t,
		 * queued: 20s, 21f, 22t
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(8, 4, 4, 16);

		DMSG(" ---- Run #17");		
		m->runNextTask();  /* Task 17 failed */
		/* iqueue: 18f, 19s, 20s
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s, 13f, 14s, 15s, 16t, 17f
		 * queued: 21f, 22t
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
	   ass_false(m->hasFullQueue());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
	   test_counts(8, 5, 4, 17);

		DMSG(" ---- Run #18");		
		m->runNextTask();  /* Task 18 failed */
		/* iqueue: 19s, 20s, 21f
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s, 13f, 14s, 15s, 16t, 17f, 18f
		 * queued: 22t
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(8, 6, 4, 18);

		DMSG(" ---- Run #19");		
		m->runNextTask();  /* Task 19 succeeded */
		/* iqueue: 20s, 21f, 22t
		 * destroyed: 1s, 2f, 4t, 3s, 5s, 6s, 7t, 8f, 9f, 10s, 11t, 12s, 13f, 14s, 15s, 16t, 17f, 18f, 19s
		 * queued: 
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
	   ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(9, 6, 4, 19);

		DMSG(" ---- Run #20");		
		m->runNextTask();  /* Task 20 succeeded */
		/* iqueue: 21f, 22t
		 * destroyed: 1s,2f,4t,3s,5s,6s,7t,8f,9f,10s,11t,12s,13f,14s,15s,16t,17f,18f,19s,20s
		 * queued: 
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 2);
		ass_eq(m->numUsed(), 2);
		test_counts(10, 6, 4, 20);

		DMSG(" ---- Run #21");		
		m->runNextTask();  /* Task 21 failed */
		/* iqueue: 22t
		 * destroyed: 1s,2f,4t,3s,5s,6s,7t,8f,9f,10s,11t,12s,13f,14s,15s,16t,17f,18f,19s,20s,21f
		 * queued: 
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 3);
		ass_eq(m->numUsed(), 1);
		test_counts(10, 7, 4, 21);

		DMSG(" ---- Run #22");		
		m->runNextTask();  /* Task 22 terminated */
		/* iqueue:
		 * destroyed: 1s,2f,4t,3s,5s,6s,7t,8f,9f,10s,11t,12s,13f,14s,15s,16t,17f,18f,19s,20s,21f,22t
		 * queued: 
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(10, 7, 5, 22);
		delete m;

		test_counts(10, 7, 5, 22);

		FINISH_TEST;
	}

	
	void testTaskRunnerAddChildTaskOnSuccess() {
		BEGIN_TEST;

		reset_counts();
		test_counts(0, 0, 0, 0);		
		TaskRunner* m = new TaskRunner("PM", 4);
		TaskPtr p = TestTask::create("Task 1", 1, 1, Task::kTSSucceeded);
		p->attachChild(TestTask::create("Task 1c", 11, 2, Task::kTSSucceeded));
		m->queueTask(p);
		m->queueTask(TestTask::create("Task 2", 2, 1, Task::kTSSucceeded));
		
		p = TestTask::create("Task 4", 4, 3, Task::kTSTerminated);
		p->attachChild(TestTask::create("Task 4c", 44, 1, Task::kTSSucceeded));						  
		m->queueTask(p);
		p = TestTask::create("Task 3", 3, 1, Task::kTSFailed);
		p->attachChild(TestTask::create("Task 3c", 33, 2, Task::kTSSucceeded));	
		m->queueTask(p);
		p.setNull();		
		
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		test_counts(0, 0, 0, 0);

		DMSG(" ---- Run #1");		
		m->runNextTask();	/* Task 1 succeeds and adds 1c */
		/* finished: 1
		 * destroyed: 
		 * iqueue: 2, 4, 3, 1c
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		test_counts(1, 0, 0, 0);

		DMSG(" ---- Run #2");		
		m->runNextTask();	/* Task 2 succeeded */
		/* finished: 1
		 * destroyed: 2
		 * iqueue: 4, 3, 1c
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		test_counts(2, 0, 0, 1);
		
		DMSG(" ---- Run #3");		
		m->runNextTask();	/* Task 4 terminate, terminate child */
		/* finished: 1
		 * destroyed: 2, 4, 4c
		 * iqueue: 3, 1c
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
	   test_counts(2, 0, 2, 3);
		
		m->queueTask(TestTask::create("Task 5", 5, 1));
		DMSG(" ---- Run #4");		
		m->runNextTask();	/* 3 fails */
		/* finished: 1
		 * destroyed: 2, 4, 4c, 3, 3c
		 * iqueue: 1c, 5
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		test_counts(2, 1, 3, 5);

		DMSG(" ---- Run #5");		
		m->runNextTask();	/* Task 1c succeeds */
		/* finished: 
		 * destroyed: 2, 4, 4c, 3, 3c, 1, 1c
		 * iqueue: 5
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		test_counts(3, 1, 3, 7);

		DMSG(" ---- Run #6");		
		m->runNextTask();	/* Task 5 succeeds */
		/* finished: 
		 * destroyed: 2, 4, 4c, 3, 3c, 1, 1c, 5
		 * iqueue:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		test_counts(4, 1, 3, 8);

	
		delete m;

		test_counts(4, 1, 3, 8);
		
		FINISH_TEST;
	}

	void testTaskRunnerClearAllWaitingTasks() {
		BEGIN_TEST;

		reset_counts();		
		TaskRunner* m = new TaskRunner("PM", 4);

		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);
		ass_false(m->hasQueued());		
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 0, 0);

		m->clearAllWaitingTasks();
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_false(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_false(m->hasQueued());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 0, 0);

		DMSG(" ---- Run #1"); /* Nothing should happen */
		m->runNextTask();
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasFullQueue());
		ass_false(m->hasQueued());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 0, 0);				

		m->queueTask(TestTask::create("Task 1", 1, 1));
		m->queueTask(TestTask::create("Task 2", 2, 2));
		m->queueTask(TestTask::create("Task 4", 4, 1));
		m->queueTask(TestTask::create("Task 3", 3, 3));
		
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_true(m->hasFullQueue());
		ass_true(m->hasQueued());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 0, 0);
		
		m->clearAllWaitingTasks();
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_false(m->messageQueue()->isEmpty());
		ass_true(m->hasFullQueue());
		ass_true(m->hasQueued());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 0, 0);		
		
		DMSG(" ---- Run #2");		
		m->runNextTask();	/* Should destroy all tasks in the queue(s) */
		/* destroyed: 1, 2, 3, 4
		 * iqueue:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);
		ass_false(m->hasFullQueue());
		ass_false(m->hasQueued());
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 4, 4);

		/* Add tasks, run once, then add more so tasks in both queues */
		m->queueTask(TestTask::create("Task 1", 1, 1));				
		TaskPtr task = m->queueTask(TestTask::create("Task 2", 2, 1));
		task->attachChild(TestTask::create("Task 2c", 22, 1));		
		m->queueTask(TestTask::create("Task 4", 4, 1));
		task = m->queueTask(TestTask::create("Task 3", 3, 3));
		task->attachChild(TestTask::create("Task 3c", 33, 2));
		task.setNull();		
		test_counts(0, 0, 4, 4);
		
		DMSG(" ---- Run #3");		
		m->runNextTask();	/* Task 1 suceeds  */
		/* destroyed: 1, 2, 3, 4, 1
		 * iqueue: 2, 3, 4
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);
		ass_false(m->hasFullQueue());
		ass_true(m->hasQueued());
		ass_eq(m->numFree(), 1);
		ass_eq(m->numUsed(), 3);
		test_counts(1, 0, 4, 5);

		m->queueTask(TestTask::create("Task 1", 1, 1));
		m->queueTask(TestTask::create("Task 2", 2, 1));
		m->queueTask(TestTask::create("Task 4", 4, 1));
		m->queueTask(TestTask::create("Task 3", 3, 3));	

		DMSG(" ---- Run #4");		
		m->runNextTask();	/* Task 2 suceeded */
	   /* destroyed: 1, 2, 3, 4, 1
		 * finished: 2
		 * iqueue: 3, 4, 1, 2c
		 * queued: 2, 3, 4
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);
		ass_true(m->hasQueued());
		ass_eq(m->numFree(), 0);
		ass_eq(m->numUsed(), 4);
		test_counts(2, 0, 4, 5);

		m->clearAllWaitingTasks();
		DMSG(" ---- Run #5");		
		m->runNextTask();	/* 2, 3, 3c, 4, 3, 4, 1, 2, 2c terminated */
	   /* destroyed: 1, 2, 3, 4, 1, 2, 3, 3c, 4, 3, 4, 1, 2c, 2
		 * finished:
		 * iqueue:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);
		ass_false(m->hasFullQueue());
		ass_false(m->hasQueued());
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(2, 0, 12, 14);
		
		delete m;

		test_counts(2, 0, 12, 14);
		
		FINISH_TEST;
	}

	
	void testTaskRunnerTerminateTaskRunner() {
		BEGIN_TEST;

		reset_counts();		
		TaskRunner* m = new TaskRunner("PM", 4);

		m->queueTask(TestTask::create("Task 1", 1, 1));
		m->queueTask(TestTask::create("Task 2", 2, 2));
		m->queueTask(TestTask::create("Task 4", 4, 1));
		m->queueTask(TestTask::create("Task 3", 3, 3));
		
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_true(m->hasFullQueue());	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 0, 0);
		

		m->terminateTaskRunner();
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);	

		DMSG(" ---- Run #1");		
		m->runNextTask();	/* Task 1, 2, 3, 4 should terminate, status = will terminate */
		/* destroyed: 1, 2, 3, 4
		 * running:
		 * queued: NONE
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSTerminated);
		ass_false(m->hasFullQueue());	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 4, 4);

		/** No taskes should be added */
		m->queueTask(TestTask::create("Task 1", 1, 1));
		m->queueTask(TestTask::create("Task 2", 2, 1));
		m->queueTask(TestTask::create("Task 4", 4, 1));
		m->queueTask(TestTask::create("Task 3", 3, 3));		

		DMSG(" ---- Run #2");		
		m->runNextTask();	/*  */
		/* destroyed: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSTerminated);
		ass_false(m->hasFullQueue());	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 4, 8);

		DMSG(" ---- Run #3");		
		m->runNextTask();	/*  */
		/* finished:
		 * destroyed: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSTerminated);
		ass_false(m->hasFullQueue());	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 4, 8);

		/** No taskes should be added */
		m->queueTask(TestTask::create("Task 1", 1, 1));
		m->queueTask(TestTask::create("Task 2", 2, 3));
		m->queueTask(TestTask::create("Task 4", 4, 1));
		m->queueTask(TestTask::create("Task 3", 3, 1));

		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSTerminated);
		ass_false(m->hasFullQueue());	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 4, 12);
		
		DMSG(" ---- Run #4");		
		m->runNextTask();	/*  */
		/* finished:
		 * destroyed: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(m->state(), TaskRunner::kTRSTerminated);
		ass_false(m->hasFullQueue());	
		ass_eq(m->numFree(), 4);
		ass_eq(m->numUsed(), 0);
		test_counts(0, 0, 4, 12);
		
		delete m;

		test_counts(0, 0, 4, 12);

		m = new TaskRunner("PM2", 4);
		m->terminateTaskRunner();
		ass_eq(m->state(), TaskRunner::kTRSNotStarted);	

		DMSG(" ---- Run #5");		
		m->runNextTask();	/*  */
		/* finished:
		 * destroyed:
		 * running:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 12);
		ass_eq(m->state(), TaskRunner::kTRSTerminated);	

		/** No taskes should be added */
		m->queueTask(TestTask::create("Task 1", 1, 1));
		m->queueTask(TestTask::create("Task 2", 2, 1));
		m->queueTask(TestTask::create("Task 4", 4, 1));
		m->queueTask(TestTask::create("Task 3", 3, 1));
		ass_eq(destroyed_count, 16);
		ass_eq(m->state(), TaskRunner::kTRSTerminated);	

		DMSG(" ---- Run #6");		
		m->runNextTask();	/*  */
		/* finished:
		 * destroyed:
		 * running:
		 * queued:
		 */
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 4);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 16);
		ass_eq(m->state(), TaskRunner::kTRSTerminated);	

		delete m;

		ass_eq(destroyed_count, 16);
		
		FINISH_TEST;
	}
	
} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroyTaskRunner();
	cc::testTaskRunnerQueueTask();
	cc::testTaskRunnerPostMessages();
	cc::testTaskRunnerRunTasksSimple();
	cc::testTaskRunnerRunTasksComplex();
	cc::testTaskRunnerAddChildTaskOnSuccess();
	cc::testTaskRunnerClearAllWaitingTasks();	
	cc::testTaskRunnerTerminateTaskRunner();
	
	return 0;
}
