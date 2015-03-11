#include "core/testcore.h"
#include "core/io/file.h"

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
	void testFileCreateAndDestroy() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File* file = new File();
		assert( !file->path() );
		delete file;

		file = new File("ThisisAtest.txt");
		assert( file->path() );
		delete file;

		/* Test path + file constructor */
		getAbsPath(path, "/test_files");
		file = new File(path, "test_files_2/testFile1.txt");
		
		getAbsPath(path, "/test_files/test_files_2/");
		assert( StringUtils::equals(path, file->getParentPath()) );
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( StringUtils::equals(path, file->absolutePath()) );
		assert( eq(file->path(), "test_files_2/testFile1.txt") );
		assert( file->exists() );
		assert( file->canRead() );

		delete file;

		/* Test removal of trailing slash in parent path */
		getAbsPath(path, "/test_files/");
		file = new File(path, "test_files_2/testFile1.txt");
	   getAbsPath(path, "/test_files/test_files_2/");
		assert( StringUtils::equals(path, file->getParentPath()) );
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( StringUtils::equals(path, file->absolutePath()) );
		assert( StringUtils::equals(file->path(), "test_files_2/testFile1.txt") );
		assert( file->exists() );
		assert( file->canRead() );

		delete file;

		/* Test with lots of slashes */
		getAbsPath(path, "/test_files///");
		file = new File(path, "test_files_2/testFile1.txt");
	   getAbsPath(path, "/test_files/test_files_2/");
		assert( StringUtils::equals(path, file->getParentPath()) );
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( StringUtils::equals(path, file->absolutePath()) );
		assert( StringUtils::equals(file->path(), "test_files_2/testFile1.txt") );
		assert( file->exists() );
		assert( file->canRead() );
		delete file;

		getAbsPath(path, "/test_files///");
		file = new File(path, "/test_files_2/testFile1.txt");
	   getAbsPath(path, "/test_files/test_files_2/");
		assert( StringUtils::equals(path, file->getParentPath()) );
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( StringUtils::equals(path, file->absolutePath()) );
		assert( StringUtils::equals(file->path(), "/test_files_2/testFile1.txt") );
		assert( file->exists() );
		assert( file->canRead() );
		delete file;

		getAbsPath(path, "/test_files///");
		file = new File(path, "////test_files_2/testFile1.txt");
	   getAbsPath(path, "/test_files/test_files_2/");
		assert( StringUtils::equals(path, file->getParentPath()) );
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( StringUtils::equals(path, file->absolutePath()) );
		assert( StringUtils::equals(file->path(), "////test_files_2/testFile1.txt") );
		assert( file->exists() );
		assert( file->canRead() );
		delete file;

		getAbsPath(path, "/test_files///");
		file = new File(path, "////test_files_2");
	   getAbsPath(path, "/test_files/");
		assert( StringUtils::equals(path, file->getParentPath()) );
		getAbsPath(path, "/test_files/test_files_2");
		assert( StringUtils::equals(path, file->absolutePath()) );
		assert( StringUtils::equals(file->path(), "////test_files_2") );
		assert( file->exists() );
		assert( file->canRead() );
		delete file;

		getAbsPath(path, "/test_files///");
		file = new File(path, "////test_files_2/");
	   getAbsPath(path, "/test_files/");
		assert( StringUtils::equals(path, file->getParentPath()) );
		getAbsPath(path, "/test_files/test_files_2/");
		assert( StringUtils::equals(path, file->absolutePath()) );
		assert( StringUtils::equals(file->path(), "////test_files_2/") );
		assert( file->exists() );
		assert( file->canRead() );
		delete file;

		FINISH_TEST;
	}	

	void testFileGetName() {
		BEGIN_TEST;

		File* file = new File();
		assert( !file->name() );
		delete file;

		file = new File("ThisIsATest.txt");
		assert( strcmp(file->name(), "ThisIsATest.txt") == 0);
		delete file;

		file = new File("this/is/a/test.txt");
		assert( strcmp(file->name(), "test.txt") == 0);
		delete file;

		file = new File("/this/is/a/test2.testing.txt");
		assert( strcmp(file->name(), "test2.testing.txt") == 0);
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->name(), ".hiddenTest") == 0);
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->name(), "directory") == 0);
		delete file;

		file = new File("/this/is/a/test/directory/");
		assert( strcmp(file->name(), "directory/") == 0);
		delete file;

		file = new File("/file");
		assert( strcmp(file->name(), "file") == 0);
		delete file;

		file = new File("/dir/");
		assert( strcmp(file->name(), "dir/") == 0);
		delete file;

		file = new File("/");
		assert( strcmp(file->name(), "") == 0);
		assert( file->name()[0] == 0);	  
		delete file;
			
			
			
		FINISH_TEST;
	}	

	void testFileGetPath() {
		BEGIN_TEST;

		File* file = new File();
		assert( !file->name() );
		delete file;

		file = new File("ThisIsATest.txt");
		assert( strcmp(file->path(), "ThisIsATest.txt") == 0);
		delete file;

		file = new File("this/is/a/test.txt");
		assert( strcmp(file->path(), "this/is/a/test.txt") == 0);
		delete file;

		file = new File("/this/is/a/test2.testing.txt");
		assert( strcmp(file->path(), "/this/is/a/test2.testing.txt") == 0);
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->path(), "/this/is/a/test/.hiddenTest") == 0);
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->path(), "/this/is/a/test/directory") == 0);
		delete file;
		
		file = new File("/this/is/a/test/directory/");
		assert( strcmp(file->path(), "/this/is/a/test/directory/") == 0);
		delete file;
			
		FINISH_TEST;
	}	

	void testFileGetAbsolutePath() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File* file = new File();
		assert( !file->name() );
		delete file;

		file = new File("ThisIsATest.txt");
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		delete file;

		file = new File("this/is/a/test.txt");
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		delete file;

		file = new File("/this/is/a/test2.testing.txt");
		std::cout << file->absolutePath() << std::endl;
		assert( strcmp(file->absolutePath(), "/this/is/a/test2.testing.txt") == 0);
		delete file;

		file = new File("test2.testing.txt");
		getAbsPath(path, "/test2.testing.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/.hiddenTest") == 0);
		delete file;

		file = new File(".hiddenTest");
		getAbsPath(path, "/.hiddenTest");
		assert( strcmp(file->absolutePath(), path) == 0);
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory") == 0);
		delete file;

		file = new File("/this/is/a/test/directory/");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory/") == 0);
		delete file;
			
		FINISH_TEST;
	}	

	void testFileGetParentPath() {
		BEGIN_TEST;

		char parentPath[ABS_PATH_BUFFER_LEN];

		File* file = new File();
		assert( !file->name() );
		delete file;

		file = new File("ThisIsATest.txt");
		getAbsPath(parentPath, NIL);
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		delete file;

		file = new File("this/is/a/test.txt");
		getAbsPath(parentPath, "/this/is/a/");
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		delete file;

		file = new File("/this/is/a/test2.testing.txt");
		assert( strcmp(file->getParentPath(), "/this/is/a/") == 0);
		delete file;

		file = new File("test2.testing.txt");
		getAbsPath(parentPath, NIL);
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		delete file;

		file = new File(".hiddenTest");
		getAbsPath(parentPath, NIL);
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		delete file;

		file = new File("/this/is/a/test/directory/");
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		delete file;
			
		FINISH_TEST;
	}	
	
	void testFileGetCanonicalPath() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File* file = new File();
		assert( !file->name() );
		delete file;

		file = new File("ThisIsATest.txt");
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		delete file;

		file = new File("this/is/a/test.txt");
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		delete file;

		file = new File("/this/is/a/test2.testing.txt");
		assert( strcmp(file->getCanonicalPath(), "/this/is/a/test2.testing.txt") == 0);
		delete file;

		file = new File("test2.testing.txt");
		getAbsPath(path, "/test2.testing.txt");
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->getCanonicalPath(), "/this/is/a/test/.hiddenTest") == 0);
		delete file;

		file = new File(".hiddenTest");
		getAbsPath(path, "/.hiddenTest");
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->getCanonicalPath(), "/this/is/a/test/directory") == 0);
		delete file;
			
		FINISH_TEST;
	}	

	void testFileCopyConstructor() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File* file2 = NIL;
		File* file = new File();
		assert( !file->name() );
		delete file;

		// #########
		// ######### TEST FILE # 1 #################//
		// #########
		file = new File("ThisIsATest.txt");
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		getAbsPath(path, NIL);
		assert( strcmp(file->getParentPath(), path) == 0);
		assert( strcmp(file->path(), "ThisIsATest.txt") == 0);
		assert( strcmp(file->name(), "ThisIsATest.txt") == 0);
		file2 = new File(*file);
		delete file;
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file2->absolutePath(), path) == 0);
		assert( strcmp(file2->getCanonicalPath(), path) == 0);
		getAbsPath(path, NIL);
		assert( strcmp(file2->getParentPath(), path) == 0);
		assert( strcmp(file2->path(), "ThisIsATest.txt") == 0);
		assert( strcmp(file2->name(), "ThisIsATest.txt") == 0);
		delete file2;

		// #########
		// ######### TEST FILE # 2 #################//
		// #########
		file = new File("this/is/a/test.txt");
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		getAbsPath(path, "/this/is/a/");
		assert( strcmp(file->getParentPath(), path) == 0);
		assert( strcmp(file->path(), "this/is/a/test.txt") == 0);
		assert( strcmp(file->name(), "test.txt") == 0);
		file2 = new File(*file);
		delete file;
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file2->absolutePath(), path) == 0);
		assert( strcmp(file2->getCanonicalPath(), path) == 0);
		getAbsPath(path, "/this/is/a/");
		assert( strcmp(file2->getParentPath(), path) == 0);
		assert( strcmp(file2->path(), "this/is/a/test.txt") == 0);
		assert( strcmp(file2->name(), "test.txt") == 0);
		delete file2;

		// #########
		// ######### TEST FILE # 3 #################//
		// #########
		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getCanonicalPath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		assert( strcmp(file->path(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->name(), "directory") == 0);
		file2 = new File(*file);
		assert( strcmp(file2->absolutePath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file2->getCanonicalPath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file2->getParentPath(), "/this/is/a/test/") == 0);
		assert( strcmp(file2->path(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file2->name(), "directory") == 0);
		delete file2;
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getCanonicalPath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		assert( strcmp(file->path(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->name(), "directory") == 0);
		delete file;

		FINISH_TEST;
	}	

	void testFileAssignmentOperator() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File file2;
		File* file = new File();
		assert( !file->name() );
		delete file;

		// #########
		// ######### TEST FILE # 1 #################//
		// #########
		DMSG("TEST FILE #1");		
		file = new File("ThisIsATest.txt");
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		getAbsPath(path, NIL);
		assert( strcmp(file->getParentPath(), path) == 0);
		assert( strcmp(file->path(), "ThisIsATest.txt") == 0);
		assert( strcmp(file->name(), "ThisIsATest.txt") == 0);
		file2 = *file;
		delete file;
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file2.absolutePath(), path) == 0);
		assert( strcmp(file2.getCanonicalPath(), path) == 0);
		getAbsPath(path, NIL);
		assert( strcmp(file2.getParentPath(), path) == 0);
		assert( strcmp(file2.path(), "ThisIsATest.txt") == 0);
		assert( strcmp(file2.name(), "ThisIsATest.txt") == 0);

		// #########
		// ######### TEST FILE # 2 #################//
		// #########
		DMSG("TEST FILE #2");
		file = new File("this/is/a/test.txt");
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( strcmp(file->getCanonicalPath(), path) == 0);
		getAbsPath(path, "/this/is/a/");
		assert( strcmp(file->getParentPath(), path) == 0);
		assert( strcmp(file->path(), "this/is/a/test.txt") == 0);
		assert( strcmp(file->name(), "test.txt") == 0);
		file2 = *file;
		delete file;
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file2.absolutePath(), path) == 0);
		assert( strcmp(file2.getCanonicalPath(), path) == 0);
		getAbsPath(path, "/this/is/a/");
		assert( strcmp(file2.getParentPath(), path) == 0);
		assert( strcmp(file2.path(), "this/is/a/test.txt") == 0);
		assert( strcmp(file2.name(), "test.txt") == 0);

		// #########
		// ######### TEST FILE # 3 #################//
		// #########
		DMSG("TEST FILE #3");
		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getCanonicalPath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		assert( strcmp(file->path(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->name(), "directory") == 0);
		file2 = *file;
		assert( strcmp(file2.absolutePath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file2.getCanonicalPath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file2.getParentPath(), "/this/is/a/test/") == 0);
		assert( strcmp(file2.path(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file2.name(), "directory") == 0);
		file2 = File();
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getCanonicalPath(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		assert( strcmp(file->path(), "/this/is/a/test/directory") == 0);
		assert( strcmp(file->name(), "directory") == 0);
		delete file;

		// #########
		// ######### TEST FILE # 4 #################//
		// #########
		DMSG("TEST FILE #4");
		file2 = File("this/is/a/test.txt");
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file2.absolutePath(), path) == 0);
		assert( strcmp(file2.getCanonicalPath(), path) == 0);
		getAbsPath(path, "/this/is/a/");
		assert( strcmp(file2.getParentPath(), path) == 0);
		assert( strcmp(file2.path(), "this/is/a/test.txt") == 0);
		assert( strcmp(file2.name(), "test.txt") == 0);


		FINISH_TEST;
	}	

	void testFileCanRead() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File();
		assert( !file->canRead() );
		delete file;

		file = new File("ThisisAtest.txt");
		assert( file->path() );
		assert( !file->canRead() );
		delete file;

		file = new File("testFile1.txt");
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );
		delete file;

			
		
		getAbsPath(path, "/testFileWrite1.txt");	
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );
		delete file;

		file = new File("test_files/test_files_2/testFile1.txt");
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );
		delete file;

		file = new File("test_files/test_files_2/readOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );
		delete file;

		file = new File("test_files/test_files_2/writeOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		//assert( !file->canRead() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		//assert( !file->canRead() );
		delete file;


	
		FINISH_TEST;
	}	

	void testFileCanWrite() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File();
		assert( !file->canWrite() );
		delete file;

		file = new File("ThisisAtest.txt");
		assert( file->path() );
		assert( !file->canWrite() );
		delete file;

		file = new File("testFile1.txt");
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canWrite() );
		delete file;

		getAbsPath(path, "/testFileWrite1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canWrite() );
		delete file;

		file = new File("test_files/test_files_2/testFile1.txt");
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canWrite() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canWrite() );
		delete file;

		file = new File("test_files/test_files_2/readOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canWrite() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canWrite() );
		delete file;

		file = new File("test_files/test_files_2/writeOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canWrite() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canWrite() );
		delete file;
	
		FINISH_TEST;
	}	

	void testFileCreateAndRemove() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File();
		assert( !file->create() );
		assert( !file->remove() );
		delete file;

		file = new File("removeMe.txt");
		getAbsPath(path, "/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canRead() );
		assert( file->create() );
		assert( !file->create() );
		assert( file->canRead() );
		assert( file->remove() );
		assert( !file->canRead() );
		delete file;

		file = new File("test_files/test_files_2/removeMe.txt");
		getAbsPath(path, "/test_files/test_files_2/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->path() );
		assert( !file->canRead() );
		assert( file->create() );
		assert( !file->create() );
		assert( file->canRead() );
		assert( file->remove() );
		assert( !file->canRead() );
		delete file;

		file = new File("test_files/test_files_2/removeMeDir");
		getAbsPath(path, "/test_files/test_files_2/removeMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->path() );
		assert( !file->canRead() );
		assert( file->makeDir() );
		assert( !file->create() );
		assert( !file->makeDir() );
		assert( file->canRead() );
		assert( file->remove() );
		assert( !file->canRead() );
		delete file;

		file = new File("removeMeDir");
		getAbsPath(path, "/removeMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->path() );
		assert( !file->canRead() );
		assert( file->makeDir() );
		assert( !file->create() );
		assert( !file->makeDir() );
		assert( file->canRead() );
		assert( file->remove() );
		assert( !file->canRead() );
		delete file;

	   file = new File("test_files/test_files_2/test/recursive/creation/file.txt");
		getAbsPath(path, "/test_files/test_files_2/test/recursive/creation/file.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->path() );
		assert( !file->canRead() );
		assert( !file->create() );
		assert( file->create(true));		
		assert( !file->create(true) );
		assert( file->canRead() );
		assert( file->remove() );
		assert( !file->canRead() );

		FilePtr parent = file->getParentFile();
		assert(parent->remove());
		parent = parent->getParentFile();
		assert(parent->remove());
		parent = parent->getParentFile();
		assert(parent->remove());
		
		delete file;

		file = new File("test_files/test_files_2/test/");
		getAbsPath(path, "/test_files/test_files_2/test/");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->path() );
		assert( !file->canRead() );
		delete file;

	
		FINISH_TEST;
	}	

	void testFileRemoveFileOnDestroy() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File("removeMe.txt");
		getAbsPath(path, "/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canRead() );
		assert( file->create() );
		assert( file->canRead() );
		assert( file->exists() );
		file->setRemoveFileOnDestroy(true);
		delete file;
		file = new File("removeMe.txt");
		getAbsPath(path, "/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canRead() );
		assert( !file->exists() );

		
		file = new File("removeMeDir");
		getAbsPath(path, "/removeMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canRead() );
		assert( file->makeDir() );
		assert( file->canRead() );
		assert( file->exists() );
		file->setRemoveFileOnDestroy(true);
		delete file;
		file = new File("removeMeDir");
		getAbsPath(path, "/removeMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canRead() );
		assert( !file->exists() );

	
		FINISH_TEST;
	}	

	void testFileEmpty() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File("removeMe.txt");
		getAbsPath(path, "/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->canRead() );
		assert( file->create() );
		assert( file->canRead() );
		assert( file->exists() );
		delete file;

		U32 test = 69;
		FILE* fileptr = fopen(path, "wb");
		fwrite(&test, sizeof(U32), 1, fileptr);
		fflush(fileptr);
		fclose(fileptr);

		U32 test2 = 0;
		fileptr = fopen(path, "rb");
		assert( test2 == 0 );
		fread(&test2, sizeof(U32), 1, fileptr);
		assert( test2 == 69 );
		fclose(fileptr);

		file = new File("removeMe.txt");
		getAbsPath(path, "/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->canRead() );
		assert( file->exists() );
		assert( file->empty() );
		delete file;

		test2 = 0;
		fileptr = fopen(path, "rb");
		assert( test2 == 0 );
		fread(&test2, sizeof(U32), 1, fileptr);
		assert( test2 == 0 );
		fclose(fileptr);
	
		FINISH_TEST;

		file = new File("removeMe.txt");
		assert( file->exists() );
		file->remove();
		assert( !file->exists() );
		delete file;

		file = new File("removeMe.txt");
		assert( !file->exists() );
		assert( file->empty() );
		assert( file->exists() );
		file->remove();
		assert( !file->exists() );


	}	


	void testFileEquals() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File *file1 = new File("testFile1.txt");
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(file1->absolutePath(), path) == 0 );
		assert( file1->canWrite() );

		getAbsPath(path, "/testFileWrite1.txt");
		File* file2 = new File(path);
		assert( strcmp(file2->absolutePath(), path) == 0 );
		assert( file2->canWrite() );

		assert ( !file1->equals(file2) );

		delete file1;
		delete file2;

		file1 = new File("test_files/test_files_2/testFile1.txt");
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( strcmp(file1->absolutePath(), path) == 0 );
		assert( file1->canWrite() );

		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		file2 = new File(path);
		assert( strcmp(file2->absolutePath(), path) == 0 );
		assert( file2->canWrite() );

		assert( file1->equals(file2) );

		delete file1;
		delete file2;

		file1 = new File("test_files/test_files_2/readOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file1->absolutePath(), path) == 0 );
		assert( !file1->canWrite() );

		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		file2 = new File(path);
		assert( strcmp(file2->absolutePath(), path) == 0 );
		assert( !file2->canWrite() );

		assert( (*file1).equals(*file2) );
		delete file1;
		delete file2;

		file1 = new File("test_files/test_files_2/writeOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(file1->absolutePath(), path) == 0 );
		assert( file1->canWrite() );

		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		file2 = new File(path);
		assert( strcmp(file2->absolutePath(), path) == 0 );
		assert( file2->canWrite() );

		assert( file1->equals(file2) );
		assert( file2->equals(file1) );
		assert( file1->equals(file1) );
		assert( (*file2).equals(*file2) );

		delete file1;

		assert( strcmp(file2->absolutePath(), path) == 0 );

		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		file1 = new File(path);
		assert( strcmp(file1->absolutePath(), path) == 0 );
		assert( !file1->canWrite() );

		assert( !file1->equals(file2) );
		assert( !(*file2).equals(*file1) );


		delete file1;
		delete file2;
	
		FINISH_TEST;
	}	

	void testFileExists() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File();
		assert( !file->canWrite() );
		delete file;

		file = new File("ThisisAtest.txt");
		assert( file->path() );
		assert( !file->exists() );
		delete file;

		file = new File("testFile1.txt");
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		getAbsPath(path, "/testFileWrite1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		file = new File("test_files/test_files_2/testFile1.txt");
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		file = new File("test_files/test_files_2/readOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		file = new File("test_files/test_files_2/writeOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		file = new File("file_tests.cpp");
		getAbsPath(path, "/file_tests.cpp");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

		getAbsPath(path, "/file_tests.cpp");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->exists() );
		delete file;

	
		FINISH_TEST;
	}	

	void testFileGetParentFile() {
		BEGIN_TEST;

		char parentPath[ABS_PATH_BUFFER_LEN];

		File* file = new File();
		assert( !file->name() );
		delete file;

		file = new File("ThisIsATest.txt");
		getAbsPath(parentPath, NIL);
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		FilePtr pfile = file->getParentFile();
		assert( strcmp(pfile->absolutePath(), parentPath) == 0);
		delete file;

		file = new File("this/is/a/test.txt");
		getAbsPath(parentPath, "/this/is/a/");
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		pfile = file->getParentFile();
		delete file;
		assert( strcmp(pfile->absolutePath(), parentPath) == 0);
		
		file = new File("/this/is/a/test2.testing.txt");
		assert( strcmp(file->getParentPath(), "/this/is/a/") == 0);
		pfile = file->getParentFile();
		assert( strcmp(pfile->absolutePath(), "/this/is/a/") == 0);
		delete file;

		file = new File("test2.testing.txt");
		getAbsPath(parentPath, NIL);
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		pfile = file->getParentFile();
		assert( strcmp(pfile->absolutePath(), parentPath) == 0);
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		pfile = file->getParentFile();
		assert( strcmp(pfile->absolutePath(), "/this/is/a/test/") == 0);
		delete file;

		file = new File(".hiddenTest");
		getAbsPath(parentPath, NIL);
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		pfile = file->getParentFile();
		assert( strcmp(pfile->absolutePath(), parentPath) == 0);
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->getParentPath(), "/this/is/a/test/") == 0);
		pfile = file->getParentFile();
		assert( strcmp(pfile->absolutePath(), "/this/is/a/test/") == 0);
		delete file;

		file = new File("test_files/test_files_2/testFile1.txt");
		getAbsPath(parentPath, "/test_files/test_files_2/");
		assert( strcmp(file->getParentPath(), parentPath) == 0);
		assert( file->exists() );
		assert( !file->isDirectory() );
		pfile = file->getParentFile();
		assert( strcmp(pfile->absolutePath(), parentPath) == 0);
		assert( pfile->exists() );
		assert( pfile->isDirectory() );
		FilePtr pfile2 = pfile->getParentFile();
		getAbsPath(parentPath, "/test_files/");
		assert( strcmp(pfile2->absolutePath(), parentPath) == 0);
		assert(pfile2->exists());
		assert( pfile2->isDirectory() );

		FilePtr pfile3 = pfile2->getParentFile();
		getAbsPath(parentPath, NIL);
		assert( strcmp(pfile3->absolutePath(), parentPath) == 0);
		assert(pfile3->exists());
		assert( pfile3->isDirectory() );

		delete file;
			
		FINISH_TEST;
	}	

	void testFileIsAbsolute() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File* file = new File();
		assert( !file->name() );
		delete file;

		file = new File("ThisIsATest.txt");
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isAbsolute() );
		delete file;

		file = new File("this/is/a/test.txt");
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isAbsolute() );

		delete file;

		file = new File("/this/is/a/test2.testing.txt");
		assert( strcmp(file->absolutePath(), "/this/is/a/test2.testing.txt") == 0);
		assert( file->isAbsolute() );
		delete file;

		file = new File("test2.testing.txt");
		getAbsPath(path, "/test2.testing.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isAbsolute() );
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/.hiddenTest") == 0);
		assert( file->isAbsolute() );
		delete file;

		file = new File(".hiddenTest");
		getAbsPath(path, "/.hiddenTest");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isAbsolute() );
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory") == 0);
		assert( file->isAbsolute() );
		delete file;
			
		FINISH_TEST;
	}

	void testFileIsDirectoryAndIsFile() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File();
		assert( !file->isDirectory() );
		assert( !file->isFile() );

		delete file;

		file = new File("ThisisAtest.txt");
		assert( file->path() );
		assert( !file->isDirectory() );
		assert( !file->isFile() );
		delete file;

		file = new File("testFile1.txt");
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		getAbsPath(path, "/testFileWrite1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		file = new File("test_files/test_files_2/testFile1.txt");
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		file = new File("test_files/test_files_2/readOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		file = new File("test_files/test_files_2/writeOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		file = new File("file_tests.cpp");
		getAbsPath(path, "/file_tests.cpp");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		getAbsPath(path, "/file_tests.cpp");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( !file->isDirectory() );
		assert( file->isFile() );
		delete file;

		file = new File("test_files");
		getAbsPath(path, "/test_files");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->isDirectory() );
		assert( !file->isFile() );
		delete file;

		getAbsPath(path, "/test_files");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->isDirectory() );
		assert( !file->isFile() );
		delete file;

		file = new File("test_files/test_files_2");
		getAbsPath(path, "/test_files/test_files_2");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->isDirectory() );
		assert( !file->isFile() );
		delete file;

		getAbsPath(path, "/test_files/test_files_2");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->isDirectory() );
		assert( !file->isFile() );
		delete file;
	
		FINISH_TEST;
	}	

	void testFileIsHidden() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];

		File* file = new File();
		assert( !file->name() );
		assert( !file->isHidden() );
		delete file;
		
		file = new File("ThisIsATest.txt");
		getAbsPath(path, "/ThisIsATest.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isHidden() );
		delete file;

		file = new File("this/is/a/test.txt");
		getAbsPath(path, "/this/is/a/test.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isHidden() );
		delete file;

		file = new File("/this/is/a/test2.testing.txt");
		assert( strcmp(file->absolutePath(), "/this/is/a/test2.testing.txt") == 0);
		assert( file->isAbsolute() );
		delete file;

		file = new File("test2.testing.txt");
		getAbsPath(path, "/test2.testing.txt");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isHidden() );
		delete file;

		file = new File("/this/is/a/test/.hiddenTest");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/.hiddenTest") == 0);
		assert( file->isHidden() );
		delete file;

		file = new File(".hiddenTest");
		getAbsPath(path, "/.hiddenTest");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( file->isHidden() );
		delete file;

		file = new File("/this/is/a/test/directory");
		assert( strcmp(file->absolutePath(), "/this/is/a/test/directory") == 0);
		assert( !file->isHidden() );
		delete file;

		file = new File("../.testingHidden");
		getAbsPath(path, "/../.testingHidden");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( file->isHidden() );
		delete file;

		file = new File("../testingNotHidden");
		getAbsPath(path, "/../testingNotHidden");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isHidden() );
		delete file;

		file = new File("./testingNotHidden");
		getAbsPath(path, "/./testingNotHidden");
		assert( strcmp(file->absolutePath(), path) == 0);
		assert( !file->isHidden() );
		delete file;
			
		FINISH_TEST;
	}

	void testFileLength() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File();
		assert( file->getLength() == 0);
		delete file;

		file = new File("ThisisAtest.txt");
		assert( file->path() );
		assert( file->getLength() == 0);
		delete file;

		file = new File("testFile1.txt");
		getAbsPath(path, "/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 117);
		delete file;

		getAbsPath(path, "/testFileWrite1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 178);
		delete file;

		file = new File("test_files/test_files_2/testFile1.txt");
		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 117);
		delete file;

		getAbsPath(path, "/test_files/test_files_2/testFile1.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 117);
		delete file;

		file = new File("test_files/test_files_2/readOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 117);
		delete file;

		getAbsPath(path, "/test_files/test_files_2/readOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 117);
		delete file;

		file = new File("test_files/test_files_2/writeOnly.txt");
		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 117);
		delete file;

		getAbsPath(path, "/test_files/test_files_2/writeOnly.txt");
		file = new File(path);
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->getLength() == 117);
		delete file;
	
		FINISH_TEST;
	}	


	void testFileRename() {
		BEGIN_TEST;

		char path[ABS_PATH_BUFFER_LEN];


		File* file = new File();
		assert( !file->rename("something") );
		delete file;

		file = new File("removeMe.txt");
		getAbsPath(path, "/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "removeMe.txt") == 0);
		assert( !file->canRead() );
		assert( !file->rename("noRemoveMe.txt") );
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "removeMe.txt") == 0);
		assert( file->create() );
		assert( file->exists() );
		assert( file->rename("noRemoveMe.txt") );
		assert( file->exists() );
		assert( !file->create() );
		getAbsPath(path, "/noRemoveMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "noRemoveMe.txt") == 0);
		assert( strcmp(file->path(), "noRemoveMe.txt") == 0);
		assert( file->remove() );
		assert( !file->exists() );
		delete file;

		file = new File("test_files/test_files_2/removeMe.txt");
		getAbsPath(path, "/test_files/test_files_2/removeMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "removeMe.txt") == 0);
		assert( !file->canRead() );
		assert( !file->rename("noRemoveMe.txt") );
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->create() );
		assert( file->exists() );
		assert( file->rename("noRemoveMe.txt") );
		assert( file->exists() );
		assert( !file->create() );
		getAbsPath(path, "/test_files/test_files_2/noRemoveMe.txt");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "noRemoveMe.txt") == 0);
		assert( strcmp(file->path(), "test_files/test_files_2/noRemoveMe.txt") == 0);
		assert( file->remove() );
		assert( !file->exists() );
		delete file;

		file = new File("test_files/test_files_2/removeMeDir");
		getAbsPath(path, "/test_files/test_files_2/removeMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "removeMeDir") == 0);
		assert( !file->canRead() );
		assert( !file->rename("noRemoveMeDir") );
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->create() );
		assert( file->exists() );
		assert( file->rename("noRemoveMeDir") );
		assert( file->exists() );
		assert( !file->create() );
		getAbsPath(path, "/test_files/test_files_2/noRemoveMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "noRemoveMeDir") == 0);
		assert( strcmp(file->path(), "test_files/test_files_2/noRemoveMeDir") == 0);
		assert( file->remove() );
		assert( !file->exists() );
		delete file;

		file = new File("removeMeDir");
		getAbsPath(path, "/removeMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "removeMeDir") == 0);
		assert( !file->canRead() );
		assert( !file->rename("noRemoveMeDir") );
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->create() );
		assert( file->exists() );
		assert( file->rename("noRemoveMeDir") );
		assert( file->exists() );
		assert( !file->create() );
		getAbsPath(path, "/noRemoveMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "noRemoveMeDir") == 0);
		assert( strcmp(file->path(), "noRemoveMeDir") == 0);

		assert( file->remove() );
		assert( !file->exists() );
		delete file;

		file = new File("removeMeDirLonger");
		getAbsPath(path, "/removeMeDirLonger");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "removeMeDirLonger") == 0);
		assert( !file->canRead() );
		assert( !file->rename("noRemoveMeDir") );
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( file->create() );
		assert( file->exists() );
		assert( file->rename("noRemoveMeDir") );
		assert( file->exists() );
		assert( !file->create() );
		getAbsPath(path, "/noRemoveMeDir");
		assert( strcmp(file->absolutePath(), path) == 0 );
		assert( strcmp(file->name(), "noRemoveMeDir") == 0);
		assert( strcmp(file->path(), "noRemoveMeDir") == 0);

		assert( file->remove() );
		assert( !file->exists() );
		delete file;

		FINISH_TEST;
	}	
}

int main(int argc, char** argv) {
	cc::testFileCreateAndDestroy();
	cc::testFileGetName();
	cc::testFileGetPath();
	cc::testFileGetAbsolutePath();
	cc::testFileGetParentPath();
	cc::testFileGetCanonicalPath();
	cc::testFileCopyConstructor();
	cc::testFileAssignmentOperator();
	cc::testFileCanRead();
	cc::testFileCanWrite();
	cc::testFileCreateAndRemove();
	cc::testFileRemoveFileOnDestroy();
	cc::testFileEmpty();
	cc::testFileEquals();
	cc::testFileExists();
	cc::testFileGetParentFile();
	cc::testFileIsAbsolute();
	cc::testFileIsDirectoryAndIsFile();
	cc::testFileIsHidden();
	cc::testFileLength();
	cc::testFileRename();
	return 0;
}

