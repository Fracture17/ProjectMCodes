import os
import shutil


os.system(R"osfmount.com -a -t file -f C:\Users\johno\Downloads\c++Testing.raw -m G: -o rw")

shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\codes.bin",
            R"G:/codes.bin")
shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\data.bin",
            R"G:/data.bin")
shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\initializers.bin",
            R"G:/initializers.bin")

os.system("osfmount.com -d -m G:")