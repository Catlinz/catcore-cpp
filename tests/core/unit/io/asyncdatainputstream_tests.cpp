#include "core/testcore.h"
#include "core/io/asyncdatainputstream.h"
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
	void testAsyncDataInputStreamBasicCreateAndDestroy() {
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

		AsyncDataInputStream* as = new AsyncDataInputStream(s);
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

		as = new AsyncDataInputStream(s);
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

		getAbsPath(path, NIL);
	
		FINISH_TEST;
	}	

	void testAsyncDataInputStreamClose() {
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

		AsyncDataInputStream* as = new AsyncDataInputStream(s);
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

	void testAsyncDataInputStreamBasicReadFunctions() {
		BEGIN_TEST;

		IOManager::initializeIOManagerInstance();

		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestBasic.txt");
		AsyncDataInputStream* as = new AsyncDataInputStream(in);

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

	void testAsyncDataInputStreamDataTypeSpecificReadFunctions() {
		BEGIN_TEST;

		IOManager::initializeIOManagerInstance();

				
		U32 u32Test = 32;
		U32 u32TestA[] = {132, 232, 332, 432};
		U64 u64Test = 64;
		U64 u64TestA[] = {164, 264, 364, 464};

		I32 i32Test = -32;
		I32 i32TestA[] = {-132, 232, -332, 432};
		I64 i64Test = -64;
		I64 i64TestA[] = {-164, 264, -364, 464};

		F32 f32Test = 1.32;
		F32 f32TestA[] = {1.32, 2.32, 3.32, 4.32};
		F64 f64Test = 1.64;
		F64 f64TestA[] = {1.64, 2.64, 3.64, 4.64};

		Boolean boolTest = true;
		Boolean boolTestA[] = {true, false, true, true };

		Char charTest = 'c';
		Char charTestA[] = {'a', 'b', 'c', 'd'};

		CString cstrTest = (Char*)malloc(sizeof(Char)*64);
		Char str[] = "Hello World!";
		memcpy(cstrTest, str, sizeof(Char)*(strlen(str)+1));

		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestData.txt");
		AsyncDataInputStream* as = new AsyncDataInputStream(in);

		
		U32 ru32Test = 0;
		U32 ru32TestA[] = {0, 0, 0, 0};
		U64 ru64Test = 0;
		U64 ru64TestA[] = {0, 0, 0, 0};

		I32 ri32Test = 0;
		I32 ri32TestA[] = {0, 0, 0, 0};
		I64 ri64Test = 0;
		I64 ri64TestA[] = {0, 0, 0, 0};

		F32 rf32Test = 0.0f;
		F32 rf32TestA[] = {0.0f, 0.0f, 0.0f, 0.0f};
		F64 rf64Test = 0.0;
		F64 rf64TestA[] = {0, 0, 0, 0};

		Boolean rboolTest = false;
		Boolean rboolTestA[] = {false, false, false, false };

		Char rcharTest = 0;
		Char rcharTestA[] = {0, 0, 0, 0};

		CString rcstrTest = (Char*)malloc(sizeof(Char)*64);
		memset(rcstrTest, 0, sizeof(Char)*64);

		AsyncReadResult* result = as->readU32(&ru32Test, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(U32) );
		assert( !result->getTask() );
		delete result;
		result = as->readU32(ru32TestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(U32)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readU64(&ru64Test, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(U64) );
		assert( !result->getTask() );
		delete result;
		result = as->readU64(ru64TestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(U64)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readI32(&ri32Test, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(I32) );
		assert( !result->getTask() );
		delete result;
		result = as->readI32(ri32TestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(I32)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readI64(&ri64Test, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(I64) );
		assert( !result->getTask() );
		delete result;
		result = as->readI64(ri64TestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(I64)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readF32(&rf32Test, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(F32) );
		assert( !result->getTask() );
		delete result;
		result = as->readF32(rf32TestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(F32)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readF64(&rf64Test, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(F64) );
		assert( !result->getTask() );
		delete result;
		result = as->readF64(rf64TestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(F64)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readBoolean(&rboolTest, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(Boolean) );
		assert( !result->getTask() );
		delete result;
		result = as->readBoolean(rboolTestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(Boolean)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readChar(&rcharTest, 1);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(Char) );
		assert( !result->getTask() );
		delete result;
		result = as->readChar(rcharTestA, 4);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == sizeof(Char)*4 );
		assert( !result->getTask() );
		delete result;

		result = as->readCStr(rcstrTest);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == (sizeof(U32) + sizeof(Char)*12) );
		assert( !result->getTask() );
		delete result;

		delete as;
		delete in;

		assert( ru32Test == u32Test );
		assert( ru32TestA[0] == u32TestA[0] && ru32TestA[1] == u32TestA[1] && ru32TestA[2] == u32TestA[2] && ru32TestA[3] == u32TestA[3] );
		assert( ru64Test == u64Test );
		assert( ru64TestA[0] == u64TestA[0] && ru64TestA[1] == u64TestA[1] && ru64TestA[2] == u64TestA[2] && ru64TestA[3] == u64TestA[3] );

		assert( ri32Test == i32Test );
		assert( ri32TestA[0] == i32TestA[0] && ri32TestA[1] == i32TestA[1] && ri32TestA[2] == i32TestA[2] && ri32TestA[3] == i32TestA[3] );
		assert( ri64Test == i64Test );
		assert( ri64TestA[0] == i64TestA[0] && ri64TestA[1] == i64TestA[1] && ri64TestA[2] == i64TestA[2] && ri64TestA[3] == i64TestA[3] );

		assert( rf32Test == f32Test );
		assert( rf32TestA[0] == f32TestA[0] && rf32TestA[1] == f32TestA[1] && rf32TestA[2] == f32TestA[2] && rf32TestA[3] == f32TestA[3] );
		assert( rf64Test == f64Test );
		assert( rf64TestA[0] == f64TestA[0] && rf64TestA[1] == f64TestA[1] && rf64TestA[2] == f64TestA[2] && rf64TestA[3] == f64TestA[3] );

		assert( rboolTest == boolTest );
		assert( rboolTestA[0] == boolTestA[0] && rboolTestA[1] == boolTestA[1] && rboolTestA[2] == boolTestA[2] && rboolTestA[3] == boolTestA[3] );

		assert( rcharTest == charTest );
		assert( rcharTestA[0] == charTestA[0] && rcharTestA[1] == charTestA[1] && rcharTestA[2] == charTestA[2] && rcharTestA[3] == charTestA[3] );

		assert( strcmp(rcstrTest, cstrTest) == 0 );

		free(cstrTest);
		free(rcstrTest);

		IOManager::destroyIOManagerInstance();

	
		FINISH_TEST;
	}	

}

int main(int argc, char** argv) {
	cc::testAsyncDataInputStreamBasicCreateAndDestroy();
	cc::testAsyncDataInputStreamClose();
	cc::testAsyncDataInputStreamBasicReadFunctions();
	cc::testAsyncDataInputStreamDataTypeSpecificReadFunctions();
	return 0;
}


