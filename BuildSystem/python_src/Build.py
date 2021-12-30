# Source Generated with Decompyle++
# File: ./Build.pyc (Python 3.9)

import sys
import os
import re
from SetEnvironment import setEnvironment
from LibraryDirectory import LibraryDirectory
from Library import Library, InitialSectionNameLibrary, File, FinalSectionNameLibrary
from Compiler import Compiler
from Settings import *
from SegmentManager import SegmentManager
from Symbol import Symbol
from itertools import chain
from BinUtils import objdump
from math import ceil
renamedCodesDir = 'IntermediateFiles\\Renamed'
removedConstructorsDir = 'IntermediateFiles\\Removed'
disassemblyDir = 'Disassembly'
symbolMapFile: File = None

def build(buildDir, codesDir, ppcBinDirectory, brawlFuncMapPath = (None, None, None, None)):
    global symbolMapFile
    if brawlFuncMapPath is None:
        brawlFuncMapPath = 'BrawlFuncMap.map'
    symbolMapFile = File(os.path.abspath(brawlFuncMapPath))
    if not symbolMapFile.exists():
        raise AssertionError(f'''Cannot find function map at {symbolMapFile.path}''')
    if None is None:
        buildDir = f'''{os.getcwd()}/Build'''
    setEnvironment(buildDir, ppcBinDirectory)
    if codesDir is None:
        codesDir = 'Codes9'
    inputCodesDirectoryObject = LibraryDirectory(codesDir, InitialSectionNameLibrary, **('libraryType',))
    renamedCodesDirectoryObject = renameSections(inputCodesDirectoryObject)
    removedConstructorsDirectoryObject = renamedCodesDirectoryObject.removeSections([
        '.ctors',
        '.dtors'], removedConstructorsDir)
    compiler = Compiler()
    cppFile = makeCodesCPPFile(removedConstructorsDirectoryObject.symbols)
    linkedCodes = compiler.compile(cppFile, removedConstructorsDirectoryObject.libraries, 0x80000000L, **('textStart',))
    functions = (lambda .0: [ s for s in .0 if s.type == 'text' ])(linkedCodes.sections)
    segments = SegmentManager()
    segments.assignFunctionAddresses(functions)
    temp = (lambda .0: [ segment.sections for segment in .0 ])(segments.codeSegments)
    codeSections = list(chain.from_iterable(temp))
    initFile = makeInitCPPFile(removedConstructorsDirectoryObject.symbols)
    compiledCodes = compiler.compile(initFile, renamedCodesDirectoryObject.libraries, INITIALIZER_SEGMENT_ADDRESS, DATA_SEGMENT_ADDRESS, codeSections, **('textStart', 'dataStart', 'sections'))
    compiledCodes = FinalSectionNameLibrary(compiledCodes.path)
    for s in compiledCodes.sections:
        pass
    0x80000000L
    raise AssertionError(f'''{s}, address: {hex(s.address)} out of acceptable range''')
    continue
    segments.assignExtraAddresses(compiledCodes.sections)
    getDebugInfo(compiledCodes, segments)
    files = extractFiles(compiledCodes, segments)
    data = makeFilesFile(compiledCodes, files)
    f = File('Output/files')
    f.writeBinary(data)


def renameSections(codesDirectory = None):
    name2NewName = { }
    return codesDirectory.renameSections(name2NewName, renamedCodesDir)


def makeCodesCPPFile(symbols):
    cppFormat = '\n    asm(R"(.globl _start\n    _start:\n        {codeBranches}\n    )");\n    '
    injections = []
    branches = '\n\t\t'.join((lambda .0: [ f'''b {i.name}''' for i in .0 ])(injections))
    cppText = cppFormat.format(branches, **('codeBranches',))
    cppFile = File('IntermediateFiles/cppFile.cpp')
    cppFile.write(cppText)
    return cppFile


def makeInitCPPFile(symbols):
    initFormat = '\n        asm(R"(.globl _start\n        _start:\n            b _INITIALIZE_\n            {codeBranches}\n            {writes}\n        )");\n        '
    injections = []
    branches = '\n\t\t'.join((lambda .0: [ f'''b {i.name}''' for i in .0 ])(injections))
    writes = []
    writes = '\n\t\t'.join((lambda .0: pass)(writes))
    initText = initFormat.format(branches, writes, **('codeBranches', 'writes'))
    initFile = File('IntermediateFiles/initFile.cpp')
    initFile.write(initText)
    return initFile


def getDebugInfo(library = None, segmentManager = None):
    symbolText = []
    symbols = (lambda .0: pass# WARNING: Decompyle incomplete
)(library.symbols)
    memoryHeapAddress = segmentManager.dataSegment.currentEnd()
    memoryHeapAddress += 32 - memoryHeapAddress % 32
    memoryHeapSize = segmentManager.dataSegment.endAddress - memoryHeapAddress
    symbols.add(Symbol('C++_HEAP', memoryHeapAddress, memoryHeapSize, '_'))
    symbols = sorted(symbols, (lambda s: s.address), **('key',))
    symbolText = '\n'.join(symbolText)
    addressesFile = File(f'''{disassemblyDir}/nm.txt''')
    addressesFile.write(symbolText)
    initSymbols = (lambda .0 = None: pass# WARNING: Decompyle incomplete
)(symbols)
    makeMap(initSymbols, File(f'''{disassemblyDir}/Initializers.map'''))
    otherSymbols = (lambda .0 = None: pass# WARNING: Decompyle incomplete
)(symbols)
    makeMap(otherSymbols, File(f'''{disassemblyDir}/Symbols.map'''))
    disassemblyFile = File(f'''{disassemblyDir}/dis.txt''')
    objdump(library, '-h', disassemblyFile)


