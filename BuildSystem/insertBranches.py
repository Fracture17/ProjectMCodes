import re
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


def insertBranches(nmFilePath, baseAddress, compressedCodesPath, finalCodePath):
    with open(compressedCodesPath, 'rb') as file:
        data = file.read()

        address2size, numInjections = getInjectionInfo(nmFilePath, baseAddress)
        #data = setReturnBranches(data, address2size, baseAddress, numInjections)

        initializerAddresses = getInitializerAddresses(nmFilePath)
        data = setInitializerBranches(data, numInjections, baseAddress, initializerAddresses)

    with open(finalCodePath, 'wb') as file:
        file.write(data)


def getInjectionInfo(nmFilePath, baseAddress):
    funcs = getAllFunctions(nmFilePath)

    address2size = {func.address: func.size for func in funcs}
    print(address2size)

    #first function is getInjections
    #8 bytes per inject, and 4 at the start
    numInjections = (address2size[baseAddress] - 4) // 8

    return address2size, numInjections


def getInitializerAddresses(nmFilePath):
    functions = getAllFunctions(nmFilePath)
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
    print(hex(functionOffset), hex(baseAddress))
    functionSize = address2size[functionAddress]
    nopAddress = functionAddress + functionSize - 4
    nopOffset = nopAddress - baseAddress

    addressToInjectAt = getInt(data, branchLocation + 4)
    #branch to the instruction after injection address
    branchInstruction = makeBranchInstruction(nopAddress, addressToInjectAt + 4)

    return branchInstruction, nopOffset


def setInitializerBranches(data, numInjections, baseAddress, initializerAddresses):
    for i, initAddress in enumerate(initializerAddresses):
        fileOffset = i * 4 + numInjections * 8 + 4
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
    maskedOffset = (2**26 - 1) & offset
    branchInstruction = maskedOffset | 0x48000001
    return branchInstruction


def getInt(data, offset):
    return int(data[offset: offset + 4].hex(), 16)


def replaceInt(data, offset, value):
    return data[:offset] + value.to_bytes(4, byteorder='big') + data[offset+4:]


