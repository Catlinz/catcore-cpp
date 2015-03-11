#include "core/io/asyncinputstream.h"
#include "core/io/inputstream.h"
#include "core/io/iomanager.h"
#include "core/io/asyncinputtask.h"
#include "core/threading/asynctaskrunner.h"



namespace Cat {

	AsyncInputStream::AsyncInputStream(InputStream* inputStream) {
		stream_ = inputStream;
	}

	AsyncReadResult* AsyncInputStream::read(VPtr buffer, Size toRead) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_1, stream_, buffer, toRead);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	

		return result;
	}

	AsyncReadResult* AsyncInputStream::read(VPtr buffer, Size count, Size size) {
		AsyncInputTask* task = new AsyncInputTask(ASYNC_READ_2, stream_, buffer, count, size);
		AsyncReadResult* result = reinterpret_cast<AsyncReadResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	

		return result;

	}

	void AsyncInputStream::close() {
		if (stream_) {
			stream_->close();
		}
	}

	Boolean AsyncInputStream::canRead() {
		if (stream_) {
			return stream_->canRead();
		}
		else {
			return false;
		}
	}


} // namespace Cat
