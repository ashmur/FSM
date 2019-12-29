import sys
import os
import shutil
from distutils.dir_util import copy_tree

#sys arguments structure:
#1 - source directory
#2 - destination directory
#3 - look in subdirectories
#4 - rebuild file system structure or just copy the files into one directory (dst)
#5 - delete src
#6 - name of the new directory
#7, 8, 9, ... - file extensions

#first step: validation of the input

numberOfMinimumArguments = 7
numberOfTotalArguments = len(sys.argv)

print(sys.argv)
if (len(sys.argv) < numberOfMinimumArguments):
    print("1") #1 means not sufficient arguments
    sys.exit()
elif (not os.path.isdir(sys.argv[1])):
    print("2") #2 means that the src is not a dir
    sys.exit()
elif (not os.path.isdir(sys.argv[2])):
    print("3") #3 means that the dst is not a dir
    sys.exit()
elif (os.path.isdir(sys.argv[2] + "\\" + sys.argv[6])):
    print("4") #4 means that dst's directory name already exists there
    sys.exit()
    
extensionsStartPosition = 7
src = sys.argv[1]
dst = sys.argv[2]
dstName = sys.argv[6]
dst = dst + "\\" + dstName
subdirectory = 0 if sys.argv[3] == '0' else 1
rebuild = 0 if sys.argv[4] == '0' else 1
deleteSrc = 0 if sys.argv[5] == '0' else 1
extensions = []
duplicates = []

if (numberOfTotalArguments==7):
    numberOfTotalArguments = numberOfTotalArguments
else:
    for i in range(extensionsStartPosition, numberOfTotalArguments):
        extensions.append(sys.argv[i])
def ignore_files_with_extensions(dirname, filenames):
    global extensions
    ignored = []
    for name in filenames:
        ignore = 1
        for i in range(0, len(extensions)):
            if (name.endswith(extensions[i])):
                ignore = 0
        #dirs don't have extension, nor simple files, so we ignore them
        if (name.find(".") == -1):
            ignore = 0
        if (ignore):
            ignored.append(name)
    return ignored
    
def subDirectoryMove():
    global src
    global extensions
    global dst
    global file
    global dubplicates
    for root, dirs, files in os.walk(src):
        for f in files:
            filename = os.path.join(root, f)
            if (numberOfTotalArguments==7):
                if (not os.path.isdir(filename)):
                    if (not os.path.isfile(dst + "\\" + f)):
                        shutil.copy2(filename, dst)
                    else:
                        duplicates.append(f)
            else:
                for i in range(0, len(extensions)):
                    if (filename.endswith(extensions[i])):
                        copy = 1
                        for copiedFile in os.listdir(dst):
                            if (f == copiedFile):
                                copy = 0
                                duplicates.append(f)
                        if(copy):
                            shutil.copy2(filename, dst)
                            break
                        
def nonSubDirectoryMove():
    global src
    global extensions
    global dst
    for file in os.listdir(src):
        filename = os.path.abspath(src + "\\" + file)
        if (numberOfTotalArguments==7):
            if (not os.path.isdir(filename)):
                shutil.copy2(filename, dst)
            else:
                continue
        else:
            for i in range(0, len(extensions)):
                if (filename.endswith(extensions[i])):
                    copy = 1
                    for copiedFile in os.listdir(dst):
                        if (file == copiedFile):
                            copy = 0
                    if(copy):
                        shutil.copy2(filename, dst)
                        break

def removeEmptyFolders(path, removeRoot=True):
    'Function to remove empty folders'
    if not os.path.isdir(path):
        return

    # remove empty subfolders
    files = os.listdir(path)
    if len(files):
        for f in files:
            fullpath = os.path.join(path, f)
            if os.path.isdir(fullpath):
                removeEmptyFolders(fullpath)

    # if folder empty, delete it
    files = os.listdir(path)
    if len(files) == 0 and removeRoot:
        os.rmdir(path)

def isNotDuplicate(filename):
    global duplicates
    for d in duplicates:
        if (d==filename):
            return False
    return True

def clean(src):
    global extensions
    global rebuild
    global subdirectory
    if (rebuild or subdirectory):
        for root, dirs, files in os.walk(src):
            for f in files:
                filename = os.path.join(root, f)
                if (numberOfTotalArguments==7):
                    if (isNotDuplicate(f)):
                        os.remove(filename)
                else:
                    for i in range(0, len(extensions)):
                        if (filename.endswith(extensions[i])):
                            if (isNotDuplicate(f)):
                                os.remove(filename)
                                continue
    else:
        for file in os.listdir(src):
            filename = os.path.abspath(src + "\\" + file)
            if (numberOfTotalArguments==7):
                if (isNotDuplicate(file)):
                    os.remove(filename)
            else:
                for i in range(0, len(extensions)):
                    if (filename.endswith(extensions[i])):
                        if (isNotDuplicate(file)):
                            os.remove(filename)
                            continue

if (deleteSrc):
    #we will delete the src after the copy, so mv is used
    if (rebuild):
        #always look in all subdirectories
        if (numberOfTotalArguments==7):
            shutil.copytree(src, dst)
        else:
            shutil.copytree(src, dst, ignore = ignore_files_with_extensions)
        #tree moved with the same structure
    else:
        os.mkdir(dst)
        if (subdirectory):
            subDirectoryMove()
            #tree moved into a single directory
        else:
            nonSubDirectoryMove()
            #files in current src directory moved into a single directory
            
    #clean moved files from src directory
    clean(src)
    removeEmptyFolders(src)
else:
    #we will NOT delete the src after the copy, so copy2 is used (thus preserving the metadata)
    if (rebuild):
        #always look in all subdirectories
        if (numberOfTotalArguments==7):
            shutil.copytree(src, dst)
        else:
            shutil.copytree(src, dst, ignore = ignore_files_with_extensions)
        #tree copied with the same structure
    else:
        os.mkdir(dst)
        if (subdirectory):
            subDirectoryMove()
            #tree copied into a single directory
        else:
            nonSubDirectoryMove()
            #files in current src directory copied into a single directory
            
print("0") #0 means success

if (len(duplicates)):
    file = open(dst + "\\FSM_SCRIPT_DUPLICATES.txt", "w")
    for d in duplicates:
        file.write(d + "\n")
    file.close()

sys.exit()