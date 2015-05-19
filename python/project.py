#!/user/bin/python

import os
import re

class CxFileTypes:
    kCppSource, kCppHeader, kGLSLFragmentShader, kGLSLVertexShader = range(4)

class CxScan:
    kScanIncludes = 1
    kScanClassesSimple = 2
    kScanClassesFull = 4
    kScanFunctions = 8
    kScanAll = 15
    

CX_FILE_EXTS = (".cpp", ".h", ".frag", ".vert")

class CxFile:
    def __init__(self, in_path, in_parent):
        self.path = os.path.abspath(in_path)
        self.name = os.path.basename(self.path)
        self.parent = in_parent
        
        self.isDir = False
        self.isSrc = False
        self.isHeader = False

        self.fileType = None

    def __str__(self):
        return self.name

    def __repr__(self):
        return str(self)

    @staticmethod
    def create(in_path, in_parent):
        if os.path.isdir(in_path):
            return CxDir(in_path, in_parent)
        elif in_path.endswith((".cpp", ".h")):
            return CxCppFile(in_path, in_parent)
        else:
            return CxFile(in_path, in_parent)
            

class CxCppInfo:
    kIncludeRegexp = re.compile(r"^[\s]*\#[\s]*include[\s]*(\"|\<)(.*)(\"|\>)")
    kClassesRegexp = re.compile(r"^.*class[\s]+([A-Za-z0-9_]+)([\s]*:[\s]*([A-Za-z0-9_]+))?")
    
    
    def __init__(self, in_cppFile):
        self.file = in_cppFile

        self._classes = None
        self._includes = None
        self._functions = None

    def includes(self):
        if self._includes is None:
            self._includes = []
            with open(self.file.path) as f:
                for line in f:
                    match = CxCppInfo.kIncludeRegexp.match(line)      
                    if not match is None: self._includes.append(match.group(2))
        return self._includes

    def classes(self):
        if self._classes is None:
            self._classes = []
            with open(self.file.path) as f:
                for line in f:
                    match = CxCppInfo.kClassesRegexp.match(line)      
                    if not match is None: self._classes.append(match.group(0))
        return self._classes


class CxCppFile(CxFile):
    
    def __init__(self, in_path, in_parent):
        CxFile.__init__(self, in_path, in_parent)
        self.isSrc = True

        self.info = CxCppInfo(self)


class CxDir(CxFile):
    def __init__(self, in_path, in_parent):
        CxFile.__init__(self, in_path, in_parent)
        self.isDir = True

        self.dirs = None
        self.files = None
        
        self.scanDir()


    def scanDir(self):
        if self.dirs is None: self.dirs = []
        if self.files is None: self.files = []
        
        for file in os.listdir(self.path):
            fullpath = os.path.join(self.path, file)
            if os.path.isdir(fullpath):
                self.dirs.append(CxFile.create(fullpath, self))
            elif file.endswith(CX_FILE_EXTS):
                self.files.append(CxFile.create(fullpath, self))

    def find(self, name):
        # First look at the files #
        for file in self.files:
            if file.name == name: return file
    
        # Then look at the directories #
        for dir in self.dirs:
            if dir.name == name: return dir

        # Then Go down into the directories if not found in this one #
        for dir in self.dirs:
            ret_val = dir.find(name)
            if not ret_val is None:  return ret_val
        
        return None
                


    
class CxModule:
    def __init__(self, in_srcDir, in_includeDir):
        self.name = in_includeDir.name
        self.src = in_srcDir
        self.include = in_includeDir

    def __str__(self):
        return "Module: " + self.name

    def __repr__(self):
        return str(self)

class CxProject:
    def __init__(self, in_name, in_projRoot, in_relSrcDir, in_relIncludeDir):
        self.name = in_name
        self.root = CxDir(in_projRoot, None)
        self.src = self.root.find(in_relSrcDir)
        self.include = self.root.find(in_relIncludeDir)

        # Get a list of the modules in the project #
        self.modules = []

        for idir in self.include.dirs:
            self.modules.append(CxModule(self.src.find(idir.name), idir))

    def __str__(self):
        return "Project: " + self.name + "[" + str(len(self.modules)) + "]"

    def __repr__(self):
        return str(self)
        
