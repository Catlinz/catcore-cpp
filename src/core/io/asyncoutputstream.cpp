#include "core/io/asyncoutputstream.h"
#include "core/io/outputstream.h"
#include "core/io/iomanager.h"
#include "core/io/asyncoutputtask.h"
#include "core/threading/asynctaskrunner.h"



namespace Cat {

	AsyncOutputStream::AsyncOutputStream(OutputStream* outputStream) {
		stream_ = outputStream;
	}

	AsyncWriteResult* AsyncOutputStream::write(VPtr buffer, Size toWrite) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_1, stream_, buffer, toWrite);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	

		return result;
	}

	AsyncWriteResult* AsyncOutputStream::write(VPtr buffer, Size count, Size size) {
		AsyncOutputTask* task = new AsyncOutputTask(ASYNC_WRITE_2, stream_, buffer, count, size);
		AsyncWriteResult* result = reinterpret_cast<AsyncWriteResult*>(task->getResult());
		
		IOManager::getInstance()->getTaskRunner()->run(task); 	

		return result;

	}

	void AsyncOutputStream::close() {
		if (stream_) {
			stream_->close();
		}
	}

	Boolean AsyncOutputStream::canWrite() {
		if (stream_) {
			return stream_->canWrite();
		}
		else {
			return false;
		}
	}


} // namespace Cat

