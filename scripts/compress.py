import os
import sys

###
def main():
    inputFilePath = sys.argv[1]
    outputFilePath = sys.argv[2]

    strip(inputFilePath, outputFilePath)


def strip(inputFilePath, outputFilePath):
    os.system(f"ppc-objcopy -O binary -S {inputFilePath} {outputFilePath}")


main()