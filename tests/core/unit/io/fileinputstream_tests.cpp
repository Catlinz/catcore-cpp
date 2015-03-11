#include "core/testcore.h"
#include "core/io/fileinputstream.h"
#include "core/io/fileoutputstream.h"
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


	void testFileInputStreamBasicCreateAndDestroy() {
		BEGIN_TEST;

char path[ABS_PATH_BUFFER_LEN];

		// Test default constructor
		FileInputStream* s = new FileInputStream();
		assert( !s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().isNull() );
		assert( !s->getFD()->isValid() );
		assert( !s->getFD()->isOpen() );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		assert( !s->canRead() );

		delete s;

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

		delete s;

		// Test FilePtr constructor
		FilePtr file = FilePtr(new File("test_files/test_files_2/readOnly.txt"));
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );

		s = new FileInputStream(file);
		assert( s->getFD()->getFileHandle() );
		assert( s->getFD()->getFile().notNull() );
		assert( s->getFD()->isValid() );
		assert( s->getFD()->isOpen() );
		assert( s->getFD()->getFile()->retainCount() == 2 );
		assert( s->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		assert( strcmp(s->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s->getFD()->getName(), path) == 0 );
		assert( s->canRead() );

		delete s;
		assert( file->retainCount() == 1 );

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

		s = new FileInputStream(desc);
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
		assert( desc->isOpen() );
		assert( desc->getFile()->retainCount() == 1 );
		assert( strcmp(desc->getFile()->absolutePath(), path) == 0 );
		delete desc;

		s = new FileInputStream("fileNotThere.txt");
		assert( !s->canRead() );

	
		FINISH_TEST;
	}	

	void testFileInputStreamCopyAndAssignment() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		// ##########
		// ########## TEST COPY CONSTRUCTOR ################ //
		// ##########

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

		FileInputStream* s2 = new FileInputStream(*s);
		assert( s2->getFD()->getFileHandle() );
		assert( s2->getFD()->getFile().notNull() );
		assert( s2->getFD()->isValid() );
		assert( s2->getFD()->isOpen() );
		assert( s2->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s2->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s2->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s2->getFD()->getName(), path) == 0 );
		assert( s2->canRead() );

		FileInputStream* s3 = new FileInputStream(*s);
		assert( s3->getFD()->getFileHandle() );
		assert( s3->getFD()->getFile().notNull() );
		assert( s3->getFD()->isValid() );
		assert( s3->getFD()->isOpen() );
		assert( s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(s3->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s3->getFD()->getName(), path) == 0 );
		assert( s3->canRead() );

		assert( s2->getFD()->isOpen() );
		assert( s2->canRead() );
		assert( s3->getFD()->isOpen() );
		assert( s3->canRead() );
		
		delete s;
		assert( s2->getFD()->getFile().notNull());		
		assert( s2->getFD()->isOpen() );
		assert( s2->canRead() );
		assert( s3->getFD()->isOpen() );
		assert( s3->canRead() );

		delete s3;
		assert( s2->getFD()->isOpen() );
		assert( s2->canRead() );
		
		s = new FileInputStream(*s2);
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

		assert( s2->getFD()->isOpen() );
		assert( s2->canRead() );
		delete s;
		assert( s2->getFD()->isOpen() );
		assert( s2->canRead() );
		delete s2;

		// ##########
		// ########## TEST ASSIGNMENT OPERATOR ################ //
		// ##########
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

		s2 = new FileInputStream();
		assert( !s2->getFD()->getFileHandle() );
		assert( !s2->canRead() );

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
		assert( s2->canRead() );

		FileDescriptor* desc = new FileDescriptor("test_files/test_files_2/readOnly.txt");
		assert( !desc->getFileHandle() );
		assert( desc->getFile().notNull() );
		assert( desc->getFile()->retainCount() == 1 );

		s3 = new FileInputStream(desc);
		assert( s3->getFD()->getFileHandle() );
		assert( s3->getFD()->getFile().notNull() );
		assert( s3->getFD()->isValid() );
		assert( s3->getFD()->isOpen() );
		assert( s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_FILE) );
		assert( !s3->getStreamDescriptor()->isStreamType(STREAM_TYPE_SOCKET) );
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(s3->getFD()->getFile()->absolutePath(), path) == 0 );
		assert( strcmp(s3->getFD()->getName(), path) == 0 );
		assert( s3->canRead() );

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
		assert( s3->canRead() && s2->canRead() && s->canRead());

		delete desc;

		delete s;

		delete s3;
		delete s2;
				
		FINISH_TEST;
	}	

	void testFileInputStreamBasicReadFunctions() {
		BEGIN_TEST;

		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestBasic.txt");
		U32 rtest1 = 0;
		U32 rtest2[] = {0, 0, 0, 0 };

		assert (in->read(&rtest1, sizeof(U32)) == sizeof(U32) );
		assert (in->read(rtest2, 4, sizeof(U32)) == sizeof(U32)*4 );

		delete in;

		assert (rtest1 == 1234);
		assert( rtest2[0] == 1 && rtest2[1] == 2 && rtest2[2] == 3 && rtest2[3] == 4 );

	
		FINISH_TEST;
	}	

	void testFileInputStreamDataTypeSpecificReadFunctions() {
		BEGIN_TEST;

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
	
	void testFileInputStreamObjectReadFunction() {
		BEGIN_TEST;

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

	void testFileInputStreamCanReadEOF() {
		BEGIN_TEST;

		FileInputStream* in = new FileInputStream("test_files/test_files_2/writeTestBasic.txt");
		U32 rtest1 = 0;
		U32 rtest2[] = {0, 0, 0, 0 };
		U32 test2 = 0;

		assert ( in->read(&rtest1, sizeof(U32)) == sizeof(U32) );
		assert ( in->read(rtest2, 4, sizeof(U32)) == sizeof(U32)*4 );

		assert( in->canRead() );
		assert ( !in->read(&test2, sizeof(U32)) );
		assert( !in->canRead() );

		


		delete in;

		assert (rtest1 == 1234);
		assert( rtest2[0] == 1 && rtest2[1] == 2 && rtest2[2] == 3 && rtest2[3] == 4 );

	
		FINISH_TEST;
	}	




}

int main(int argc, char** argv) {
	cc::testFileInputStreamBasicCreateAndDestroy();
	cc::testFileInputStreamCopyAndAssignment();
	cc::testFileInputStreamBasicReadFunctions();
	cc::testFileInputStreamDataTypeSpecificReadFunctions();
	cc::testFileInputStreamObjectReadFunction();
	cc::testFileInputStreamCanReadEOF();
	return 0;
}

