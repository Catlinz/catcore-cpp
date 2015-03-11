#ifndef CAT_CORE_IO_ASYNCINPUTTASK_H
#define CAT_CORE_IO_ASYNCINPUTTASK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncinputtask.h
 * @brief Definition of the AsyncInputTask to run Async Input calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 17, 2013
 */

#include "core/threading/asynctask.h"
#include "core/threading/asyncresult.h"

namespace Cat {

	enum AsyncInputType {
		ASYNC_READ_1, ASYNC_READ_2, 
		ASYNC_READ_U32, ASYNC_READ_U64, 
		ASYNC_READ_I32, ASYNC_READ_I64,
		ASYNC_READ_F32, ASYNC_READ_F64,
		ASYNC_READ_BOOLEAN,
		ASYNC_READ_CHAR, 
		ASYNC_READ_CSTR,
		ASYNC_READ_OBJECT
	};

	class InputStream;
	class AsyncReadResult;

	/**
	 * @class AsyncInputTask asyncinputtask.h "core/io/asyncinputtask.h"
	 * @brief The AsyncTask responsible for completing the async reads.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Oct 17, 2013
	 */
	class AsyncInputTask : public AsyncTask {
	  public:
		/**
		 * @brief Creates a new AsyncInputTask of the specified type.
		 * @param inputType The type of input operation to perform.
		 * @param inputStream The InputStream to read from.
		 * @param buffer The buffer to read the data into.
		 * @param arg1 The first argument to the read call.
		 * @param arg2 The (optional) second argument to the read call.
		 */
		AsyncInputTask(AsyncInputType inputType, InputStream* inputStream, VPtr buffer, Size arg1 = 0, Size arg2 = 0);

		/**
		 * @brief destroys the AsyncInputTask.
		 */
		~AsyncInputTask();

		/**
		 * @brief Performs the read.
		 * @return The return value of the thread.
		 */
		I32 run();

		/**
		 * @brief Destroyes the AsyncInputTask after it's finished running.
		 */
		void destroy();

		/**
		 * @brief debugging info.
		 */
		char* getInfo() const;

		/**
		 * @brief Method called when the input call is complete.
		 */
		void onCompletion();

	  private:
		AsyncInputType		type_;
		InputStream* 		stream_;
		VPtr					buffer_;
		Size					arg1_;
		Size					arg2_;
		Size					bytesRead_;

	};

	class AsyncReadResult : public AsyncResult {
	  public:
		/**
		 * @brief Constructs an AsyncResult for an AsyncInputTask.
		 * @param task The AsyncInputTask the result belongs to.
		 * @param buffer The buffer the data will be stored in.
		 */
		AsyncReadResult(AsyncInputTask* task, VPtr buffer) 
			: AsyncResult(reinterpret_cast<AsyncTask*>(task)), m_pBuffer(buffer), m_bytesRead(0) {}
		

		~AsyncReadResult() { setTask(NIL); }
		
		/**
		 * @brief Get the buffer that the data was read into.
		 * @return The buffer the data was read into.
		 */
		VPtr getResult() { return m_pBuffer; }
		
		/**
		 * @brief Called when the task is completed and stores the amount read.
		 * @param bytesRead The total bytes read.
		 */
		void taskCompleted(Size bytesRead);

		/**
		 * @brief Get the number of bytes read.
		 * @return The number of bytes read.
		 */
		inline Size getBytesRead() const { return m_bytesRead; }
		
	  private:
		VPtr				   m_pBuffer;
		Size					m_bytesRead;
	};

} // namespace Cat

#endif // CAT_CORE_IO_ASYNCINPUTTASK_H

