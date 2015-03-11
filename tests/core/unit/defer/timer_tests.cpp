#include "core/testcore.h"
#include "core/defer/timer.h"

namespace cc {



	U32 destroyed_count = 0;
	U32 s_fired_count = 0;	
	U32 r_fired_count = 0;

	void reset_counts() {
		destroyed_count = 0;
		s_fired_count = 0;
		r_fired_count = 0;
	}
	
	class SingularOne : public TimedAction {
	  public:
		SingularOne(const Char* name, const TimeVal& timeToWait, F32 time = 1.0f)
			: TimedAction(name, timeToWait), m_time(time) {}

		~SingularOne() {
			DMSG("SingularOne " << name() << " destroyed!");
			destroyed_count++;
		}		

		Boolean fire() {
			usleep(Time::secondsToNano(m_time)/1000);
			s_fired_count++;			
			DMSG("Singular Action " << name() << " fired!");
			return true;			
		}

		void onInitialize() {
			DMSG("Singular Action " << name() << " Initialized.");
		}		

		inline static TimedActionPtr create(const Char* name, const TimeVal& timeToWait, F32 time = 1.0f) { return TimedActionPtr(new SingularOne(name, timeToWait, time)); }		

	  private:
		F32 m_time;
	};

	class RepeatedOne : public TimedAction {
	  public:
	   RepeatedOne(const Char* name, const TimeVal& timeToWait, F32 time = 1.0f, U32 repeats = 1)
			: TimedAction(name, timeToWait), m_time(time), m_repeats(repeats) {}

		~RepeatedOne() {
			DMSG("RepeatedOne " << name() << " destroyed!");
			destroyed_count++;
		}
		
		Boolean fire() {
			usleep(Time::secondsToNano(m_time)/1000);
			r_fired_count++;			
			DMSG("Repeated Action " << name() << " fired!");
			--m_repeats;
			if (m_repeats <= 0) {
				DMSG("Repeated Action " << name() << " Finished repeating!");
			}			
			return (m_repeats > 0);			
		}

		void onInitialize() {
			DMSG("Repeated Action " << name() << " Initialized.");
		}

		inline static TimedActionPtr create(const Char* name, const TimeVal& timeToWait, F32 time = 1.0f, U32 repeats = 1) { return TimedActionPtr(new RepeatedOne(name, timeToWait, time, repeats)); }		


	  private:
		F32 m_time;
		U32 m_repeats;		
	};
		

	void testTimerCreateAndDestroy() {
		BEGIN_TEST;

		Timer* t1 = new Timer(10);
		ass_eq(t1->singularQueue()->capacity(), 10);
		ass_eq(t1->repeatedQueue()->capacity(), 10);
		ass_true(t1->singularQueue()->isEmpty());
		ass_true(t1->repeatedQueue()->isEmpty());
		ass_eq(t1->messageQueue()->capacity(), 68);
		ass_true(t1->messageQueue()->isEmpty());		
		ass_eq(t1->singular()->next, t1->singular());
		ass_eq(t1->repeated()->next, t1->repeated());
		delete t1;

		t1 = new Timer(11, 22);
		ass_eq(t1->singularQueue()->capacity(), 11);
		ass_eq(t1->repeatedQueue()->capacity(), 11);
		ass_true(t1->singularQueue()->isEmpty());
		ass_true(t1->repeatedQueue()->isEmpty());
		ass_eq(t1->messageQueue()->capacity(), 55);
		ass_true(t1->messageQueue()->isEmpty());		
		ass_eq(t1->singular()->next, t1->singular());
		ass_eq(t1->repeated()->next, t1->repeated());
		delete t1;		
		
		
		FINISH_TEST;
	}

	void testTimerRegisterSingularAndRepeated() {
		BEGIN_TEST;

		destroyed_count = 0;		

		Timer* t = new Timer(10);
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());


		TimedActionPtr action = SingularOne::create("S1", TimeVal(Time::secondsToRaw(0.016)));		
		U64 actionID = t->registerSingular(action);
		ass_eq(actionID, 1);		
		ass_eq(t->singularQueue()->capacity(), 10);	
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_false(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);
		ass_true(t->singularQueue()->at(1).isNull());

