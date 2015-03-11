#include "core/testcore.h"
#include "core/defer/messagequeue.h"

namespace cc {

	class TestHandlerOne {
	  public:
		TestHandlerOne(I32 val)  : m_val(val) {}

		void setVal(I32 val) { m_val = val; }

		static void setVal(VPtr obj, Byte* data) {
			reinterpret_cast<TestHandlerOne*>(obj)->setVal(*(reinterpret_cast<I32*>(data)));
		}

		inline I32 val() const { return m_val; }		

	  private:
		I32 m_val;
	};

	class TestHandlerTwo {
	  public:
		TestHandlerTwo(I32 val1, F64 val2)  : m_val1(val1), m_val2(val2) {}

		void setVals(I32 val1, F64 val2) { m_val1 = val1; m_val2 = val2; }

		static void setVals(VPtr obj, Byte* data) {
			reinterpret_cast<TestHandlerTwo*>(obj)->setVals(
				(reinterpret_cast<I32*>(data))[0],
				(reinterpret_cast<F64*>(&data[sizeof(I32)]))[0]
				);
		}		

		inline I32 val1() const { return m_val1; }
		inline F64 val2() const { return m_val2; }
	  private:
		I32 m_val1;
		F64 m_val2;		
	};	
		

	void testMessageQueueCreateAndDestroy() {
		BEGIN_TEST;

		MessageQueue* queue = new MessageQueue();
		ass_eq(queue->maxMessageTypeID(), 0);		
		ass_eq(queue->messagesOne()->capacity(), 0);
		ass_eq(queue->messagesTwo()->capacity(), 0);
		ass_eq(queue->messages(), NIL);
		ass_eq(queue->processing(), NIL);
		ass_eq(queue->handlers(), NIL);
		
		delete queue;

		queue = new MessageQueue(32, 10);
		ass_eq(queue->maxMessageTypeID(), 10);		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(queue->processing(), NIL);
		ass_neq(queue->handlers(), NIL);
		
		delete queue;		
		
		FINISH_TEST;
	}

	void testMessageQueuePostMessage() {
		BEGIN_TEST;	

		MessageQueue *queue = new MessageQueue(32, 10);
		ass_eq(queue->maxMessageTypeID(), 10);		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(queue->processing(), NIL);
		ass_neq(queue->handlers(), NIL);

		queue->postMessage(Message(1));		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());

