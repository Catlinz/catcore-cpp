#include "core/testcore.h"
#include "core/threading/processrunner.h"

namespace cc {

	I32 destroyed_count = 0;	

	class TestProcess : public Process {		
	  public:
		TestProcess()
			: Process(), m_ran(0), m_val(0) {}

		TestProcess(OID pid, I32 val, U32 timeToRun = 100)
			: Process(pid), m_ran(0), m_val(val), m_timeToRun(timeToRun) {}

		TestProcess(const Char* name, I32 val, U32 timeToRun = 100)
			: Process(name), m_ran(0), m_val(val), m_timeToRun(timeToRun) {}

		~TestProcess() {
			DMSG("Test Process " << m_val << " destroyed.");
			destroyed_count++;			
		}		

		void onFailure() {
			DMSG("Test process (" << m_val << ") failed!");
		}
		void onInitialize() {
			DMSG("Test process (" << m_val << ") initialized!");
		}
		void onPause() {
			DMSG("Test process (" << m_val << ") paused!");
		}
		void onResume() {
			DMSG("Test process (" << m_val << ") resumed!");
		}
		void onSuccess() {
			DMSG("Test process (" << m_val << ") succeeded!");
		}
		void onTermination() {
			DMSG("Test process (" << m_val << ") terminated!");
		}

		void run(U32 time) {
			DMSG("Process " << m_val << " recieved " << time << " time.");
			m_ran += time;
			setPriorityModifier(priorityModifier() + 1);			
			if (m_ran >= m_timeToRun) {
				succeeded();
			}			
		}

		inline U32 ran() const { return m_ran; }
		inline I32 val() const { return m_val; }
		inline U32 timeToRun() const { return m_timeToRun; }		

		inline static ProcessPtr create(const Char* name, I32 val, U32 timeToRun = 100) {
			return ProcessPtr(new TestProcess(name, val, timeToRun));
		}		

	  private:
		U32 m_ran;		
		I32 m_val;
		U32 m_timeToRun;		
	};

