#include "core/testcore.h"
#include "core/threading/process.h"

namespace cc {

	I32 destroyed_count = 0;	

	class TestProcess : public Process {		
	  public:
		TestProcess()
			: Process(), m_ran(0), m_val(0) {}

		TestProcess(OID pid, I32 val)
			: Process(pid), m_ran(0), m_val(val) {}

		TestProcess(const Char* name, I32 val)
			: Process(name), m_ran(0), m_val(val) {}

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
		void onSuccess() {
			DMSG("Test process (" << m_val << ") succeeded!");
		}
		void onTermination() {
			DMSG("Test process (" << m_val << ") terminated!");
		}

		void run(U32 time) {
			DMSG("Process " << m_val << " recieved " << time << " time.");
			m_ran += time;			
		}

		inline U32 ran() const { return m_ran; }
		inline I32 val() const { return m_val; }

		inline static ProcessPtr create(const Char* name, I32 val) {
			return ProcessPtr(new TestProcess(name, val));
		}		

	  private:
		U32 m_ran;		
		I32 m_val;
	};
	
		

	void testCreateAndDestroyProcess() {
		BEGIN_TEST;

		TestProcess* tp1 = new TestProcess();
		ass_eq(tp1->ran(), 0);
		ass_eq(tp1->val(), 0);
		ass_true(tp1->child().isNull());
		ass_eq(tp1->getRequestedRunTime(100), 100);
		ass_false(tp1->hasChild());
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_false(tp1->isInitialized());
		ass_false(tp1->isPaused());
		ass_eq(tp1->name(), NIL);
		ass_eq(tp1->priorityModifier(), 1);
		ass_true(tp1->parent().isNull());
		ass_eq(tp1->pID(), 0);
		ass_eq(tp1->priority(), 1);
		ass_eq(tp1->retainCount(), 0);
		ass_eq(tp1->state(), Process::kPSNotStarted);
		ass_true(tp1->takeChild().isNull());
		ass_false(tp1->wasRemoved());

		TestProcess* tp2 = new TestProcess(1234, 1);
		ass_eq(tp2->ran(), 0);
		ass_eq(tp2->val(), 1);
		ass_true(tp2->child().isNull());
		ass_eq(tp2->getRequestedRunTime(100), 100);
		ass_false(tp2->hasChild());
		ass_false(tp2->isAlive());
		ass_false(tp2->isDead());
		ass_false(tp2->isInitialized());
		ass_false(tp2->isPaused());
		ass_eq(tp2->name(), NIL);
		ass_eq(tp2->priorityModifier(), 1);
		ass_true(tp2->parent().isNull());
		ass_eq(tp2->pID(), 1234);
		ass_eq(tp2->priority(), 1);
		ass_eq(tp2->retainCount(), 0);
		ass_eq(tp2->state(), Process::kPSNotStarted);
		ass_true(tp2->takeChild().isNull());
		ass_false(tp2->wasRemoved());

		TestProcess* tp3 = new TestProcess("A process", 2);
		ass_eq(tp3->ran(), 0);
		ass_eq(tp3->val(), 2);
		ass_true(tp3->child().isNull());
		ass_eq(tp3->getRequestedRunTime(100), 100);
		ass_false(tp3->hasChild());
		ass_false(tp3->isAlive());
		ass_false(tp3->isDead());
		ass_false(tp3->isInitialized());
		ass_false(tp3->isPaused());
		ass_true(strcmp(tp3->name(), "A process") == 0);
		ass_eq(tp3->priorityModifier(), 1);
		ass_true(tp3->parent().isNull());
		ass_eq(tp3->pID(), crc32("A process"));
		ass_eq(tp3->priority(), 1);
		ass_eq(tp3->retainCount(), 0);
		ass_eq(tp3->state(), Process::kPSNotStarted);
		ass_true(tp3->takeChild().isNull());
		ass_false(tp3->wasRemoved());
		
		delete tp1;
		delete tp2;
		delete tp3;				

		FINISH_TEST;
	}

