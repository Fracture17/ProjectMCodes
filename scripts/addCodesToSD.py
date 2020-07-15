import os
import shutil


codeFolder = R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output"

#os.system("osfmount.com -D -m G:")

os.system(R"osfmount.com -a -t file -f C:\Users\johno\Downloads\c++Testing.raw -m G: -o rw")


for file in os.listdir(codeFolder):
    try:
        shutil.copy(f"{codeFolder}/{file}",
                    f"G:/{file}")
        print(file)
    except Exception as e:
        print(e)


#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\a.bin",
            #R"G:/a.bin")
#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\b.bin",
            #R"G:/b.bin")
#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\c.bin",
            #R"G:/c.bin")
#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\d.bin",
            #R"G:/d.bin")
#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\e.bin",
            #R"G:/e.bin")
##shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\f.bin",
##            R"G:/f.bin")
#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\data.bin",
            #R"G:/data.bin")
#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\compressedInitializers",
            #R"G:/init.bin")
#shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\Setup.bin",
            #R"G:/setup.bin")
##shutil.copy(R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\output\initializers.bin",
##            R"G:/initializers.bin")



os.system("osfmount.com -D -m G:")

input()