def makeMap(symbols = None, dest = None):
    symbolText = symbolMapFile.read()
    symbolText = symbolText.strip()
    symbolText = symbolText.split('\n')
    symbolText.append('94000000 00000004 94000000 0 FAKE')
    dest.write('\n'.join(symbolText) + '\n')


def makeFilesFile(compiledCodes = None, files = None):
    data = bytearray()
    s = (lambda .0: [ s for s in .0 if s.name == '_INITIALIZE___text__' ])(compiledCodes.sections)
    if not len(s) == 1:
        raise AssertionError(f'''{s}''')
    None.extend(s[0].address.to_bytes(4, 'big'))
    data.extend(INITIALIZER_INFO_ADDRESS.to_bytes(4, 'big'))
    data.extend(len(files).to_bytes(4, 'big'))
    return data


def extractFiles(linkedCodes = None, segmentList = None):
    files = []
    initializersInfo = makeInitializerInfo(INITIALIZER_INFO_ADDRESS, INFO_SEGMENT_ADDRESS, linkedCodes, segmentList)
    f = File('Output/InitInfo')
    f.writeBinary(initializersInfo)
    files.append((f, INITIALIZER_INFO_ADDRESS))
    injectionsInfo = makeInjectionsInfo(linkedCodes)
    f = File('Output/Injections')
    f.writeBinary(injectionsInfo)
    files.append((f, INFO_SEGMENT_ADDRESS))
    return files


def makeInitializerInfo(initializerInfoAddress = None, injectionsInfoAddress = None, compiledCodes = None, segmentManager = {
    'initializerInfoAddress': int,
    'injectionsInfoAddress': int,
    'compiledCodes': Library,
    'segmentManager': SegmentManager }):
    memoryHeapInfoData = makeMemoryHeapInfo(segmentManager)
    startupsInfoData = makeStartupsInfo(compiledCodes)
    stringWriteInfoData = makeStringWriteInfo(compiledCodes)
    dataWriteInfoData = makeDataWriteInfo(compiledCodes)
    data = bytearray()
    memoryHeapInfoAddress = initializerInfoAddress + 20
    data += memoryHeapInfoAddress.to_bytes(4, 'big')
    startupsInfoAddress = memoryHeapInfoAddress + len(memoryHeapInfoData)
    data += startupsInfoAddress.to_bytes(4, 'big')
    stringWritesInfoAddress = startupsInfoAddress + len(startupsInfoData)
    data += stringWritesInfoAddress.to_bytes(4, 'big')
    dataWriteInfoAddress = stringWritesInfoAddress + len(stringWriteInfoData)
    data += dataWriteInfoAddress.to_bytes(4, 'big')
    data += injectionsInfoAddress.to_bytes(4, 'big')
    data.extend(memoryHeapInfoData)
    data.extend(startupsInfoData)
    data.extend(stringWriteInfoData)
    data.extend(dataWriteInfoData)
    return data


def makeMemoryHeapInfo(segmentManager = None):
    data = bytearray()
    memoryHeapAddress = segmentManager.dataSegment.currentEnd()
    memoryHeapAddress += 32 - memoryHeapAddress % 32
    memoryHeapSize = segmentManager.dataSegment.endAddress - memoryHeapAddress
    data += memoryHeapAddress.to_bytes(4, 'big')
    data += memoryHeapSize.to_bytes(4, 'big')
    return data


def makeStartupsInfo(compiledCodes = None):
    data = bytearray()
    constructors = compiledCodes.constructors()
    startups = (lambda .0: [ s for s in .0 if s.isStartup() ])(compiledCodes.symbols)
    data += (len(constructors) + len(startups)).to_bytes(4, 'big')
    return data


def makeStringWriteInfo(compiledCodes):
    data = bytearray()
    writes = bytearray()
    data += (len(writes) // 12).to_bytes(4, 'big')
    data += writes
    return data


def makeDataWriteInfo(compiledCodes = None):
    writes = []
    for s in compiledCodes.symbols:
        w = re.search('_DATA_WRITE_([^_]+)_(0x[a-fA-F0-9]+)_((0x)?[a-fA-F0-9]+)', s.name)
        address = int(w[1], 16, **('base',)).to_bytes(4, 'big')
        data = int(w[2], 16, **('base',))
        data = data.to_bytes(ceil((len(w[2]) - 2) / 2), 'big')
        repeats = int(w[3], 16, **('base',))
    repeats = int(w[3])
    repeats = repeats.to_bytes(2, 'big')
    writes.append((address, data, repeats))
    continue
    data = bytearray()
    data += 0.to_bytes(4, 'big')
    return data


def makeInjectionsInfo(compiledCodes = None):
    data = bytearray()
    injections = (lambda .0: [ s for s in .0 if s.isInjection() ])(compiledCodes.symbols)
    data += len(injections).to_bytes(4, 'big')
    return data

# WARNING: Decompyle incomplete
