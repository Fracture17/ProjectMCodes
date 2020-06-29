import sys
import re
import os
from dataclasses import dataclass


@dataclass
class Function:
    address: int
    size: int
    name: str

    def __post_init__(self):
        if isinstance(self.address, str):
            self.address = int(self.address, 16)
        if isinstance(self.size, str):
            self.size = int(self.size, 16)


def main():
    nmFilePath = sys.argv[1]
    baseAddress = int(sys.argv[2], 16)
    compressedCodesPath = sys.argv[3]
    finalCodePath = sys.argv[4]

    print(sys.argv)

    insertBranches(nmFilePath, baseAddress, compressedCodesPath, finalCodePath)


def saveNM(inputFilePath, nmFilePath):
    os.system(f"ppc-nm -C -S -l -v {inputFilePath} > {nmFilePath}")


def insertBranches(nmFilePath, baseAddress, compressedCodesPath, finalCodePath):
    with open(compressedCodesPath, 'rb') as file:
        data = file.read()

        address2size, numInjections = getInjectionInfo(nmFilePath, baseAddress)
        data = setReturnBranches(data, address2size, baseAddress, numInjections)

        initializerAddresses = getInitializerAddresses(nmFilePath)
        data = setInitializerBranches(data, numInjections, baseAddress, initializerAddresses)

    with open(finalCodePath, 'wb') as file:
        file.write(data)


def getInjectionInfo(nmFilePath, baseAddress):
    funcs = getAllFunctions(nmFilePath)

    address2size = {func.address: func.size for func in funcs}

    #base address is first function address
    #baseAddress = funcs[0].address

    #first function is getInjections
    #8 bytes per inject, and 4 at the start
    print(address2size[baseAddress])
    numInjections = (address2size[baseAddress] - 4) // 8

    return address2size, numInjections


def getInitializerAddresses(nmFilePath):
    functions = getAllFunctions(nmFilePath)
    print(functions)
    initializers = [func for func in functions
                    if func.name.startswith('_GLOBAL__sub_I_')]
    initializerAddresses = [i.address for i in initializers]
    return initializerAddresses


def getAllFunctions(nmFilePath):
    with open(nmFilePath, 'r') as file:
        #8 char address, 8 char size, name
        funcs = re.findall(r"([0-9a-zA-Z]{8}) ([0-9a-zA-Z]{8}) . (.*)", file.read())
        funcs = [Function(*f) for f in funcs]
        return funcs


def setReturnBranches(data, address2size, baseAddress, numInjections):
    for i in range(numInjections):
        branchInstruction, branchOffset = makeReturnBranchInstruction(data, address2size, baseAddress, i)
        data = replaceInt(data, branchOffset, branchInstruction)

    return data


def makeReturnBranchInstruction(data, address2size, baseAddress, i):
    branchLocation = i * 8 + 4
    functionOffset = getBranchTargetOffset(data, branchLocation)
    functionAddress = functionOffset + baseAddress
    functionSize = address2size[functionAddress]
    nopAddress = functionAddress + functionSize - 4
    nopOffset = nopAddress - baseAddress

    addressToInjectAt = getInt(data, branchLocation + 4)
    #branch to the instruction after injection address
    branchInstruction = makeBranchInstruction(nopAddress, addressToInjectAt + 4)

    return branchInstruction, nopOffset


def setInitializerBranches(data, numInjections, baseAddress, initializerAddresses):
    print(hex(baseAddress))
    print(numInjections)
    for i, initAddress in enumerate(initializerAddresses):
        fileOffset = i * 4 + numInjections * 8 + 4
        print(fileOffset)
        branchAddress = baseAddress + fileOffset
        instruction = makeBranchLinkInstruction(branchAddress, initAddress)
        data = replaceInt(data, fileOffset, instruction)

    return data


def getBranchTargetOffset(data, offset):
    branch = getInt(data, offset)
    branchTarget = (branch & ~0xFC000003) + offset
    return branchTarget


def makeBranchInstruction(startAddress, targetAddress):
    offset = targetAddress - startAddress
    maskedOffset = (2**26 - 1) & offset
    branchInstruction = maskedOffset | 0x48000000
    return branchInstruction


def makeBranchLinkInstruction(startAddress, targetAddress):
    offset = targetAddress - startAddress
    print(hex(startAddress), hex(targetAddress), hex(offset), offset.bit_length())
    maskedOffset = (2**26 - 1) & offset
    branchInstruction = maskedOffset | 0x48000001
    return branchInstruction


def getInt(data, offset):
    return int(data[offset: offset + 4].hex(), 16)


def replaceInt(data, offset, value):
    return data[:offset] + value.to_bytes(4, byteorder='big') + data[offset+4:]


main()
