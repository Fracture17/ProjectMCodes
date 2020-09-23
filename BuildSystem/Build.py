import sys

from Main import build


def main():
    baseDirectory = sys.argv[1]
    ppcBinDirectory = sys.argv[2]

    build(baseDirectory, ppcBinDirectory)


main()
