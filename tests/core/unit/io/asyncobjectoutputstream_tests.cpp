#include "core/testcore.h"
#include "core/io/asyncobjectoutputstream.h"
#include "core/io/asyncobjectinputstream.h"
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
	
	class TestObject : public Serializable {
		public:
			TestObject() {
				x_ = y_ = z_ = 0.0;
				name_ = NIL;
			}
			
			~TestObject() {
				if (name_) {
					free(name_);
					name_ = NIL;
				}
			}

			void set(F64 x, F64 y, F64 z, const Char* name) {
				x_ = x;
				y_ = y;
				z_ = z;

				name_ = (Char*)malloc(sizeof(Char)*(strlen(name)+1));
				memcpy(name_, name, sizeof(Char)*(strlen(name)+1));
			}

			Boolean testValues(F64 x, F64 y, F64 z, const Char* name) {
				return x == x_ && y == y_ && z == z_ && (strcmp(name_, name) == 0);
			}

			Size read(InputStream* input) {
				DataInputStream* stream = (DataInputStream*)input;
				Size amountRead = 0;
				amountRead += stream->readF64(&x_, 1);
				amountRead += stream->readF64(&y_, 1);
				amountRead += stream->readF64(&z_, 1);

				name_ = (Char*)malloc(sizeof(Char)*64);

				amountRead += stream->readCStr(name_);
				return amountRead;
			}
			Size write(OutputStream* output) {
				DataOutputStream* stream = (DataOutputStream*)output;
				Size written = 0;
				written += stream->writeF64(&x_, 1);
				written += stream->writeF64(&y_, 1);
				written += stream->writeF64(&z_, 1);
				written += stream->writeCStr(name_);
				return written;
			}

		

			F64 x_;
			F64 y_;
			F64 z_;
			Char* name_;
	};



	void testAsyncObjectOutputStreamBasicCreateAndDestroy() {
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

		AsyncObjectOutputStream* as = new AsyncObjectOutputStream(s);
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

		as = new AsyncObjectOutputStream(s);
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

	void testAsyncObjectOutputStreamClose() {
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

		AsyncObjectOutputStream* as = new AsyncObjectOutputStream(s);
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

	void testAsyncObjectOutputStreamBasicReadFunctions() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		IOManager::initializeIOManagerInstance();

		File* file = new File("test_files/test_files_2/writeTestBasic.txt");
		file->remove();
		assert( file->create() );
		delete file;

		FileOutputStream* s = new FileOutputStream("test_files/test_files_2/writeTestBasic.txt");
		assert( s->canWrite() );
		AsyncObjectOutputStream* ao = new AsyncObjectOutputStream(s);
		
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
		AsyncObjectInputStream* as = new AsyncObjectInputStream(in);

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

	void testAsyncObjectOutputStreamDataTypeSpecificReadFunctions() {
		BEGIN_TEST;

		IOManager::initializeIOManagerInstance();

		File* file = new File("test_files/test_files_2/writeTestData.txt");
		file->remove();
		assert( file->create() );
		delete file;

		FileOutputStream* s = new FileOutputStream("test_files/test_files_2/writeTestData.txt");
		assert( s->canWrite() );
		AsyncObjectOutputStream* ao = new AsyncObjectOutputStream(s);

				
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

		AsyncWriteResult* wresult = ao->writeU32(&u32Test, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(U32) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeU32(u32TestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(U32)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeU64(&u64Test, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(U64) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeU64(u64TestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(U64)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeI32(&i32Test, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(I32) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeI32(i32TestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(I32)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeI64(&i64Test, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(I64) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeI64(i64TestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(I64)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeF32(&f32Test, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(F32) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeF32(f32TestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(F32)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeF64(&f64Test, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(F64) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeF64(f64TestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(F64)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeBoolean(&boolTest, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(Boolean) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeBoolean(boolTestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(Boolean)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeChar(&charTest, 1);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(Char) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		wresult = ao->writeChar(charTestA, 4);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == sizeof(Char)*4 );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		wresult = ao->writeCStr(cstrTest);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == (sizeof(U32) + sizeof(Char)*12) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;

		ao->close();
		delete ao;
		delete s;


		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestData.txt");
		AsyncObjectInputStream* as = new AsyncObjectInputStream(in);

		
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

	void testAsyncObjectOutputStreamObjectReadFunction() {
		BEGIN_TEST;

		IOManager::initializeIOManagerInstance();

		File* file = new File("test_files/test_files_2/writeTestObject.txt");
		file->remove();
		assert( file->create() );
		delete file;

		FileOutputStream* s = new FileOutputStream("test_files/test_files_2/writeTestObject.txt");
		assert( s->canWrite() );
		AsyncObjectOutputStream* ao = new AsyncObjectOutputStream(s);
		
		TestObject object;
		object.set(1, 2, 3.4, "MeowMix");

		TestObject* object2 = new TestObject();
		object2->set(1.2, 2.2, 3.2, "MooerTest");

		AsyncWriteResult* wresult = ao->writeObject(&object);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == (sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(7)) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;
		
		wresult = ao->writeObject(object2);
		assert( wresult );
		assert( wresult->waitForResult() );
		assert( wresult->getBytesWritten() == (sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(9)) );
		assert( !wresult->getResult() );
		assert( !wresult->getTask() );
		delete wresult;


		ao->close();

		delete ao;
		delete s;


		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestObject.txt");
		AsyncObjectInputStream* as = new AsyncObjectInputStream(in);

		TestObject robject;
		TestObject* robject2 = new TestObject();

		AsyncReadResult* result = as->readObject(&robject);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == (sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(7)) );
		assert( !result->getTask() );
		delete result;

		result = as->readObject(robject2);
		assert( result );
		assert( result->waitForResult() );
		assert( result->getBytesRead() == (sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(9)) );
		assert( !result->getTask() );
		delete result;

		delete as;
		delete in;

		assert( robject.testValues(1, 2, 3.4, "MeowMix") );
		assert( robject2->testValues(1.2, 2.2, 3.2, "MooerTest") );

		IOManager::destroyIOManagerInstance();


		FINISH_TEST;
	}	


}

int main(int argc, char** argv) {
	cc::testAsyncObjectOutputStreamBasicCreateAndDestroy();
	cc::testAsyncObjectOutputStreamClose();
	cc::testAsyncObjectOutputStreamBasicReadFunctions();
	cc::testAsyncObjectOutputStreamDataTypeSpecificReadFunctions();
	cc::testAsyncObjectOutputStreamObjectReadFunction();
	return 0;
}





