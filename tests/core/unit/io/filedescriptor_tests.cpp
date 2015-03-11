#include "core/testcore.h"
#include "core/io/filedescriptor.h"

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
	void testFileDescriptorBasicCreateAndDestroy() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		DMSG("Create empty FileDescriptor.");		
		FileDescriptor* desc = new FileDescriptor();
		assert( !desc->getFileHandle() );
		assert( desc->getFile().isNull() );
		assert( !desc->getName() );
		assert( !desc->isValid() );
		assert( !desc->isOpen() );
		assert( desc->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc->isStreamType(STREAM_TYPE_SOCKET) );

		delete desc;

		DMSG("Create non-empty FileDescriptor.");
		desc = new FileDescriptor("testFile1.txt");
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

		delete desc;

		DMSG("Create non-empty FileDescriptor from File.");
		FilePtr file = FilePtr(new File("test_files/test_files_2/readOnly.txt"));
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );

		desc = new FileDescriptor(file);
		assert( !desc->getFileHandle() );
		assert( desc->getFile().notNull() );
		assert( desc->getFile()->retainCount() == 2 );
		assert( strcmp(desc->getFile()->absolutePath(), path) == 0 );
		assert( desc->getName() );
		assert( strcmp(desc->getName(), path) == 0 );
		assert( desc->isValid() );
		assert( !desc->isOpen() );
		assert( desc->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc->isStreamType(STREAM_TYPE_SOCKET) );

		delete desc;
		assert( file->retainCount() == 1 );

		file.setNull();
			
		FINISH_TEST;
	}	

	void testFileDescriptorCopyAndAssignmentOnUnopenedFiles() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		// ##########
		// ########## TEST COPY CONSTRUCTOR ################ //
		// ##########
		FileDescriptor *desc = new FileDescriptor("testFile1.txt");
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

		FileDescriptor *desc2 = new FileDescriptor(*desc);
		assert( !desc2->getFileHandle() );
		assert( desc2->getFile().notNull() );
		assert( desc2->getFile()->retainCount() == 2 );
		assert( strcmp(desc2->getFile()->absolutePath(), path) == 0 );
		assert( desc2->getName() );
		assert( strcmp(desc2->getName(), path) == 0 );
		assert( desc2->isValid() );
		assert( !desc2->isOpen() );
		assert( desc2->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc2->isStreamType(STREAM_TYPE_SOCKET) );

		FileDescriptor *desc3 = new FileDescriptor(*desc);
		assert( !desc3->getFileHandle() );
		assert( desc3->getFile().notNull() );
		assert( desc3->getFile()->retainCount() == 3 );
		assert( strcmp(desc3->getFile()->absolutePath(), path) == 0 );
		assert( desc3->getName() );
		assert( strcmp(desc3->getName(), path) == 0 );
		assert( desc3->isValid() );
		assert( !desc3->isOpen() );
		assert( desc3->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc3->isStreamType(STREAM_TYPE_SOCKET) );

		assert( desc->getFile()->retainCount() == 3 );
		delete desc;

		assert( desc3->getFile()->retainCount() == 2 );
		assert( desc2->getFile()->retainCount() == 2 );

		delete desc3;
		assert( desc2->getFile()->retainCount() == 1 );
		delete desc2;


		// ##########
		// ########## TEST ASSIGNMENT OPERATOR ################ //
		// ##########
		desc = new FileDescriptor("testFile1.txt");
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

		FilePtr file = FilePtr(new File("test_files/test_files_2/readOnly.txt"));
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );

		desc2 = new FileDescriptor(file);
		assert( !desc2->getFileHandle() );
		assert( desc2->getFile().notNull() );
		assert( desc2->getFile()->retainCount() == 2 );
		assert( strcmp(desc2->getFile()->absolutePath(), path) == 0 );
		assert( desc2->getName() );
		assert( strcmp(desc2->getName(), path) == 0 );
		assert( desc2->isValid() );
		assert( !desc2->isOpen() );
		assert( desc2->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc2->isStreamType(STREAM_TYPE_SOCKET) );

		*desc2 = *desc;

		assert(file->retainCount() == 1);
		assert(desc2->getFile()->retainCount() == 2);
		assert(desc->getFile()->retainCount() == 2);

		delete desc2;

		assert(desc->getFile()->retainCount() == 1);
		delete desc;

		assert( file->retainCount() == 1 );
			
		FINISH_TEST;
	}	

	void testFileDescriptorOpen() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		// Try opening with no file.
		FileDescriptor *desc = new FileDescriptor();
		assert( !desc->open() );
		assert( !desc->isOpen() );
		assert( !desc->getMode() );
		delete desc;

		// Try opening successfully in read mode.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open() );
		assert( desc->isOpen() );
		assert( strcmp(desc->getMode(), "rb") == 0 );
		delete desc;

		// Try opening successfully in append mode.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open("ab") );
		assert( desc->isOpen() );
		assert( strcmp(desc->getMode(), "ab") == 0 );

		delete desc;

		// Try opening successfully in append / update mode.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open("a+b") );
		assert( desc->isOpen() );
		assert( strcmp(desc->getMode(), "a+b") == 0 );

		delete desc;


		// Try opening successfully a read only file in read mode.
		FilePtr file = FilePtr(new File("test_files/test_files_2/readOnly.txt"));
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );

		desc = new FileDescriptor(file);
		assert( !desc->isOpen() );
		assert( desc->open("rb") );
		assert( desc->isOpen() );
		assert( strcmp(desc->getMode(), "rb") == 0 );

		delete desc;


		// Try opening file in wrong mode (readonly in append).
		desc = new FileDescriptor(file);
		assert( !desc->isOpen() );
		assert( !desc->open("ab") );
		assert( !desc->isOpen() );
		delete desc;

		// Try opening file in wrong mode (readonly in append / update).
		desc = new FileDescriptor(file);
		assert( !desc->isOpen() );
		assert( !desc->open("a+b") );
		assert( !desc->isOpen() );
		delete desc;

		file.setNull();		

		// Try opening an already opened file.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open() );
		assert( desc->isOpen() );
		assert( strcmp(desc->getMode(), "rb") == 0 );


		FileDescriptor *desc2 = new FileDescriptor();
		*desc2 = *desc;

		assert( desc2->isOpen() );
		assert( !desc2->open() );
		assert( desc2->isOpen() && desc->isOpen() );
		assert( !desc2->open("a+b") );
		assert( desc2->isOpen() && desc->isOpen() );
		assert( strcmp(desc->getMode(), "rb") == 0 );


		delete desc;
		assert( desc2->isOpen() );
		delete desc2;

		// Try opening a non-existant file.
		desc = new FileDescriptor("fileNotThere.txt");
		assert( !desc->open() );
		assert( !desc->isOpen() );
		assert( !desc->getFile()->exists() );
		delete desc;

		// Try opening a directory.
		desc = new FileDescriptor("test_files");
		assert( !desc->isOpen() );
		assert( desc->open() );
		assert( desc->isOpen() );
		assert( desc->getFile()->exists() && desc->getFile()->isDirectory() );

		D(std::cout << std::flush);
		
		// Try opening file with multiple FD's existing.

		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );

		desc2 = new FileDescriptor(*desc);
		assert( !desc2->isOpen() && !desc->isOpen() );

		assert( desc2->open("a+b") );
		assert( desc->isOpen() && desc2->isOpen() );
		delete desc;
		desc = new FileDescriptor(*desc2);
		assert( desc->isOpen() && desc2->isOpen() );
		assert( strcmp(desc2->getMode(), "a+b") == 0 );
		assert( strcmp(desc->getMode(), "a+b") == 0 );

		delete desc2;
		assert( desc->isOpen() );
		delete desc;


		FINISH_TEST;
	}	


	void testFileDescriptorCopyAndAssignmentOnOpenedFiles() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		// ##########
		// ########## TEST COPY CONSTRUCTOR ################ //
		// ##########
		FileDescriptor *desc = new FileDescriptor("testFile1.txt");
		assert( desc->open() );
		assert( desc->getFileHandle() );
		assert( desc->getFile().notNull() );
		assert( desc->getFile()->retainCount() == 1 );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(desc->getFile()->absolutePath(), path) == 0 );
		assert( desc->getName() );
		assert( strcmp(desc->getName(), path) == 0 );
		assert( desc->isValid() );
		assert( desc->isOpen() );
		assert( desc->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc->isStreamType(STREAM_TYPE_SOCKET) );

		FileDescriptor *desc2 = new FileDescriptor(*desc);
		assert( desc2->getFileHandle() );
		assert( desc2->getFile().notNull() );
		assert( desc2->getFile()->retainCount() == 2 );
		assert( strcmp(desc2->getFile()->absolutePath(), path) == 0 );
		assert( desc2->getName() );
		assert( strcmp(desc2->getName(), path) == 0 );
		assert( desc2->isValid() );
		assert( desc2->isOpen() );
		assert( desc2->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc2->isStreamType(STREAM_TYPE_SOCKET) );

		FileDescriptor *desc3 = new FileDescriptor(*desc);
		assert( desc3->getFileHandle() );
		assert( desc3->getFile().notNull() );
		assert( desc3->getFile()->retainCount() == 3 );
		assert( strcmp(desc3->getFile()->absolutePath(), path) == 0 );
		assert( desc3->getName() );
		assert( strcmp(desc3->getName(), path) == 0 );
		assert( desc3->isValid() );
		assert( desc3->isOpen() );
		assert( desc3->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc3->isStreamType(STREAM_TYPE_SOCKET) );

		assert( desc->getFile()->retainCount() == 3 );
		delete desc;

		assert( desc3->getFile()->retainCount() == 2 );
		assert( desc2->getFile()->retainCount() == 2 );

		delete desc3;
		assert( desc2->getFile()->retainCount() == 1 );
		delete desc2;


		// ##########
		// ########## TEST ASSIGNMENT OPERATOR ################ //
		// ##########
		desc = new FileDescriptor("testFile1.txt");
		assert( desc->open() );
		assert( desc->getFileHandle() );
		assert( desc->getFile().notNull() );
		assert( desc->getFile()->retainCount() == 1 );
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(desc->getFile()->absolutePath(), path) == 0 );
		assert( desc->getName() );
		assert( strcmp(desc->getName(), path) == 0 );
		assert( desc->isValid() );
		assert( desc->isOpen() );
		assert( desc->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc->isStreamType(STREAM_TYPE_SOCKET) );

		FilePtr file = FilePtr(new File("test_files/test_files_2/readOnly.txt"));
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );

		desc2 = new FileDescriptor(file);
		assert( desc2->open() );
		assert( desc2->getFileHandle() );
		assert( desc2->getFile().notNull() );
		assert( desc2->getFile()->retainCount() == 2 );
		assert( strcmp(desc2->getFile()->absolutePath(), path) == 0 );
		assert( desc2->getName() );
		assert( strcmp(desc2->getName(), path) == 0 );
		assert( desc2->isValid() );
		assert( desc2->isOpen() );
		assert( desc2->isStreamType(STREAM_TYPE_FILE) );
		assert( !desc2->isStreamType(STREAM_TYPE_SOCKET) );

		*desc2 = *desc;

		assert(file->retainCount() == 1);
		assert(desc2->getFile()->retainCount() == 2);
		assert(desc->getFile()->retainCount() == 2);

		delete desc2;

		assert( desc->getFile()->retainCount() == 1 );
		delete desc;

		assert( file->retainCount() == 1 );
			
		FINISH_TEST;
	}	

	void testFileDescriptorClose() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		// Try closing with no file.
		FileDescriptor *desc = new FileDescriptor();
		desc->close();
		delete desc;

		// Try closing successfully in read mode.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open() );
		assert( desc->isOpen() );
		desc->close();
		assert( !desc->isOpen() );
		delete desc;

		// Try closing successfully in append mode.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open("ab") );
		assert( desc->isOpen() );
		desc->close();
		assert( !desc->isOpen() );
		delete desc;

		// Try closing successfully in append / update mode.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open("a+b") );
		assert( desc->isOpen() );
		desc->close();
		assert( !desc->isOpen() );
		delete desc;


		// Try closing successfully a read only file in read mode.
		FilePtr file = FilePtr(new File("test_files/test_files_2/readOnly.txt"));
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );

		desc = new FileDescriptor(file);
		assert( !desc->isOpen() );
		assert( desc->open("rb") );
		assert( desc->isOpen() );
		desc->close();
		assert( !desc->isOpen() );
		delete desc;


	
		// Try closing an already closed file.
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open() );
		assert( desc->isOpen() );
		desc->close();
		assert( !desc->isOpen() );
		desc->close();

		delete desc;

	
		// Try closing file with multiple FD's existing.
		
		desc = new FileDescriptor("testFile1.txt");
		assert( !desc->isOpen() );
		assert( desc->open() );
		assert( desc->isOpen() );

		FileDescriptor *desc2 = new FileDescriptor(*desc);
		assert( desc2->isOpen() && desc->isOpen() );

		desc2->close();

		assert( !desc->isOpen() && !desc2->isOpen() );

		// Try reopening file.
		assert( desc->open("a+b") );
		assert( desc->isOpen() && desc2->isOpen() );

		delete desc;
		assert( desc2->isOpen() );
		delete desc2;


		FINISH_TEST;
	}	


}

int main(int argc, char** argv) {
	cc::testFileDescriptorBasicCreateAndDestroy();
	cc::testFileDescriptorCopyAndAssignmentOnUnopenedFiles();
	cc::testFileDescriptorOpen();
	cc::testFileDescriptorCopyAndAssignmentOnOpenedFiles();
	cc::testFileDescriptorClose();
	return 0;
}