	void testGetRequestedRunTime() {
		BEGIN_TEST;

		TestProcess* tp1 = new TestProcess();
		ass_eq(tp1->priority(), 1);
		ass_eq(tp1->priorityModifier(), 1);
		ass_eq(tp1->getRequestedRunTime(100), 100);

		tp1->setPriority(4);
		ass_eq(tp1->priority(), 4);
		ass_eq(tp1->priorityModifier(), 1);
		ass_eq(tp1->getRequestedRunTime(100), 400);

		tp1->setPriority(1);
		tp1->setPriorityModifier(2);
		ass_eq(tp1->priority(), 1);
		ass_eq(tp1->priorityModifier(), 2);
		ass_eq(tp1->getRequestedRunTime(100), 200);

		tp1->setPriority(4);
		ass_eq(tp1->priority(), 4);
		ass_eq(tp1->priorityModifier(), 2);
		ass_eq(tp1->getRequestedRunTime(100), 800);

		delete tp1;		

		TestProcess* tp2 = new TestProcess(1234, 1);
		ass_eq(tp2->priority(), 1);
		ass_eq(tp2->priorityModifier(), 1);
		ass_eq(tp2->getRequestedRunTime(100), 100);

		tp2->setPriority(4);
		ass_eq(tp2->priority(), 4);
		ass_eq(tp2->priorityModifier(), 1);
		ass_eq(tp2->getRequestedRunTime(100), 400);

		tp2->setPriority(1);
		tp2->setPriorityModifier(2);
		ass_eq(tp2->priority(), 1);
		ass_eq(tp2->priorityModifier(), 2);
		ass_eq(tp2->getRequestedRunTime(100), 200);

		tp2->setPriority(4);
		ass_eq(tp2->priority(), 4);
		ass_eq(tp2->priorityModifier(), 2);
		ass_eq(tp2->getRequestedRunTime(100), 800);

		delete tp2;		
		
		ProcessPtr tp3 = TestProcess::create("A process", 2);
		ass_eq(tp3->priority(), 1);
		ass_eq(tp3->priorityModifier(), 1);
		ass_eq(tp3->getRequestedRunTime(100), 100);

		tp3->setPriority(4);
		ass_eq(tp3->priority(), 4);
		ass_eq(tp3->priorityModifier(), 1);
		ass_eq(tp3->getRequestedRunTime(100), 400);

		tp3->setPriority(1);
		tp3->setPriorityModifier(2);
		ass_eq(tp3->priority(), 1);
		ass_eq(tp3->priorityModifier(), 2);
		ass_eq(tp3->getRequestedRunTime(100), 200);

		tp3->setPriority(4);
		ass_eq(tp3->priority(), 4);
		ass_eq(tp3->priorityModifier(), 2);
		ass_eq(tp3->getRequestedRunTime(100), 800);
		
		FINISH_TEST;
	}

	void testProcessState() {
		BEGIN_TEST;

		ProcessPtr tp1 = TestProcess::create("A process", 2);	
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_false(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSNotStarted);
		ass_false(tp1->wasRemoved());

		// Try and pause process (shouldn't work)
		tp1->pause();
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_false(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSNotStarted);
		ass_false(tp1->wasRemoved());

		// Try and resume process (shouldn't work)
		tp1->resume();
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_false(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSNotStarted);
		ass_false(tp1->wasRemoved());


		// Initialize the process.
		tp1->initialize();
		tp1->onInitialize();
		ass_true(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSRunning);
		ass_false(tp1->wasRemoved());

		// pause the process (should work now).
		tp1->pause();
		ass_true(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_true(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSPaused);
		ass_false(tp1->wasRemoved());
		
		// resume the process (should work now).
		tp1->resume();
		ass_true(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSRunning);
		ass_false(tp1->wasRemoved());

		// succeed
		tp1->succeeded();
		tp1->onSuccess();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSSucceeded);
		ass_false(tp1->wasRemoved());

		// fail
		tp1->failed();
		tp1->onFailure();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSFailed);
		ass_false(tp1->wasRemoved());

		// should faile
		tp1->terminate();
		ass_eq(tp1->state(), Process::kPSFailed);

		tp1->succeeded();
		tp1->terminate();
		ass_eq(tp1->state(), Process::kPSSucceeded);

		tp1->remove();
		tp1->terminate();
		ass_eq(tp1->state(), Process::kPSRemoved);

		tp1->markForRemoval();
		tp1->terminate();
		ass_eq(tp1->state(), Process::kPSWillBeRemoved);

		// Following terminations should succeed
		
