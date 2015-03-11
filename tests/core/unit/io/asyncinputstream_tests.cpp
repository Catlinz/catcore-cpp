#include "core/testcore.h"
#include "core/io/asyncinputstream.h"
#include "core/io/fileinputstream.h"
#include "core/io/fileoutputstream.h"
#include "core/io/file.h"
#include "core/io/serializable.h"
#include "core/io/iomanager.h"
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
	void testAsyncInputStreamBasicCreateAndDestroy() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		IOManager::initializeIOManagerInstance();

		// Test default constructor
		FileInputStream* s = new FileInputStream();
		assert( !s->getFD()->getFileHandle() );
		assert( !s->getFD()->getFile().notNull() );
		assert( !s->getFD()->isValid() );
		assert( !s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		assert( !s->canRead() );

		AsyncInputStream* as = new AsyncInputStream(s);
		assert( !as->canRead() );
		assert( as->getInputStream() );

		delete s;
		delete as;

		// Test filename constructor
		s = new FileInputStream("testFile1.txt");
		assert( s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( s->canRead() );

		as = new AsyncInputStream(s);
		assert( as->canRead() );
		assert( as->getInputStream() );
		assert( strcmp(((FileInputStream*)as->getInputStream())->getFD()->getFile()->absolutePath(), path) == 0 );

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
		assert( s->canRead() );

		delete s;

		IOManager::destroyIOManagerInstance();

	
		FINISH_TEST;
	}	

	void testAsyncInputStreamClose() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		IOManager::initializeIOManagerInstance();


		// Test default constructor
		FileInputStream* s = new FileInputStream("testFile1.txt");
		assert( s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( s->canRead() );

		AsyncInputStream* as = new AsyncInputStream(s);
		assert( as->canRead() );
		assert( as->getInputStream() );
		assert( strcmp(((FileInputStream*)as->getInputStream())->getFD()->getFile()->absolutePath(), path) == 0 );

		as->close();
		assert( !as->canRead() );
		assert( !s->getFD()->isOpen() );

		delete s;
		delete as;

		IOManager::destroyIOManagerInstance();

		FINISH_TEST;


	}

	void testAsyncInputStreamBasicReadFunctions() {
		BEGIN_TEST;

		IOManager::initializeIOManagerInstance();

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
		std::cout << result->getBytesRead() << std::endl;
		
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
	cc::testAsyncInputStreamBasicCreateAndDestroy();
	cc::testAsyncInputStreamClose();
	cc::testAsyncInputStreamBasicReadFunctions();
	return 0;
}


