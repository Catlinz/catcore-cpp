#!/usr/bin/python

from optparse import OptionParser
import subprocess
import random
import os
import sys
import re

TEST_NAME = "Core"
FILE_PATH = os.path.split(os.path.realpath(__file__))[0]
TEST_BASE_DIR = os.path.abspath(FILE_PATH+"/unit")+"/"
TEST_TYPE = "unit"
ERRORSORWARNINGS = dict()

MSG_STRING = "#--> Message"
WARN_STRING = "#----> Warning"
ERR_STRING = "#------> ERROR"
MAKE_ERR = "make: ***"
COMPILE_WARN_FN_RE = "^[a-z0-9\-\_]+\.cpp\:\sIn\sfunction"
COMPILE_WARN_RE = "^[a-z0-9\-\_\.\/]+\.(cpp|h)\:[0-9\:]+\:\swarning\:"
COMPILE_ERROR_RE = "^[a-z0-9\-\_\.\/]+\.(cpp|h)\:[0-9\:]+\:\serror\:"
COMPILE_NOTE_RE = "^[a-z0-9\-\_\.\/]+\.(cpp|h)\:[0-9\:]+\:\snote\:"
ASSERT_FAILED_STRING = "Assertion failed:"
BOLD = "\033[1m"
UNDERLINED = "\033[4m"
RESET = "\033[0m"
RED = "\033[31m"
YELLOW = "\033[33m"
GREEN = "\033[32m"
DARKGREY_BG = "\033[100m"
LIGHTBLUE_BG = "\033[104m"
YELLOW_BG = "\033[43m\033[30m"
RED_BG = "\033[41m"
CYAN = "\033[96m"

def printOutputLine(strline):
        line = str(strline.rstrip())
        if line.find(WARN_STRING) == 0 or \
           re.match(COMPILE_WARN_FN_RE, line) != None or \
           re.match(COMPILE_WARN_RE, line) != None:
                print(BOLD+YELLOW+line+RESET)
        elif line.find(ERR_STRING) == 0 or \
             line.find(ASSERT_FAILED_STRING) == 0 or \
             line.find(MAKE_ERR) == 0 or \
             re.match(COMPILE_ERROR_RE, line) != None:
                print(BOLD+RED+line+RESET)
        elif re.match(COMPILE_NOTE_RE, line) != None:
                print(BOLD+CYAN+line+RESET)
        else:
                print(line)


def printRunningTestName(test):
         module = getModuleNameFromTestPath(test)
         testname = getTestNameFromTestPath(test)
         print BOLD+DARKGREY_BG+"\n--------------------------------------------------"
         print "----- Running %s test %s.%s.%s -----" % (TEST_TYPE, TEST_NAME, module, testname)
         print "--------------------------------------------------"+RESET+"\n"
         
         


def runMakeCommand(command, options):
        if (options.extraFlags and len(options.extraFlags) > 0):
                flags = ""
                for flag in options.extraFlags:
                        flags = flags + "-D"+flag+" ";
                flags.strip()
                cmd = ["make", "EXTRAFLAGS="+flags, command]
        else:
                cmd = ["make", command]
	print ">>>> %s%s:%s" % (BOLD, " ".join(cmd), RESET)
	p = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
        for line in iter(p.stdout.readline, b''):
                printOutputLine(line)   
	stdout = p.communicate()
	return p.returncode

def cleanProjectForTests(options, module):
	cmd = ["./build.py", "--clean", "--%s" % (options.makeType), "-m", module]
        if (options.extraFlags and len(options.extraFlags) > 0):
                for flag in options.extraFlags:
                        cmd.append("-x")
                        cmd.append(flag)
	print ">>>> %s%s:%s" % (BOLD, " ".join(cmd), RESET)
	p = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
	for line in iter(p.stdout.readline, b''):
                printOutputLine(line) 
        stdout = p.communicate()
	return p.returncode

def buildProjectForTests(options, module):
	cmd = ["./build.py", "--%s" % (options.makeType), "-m", module]
        if (options.extraFlags and len(options.extraFlags) > 0):
                for flag in options.extraFlags:
                        cmd.append("-x")
                        cmd.append(flag)
	print ">>>> %s%s:%s" % (BOLD, " ".join(cmd), RESET)
	p = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
        for line in iter(p.stdout.readline, b''):
                printOutputLine(line)    
	stdout = p.communicate()
	return p.returncode


def getListOfTestModuleDirs():
        baseDir = getTestDir()
        testDirs = [  os.path.abspath(baseDir+f)+"/" for f in os.listdir(baseDir) if os.path.isdir(baseDir+f) and f != "build" and f != "bin" and f != "test_files" ]
        return testDirs

def getListOfTests():
        tests = list()
        testDirs = getListOfTestModuleDirs()
        for dir in testDirs:
                tests.extend([  os.path.splitext(dir+"/"+f)[0] for f in os.listdir(dir) if os.path.splitext(f)[1] == ".cpp" ])
	return tests

