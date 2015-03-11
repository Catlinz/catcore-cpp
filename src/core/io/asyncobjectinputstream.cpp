#include "core/io/asyncobjectinputstream.h"
#include "core/io/iomanager.h"
#include "core/io/asyncinputtask.h"
#include "core/threading/asynctaskrunner.h"


namespace Cat {

	AsyncReadResult* AsyncObjectInputStream::readObject(Serialisable* object) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_OBJECT, getInputStream(), object);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}
} // namespace Cat

