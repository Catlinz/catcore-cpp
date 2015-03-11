#include <cstdio>
#include "core/io/asyncinputtask.h"
#include "core/io/objectinputstream.h"


namespace Cat {

	AsyncInputTask::AsyncInputTask(AsyncInputType inputType, InputStream* inputStream, VPtr buffer, Size arg1, Size arg2) {
		type_ = inputType;
		stream_ = inputStream;
		buffer_ = buffer;
		bytesRead_ = 0;
		arg1_ = arg1;
		arg2_ = arg2;
		setResult(reinterpret_cast<AsyncResult*>(new AsyncReadResult(this, buffer_)));
		setDestroyable(true);		
	}

	AsyncInputTask::~AsyncInputTask() {
		destroy();
	}

	I32 AsyncInputTask::run() {
		switch(type_) {
			case ASYNC_READ_1:
				bytesRead_ = stream_->read(buffer_, arg1_);
				break;
			case ASYNC_READ_2:
				bytesRead_ = stream_->read(buffer_, arg1_, arg2_);
				break;
			case ASYNC_READ_U32:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readU32(buffer_, arg1_);
				break;
			case ASYNC_READ_U64:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readU64(buffer_, arg1_);
				break;
			case ASYNC_READ_I32:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readI32(buffer_, arg1_);
				break;
			case ASYNC_READ_I64:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readI64(buffer_, arg1_);
				break;
			case ASYNC_READ_F32:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readF32(buffer_, arg1_);
				break;
			case ASYNC_READ_F64:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readF64(buffer_, arg1_);
				break;
			case ASYNC_READ_BOOLEAN:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readBoolean(buffer_, arg1_);
				break;
			case ASYNC_READ_CHAR:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readChar(buffer_, arg1_);
				break;
			case ASYNC_READ_CSTR:
				bytesRead_ = reinterpret_cast<DataInputStream*>(stream_)->readCStr(reinterpret_cast<CStr>(buffer_));
				break;
			case ASYNC_READ_OBJECT:
				bytesRead_ = reinterpret_cast<ObjectInputStream*>(stream_)->readObject(reinterpret_cast<Serialisable*>(buffer_));
				break;
		}
		return 0;
	}


	void AsyncInputTask::destroy() {
		AsyncTask::destroy();
		stream_ = NIL;
	}

	void AsyncInputTask::onCompletion() {
		if (getResult()) {
			reinterpret_cast<AsyncReadResult*>(getResult())->taskCompleted(bytesRead_);
		}
	}

	char* AsyncInputTask::getInfo() const {
		Char* str = (Char*)malloc(sizeof(Char)*64);
		sprintf(str, "AsyncInputTask info: %p", this);
		return str;
	}

	//
	// ############### ASYNC READ RESULT ######################
	//
	

	void AsyncReadResult::taskCompleted(Size bytesRead) {
		resultSync().lock();
		m_bytesRead = bytesRead;
		setComplete(true);		
		resultSync().broadcast();
		resultSync().unlock();
	}


} // namespace Cat