def getListOfTestsForModule(dir):
        baseDir = getTestDir()+dir+"/"
        tests = [  os.path.splitext(os.path.abspath(baseDir+f))[0] for f in os.listdir(baseDir) if os.path.splitext(f)[1] == ".cpp" ]
	return tests

def getModuleNameFromTestPath(path):
        return os.path.basename(os.path.dirname(path))

def getTestNameFromTestPath(path):
        return os.path.basename(os.path.splitext(path)[0].split("_")[0])

def makeProjectForTests(options):
	os.chdir(FILE_PATH)
	os.chdir("../../")
	if cleanProjectForTests(options, TEST_NAME) != 0:
		print "%s%sError cleaning %s Project!%s" % (BOLD, RED, TEST_NAME, RESET)
		exit(1)
	if buildProjectForTests(options, TEST_NAME) != 0:
		print "%s%sError making %s Project!%s" % (BOLD, RED, TEST_NAME, RESET)
		exit(1)

def makeAllTests(options):
        dirs = getListOfTestModuleDirs()
        for dir in dirs:
                module = getModuleNameFromTestPath(dir)
                makeAllTestsForModule(module, options)

def makeAllTestsForModule(module, options):
        baseDir = getTestDir()+module+"/"
        print "%sBuilding %s.%s %s tests!\n-------------------------------%s" % (BOLD, TEST_NAME, module, TEST_TYPE, RESET)
        os.chdir(baseDir)
        if runMakeCommand("clean", options) != 0:
                print "%s%sError cleaning %s.%s %s tests!%s" % (BOLD, RED, TEST_NAME, module, TEST_TYPE, RESET)
                exit(1)
        if runMakeCommand(options.makeType, options) != 0:
                print "%s%sError making %s.%s %s tests!%s" % (BOLD, RED, TEST_NAME, module, TEST_TYPE, RESET)
                exit(1)
                        
        
def makeTest(test, options):
	os.chdir(FILE_PATH)
	os.chdir("../")
        baseDir = getTestDir()
        module = getModuleNameFromTestPath(baseDir+test)
        testname = getTestNameFromTestPath(baseDir+test)
	print "%sBuilding %s.%s.%s %s tests!\n-------------------------------%s" % (BOLD, TEST_NAME, module, testname, TEST_TYPE, RESET)
	os.chdir(baseDir+module+"/")
	if runMakeCommand("clean", options) != 0:
		print "%s%sError cleaning %s.%s.%s %s tests!%s" % (BOLD, RED, TEST_NAME, module, testname, TEST_TYPE, RESET)
		exit(1)
	makeCmd = "%s_tests_TEST" % (testname)
	if options.makeType == "release":
		makeCmd = "RELEASE=1 %s" % (makeCmd)

	if runMakeCommand(makeCmd, options) != 0:
		print "%s%sError making %s.%s.%s %s tests!%s" % (BOLD, RED, TEST_NAME, module, testname, TEST_TYPE, RESET)
		exit(1)

def getTestDir():
        return TEST_BASE_DIR

def runAllTests(options):
	makeAllTests(options)
	print BOLD+"\nRunning all "+TEST_NAME+" "+TEST_TYPE+" tests!\n"+RESET
	tests = getListOfTests()
	dir = getTestDir()
	for test in tests:                
                lines = list()
                module = getModuleNameFromTestPath(test)
                os.chdir(dir+module+"/")
                testname = getTestNameFromTestPath(test)
		cmd = [os.path.realpath(dir+"/bin/"+module+"/"+testname+"_tests_TEST")]
		printRunningTestName(test)
 		p = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
		for line in iter(p.stdout.readline, b''):
                        printOutputLine(line)   
                        lines.append(str(line.rstrip()))
                stdout = p.communicate()
		errors = list()
		warnings = list()

		curTest = ""
		for line in lines:
			if line[0:13] == ">>> BEGINNING":
				curTest = line[14:]
			elif line[0:14] == "#----> Warning":
				warnings.append([curTest, line])
			elif line[0:14] == "#------> ERROR":
				errors.append([curTest, line])
			elif line[0:17] == "Assertion failed:":
				errors.append([curTest, line])
		if p.returncode != 0:
			errors.append([curTest, "%s%sProgram terminated with return code %d%s" % (BOLD, RED, p.returncode, RESET)])
		if len(errors) > 0 or len(warnings) > 0:
			ERRORSORWARNINGS[module+"."+testname] = [errors, warnings]

