#ifndef CAT_CORE_IO_ASYNCOUTPUTTASK_H
#define CAT_CORE_IO_ASYNCOUTPUTTASK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file asyncoutputtask.h
 * @brief Definition of the AsyncOutputTask to run Async Output calls.
 *
 * @author: Catlin Zilinski
 * @date: Oct 18, 2013
 */

#include "core/threading/asynctask.h"
#include "core/threading/asyncresult.h"

namespace Cat {

	enum AsyncOutputType {
		ASYNC_WRITE_1, ASYNC_WRITE_2, 
		ASYNC_WRITE_U32, ASYNC_WRITE_U64, 
		ASYNC_WRITE_I32, ASYNC_WRITE_I64,
		ASYNC_WRITE_F32, ASYNC_WRITE_F64,
		ASYNC_WRITE_BOOLEAN,
		ASYNC_WRITE_CHAR, 
		ASYNC_WRITE_CSTR,
		ASYNC_WRITE_OBJECT
	};

	class OutputStream;
	class AsyncWriteResult;

	/**
	 * @class AsyncOutputTask asyncoutputtask.h "core/io/asyncoutputtask.h"
	 * @brief The AsyncTask responsible for completing the async writes.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Oct 18, 2013
	 */
	class AsyncOutputTask : public AsyncTask {
	  public:
		/**
		 * @brief Creates a new AsyncOutputTask of the specified type.
		 * @param outputType The type of output operation to perform.
		 * @param outputStream The OutputStream to write from.
		 * @param buffer The buffer to write the data from.
		 * @param arg1 The first argument to the write call.
		 * @param arg2 The (optional) second argument to the write call.
		 */
		AsyncOutputTask(AsyncOutputType outputType, OutputStream* outputStream, VPtr buffer, Size arg1 = 0, Size arg2 = 0);

		/**
		 * @brief destroys the AsyncOutputTask.
		 */
		~AsyncOutputTask();

		/**
		 * @brief Performs the write.
		 * @return The return value of the thread.
		 */
		I32 run();

		/**
		 * @brief Destroyes the AsyncOutputTask after it's finished running.
		 */
		void destroy();

		/**
		 * @brief debugging info.
		 */
		char* getInfo() const;

		/**
		 * @brief Method called when the output call is complete.
		 */
		void onCompletion();

	  private:
		AsyncOutputType		type_;
		OutputStream* 		stream_;
		VPtr					buffer_;
		Size					arg1_;
		Size					arg2_;
		Size					bytesWritten_;
	};

	class AsyncWriteResult : public AsyncResult {
	  public:
		/**
		 * @brief Constructs an AsyncResult for an AsyncOutputTask.
		 * @param task The AsyncOutputTask the result belongs to.
		 */
		AsyncWriteResult(AsyncOutputTask* task) 
			: AsyncResult(reinterpret_cast<AsyncTask*>(task)) {}
		

		~AsyncWriteResult() { setTask(NIL); }
	
		/**
		 * @brief Always returns NIL.
		 * @return NIL
		 */
		VPtr getResult() { return NIL; }
		
		/**
		 * @brief Called when the task is completed and stores the amount written.
		 * @param bytesWritten The total bytes written.
		 */
		void taskCompleted(Size bytesWritten);

		/**
		 * @brief Get the number of bytes written.
		 * @return The number of bytes written.
		 */
		inline Size getBytesWritten() const { return m_bytesWritten; }
		
	  private:
		Size					m_bytesWritten;
	};

} // namespace Cat

#endif // CAT_CORE_IO_ASYNCOUTPUTTASK_H


