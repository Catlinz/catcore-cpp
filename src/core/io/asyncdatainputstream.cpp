#include "core/io/asyncdatainputstream.h"
#include "core/io/iomanager.h"
#include "core/io/asyncinputtask.h"
#include "core/threading/asynctaskrunner.h"


namespace Cat {

	AsyncReadResult* AsyncDataInputStream::readU32(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_U32, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readU64(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_U64, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readI32(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_I32, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readI64(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_I64, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readF32(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_F32, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readF64(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_F64, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readBoolean(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_BOOLEAN, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readChar(VPtr buffer, Size count) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_CHAR, getInputStream(), buffer, count);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}

	AsyncReadResult* AsyncDataInputStream::readCStr(CStr buffer) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_CSTR, getInputStream(), buffer);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	
		return result;
	}










} // namespace Cat
