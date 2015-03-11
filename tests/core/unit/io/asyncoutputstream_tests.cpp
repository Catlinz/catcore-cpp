#include "core/testcore.h"
#include "core/io/asyncoutputstream.h"
#include "core/io/asyncinputstream.h"
#include "core/io/fileinputstream.h"
#include "core/io/fileoutputstream.h"
#include "core/io/file.h"
#include "core/io/serializable.h"
#include "core/io/iomanager.h"
#include "core/io/asyncoutputtask.h"
#include "core/io/asyncinputtask.h"

#define ABS_PATH_BUFFER_LEN 1024

void getAbsPath(char* dest, const char* name) {
	getcwd(dest, ABS_PATH_BUFFER_LEN);
	if (name) {
		strcat(dest, name);
	} else  {
		size_t len = strlen(dest);
		if (dest[len-1] != '/');
		dest[len] = '/';
		dest[len+1] = '\0';
	}	
}

namespace cc {
	void testAsyncOutputStreamBasicCreateAndDestroy() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		IOManager::initializeIOManagerInstance();

		// Test default constructor
		FileOutputStream* s = new FileOutputStream();
		assert( !s->getFD()->getFileHandle() );
		assert( !s->getFD()->getFile().notNull() );
		assert( !s->getFD()->isValid() );
		assert( !s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		assert( !s->canWrite() );

		AsyncOutputStream* as = new AsyncOutputStream(s);
		assert( !as->canWrite() );
		assert( as->getOutputStream() );

		delete s;
		delete as;

		// Test filename constructor
		s = new FileOutputStream("testFile1.txt");
		assert( s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( s->canWrite() );

		as = new AsyncOutputStream(s);
		assert( as->canWrite() );
		assert( as->getOutputStream() );
		assert( strcmp(((FileOutputStream*)as->getOutputStream())->getFD()->getFile()->absolutePath(), path) == 0 );

		delete as;

		assert( s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( s->canWrite() );

		delete s;

		IOManager::destroyIOManagerInstance();

	
		FINISH_TEST;
	}	

	void testAsyncOutputStreamClose() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		IOManager::initializeIOManagerInstance();


		// Test default constructor
		FileOutputStream* s = new FileOutputStream("testFile1.txt");
		assert( s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( s->canWrite() );

		AsyncOutputStream* as = new AsyncOutputStream(s);
		assert( as->canWrite() );
		assert( as->getOutputStream() );
		assert( strcmp(((FileOutputStream*)as->getOutputStream())->getFD()->getFile()->absolutePath(), path) == 0 );

		as->close();
		assert( !as->canWrite() );
		assert( !s->getFD()->isOpen() );

		delete s;
		delete as;

		IOManager::destroyIOManagerInstance();

		FINISH_TEST;


	}

	void testAsyncOutputStreamBasicReadFunctions() {
		BEGIN_TEST;

		IOManager::initializeIOManagerInstance();

		File* file = new File("test_files/test_files_2/writeTestBasic.txt");
		file->remove();
		assert( file->create() );
		delete file;

		FileOutputStream* s = new FileOutputStream("test_files/test_files_2/writeTestBasic.txt");
		assert( s->canWrite() );
		AsyncOutputStream* ao = new AsyncOutputStream(s);
		
		U32 test1 = 1234;
		U32 test2[] = {1, 2, 3, 4};

		AsyncWriteResult* wresult = ao->write(&test1, sizeof(U32));
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(U32) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->write(test2, 4, sizeof(U32));
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(U32)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		ao->close();

		delete ao;
		delete s;


		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestBasic.txt");
		AsyncInputStream* as = new AsyncInputStream(in);

		U32 rtest1 = 0;
		U32 rtest2[] = {0, 0, 0, 0 };

		AsyncReadResult* result = as->read(&rtest1, sizeof(U32));
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(U32) );

		assert( !result->getTask() );
		delete result;

		result = as->read(rtest2, 4, sizeof(U32));
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(U32)*4 );

		assert( !result->getTask() );
		delete result;

		delete as;
		delete in;

		assert (rtest1 == 1234);
		assert( rtest2[0] == 1 && rtest2[1] == 2 && rtest2[2] == 3 && rtest2[3] == 4 );

		IOManager::destroyIOManagerInstance();

	
		FINISH_TEST;
	}	




}

int main(int argc, char** argv) {
	cc::testAsyncOutputStreamBasicCreateAndDestroy();
	cc::testAsyncOutputStreamClose();
	cc::testAsyncOutputStreamBasicReadFunctions();
	return 0;
}



