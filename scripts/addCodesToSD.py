import os
import shutil


baseSDPath = "Project+/codes"
codeFolder = R"C:\Users\johno\CLionProjects\ProjectMCodes\cmake-build-debug-mingw-powerpc\Output"
sdPath = R"C:\Users\johno\Documents\Project+v2.15_C++\sd.raw"
mountDrive = 'G'

os.system(Rf"osfmount.com -a -t file -f {sdPath} -m {mountDrive}: -o rw")

print()
for file in os.listdir(f"{mountDrive}:/{baseSDPath}"):
    try:
        os.remove(f"{mountDrive}:/{baseSDPath}/{file}")
        print("Removing", file)
    except Exception as e:
        print(e)
print()


for file in os.listdir(codeFolder):
    try:
        shutil.copy(f"{codeFolder}/{file}",
                    f"{mountDrive}:/{baseSDPath}/{file}")
        print("Adding", file)
    except Exception as e:
        print(e)
print()


os.system(f"osfmount.com -D -m {mountDrive}:")