def runAllTestsForModule(module, options):
	makeAllTestsForModule(module, options)
	print "\n%sRunning all %s.%s %s tests!\n%s" %(BOLD, TEST_NAME, module, TEST_TYPE, RESET)
	tests = getListOfTestsForModule(module)
	dir = getTestDir()
        os.chdir(dir+module+"/")
	for test in tests:    
                lines = list()
                testname = getTestNameFromTestPath(test)
		cmd = [os.path.realpath(dir+"/bin/"+module+"/"+testname+"_tests_TEST")]
		printRunningTestName(test)
 		p = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
		for line in iter(p.stdout.readline, b''):
                        printOutputLine(line)
                        lines.append(str(line.rstrip()))
                        
                stdout = p.communicate()
		errors = list()
		warnings = list()
		curTest = ""
		for line in lines:
			if line[0:13] == ">>> BEGINNING":
				curTest = line[14:]
			elif line[0:14] == "#----> Warning":
				warnings.append([curTest, line])
			elif line[0:14] == "#------> ERROR":
				errors.append([curTest, line])
			elif line[0:17] == "Assertion failed:":
				errors.append([curTest, line])
		if p.returncode != 0:
			errors.append([curTest, "%s%sProgram terminated with return code %d%s" % (BOLD, RED, p.returncode, RESET)])
		if len(errors) > 0 or len(warnings) > 0:
			ERRORSORWARNINGS[module+"."+test] = [errors, warnings]


def runTest(test, options):
        lines = list()
	makeTest(test, options)
        module = getModuleNameFromTestPath(test)
        testname = getTestNameFromTestPath(test)
	printRunningTestName(test)
	dir = getTestDir()
        os.chdir(dir+module+"/")
	cmd = [os.path.realpath(dir+"/bin/"+test+"_tests_TEST")]
	p = subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
        for line in iter(p.stdout.readline, b''):
                printOutputLine(line)   
                lines.append(str(line.rstrip()))
        stdout = p.communicate()
	errors = list()
	warnings = list()

	curTest = ""
	for line in lines:
		if line[0:13] == ">>> BEGINNING":
			curTest = line[14:]
		elif line[0:14] == "#----> Warning":
			warnings.append([curTest, line])
		elif line[0:14] == "#------> ERROR":
			errors.append([curTest, line])
		elif line[0:17] == "Assertion failed:":
			errors.append([curTest, line])
	if p.returncode != 0:
		errors.append([curTest, "%s%sProgram terminated with return code %d%s" % (BOLD, RED, p.returncode, RESET)])
	if len(errors) > 0 or len(warnings) > 0:
		ERRORSORWARNINGS[module+"."+test] = [errors, warnings]

def doesHaveErrors():
        hasError = False
        for item in ERRORSORWARNINGS:
                if len(ERRORSORWARNINGS[item][0]) > 0:
                        hasError = True
        return hasError

if __name__=='__main__':
	parser = OptionParser()
	parser.add_option("-r", "--release", action="store_const", const="release", dest="makeType")
	parser.add_option("-d", "--debug", action="store_const", const="debug", dest="makeType", default="debug")
	parser.add_option("-s", "--single", action="store_true", dest="single", default=False)
	parser.add_option("-n", "--nomake", action="store_true", dest="nomake", default=False)
        parser.add_option("-m", "--module", action="store_true", dest="module", default=False)
        parser.add_option("-i", "--integration", action="store_true", dest="integration", default=False)
        parser.add_option("-x", "--extra", action="append", dest="extraFlags", default=[])

	(options, args) = parser.parse_args()
        if options.integration == True:
                TEST_BASE_DIR = os.path.abspath(FILE_PATH+"/integration")+"/"
                TEST_TYPE = "integration"
	if options.nomake == False:
		makeProjectForTests(options)
        if options.module:
                if len(args) < 1:
                        print "Error, --module with no module specified"
                        exit(1)
                runAllTestsForModule(args[0], options)
	elif options.single:
		if len(args) < 1:
			print "Error, --single with no test specified!"
			exit(1)
		runTest(args[0], options)
	else:
		runAllTests(options)

        if doesHaveErrors() == False:
                print BOLD+GREEN+"\n####################################################"
                print "####  ALL TESTS SUCCESSFULLY PASSED :)  ####"
                print "####################################################"+RESET+"\n"
        else:
                print BOLD+RED+"\n####################################################"
                print "####  ERRORS ENCOUNTERED RUNNING TESTS :(  ####"
                print "####################################################"+RESET+"\n"
	for item in ERRORSORWARNINGS:
		print "%s%s--------------------- %s -------------------%s" % (BOLD, DARKGREY_BG, item, RESET)		
		cur = ""
                if len(ERRORSORWARNINGS[item][1]) > 0:
                        print BOLD+">>> "+YELLOW_BG+"Warnings:"+RESET
                        for warning in ERRORSORWARNINGS[item][1]:
                                if warning[0] != cur:
                                        cur = warning[0]
                                        print "%s%s    %s: %s" % (BOLD, UNDERLINED, cur, RESET)
                                print "%s%s    %s%s" % (BOLD, YELLOW, warning[1], RESET)
                cur = ""
                if len(ERRORSORWARNINGS[item][0]) > 0:
                        print BOLD+">>> "+RED_BG+"Errors:"+RESET
                        for error in ERRORSORWARNINGS[item][0]:
                                if error[0] != cur:
                                        cur = error[0]
                                        print "%s%s    %s: %s" % (BOLD, UNDERLINED, cur, RESET)
                                print "%s%s    %s%s" % (BOLD, RED, error[1], RESET)



