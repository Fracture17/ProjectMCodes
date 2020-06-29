import sys
import os

###
def main():
    inputFilePath = sys.argv[1]
    disassemblyFilePath = sys.argv[2]

    saveDisassembly(inputFilePath, disassemblyFilePath)


def saveDisassembly(inputFilePath, disassemblyFilePath):
    os.system(f"ppc-objdump -C -S -l -h -M broadway {inputFilePath} > {disassemblyFilePath}")


main()