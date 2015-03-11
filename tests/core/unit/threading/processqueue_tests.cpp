#include "core/testcore.h"
#include "core/threading/processqueue.h"

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
	
		

	void testCreateAndDestroyProcessQueue() {
		BEGIN_TEST;

		ProcessQueue queue1;

		ass_false(queue1.hasProcesses());
		ass_true(queue1.isFull());
		ass_eq(queue1.available(), 0);
		ass_eq(queue1.processCount(), 0);

		queue1.initWithCapacity(10);
		ass_false(queue1.hasProcesses());
		ass_false(queue1.isFull());
		ass_eq(queue1.available(), 10);
		ass_eq(queue1.processCount(), 0);
		ass_eq(queue1.first(), queue1.root());

		ProcessQueue queue2(5);
		ass_false(queue2.hasProcesses());
		ass_false(queue2.isFull());
		ass_eq(queue2.available(), 5);
		ass_eq(queue2.processCount(), 0);
		ass_eq(queue2.first(), queue2.root());

		ProcessQueue queue3(queue2);
		ass_false(queue3.hasProcesses());
		ass_false(queue3.isFull());
		ass_eq(queue3.available(), 5);
		ass_eq(queue3.processCount(), 0);
		ass_eq(queue3.first(), queue3.root());

		queue3 = queue1;
		ass_false(queue3.hasProcesses());
		ass_false(queue3.isFull());
		ass_eq(queue3.available(), 10);
		ass_eq(queue3.processCount(), 0);
		ass_eq(queue3.first(), queue3.root());

		destroyed_count = 0;		
		ProcessQueue* queue4 = new ProcessQueue(11);
		ass_false(queue4->hasProcesses());
		ass_false(queue4->isFull());
		ass_eq(queue4->available(), 11);
		ass_eq(queue4->processCount(), 0);
		ass_eq(queue4->first(), queue4->root());
		queue4->push(TestProcess::create("Process 1", 1));
		queue4->push(TestProcess::create("Process 2", 2));
		queue4->push(TestProcess::create("Process 3", 3));

		ass_eq(destroyed_count, 0);		
		delete queue4;		
		ass_eq(destroyed_count, 3);
		
		FINISH_TEST;
	}

	void testCopyProcessQueue() {
		BEGIN_TEST;

		destroyed_count = 0;		
		ProcessQueue queue1(5);
	   queue1.push(TestProcess::create("Process 1", 1));
		queue1.push(TestProcess::create("Process 2", 2));
		queue1.push(TestProcess::create("Process 3", 3));
		ass_true(queue1.get("Process 1").notNull());
		ass_true(queue1.get("Process 2").notNull());
		ass_true(queue1.get("Process 3").notNull());
		ass_eq(queue1.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue1.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue1.get("Process 1")->pID(), crc32("Process 1"));

		ass_eq(destroyed_count, 0);		
		ProcessQueue queue2(queue1);
		ass_eq(destroyed_count, 0);
		ass_true(queue2.get("Process 1").notNull());
		ass_true(queue2.get("Process 2").notNull());
		ass_true(queue2.get("Process 3").notNull());
		ass_eq(queue2.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue2.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue2.get("Process 1")->pID(), crc32("Process 1"));

		queue2.clear();		
		ass_true(queue2.get("Process 1").isNull());
		ass_true(queue2.get("Process 2").isNull());
		ass_true(queue2.get("Process 3").isNull());
		ass_eq(destroyed_count, 0);
		
		ass_true(queue1.get("Process 1").notNull());
		ass_true(queue1.get("Process 2").notNull());
		ass_true(queue1.get("Process 3").notNull());
		ass_eq(queue1.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue1.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue1.get("Process 1")->pID(), crc32("Process 1"));

		queue2 = queue1;
		ass_eq(destroyed_count, 0);
		ass_true(queue2.get("Process 1").notNull());
		ass_true(queue2.get("Process 2").notNull());
		ass_true(queue2.get("Process 3").notNull());
		ass_eq(queue2.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue2.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue2.get("Process 1")->pID(), crc32("Process 1"));

		queue1.clear();
		ass_eq(destroyed_count, 0);
		ass_true(queue1.get("Process 1").isNull());
		ass_true(queue1.get("Process 2").isNull());
		ass_true(queue1.get("Process 3").isNull());

		ass_eq(destroyed_count, 0);
		ass_true(queue2.get("Process 1").notNull());
		ass_true(queue2.get("Process 2").notNull());
		ass_true(queue2.get("Process 3").notNull());
		ass_eq(queue2.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue2.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue2.get("Process 1")->pID(), crc32("Process 1"));

		queue2.clear();
		ass_eq(destroyed_count, 3);
		ass_true(queue1.get("Process 1").isNull());
		ass_true(queue1.get("Process 2").isNull());
		ass_true(queue1.get("Process 3").isNull());
		

		FINISH_TEST;
	}
	

	void testProcessQueuePush() {
		BEGIN_TEST;

		ProcessQueue queue(4);
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);
		ass_eq(queue.first(), queue.root());

		Boolean retVal = 0;
		destroyed_count = 0;		
		
		retVal = queue.push(TestProcess::create("Process 1", 1));
		ass_true(retVal);		
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);
		ass_neq(queue.first(), queue.root());
		ass_eq(queue.first()->process->pID(), crc32("Process 1"));		
		ass_eq(destroyed_count, 0);		
		
		retVal = queue.push(TestProcess::create("Process 2", 2));
		ass_true(retVal);	
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 2);
		ass_eq(queue.processCount(), 2);
		ass_neq(queue.first(), queue.root());
		ass_eq(queue.first()->process->pID(), crc32("Process 1"));
		ass_eq(destroyed_count, 0);		

		retVal = queue.push(TestProcess::create("Process 3", 3));
		ass_true(retVal);	
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 1);
		ass_eq(queue.processCount(), 3);
		ass_neq(queue.first(), queue.root());
		ass_eq(queue.first()->process->pID(), crc32("Process 1"));
		ass_eq(destroyed_count, 0);		

		retVal = queue.push(TestProcess::create("Process 4", 4));
		ass_true(retVal);	
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);
		ass_neq(queue.first(), queue.root());
		ass_eq(queue.first()->process->pID(), crc32("Process 1"));
		ass_eq(destroyed_count, 0);		

		retVal = queue.push(TestProcess::create("Process 5", 5));
		ass_false(retVal);	
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);
		ass_neq(queue.first(), queue.root());
		ass_eq(queue.first()->process->pID(), crc32("Process 1"));
		ass_eq(destroyed_count, 1);		
		
		FINISH_TEST;
	}

	void testProcessQueueGet() {
		BEGIN_TEST;

		ProcessQueue queue(4);
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);
		ass_eq(queue.first(), queue.root());

		Boolean retVal = 0;
		destroyed_count = 0;

		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		
		retVal = queue.push(TestProcess::create("Process 1", 1));
		ass_true(retVal);		
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);
		ass_neq(queue.first(), queue.root());
		ass_eq(queue.first()->process->pID(), crc32("Process 1"));		
		ass_eq(destroyed_count, 0);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		
		retVal = queue.push(TestProcess::create("Process 2", 2));
		ass_true(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		
		retVal = queue.push(TestProcess::create("Process 3", 3));
		ass_true(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));		
		
		retVal = queue.push(TestProcess::create("Process 4", 4));		
		ass_true(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		
		retVal = queue.push(TestProcess::create("Process 5", 5));
		ass_false(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
	

		FINISH_TEST;
	}

	void testProcessQueueMarkForRemoval() {
		BEGIN_TEST;

		ProcessQueue queue(4);
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);
		ass_eq(queue.first(), queue.root());

		Boolean retVal = 0;
		destroyed_count = 0;

		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		
		retVal = queue.push(TestProcess::create("Process 1", 1));
		ass_true(retVal);		
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));		
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);

		queue.markForRemoval(queue.getNode("Process 1"));
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(destroyed_count, 0);
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);
		
		retVal = queue.push(TestProcess::create("Process 2", 2));
		ass_true(retVal);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 2);
		ass_eq(queue.processCount(), 2);
		
		retVal = queue.push(TestProcess::create("Process 3", 3));
		ass_true(retVal);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 1);
		ass_eq(queue.processCount(), 3);
		
		retVal = queue.push(TestProcess::create("Process 4", 4));		
		ass_true(retVal);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);
		
		retVal = queue.push(TestProcess::create("Process 5", 5));
		ass_false(retVal);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);

		ass_eq(destroyed_count, 1);
		queue.markForRemoval(queue.getNode("Process 3"));
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);
		ass_eq(destroyed_count, 1);

		queue.markForRemoval(queue.getNode("Process 4"));
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);
		ass_eq(destroyed_count, 1);

		queue.markForRemoval(queue.getNode("Process 2"));
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);
		ass_eq(destroyed_count, 1);

		FINISH_TEST;
	}

	void testProcessQueueRemove() {
		BEGIN_TEST;

		ProcessQueue queue(4);
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);
		ass_eq(queue.first(), queue.root());

		Boolean retVal = 0;
		destroyed_count = 0;

		ProcessQueueNode* node = NIL;		
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());

		retVal = queue.push(TestProcess::create("Process 1", 1));
		ass_true(retVal);				
		ass_eq(destroyed_count, 0);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);

		node = queue.getNode("Process 1");		
		queue.markForRemoval(node);		
		queue.remove(node);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(destroyed_count, 1);
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);

		retVal = queue.push(TestProcess::create("Process 1", 1));
		ass_true(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);
		
		retVal = queue.push(TestProcess::create("Process 2", 2));
		ass_true(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 2);
		ass_eq(queue.processCount(), 2);
		
		retVal = queue.push(TestProcess::create("Process 3", 3));
		ass_true(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 1);
		ass_eq(queue.processCount(), 3);
		
		retVal = queue.push(TestProcess::create("Process 4", 4));		
		ass_true(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);
		
		retVal = queue.push(TestProcess::create("Process 5", 5));
		ass_false(retVal);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);

		ass_eq(destroyed_count, 2);
		node = queue.getNode("Process 3");	
		queue.markForRemoval(node);		
		queue.remove(node);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").notNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 1);
		ass_eq(queue.processCount(), 3);
		ass_eq(destroyed_count, 3);

		node = queue.getNode("Process 4");	
		queue.markForRemoval(node);		
		queue.remove(node);
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 2);
		ass_eq(queue.processCount(), 2);
		ass_eq(destroyed_count, 4);

		node = queue.getNode("Process 1");	
		queue.markForRemoval(node);		
		queue.remove(node);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);
		ass_eq(destroyed_count, 5);

		retVal = queue.push(TestProcess::create("Process 5", 5));
		ass_true(retVal);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").notNull());
		ass_eq(queue.get("Process 5")->pID(), crc32("Process 5"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 2);
		ass_eq(queue.processCount(), 2);
		ass_eq(destroyed_count, 5);

		node = queue.getNode("Process 2");	
		queue.markForRemoval(node);		
		queue.remove(node);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").notNull());
		ass_eq(queue.get("Process 5")->pID(), crc32("Process 5"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);
		ass_eq(destroyed_count, 6);

		node = queue.getNode("Process 5");	
		queue.markForRemoval(node);		
		queue.remove(node);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_true(queue.get("Process 5").isNull());
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);
		ass_eq(destroyed_count, 7);

		FINISH_TEST;
	}

	void testProcessQueueClear() {
		BEGIN_TEST;

		ProcessQueue queue(4);
		queue.push(TestProcess::create("Process 1", 1));
		queue.push(TestProcess::create("Process 2", 2));
		queue.push(TestProcess::create("Process 3", 3));
		queue.push(TestProcess::create("Process 4", 4));
		ass_true(queue.get("Process 1").notNull());
		ass_true(queue.get("Process 2").notNull());
		ass_true(queue.get("Process 3").notNull());
		ass_true(queue.get("Process 4").notNull());
		ass_eq(queue.get("Process 4")->pID(), crc32("Process 4"));
		ass_eq(queue.get("Process 3")->pID(), crc32("Process 3"));
		ass_eq(queue.get("Process 2")->pID(), crc32("Process 2"));
		ass_eq(queue.get("Process 1")->pID(), crc32("Process 1"));
		ass_true(queue.hasProcesses());
		ass_true(queue.isFull());
		ass_eq(queue.available(), 0);
		ass_eq(queue.processCount(), 4);

		destroyed_count = 0;
		ass_eq(destroyed_count, 0);		
		queue.clear();
		ass_eq(destroyed_count, 4);
		ass_true(queue.get("Process 1").isNull());
		ass_true(queue.get("Process 2").isNull());
		ass_true(queue.get("Process 3").isNull());
		ass_true(queue.get("Process 4").isNull());
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);

		queue.push(TestProcess::create("Process 5", 5));
		ass_true(queue.get("Process 5").notNull());
		ass_eq(queue.get("Process 5")->pID(), crc32("Process 5"));
		ass_true(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 3);
		ass_eq(queue.processCount(), 1);

		queue.clear();
		ass_eq(destroyed_count, 5);
		ass_true(queue.get("Process 5").isNull());
		ass_false(queue.hasProcesses());
		ass_false(queue.isFull());
		ass_eq(queue.available(), 4);
		ass_eq(queue.processCount(), 0);

		FINISH_TEST;
	}
	
	
	
		
} // namespace cc

int main(int argc, char** argv) {
	cc::testCreateAndDestroyProcessQueue();
	cc::testCopyProcessQueue();	
	cc::testProcessQueuePush();
	cc::testProcessQueueGet();
	cc::testProcessQueueMarkForRemoval();
	cc::testProcessQueueRemove();
	cc::testProcessQueueClear();	
	return 0;
}
