#include "core/testcore.h"
#include "core/io/fileoutputstream.h"
#include "core/io/fileinputstream.h"
#include "core/io/file.h"
#include "core/io/serializable.h"

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


	void testFileOutputStreamBasicCreateAndDestroy() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		// Test default constructor
		FileOutputStream* s = new FileOutputStream();
		assert( !s->getFD()->getFileHandle() );
		assert( !s->getFD()->getFile().notNull() );
		assert( !s->getFD()->isValid() );
		assert( !s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		assert( !s->canWrite() );
		delete s;

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


		delete s;

		// Test File* constructor
		FilePtr file = FilePtr(new File("test_files/test_files_2/writeOnly.txt"));
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canWrite() );

		s = new FileOutputStream(file);
		assert( s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( s->getFD()->isOpen() );
		assert( s->getFD()->getFile()->retainCount() == 2 );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( s->canWrite() );


		delete s;
		assert( file->retainCount() == 1 );
		file.setNull();		

		// Test FileDescriptor* constructor.
		FileDescriptor* desc = new FileDescriptor("testFile1.txt");
		assert( !desc->getFileHandle() );
		assert( desc->getFile().notNull() );
		assert( desc->getFile()->retainCount() == 1 );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(desc->getFile()->absolutePath(), path) == 0 );
		assert( desc->getName() );
		assert( strcmp(desc->getName(), path) == 0 );
		assert( desc->isValid() );
		assert( !desc->isOpen() );
		assert( desc->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc->isStreamType(STREAM_TYPE_SOCKET) );

		s = new FileOutputStream(desc);
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
		assert( desc->isOpen() );
		assert( desc->getFile()->retainCount() == 1 );
		assert( strcmp(desc->getFile()->absolutePath(), path) == 0 );
		delete desc;

		desc = new FileDescriptor("test_files/test_files_2/readOnly.txt");
		assert( !desc->getFileHandle() );
		assert( desc->getFile().notNull() );
		assert( desc->getFile()->retainCount() == 1 );
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(desc->getFile()->absolutePath(), path) == 0 );
		assert( desc->getName() );
		assert( strcmp(desc->getName(), path) == 0 );
		assert( desc->isValid() );
		assert( !desc->isOpen() );
		assert( desc->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc->isStreamType(STREAM_TYPE_SOCKET) );



		s = new FileOutputStream(desc);
		assert( !s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( !s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( !s->canWrite() );

		delete s;

		desc->open();
		assert( desc->isOpen() );
		assert( desc->getFile()->canRead() );

		s = new FileOutputStream(desc);
		assert( !s->canWrite() );
		delete s;
		delete desc;

		s = new FileOutputStream("test_files/test_files_2/readOnly.txt");
		assert( !s->canWrite() );
		delete s;

	
		FINISH_TEST;
	}	

	void testFileOutputStreamCopyAndAssignment() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		// ##########
		// ########## TEST COPY CONSTRUCTOR ################ //
		// ##########

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


		FileOutputStream* s2 = new FileOutputStream(*s);
		assert( s2->getFD()->getFileHandle() );
		assert( s2->getFD()->getFile().notNull() );
		assert( s2->getFD()->isValid() );
		assert( s2->getFD()->isOpen() );
		assert( s2->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s2->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s2->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s2->getFD()->getName(), path) == 0 );
		assert( s2->canWrite() );


		FileOutputStream* s3 = new FileOutputStream(*s);
		assert( s3->getFD()->getFileHandle() );
		assert( s3->getFD()->getFile().notNull() );
		assert( s3->getFD()->isValid() );
		assert( s3->getFD()->isOpen() );
		assert( s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s3->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s3->getFD()->getName(), path) == 0 );
		assert( s3->canWrite() );


		delete s;
		assert( s2->getFD()->isOpen() );
		assert( s2->canWrite() );
		assert( s3->getFD()->isOpen() );
		assert( s3->canWrite() );

		delete s3;
		assert( s2->getFD()->isOpen() );
		assert( s2->canWrite() );
		
		s = new FileOutputStream(*s2);
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

		assert( s2->getFD()->isOpen() );
		assert( s2->canWrite() );
		delete s;
		assert( s2->getFD()->isOpen() );
		assert( s2->canWrite() );
		delete s2;

		// ##########
		// ########## TEST ASSIGNMENT OPERATOR ################ //
		// ##########
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

		s2 = new FileOutputStream();
		assert( !s2->getFD()->getFileHandle() );
		assert( !s2->canWrite() );

		*s2 = *s;
		assert( s2->getFD()->getFileHandle() );
		assert( s2->getFD()->getFile().notNull() );
		assert( s2->getFD()->isValid() );
		assert( s2->getFD()->isOpen() );
		assert( s2->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s2->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s2->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s2->getFD()->getName(), path) == 0 );
		assert( s2->canWrite() );

		FileDescriptor* desc = new FileDescriptor("test_files/test_files_2/writeOnly.txt");
		assert( !desc->getFileHandle() );
		assert( desc->getFile().notNull() );
		assert( desc->getFile()->retainCount() == 1 );

		s3 = new FileOutputStream(desc);
		assert( s3->getFD()->getFileHandle() );
		assert( s3->getFD()->getFile().notNull() );
		assert( s3->getFD()->isValid() );
		assert( s3->getFD()->isOpen() );
		assert( s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(s3->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s3->getFD()->getName(), path) == 0 );
		assert( s3->canWrite() );

		assert( desc->getFile()->retainCount() == 2 );

		*s3 = *s;

		assert( desc->getFile()->retainCount() == 1 );

		assert( s3->getFD()->getFileHandle() );
		assert( s3->getFD()->getFile().notNull() );
		assert( s3->getFD()->isValid() );
		assert( s3->getFD()->isOpen() );
		assert( s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s3->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s3->getFD()->getName(), path) == 0 );
		assert( s3->canWrite() && s2->canWrite() && s->canWrite());

		delete desc;
		delete s;
		delete s3;
		delete s2;
				
		FINISH_TEST;
	}	


	void testFileOutputStreamBasicWriteFunctions() {
		BEGIN_TEST;

		File* file = new File("test_files/test_files_2/writeTestBasic.txt");
		file->remove();
		assert( file->create() );
		delete file;

		FileOutputStream* s = new FileOutputStream("test_files/test_files_2/writeTestBasic.txt");
		assert( s->canWrite() );
		
		U32 test1 = 1234;
		U32 test2[] = {1, 2, 3, 4};

		assert ( s->write(&test1, sizeof(U32)) == sizeof(U32) );
		assert ( s->write(test2, 4, sizeof(U32)) == sizeof(U32)*4 );

		s->close();

		delete s;

		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestBasic.txt");
		U32 rtest1 = 0;
		U32 rtest2[] = {0, 0, 0, 0 };

		assert (in->read(&rtest1, sizeof(U32)) == sizeof(U32) );
		assert (in->read(rtest2, 4, sizeof(U32)) == sizeof(U32)*4 );

		delete in;

		assert (rtest1 == test1);
		assert( rtest2[0] == test2[0] && rtest2[1] == test2[1] && rtest2[2] == test2[2] && rtest2[3] == test2[3] );

	
		FINISH_TEST;
	}	

	void testFileOutputStreamDataTypeSpecificWriteFunctions() {
		BEGIN_TEST;


		File* file = new File("test_files/test_files_2/writeTestData.txt");
		file->remove();
		assert( file->create() );
		delete file;

		FileOutputStream* s = new FileOutputStream("test_files/test_files_2/writeTestData.txt");
		assert( s->canWrite() );
		
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


		assert ( s->writeU32(&u32Test, 1) == sizeof(U32) );
		assert ( s->writeU32(u32TestA, 4) == sizeof(U32)*4 );
		assert ( s->writeU64(&u64Test, 1) == sizeof(U64) );
		assert ( s->writeU64(u64TestA, 4) == sizeof(U64)*4 );

		assert ( s->writeI32(&i32Test, 1) == sizeof(I32) );
		assert ( s->writeI32(i32TestA, 4) == sizeof(I32)*4 );
		assert ( s->writeI64(&i64Test, 1) == sizeof(I64) );
		assert ( s->writeI64(i64TestA, 4) == sizeof(I64)*4 );

		assert ( s->writeF32(&f32Test, 1) == sizeof(F32) );
		assert ( s->writeF32(f32TestA, 4) == sizeof(F32)*4 );
		assert ( s->writeF64(&f64Test, 1) == sizeof(F64) );
		assert ( s->writeF64(f64TestA, 4) == sizeof(F64)*4 );

		assert ( s->writeBoolean(&boolTest, 1) == sizeof(Boolean) );
		assert ( s->writeBoolean(boolTestA, 4) == sizeof(Boolean)*4 );

		assert ( s->writeChar(&charTest, 1) == sizeof(Char) );
		assert ( s->writeChar(charTestA, 4) == sizeof(Char)*4 );

		assert ( s->writeCStr(cstrTest) == (sizeof(U32) + sizeof(Char)*12) );

		s->close();

		delete s;

		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestData.txt");
		
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

		assert ( in->readU32(&ru32Test, 1) == sizeof(U32) );
		assert ( in->readU32(ru32TestA, 4) == sizeof(U32)*4 );
		assert ( in->readU64(&ru64Test, 1) == sizeof(U64) );
		assert ( in->readU64(ru64TestA, 4) == sizeof(U64)*4 );

		assert ( in->readI32(&ri32Test, 1) == sizeof(I32) );
		assert ( in->readI32(ri32TestA, 4) == sizeof(I32)*4 );
		assert ( in->readI64(&ri64Test, 1) == sizeof(I64) );
		assert ( in->readI64(ri64TestA, 4) == sizeof(I64)*4 );

		assert ( in->readF32(&rf32Test, 1) == sizeof(F32) );
		assert ( in->readF32(rf32TestA, 4) == sizeof(F32)*4 );
		assert ( in->readF64(&rf64Test, 1) == sizeof(F64) );
		assert ( in->readF64(rf64TestA, 4) == sizeof(F64)*4 );

		assert ( in->readBoolean(&rboolTest, 1) == sizeof(Boolean) );
		assert ( in->readBoolean(rboolTestA, 4) == sizeof(Boolean)*4 );

		assert ( in->readChar(&rcharTest, 1) == sizeof(Char) );
		assert ( in->readChar(rcharTestA, 4) == sizeof(Char)*4 );

		assert ( in->readCStr(rcstrTest) == (sizeof(U32) + sizeof(Char)*12) );


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

	
		FINISH_TEST;
	}	

	void testFileOutputStreamObjectWriteFunction() {
		BEGIN_TEST;

		File* file = new File("test_files/test_files_2/writeTestObject.txt");
		file->remove();
		assert( file->create() );
		delete file;

		FileOutputStream* s = new FileOutputStream("test_files/test_files_2/writeTestObject.txt");
		assert( s->canWrite() );
		
		TestObject object;
		object.set(1, 2, 3.4, "MeowMix");

		TestObject* object2 = new TestObject();
		object2->set(1.2, 2.2, 3.2, "MooerTest");
		
		assert ( s->writeObject(&object) == sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(7) );
		assert ( s->writeObject(object2) == sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(9) );

		s->close();

		delete s;

		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestObject.txt");
	

		TestObject robject;
		TestObject* robject2 = new TestObject();

		assert ( in->readObject(&robject) == sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(7) );
		assert ( in->readObject(robject2) == sizeof(F64)*3 + sizeof(U32) + sizeof(Char)*(9) );

		delete in;

		assert( robject.testValues(1, 2, 3.4, "MeowMix") );
		assert( robject2->testValues(1.2, 2.2, 3.2, "MooerTest") );

		FINISH_TEST;
	}	



}

int main(int argc, char** argv) {
	cc::testFileOutputStreamBasicCreateAndDestroy();
	cc::testFileOutputStreamCopyAndAssignment();
	cc::testFileOutputStreamBasicWriteFunctions();
	cc::testFileOutputStreamDataTypeSpecificWriteFunctions();
	cc::testFileOutputStreamObjectWriteFunction();
	return 0;
}


