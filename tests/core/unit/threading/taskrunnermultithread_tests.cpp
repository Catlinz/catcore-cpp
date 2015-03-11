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
	
	void testRunAndTerminateTaskRunner() {
		BEGIN_TEST;

		TaskRunner* tr = new TaskRunner("TR", 4);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->state(), TaskRunner::kTRSNotStarted);
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());		

		tr->run();
		tr->waitUntilStarted();

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());

		tr->terminateTaskRunner();
		tr->waitForTermination();

		ass_eq(tr->state(), TaskRunner::kTRSTerminated);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		
		delete tr;		
		
		FINISH_TEST;
	}

	void testTaskRunnerQueueAndRunTask() {
		BEGIN_TEST;

		reset_counts();		

		TaskRunner* tr = new TaskRunner("TR", 4);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->state(), TaskRunner::kTRSNotStarted);
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		test_counts(0, 0, 0, 0);		

		tr->run();
		
		tr->waitUntilStarted();

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));		
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());		
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());		
		test_counts(0, 0, 0, 0);		

		TaskPtr retVal;
		DMSG("Running Task #1.");		
		TaskPtr tp1 = TestTask::create("Task 1", 1, 1);
	   ass_eq(((TestTask*)tp1.ptr())->ran(), 0);			
		retVal = tr->queueTask(tp1);
		ass_true(retVal.notNull());
		((TestTask*)tp1.ptr())->waitForFinished();
		ass_false(tp1->isAlive());
		ass_eq(((TestTask*)tp1.ptr())->ran(), 1);
		tp1.setNull();		

		DMSG("Running Task #2.");	
		TaskPtr tp2 = TestTask::create("Task 2", 2, 4);
	   ass_eq(((TestTask*)tp2.ptr())->ran(), 0);			
		retVal = tr->queueTask(tp2);
		ass_true(retVal.notNull());

		DMSG("Running Task #3.");	
		TaskPtr tp3 = TestTask::create("Task 3", 3, 4);
	   ass_eq(((TestTask*)tp3.ptr())->ran(), 0);			
		retVal = tr->queueTask(tp3);
		ass_true(retVal.notNull());

		DMSG("Running Task #4.");	
		TaskPtr tp4 = TestTask::create("Task 4", 4, 1);
	   ass_eq(((TestTask*)tp4.ptr())->ran(), 0);			
		retVal = tr->queueTask(tp4);
		ass_true(retVal.notNull());

		DMSG("Running Task #5.");	
		TaskPtr tp5 = TestTask::create("Task 5", 5, 9);
	   ass_eq(((TestTask*)tp5.ptr())->ran(), 0);			
		retVal = tr->queueTask(tp5);
		ass_true(retVal.notNull());

		retVal.setNull();		

		DMSG("Waiting for Task #2 to finish.");	
		((TestTask*)tp2.ptr())->waitForFinished(); // 2
		ass_false(tp2->isAlive());
		ass_eq(((TestTask*)tp2.ptr())->ran(), 4);
		tp2.setNull();		
		DMSG("Waiting for Task #3 to finish.");
		((TestTask*)tp3.ptr())->waitForFinished(); // 3
		ass_false(tp3->isAlive());
		ass_eq(((TestTask*)tp3.ptr())->ran(), 4);
		tp3.setNull();
		DMSG("Waiting for Task #4 to finish.");
		((TestTask*)tp4.ptr())->waitForFinished(); // 4
		ass_false(tp4->isAlive());
		ass_eq(((TestTask*)tp4.ptr())->ran(), 1);
		tp4.setNull();
		DMSG("Waiting for Task #5 to finish.");
		((TestTask*)tp5.ptr())->waitForFinished(); // 5
		ass_false(tp5->isAlive());
		ass_eq(((TestTask*)tp5.ptr())->ran(), 9);
		tp5.setNull();		

		DMSG("Terminating Task Runner.");
		tr->terminateTaskRunner();
		DMSG("Waiting for Task Runner to terminate.");
		tr->waitForTermination();
	   test_counts(5, 0, 0, 5);		

		ass_eq(tr->state(), TaskRunner::kTRSTerminated);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));		
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());		
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());		

		delete tr;
		
		FINISH_TEST;
	}

	void testTaskRunnerQueueAndRunLongTaskAndTerminate() {
		BEGIN_TEST;

		reset_counts();		

		TaskRunner* tr = new TaskRunner("TR", 4);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->state(), TaskRunner::kTRSNotStarted);
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		test_counts(0, 0, 0, 0);		

		tr->run();		
		tr->waitUntilStarted();

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		test_counts(0, 0, 0, 0);		

		
		TaskPtr retVal;
		TaskPtr p1 = LongTestTask::create("Long Task 1", 1, 1);		
		retVal = tr->queueTask(p1);
		ass_true(retVal.notNull());
		TaskPtr p2 = LongTestTask::create("Long Task 2", 2, 3);		
		retVal = tr->queueTask(p2);
		ass_true(retVal.notNull());
		TaskPtr p3 = LongTestTask::create("Long Task 3", 3, 1);
		retVal = tr->queueTask(p3);
		ass_true(retVal.notNull());
		TaskPtr p4 = LongTestTask::create("Long Task 4", 4);
		retVal = tr->queueTask(p4);
		ass_true(retVal.notNull());
		retVal.setNull();		

		DMSG("Wait for Task #1 to start.");		
		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		ass_eq(((LongTestTask*)(p1.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p2.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p3.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p4.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 3 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());

		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();
		((LongTestTask*)(p1.ptr()))->waitForFinished();
		ass_eq(((LongTestTask*)(p1.ptr()))->state(), Task::kTSRemoved);
		p1.setNull();
		test_counts(1, 0, 0, 1);

		DMSG("Wait for Task #2 to start.");		
		((LongTestTask*)(p2.ptr()))->waitUntilStarted();
		ass_eq(((LongTestTask*)(p2.ptr()))->state(), Task::kTSRunning);
		ass_eq(((LongTestTask*)(p3.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(((LongTestTask*)(p4.ptr()))->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 2 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());
		ass_false(tr->hasFullQueue());
		ass_true(tr->hasQueued());
		
		p3.setNull();
		p4.setNull();
		
		test_counts(1, 0, 0, 1);
		ass_eq(tr->numFree(), 2);
		ass_eq(tr->numUsed(), 2);
		ass_false(tr->hasFullQueue());
		ass_true(tr->hasQueued());

		DMSG("Terminate Task Runner");		
		tr->terminateTaskRunner();
		((LongTestTask*)(p2.ptr()))->setTaskAsFailed();
		p2.setNull();		
		tr->waitForTermination();
		test_counts(1, 1, 2, 4);
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		ass_eq(tr->state(), TaskRunner::kTRSTerminated);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());

		delete tr;		

		tr = new TaskRunner("TR2", 4);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR2"), 0);
		ass_eq(tr->oID(), crc32("TR2"));
		ass_eq(tr->state(), TaskRunner::kTRSNotStarted);		
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());		
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
	   test_counts(1, 1, 2, 4);		

		DMSG("Running Task Runner #2");	
		tr->run();		
		tr->waitUntilStarted();

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR2"), 0);
		ass_eq(tr->oID(), crc32("TR2"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		test_counts(1, 1, 2, 4);

		DMSG("Queuing Tasks.");	
		p1 = tr->queueTask(LongTestTask::create("Long Task 5", 5, 1));
		ass_true(p1.notNull());
		retVal = tr->queueTask(LongTestTask::create("Long Task 6", 6, 3));
		ass_true(retVal.notNull());
		retVal = tr->queueTask(LongTestTask::create("Long Task 7", 7, 1));
		ass_true(retVal.notNull());
		retVal = tr->queueTask(LongTestTask::create("Long Task 8", 8));
		ass_true(retVal.notNull());

		retVal.setNull();
		((LongTestTask*)(p1.ptr()))->waitUntilStarted();

		DMSG("Terminate Task Runner #2");
		tr->terminateTaskRunner();
		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();
		p1.setNull();		
		tr->waitForTermination();
		test_counts(2, 1, 5, 8);
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		ass_eq(tr->state(), TaskRunner::kTRSTerminated);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR2"), 0);
		ass_eq(tr->oID(), crc32("TR2"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());

		delete tr;

		test_counts(2, 1, 5, 8);
		
		FINISH_TEST;
	}

	void testTaskRunnerQueueAndRunLongTaskComplex() {
		BEGIN_TEST;

		reset_counts();		

		TaskRunner* tr = new TaskRunner("TR", 4);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->state(), TaskRunner::kTRSNotStarted);
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		test_counts(0, 0, 0, 0);

		DMSG("Run Task Runner.");
		tr->run();		
		tr->waitUntilStarted();

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));		
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());		
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		test_counts(0, 0, 0, 0);

		DMSG("Queueing Tasks 1-4.");
		TaskPtr p1 = tr->queueTask(LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		TaskPtr p2 = tr->queueTask(LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		TaskPtr p3 = tr->queueTask(LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		TaskPtr p4 = tr->queueTask(LongTestTask::create("Long Task 4", 4, 1));
		ass_true(p4.notNull());

		DMSG("Waiting For Task #1 to start.");
		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		ass_eq(p1->state(), Task::kTSRunning);
		ass_eq(p2->state(), Task::kTSNotStarted);
		ass_eq(p3->state(), Task::kTSNotStarted);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 3 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());
		ass_false(tr->hasFullQueue());
		test_counts(0, 0, 0, 0);
		DMSG("Waiting For Task #1 to finish.");
		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();	
		((LongTestTask*)(p1.ptr()))->waitForFinished();
		p1.setNull();
		test_counts(1, 0, 0, 1);
		
		DMSG("Waiting For Task #2 to start.");
		((LongTestTask*)(p2.ptr()))->waitUntilStarted();
		ass_eq(p2->state(), Task::kTSRunning);
		ass_eq(p3->state(), Task::kTSNotStarted);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 2);
		ass_eq(tr->numUsed(), 2);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasQueued());
		test_counts(1, 0, 0, 1);
		DMSG("Waiting For Task #2 to finish.");
		((LongTestTask*)(p2.ptr()))->setTaskAsFailed();
		((LongTestTask*)(p2.ptr()))->waitForFinished();
		p2.setNull();
		test_counts(1, 1, 0, 2);
		
		DMSG("Waiting For Task #3 to start.");
		((LongTestTask*)(p3.ptr()))->waitUntilStarted();
		ass_eq(p3->state(), Task::kTSRunning);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 3);
		ass_eq(tr->numUsed(), 1);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasQueued());
		test_counts(1, 1, 0, 2);
		DMSG("Waiting For Task #3 to finish.");
		((LongTestTask*)(p3.ptr()))->setTaskAsFailed();
		((LongTestTask*)(p3.ptr()))->waitForFinished();
		p3.setNull();
		test_counts(1, 2, 0, 3);

		DMSG("Waiting For Task #4 to start.");
		((LongTestTask*)(p4.ptr()))->waitUntilStarted();
		ass_eq(p4->state(), Task::kTSRunning);
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());
		test_counts(1, 2, 0, 3);
		DMSG("Waiting For Task #4 to finish.");
		((LongTestTask*)(p4.ptr()))->setTaskAsSucceeded();
		((LongTestTask*)(p4.ptr()))->waitForFinished();
		p4.setNull();
		test_counts(2, 2, 0, 4);

		DMSG("Queuing Tasks 5-8");
		p1 = tr->queueTask(LongTestTask::create("Long Task 5", 5, 2));
		ass_true(p1.notNull());
		p2 = tr->queueTask(LongTestTask::create("Long Task 6", 6, 1));
		ass_true(p2.notNull());
		p3 = tr->queueTask(LongTestTask::create("Long Task 7", 7, 2));
		ass_true(p3.notNull());
		p4 = tr->queueTask(LongTestTask::create("Long Task 8", 8, 1));
		ass_true(p4.notNull());

		DMSG("Waiting For Task #5 to start.");
		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		ass_eq(p1->state(), Task::kTSRunning);
		ass_eq(p2->state(), Task::kTSNotStarted);
		ass_eq(p3->state(), Task::kTSNotStarted);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 3 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());
		ass_false(tr->hasFullQueue());
	   test_counts(2, 2, 0, 4);

		DMSG("Waiting For Task #5 to finish.");
		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();
		((LongTestTask*)(p1.ptr()))->waitForFinished();
		p1.setNull();
	   test_counts(3, 2, 0, 5);

		DMSG("Waiting For Task #6 to start.");
		((LongTestTask*)(p2.ptr()))->waitUntilStarted();
		ass_eq(p2->state(), Task::kTSRunning);
		ass_eq(p3->state(), Task::kTSNotStarted);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 2);
		ass_eq(tr->numUsed(), 2);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasQueued());
	   test_counts(3, 2, 0, 5);

		DMSG("Queuing Tasks 9-12");
		TaskPtr p5 = tr->queueTask(LongTestTask::create("Long Task 9", 9, 1));
		ass_true(p5.notNull());
		TaskPtr p6 = tr->queueTask(LongTestTask::create("Long Task 10", 10, 1));
		ass_true(p6.notNull());
		TaskPtr p7 = tr->queueTask(LongTestTask::create("Long Task 11", 11, 1));
		ass_true(p7.notNull());
		TaskPtr p8 = tr->queueTask(LongTestTask::create("Long Task 12", 12, 2));
		ass_true(p8.notNull());

		DMSG("Waiting For Task #6 to finish.");
		((LongTestTask*)(p2.ptr()))->setTaskAsFailed();
		((LongTestTask*)(p2.ptr()))->waitForFinished();
		p2.setNull();
		DMSG("Waiting For Task #7 to start.");
		((LongTestTask*)(p3.ptr()))->waitUntilStarted();
		DMSG("Waiting For Task #7 to finish.");
		((LongTestTask*)(p3.ptr()))->setTaskAsSucceeded();
		((LongTestTask*)(p3.ptr()))->waitForFinished();
		p3.setNull();

		DMSG("Waiting For Task #8 to start.");
		((LongTestTask*)(p4.ptr()))->waitUntilStarted();
		ass_eq(p4->state(), Task::kTSRunning);
		ass_eq(p5->state(), Task::kTSNotStarted);
		ass_eq(p6->state(), Task::kTSNotStarted);
		ass_eq(p7->state(), Task::kTSNotStarted);
		ass_eq(p8->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 1);
		ass_eq(tr->numUsed(), 3);
		ass_false(tr->hasFullQueue());
		ass_false(tr->inputQueue()->isEmpty());
		ass_true(tr->hasQueued());
	   test_counts(4, 3, 0, 7);

		DMSG("Waiting For Task #8 to finish.");
		((LongTestTask*)(p4.ptr()))->setTaskAsSucceeded();
		((LongTestTask*)(p4.ptr()))->waitForFinished();
		p4.setNull();
		test_counts(5, 3, 0, 8);

		DMSG("Waiting For Task #9 to start.");
		((LongTestTask*)(p5.ptr()))->waitUntilStarted();
		ass_eq(p5->state(), Task::kTSRunning);
		ass_eq(p6->state(), Task::kTSNotStarted);
		ass_eq(p7->state(), Task::kTSNotStarted);
		ass_eq(p8->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 1);
		ass_eq(tr->numUsed(), 3);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasQueued());
	   test_counts(5, 3, 0, 8);


		DMSG("Queuing Tasks 1-2 (again).");
		p1 = tr->queueTask(LongTestTask::create("Long Task 1", 1, 2));
		ass_true(p1.notNull());
		p2 = tr->queueTask(LongTestTask::create("Long Task 2", 2, 1));
		ass_true(p2.notNull());
		p3 = tr->queueTask(LongTestTask::create("Long Task 3", 3, 2));
		ass_true(p3.notNull());
		p4 = tr->queueTask(LongTestTask::create("Long Task 4", 4, 1));
		ass_true(p4.notNull());

		
		DMSG("Waiting For Task #9 to finish.");
		((LongTestTask*)(p5.ptr()))->setTaskAsSucceeded();
		((LongTestTask*)(p5.ptr()))->waitForFinished();
		p5.setNull();
		test_counts(6, 3, 0, 9);

		

		DMSG("Waiting For Task #10 to start.");
		((LongTestTask*)(p6.ptr()))->waitUntilStarted();
		ass_eq(p6->state(), Task::kTSRunning);
		ass_eq(p7->state(), Task::kTSNotStarted);
		ass_eq(p8->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 1);
		ass_eq(tr->numUsed(), 3);
		ass_false(tr->hasFullQueue());
		ass_false(tr->inputQueue()->isEmpty());
		ass_true(tr->hasQueued());
	   test_counts(6, 3, 0, 9);

		p7.setNull();
		p8.setNull();
		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();

	   test_counts(6, 3, 0, 9);

		DMSG("Terminating Task Runner.");
		tr->terminateTaskRunner();
		DMSG("Waiting For Task #10 to finish.");
		((LongTestTask*)(p6.ptr()))->setTaskAsSucceeded();
		p6.setNull();

		DMSG("Waiting for Task Runner to terminate.");
		tr->waitForTermination();
		test_counts(7, 3, 6, 16);

		ass_eq(tr->state(), TaskRunner::kTRSTerminated);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));		
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());		
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());

		delete tr;

		test_counts(7, 3, 6, 16);		
		
		FINISH_TEST;
	}
	
	void testTaskRunnerQueueAndRunLongTaskAddChildTask() {
		BEGIN_TEST;

		reset_counts();		

		TaskRunner* tr = new TaskRunner("TR", 4);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->state(), TaskRunner::kTRSNotStarted);
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		test_counts(0, 0, 0, 0);		

		tr->run();
		tr->waitUntilStarted();

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));		
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());		
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasRunningTask());	
		ass_false(tr->hasQueued());
		test_counts(0, 0, 0, 0);

		
		TaskPtr p1 = tr->queueTask(LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		p1->attachChild(LongTestTask::create("Long Task 1c", 11, 1));		
		TaskPtr p3 = tr->queueTask(LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		TaskPtr p2 = tr->queueTask(LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		p3->attachChild(LongTestTask::create("Long Task 3c", 33, 2));
		TaskPtr p4 = tr->queueTask(LongTestTask::create("Long Task 4", 4, 1));
		ass_true(p4.notNull());
		p4->attachChild(TestTask::create("Task 4c", 44, 2));

		TaskPtr p3c = p3->child();		
		TaskPtr p4c = p4->child();
		
		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		ass_eq(p1->state(), Task::kTSRunning);
		ass_eq(p2->state(), Task::kTSNotStarted);
		ass_eq(p3->state(), Task::kTSNotStarted);
		ass_eq(p4->state(), Task::kTSNotStarted);
	   ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 3 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());
		ass_false(tr->hasFullQueue());
		test_counts(0, 0, 0, 0);

		/* Fail task 1 */ /* Q: 3, 2, 4 */
		((LongTestTask*)(p1.ptr()))->setTaskAsFailed();
		p1.setNull();
		while(failure_count != 1) {
			usleep(10000);
		}
		ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 2 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());
		ass_false(tr->hasFullQueue());
		ass_true(tr->hasQueued());
		test_counts(0, 1, 1, 2);


		/* Succeed task 3 */ /* Q: 3, 2, 4 */
		((LongTestTask*)(p3.ptr()))->waitUntilStarted();
		/* Q: 2, 4 */
		((LongTestTask*)(p3.ptr()))->setTaskAsSucceeded();
		p3.setNull();
		((LongTestTask*)(p2.ptr()))->waitUntilStarted(); /* Q: 4, 3c */
		ass_eq(tr->numFree(), 2);
		ass_eq(tr->numUsed(), 2);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasRunningTask());	
		ass_true(tr->hasQueued());
		test_counts(1, 1, 1, 2);

		/* Fail task 2 */
		((LongTestTask*)(p2.ptr()))->setTaskAsFailed();
		p2.setNull();
		((LongTestTask*)(p4.ptr()))->waitUntilStarted(); /* Q: 3c */
		ass_eq(tr->numFree(), 3);
		ass_eq(tr->numUsed(), 1);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasRunningTask());	
		ass_true(tr->hasQueued());
		test_counts(1, 2, 1, 3);

		/* Succeed task 4  */
		((LongTestTask*)(p4.ptr()))->setTaskAsSucceeded();
		p4.setNull();
		((LongTestTask*)(p3c.ptr()))->waitUntilStarted(); /* Q: 4c */
		ass_eq(tr->numFree(), 3);
		ass_eq(tr->numUsed(), 1);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasRunningTask());	
		ass_true(tr->hasQueued());
		test_counts(2, 2, 1, 3);

		/* Failed task 3c */
		((LongTestTask*)(p3c.ptr()))->setTaskAsFailed();
		p3c.setNull();
		((LongTestTask*)(p4c.ptr()))->waitUntilStarted(); /* Q:  */
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());

		/* wait for 4c to succeed (Normal TestTask) */
		((TestTask*)(p4c.ptr()))->waitForFinished();	
		p4c.setNull();
		while(destroyed_count < 6) {
			usleep(10000);
		}
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());
		ass_false(tr->hasRunningTask());		
		test_counts(3, 3, 1, 7);

		tr->terminateTaskRunner();
		tr->waitForTermination();
		test_counts(3, 3, 1, 7);

		ass_eq(tr->state(), TaskRunner::kTRSTerminated);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());
		ass_false(tr->hasRunningTask());		
		test_counts(3, 3, 1, 7);
		delete tr;

		test_counts(3, 3, 1, 7);
		
		FINISH_TEST;
	}


	void testTaskRunnerQueueAndRunLongTaskClearAllWaitingTasks() {
		BEGIN_TEST;

	   reset_counts();
		
		TaskRunner* tr = new TaskRunner("TR", 4);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->state(), TaskRunner::kTRSNotStarted);
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		ass_false(tr->hasRunningTask());		
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		test_counts(0, 0, 0, 0);		

		tr->run();		
		tr->waitUntilStarted();

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));		
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());		
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_false(tr->hasFullQueue());
		ass_false(tr->hasQueued());
		ass_false(tr->hasRunningTask());		
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		test_counts(0, 0, 0, 0);	

		TaskPtr p1 = tr->queueTask(LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		p1->attachChild(LongTestTask::create("Long Task 1c", 11, 1));		
		TaskPtr p3 = tr->queueTask(LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		TaskPtr p2 = tr->queueTask(LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		p3->attachChild(LongTestTask::create("Long Task 3c", 33, 2));
		TaskPtr p4 = tr->queueTask(LongTestTask::create("Long Task 4", 4, 1));
		ass_true(p4.notNull());
		p4->attachChild(TestTask::create("Task 4c", 44, 2));

		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		ass_eq(p1->state(), Task::kTSRunning);
		ass_eq(p2->state(), Task::kTSNotStarted);
		ass_eq(p3->state(), Task::kTSNotStarted);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 3 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());
		ass_false(tr->hasFullQueue());
		test_counts(0, 0, 0, 0);

		p2.setNull();
		p3.setNull();
		p4.setNull();		

		/* Terminate all the waiting tasks */
		tr->clearAllWaitingTasks();
		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();
		p1.setNull();

		while (destroyed_count != 7) {
			usleep(10000);
		}
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());
		test_counts(1, 0, 6, 7);

	   sleep(1);

		p1 = tr->queueTask(LongTestTask::create("Long Task 1", 1, 1));
		ass_true(p1.notNull());
		p1->attachChild(LongTestTask::create("Long Task 1c", 11, 1));
		p3 = tr->queueTask(LongTestTask::create("Long Task 3", 3, 1));
		ass_true(p3.notNull());
		p2 = tr->queueTask(LongTestTask::create("Long Task 2", 2, 3));
		ass_true(p2.notNull());
		p3->attachChild(LongTestTask::create("Long Task 3c", 33, 2));
		p4 = tr->queueTask(LongTestTask::create("Long Task 4", 4, 1));
		ass_true(p4.notNull());
		p4->attachChild(TestTask::create("Task 4c", 44, 2));

		((LongTestTask*)(p1.ptr()))->waitUntilStarted();
		ass_eq(p1->state(), Task::kTSRunning);
		ass_eq(p2->state(), Task::kTSNotStarted);
		ass_eq(p3->state(), Task::kTSNotStarted);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 4 - tr->numUsed());
		ass_eq(tr->numUsed(), 4 - tr->numFree());
		ass_true(tr->numUsed() == 3 || !tr->inputQueue()->isEmpty());		
		ass_true(tr->hasQueued());
		ass_false(tr->hasFullQueue());
	   test_counts(1, 0, 6, 7);
		
		((LongTestTask*)(p1.ptr()))->setTaskAsSucceeded();
		p1.setNull();
		((LongTestTask*)(p3.ptr()))->waitUntilStarted();
		ass_eq(p2->state(), Task::kTSNotStarted);
		ass_eq(p3->state(), Task::kTSRunning);
		ass_eq(p4->state(), Task::kTSNotStarted);
		ass_eq(tr->numFree(), 1);
		ass_eq(tr->numUsed(), 3);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_true(tr->hasQueued());
	   test_counts(2, 0, 6, 7);
		
		p2.setNull();
		p4.setNull();
		
		/* Terminate all the waiting tasks */
		tr->clearAllWaitingTasks();
		((LongTestTask*)(p3.ptr()))->setTaskAsFailed();
		p3.setNull();

		while (destroyed_count != 14) {
			usleep(10000);
		}

		ass_eq(tr->state(), TaskRunner::kTRSRunning);
		ass_true(tr->isRunning());
		ass_false(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());
		test_counts(2, 1, 11, 14);			  

		tr->terminateTaskRunner();
		tr->waitForTermination();
		
		ass_eq(tr->state(), TaskRunner::kTRSTerminated);
		ass_false(tr->isRunning());
		ass_true(tr->isStopped());
		ass_eq(strcmp(tr->name(), "TR"), 0);
		ass_eq(tr->oID(), crc32("TR"));
		ass_eq(tr->inputQueue()->capacity(), 4);
		ass_true(tr->inputQueue()->isEmpty());
		ass_eq(tr->messageQueue()->capacity(), 4);
		ass_true(tr->messageQueue()->isEmpty());
		ass_eq(tr->numFree(), 4);
		ass_eq(tr->numUsed(), 0);
		ass_false(tr->hasFullQueue());
		ass_true(tr->inputQueue()->isEmpty());
		ass_false(tr->hasQueued());
		test_counts(2, 1, 11, 14);
		
		delete tr;		
		
		FINISH_TEST;
	}
	
} // namespace cc

int main(int argc, char** argv) {
	cc::testRunAndTerminateTaskRunner();
	cc::testTaskRunnerQueueAndRunTask();
	cc::testTaskRunnerQueueAndRunLongTaskAndTerminate();
	cc::testTaskRunnerQueueAndRunLongTaskComplex();	
	cc::testTaskRunnerQueueAndRunLongTaskAddChildTask();	
	cc::testTaskRunnerQueueAndRunLongTaskClearAllWaitingTasks();
	
	return 0;
}
