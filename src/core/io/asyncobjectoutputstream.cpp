#include "core/io/asyncobjectoutputstream.h"
#include "core/io/iomanager.h"
#include "core/io/asyncoutputtask.h"
#include "core/threading/asynctaskrunner.h"


namespace Cat {

	AsyncWriteResult* AsyncObjectOutputStream::writeObject(Serialisable* object) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_OBJECT, getOutputStream(), object);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}
} // namespace Cat


