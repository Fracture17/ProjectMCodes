import os
import sys


def main():
    print(sys.argv)
    inputFilePath = sys.argv[1]
    textOutputPath = sys.argv[2]
    dataOutputPath = sys.argv[3]

    getText(inputFilePath, textOutputPath)
    getData(inputFilePath, dataOutputPath)


def getText(inputFilePath, textOutputPath):
    os.system(f"ppc-objcopy --only-section=.text {inputFilePath} {textOutputPath}")


def getData(inputFilePath, dataOutputPath):
    os.system(f"ppc-objcopy --remove-section=.text {inputFilePath} {dataOutputPath}")


main()