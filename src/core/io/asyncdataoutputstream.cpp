#include "core/io/asyncdataoutputstream.h"
#include "core/io/iomanager.h"
#include "core/io/asyncoutputtask.h"
#include "core/threading/asynctaskrunner.h"


namespace Cat {

	AsyncWriteResult* AsyncDataOutputStream::writeU32(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_U32, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeU64(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_U64, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeI32(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_I32, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeI64(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_I64, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeF32(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_F32, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeF64(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_F64, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeBoolean(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_BOOLEAN, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeChar(VPtr buffer, Size count) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_CHAR, getOutputStream(), buffer, count);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncWriteResult* AsyncDataOutputStream::writeCStr(Char* buffer) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_CSTR, getOutputStream(), buffer);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}










} // namespace Cat

