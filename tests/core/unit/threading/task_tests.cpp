#include "core/testcore.h"
#include "core/threading/task.h"

namespace cc {

	I32 destroyed_count = 0;

	class TestTask : public Task {		
	  public:
		TestTask()
			: Task(), m_val(0) {}

		TestTask(OID pid, I32 val)
			: Task(pid), m_val(val) {}

		TestTask(const Char* name, I32 val)
			: Task(name), m_val(val) {}

		~TestTask() {
			DMSG("Test Task " << m_val << " destroyed.");
			destroyed_count++;			
		}		

		void onFailure() {
			DMSG("Test task (" << m_val << ") failed!");
		}
		void onInitialize() {
			DMSG("Test task (" << m_val << ") initialized!");
		}
		void onSuccess() {
			DMSG("Test task (" << m_val << ") succeeded!");
		}
		void onTermination() {
			DMSG("Test task (" << m_val << ") terminated!");
		}

		void run() {
			DMSG("Task " << m_val << " ran.");
			succeeded();			
		}

		inline I32 val() const { return m_val; }

		inline static TaskPtr create(const Char* name, I32 val) {
			return TaskPtr(new TestTask(name, val));
		}		

	  private:
		I32 m_val;
	};
	
		

	void testCreateAndDestroyTask() {
		BEGIN_TEST;

		TestTask* tp1 = new TestTask();
		ass_eq(tp1->val(), 0);
		ass_true(tp1->child().isNull());
		ass_false(tp1->hasChild());
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_false(tp1->isInitialized());
		ass_eq(tp1->name(), NIL);
		ass_true(tp1->parent().isNull());
		ass_eq(tp1->oID(), 0);
		ass_eq(tp1->priority(), 1);
		ass_eq(tp1->retainCount(), 0);
		ass_eq(tp1->state(), Task::kTSNotStarted);
		ass_true(tp1->takeChild().isNull());
		ass_false(tp1->wasRemoved());

		TestTask* tp2 = new TestTask(1234, 1);
		ass_eq(tp2->val(), 1);
		ass_true(tp2->child().isNull());
		ass_false(tp2->hasChild());
		ass_false(tp2->isAlive());
		ass_false(tp2->isDead());
		ass_false(tp2->isInitialized());
		ass_eq(tp2->name(), NIL);
		ass_true(tp2->parent().isNull());
		ass_eq(tp2->oID(), 1234);
		ass_eq(tp2->priority(), 1);
		ass_eq(tp2->retainCount(), 0);
		ass_eq(tp2->state(), Task::kTSNotStarted);
		ass_true(tp2->takeChild().isNull());
		ass_false(tp2->wasRemoved());

		TestTask* tp3 = new TestTask("A task", 2);
		ass_eq(tp3->val(), 2);
		ass_true(tp3->child().isNull());
		ass_false(tp3->hasChild());
		ass_false(tp3->isAlive());
		ass_false(tp3->isDead());
		ass_false(tp3->isInitialized());
		ass_true(strcmp(tp3->name(), "A task") == 0);
		ass_true(tp3->parent().isNull());
		ass_eq(tp3->oID(), crc32("A task"));
		ass_eq(tp3->priority(), 1);
		ass_eq(tp3->retainCount(), 0);
		ass_eq(tp3->state(), Task::kTSNotStarted);
		ass_true(tp3->takeChild().isNull());
		ass_false(tp3->wasRemoved());
		
		delete tp1;
		delete tp2;
		delete tp3;				

		FINISH_TEST;
	}

