#include "core/testcore.h"
#include "core/threading/processmanager.h"
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

	void testCreateAndDestroyProcessManager() {
		BEGIN_TEST;

		ProcessManager* runner = new ProcessManager();
		delete runner;

		runner = new ProcessManager(8);
		ass_eq(runner->maxProcessRunners(), 8);
		ass_eq(runner->numProcessRunners(), 0);

		delete runner;		

		FINISH_TEST;
	}

	void testProcessManagerCreateAndGetProcessRunner() {
		BEGIN_TEST;

		ProcessManager* runner = new ProcessManager(2);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 2);
		
		retVal = runner->createProcessRunner("PM3", 5);
		ass_false(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 2);

		ProcessRunner* pm = runner->getProcessRunner("PM1");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);

		pm = runner->getProcessRunner("PM2");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 48);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);	

		pm = runner->getProcessRunner("PM3");
		ass_eq(pm, NIL);	  
		
		
		delete runner;		

		FINISH_TEST;
	}

	void testProcessManagerStartProcessRunners() {
		BEGIN_TEST;

		ProcessManager* runner = new ProcessManager(2);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 2);
		
		retVal = runner->createProcessRunner("PM3", 5);
		ass_false(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 2);

		ProcessRunner* pm = runner->getProcessRunner("PM1");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);

		pm = runner->getProcessRunner("PM2");
	   ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 48);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);	

		pm = runner->getProcessRunner("PM3");
		ass_eq(pm, NIL);

		runner->startProcessRunners();
		
		pm = runner->getProcessRunner("PM1");
	   ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);

		pm = runner->getProcessRunner("PM2");
	   ass_true(pm->isRunning());
		ass_false(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->state(), ProcessRunner::kPMSRunning);
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 48);
		ass_true(pm->messageQueue()->isEmpty());
		
		
		ass_eq(destroyed_count, 0);	

		pm = runner->getProcessRunner("PM3");
		ass_eq(pm, NIL);
		
		
		delete runner;		

		FINISH_TEST;
	}

	void testProcessManagerQueueProcess() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessManager* runner = new ProcessManager(2);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 2);

		runner->startProcessRunners();		

		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = runner->queueProcess("PM1", p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = runner->queueProcess("PM2", p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = runner->queueProcess("PM1", p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4);
		retVal = runner->queueProcess("PM2", p4);
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

		retVal = runner->queueProcess("PM3", LongTestProcess::create("Process 5", 5, 1));
		ass_false(retVal);

		
		ass_eq(destroyed_count, 1);		
		
		delete runner;

		ass_eq(destroyed_count, 5);		

		FINISH_TEST;
	}

	void testProcessManagerTerminateProcessRunner() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessManager* runner = new ProcessManager(2);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 2);

		runner->startProcessRunners();		

		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = runner->queueProcess("PM1", p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = runner->queueProcess("PM2", p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = runner->queueProcess("PM1", p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4);
		retVal = runner->queueProcess("PM2", p4);
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

		ProcessRunner* pm = runner->getProcessRunner("PM1");
		runner->terminateProcessRunner("PM1");
		pm->waitForTermination();

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 2);

		sleep(1);		

		pm = runner->getProcessRunner("PM2");
		runner->terminateProcessRunner("PM2");
		pm->waitForTermination();

		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 48);
		ass_true(pm->messageQueue()->isEmpty());
		
		
		
		ass_eq(destroyed_count, 4);		
		
		delete runner;

		ass_eq(destroyed_count, 4);		

		FINISH_TEST;
	}

	void testProcessManagerTerminateAllProcessRunners() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessManager* runner = new ProcessManager(2);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 2);
		ass_eq(runner->numProcessRunners(), 2);

		runner->waitForAllProcessRunnersToTerminate();	
		runner->startProcessRunners();		

		ProcessPtr p1 = LongTestProcess::create("Long Process 1", 1, 1);		
		retVal = runner->queueProcess("PM1", p1);
		ass_true(retVal);
		ProcessPtr p2 = LongTestProcess::create("Long Process 2", 2, 3);		
		retVal = runner->queueProcess("PM2", p2);
		ass_true(retVal);
		ProcessPtr p3 = LongTestProcess::create("Long Process 3", 3, 1);
		retVal = runner->queueProcess("PM1", p3);
		ass_true(retVal);
		ProcessPtr p4 = LongTestProcess::create("Long Process 4", 4);
		retVal = runner->queueProcess("PM2", p4);
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

		runner->terminateAllProcessRunners();
		runner->waitForAllProcessRunnersToTerminate();

		ass_eq(destroyed_count, 4);

		ProcessRunner* pm = runner->getProcessRunner("PM1");
		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM1"), 0);
		ass_eq(pm->oID(), crc32("PM1"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 4);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 6);
		ass_true(pm->messageQueue()->isEmpty());

		pm = runner->getProcessRunner("PM2");
		ass_eq(pm->state(), ProcessRunner::kPMSTerminated);
		ass_false(pm->isRunning());
		ass_true(pm->isStopped());
		ass_eq(strcmp(pm->name(), "PM2"), 0);
		ass_eq(pm->oID(), crc32("PM2"));
		ass_eq(pm->processCount(), 0);
		ass_eq(pm->inputQueue()->capacity(), 32);
		ass_true(pm->inputQueue()->isEmpty());
		ass_false(pm->hasRemoved());
		ass_eq(pm->messageQueue()->capacity(), 48);
		ass_true(pm->messageQueue()->isEmpty());
		
		delete runner;

		ass_eq(destroyed_count, 4);		

		FINISH_TEST;
	}

	void testProcessManagerGetAndTerminateProcesses() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessManager* runner = new ProcessManager(4);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 2);

		retVal = runner->createProcessRunner("PM3");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 3);

		retVal = runner->createProcessRunner("PM4");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 4);

		runner->startProcessRunners();		

		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 1", 1, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 2", 2, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 3", 3, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 4", 4, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 5", 5, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 6", 6, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 7", 7, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 8", 8, 2));
		ass_true(retVal);
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 9", 9, 4));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 10", 10, 2));
		ass_true(retVal);

		while (runner->getProcess("Long Process 1", "PM1").isNull() ||
				 runner->getProcess("Long Process 1", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 2", "PM2").isNull() ||
				 runner->getProcess("Long Process 2", "PM2")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 3", "PM1").isNull() ||
				 runner->getProcess("Long Process 3", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 4", "PM2").isNull() ||
				 runner->getProcess("Long Process 4", "PM2")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 5", "PM3").isNull() ||
				 runner->getProcess("Long Process 5", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 6", "PM3").isNull() ||
				 runner->getProcess("Long Process 6", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 7", "PM4").isNull() ||
				 runner->getProcess("Long Process 7", "PM4")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 8", "PM3").isNull() ||
				 runner->getProcess("Long Process 8", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 9", "PM1").isNull() ||
				 runner->getProcess("Long Process 9", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 10", "PM4").isNull() ||
				 runner->getProcess("Long Process 10", "PM4")->state() != Process::kPSRunning)
		{ usleep(10000); }

		ass_eq(runner->getProcess("Long Process 9", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 1", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 2", "PM2")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 7", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 8", "PM3")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 3", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 10", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 4", "PM2")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 6", "PM3")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 5", "PM3")->state(), Process::kPSRunning);

		/* Test the first get method, specifying the process manager */
		ass_eq(runner->getProcess("Long Process 9", "PM1")->pID(), crc32("Long Process 9"));
		ass_eq(runner->getProcess("Long Process 1", "PM1")->pID(), crc32("Long Process 1"));
		ass_eq(runner->getProcess("Long Process 2", "PM2")->pID(), crc32("Long Process 2"));
		ass_eq(runner->getProcess("Long Process 7", "PM4")->pID(), crc32("Long Process 7"));
		ass_eq(runner->getProcess("Long Process 8", "PM3")->pID(), crc32("Long Process 8"));
		ass_eq(runner->getProcess("Long Process 3", "PM1")->pID(), crc32("Long Process 3"));
		ass_eq(runner->getProcess("Long Process 10", "PM4")->pID(), crc32("Long Process 10"));
		ass_eq(runner->getProcess("Long Process 4", "PM2")->pID(), crc32("Long Process 4"));
		ass_eq(runner->getProcess("Long Process 6", "PM3")->pID(), crc32("Long Process 6"));
		ass_eq(runner->getProcess("Long Process 5", "PM3")->pID(), crc32("Long Process 5"));

		/* Test the second get method, not specifying the process manager */
		ass_eq(runner->getProcess("Long Process 9")->pID(), crc32("Long Process 9"));
		ass_eq(runner->getProcess("Long Process 1")->pID(), crc32("Long Process 1"));
		ass_eq(runner->getProcess("Long Process 2")->pID(), crc32("Long Process 2"));
		ass_eq(runner->getProcess("Long Process 7")->pID(), crc32("Long Process 7"));
		ass_eq(runner->getProcess("Long Process 8")->pID(), crc32("Long Process 8"));
		ass_eq(runner->getProcess("Long Process 3")->pID(), crc32("Long Process 3"));
		ass_eq(runner->getProcess("Long Process 10")->pID(), crc32("Long Process 10"));
		ass_eq(runner->getProcess("Long Process 4")->pID(), crc32("Long Process 4"));
		ass_eq(runner->getProcess("Long Process 6")->pID(), crc32("Long Process 6"));
		ass_eq(runner->getProcess("Long Process 5")->pID(), crc32("Long Process 5"));

		/* Make a few processes change state and finish */
		((LongTestProcess*)(runner->getProcess("Long Process 2").ptr()))->setProcessAsFailed();
		((LongTestProcess*)(runner->getProcess("Long Process 7").ptr()))->setProcessAsSucceeded();
		((LongTestProcess*)(runner->getProcess("Long Process 8").ptr()))->setProcessAsFailed();
		((LongTestProcess*)(runner->getProcess("Long Process 3").ptr()))->setProcessAsSucceeded();
		while (runner->getProcess("Long Process 2").notNull()) { usleep(10000); }		
		while (runner->getProcess("Long Process 7").notNull()) { usleep(10000); }
		while (runner->getProcess("Long Process 8").notNull()) { usleep(10000); }
		while (runner->getProcess("Long Process 3").notNull()) { usleep(10000); }

		/* Terminate some processes */
		runner->terminateProcess("Long Process 9");		
		runner->terminateProcess("Long Process 1", "PM1");		
		runner->terminateProcess("Long Process 10", "PM4");		
		runner->terminateProcess("Long Process 4");		
		runner->terminateProcess("Long Process 6");		
		runner->terminateProcess("Long Process 5", "PM3");		

		sleep(1);		
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 11", 11, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 12", 12, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 13", 13, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 14", 14, 1));
		ass_true(retVal);
			
		delete runner;

		ass_eq(destroyed_count, 14);		

		FINISH_TEST;
	}

	void testProcessManagerPauseAndResumeProcesses() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessManager* runner = new ProcessManager(4);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 2);

		retVal = runner->createProcessRunner("PM3");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 3);

		retVal = runner->createProcessRunner("PM4");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 4);

		runner->startProcessRunners();		

		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 1", 1, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 2", 2, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 3", 3, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 4", 4, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 5", 5, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 6", 6, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 7", 7, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 8", 8, 2));
		ass_true(retVal);
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 9", 9, 4));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 10", 10, 2));
		ass_true(retVal);

		while (runner->getProcess("Long Process 1", "PM1").isNull() ||
				 runner->getProcess("Long Process 1", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 2", "PM2").isNull() ||
				 runner->getProcess("Long Process 2", "PM2")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 3", "PM1").isNull() ||
				 runner->getProcess("Long Process 3", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 4", "PM2").isNull() ||
				 runner->getProcess("Long Process 4", "PM2")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 5", "PM3").isNull() ||
				 runner->getProcess("Long Process 5", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 6", "PM3").isNull() ||
				 runner->getProcess("Long Process 6", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 7", "PM4").isNull() ||
				 runner->getProcess("Long Process 7", "PM4")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 8", "PM3").isNull() ||
				 runner->getProcess("Long Process 8", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 9", "PM1").isNull() ||
				 runner->getProcess("Long Process 9", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 10", "PM4").isNull() ||
				 runner->getProcess("Long Process 10", "PM4")->state() != Process::kPSRunning)
		{ usleep(10000); }

		ass_eq(runner->getProcess("Long Process 9", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 1", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 2", "PM2")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 7", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 8", "PM3")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 3", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 10", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 4", "PM2")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 6", "PM3")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 5", "PM3")->state(), Process::kPSRunning);

		/* Pause a few of the processes. */
		runner->pauseProcess("Long Process 9", "PM1");
		runner->pauseProcess("Long Process 8");
		runner->pauseProcess("Long Process 4", "PM2");
		runner->pauseProcess("Long Process 10", "PM4");
		runner->pauseProcess("Long Process 1");

		while (runner->getProcess("Long Process 9", "PM1")->state() == Process::kPSRunning) { usleep(10000); }
		while (runner->getProcess("Long Process 8", "PM3")->state() == Process::kPSRunning) { usleep(10000); }
		while (runner->getProcess("Long Process 4", "PM2")->state() == Process::kPSRunning) { usleep(10000); }
		while (runner->getProcess("Long Process 10", "PM4")->state() == Process::kPSRunning) { usleep(10000); }
		while (runner->getProcess("Long Process 1", "PM1")->state() == Process::kPSRunning) { usleep(10000); }

		ass_eq(runner->getProcess("Long Process 9", "PM1")->state(), Process::kPSPaused);
		ass_eq(runner->getProcess("Long Process 8", "PM3")->state(), Process::kPSPaused);
		ass_eq(runner->getProcess("Long Process 4", "PM2")->state(), Process::kPSPaused);
		ass_eq(runner->getProcess("Long Process 10", "PM4")->state(), Process::kPSPaused);
		ass_eq(runner->getProcess("Long Process 1", "PM1")->state(), Process::kPSPaused);

		runner->resumeProcess("Long Process 9");
		runner->resumeProcess("Long Process 10", "PM4");

		while (runner->getProcess("Long Process 9", "PM1")->state() != Process::kPSRunning) { usleep(10000); }
		while (runner->getProcess("Long Process 10", "PM4")->state() != Process::kPSRunning) { usleep(10000); }

		ass_eq(runner->getProcess("Long Process 9", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 8", "PM3")->state(), Process::kPSPaused);
		ass_eq(runner->getProcess("Long Process 4", "PM2")->state(), Process::kPSPaused);
		ass_eq(runner->getProcess("Long Process 10", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 1", "PM1")->state(), Process::kPSPaused);

		runner->resumeProcess("Long Process 8");
		runner->resumeProcess("Long Process 4", "PM2");
		runner->resumeProcess("Long Process 1");

		while (runner->getProcess("Long Process 8", "PM3")->state() != Process::kPSRunning) { usleep(10000); }
		while (runner->getProcess("Long Process 4", "PM2")->state() != Process::kPSRunning) { usleep(10000); }
		while (runner->getProcess("Long Process 1", "PM1")->state() != Process::kPSRunning) { usleep(10000); }

		ass_eq(runner->getProcess("Long Process 9", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 8", "PM3")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 4", "PM2")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 10", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 1", "PM1")->state(), Process::kPSRunning);
		
		/* Terminate some processes */
		runner->terminateProcess("Long Process 9");		
		runner->terminateProcess("Long Process 1", "PM1");		
		runner->terminateProcess("Long Process 10", "PM4");		
		runner->terminateProcess("Long Process 4");		
		runner->terminateProcess("Long Process 6");		
		runner->terminateProcess("Long Process 5", "PM3");		

		sleep(1);		
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 11", 11, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 12", 12, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 13", 13, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 14", 14, 1));
		ass_true(retVal);
			
		delete runner;

		ass_eq(destroyed_count, 14);		

		FINISH_TEST;
	}

	void testProcessManagerTerminateAllProcesses() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessManager* runner = new ProcessManager(4);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 0);

		Boolean retVal;		
		retVal = runner->createProcessRunner("PM1", 4);
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 1);
		
		retVal = runner->createProcessRunner("PM2");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 2);

		retVal = runner->createProcessRunner("PM3");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 3);

		retVal = runner->createProcessRunner("PM4");
		ass_true(retVal);
		ass_eq(runner->maxProcessRunners(), 4);
		ass_eq(runner->numProcessRunners(), 4);

		runner->startProcessRunners();		

		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 1", 1, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 2", 2, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 3", 3, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 4", 4, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 5", 5, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 6", 6, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 7", 7, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 8", 8, 2));
		ass_true(retVal);
		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 9", 9, 4));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 10", 10, 2));
		ass_true(retVal);

		while (runner->getProcess("Long Process 1", "PM1").isNull() ||
				 runner->getProcess("Long Process 1", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 2", "PM2").isNull() ||
				 runner->getProcess("Long Process 2", "PM2")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 3", "PM1").isNull() ||
				 runner->getProcess("Long Process 3", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 4", "PM2").isNull() ||
				 runner->getProcess("Long Process 4", "PM2")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 5", "PM3").isNull() ||
				 runner->getProcess("Long Process 5", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 6", "PM3").isNull() ||
				 runner->getProcess("Long Process 6", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 7", "PM4").isNull() ||
				 runner->getProcess("Long Process 7", "PM4")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 8", "PM3").isNull() ||
				 runner->getProcess("Long Process 8", "PM3")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 9", "PM1").isNull() ||
				 runner->getProcess("Long Process 9", "PM1")->state() != Process::kPSRunning)
		{ usleep(10000); }
		while (runner->getProcess("Long Process 10", "PM4").isNull() ||
				 runner->getProcess("Long Process 10", "PM4")->state() != Process::kPSRunning)
		{ usleep(10000); }

		ass_eq(runner->getProcess("Long Process 9", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 1", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 2", "PM2")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 7", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 8", "PM3")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 3", "PM1")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 10", "PM4")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 4", "PM2")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 6", "PM3")->state(), Process::kPSRunning);
		ass_eq(runner->getProcess("Long Process 5", "PM3")->state(), Process::kPSRunning);

		runner->terminateAllProcesses("PM1");
		ProcessRunner* pm = runner->getProcessRunner("PM1");
		while (pm->processCount() > 0) { usleep(10000); }

		sleep(1);
		ass_eq(runner->getProcessRunner("PM1")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM2")->processCount(), 2);		
		ass_eq(runner->getProcessRunner("PM3")->processCount(), 3);
		ass_eq(runner->getProcessRunner("PM4")->processCount(), 2);
		ass_eq(destroyed_count, 3);	

		runner->terminateAllProcesses("PM3");
		pm = runner->getProcessRunner("PM3");
		while (pm->processCount() > 0) { usleep(10000); }

		sleep(1);
		ass_eq(runner->getProcessRunner("PM1")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM2")->processCount(), 2);		
		ass_eq(runner->getProcessRunner("PM3")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM4")->processCount(), 2);
		ass_eq(destroyed_count, 6);	

		runner->terminateAllProcesses("PM2");
		pm = runner->getProcessRunner("PM2");
		while (pm->processCount() > 0) { usleep(10000); }

		sleep(1);
		ass_eq(runner->getProcessRunner("PM1")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM2")->processCount(), 0);		
		ass_eq(runner->getProcessRunner("PM3")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM4")->processCount(), 2);
		ass_eq(destroyed_count, 8);	

		runner->terminateAllProcesses("PM4");
		pm = runner->getProcessRunner("PM4");
		while (pm->processCount() > 0) { usleep(10000); }

		sleep(1);
		ass_eq(runner->getProcessRunner("PM1")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM2")->processCount(), 0);		
		ass_eq(runner->getProcessRunner("PM3")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM4")->processCount(), 0);
		ass_eq(destroyed_count, 10);

		retVal = runner->queueProcess("PM1", LongTestProcess::create("Long Process 11", 11, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM2", LongTestProcess::create("Long Process 12", 12, 3));
		ass_true(retVal);
		retVal = runner->queueProcess("PM3", LongTestProcess::create("Long Process 13", 13, 1));
		ass_true(retVal);
		retVal = runner->queueProcess("PM4", LongTestProcess::create("Long Process 14", 14, 1));
		ass_true(retVal);

				
		runner->terminateAllProcesses();
		
		pm = runner->getProcessRunner("PM1");
		while (pm->processCount() > 0) { usleep(10000); }
		pm = runner->getProcessRunner("PM2");
		while (pm->processCount() > 0) { usleep(10000); }
		pm = runner->getProcessRunner("PM3");
		while (pm->processCount() > 0) { usleep(10000); }
		pm = runner->getProcessRunner("PM4");
		while (pm->processCount() > 0) { usleep(10000); }

		sleep(1);
		
		ass_eq(runner->getProcessRunner("PM1")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM2")->processCount(), 0);		
		ass_eq(runner->getProcessRunner("PM3")->processCount(), 0);
		ass_eq(runner->getProcessRunner("PM4")->processCount(), 0);
		ass_eq(destroyed_count, 14);

		delete runner;

		ass_eq(destroyed_count, 14);	

		FINISH_TEST;
	}

} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroyProcessManager();
	cc::testProcessManagerCreateAndGetProcessRunner();
	cc::testProcessManagerStartProcessRunners();
	cc::testProcessManagerQueueProcess();
	cc::testProcessManagerTerminateProcessRunner();
	cc::testProcessManagerTerminateAllProcessRunners();
	cc::testProcessManagerGetAndTerminateProcesses();
	cc::testProcessManagerPauseAndResumeProcesses();
	cc::testProcessManagerTerminateAllProcesses();	
	
	return 0;
}