		// terminate from paused
		tp1->initialize();		
		tp1->pause();
		ass_eq(tp1->state(), Process::kPSPaused);
		tp1->terminate();
		tp1->onTermination();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSTerminated);
		ass_false(tp1->wasRemoved());

		// terminate from running
		tp1->initialize();
		ass_eq(tp1->state(), Process::kPSRunning);
		tp1->terminate();
		tp1->onTermination();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSTerminated);
		ass_false(tp1->wasRemoved());

		// terminate from not Started
		ProcessPtr tp2 = TestProcess::create("A process 2", 3);	
		ass_false(tp2->isAlive());
		ass_false(tp2->isDead());
		ass_false(tp2->isInitialized());
		ass_false(tp2->isPaused());	
		ass_eq(tp2->state(), Process::kPSNotStarted);
		ass_false(tp2->wasRemoved());
		tp2->terminate();
		tp2->onTermination();		
		ass_false(tp2->isAlive());
		ass_true(tp2->isDead());
		ass_true(tp2->isInitialized());
		ass_false(tp2->isPaused());	
		ass_eq(tp2->state(), Process::kPSTerminated);
		ass_false(tp2->wasRemoved());

		// will be removed
		tp1->markForRemoval();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSWillBeRemoved);
		ass_true(tp1->wasRemoved());

		// removed
		tp1->remove();		
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_false(tp1->isPaused());	
		ass_eq(tp1->state(), Process::kPSRemoved);
		ass_true(tp1->wasRemoved());
		
		
		FINISH_TEST;
	}

	void testAttachAndRemoveChildProcess() {
		BEGIN_TEST;

		destroyed_count = 0;
		
		ProcessPtr tp = TestProcess::create("A process", 4);
		ass_true(tp->child().isNull());		
		ass_false(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 1);
		ass_true(tp->takeChild().isNull());
		
		ProcessPtr cp = TestProcess::create("A child process", 5);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().isNull());	
		ass_eq(cp->retainCount(), 1);
		ass_true(cp->takeChild().isNull());

		// Attach the child process.
		tp->attachChild(cp);
		ass_true(tp->child().notNull());
		ass_eq(tp->child()->pID(), cp->pID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->pID(), tp->pID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());

		// Take the child process
		tp->takeChild();
		ass_true(tp->child().isNull());		
		ass_false(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 1);
		ass_true(tp->takeChild().isNull());
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().isNull());	
		ass_eq(cp->retainCount(), 1);
		ass_true(cp->takeChild().isNull());

		ass_eq(destroyed_count, 0);
		
		// Add the child again.
		tp->attachChild(cp);
		ass_true(tp->child().notNull());
		ass_eq(tp->child()->pID(), cp->pID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->pID(), tp->pID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());

		cp = ProcessPtr::nullPtr();

		ass_eq(tp->child()->retainCount(), 1);

		// take child again
		cp = tp->takeChild();
		ass_eq(destroyed_count, 0);
		ass_true(tp->child().isNull());		
		ass_false(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 1);
		ass_true(tp->takeChild().isNull());
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().isNull());	
		ass_eq(cp->retainCount(), 1);
		ass_true(cp->takeChild().isNull());

		// Add and take the child, but this time should be destroyed.
		tp->attachChild(cp);
		ass_true(tp->child().notNull());
		ass_eq(tp->child()->pID(), cp->pID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->pID(), tp->pID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());
		
		cp = ProcessPtr::nullPtr();
		ass_eq(tp->child()->retainCount(), 1);
		ass_eq(destroyed_count, 0);

		tp->takeChild();
		ass_true(tp->child().isNull());		
		ass_false(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 1);
		ass_true(tp->takeChild().isNull());
		ass_eq(destroyed_count, 1);

		// Test the same, but with detach child.
		cp = TestProcess::create("Another child process", 6);
		tp->attachChild(cp);
		ass_true(tp->child().notNull());
		ass_eq(tp->child()->pID(), cp->pID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->pID(), tp->pID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());

		tp->detachChild();		
		ass_eq(destroyed_count, 1);
		ass_true(tp->child().isNull());		
		ass_false(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 1);
		ass_true(tp->takeChild().isNull());
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().isNull());	
		ass_eq(cp->retainCount(), 1);
		ass_true(cp->takeChild().isNull());
		
		tp->attachChild(cp);
		ass_true(tp->child().notNull());
		ass_eq(tp->child()->pID(), cp->pID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->pID(), tp->pID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());
		
		cp = ProcessPtr::nullPtr();
		ass_eq(tp->child()->retainCount(), 1);
		ass_eq(destroyed_count, 1);

		tp->detachChild();
		ass_true(tp->child().isNull());		
		ass_false(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 1);
		ass_true(tp->takeChild().isNull());
		ass_eq(destroyed_count, 2);
		
		FINISH_TEST;
	}

	void testRunProcess() {
		BEGIN_TEST;

		TestProcess* tp = new TestProcess("Testy", 34);
		ProcessPtr ptr = ProcessPtr(tp);
		
		ass_eq(tp->ran(), 0);

		ptr->run(100);
		ptr->setPriorityModifier(2);		
		ass_eq(tp->ran(), 100);	  

		ptr->run(ptr->getRequestedRunTime(100));
		ptr->setPriorityModifier(3);	
		ass_eq(tp->ran(), 300);

		ptr->run(ptr->getRequestedRunTime(100));
		ptr->setPriorityModifier(4);
		ass_eq(tp->ran(), 600);
		

		FINISH_TEST;
	}	

} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroyProcess();
	cc::testGetRequestedRunTime();
	cc::testProcessState();
	cc::testAttachAndRemoveChildProcess();
	cc::testRunProcess();	
	return 0;
}
