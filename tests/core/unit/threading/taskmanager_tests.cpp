#include "core/testcore.h"
#include "core/threading/taskmanager.h"
#include "core/threading/taskrunner.h"


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
			: Task(),m_val(0), m_ran(0), m_timeToRun(0) {}

		TestTask(OID pid, I32 val, U32 timeToRun = 1, Task::TaskState finishState = Task::kTSSucceeded)
			: Task(pid), m_val(val), m_ran(0), m_timeToRun(timeToRun), m_finishState(finishState) {}

		TestTask(const Char* name, I32 val, U32 timeToRun = 1, Task::TaskState finishState = Task::kTSSucceeded)
			: Task(name), m_val(val), m_ran(0), m_timeToRun(timeToRun), m_finishState(finishState) {}

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
			usleep(100000*m_timeToRun);
			DMSG("Task " << m_val << " ran for " << m_timeToRun << " time.");
			m_ran += m_timeToRun;			
			
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

		inline U32 ran() const { return m_ran; }		
		inline I32 val() const { return m_val; }
		inline U32 timeToRun() const { return m_timeToRun; }		

		inline static TaskPtr create(const Char* name, I32 val, U32 timeToRun = 1, Task::TaskState finishState = Task::kTSSucceeded) {
			return TaskPtr(new TestTask(name, val, timeToRun, finishState));
		}		

	  private:
		I32 m_val;
		U32 m_ran;		
		U32 m_timeToRun;
		Task::TaskState m_finishState;		
	};
	

	class LongTestTask : public Task {		
	  public:
		ConditionVariable lock;
		
		LongTestTask()
			: Task(), m_sudoState(0), m_ran(0), m_val(0), m_timeEachRun(1) {}

		LongTestTask(OID pid, I32 val, U32 timeEachRun = 1)
			: Task(pid), m_sudoState(0), m_ran(0), m_val(val), m_timeEachRun(timeEachRun) {}

		LongTestTask(const Char* name, I32 val, U32 timeEachRun = 1)
			: Task(name), m_sudoState(0), m_ran(0), m_val(val), m_timeEachRun(timeEachRun) {}

		~LongTestTask() {
			locky.lock();			
			DMSG("Long Test Task " << m_val << " destroyed." << std::flush);
			destroyed_count++;
			locky.unlock();			
		}		

		void onFailure() {
			locky.lock();
			DMSG("Long Test task (" << m_val << ") failed!" << std::flush);
			failure_count++;			
			locky.unlock();
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onInitialize() {
			locky.lock();
			DMSG("Long Test task (" << m_val << ") initialized!" << std::flush);
			locky.unlock();
			lock.lock();
			lock.broadcast();
			lock.unlock();			
		}	
		void onSuccess() {
			locky.lock();
			DMSG("Long Test task (" << m_val << ") succeeded!" << std::flush);
			success_count++;
			locky.unlock();
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onTermination() {
			locky.lock();
			DMSG("Long Test task (" << m_val << ") terminated!" << std::flush);
			terminated_count++;
			locky.unlock();
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}

		void run() {
			while (true) {				
				DMSG("Long Test Task [" << m_val << "] has run for " << m_timeEachRun << " ticks." << std::flush);
				usleep(100000*m_timeEachRun);			
				m_ran += m_timeEachRun;
				DMSG("Long Test Task [" << m_val << "] has been running for " << m_ran << " ticks." << std::flush);

				lock.lock();
				if (m_sudoState > 0) {
					succeeded();					
				}
				else if (m_sudoState < 0) {
					failed();					
				}			
				lock.unlock();
				if (m_sudoState != 0) {
					return;
				}				
			}
			
		}

		void waitForFinished() {
			lock.lock();
			while (state() == Task::kTSNotStarted || isAlive()) {
				lock.wait();
			}
			lock.unlock();
		}

		void waitUntilStarted() {
			lock.lock();
			while (state() == Task::kTSNotStarted) {
				lock.wait();
			}
			lock.unlock();
		}
		
		void setTaskAsSucceeded() {
			lock.lock();
			m_sudoState = 1;			
			lock.unlock();
		}

		void setTaskAsFailed() {
			lock.lock();
			m_sudoState = -1;
			lock.unlock();
		}

		inline U32 ran() const { return m_ran; }
		inline I32 val() const { return m_val; }
		inline U32 timeEachRun() const { return m_timeEachRun; }
		inline I32 sudoState() const { return m_sudoState; }		

		inline static TaskPtr create(const Char* name, I32 val, U32 timeEachRun = 1) {
			return TaskPtr(new LongTestTask(name, val, timeEachRun));
		}		

	  private:
		I32 m_sudoState;		
		U32 m_ran;		
		I32 m_val;
		U32 m_timeEachRun;		
	};

	void testCreateAndDestroyTaskManager() {
		BEGIN_TEST;

		TaskManager* runner = new TaskManager();
		delete runner;

		runner = new TaskManager(8);
		ass_eq(runner->maxTaskRunners(), 8);
		ass_eq(runner->numTaskRunners(), 0);

		delete runner;		

		FINISH_TEST;
	}

	void testTaskManagerCreateAndGetTaskRunner() {
		BEGIN_TEST;

		reset_counts();
		
		TaskManager* runner = new TaskManager(2);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 0);

		Boolean retVal;		
		retVal = runner->createTaskRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 1);
		
		retVal = runner->createTaskRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 2);
		
		retVal = runner->createTaskRunner("PM3", 5);
		ass_false(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 2);

		TaskRunner* pm = runner->getTaskRunner("PM1");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->state(), TaskRunner::kTRSNotStarted);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_eq(pm->messageQueue()->capacity(), 4);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 4);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(0, 0, 0, 0);		

		pm = runner->getTaskRunner("PM2");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->state(), TaskRunner::kTRSNotStarted);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_eq(pm->messageQueue()->capacity(), 32);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 32);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(0, 0, 0, 0);	

		pm = runner->getTaskRunner("PM3");
		ass_eq(pm, NIL);	  
		
		delete runner;
		
		test_counts(0, 0, 0, 0);	

		FINISH_TEST;
	}

	void testTaskManagerStartTaskRunners() {
		BEGIN_TEST;

		TaskManager* runner = new TaskManager(2);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 0);

		Boolean retVal;		
		retVal = runner->createTaskRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 1);
		
		retVal = runner->createTaskRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 2);
		
		retVal = runner->createTaskRunner("PM3", 5);
		ass_false(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 2);

		TaskRunner* pm = runner->getTaskRunner("PM1");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->state(), TaskRunner::kTRSNotStarted);	
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_eq(pm->messageQueue()->capacity(), 4);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 4);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(0, 0, 0, 0);

		pm = runner->getTaskRunner("PM2");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->state(), TaskRunner::kTRSNotStarted);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_eq(pm->messageQueue()->capacity(), 32);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 32);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(0, 0, 0, 0);

		pm = runner->getTaskRunner("PM3");
		ass_eq(pm, NIL);

		runner->startTaskRunners();
		
		pm = runner->getTaskRunner("PM1");
	   ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->state(), TaskRunner::kTRSRunning);		
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());		
		ass_eq(pm->messageQueue()->capacity(), 4);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);
		ass_eq(pm->numFree(), 4);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(0, 0, 0, 0);

		pm = runner->getTaskRunner("PM2");
	   ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->state(), TaskRunner::kTRSRunning);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_eq(pm->messageQueue()->capacity(), 32);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 32);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(0, 0, 0, 0);
		
		pm = runner->getTaskRunner("PM3");
		ass_eq(pm, NIL);
	
				
		delete runner;

		test_counts(0, 0, 0, 0);

		FINISH_TEST;
	}

	void testTaskManagerQueueTask() {
		BEGIN_TEST;

	   reset_counts();		

		TaskManager* runner = new TaskManager(2);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 0);

		Boolean retValB;		
		retValB = runner->createTaskRunner("PM1", 4);
		ass_true(retValB);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 1);
		
		retValB = runner->createTaskRunner("PM2");
		ass_true(retValB);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 2);

		runner->startTaskRunners();		

		TaskPtr p1 = runner->queueTask("PM1", LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		TaskPtr p2 = runner->queueTask("PM2", LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		TaskPtr p3 = runner->queueTask("PM1", LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		TaskPtr p4 = runner->queueTask("PM2", LongTestTask::create("Long Task 4", 4));
		ass_true(p4.notNull());

		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		((LongTestTask*)(p2.ptr()))->waitUntilStarted();
		ass_eq(((LongTestTask*)(p1.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p2.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p3.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p4.ptr()))->state(), Task::kTSNotStarted);

		test_counts(0, 0, 0, 0);
		TaskPtr tp5 = runner->queueTask("PM3", LongTestTask::create("Task 5", 5, 1));
		ass_true(tp5.isNull());
		test_counts(0, 0, 0, 1);
		runner->terminateAllTaskRunners();		

		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();
		((LongTestTask*)(p2.ptr()))->setTaskAsFailed();
		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();		

		runner->waitForAllTaskRunnersToTerminate();		
		test_counts(1, 1, 2, 5);		
		
		delete runner;

		test_counts(1, 1, 2, 5);	

		FINISH_TEST;
	}

	void testTaskManagerTerminateTaskRunner() {
		BEGIN_TEST;

		reset_counts();		

		TaskManager* runner = new TaskManager(2);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 0);

		Boolean retVal;		
		retVal = runner->createTaskRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 1);
		
		retVal = runner->createTaskRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 2);

		runner->startTaskRunners();		

		TaskPtr p1 = runner->queueTask("PM1", LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		TaskPtr p2 = runner->queueTask("PM2", LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		TaskPtr p3 = runner->queueTask("PM1", LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		TaskPtr p4 = runner->queueTask("PM2", LongTestTask::create("Long Task 4", 4));
		ass_true(p4.notNull());

		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		((LongTestTask*)(p2.ptr()))->waitUntilStarted();
		ass_eq(((LongTestTask*)(p1.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p2.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p3.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p4.ptr()))->state(), Task::kTSNotStarted);

		p3.setNull();
		p4.setNull();

		TaskRunner* pm = runner->getTaskRunner("PM1");
		ass_false(pm->hasFullQueue());
		ass_true(pm->hasRunningTask());		
		test_counts(0, 0, 0, 0);
		
		runner->terminateTaskRunner("PM1");
		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();
		p1.setNull();
		pm->waitForTermination();
				
		ass_eq(pm->state(), TaskRunner::kTRSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));	
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());		
		ass_eq(pm->messageQueue()->capacity(), 4);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 4);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(1, 0, 1, 2);	

		sleep(1);		

		pm = runner->getTaskRunner("PM2");
		ass_false(pm->hasFullQueue());
		ass_true(pm->hasRunningTask());		
		test_counts(1, 0, 1, 2);
		runner->terminateTaskRunner("PM2");
		((LongTestTask*)(p2.ptr()))->setTaskAsSucceeded();
		p2.setNull();
		pm->waitForTermination();		

		ass_eq(pm->state(), TaskRunner::kTRSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));		
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());		
		ass_eq(pm->messageQueue()->capacity(), 32);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 32);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(2, 0, 2, 4);						
		
		delete runner;

		test_counts(2, 0, 2, 4);		

		FINISH_TEST;
	}

	void testTaskManagerTerminateAllTaskRunners() {
		BEGIN_TEST;

		reset_counts();	

		TaskManager* runner = new TaskManager(2);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 0);

		Boolean retVal;		
		retVal = runner->createTaskRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 1);
		
		retVal = runner->createTaskRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 2);
		ass_eq(runner->numTaskRunners(), 2);

		runner->waitForAllTaskRunnersToTerminate();	
		runner->startTaskRunners();		

		TaskPtr p1 = runner->queueTask("PM1", LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		TaskPtr p2 = runner->queueTask("PM2", LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		TaskPtr p3 = runner->queueTask("PM1", LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		TaskPtr p4 = runner->queueTask("PM2", LongTestTask::create("Long Task 4", 4));
		ass_true(p4.notNull());

		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		((LongTestTask*)(p2.ptr()))->waitUntilStarted();
		ass_eq(((LongTestTask*)(p1.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p2.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p3.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p4.ptr()))->state(), Task::kTSNotStarted);

		p3.setNull();
		p4.setNull();

		runner->terminateAllTaskRunners();
		((LongTestTask*)(p1.ptr()))->setTaskAsFailed();
		((LongTestTask*)(p2.ptr()))->setTaskAsSucceeded();
		p1.setNull();
		p2.setNull();		
		runner->waitForAllTaskRunnersToTerminate();

		test_counts(1, 1, 2, 4);

		TaskRunner* pm = runner->getTaskRunner("PM1");
		ass_eq(pm->state(), TaskRunner::kTRSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));		
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());		
		ass_eq(pm->messageQueue()->capacity(), 4);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 4);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(1, 1, 2, 4);

		pm = runner->getTaskRunner("PM2");
		ass_eq(pm->state(), TaskRunner::kTRSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_eq(pm->messageQueue()->capacity(), 32);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(pm->numFree(), 32);
		ass_eq(pm->numUsed(), 0);
		ass_false(pm->hasFullQueue());
		ass_false(pm->hasRunningTask());		
		test_counts(1, 1, 2, 4);
		
		delete runner;

			test_counts(1, 1, 2, 4);		

		FINISH_TEST;
	}

	void testTaskManagerClearAllWaitingTasks() {
		BEGIN_TEST;

		reset_counts();	

		TaskManager* runner = new TaskManager(4);
		ass_eq(runner->maxTaskRunners(), 4);
		ass_eq(runner->numTaskRunners(), 0);

		Boolean retVal;		
		retVal = runner->createTaskRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 4);
		ass_eq(runner->numTaskRunners(), 1);
		
		retVal = runner->createTaskRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 4);
		ass_eq(runner->numTaskRunners(), 2);

		retVal = runner->createTaskRunner("PM3");
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 4);
		ass_eq(runner->numTaskRunners(), 3);

		retVal = runner->createTaskRunner("PM4");
		ass_true(retVal);
		ass_eq(runner->maxTaskRunners(), 4);
		ass_eq(runner->numTaskRunners(), 4);

		runner->startTaskRunners();		

		TaskPtr p1 = runner->queueTask("PM1", LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		TaskPtr p2 = runner->queueTask("PM2", LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		TaskPtr p3 = runner->queueTask("PM1", LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		TaskPtr p4 = runner->queueTask("PM2", LongTestTask::create("Long Task 4", 4, 1));
		ass_true(p4.notNull());
		TaskPtr p5 = runner->queueTask("PM3", LongTestTask::create("Long Task 5", 5, 1));
		ass_true(p5.notNull());
		TaskPtr p6 = runner->queueTask("PM3", LongTestTask::create("Long Task 6", 6, 3));
		ass_true(p6.notNull());
		TaskPtr p7 = runner->queueTask("PM4", LongTestTask::create("Long Task 7", 7, 1));
		ass_true(p7.notNull());
		TaskPtr p8 = runner->queueTask("PM3", LongTestTask::create("Long Task 8", 8, 2));
		ass_true(p8.notNull());
		TaskPtr p9 = runner->queueTask("PM1", LongTestTask::create("Long Task 9", 9, 4));
		ass_true(p9.notNull());
		TaskPtr p10 = runner->queueTask("PM4", LongTestTask::create("Long Task 10", 10, 2));
		ass_true(p10.notNull());

		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		((LongTestTask*)(p2.ptr()))->waitUntilStarted();
		((LongTestTask*)(p5.ptr()))->waitUntilStarted();
		((LongTestTask*)(p7.ptr()))->waitUntilStarted();

		ass_eq(((LongTestTask*)(p1.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p2.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p5.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p7.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p3.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p4.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p6.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p8.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p9.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p10.ptr()))->state(), Task::kTSNotStarted);

		p3.setNull();
		p4.setNull();
		p6.setNull();
		p8.setNull();
		p9.setNull();
		p10.setNull();

		runner->clearAllWaitingTasks("PM1");
		((LongTestTask*)(p1.ptr()))->setTaskAsFailed();
		p1.setNull();				
		
		while (destroyed_count != 3) { usleep(10000); }		

		sleep(1);
		ass_false(runner->getTaskRunner("PM1")->hasRunningTask());
		ass_true(runner->getTaskRunner("PM2")->hasRunningTask());
		ass_true(runner->getTaskRunner("PM3")->hasRunningTask());
		ass_true(runner->getTaskRunner("PM4")->hasRunningTask());
		test_counts(0, 1, 2, 3);		

	   runner->clearAllWaitingTasks("PM3");
		((LongTestTask*)(p5.ptr()))->setTaskAsSucceeded();
		p5.setNull();
		while (destroyed_count != 6) { usleep(10000); }		

		sleep(1);
		ass_false(runner->getTaskRunner("PM1")->hasRunningTask());
		ass_true(runner->getTaskRunner("PM2")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM3")->hasRunningTask());
		ass_true(runner->getTaskRunner("PM4")->hasRunningTask());
		test_counts(1, 1, 4, 6);		

		runner->clearAllWaitingTasks("PM2");
		((LongTestTask*)(p2.ptr()))->setTaskAsSucceeded();
		p2.setNull();
		while (destroyed_count != 8) { usleep(10000); }		

		sleep(1);
		ass_false(runner->getTaskRunner("PM1")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM2")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM3")->hasRunningTask());
		ass_true(runner->getTaskRunner("PM4")->hasRunningTask());
		test_counts(2, 1, 5, 8);
		
		runner->clearAllWaitingTasks("PM4");
		((LongTestTask*)(p7.ptr()))->setTaskAsSucceeded();
		p7.setNull();
		while (destroyed_count != 10) { usleep(10000); }		

		sleep(1);
		ass_false(runner->getTaskRunner("PM1")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM2")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM3")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM4")->hasRunningTask());
		test_counts(3, 1, 6, 10);

		p1 = runner->queueTask("PM1", LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		p2 = runner->queueTask("PM2", LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		p3 = runner->queueTask("PM1", LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		p4 = runner->queueTask("PM2", LongTestTask::create("Long Task 4", 4, 1));
		ass_true(p4.notNull());
		p5 = runner->queueTask("PM3", LongTestTask::create("Long Task 5", 5, 1));
		ass_true(p5.notNull());
		p6 = runner->queueTask("PM3", LongTestTask::create("Long Task 6", 6, 3));
		ass_true(p6.notNull());
		p7 = runner->queueTask("PM4", LongTestTask::create("Long Task 7", 7, 1));
		ass_true(p7.notNull());
		p8 = runner->queueTask("PM3", LongTestTask::create("Long Task 8", 8, 2));
		ass_true(p8.notNull());
		p9 = runner->queueTask("PM1", LongTestTask::create("Long Task 9", 9, 4));
		ass_true(p9.notNull());
		p10 = runner->queueTask("PM4", LongTestTask::create("Long Task 10", 10, 2));
		ass_true(p10.notNull());

		p3.setNull();
		p4.setNull();
		p6.setNull();
		p8.setNull();
		p9.setNull();
		p10.setNull();

		sleep(1);		
				
		runner->clearAllWaitingTasks();
		((LongTestTask*)(p1.ptr()))->setTaskAsFailed();
		p1.setNull();
		((LongTestTask*)(p2.ptr()))->setTaskAsSucceeded();
		p2.setNull();
		((LongTestTask*)(p5.ptr()))->setTaskAsSucceeded();
		p5.setNull();
		((LongTestTask*)(p7.ptr()))->setTaskAsFailed();
		p7.setNull();		
		
		while(destroyed_count != 20) { usleep(10000); }		

		sleep(1);
		ass_false(runner->getTaskRunner("PM1")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM2")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM3")->hasRunningTask());
		ass_false(runner->getTaskRunner("PM4")->hasRunningTask());
		test_counts(5, 3, 12, 20);

		delete runner;

		test_counts(5, 3, 12, 20);

		FINISH_TEST;
	}

} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroyTaskManager();
	cc::testTaskManagerCreateAndGetTaskRunner();
	cc::testTaskManagerStartTaskRunners();
	cc::testTaskManagerQueueTask();
	cc::testTaskManagerTerminateTaskRunner();
	cc::testTaskManagerTerminateAllTaskRunners();
	cc::testTaskManagerClearAllWaitingTasks();	
	
	return 0;
}