		action = RepeatedOne::create("R1", TimeVal(Time::secondsToRaw(0.016)));		
		actionID = t->registerRepeated(action);
		ass_eq(actionID, 2);	
		ass_eq(t->singularQueue()->capacity(), 10);	
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_false(t->singularQueue()->isEmpty());
		ass_false(t->repeatedQueue()->isEmpty());
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);
		ass_true(t->singularQueue()->at(1).isNull());
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_true(t->repeatedQueue()->at(1).isNull());

		action = SingularOne::create("S3", TimeVal(Time::secondsToRaw(0.016)));
		actionID = t->registerSingular(action);
		ass_eq(actionID, 3);
		action = SingularOne::create("S2", TimeVal(Time::secondsToRaw(0.016)));
		actionID = t->registerSingular(action);
		ass_eq(actionID, 4);
		ass_eq(t->singularQueue()->capacity(), 10);	
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_false(t->singularQueue()->isEmpty());
		ass_false(t->repeatedQueue()->isEmpty());
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);
		ass_eq(strcmp(t->singularQueue()->at(1)->name(), "S3"), 0);
		ass_eq(strcmp(t->singularQueue()->at(2)->name(), "S2"), 0);
		ass_true(t->singularQueue()->at(3).isNull());
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_true(t->repeatedQueue()->at(1).isNull());

		action = RepeatedOne::create("R4", TimeVal(Time::secondsToRaw(0.016)));		
		actionID = t->registerRepeated(action);
		ass_eq(actionID, 5);
		action = RepeatedOne::create("R3", TimeVal(Time::secondsToRaw(0.016)));		
		actionID = t->registerRepeated(action);
		ass_eq(actionID, 6);
		action = RepeatedOne::create("R8", TimeVal(Time::secondsToRaw(0.016)));		
		actionID = t->registerRepeated(action);
		ass_eq(actionID, 7);
		ass_eq(t->singularQueue()->capacity(), 10);	
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_false(t->singularQueue()->isEmpty());
		ass_false(t->repeatedQueue()->isEmpty());
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);
		ass_eq(strcmp(t->singularQueue()->at(1)->name(), "S3"), 0);
		ass_eq(strcmp(t->singularQueue()->at(2)->name(), "S2"), 0);
		ass_true(t->singularQueue()->at(3).isNull());
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(1)->name(), "R4"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(2)->name(), "R3"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(3)->name(), "R8"), 0);
		ass_true(t->repeatedQueue()->at(4).isNull());

		action.setNull();		

		ass_eq(destroyed_count, 0);		
		delete t;
		ass_eq(destroyed_count, 7);	
		
		
		FINISH_TEST;
	}

	void testTimerConsumeInputQueues() {
		BEGIN_TEST;

		destroyed_count = 0;		

		Timer* t = new Timer(10);
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());

		t->consumeInputQueues();
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());

		TimedActionPtr action = SingularOne::create("S1", TimeVal(Time::secondsToRaw(0.016)));		
		t->registerSingular(action);
		action.setNull();		
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		t->consumeInputQueues();
	
		ass_true(t->singularQueue()->at(0).isNull());
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_neq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		t->consumeInputQueues();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_neq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());

		
		action = RepeatedOne::create("R1", TimeVal(Time::secondsToRaw(0.016)));		
		t->registerRepeated(action);		
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		t->consumeInputQueues();
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_neq(t->singular()->next, t->singular());
		ass_neq(t->repeated()->next, t->repeated());
		t->consumeInputQueues();
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_neq(t->singular()->next, t->singular());
		ass_neq(t->repeated()->next, t->repeated());

		action = SingularOne::create("S3", TimeVal(Time::secondsToRaw(0.016)));
		t->registerSingular(action);
		action = SingularOne::create("S2", TimeVal(Time::secondsToRaw(0.016)));
		t->registerSingular(action);
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S3"), 0);
		ass_eq(strcmp(t->singularQueue()->at(1)->name(), "S2"), 0);
		ass_true(t->singularQueue()->at(2).isNull());   

		action = RepeatedOne::create("R4", TimeVal(Time::secondsToRaw(0.016)));		
		t->registerRepeated(action);
		action = RepeatedOne::create("R3", TimeVal(Time::secondsToRaw(0.016)));		
		t->registerRepeated(action);
		action = RepeatedOne::create("R8", TimeVal(Time::secondsToRaw(0.016)));		
		t->registerRepeated(action);
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R4"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(1)->name(), "R3"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(2)->name(), "R8"), 0);
		ass_true(t->repeatedQueue()->at(3).isNull());

		t->consumeInputQueues();	  
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_neq(t->singular()->next, t->singular());
		ass_neq(t->repeated()->next, t->repeated());
		
		action.setNull();		

		ass_eq(destroyed_count, 0);		
		delete t;
		ass_eq(destroyed_count, 7);	
		
		
		FINISH_TEST;
	}

	void testTimerProcessMessages() {
		BEGIN_TEST;

		destroyed_count = 0;		

		Timer* t = new Timer(10);
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());


		/* TEST WITH NOTHING IN THE MESSAGE QUEUE */
		DMSG("Test ProcessMessages() with nothing in the message queue.");		
		t->processMessages();


		/* TEST REMOVING WITH NOTHING ADDED OR IN THE INPUT QUEUES */
		DMSG("Test Unregister with nothing added or in input queues.");	
		t->unregisterSingular(1);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		t->unregisterRepeated(1);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		t->unregisterSingular(1);
		t->unregisterRepeated(1);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());


		/* TEST REMOVING WITH THINGS IN THE INPUT QUEUES */
		DMSG("Test Unregister with stuff in the input queues.");
		TimedActionPtr action = SingularOne::create("S1", TimeVal(Time::secondsToRaw(0.016)));		
		U64 a1 = t->registerSingular(action);
		action = RepeatedOne::create("R1", TimeVal(Time::secondsToRaw(0.016)));		
		U64 a2 = t->registerRepeated(action);
		action.setNull();		
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_true(t->messageQueue()->isEmpty());		

		t->unregisterSingular(a1);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_true(t->messageQueue()->isEmpty());	
		t->unregisterRepeated(a2);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_true(t->messageQueue()->isEmpty());	
		t->unregisterSingular(a1);
		t->unregisterRepeated(a2);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_true(t->messageQueue()->isEmpty());	


		/* TEST REMOVING SINGULAR AND REPEATED NOT THERE ACTION */
		DMSG("Test Unregister with stuff added, but wrong actionID's");
		t->consumeInputQueues();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());
		
		t->unregisterSingular(a2);
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());

		t->unregisterRepeated(a1);
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());

		ass_eq(destroyed_count, 0);		
		
		/* TEST REMOVING SINGULAR ACTION */
		DMSG("Test UnregisterSingular().");
		t->unregisterSingular(a1);
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 1);
		
		/* TEST REMOVING REPEATED ACTION */
		DMSG("Test UnregisterRepeated().");
		t->unregisterRepeated(a2);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_false(t->messageQueue()->isEmpty());
		t->processMessages();
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 2);
		
		/* TEST REMOVING WITH MULTIPLE ACTIONS */
		DMSG("Test UnregisterSingular() and UnregisterRepeated() with multiple actions.");
		action = SingularOne::create("S3", TimeVal(Time::secondsToRaw(0.016)));
		U64 as1 = t->registerSingular(action);
		action = SingularOne::create("S2", TimeVal(Time::secondsToRaw(0.016)));
		U64 as2 = t->registerSingular(action);
		action = SingularOne::create("S5", TimeVal(Time::secondsToRaw(0.016)));
		U64 as3 = t->registerSingular(action);
		action = SingularOne::create("S4", TimeVal(Time::secondsToRaw(0.016)));
		U64 as4 = t->registerSingular(action);
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S3"), 0);
		ass_eq(strcmp(t->singularQueue()->at(1)->name(), "S2"), 0);
		ass_eq(strcmp(t->singularQueue()->at(2)->name(), "S5"), 0);
		ass_eq(strcmp(t->singularQueue()->at(3)->name(), "S4"), 0);
		ass_true(t->singularQueue()->at(4).isNull());   
		action = RepeatedOne::create("R4", TimeVal(Time::secondsToRaw(0.016)));		
		U64 ar1 = t->registerRepeated(action);
		action = RepeatedOne::create("R3", TimeVal(Time::secondsToRaw(0.016)));		
		U64 ar2 = t->registerRepeated(action);
		action = RepeatedOne::create("R8", TimeVal(Time::secondsToRaw(0.016)));		
		U64 ar3 = t->registerRepeated(action);
		ass_gt(ar3, 0);		
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R4"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(1)->name(), "R3"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(2)->name(), "R8"), 0);
		ass_true(t->repeatedQueue()->at(3).isNull());
		t->consumeInputQueues();
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as2);
		ass_eq(t->singular()->next->next->next->ptr->actionID(), as3);
		ass_eq(t->singular()->next->next->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar2);
		ass_eq(t->repeated()->next->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next->next, t->repeated());
		action.setNull();
		
		/* Post message to Remove S2 (Middle) */
		DMSG("Posting message to remove S2.");
		t->unregisterSingular(as2);
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_false(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as2);
		ass_eq(t->singular()->next->next->next->ptr->actionID(), as3);
		ass_eq(t->singular()->next->next->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar2);
		ass_eq(t->repeated()->next->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next->next, t->repeated());
		ass_eq(destroyed_count, 2);
		/* Process message to remove S2 (should be removed now) */
		DMSG("Processing message and removing S2.");
		t->processMessages();
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as3);
		ass_eq(t->singular()->next->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar2);
		ass_eq(t->repeated()->next->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next->next, t->repeated());
		ass_eq(destroyed_count, 3);
		
		/* Post message to Remove R8 (End) */
		DMSG("Posting message to remove R8.");
		t->unregisterRepeated(ar3);
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_false(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as3);
		ass_eq(t->singular()->next->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar2);
		ass_eq(t->repeated()->next->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next->next, t->repeated());
		ass_eq(destroyed_count, 3);
		/* Process message to remove S8 (should be removed now) */
		DMSG("Processing message and removing R8.");
		t->processMessages();
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as3);
		ass_eq(t->singular()->next->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar2);
		ass_eq(t->repeated()->next->next->next, t->repeated());
		ass_eq(destroyed_count, 4);
		

		/* TEST UNREGISTERING MULTIPLE ACTIONS */
		t->unregisterRepeated(ar1);
		t->unregisterSingular(as3);
		t->unregisterSingular(as1);		
		t->unregisterRepeated(ar2);
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_false(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as3);
		ass_eq(t->singular()->next->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar2);
		ass_eq(t->repeated()->next->next->next, t->repeated());
		ass_eq(destroyed_count, 4);

		t->processMessages();
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_eq(destroyed_count, 8);

		ass_eq(destroyed_count, 8);		
		delete t;
		ass_eq(destroyed_count, 9);	
		
		
		FINISH_TEST;
	}

	void testTimerTick() {
		BEGIN_TEST;

		reset_counts();		

		Timer* t = new Timer(10);
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());


		/* TEST TICKING WITH NO MESSAGES OR ACTIONS */
		DMSG("Test empty tick().");
		t->tick();
		ass_eq(t->singularQueue()->capacity(), 10);
		ass_eq(t->repeatedQueue()->capacity(), 10);
		ass_true(t->singularQueue()->isEmpty());
		ass_true(t->repeatedQueue()->isEmpty());
		ass_eq(t->messageQueue()->capacity(), 68);
		ass_true(t->messageQueue()->isEmpty());		
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());		


		/* TEST TICKING WITH INPUT IN QUEUES */
		DMSG("Test tick() with stuff in the input queues.");
		TimedActionPtr action = SingularOne::create("S1", TimeVal(Time::secondsToRaw(1.0)));		
		U64 a1 = t->registerSingular(action);
		action = RepeatedOne::create("R1", TimeVal(Time::secondsToRaw(1.0)));		
		U64 a2 = t->registerRepeated(action);
		action.setNull();		
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_true(t->messageQueue()->isEmpty());

		t->tick();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());

		
		/* TEST TICK AND REMOVE ACTIONS */
		ass_eq(destroyed_count, 0);		
		DMSG("Test tick() and unregister");
		t->unregisterSingular(a1);
		t->unregisterRepeated(a2);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_false(t->messageQueue()->isEmpty());
		t->tick();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 2);


		/* TEST TICKING WITH INPUT QUEUES AND UNREGISTER AT SAME TIME */
		DMSG("Test tick() with stuff in the input queues and unregister");
		action = SingularOne::create("S1", TimeVal(Time::secondsToRaw(1.0)));		
		a1 = t->registerSingular(action);
		action = RepeatedOne::create("R1", TimeVal(Time::secondsToRaw(1.0)));		
		a2 = t->registerRepeated(action);
		action.setNull();		
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		t->unregisterSingular(a1);
		t->unregisterRepeated(a2);
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S1"), 0);	
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R1"), 0);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_false(t->messageQueue()->isEmpty());
		t->tick();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
	   ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);
		
		/* TEST TICKING AND FIRING */
		DMSG("Test tick() and firing Single and 1 Repeat");
		action = SingularOne::create("S1", TimeVal(Time::secondsToRaw(0.01)), 0.1);		
		a1 = t->registerSingular(action);
		action = RepeatedOne::create("R1", TimeVal(Time::secondsToRaw(0.01)), 0.1);		
		a2 = t->registerRepeated(action);
		action.setNull();
		t->tick();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());

		usleep(5000);
		t->tick();
		ass_eq(s_fired_count, 0);
		ass_eq(r_fired_count, 0);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 4);
		usleep(5000);
		t->tick();
		ass_eq(s_fired_count, 1);
		ass_eq(r_fired_count, 1);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);

		
		/* TEST REPEATING */
		DMSG("Test tick() and firing Single and 2 Repeat");
		action = SingularOne::create("S2", TimeVal(Time::secondsToRaw(0.01)), 0.00001);		
		a1 = t->registerSingular(action);
		action = RepeatedOne::create("R2", TimeVal(Time::secondsToRaw(0.01)), 0.00001, 2);		
		a2 = t->registerRepeated(action);
		action.setNull();
		t->tick();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());		
		usleep(5000);
		
		t->tick();
		ass_eq(s_fired_count, 1);
		ass_eq(r_fired_count, 1);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 6);
		usleep(5000);
		
		t->tick(); /* S2 fire, R2 fire and reset */
		ass_eq(s_fired_count, 2);
		ass_eq(r_fired_count, 2);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
	   ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_neq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 7);
		usleep(5000);
		
		t->tick();		
		ass_eq(s_fired_count, 2);
		ass_eq(r_fired_count, 2);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
	   ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_neq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 7);	
		usleep(5000);
		
		t->tick();	
		ass_eq(s_fired_count, 2);
		ass_eq(r_fired_count, 3);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);

		/* TEST REMOVING BEFORE SHOULD FIRE */
		DMSG("Test tick() and remove before should fire.");
		action = SingularOne::create("S3", TimeVal(Time::secondsToRaw(0.01)), 0.00001);		
		a1 = t->registerSingular(action);
		action = RepeatedOne::create("R3", TimeVal(Time::secondsToRaw(0.01)), 0.00001, 2);		
		a2 = t->registerRepeated(action);
		action.setNull();
		t->tick();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());		
		usleep(5000);
		
		t->tick();
		ass_eq(s_fired_count, 2);
		ass_eq(r_fired_count, 3);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 8);
		usleep(5000);

		t->unregisterSingular(a1);
		t->unregisterRepeated(a2);
		t->tick();		
		ass_eq(s_fired_count, 2);
		ass_eq(r_fired_count, 3);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 10);

		/* TEST MULTIPLE WITH DIFFERENT TIMES */
		DMSG("Test tick() and fire with multiple different times");
		action = SingularOne::create("S3", TimeVal(Time::secondsToRaw(0.01)), 0.00001);
		U64 as1 = t->registerSingular(action);
		action = SingularOne::create("S2", TimeVal(Time::secondsToRaw(0.005)), 0.00001);
		U64 as2 = t->registerSingular(action);
		action = SingularOne::create("S5", TimeVal(Time::secondsToRaw(0.005)), 0.00001);
		U64 as3 = t->registerSingular(action);
		action = SingularOne::create("S4", TimeVal(Time::secondsToRaw(0.01)), 0.00001);
		U64 as4 = t->registerSingular(action);
		ass_eq(strcmp(t->singularQueue()->at(0)->name(), "S3"), 0);
		ass_eq(strcmp(t->singularQueue()->at(1)->name(), "S2"), 0);
		ass_eq(strcmp(t->singularQueue()->at(2)->name(), "S5"), 0);
		ass_eq(strcmp(t->singularQueue()->at(3)->name(), "S4"), 0);
		ass_true(t->singularQueue()->at(4).isNull());   
		action = RepeatedOne::create("R4", TimeVal(Time::secondsToRaw(0.01)), 0.00001, 2);		
		U64 ar1 = t->registerRepeated(action);
		action = RepeatedOne::create("R3", TimeVal(Time::secondsToRaw(0.005)), 0.00001, 1);		
		U64 ar2 = t->registerRepeated(action);
		action = RepeatedOne::create("R8", TimeVal(Time::secondsToRaw(0.01)), 0.00001, 3);		
		U64 ar3 = t->registerRepeated(action);
		ass_gt(ar3, 0);		
		ass_eq(strcmp(t->repeatedQueue()->at(0)->name(), "R4"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(1)->name(), "R3"), 0);
		ass_eq(strcmp(t->repeatedQueue()->at(2)->name(), "R8"), 0);
		ass_true(t->repeatedQueue()->at(3).isNull());
		t->tick();		
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as2);
		ass_eq(t->singular()->next->next->next->ptr->actionID(), as3);
		ass_eq(t->singular()->next->next->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar2);
		ass_eq(t->repeated()->next->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next->next, t->repeated());
		action.setNull();

		usleep(5000);
		t->tick(); /* S2, S5, fire and finish, R3 fire and finish */
		ass_eq(s_fired_count, 4);
		ass_eq(r_fired_count, 4);
		ass_eq(t->singular()->next->ptr->actionID(), as1);
		ass_eq(t->singular()->next->next->ptr->actionID(), as4);
		ass_eq(t->singular()->next->next->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next, t->repeated());
		ass_eq(destroyed_count, 13);

		usleep(5000);
		t->tick(); /* S3, S4, fire and finish, R4, R8 fire */
		ass_eq(s_fired_count, 6);
		ass_eq(r_fired_count, 6);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next, t->repeated());
		ass_eq(destroyed_count, 15);

		usleep(5000);
		t->tick(); /* Nothing */
		ass_eq(s_fired_count, 6);
		ass_eq(r_fired_count, 6);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar1);
		ass_eq(t->repeated()->next->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next->next, t->repeated());
		ass_eq(destroyed_count, 15);

		usleep(5000);
		t->tick(); /* R4, R8 fire, R4 finsihes */
		ass_eq(s_fired_count, 6);
		ass_eq(r_fired_count, 8);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next, t->repeated());
		ass_eq(destroyed_count, 16);

			usleep(5000);
		t->tick(); /* R4, R8 fire, R4 finsihes */
		ass_eq(s_fired_count, 6);
		ass_eq(r_fired_count, 8);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next->ptr->actionID(), ar3);
		ass_eq(t->repeated()->next->next, t->repeated());
		ass_eq(destroyed_count, 16);

			usleep(5000);
		t->tick(); /* R8 fires, R8 finsihes */
		ass_eq(s_fired_count, 6);
		ass_eq(r_fired_count, 9);
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_eq(destroyed_count, 17);


		/* TEST UNREGISTER AFTER ALREADY FIRED */
		DMSG("Test tick() unregister after fired.");
		action = SingularOne::create("S2", TimeVal(Time::secondsToRaw(0.01)), 0.00001);		
		a1 = t->registerSingular(action);
		action = RepeatedOne::create("R2", TimeVal(Time::secondsToRaw(0.01)), 0.00001, 2);		
		a2 = t->registerRepeated(action);
		action.setNull();
		t->tick();
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());		
		usleep(5000);
		
		t->tick();
		ass_eq(s_fired_count, 6);
		ass_eq(r_fired_count, 9);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next->ptr->actionID(), a1);
		ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 17);
		usleep(5000);
		
		t->tick(); /* S2 fire, R2 fire and reset */
		ass_eq(s_fired_count, 7);
		ass_eq(r_fired_count, 10);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
	   ass_eq(t->repeated()->next->ptr->actionID(), a2);
		ass_neq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 18);
		usleep(5000);

		t->unregisterSingular(a1);
		t->unregisterRepeated(a2);

		t->tick(); /* R2 removed */
		ass_eq(s_fired_count, 7);
		ass_eq(r_fired_count, 10);
		ass_true(t->singularQueue()->at(0).isNull());
		ass_true(t->repeatedQueue()->at(0).isNull());
		ass_eq(t->singular()->next, t->singular());
		ass_eq(t->repeated()->next, t->repeated());
		ass_true(t->messageQueue()->isEmpty());
		ass_eq(destroyed_count, 19);
		usleep(5000);
		
		ass_eq(destroyed_count, 19);		
		delete t;
		ass_eq(destroyed_count, 19);	
		
		
		FINISH_TEST;
	}
	

} // namespace cc

int main(int argc, char** argv) {
	cc::testTimerCreateAndDestroy();
	cc::testTimerRegisterSingularAndRepeated();
	cc::testTimerConsumeInputQueues();	
	cc::testTimerProcessMessages();
	cc::testTimerTick();	
			
	return 0;
}


