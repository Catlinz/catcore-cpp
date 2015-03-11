#include "core/testcore.h"
#include "core/threading/processrunner.h"

namespace cc {

	I32 destroyed_count = 0;	

	class TestProcess : public Process {		
	  public:
		ConditionVariable lock;
		
		TestProcess()
			: Process(), m_ran(0), m_val(0) {}

		TestProcess(OID pid, I32 val, U32 timeToRun = 1)
			: Process(pid), m_ran(0), m_val(val), m_timeToRun(timeToRun) {}

		TestProcess(const Char* name, I32 val, U32 timeToRun = 1)
			: Process(name), m_ran(0), m_val(val), m_timeToRun(timeToRun) {}

		~TestProcess() {
			DMSG("Test Process " << m_val << " destroyed." << std::flush);
			destroyed_count++;			
		}		

		void onFailure() {
			DMSG("Test process (" << m_val << ") failed!" << std::flush);
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onInitialize() {
			DMSG("Test process (" << m_val << ") initialized!" << std::flush);
		}
		void onPause() {
			DMSG("Test process (" << m_val << ") paused!");
		}
		void onResume() {
			DMSG("Test process (" << m_val << ") resumed!");
		}
		void onSuccess() {
			DMSG("Test process (" << m_val << ") succeeded!" << std::flush);
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onTermination() {
			DMSG("Test process (" << m_val << ") terminated!" << std::flush);
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}

		void run(U32 time) {
			DMSG("Process [" << m_val << "] recieved " << time << " ticks." << std::flush);
			usleep(100000*time);			
			m_ran += time;
			setPriorityModifier(priorityModifier() + 1);
			DMSG("Process [" << m_val << "] ran for " << time << " tick." << std::flush);
			if (m_ran >= m_timeToRun) {
				succeeded();			
			}			
		}

		void waitForFinished() {
			lock.lock();
			while (state() == Process::kPSNotStarted || isAlive()) {
				lock.wait();
			}
			lock.unlock();
		}		

		inline U32 ran() const { return m_ran; }
		inline I32 val() const { return m_val; }
		inline U32 timeToRun() const { return m_timeToRun; }		

		inline static ProcessPtr create(const Char* name, I32 val, U32 timeToRun = 1) {
			return ProcessPtr(new TestProcess(name, val, timeToRun));
		}		

	  private:
		U32 m_ran;		
		I32 m_val;
		U32 m_timeToRun;		
	};	

	class LongTestProcess : public Process {		
	  public:
		ConditionVariable lock;
		
		LongTestProcess()
			: Process(), m_sudoState(0), m_ran(0), m_val(0) {}

		LongTestProcess(OID pid, I32 val, U32 timeEachRun = 1)
			: Process(pid), m_sudoState(0), m_ran(0), m_val(val), m_timeEachRun(timeEachRun) {}

		LongTestProcess(const Char* name, I32 val, U32 timeEachRun = 1)
			: Process(name), m_sudoState(0), m_ran(0), m_val(val), m_timeEachRun(timeEachRun) {}

		~LongTestProcess() {
			DMSG("Long Test Process " << m_val << " destroyed." << std::flush);
			destroyed_count++;			
		}		

		void onFailure() {
			DMSG("Long Test process (" << m_val << ") failed!" << std::flush);
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onInitialize() {
			DMSG("Long Test process (" << m_val << ") initialized!" << std::flush);
			lock.lock();
			lock.broadcast();
			lock.unlock();			
		}
		void onPause() {
			DMSG("Long Test process (" << m_val << ") paused!");
		}
		void onResume() {
			DMSG("Long Test process (" << m_val << ") resumed!");
		}
		void onSuccess() {
			DMSG("Long Test process (" << m_val << ") succeeded!" << std::flush);
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}
		void onTermination() {
			DMSG("Long Test process (" << m_val << ") terminated!" << std::flush);
			lock.lock();
			lock.broadcast();
			lock.unlock();	
		}

		void run(U32 time) {
			DMSG("Long Test Process [" << m_val << "] recieved " << time << " ticks." << std::flush);
			usleep(100000*m_timeEachRun);			
			m_ran += time;
			setPriorityModifier(priorityModifier() + 1);
			DMSG("Long Test Process [" << m_val << "] has been running for " << m_ran << " ticks." << std::flush);

			lock.lock();
			if (m_sudoState > 0) {
				succeeded();
			}
			else if (m_sudoState < 0) {
				failed();
			}
			lock.unlock();			
		}

		void waitForFinished() {
			lock.lock();
			while (state() == Process::kPSNotStarted || isAlive()) {
				lock.wait();
			}
			lock.unlock();
		}

		void waitUntilStarted() {
			lock.lock();
			while (state() == Process::kPSNotStarted) {
				lock.wait();
			}
			lock.unlock();
		}
		

		void setProcessAsSucceeded() {
			lock.lock();
			m_sudoState = 1;			
			lock.unlock();
		}

		void setProcessAsFailed() {
			lock.lock();
			m_sudoState = -1;
			lock.unlock();
		}

		inline U32 ran() const { return m_ran; }
		inline I32 val() const { return m_val; }
		inline U32 timeEachRun() const { return m_timeEachRun; }
		inline I32 sudoState() const { return m_sudoState; }		

		inline static ProcessPtr create(const Char* name, I32 val, U32 timeEachRun = 1) {
			return ProcessPtr(new LongTestProcess(name, val, timeEachRun));
		}		

	  private:
		I32 m_sudoState;		
		U32 m_ran;		
		I32 m_val;
		U32 m_timeEachRun;		
	};
	
	void testRunAndTerminateProcessRunner() {
		BEGIN_TEST;

		ProcessRunner* pm = new ProcessRunner("PM", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		pm->terminateProcessRunner();
		pm->waitForTermination();

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		
		delete pm;		
		
		FINISH_TEST;
	}

	void testProcessRunnerQueueAndRunProcess() {
		BEGIN_TEST;

		destroyed_count = 0;

		ProcessRunner* pm = new ProcessRunner("PM", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);		

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		
		ass_eq(destroyed_count, 0);	

		Boolean retVal;
		ProcessPtr tp1 = TestProcess::create("Process 1", 1, 1);
	   ass_eq(((TestProcess*)tp1.ptr())->ran(), 0);			
		retVal = pm->queueProcess(tp1);
		ass_true(retVal);
		((TestProcess*)tp1.ptr())->waitForFinished();
		ass_false(tp1->isAlive());
		ass_eq(((TestProcess*)tp1.ptr())->ran(), 1);	

		ProcessPtr tp2 = TestProcess::create("Process 2", 2, 4);
	   ass_eq(((TestProcess*)tp2.ptr())->ran(), 0);			
		retVal = pm->queueProcess(tp2);
		ass_true(retVal);
		ProcessPtr tp3 = TestProcess::create("Process 3", 3, 4);
	   ass_eq(((TestProcess*)tp3.ptr())->ran(), 0);			
		retVal = pm->queueProcess(tp3);
		ass_true(retVal);
		ProcessPtr tp4 = TestProcess::create("Process 4", 4, 1);
	   ass_eq(((TestProcess*)tp4.ptr())->ran(), 0);			
		retVal = pm->queueProcess(tp4);
		ass_true(retVal);
		ProcessPtr tp5 = TestProcess::create("Process 5", 5, 9);
	   ass_eq(((TestProcess*)tp5.ptr())->ran(), 0);			
		retVal = pm->queueProcess(tp5);
		ass_true(retVal);

		tp1.setNull();
		((TestProcess*)tp2.ptr())->waitForFinished(); // 2
		ass_false(tp2->isAlive());
		ass_eq(((TestProcess*)tp2.ptr())->ran(), 6);
		tp2.setNull();
		((TestProcess*)tp3.ptr())->waitForFinished(); // 3
		ass_false(tp3->isAlive());
		ass_eq(((TestProcess*)tp3.ptr())->ran(), 6);
		tp3.setNull();
		((TestProcess*)tp4.ptr())->waitForFinished(); // 4
		ass_false(tp4->isAlive());
		ass_eq(((TestProcess*)tp4.ptr())->ran(), 1);
		tp4.setNull();
		((TestProcess*)tp5.ptr())->waitForFinished(); // 5
		ass_false(tp5->isAlive());
		ass_eq(((TestProcess*)tp5.ptr())->ran(), 10);
		tp5.setNull();		

		pm->terminateProcessRunner();
		pm->waitForTermination();
		ass_eq(destroyed_count, 5);	

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		delete pm;
		
		FINISH_TEST;
	}

	void testProcessRunnerQueueAndRunLongProcessAndTerminate() {
		BEGIN_TEST;

		destroyed_count = 0;

		ProcessRunner* pm = new ProcessRunner("PM", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);		

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);	

		
		Boolean retVal;
		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = pm->queueProcess(p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = pm->queueProcess(p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = pm->queueProcess(p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4);
		retVal = pm->queueProcess(p4);
		ass_true(retVal);

		((LongTestProcess*)(p1.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p2.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p3.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p4.ptr()))->waitUntilStarted();

		ass_eq(((LongTestProcess*)(p1.ptr()))->state(), Process::kPSRunning);
		ass_eq(((LongTestProcess*)(p2.ptr()))->state(), Process::kPSRunning);
		ass_eq(((LongTestProcess*)(p3.ptr()))->state(), Process::kPSRunning);
		ass_eq(((LongTestProcess*)(p4.ptr()))->state(), Process::kPSRunning);

		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();		

		pm->terminateProcessRunner();
		pm->waitForTermination();
		ass_eq(destroyed_count, 4);	

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		delete pm;		

		pm = new ProcessRunner("PM2", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);		

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);	
		
		retVal = pm->queueProcess(LongTestProcess::create("Long Process 5", 5, 1));
		ass_true(retVal);
		retVal = pm->queueProcess(LongTestProcess::create("Long Process 6", 6, 3));
		ass_true(retVal);
		retVal = pm->queueProcess(LongTestProcess::create("Long Process 7", 7, 1));
		ass_true(retVal);
		retVal = pm->queueProcess(LongTestProcess::create("Long Process 8", 8));
		ass_true(retVal);	

		pm->terminateProcessRunner();
		pm->waitForTermination();
		ass_eq(destroyed_count, 8);	

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		delete pm;
		
		FINISH_TEST;
	}

	void testProcessRunnerQueueAndRunLongProcessGetAndTerminateProcess() {
		BEGIN_TEST;

		destroyed_count = 0;

		ProcessRunner* pm = new ProcessRunner("PM", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);		

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);	

		
		Boolean retVal;
		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = pm->queueProcess(p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = pm->queueProcess(p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = pm->queueProcess(p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4, 1);
		retVal = pm->queueProcess(p4);
		ass_true(retVal);

		((LongTestProcess*)(p1.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p2.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p3.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p4.ptr()))->waitUntilStarted();

		ass_eq(p1->state(), Process::kPSRunning);
		ass_eq(p2->state(), Process::kPSRunning);
		ass_eq(p3->state(), Process::kPSRunning);
		ass_eq(p4->state(), Process::kPSRunning);

		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();

		/* Test Getting the process */
		ProcessPtr ptr = pm->getProcess("Process 2");
		ass_true(ptr.isNull());
		ptr = pm->getProcess("Long Process 3");
		ass_eq(((LongTestProcess*)(ptr.ptr()))->val(), 3);
		ptr = pm->getProcess("Long Process 1");
		ass_eq(((LongTestProcess*)(ptr.ptr()))->val(), 1);
		ptr = pm->getProcess("Long Process 4");
		ass_eq(((LongTestProcess*)(ptr.ptr()))->val(), 4);
		ptr = pm->getProcess("Long Process 2");
		ass_eq(((LongTestProcess*)(ptr.ptr()))->val(), 2);

		ptr.setNull();

	   sleep(1);
		/* Succeed process 3 */
		((LongTestProcess*)(pm->getProcess("Long Process 3").ptr()))->setProcessAsSucceeded();

		/* Add process 5 and 6 */
		p1 = LongTestProcess::create("Long Process 5", 5, 1);
		retVal = pm->queueProcess(p1);
		ass_true(retVal);
		p2 = LongTestProcess::create("Long Process 6", 6, 2);
		retVal = pm->queueProcess(p2);
		ass_true(retVal);
		
		sleep(1);
		/* Fail process 1 , Succeed process 4*/
		((LongTestProcess*)(pm->getProcess("Long Process 1").ptr()))->setProcessAsFailed();
		((LongTestProcess*)(pm->getProcess("Long Process 4").ptr()))->setProcessAsSucceeded();
		
		sleep(1);
		
		/* add process 3*/		
		p3 = LongTestProcess::create("Long Process 3", 3, 3);		
		retVal = pm->queueProcess(p3);
		ass_true(retVal);
		
		((LongTestProcess*)(p1.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p2.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p3.ptr()))->waitUntilStarted();

		ass_eq(p1->state(), Process::kPSRunning);
		ass_eq(p2->state(), Process::kPSRunning);
		ass_eq(p3->state(), Process::kPSRunning);

		p1.setNull();
		p2.setNull();
		p3.setNull();
		
		sleep(1);
		/* Fail process 5 , terminate process 1, 3 (1 not there)*/
		((LongTestProcess*)(pm->getProcess("Long Process 5").ptr()))->setProcessAsFailed();
		pm->terminateProcess("Long Process 1");
		pm->terminateProcess("Long Process 3");

		sleep(1);		
		 
		pm->terminateProcessRunner();
		pm->waitForTermination();
		ass_eq(destroyed_count, 7);	

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		delete pm;		
		
		FINISH_TEST;
	}

	void testProcessRunnerQueueAndRunLongProcessPauseAndResumeProcess() {
		BEGIN_TEST;

		destroyed_count = 0;

		ProcessRunner* pm = new ProcessRunner("PM", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);		

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);	

		
		Boolean retVal;
		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = pm->queueProcess(p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = pm->queueProcess(p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = pm->queueProcess(p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4, 1);
		retVal = pm->queueProcess(p4);
		ass_true(retVal);

		((LongTestProcess*)(p1.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p2.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p3.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p4.ptr()))->waitUntilStarted();

		ass_eq(p1->state(), Process::kPSRunning);
		ass_eq(p2->state(), Process::kPSRunning);
		ass_eq(p3->state(), Process::kPSRunning);
		ass_eq(p4->state(), Process::kPSRunning);

		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();

		/* Get and pause some of the processes */
		pm->pauseProcess("Long Process 1");
		pm->pauseProcess("Long Process 3");

		while (pm->getProcess("Long Process 1")->state() == Process::kPSRunning) {
			usleep(10000);
		}
		while (pm->getProcess("Long Process 3")->state() == Process::kPSRunning) {
			usleep(10000);
		}

		ProcessPtr ptr = pm->getProcess("Long Process 1");
		ass_true(ptr->isPaused());
		ptr = pm->getProcess("Long Process 3");
		ass_true(ptr->isPaused());
		ptr = pm->getProcess("Long Process 2");
		ass_false(ptr->isPaused());
		ptr = pm->getProcess("Long Process 4");
		ass_false(ptr->isPaused());
		
		ptr.setNull();

		pm->terminateProcess("Long Process 2");
		pm->terminateProcess("Long Process 4");

		while (pm->processCount() > 2) {
			usleep(10000);
		}	
		ass_eq(pm->processCount(), 2);
		ass_false(pm->hasRunning());
		ass_false(pm->hasRemoved());
		ass_true(pm->hasPaused());		
		ptr = pm->getProcess("Long Process 1");
		ass_true(ptr->isPaused());
		ptr = pm->getProcess("Long Process 3");
		ass_true(ptr->isPaused());
		ptr.setNull();

		pm->resumeProcess("Long Process 1");
		while (!pm->hasRunning()) {
			usleep(10000);
		}

		ass_eq(pm->processCount(), 2);
		ass_true(pm->hasRunning());
		ass_false(pm->hasRemoved());
		ass_true(pm->hasPaused());		
		ptr = pm->getProcess("Long Process 1");
		ass_false(ptr->isPaused());
		ptr = pm->getProcess("Long Process 3");
		ass_true(ptr->isPaused());
		ptr.setNull();

		pm->resumeProcess("Long Process 3");
		while (pm->getProcess("Long Process 3")->state() != Process::kPSRunning) {
			usleep(10000);
		}

		ass_eq(pm->processCount(), 2);
		ass_true(pm->hasRunning());
		ass_false(pm->hasRemoved());
		ass_false(pm->hasPaused());		
		ptr = pm->getProcess("Long Process 1");
		ass_false(ptr->isPaused());
		ptr = pm->getProcess("Long Process 3");
		ass_false(ptr->isPaused());
		ptr.setNull();
		 
		pm->terminateProcessRunner();
		pm->waitForTermination();
		ass_eq(destroyed_count, 4);	

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		delete pm;

		ass_eq(destroyed_count, 4);
		
		FINISH_TEST;
	}

	void testProcessRunnerQueueAndRunLongProcessAddChildProcess() {
		BEGIN_TEST;

		destroyed_count = 0;

		ProcessRunner* pm = new ProcessRunner("PM", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);		

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);	

		
		Boolean retVal;
		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);
		p1->attachChild(LongTestProcess::create("Long Process 1c", 11, 1));
		retVal = pm->queueProcess(p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = pm->queueProcess(p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		p3->attachChild(LongTestProcess::create("Long Process 3c", 33, 2));
		retVal = pm->queueProcess(p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4, 1);
		p4->attachChild(TestProcess::create("Process 4c", 44, 5));
		retVal = pm->queueProcess(p4);
		ass_true(retVal);

		((LongTestProcess*)(p1.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p2.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p3.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p4.ptr()))->waitUntilStarted();

		ass_eq(p1->state(), Process::kPSRunning);
		ass_eq(p2->state(), Process::kPSRunning);
		ass_eq(p3->state(), Process::kPSRunning);
		ass_eq(p4->state(), Process::kPSRunning);

		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();


		ass_eq(pm->processCount(), 4);
		ass_eq(destroyed_count, 0);

		/* Fail process 1 */
		((LongTestProcess*)(pm->getProcess("Long Process 1").ptr()))->setProcessAsFailed();

		while(pm->processCount() > 3) {
			usleep(10000);
		}

		ass_eq(pm->processCount(), 3);
		ass_eq(destroyed_count, 2);

		/* Succeed process 3 */
		((LongTestProcess*)(pm->getProcess("Long Process 3").ptr()))->setProcessAsSucceeded();

		while(pm->getProcess("Long Process 3c").isNull() ||
				pm->getProcess("Long Process 3c")->state() != Process::kPSRunning || pm->processCount() > 3) {
			usleep(10000);
		}

		ass_eq(pm->processCount(), 3);
		ass_eq(destroyed_count, 2);

		while(pm->getProcess("Long Process 3").notNull()){
			usleep(10000);
		}

		ass_true(pm->getProcess("Long Process 3").isNull());

		/* Succeed process 4 and wait for child to finish */
		((LongTestProcess*)(pm->getProcess("Long Process 4").ptr()))->setProcessAsSucceeded();
		while(pm->processCount() > 2) {
			usleep(10000);		
		}

		ass_eq(pm->processCount(), 2);
		ass_eq(destroyed_count, 4);

		/* Fail 3c */
		((LongTestProcess*)(pm->getProcess("Long Process 3c").ptr()))->setProcessAsFailed();
		while (pm->processCount() > 1) {
			usleep(10000);
		}

		ass_eq(pm->processCount(), 1);
		ass_eq(destroyed_count, 6);

		pm->terminateProcessRunner();
		pm->waitForTermination();
		ass_eq(destroyed_count, 7);	

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		delete pm;

		ass_eq(destroyed_count, 7);
		
		FINISH_TEST;
	}


	void testProcessRunnerQueueAndRunLongProcessTerminateAllProcesses() {
		BEGIN_TEST;

		destroyed_count = 0;

		ProcessRunner* pm = new ProcessRunner("PM", 4);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);		

		pm->run();
		
		pm->waitUntilStarted();

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);	

		
		Boolean retVal;
		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = pm->queueProcess(p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = pm->queueProcess(p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = pm->queueProcess(p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4, 1);
		retVal = pm->queueProcess(p4);
		ass_true(retVal);

		((LongTestProcess*)(p1.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p2.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p3.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p4.ptr()))->waitUntilStarted();

		ass_eq(p1->state(), Process::kPSRunning);
		ass_eq(p2->state(), Process::kPSRunning);
		ass_eq(p3->state(), Process::kPSRunning);
		ass_eq(p4->state(), Process::kPSRunning);

		

		/* Terminate all the processes */
		pm->terminateAllProcesses();

		((LongTestProcess*)(p1.ptr()))->waitForFinished();
		((LongTestProcess*)(p2.ptr()))->waitForFinished();
		((LongTestProcess*)(p3.ptr()))->waitForFinished();
		((LongTestProcess*)(p4.ptr()))->waitForFinished();

		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();
		

	   sleep(1);

		p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = pm->queueProcess(p1);
		ass_true(retVal);
		p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = pm->queueProcess(p2);
		ass_true(retVal);
		p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = pm->queueProcess(p3);
		ass_true(retVal);
		p4 = LongTestProcess::create("Long Process 4", 4, 1);
		retVal = pm->queueProcess(p4);
		ass_true(retVal);

		((LongTestProcess*)(p1.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p2.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p3.ptr()))->waitUntilStarted();
		((LongTestProcess*)(p4.ptr()))->waitUntilStarted();

		ass_eq(p1->state(), Process::kPSRunning);
		ass_eq(p2->state(), Process::kPSRunning);
		ass_eq(p3->state(), Process::kPSRunning);
		ass_eq(p4->state(), Process::kPSRunning);

		p1.setNull();
		p2.setNull();
		p3.setNull();
		p4.setNull();

		retVal = pm->queueProcess(LongTestProcess::create("Long Process 5", 5, 1));
		ass_true(retVal);
		retVal = pm->queueProcess(LongTestProcess::create("Long Process 6", 6, 4));
		ass_true(retVal);
		retVal = pm->queueProcess(LongTestProcess::create("Long Process 7", 7, 2));
		ass_true(retVal);
		retVal = pm->queueProcess(LongTestProcess::create("Long Process 8", 8, 3));
		ass_true(retVal);

		pm->terminateAllProcesses();

		while (pm->processCount() > 0 || pm->hasRemoved()) {
			sleep(1);
			std::cout << "ProcessCount: " << pm->processCount() << std::endl << std::flush;
			std::cout << "hasRemoved: " << pm->hasRemoved() << std::endl << std::flush;
		}

		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 12);	
		
		pm->terminateProcessRunner();
		pm->waitForTermination();
		ass_eq(destroyed_count, 12);	

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM"), 0);
		ass_eq(pm->oID(), crc32("PM"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		delete pm;		
		
		FINISH_TEST;
	}
	
} // namespace cc

int main(int argc, char** argv) {
	cc::testRunAndTerminateProcessRunner();
	cc::testProcessRunnerQueueAndRunProcess();
	cc::testProcessRunnerQueueAndRunLongProcessAndTerminate();	
	cc::testProcessRunnerQueueAndRunLongProcessGetAndTerminateProcess();
	cc::testProcessRunnerQueueAndRunLongProcessPauseAndResumeProcess();
	cc::testProcessRunnerQueueAndRunLongProcessAddChildProcess();	
	cc::testProcessRunnerQueueAndRunLongProcessTerminateAllProcesses();
	
	return 0;
}