		SimpleQueue<Message>::Iterator itr = queue->messagesOne()->begin();
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val().type(), 1);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());

		queue->postMessage(Message(4));
		queue->postMessage(Message(3));
		queue->postMessage(Message(9));

		itr = queue->messagesOne()->begin();
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val().type(), 1);
		itr.next();		
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val().type(), 4);
		itr.next();		
		ass_true(itr.isValid());
		ass_true(itr.hasNext());
		ass_eq(itr.val().type(), 3);
		itr.next();		
		ass_true(itr.isValid());
		ass_false(itr.hasNext());
		ass_eq(itr.val().type(), 9);
		itr.next();
		ass_false(itr.isValid());
		ass_false(itr.hasNext());
		
		delete queue;		
		
		FINISH_TEST;
	}

	void testMessageQueueRegisterAndRemoveHandler() {
		BEGIN_TEST;	

		MessageQueue *queue = new MessageQueue(32, 10);
		ass_eq(queue->maxMessageTypeID(), 10);		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(queue->processing(), NIL);
		ass_neq(queue->handlers(), NIL);

		TestHandlerOne t1(3);		

		queue->registerMessageHandler(2, MessageHandler(&t1, &TestHandlerOne::setVal));
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 0);
		ass_eq(queue->handlers()[2].size(), 1);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);

		TestHandlerTwo t2(5, -903.0);	
		queue->registerMessageHandler(2, MessageHandler(&t2, &TestHandlerTwo::setVals));
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 0);
		ass_eq(queue->handlers()[2].size(), 2);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);
		

		TestHandlerTwo t3(5, -9.0);
		queue->registerMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		queue->registerMessageHandler(1, MessageHandler(&t1, &TestHandlerOne::setVal));
		queue->registerMessageHandler(2, MessageHandler(&t3, &TestHandlerTwo::setVals));
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 1);
		ass_eq(queue->handlers()[2].size(), 3);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[4].size(), 1);
		ass_eq(queue->handlers()[5].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);


		/* Removal */
		/* fail */
		queue->removeMessageHandler(4, MessageHandler(&t2, &TestHandlerTwo::setVals));
		queue->processInternalMessages();		
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 1);
		ass_eq(queue->handlers()[2].size(), 3);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[4].size(), 1);
		ass_eq(queue->handlers()[5].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);

		queue->removeMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		ass_eq(queue->handlers()[4].size(), 1);
		queue->processInternalMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 1);
		ass_eq(queue->handlers()[2].size(), 3);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[4].size(), 0);
		ass_eq(queue->handlers()[5].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);

		queue->removeMessageHandler(2, MessageHandler(&t2, &TestHandlerTwo::setVals));
		ass_eq(queue->handlers()[2].size(), 3);
		queue->processInternalMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 1);
		ass_eq(queue->handlers()[2].size(), 2);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[4].size(), 0);
		ass_eq(queue->handlers()[5].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);

		queue->removeMessageHandler(1, MessageHandler(&t1, &TestHandlerOne::setVal));
		ass_eq(queue->handlers()[1].size(), 1);
		queue->processInternalMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 0);
		ass_eq(queue->handlers()[2].size(), 2);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[4].size(), 0);
		ass_eq(queue->handlers()[5].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);

		queue->removeMessageHandler(2, MessageHandler(&t1, &TestHandlerOne::setVal));
		queue->removeMessageHandler(2, MessageHandler(&t3, &TestHandlerTwo::setVals));
		ass_eq(queue->handlers()[2].size(), 2);
		queue->processInternalMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->handlers()[0].size(), 0);
		ass_eq(queue->handlers()[1].size(), 0);
		ass_eq(queue->handlers()[2].size(), 0);
		ass_eq(queue->handlers()[3].size(), 0);
		ass_eq(queue->handlers()[4].size(), 0);
		ass_eq(queue->handlers()[5].size(), 0);
		ass_eq(queue->handlers()[10].size(), 0);
		delete queue;
		FINISH_TEST;
	}

	void testMessageQueueTriggerMessage() {
		BEGIN_TEST;

		MessageQueue *queue = new MessageQueue(32, 10);
		ass_eq(queue->maxMessageTypeID(), 10);		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(queue->processing(), NIL);
		ass_neq(queue->handlers(), NIL);

		TestHandlerOne t1(3);		
		queue->registerMessageHandler(2, MessageHandler(&t1, &TestHandlerOne::setVal));
		TestHandlerTwo t2(5, -903.4);	
		queue->registerMessageHandler(2, MessageHandler(&t2, &TestHandlerTwo::setVals));
		TestHandlerTwo t3(9, -9.0);
		queue->registerMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		queue->registerMessageHandler(1, MessageHandler(&t1, &TestHandlerOne::setVal));
		queue->registerMessageHandler(2, MessageHandler(&t3, &TestHandlerTwo::setVals));

		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 9);
		ass_eq(t3.val2(), -9.0);
		
		Byte data[16];		

		reinterpret_cast<I32*>(data)[0] = 34;
		reinterpret_cast<F64*>(&data[sizeof(I32)])[0] = 66.77;
		queue->triggerMessage(Message(4, data));
		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);

		reinterpret_cast<I32*>(data)[0] = 12;
		queue->triggerMessage(Message(1, data));
		ass_eq(t1.val(), 12);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);

		reinterpret_cast<I32*>(data)[0] = -99;
		reinterpret_cast<F64*>(&data[sizeof(I32)])[0] = 3.141;
		queue->triggerMessage(Message(2, data));
		ass_eq(t1.val(), -99);
		ass_eq(t2.val1(), -99);
		ass_eq(t2.val2(), 3.141);
		ass_eq(t3.val1(), -99);
		ass_eq(t3.val2(), 3.141);

		queue->removeMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		reinterpret_cast<I32*>(data)[0] = 34;
		reinterpret_cast<F64*>(&data[sizeof(I32)])[0] = 66.77;
		queue->triggerMessage(Message(4, data));
		ass_eq(t1.val(), -99);
		ass_eq(t2.val1(), -99);
		ass_eq(t2.val2(), 3.141);
		ass_eq(t3.val1(), -99);
		ass_eq(t3.val2(), 3.141);
		

		delete queue;		

		FINISH_TEST;
	}

	void testMessageQueuePostAndProcessMessages() {
		BEGIN_TEST;

		MessageQueue *queue = new MessageQueue(32, 10);
		ass_eq(queue->maxMessageTypeID(), 10);		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(queue->processing(), NIL);
		ass_neq(queue->handlers(), NIL);

		TestHandlerOne t1(3);		
		queue->registerMessageHandler(2, MessageHandler(&t1, &TestHandlerOne::setVal));
		TestHandlerTwo t2(5, -903.4);	
		queue->registerMessageHandler(2, MessageHandler(&t2, &TestHandlerTwo::setVals));
		TestHandlerTwo t3(9, -9.0);
		queue->registerMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		queue->registerMessageHandler(1, MessageHandler(&t1, &TestHandlerOne::setVal));
		queue->registerMessageHandler(2, MessageHandler(&t3, &TestHandlerTwo::setVals));

		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 9);
		ass_eq(t3.val2(), -9.0);
		
		Byte data1[16];		

		reinterpret_cast<I32*>(data1)[0] = 34;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = 66.77;
		queue->postMessage(Message(4, data1));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);
		reinterpret_cast<I32*>(data1)[0] = -1;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = -1.0;

		Byte data2[16];	
		reinterpret_cast<I32*>(data2)[0] = 12;
		queue->postMessage(Message(1, data2));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 12);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);
		reinterpret_cast<I32*>(data2)[0] = -1;

		Byte data3[16];
		reinterpret_cast<I32*>(data3)[0] = -99;
		reinterpret_cast<F64*>(&data3[sizeof(I32)])[0] = 3.141;
		queue->postMessage(Message(2, data3));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), -99);
		ass_eq(t2.val1(), -99);
		ass_eq(t2.val2(), 3.141);
		ass_eq(t3.val1(), -99);
		ass_eq(t3.val2(), 3.141);

		reinterpret_cast<I32*>(data1)[0] = -8;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = 88.93;
		reinterpret_cast<I32*>(data2)[0] = 78;
		reinterpret_cast<I32*>(data3)[0] = 231;
		reinterpret_cast<F64*>(&data3[sizeof(I32)])[0] = -3.336;
		
	   queue->postMessage(Message(2, data3));
		queue->postMessage(Message(1, data2));
		queue->postMessage(Message(4, data1));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 78);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), -8);
		ass_eq(t3.val2(), 88.93);

		queue->postMessage(Message(2, data3));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 231);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), 231);
		ass_eq(t3.val2(), -3.336);

		queue->postMessage(Message(2, data3));
		queue->postMessage(Message(1, data2));
		queue->postMessage(Message(4, data1));
		queue->postMessage(Message(2, data3));
		queue->postMessage(Message(1, data2));
		queue->postMessage(Message(2, data3));
		queue->postMessage(Message(4, data1));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 231);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), -8);
		ass_eq(t3.val2(), 88.93);

		queue->removeMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		queue->postMessage(Message(1, data2));
		queue->postMessage(Message(2, data3));
		queue->postMessage(Message(4, data1));
		queue->processMessages();
		ass_eq(t1.val(), 231);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), 231);
		ass_eq(t3.val2(), -3.336);
		
		delete queue;		

		FINISH_TEST;
	}

	void testMessageQueuePostAndProcessMessagesWithSender() {
		BEGIN_TEST;

		MessageQueue *queue = new MessageQueue(32, 10);
		ass_eq(queue->maxMessageTypeID(), 10);		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(queue->processing(), NIL);
		ass_neq(queue->handlers(), NIL);

		TestHandlerOne t1(3);		
		queue->registerMessageHandler(2, MessageHandler(&t1, &TestHandlerOne::setVal));
		TestHandlerTwo t2(5, -903.4);	
		queue->registerMessageHandler(2, MessageHandler(&t2, &TestHandlerTwo::setVals));
		TestHandlerTwo t3(9, -9.0);
		queue->registerMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		queue->registerMessageHandler(1, MessageHandler(&t1, &t2, &TestHandlerOne::setVal));
		queue->registerMessageHandler(2, MessageHandler(&t3, &t1, &TestHandlerTwo::setVals));

		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 9);
		ass_eq(t3.val2(), -9.0);
		
		Byte data1[16];
		
		reinterpret_cast<I32*>(data1)[0] = 34;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = 66.77;
		queue->postMessage(Message(4, data1));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);
		reinterpret_cast<I32*>(data1)[0] = -1;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = -1.0;

		Byte data2[16];
		/* Shouldn't do anything since the Sender is not t2 (NO SENDER) */
		reinterpret_cast<I32*>(data2)[0] = 12;
		queue->postMessage(Message(1, data2));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);

		/* Shouldn't do anything since the Sender is not t2 (WRONG SENDER) */
		queue->postMessage(Message(1, &t3, data2));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);

		/* Shouldn't do anything since the Sender is not t2 (WRONG SENDER) */
		queue->postMessage(Message(1, &t1, data2));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);

		/* SHOULD do something since have right sender */
		queue->postMessage(Message(1, &t2, data2));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 12);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);
		reinterpret_cast<I32*>(data2)[0] = -1;

		Byte data3[16];
		/* SHouldn't do anything to t3, since have wrong sender */
		reinterpret_cast<I32*>(data3)[0] = -99;
		reinterpret_cast<F64*>(&data3[sizeof(I32)])[0] = 3.141;
		queue->postMessage(Message(2, &t2, data3));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), -99);
		ass_eq(t2.val1(), -99);
		ass_eq(t2.val2(), 3.141);
	   ass_eq(t3.val1(), 34);
		ass_eq(t3.val2(), 66.77);

		/* Should set t3 now, since have right sender */
		reinterpret_cast<I32*>(data3)[0] = -99;
		reinterpret_cast<F64*>(&data3[sizeof(I32)])[0] = 3.141;
		queue->postMessage(Message(2, &t1, data3));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), -99);
		ass_eq(t2.val1(), -99);
		ass_eq(t2.val2(), 3.141);
		ass_eq(t3.val1(), -99);
		ass_eq(t3.val2(), 3.141);

		reinterpret_cast<I32*>(data1)[0] = -8;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = 88.93;
		reinterpret_cast<I32*>(data2)[0] = 78;
		reinterpret_cast<I32*>(data3)[0] = 231;
		reinterpret_cast<F64*>(&data3[sizeof(I32)])[0] = -3.336;

		/* Should call both sender specific and non-sender specific */
	   queue->postMessage(Message(2, &t1, data3));
		queue->postMessage(Message(1, &t2, data2));
		queue->postMessage(Message(4, data1));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 78);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), -8);
		ass_eq(t3.val2(), 88.93);

		queue->postMessage(Message(2, &t1, data3));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 231);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), 231);
		ass_eq(t3.val2(), -3.336);

		queue->postMessage(Message(2, &t1, data3));
		queue->postMessage(Message(1, &t2, data2));
		queue->postMessage(Message(4, data1));
		queue->postMessage(Message(2, &t1, data3));
		queue->postMessage(Message(1, &t2, data2));
		queue->postMessage(Message(2, &t1, data3));
		queue->postMessage(Message(4, data1));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 231);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), -8);
		ass_eq(t3.val2(), 88.93);

		queue->removeMessageHandler(4, MessageHandler(&t3, &TestHandlerTwo::setVals));
		queue->postMessage(Message(1, &t2, data2));
		queue->postMessage(Message(2, &t1, data3));
		queue->postMessage(Message(4, data1));
		queue->processMessages();
		ass_eq(t1.val(), 231);
		ass_eq(t2.val1(), 231);
		ass_eq(t2.val2(), -3.336);
		ass_eq(t3.val1(), 231);
		ass_eq(t3.val2(), -3.336);
		
		delete queue;		

		FINISH_TEST;
	}

	void testMessageQueuePostAndProcessMessagesWithSenderMultiple() {
		BEGIN_TEST;

		MessageQueue *queue = new MessageQueue(32, 10);
		ass_eq(queue->maxMessageTypeID(), 10);		
		ass_eq(queue->messagesOne()->capacity(), 32);
		ass_eq(queue->messagesTwo()->capacity(), 32);
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(queue->processing(), NIL);
		ass_neq(queue->handlers(), NIL);

		TestHandlerOne t1(3);
		TestHandlerTwo t2(5, -903.4);
		TestHandlerTwo t3(9, -9.0);
		queue->registerMessageHandler(1, MessageHandler(&t1, &t2, &TestHandlerOne::setVal));
		queue->registerMessageHandler(1, MessageHandler(&t2, &t1, &TestHandlerTwo::setVals));
		queue->registerMessageHandler(1, MessageHandler(&t3, &t1, &TestHandlerTwo::setVals));

		ass_eq(t1.val(), 3);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 9);
		ass_eq(t3.val2(), -9.0);
		
		Byte data1[16];
		reinterpret_cast<I32*>(data1)[0] = 34;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = 66.77;

		/* Should set t1 */
		queue->postMessage(Message(1, &t2, data1));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 34);
		ass_eq(t2.val1(), 5);
		ass_eq(t2.val2(), -903.4);
		ass_eq(t3.val1(), 9);
		ass_eq(t3.val2(), -9.0);

		reinterpret_cast<I32*>(data1)[0] = -99;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = 3.44;

		/* Should set t2, t3 */
		queue->postMessage(Message(1, &t1, data1));
		ass_true(queue->messagesOne()->isEmpty());
		ass_false(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		ass_eq(t1.val(), 34);
		ass_eq(t2.val1(), -99);
		ass_eq(t2.val2(), 3.44);
		ass_eq(t3.val1(), -99);
		ass_eq(t3.val2(), 3.44);

		/* Should set t1, t2, t3 */
		reinterpret_cast<I32*>(data1)[0] = 69;
		reinterpret_cast<F64*>(&data1[sizeof(I32)])[0] = -4.0;
		Byte data2[16];
		reinterpret_cast<I32*>(data2)[0] = 203;
		reinterpret_cast<F64*>(&data2[sizeof(I32)])[0] = -66.77;

		queue->postMessage(Message(1, &t2, data1));
		queue->postMessage(Message(1, &t1, data2));
		ass_false(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesOne());
		queue->processMessages();
		ass_true(queue->messagesOne()->isEmpty());
		ass_true(queue->messagesTwo()->isEmpty());
		ass_eq(queue->messages(), queue->messagesTwo());
		ass_eq(t1.val(), 69);
		ass_eq(t2.val1(), 203);
		ass_eq(t2.val2(), -66.77);
		ass_eq(t3.val1(), 203);
		ass_eq(t3.val2(), -66.77);
		
		delete queue;		

		FINISH_TEST;
	}	

} // namespace cc

int main(int argc, char** argv) {
	cc::testMessageQueueCreateAndDestroy();
	cc::testMessageQueuePostMessage();
	cc::testMessageQueueRegisterAndRemoveHandler();
	cc::testMessageQueueTriggerMessage();
	cc::testMessageQueuePostAndProcessMessages();
	cc::testMessageQueuePostAndProcessMessagesWithSender();
	cc::testMessageQueuePostAndProcessMessagesWithSenderMultiple();	
			
	return 0;
}


