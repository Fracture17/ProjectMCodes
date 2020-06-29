import os
import sys
import re


def main():
    directory = sys.argv[1]
    cppArgs = sys.argv[2]
    linkerFlags = sys.argv[3]
    output = sys.argv[4]
    return

    files = [file for file in os.listdir() if file.endswith('.s')]

    #for file in files:
    #    makeLocalsGlobal(f"{directory}/{file}", file)

    files = [f"{directory}/{file}" for file in files]
    files = ' '.join(files)

    print(output)

    os.system(f"ppc-gcc {files} {cppArgs}  -o {output}")



def makeLocalsGlobal(path, fileName):
    with open(path, 'r') as file:
        text = file.read()
        #localLabelRegex = r"(^|[^L[a-zA-Z0-9_\.\"]|(\.))(L[a-zA-Z0-9_]+)"
        localLabelRegex = r"(\.)(L[a-zA-Z0-9_]+)"
        fileName = fixFileName(fileName)

        newText = re.sub(localLabelRegex, rf"__{fileName}__\2", text)

    with open(path, 'w') as file:
        file.write(newText)


def fixFileName(name: str):
    name, *_ = name.partition('.s')
    name = name.replace("+", "_PLUS_")
    return name


def separateAll(directory, outputFilePath):
    for file in os.listdir(directory):
        if file.endswith('.s'):
            pass


main()