	void testTaskState() {
		BEGIN_TEST;

		TaskPtr tp1 = TestTask::create("A task", 2);	
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_false(tp1->isInitialized());
		ass_eq(tp1->state(), Task::kTSNotStarted);
		ass_false(tp1->wasRemoved());

		// Initialize the task.
		tp1->initialize();
		tp1->onInitialize();
		ass_true(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_eq(tp1->state(), Task::kTSRunning);
		ass_false(tp1->wasRemoved());

		// succeed
		tp1->succeeded();
		tp1->onSuccess();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_eq(tp1->state(), Task::kTSSucceeded);
		ass_false(tp1->wasRemoved());

		// fail
		tp1->failed();
		tp1->onFailure();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_eq(tp1->state(), Task::kTSFailed);
		ass_false(tp1->wasRemoved());

		// should faile
		tp1->terminate();
		ass_eq(tp1->state(), Task::kTSFailed);
		tp1->succeeded();
		tp1->terminate();
		ass_eq(tp1->state(), Task::kTSSucceeded);
		tp1->remove();
		tp1->terminate();
		ass_eq(tp1->state(), Task::kTSRemoved);

		// Following terminations should succeed
		
		// terminate from running
		tp1->initialize();
		ass_eq(tp1->state(), Task::kTSRunning);
		tp1->terminate();
		tp1->onTermination();		
		ass_false(tp1->isAlive());
		ass_true(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_eq(tp1->state(), Task::kTSTerminated);
		ass_false(tp1->wasRemoved());

		// terminate from not Started
		TaskPtr tp2 = TestTask::create("A task 2", 3);	
		ass_false(tp2->isAlive());
		ass_false(tp2->isDead());
		ass_false(tp2->isInitialized());
		ass_eq(tp2->state(), Task::kTSNotStarted);
		ass_false(tp2->wasRemoved());
		tp2->terminate();
		tp2->onTermination();		
		ass_false(tp2->isAlive());
		ass_true(tp2->isDead());
		ass_true(tp2->isInitialized());
		ass_eq(tp2->state(), Task::kTSTerminated);
		ass_false(tp2->wasRemoved());

		// removed
		tp1->remove();		
		ass_false(tp1->isAlive());
		ass_false(tp1->isDead());
		ass_true(tp1->isInitialized());
		ass_eq(tp1->state(), Task::kTSRemoved);
		ass_true(tp1->wasRemoved());
		
		FINISH_TEST;
	}

	void testAttachAndRemoveChildTask() {
		BEGIN_TEST;

		destroyed_count = 0;
		
		TaskPtr tp = TestTask::create("A task", 4);
		ass_true(tp->child().isNull());		
		ass_false(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 1);
		ass_true(tp->takeChild().isNull());
		
		TaskPtr cp = TestTask::create("A child task", 5);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().isNull());	
		ass_eq(cp->retainCount(), 1);
		ass_true(cp->takeChild().isNull());

		// Attach the child task.
		tp->attachChild(cp);
		ass_true(tp->child().notNull());
		ass_eq(tp->child()->oID(), cp->oID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->oID(), tp->oID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());

		// Take the child task
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
		ass_eq(tp->child()->oID(), cp->oID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->oID(), tp->oID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());

		cp = TaskPtr::nullPtr();

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
		ass_eq(tp->child()->oID(), cp->oID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->oID(), tp->oID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());
		
		cp = TaskPtr::nullPtr();
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
		cp = TestTask::create("Another child task", 6);
		tp->attachChild(cp);
		ass_true(tp->child().notNull());
		ass_eq(tp->child()->oID(), cp->oID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->oID(), tp->oID());
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
		ass_eq(tp->child()->oID(), cp->oID());		
		ass_true(tp->hasChild());	
		ass_true(tp->parent().isNull());	
		ass_eq(tp->retainCount(), 2);
		ass_true(cp->child().isNull());		
		ass_false(cp->hasChild());	
		ass_true(cp->parent().notNull());
		ass_eq(cp->parent()->oID(), tp->oID());
		ass_eq(cp->retainCount(), 2);
		ass_true(cp->takeChild().isNull());
		
		cp = TaskPtr::nullPtr();
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

	void testRunTask() {
		BEGIN_TEST;

		TestTask* tp = new TestTask("Testy", 34);
		TaskPtr ptr = TaskPtr(tp);
		
		ptr->run();
		ass_eq(ptr->state(), Task::kTSSucceeded);		

		FINISH_TEST;
	}	

} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroyTask();
	cc::testTaskState();
	cc::testAttachAndRemoveChildTask();
	cc::testRunTask();	
	return 0;
}
