#include <cstdio>
#include "core/io/asyncoutputtask.h"
#include "core/io/objectoutputstream.h"


namespace Cat {

	AsyncOutputTask::AsyncOutputTask(AsyncOutputType outputType, OutputStream* outputStream, VPtr buffer, Size arg1, Size arg2) {
		type_ = outputType;
		stream_ = outputStream;
		buffer_ = buffer;
		bytesWritten_ = 0;
		arg1_ = arg1;
		arg2_ = arg2;
		setResult(reinterpret_cast<AsyncResult*>(new AsyncWriteResult(this)));
		setDestroyable(true);	
	}

	AsyncOutputTask::~AsyncOutputTask() {
		destroy();
	}

	I32 AsyncOutputTask::run() {
		switch(type_) {
			case ASYNC_WRITE_1:
				bytesWritten_ = stream_->write(buffer_, arg1_);
				break;
			case ASYNC_WRITE_2:
				bytesWritten_ = stream_->write(buffer_, arg1_, arg2_);
				break;
			case ASYNC_WRITE_U32:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeU32(buffer_, arg1_);
				break;
			case ASYNC_WRITE_U64:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeU64(buffer_, arg1_);
				break;
			case ASYNC_WRITE_I32:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeI32(buffer_, arg1_);
				break;
			case ASYNC_WRITE_I64:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeI64(buffer_, arg1_);
				break;
			case ASYNC_WRITE_F32:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeF32(buffer_, arg1_);
				break;
			case ASYNC_WRITE_F64:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeF64(buffer_, arg1_);
				break;
			case ASYNC_WRITE_BOOLEAN:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeBoolean(buffer_, arg1_);
				break;
			case ASYNC_WRITE_CHAR:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeChar(buffer_, arg1_);
				break;
			case ASYNC_WRITE_CSTR:
				bytesWritten_ = reinterpret_cast<DataOutputStream*>(stream_)->writeCStr(reinterpret_cast<CStr>(buffer_));
				break;
			case ASYNC_WRITE_OBJECT:
				bytesWritten_ = reinterpret_cast<ObjectOutputStream*>(stream_)->writeObject(reinterpret_cast<Serialisable*>(buffer_));
				break;
		}
		return 0;
	}

	void AsyncOutputTask::destroy() {
	   AsyncTask::destroy();
		stream_ = NIL;
	}

	void AsyncOutputTask::onCompletion() {
		if (getResult()) {
			reinterpret_cast<AsyncWriteResult*>(getResult())->taskCompleted(bytesWritten_);
		}
	}

	char* AsyncOutputTask::getInfo() const {
		Char* str = (Char*)malloc(sizeof(Char)*64);
		sprintf(str, "AsyncOutputTask info: %p", this);
		return str;
	}

	//
	// ############### ASYNC WRITE RESULT ######################
	//

	void AsyncWriteResult::taskCompleted(Size bytesWritten) {
		resultSync().lock();
		setComplete(true);	
		m_bytesWritten = bytesWritten;
	   resultSync().broadcast();
		resultSync().unlock();
	}

} // namespace Cat