	void testCreateAndDestroyProcessRunner() {
		BEGIN_TEST;

		ProcessRunner* pm1 = new ProcessRunner();
		ass_false(pm1->isRunning());
		ass_true(pm1->isStopped());
		ass_eq(pm1->name(), NIL);
		ass_eq(pm1->oID(), 0);
		ass_eq(pm1->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm1->inputQueue()->capacity(), 0);
		ass_eq(pm1->messageQueue()->capacity(), 0);
		ass_eq(pm1->processCount(), 0);
		ass_eq(pm1->numFreeNodes(), 0);
		ass_eq(pm1->numUsedNodes(), 0);	

		ProcessRunner* pm2 = new ProcessRunner("PM2");
		ass_false(pm2->isRunning());
		ass_true(pm2->isStopped());
		ass_eq(strcmp(pm2->name(), "PM2"), 0);
		ass_eq(pm2->oID(), crc32("PM2"));
		ass_eq(pm2->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm2->processCount(), 0);
		ass_eq(pm2->inputQueue()->capacity(), 32);
		ass_true(pm2->inputQueue()->isEmpty());
		ass_eq(pm2->messageQueue()->capacity(), 48);
		ass_true(pm2->messageQueue()->isEmpty());
		ass_false(pm2->hasRemoved());
		ass_false(pm2->hasRunning());
		ass_false(pm2->hasPaused());
		ass_true(pm2->hasFreeRoom());
		ass_eq(pm2->numFreeNodes(), 96);
		ass_eq(pm2->numUsedNodes(), 0);	
		
		ProcessRunner* pm3 = new ProcessRunner("PM3", 8);
		ass_false(pm3->isRunning());
		ass_true(pm3->isStopped());
		ass_eq(strcmp(pm3->name(), "PM3"), 0);
		ass_eq(pm3->oID(), crc32("PM3"));
		ass_eq(pm3->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(pm3->processCount(), 0);
		ass_eq(pm3->inputQueue()->capacity(), 8);
		ass_true(pm3->inputQueue()->isEmpty());   
		ass_eq(pm3->messageQueue()->capacity(), 12);
		ass_true(pm3->messageQueue()->isEmpty());
		ass_false(pm3->hasRemoved());
		ass_false(pm3->hasRunning());
		ass_false(pm3->hasPaused());
		ass_true(pm3->hasFreeRoom());
		ass_eq(pm3->numFreeNodes(), 24);
		ass_eq(pm3->numUsedNodes(), 0);		
		
		
		delete pm1;
		delete pm2;
		delete pm3;		
		
		FINISH_TEST;
	}

	void testProcessRunnerQueueProcess() {
		BEGIN_TEST;

		destroyed_count = 0;
		
		ProcessRunner* m = new ProcessRunner("PM", 4);
		ass_false(m->isRunning());
		ass_true(m->isStopped());
		ass_eq(strcmp(m->name(), "PM"), 0);
		ass_eq(m->oID(), crc32("PM"));
		ass_eq(m->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_false(m->hasRunning());
		ass_false(m->hasPaused());
		ass_true(m->hasFreeRoom());
		ass_eq(m->numFreeNodes(), 12);
		ass_eq(m->numUsedNodes(), 0);
		
		Boolean retVal;

		retVal = m->queueProcess(TestProcess::create("Process 1", 1));
		ass_true(retVal);
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		retVal = m->queueProcess(TestProcess::create("Process 2", 2));
		ass_true(retVal);
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		
		retVal = m->queueProcess(TestProcess::create("Process 3", 3));
		ass_true(retVal);
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		
		retVal = m->queueProcess(TestProcess::create("Process 4", 4));
		ass_true(retVal);
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		
		ass_eq(destroyed_count, 0);
		
		retVal = m->queueProcess(TestProcess::create("Process 5", 5));
		ass_false(retVal);

		ass_eq(destroyed_count, 1);

		delete m;
		ass_eq(destroyed_count, 5);		
		
		FINISH_TEST;
	}

	void testProcessRunnerPostMessages() {
		BEGIN_TEST;

		ProcessRunner* m = new ProcessRunner("PM", 4);
		ass_false(m->isRunning());
		ass_true(m->isStopped());
		ass_eq(strcmp(m->name(), "PM"), 0);
		ass_eq(m->oID(), crc32("PM"));
		ass_eq(m->state(), ProcessRunner::kPMSNotStarted);
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_false(m->hasRunning());
		ass_false(m->hasPaused());
		ass_true(m->hasFreeRoom());
		ass_eq(m->numFreeNodes(), 12);
		ass_eq(m->numUsedNodes(), 0);

		Boolean retVal = false;
		retVal = m->terminateProcess(342);
		ass_true(retVal);		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_false(m->messageQueue()->isEmpty());
		
		retVal = m->terminateAllProcesses();
		ass_true(retVal);
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_false(m->messageQueue()->isEmpty());

		retVal = m->terminateProcessRunner();
		ass_true(retVal);
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_false(m->messageQueue()->isEmpty());

		SimpleQueue<ProcessRunner::PMMessage>::Iterator itr = m->messageQueue()->begin();			
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateProcess);
		ass_eq(itr.val().pid, 342);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateAllProcesses);
		ass_eq(itr.val().pid, 0);

		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateProcessRunner);
		ass_eq(itr.val().pid, 0);

		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());

		retVal = m->terminateProcess(66);
		ass_true(retVal);		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_false(m->messageQueue()->isEmpty());

		retVal = m->pauseProcess(88);
		ass_true(retVal);		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_false(m->messageQueue()->isEmpty());

		retVal = m->resumeProcess(99);
		ass_true(retVal);		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_false(m->messageQueue()->isEmpty());
		
		retVal = m->resumeProcess(999);
		ass_false(retVal);

		itr = m->messageQueue()->begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateProcess);
		ass_eq(itr.val().pid, 342);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateAllProcesses);
		ass_eq(itr.val().pid, 0);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateProcessRunner);
		ass_eq(itr.val().pid, 0);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateProcess);
		ass_eq(itr.val().pid, 66);

		itr.next();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMPauseProcess);
		ass_eq(itr.val().pid, 88);

		itr.next();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMResumeProcess);
		ass_eq(itr.val().pid, 99);

		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());		
		ass_eq(itr.val().type, ProcessRunner::kPMMTerminateProcess);
		ass_eq(itr.val().pid, 342);
		
		delete m;		
		
		FINISH_TEST;
	}

	void testProcessRunnerRunProcessesSimple() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessRunner* m = new ProcessRunner("PM", 4);

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		

		m->runProcesses(100);	/* Process 1 should finish */	
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->runProcesses(100);		/* Process 2 and 3 should finish */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);		

		m->runProcesses(100);		/* Process 4 should finish, Process 1 destroyed */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 1);

		m->runProcesses(100);		/* Process 2, 3, destroyed */
		ass_eq(m->processCount(), 1);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 3);

		m->runProcesses(100);		/* Process 4, destroyed */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		
		ass_eq(destroyed_count, 4);
		
		delete m;		

		FINISH_TEST;
	}

	void testProcessRunnerRunProcessesComplex() {
		BEGIN_TEST;

		destroyed_count = 0;		

		ProcessRunner* m = new ProcessRunner("PM", 4);

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		
		DMSG(" ---- Run #1");		
		m->runProcesses(100);	/* Process 1 should finish */
		/* finished: 1
		 * running: 2,3, 4
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->queueProcess(TestProcess::create("Process 5", 5, 200));
		m->queueProcess(TestProcess::create("Process 6", 6, 100));		

		DMSG(" ---- Run #2");
		m->runProcesses(100); /* Process 2, 3, 6 should finish */
		/* finished: 1, 2, 3, 6.
		 * running: 4, 5
		 * queued: 
		 */
		ass_eq(m->processCount(), 6);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		DMSG(" ---- Run #3");
		ass_eq(destroyed_count, 0);		
		m->runProcesses(100);		/* Process 4, 5 should finish, Process 1 destroyed */
		/* finished: 2, 3, 6, 4, 5
		 * destroyed: 1
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 1);

		DMSG(" ---- Run #4");
		m->runProcesses(100);  	/* Process 2, 3, 6, destroyed */
		/* finished: 4, 5
		 * destroyed: 1, 2, 3, 6
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);

		DMSG(" ---- Run #5");
		m->runProcesses(100);		/* Process 4, 5, destroyed */
		/* finished:
		 * destroyed: 1, 2, 3, 6, 4, 5
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		DMSG(" ---- Run #6");
		m->runProcesses(100);		/*  */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 5, 6
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		m->queueProcess(TestProcess::create("Process 7", 7, 200));
		m->queueProcess(TestProcess::create("Process 8", 8, 200));
		m->queueProcess(TestProcess::create("Process 9", 9, 200));

		DMSG(" ---- Run #7");
		m->runProcesses(100);  
		/* finished:
		 * destroyed: 1, 2, 3, 4, 5, 6
		 * running: 7, 8, 9
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		DMSG(" ---- Run #8");
		m->runProcesses(100);  /* Process 7, 8, 9 finished */
		/* finished: 7, 8, 9
		 * destroyed: 1, 2, 3, 4, 5, 6,
		 * running: 
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);
		
		m->queueProcess(TestProcess::create("Process 10", 10, 100));
		
		DMSG(" ---- Run #9");		
		m->runProcesses(100);  /* Process 10 finished */
		/* finished: 7, 8, 9, 10
		 * destroyed: 1, 2, 3, 4, 5, 6,
		 * running:  
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		m->queueProcess(TestProcess::create("Process 11", 11, 100));
		m->queueProcess(TestProcess::create("Process 12", 12, 100));
		m->queueProcess(TestProcess::create("Process 13", 13, 100));
		m->queueProcess(TestProcess::create("Process 14", 14, 100));
		
		DMSG(" ---- Run #10");		
		m->runProcesses(100);  /* Process 7, 8, 9 destroyed, 11, 12, 13, 14 finished */
		/* finished: 10, 11, 12, 13, 14
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9
		 * running:  
		 * queued: 
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 9);

		DMSG(" ---- Run #11");		
		m->runProcesses(100);  /* Process 10 destroyed, 14 finished */
		/* finished: 11, 12, 13, 14
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10
		 * running:  
		 * queued: 
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 10);

		DMSG(" ---- Run #12");		
		m->runProcesses(100);  /* Process 11, 12, 13, 14 destroyed */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14
		 * running:  
		 * queued: 
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);

		m->queueProcess(TestProcess::create("Process 15", 15, 1000));
		m->queueProcess(TestProcess::create("Process 16", 16, 1000));
		m->queueProcess(TestProcess::create("Process 17", 17, 1000));
		m->queueProcess(TestProcess::create("Process 18", 18, 1000));

		DMSG(" ---- Run #13");		
		m->runProcesses(100);  /* Process 15, 16, 17, 18 started */
		/* finished: 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14
		 * running: 15, 16, 17, 18
		 * queued: 
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);

		m->queueProcess(TestProcess::create("Process 19", 19, 600));
		m->queueProcess(TestProcess::create("Process 20", 20, 600));
		m->queueProcess(TestProcess::create("Process 21", 21, 600));
		m->queueProcess(TestProcess::create("Process 22", 22, 600));

		DMSG(" ---- Run #14");		
		m->runProcesses(100);  /* Process 19, 20, 21, 22 started */
		/* finished: 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14
		 * running: 15, 16, 17, 18, 19, 20, 21, 22
		 * queued: 
		 */
		ass_eq(m->processCount(), 8);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);
		
		m->queueProcess(TestProcess::create("Process 23", 23, 300));
		m->queueProcess(TestProcess::create("Process 24", 24, 300));
		m->queueProcess(TestProcess::create("Process 25", 25, 300));
		m->queueProcess(TestProcess::create("Process 26", 26, 300));

		DMSG(" ---- Run #15");		
		m->runProcesses(100);  /* Process 23, 24, 25, 26 started */
		/* finished: 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14
		 * running: 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
		 * queued: 
		 */
		ass_eq(m->processCount(), 12);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);
		
		m->queueProcess(TestProcess::create("Process 27", 27, 100));
		m->queueProcess(TestProcess::create("Process 28", 28, 100));
		m->queueProcess(TestProcess::create("Process 29", 29, 100));
		m->queueProcess(TestProcess::create("Process 30", 30, 100));

		DMSG(" ---- Run #16");		
		m->runProcesses(100);  /* Process 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 finished */
		/* finished: 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14
		 * running: 
		 * queued: 27, 28, 29, 30
		 */
		ass_eq(m->processCount(), 12);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);

		DMSG(" ---- Run #17");		
		m->runProcesses(100);  /* Process 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 finished */
		/* finished: 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14
		 * running: 
		 * queued: 27, 28, 29, 30
		 */
		ass_eq(m->processCount(), 12);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);

		DMSG(" ---- Run #18");		
		m->runProcesses(100);  /* Process 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 destroyed, 27, 28, 29 30 finished */
		/* finished: 27, 28, 29, 30 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
		 * running: 
		 * queued: 
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 26);

		DMSG(" ---- Run #19");		
		m->runProcesses(100);  /* */
		/* finished: 27, 28, 29, 30 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
		 * running: 
		 * queued: 
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 26);

		DMSG(" ---- Run #20");		
		m->runProcesses(100);  /* Process 27, 28, 29 30 destroyed */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8 , 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 
		 * running: 
		 * queued: 
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 30);
		
		delete m;

		ass_eq(destroyed_count, 30);

		FINISH_TEST;
	}

	void testProcessRunnerTerminateProcess() {
		BEGIN_TEST;

		destroyed_count = 0;		
		ass_eq(destroyed_count, 0);
		ProcessRunner* m = new ProcessRunner("PM", 4);

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		DMSG(" ---- Run #1");		
		m->runProcesses(100);	/* Process 1 should finish */
		/* finished: 1
		 * running: 2,3, 4
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateProcess("Process 1");
		m->terminateProcess("Process 2");

		DMSG(" ---- Run #2");		
		m->runProcesses(100);	/* Process 3 should finish, 2 terminate */
		/* finished: 1, 2, 3
		 * running: 4
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateProcess("Process 4");		

		ass_eq(destroyed_count, 0);		
		DMSG(" ---- Run #3");		
		m->runProcesses(100);	/* Process 2 terminate, 1 destroyed */
		/* finished: 2, 3, 4
		 * destroyed: 1
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 1);
		
		m->queueProcess(TestProcess::create("Process 5", 5, 100));
		m->terminateProcess("Process 5");
		ass_eq(destroyed_count, 1);
		DMSG(" ---- Run #4");		
		m->runProcesses(100);	/* Process 5 terminated, 2, 3 destroyed */
		/* finished: 4, 5
		 * destroyed: 1, 2, 3,
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 3);

		m->queueProcess(TestProcess::create("Process 6", 6, 100));
		m->queueProcess(TestProcess::create("Process 7", 7, 200));
		m->queueProcess(TestProcess::create("Process 8", 8, 300));
		m->terminateProcess("Process 6");
		m->terminateProcess("Process 7");
		m->terminateProcess("Process 8");
		ass_eq(destroyed_count, 3);
		DMSG(" ---- Run #5");		
		m->runProcesses(100);	/* Process 6, 7, 8 terminated, 4 destroyed */
		/* finished: 5, 6, 7, 8
		 * destroyed: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);

		DMSG(" ---- Run #6");		
		m->runProcesses(100);	/* Process 5 destroyed */
		/* finished: 6, 7, 8
		 * destroyed: 1, 2, 3, 4, 5
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 5);

		DMSG(" ---- Run #7");		
		m->runProcesses(100);	/* Process 6, 7, 8 destroyed */
		/* finished: 
		 * destroyed: 1, 2, 3, 4, 5, 6, 7, 8
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);
		
		delete m;

		ass_eq(destroyed_count, 8);
		
		FINISH_TEST;
	}

	void testProcessRunnerAddChildProcessOnSuccess() {
		BEGIN_TEST;

		destroyed_count = 0;		
		ass_eq(destroyed_count, 0);
		ProcessRunner* m = new ProcessRunner("PM", 4);
		ProcessPtr p = TestProcess::create("Process 1", 1, 100);
		p->attachChild(TestProcess::create("Process 1c", 11, 300));
		m->queueProcess(p);
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		
		p = TestProcess::create("Process 4", 4, 500);
		p->attachChild(TestProcess::create("Process 4c", 44, 100));						  
		m->queueProcess(p);
		p = TestProcess::create("Process 3", 3, 300);
		p->attachChild(TestProcess::create("Process 3c", 33, 100));	
		m->queueProcess(p);
		p.setNull();		
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		DMSG(" ---- Run #1");		
		m->runProcesses(100);	/* Process 1 should finish and add 1c */
		/* finished: 1
		 * running: 2, 3, 4, 1c
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateProcess("Process 1");
		m->terminateProcess("Process 2");

		DMSG(" ---- Run #2");		
		m->runProcesses(100);	/* Process 3, 1c, 3c should finish, add 3c, 2 terminate */
		/* finished: 1, 2, 3, 1c, 3c
		 * running: 4
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 6);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateProcess("Process 4");		

		ass_eq(destroyed_count, 0);		
		DMSG(" ---- Run #3");		
		m->runProcesses(100);	/* Process 4 terminate, 4c destroyed, 1 removed (not destroyed (child)) */
		/* finished: 2, 3, 4, 1c, 3c
		 * removed: 1
		 * destroyed: 4c
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 1);
		
		m->queueProcess(TestProcess::create("Process 5", 5, 100));
		m->terminateProcess("Process 5");
		ass_eq(destroyed_count, 1);
		DMSG(" ---- Run #4");		
		m->runProcesses(100);	/* Process 5 terminated, 1, 2, 1c, 3, 3c, destroyed */
		/* finished: 4, 5
		 * removed:
		 * destroyed: 4c, 1, 2, 1c, 3, 3c,
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		m->queueProcess(TestProcess::create("Process 6", 6, 100));
		m->queueProcess(TestProcess::create("Process 7", 7, 200));
		m->queueProcess(TestProcess::create("Process 8", 8, 300));
		m->terminateProcess("Process 6");
		m->terminateProcess("Process 7");
		m->terminateProcess("Process 8");
		ass_eq(destroyed_count, 6);
		DMSG(" ---- Run #5");		
		m->runProcesses(100);	/* Process 6, 7, 8 terminated, 4 destroyed */
		/* finished: 5, 6, 7, 8
		 * destroyed: 4c, 1, 2, 3, 4, 1c, 3c
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 7);

		DMSG(" ---- Run #6");		
		m->runProcesses(100);	/* Process 5 destroyed */
		/* finished: 6, 7, 8
		 * destroyed: 4c, 1, 2, 3, 4, 1c, 3c, 5
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);

		DMSG(" ---- Run #7");		
		m->runProcesses(100);	/* Process 6, 7, 8 destroyed */
		/* finished: 
		 * destroyed: 4c, 1, 2, 3, 4, 1c, 3c, 5, 6, 7, 8
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 11);
		
		delete m;

		ass_eq(destroyed_count, 11);
		
		FINISH_TEST;
	}

	void testProcessRunnerPauseAndResumeProcess() {
		BEGIN_TEST;

		destroyed_count = 0;		
		ass_eq(destroyed_count, 0);
		ProcessRunner* m = new ProcessRunner("PM", 4);

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		DMSG(" ---- Run #1");		
		m->runProcesses(100);	/* Process 1 should finish */
		/* finished: 1
		 * running: 2,3, 4
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_false(m->hasPaused());
		ass_true(m->hasRunning());
				
		m->pauseProcess("Process 1");
		m->pauseProcess("Process 2");

		DMSG(" ---- Run #2");		
		m->runProcesses(100);	/* Process 3 should finish, 2 pause */
		/* finished: 1, 3
		 * running: 4
		 * paused: 2
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_true(m->hasPaused());
		ass_true(m->hasRunning());		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateProcess("Process 4");		

		ass_eq(destroyed_count, 0);		
		DMSG(" ---- Run #3");		
		m->runProcesses(100);	/* Process 4 terminate, 1 destroyed */
		/* finished: 3, 4
		 * destroyed: 1
		 * paused: 2
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_false(m->hasRunning());
		ass_true(m->hasPaused());		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 1);
		
		m->queueProcess(TestProcess::create("Process 5", 5, 500));
		ass_eq(destroyed_count, 1);
		DMSG(" ---- Run #4");		
		m->runProcesses(100);	/* Process 5 started, 3 destroyed */
		/* finished: 4
		 * destroyed: 1, 3,
		 * paused: 2, 5
		 * running: 5
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_true(m->hasRunning());
		ass_true(m->hasPaused());		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 2);

		m->pauseProcess("Process 5");
		
		m->queueProcess(TestProcess::create("Process 6", 6, 100));
		m->queueProcess(TestProcess::create("Process 7", 7, 200));
		m->queueProcess(TestProcess::create("Process 8", 8, 300));
		
		ass_eq(destroyed_count, 2);
		DMSG(" ---- Run #5");		
		m->runProcesses(100);	/* Process 6, finished, 4 destroyed, 5 paused */
		/* finished: 6
		 * destroyed: 1, 3, 4
		 * running: 8, 7
		 * paused: 2, 5
		 * queued:
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_true(m->hasRunning());
		ass_true(m->hasPaused());		
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 3);

		m->pauseProcess("Process 7");

		DMSG(" ---- Run #6");		
		m->runProcesses(100);	/* Process 8 finished, 7 paused */
		/* finished: 6, 8
		 * destroyed: 1, 3, 4
		 * paused: 2, 5, 7
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_false(m->hasRunning());
		ass_true(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 3);

		DMSG(" ---- Run #7");		
		m->runProcesses(100);	/* Process 6, destroyed */
		/* finished: 8
		 * destroyed: 1, 3, 4, 6
		 * running:
		 * paused: 2, 5, 7
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRunning());
		ass_true(m->hasRemoved());	
		ass_true(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);

		DMSG(" ---- Run #8");		
		m->runProcesses(100);	/* Process 8, destroyed */
		/* finished: 
		 * destroyed: 1, 3, 4, 6, 8
		 * running:
		 * paused: 2, 5, 7
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRunning());
		ass_false(m->hasRemoved());	
		ass_true(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 5);
	
		m->resumeProcess("Process 2");
		m->resumeProcess("Process 5");

		DMSG(" ---- Run #9");		
		m->runProcesses(100);	/* Process 5 resumed, 2 finished*/
		/* finished: 2
		 * destroyed: 1, 3, 4, 6, 8
		 * running: 5
		 * paused: 7
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());		
		ass_true(m->hasRunning());
		ass_true(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 5);

		DMSG(" ---- Run #10");		
		m->runProcesses(100);	/* 5 finished */
		/* finished: 2, 5
		 * destroyed: 1, 3, 4, 6, 8
		 * running:
		 * paused: 7
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());		
		ass_false(m->hasRunning());
		ass_true(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 5);

		DMSG(" ---- Run #11");		
		m->runProcesses(100);	/* 2 terminated*/
		/* finished: 5
		 * destroyed: 1, 3, 4, 6, 8, 2
		 * running: 
		 * paused: 7
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());		
		ass_false(m->hasRunning());
		ass_true(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		m->resumeProcess("Process 7");		

		DMSG(" ---- Run #12");		
		m->runProcesses(100);	/* 5 terminated, Process 7 finished*/
		/* finished: 7
		 * destroyed: 1, 3, 4, 6, 8, 2, 5
		 * running: 
		 * paused:
		 * queued:
		 */
		ass_eq(m->processCount(), 1);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());		
		ass_false(m->hasRunning());
		ass_false(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 7);

		DMSG(" ---- Run #13");		
		m->runProcesses(100);	/* */
		/* finished: 7
		 * destroyed: 1, 3, 4, 6, 8, 2, 5
		 * running:
		 * paused:
		 * queued:
		 */
		ass_eq(m->processCount(), 1);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());		
		ass_false(m->hasRunning());
		ass_false(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 7);

		DMSG(" ---- Run #14");		
		m->runProcesses(100);	/* Process 7 destroyed */
		/* finished:
		 * destroyed: 1, 3, 4, 6, 8, 2, 5, 7
		 * running:
		 * paused:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());		
		ass_false(m->hasRunning());
		ass_false(m->hasPaused());	
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);

		
		delete m;

		ass_eq(destroyed_count, 8);
		
		FINISH_TEST;
	}

	
	void testProcessRunnerAddFinishedProcess() {
		BEGIN_TEST;

		destroyed_count = 0;		
		ass_eq(destroyed_count, 0);
		ProcessRunner* m = new ProcessRunner("PM", 4);

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		DMSG(" ---- Run #1");		
		m->runProcesses(100);	/* Process 1 should finish */
		/* finished: 1
		 * running: 2,3, 4
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		
		m->terminateProcess("Process 2");

		DMSG(" ---- Run #2");		
		m->runProcesses(100);	/* Process 3 should finish, 2 terminate */
		/* finished: 1, 2, 3
		 * running: 4
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateProcess("Process 4");
		m->queueProcess(TestProcess::create("Process 1", 1, 200));		

		ass_eq(destroyed_count, 0);		
		DMSG(" ---- Run #3");		
		m->runProcesses(100);	/* Process 4 terminate, 1 destroyed */
		/* finished: 2, 3, 4
		 * destroyed: 1
		 * running: 1
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 1);
		
		m->queueProcess(TestProcess::create("Process 5", 5, 100));
		m->terminateProcess("Process 5");
		ass_eq(destroyed_count, 1);
		DMSG(" ---- Run #4");		
		m->runProcesses(100);	/* Process 1 finished, 5 terminated, 2, 3 destroyed */
		/* finished: 4, 5, 1
		 * destroyed: 1, 2, 3,
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 3);

		m->queueProcess(TestProcess::create("Process 5", 5, 100));
		m->queueProcess(TestProcess::create("Process 6", 6, 200));
		m->terminateProcess("Process 6");
		ass_eq(destroyed_count, 3);
		DMSG(" ---- Run #5");		
		m->runProcesses(100);	/* Process 5, finished 6 terminated, 4 destroyed */
		/* finished: 1, 5, 6, 5
		 * destroyed: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);

		m->queueProcess(TestProcess::create("Process 6", 6, 100));
		m->terminateProcess("Process 6");		

		DMSG(" ---- Run #6");		
		m->runProcesses(100);	/* Process 1, 5 destroyed, 6 terminated */
		/* finished: 6, 5, 6
		 * destroyed: 1, 2, 3, 4, 5, 1
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		m->queueProcess(TestProcess::create("Process 6", 61, 200));

		DMSG(" ---- Run #7");		
		m->runProcesses(100);	/* Process 6, 5 destroyed */
		/* finished: 6
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5
		 * running: 6
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);

		TestProcess* tp1 = (TestProcess*)(m->getProcess("Process 6").ptr());
		ass_eq(tp1->val(), 61);		

		DMSG(" ---- Run #8");		
		m->runProcesses(100);	/* Process 6, finished, 6 destroyed */
		/* finished: 6
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 1);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 9);

		DMSG(" ---- Run #9");		
		m->runProcesses(100);
		/* finished: 6
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 1);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 9);

		DMSG(" ---- Run #10");		
		m->runProcesses(100);	/* Process 6 destroyed */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 10);

		m->queueProcess(TestProcess::create("Process 7", 7, 100));
		DMSG(" ---- Run #11");		
		m->runProcesses(100);	/* Process 7 finished */
		/* finished: 7
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 1);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 10);

		m->queueProcess(TestProcess::create("Process 7", 7, 100));
		DMSG(" ---- Run #12");		
		m->runProcesses(100);	/* Process 7 finished */
		/* finished: 7, 7
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 10);

		m->queueProcess(TestProcess::create("Process 7", 7, 100));
		m->terminateProcess("Process 7");		
		DMSG(" ---- Run #13");		
		m->runProcesses(100);	/* Process 7 terminated, 7 destroyed */
		/* finished: 7, 7
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 11);

		DMSG(" ---- Run #14");		
		m->runProcesses(100);	/* Process 7 destroyed */
		/* finished: 7
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 1);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 12);

		DMSG(" ---- Run #15");		
		m->runProcesses(100);	/* Process 7 destroyed */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		
		ass_eq(destroyed_count, 13);

		m->queueProcess(TestProcess::create("Process 8", 8, 1500));
		m->queueProcess(TestProcess::create("Process 9", 9, 1500));
		m->queueProcess(TestProcess::create("Process 10", 10, 1500));
		m->queueProcess(TestProcess::create("Process 11", 11, 100));

		DMSG(" ---- Run #16");		
		m->runProcesses(100);	/* Process 11 finished 100*/
		/* finished: 11
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7
		 * running: 8, 9, 10
		 * queued: 
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 13);

		DMSG(" ---- Run #17");		
		m->runProcesses(100);	/* Process 11 finished 300*/
		/* finished: 11
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7
		 * running: 8, 9, 10
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 13);

		m->queueProcess(TestProcess::create("Process 11", 11, 100));
		m->queueProcess(TestProcess::create("Process 12", 81, 100));

		DMSG(" ---- Run #18");		
		m->runProcesses(100);	/* Process 11 destroyed, 11, 12 finished 600*/
		/* finished: 11, 12
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7, 11
		 * running: 8, 9, 10
		 * queued: 
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);

		DMSG(" ---- Run #19");		
		m->runProcesses(100); /* 1200 */
		/* finished: 11, 12
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7, 11
		 * running: 8, 9, 10
		 * queued:
		 */
		ass_eq(m->processCount(), 5);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 14);

		TestProcess* tp2 = (TestProcess*)(m->getProcess("Process 8").ptr());
		ass_eq(tp2->val(), 8);		

		DMSG(" ---- Run #20");		
		m->runProcesses(100);	/* Process 8, 9, 10, 8 finished, 11, 12 destroyed 2400*/
		/* finished: 8, 9, 10
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7, 11, 11, 12
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 16);

		DMSG(" ---- Run #21");		
		m->runProcesses(100);
		/* finished: 8, 9, 10,
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7, 11, 11
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 16);

		DMSG(" ---- Run #22");		
		m->runProcesses(100); /* process 8, 9, 10 destroyed */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 5, 1, 6, 5, 6, 6, 7, 7, 7, 11, 11, 8, 9, 10, 8
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 19);
		
		delete m;

		ass_eq(destroyed_count, 19);
		
		FINISH_TEST;
	}

	void testProcessRunnerTerminateAllProcesses() {
		BEGIN_TEST;

		
		destroyed_count = 0;		
		ass_eq(destroyed_count, 0);
		ProcessRunner* m = new ProcessRunner("PM", 4);

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateAllProcesses();		

		DMSG(" ---- Run #1");		
		m->runProcesses(100);	/* Process 1, 2, 3, 4 should terminate */
		/* finished: 1, 2, 3, 4
		 * running:
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));

		DMSG(" ---- Run #2");		
		m->runProcesses(100);	/* Process 1 finished, 2, 3, 4 started */
		/* finished: 1, 2, 3, 4, 1
		 * running: 2, 3, 4
		 * queued:
		 */
		ass_eq(m->processCount(), 8);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());		
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		ass_eq(destroyed_count, 0);		
		DMSG(" ---- Run #3");		
		m->runProcesses(100);	/* Process 1, 2, 3, 4 destroyed, 2, 3 finished */
		/* finished: 1, 2, 3
		 * destroyed: 1, 2, 3, 4
		 * running: 4
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);

		m->terminateAllProcesses();
		
		ass_eq(destroyed_count, 4);
		DMSG(" ---- Run #4");		
		m->runProcesses(100);	/* Process 4 terminated, 1 destroyed */
		/* finished: 2, 3, 4 
		 * destroyed: 1, 2, 3, 4, 1
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 5);


		m->queueProcess(TestProcess::create("Process 5", 6, 100));
		m->queueProcess(TestProcess::create("Process 6", 7, 200));
		m->queueProcess(TestProcess::create("Process 7", 8, 300));
		ass_eq(destroyed_count, 5);
		DMSG(" ---- Run #5");		
		m->runProcesses(100);	/* Process 5 finished, 2, 3 destroyed */
		/* finished:  4, 5
		 * destroyed: 1, 2, 3, 4, 1, 2, 3,
		 * running: 6, 7
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 7);

		DMSG(" ---- Run #6");		
		m->runProcesses(100);	/* Process 4 destroyed, 6, 7 finished */
		/* finished: 5, 6, 7
		 * destroyed: 1, 2, 3, 4, 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 3);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);

		DMSG(" ---- Run #7");		
		m->runProcesses(100);	/* Process 5 destroyed  */
		/* finished: 6, 7
		 * destroyed: 1, 2, 3, 4, 1, 2, 3, 4, 5
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 2);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 9);

		DMSG(" ---- Run #8");		
		m->runProcesses(100);	/* 6, 7 destroyed  */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 1, 2, 3, 4, 5, 6, 7
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_false(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 11);

		DMSG(" ---- Run #9");		
		m->runProcesses(100);	/* */
		/* finished:
		 * destroyed: 1, 2, 3, 4, 1, 2, 3, 4, 5, 6, 7
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 11);
		
		delete m;

		ass_eq(destroyed_count, 11);

		FINISH_TEST;
	}

		void testProcessRunnerTerminateProcessRunner() {
		BEGIN_TEST;

		destroyed_count = 0;		
		ass_eq(destroyed_count, 0);
		ProcessRunner* m = new ProcessRunner("PM", 4);

		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_false(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());

		m->terminateProcessRunner();
		ass_eq(m->state(), ProcessRunner::kPMSNotStarted);	

		DMSG(" ---- Run #1");		
		m->runProcesses(100);	/* Process 1, 2, 3, 4 should terminate, status = will terminate */
		/* finished: 1, 2, 3, 4
		 * running:
		 * queued: NONE
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 0);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);		

		/** No processes should be added */
		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));		

		DMSG(" ---- Run #2");		
		m->runProcesses(100);	/*  */
		/* finished: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 4);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_true(m->hasRemoved());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);	

		DMSG(" ---- Run #3");		
		m->runProcesses(100);	/* Process 1, 2, 3, 4 destroyed, status = Will terminate */
		/* finished:
		 * destroyed: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);	

		/** No processes should be added */
		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));

		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 12);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);	
		
		DMSG(" ---- Run #4");		
		m->runProcesses(100);	/*  */
		/* finished:
		 * destroyed: 1, 2, 3, 4
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 12);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);	
		
		delete m;

		ass_eq(destroyed_count, 12);

		m = new ProcessRunner("PM2", 4);
		m->terminateProcessRunner();
		ass_eq(m->state(), ProcessRunner::kPMSNotStarted);	

		DMSG(" ---- Run #5");		
		m->runProcesses(100);	/*  */
		/* finished:
		 * destroyed:
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 12);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);	

		/** No processes should be added */
		m->queueProcess(TestProcess::create("Process 1", 1, 100));
		m->queueProcess(TestProcess::create("Process 2", 2, 200));
		m->queueProcess(TestProcess::create("Process 4", 4, 500));
		m->queueProcess(TestProcess::create("Process 3", 3, 300));
		ass_eq(destroyed_count, 16);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);	

		DMSG(" ---- Run #6");		
		m->runProcesses(100);	/*  */
		/* finished:
		 * destroyed:
		 * running:
		 * queued:
		 */
		ass_eq(m->processCount(), 0);
		ass_eq(m->inputQueue()->capacity(), 4);
		ass_true(m->inputQueue()->isEmpty());
		ass_eq(m->messageQueue()->capacity(), 6);
		ass_true(m->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 16);
		ass_eq(m->state(), ProcessRunner::kPMSTerminated);	

		delete m;

		ass_eq(destroyed_count, 16);
		
		FINISH_TEST;
	}
	
} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroyProcessRunner();
	cc::testProcessRunnerQueueProcess();
	cc::testProcessRunnerPostMessages();
	cc::testProcessRunnerRunProcessesSimple();
	cc::testProcessRunnerRunProcessesComplex();
	cc::testProcessRunnerTerminateProcess();
	cc::testProcessRunnerAddChildProcessOnSuccess();	
	cc::testProcessRunnerPauseAndResumeProcess();	
	cc::testProcessRunnerAddFinishedProcess();
	cc::testProcessRunnerTerminateAllProcesses();
	cc::testProcessRunnerTerminateProcessRunner();
	
	return 0;
}
