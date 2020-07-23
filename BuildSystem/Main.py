import subprocess
import os
import re
import shutil

from Settings import *
from Function import Function
from Section import Section
from makeInjections import makeInjectionsFile
from makeFunctionMap import makeMap
from makeLoadSetup import makeLoadSetupFile
from makeSetup import makeSetupFile
from insertBranches import insertBranches


allSections = []
sectionsUsedByCodes = []
codeDataSectionSize = -1

def build(baseDirectory, ppcBinDirectory):
    setPaths(baseDirectory, ppcBinDirectory)

    cleanFolders()

    buildCodes()

    buildInitializers()

    buildData()

    buildSetupFile()

    buildLoadSetupFile()


def setPaths(baseDirectory, ppcBinDirectory):
    os.chdir(baseDirectory)

    global ppcObjCopy
    global ppcNM
    global ppcCompiler
    global ppcLinker
    global ppcObjDump
    ppcObjCopy = f"{ppcBinDirectory}/{ppcObjCopy}"
    ppcNM = f"{ppcBinDirectory}/{ppcNM}"
    ppcCompiler = f"{ppcBinDirectory}/{ppcCompiler}"
    ppcLinker = f"{ppcBinDirectory}/{ppcLinker}"
    ppcObjDump = f"{ppcBinDirectory}/{ppcObjDump}"


def cleanFolders():
    resetFolder(intermediateFilesDirectory)
    resetFolder(outputDirectory)
    resetFolder(disassemblyDirectory)

    resetFolder(renamedCodesCodesFunctionListsDirectory)
    resetFolder(renamedCodesDirectory)
    resetFolder(removedConstructorsCodesFunctionListsDirectory)
    resetFolder(removedConstructorsCodeDirectory)


def resetFolder(directory):
    try:
        clearFolder(directory)
    except:
        os.mkdir(directory)


def clearFolder(directory):
    for file in os.listdir(directory):
        try:
            shutil.rmtree(f"{directory}/{file}")
        except:
            os.remove(f"{directory}/{file}")


def buildCodes():
    global allSections
    global sectionsUsedByCodes

    #each function's name begins with .text
    #to keep them seperate after linking, remove the .text
    #also remove the .ctors and .dtors so unesassarry code can be removed
    #do this for each code file
    for file in os.listdir(includedCodesDirectory):
        sections = renameSectionsAndRemoveConstructorsForCodeFile(file)
        allSections.extend(sections)
    allSections = [Section(name, -1, int(size, 16), type) for name, type, size in allSections]

    #get function names from edited codes
    makeFunctionListsFromCodes(removedConstructorsCodeDirectory, removedConstructorsCodesFunctionListsDirectory)

    #make cpp file for codes
    makeInjectionsFile(removedConstructorsCodesFunctionListsDirectory, codesCPPPath, False)

    #compile and link edited codes
    #this is the first time they are linked, which is purely to see which functions remain after being garbage collected
    #another link will be required to put the functions in specific addresses
    codesObjectPath = getObjectPath(codesCPPPath)
    compile(compilerSettings, codesCPPPath, codesObjectPath)

    codeLibraries = ' '.join([f"{removedConstructorsCodeDirectory}/{file}" for file in os.listdir(removedConstructorsCodeDirectory)])
    #Ttext can be pretty much anything, since it only affects some symbols in the map files in practice
    #Needs to be in the 0x80000000 area though, or it will make the linker use bctrl
    codesExtraLinkerSettings = "-Ttext=0x81234568 "
    link(linkerSettings, codesObjectPath, codesExtraLinkerSettings, codeLibraries, linkedCodesPath)

    #get list of remaining functions
    sectionsText, unnamedDaatSections = getSectionsText(linkedCodesPath)
    sectionsUsedByCodes = [s for s in allSections if s.name in sectionsText]

    #assign each function a place in the list of availible memory segments
    functions = [s for s in sectionsUsedByCodes if s.type == '.text']
    data = [s for s in sectionsUsedByCodes if s.type != '.text']

    assignFunctionAddresses(functions)
    assignDataSections(data, unnamedDaatSections)

    #create a list of linker commands that force each function to be in the assigned address
    sectionAddressLinkerCommands = []
    for s in sectionsUsedByCodes:
        command = f"--section-start={s.name}={hex(s.address)}"
        sectionAddressLinkerCommands.append(command)
    sectionAddressLinkerCommands = ' '.join(sectionAddressLinkerCommands)

    with open(setCodeSectionAddressesLinkerCommandsFilePath, 'w') as file:
        file.write(sectionAddressLinkerCommands)

    #link the codes again but this time force each function to go to its assigned address
    codesExtraLinkerSettings = f"@{setCodeSectionAddressesLinkerCommandsFilePath} -Ttext=0x81234568 --section-start=.rodata={hex(dataStartAddress)}"
    link(linkerSettings, codesObjectPath, codesExtraLinkerSettings, codeLibraries, linkedCodesPath)

    #extract local data from code file
    os.system(f"{ppcObjCopy} --only-section=.rodata {linkedCodesPath} {extractedCodeDataPath}")

    #compress local code data
    compress(extractedCodeDataPath, finalCodeDataPath)

    #make list of addresses, includes data addresses
    makeNM(linkedCodesPath, codesAddressesPath)

    #make dissasembly file
    makeDisassembly(linkedCodesPath, f"{disassemblyDirectory}/codesDis.txt")

    #make map file, includes data addresses
    makeMap(codesAddressesPath, codesMapPath, brawlFunctionMapPath)


#extracts the initializers code from the linked file and does everythign required to finish initializers
def buildInitializers():
    #get list of all functions from included code static libraries
    makeFunctionListsFromCodes(renamedCodesDirectory, renamedCodesCodesFunctionListsDirectory)

    #make c++ file that references all injections
    makeInjectionsFile(renamedCodesCodesFunctionListsDirectory, initializersCPPPath, True)

    #compile c++ file
    initializersObjectPath = getObjectPath(initializersCPPPath)
    compile(compilerSettings, initializersCPPPath, initializersObjectPath)

    #link new object file with code libraries
    #linking and compiling are seperate because it makes it more portable
    initializerLibraries = ' '.join([f"{renamedCodesDirectory}/{file}" for file in os.listdir(renamedCodesDirectory)])

    #take all the data sections from codes, and force them to be in the same place
    dataSections = [s for s in sectionsUsedByCodes if s.type != '.text']
    sectionAddressLinkerCommands = []
    for s in dataSections:
        command = f"--section-start={s.name}={hex(s.address)}"
        sectionAddressLinkerCommands.append(command)
    sectionAddressLinkerCommands = ' '.join(sectionAddressLinkerCommands)

    with open(setInitializerDataAddressesLinkerCommandsFilePath, 'w') as file:
        file.write(sectionAddressLinkerCommands)

    print(codeDataSectionSize)
    initializersExtraLinkerSettings = f"@{setInitializerDataAddressesLinkerCommandsFilePath} -Ttext={hex(initializersStartAddress)} --section-start=.rodata={hex(dataStartAddress + codeDataSectionSize)}"
    link(linkerSettings, initializersObjectPath, initializersExtraLinkerSettings, initializerLibraries, linkedInitializersPath)

    #extract the initializer functions from the linked file
    textSectionsLinkerCommands = '--only-section=.text'
    for s in allSections:
        if s.type == '.text':
            textSectionsLinkerCommands += f" --only-section={s.name}"

    with open(extractSectionsLinkerCommandsFilePath, 'w') as file:
        file.write(textSectionsLinkerCommands)

    os.system(f"{ppcObjCopy} @{extractSectionsLinkerCommandsFilePath} {linkedInitializersPath} {extractedInitializersPath}")

    #gets the function address info from extracted initializers
    makeNM(extractedInitializersPath, initializersAddressesPath)

    #creates dissasembly file from initializers
    makeDisassembly(linkedInitializersPath, initializersDisassemblyPath)

    #makes map file from initializers.  Does not include data info
    #TODO: add data addresses and remove garbage symbols from begining
    makeMap(initializersAddressesPath, initializersMapPath, brawlFunctionMapPath)

    #remove all debug info and puts all code in the correct positions
    compress(extractedInitializersPath, compressedInitializersPath)

    #old code, only used because it inserts the addresses of the contructor functions, which are not availible at link time
    insertBranches(initializersAddressesPath, initializersStartAddress, compressedInitializersPath, finalInitializersPath)


#extracts the data sections from the linked file and does everything required to finish data
def buildData():
    #isolate the data sections from the linked file
    textSectionsLinkerCommands = ''
    for s in ['.rodata', '.bss', '.sbss', '.data', '.sdata']:
        textSectionsLinkerCommands += f" --only-section={s}"
    for s in allSections:
        if s.type != '.text':
            textSectionsLinkerCommands += f" --only-section={s.name}"

    with open(extractSectionsLinkerCommandsFilePath, 'w') as file:
        file.write(textSectionsLinkerCommands)

    os.system(f"{ppcObjCopy} @{extractSectionsLinkerCommandsFilePath} {linkedInitializersPath} {extractedDataPath}")

    #get the address info from extracted data file
    makeNM(extractedDataPath, dataAddressesPath)

    #remove all debug info and puts all data in the correct positions
    compress(extractedDataPath, finalDataPath)


#The setup file loads the data, initializers, and code files, and calls the initializer's startup function
def buildSetupFile():
    #for each segment with functions assigned to it, give it a unique name and extract all the functions to a file
    for i, segment in enumerate(codeSegments):
        if segment.functions:
            segment.setName(chr(ord('A') + i))
            sections = []
            for func in segment.functions:
                sections.append(f"--only-section={func.name}")
            sections = ' '.join(sections)
            os.system(f"{ppcObjCopy} {sections} {linkedCodesPath} {intermediateFilesDirectory}/{segment.name}")
            compress(f"{intermediateFilesDirectory}/{segment.name}", f"{outputDirectory}/{segment.filename}")

    #get a list of all injections, and make the branch info
    with open(codesAddressesPath, 'r') as file:
        injectionInfo = []
        text = file.read()
        injectionRegex = r"([0-9a-f]{8}) ([0-9a-f]{8}) [tT] [a-zA-Z0-9_]*_INJECTION_((0x)?[0-9a-fA-F]{8})"
        injections = re.findall(injectionRegex, text)
        for injection in injections:
            injectedFunctionAddress, size, injectionAddress, *_ = injection
            injectedFunctionAddress = hex(int(injectedFunctionAddress, 16))
            returnBranchAddress = int(size, 16) + int(injectedFunctionAddress, 16) - 4
            returnBranchAddress = hex(returnBranchAddress)
            injectionInfo.append((injectionAddress, injectedFunctionAddress, returnBranchAddress))

    #find the address of the startup function in initializers
    with open(initializersAddressesPath, 'r') as file:
        text = file.read()
        startupFunctionInfo = re.findall(r"([0-9a-f]{8}) [0-9a-f]{8} [tT] __STARTUP_FUNCTIONS__", text)[0]
        startupFunctionAddress = hex(int(startupFunctionInfo, 16))

    #add the initializer and data files to the setup list
    initSegment = CodeSegment(initializersStartAddress, initializersStartAddress)
    initSegment.setName("Init")
    codeSegments.append(initSegment)

    #objcopy doesn't add leading 0s to data file
    #add offset of when it does start and add to load address
    loadedSectionRegex = r" +[0-9]+ +[\.a-zA-Z0-9_]+ +[0-9a-f]{8}  +[0-9a-f]{8}  +([0-9a-f]{8}).+\n.+LOAD"
    text = subprocess.check_output(f"{ppcObjDump} -h {extractedDataPath}", shell=True)
    text = text.decode('utf-8')
    try:
        firstLoadedSectionAddress = int(re.findall(loadedSectionRegex, text)[0], 16)
    except:
        print("NO DATA")
        firstLoadedSectionAddress = dataStartAddress
    print(hex(firstLoadedSectionAddress))

    dataSegment = CodeSegment(firstLoadedSectionAddress, firstLoadedSectionAddress)
    dataSegment.setName("Data")
    codeSegments.append(dataSegment)

    if firstLoadedSectionAddress != dataStartAddress:
        codeDataSegment = CodeSegment(dataStartAddress, dataStartAddress)
        codeDataSegment.setName("CData")
        codeSegments.append(codeDataSegment)

    fileSegments = [s for s in codeSegments if s.name is not None]
    makeSetupFile(fileSegments, injectionInfo, startupFunctionAddress, setupCPPPath, setupHeaderPath)

    setupObjectPath = getObjectPath(setupCPPPath)
    compile(compilerSettings, setupCPPPath, setupObjectPath)
    setupExtraLinkerSettings = f"--no-gc-sections -Ttext={hex(setupStartAddress)}"
    link(linkerSettings, setupObjectPath, setupExtraLinkerSettings, "", linkedSetupPath)

    compress(linkedSetupPath, finalSetupPath)

    makeNM(linkedSetupPath, setupAddressesPath)

    makeDisassembly(linkedSetupPath, setupDisassemblyPath)
    makeMap(setupAddressesPath, setupMapPath, brawlFunctionMapPath)


#LoadSetupFile loads the setup file and calls its setup function
#it should be added to the standard gct
def buildLoadSetupFile():
    makeLoadSetupFile(loadSetupFilePath, hex(setupStartAddress))

    loadSetupObjectPath = getObjectPath(loadSetupFilePath)
    compile(compilerSettings, loadSetupFilePath, loadSetupObjectPath)

    loadSetupExtraLinkerSettings = "--no-gc-sections"
    link(linkerSettings, loadSetupObjectPath, loadSetupExtraLinkerSettings, "", linkedLoadSetupFilePath)

    os.system(f"{ppcObjCopy} -O binary -S --only-section=.text {linkedLoadSetupFilePath} {loadSetupBinaryPath}")

    #convert binary to text hex to be added to gct
    with open(loadSetupBinaryPath, 'rb') as file:
        data = file.read()
        text = data.hex()

    with open(loadSetupASMPath, "w") as file:
        for i in range(0, len(text), 16):
            word1 = text[i: i + 8]
            word2 = text[i + 8: i + 16]
            file.write(f"* {word1.upper()} {word2.upper()}\n")


def makeFunctionListsFromCodes(codesDirectory, functionListsDirectory):
    for file in os.listdir(codesDirectory):
        makeNM(f"{codesDirectory}/{file}", f"{functionListsDirectory}/{file}.txt")


def compile(compilerArgs, cppFilePath, objPath):
    #print(f"{ppcCompiler} -c {compilerArgs} {cppFilePath} -o {objPath} -save-temps=obj")
    os.system(f"{ppcCompiler} -c {compilerArgs} {cppFilePath} -o {objPath} -save-temps=obj")


def link(linkerArgs, objectFilePath, extraArgs, libraries, outputPath):
    #print(f"{ppcLinker} {linkerArgs} {objectFilePath} {extraArgs} {libraries} {libraries} {libraries} -o {outputPath}")
    os.system(f"{ppcLinker} {linkerArgs} {objectFilePath} {extraArgs} {libraries} {libraries} {libraries} -o {outputPath}")


def renameSectionsAndRemoveConstructorsForCodeFile(codeName):
    name2NewSectionName = getNewSectionNames(f"{includedCodesDirectory}/{codeName}")

    renames = []
    for name, newName in name2NewSectionName.items():
        renames.append(f"--rename-section {name}={newName[0]}")
    renames = ' '.join(renames)
    with open(renameFunctionsCommandFilePath, 'w') as file:
        file.write(renames)
    os.system(f"{ppcObjCopy} @{renameFunctionsCommandFilePath} {includedCodesDirectory}/{codeName} {renamedCodesDirectory}/{codeName}")
    os.system(f"{ppcObjCopy} --remove-section=.ctors --remove-section=.dtors {renamedCodesDirectory}/{codeName} {removedConstructorsCodeDirectory}/{codeName}")

    return list(name2NewSectionName.values())


#gets all functions in file, assuming that they are all their own section
def getSectionsText(file):
    data = subprocess.check_output(f"{ppcObjDump} -h {file}", shell=True)
    text = data.decode('utf-8')
    functions = re.findall(r"[0-9]+ ([a-zA-Z_][a-zA-Z0-9_]+) +[0-9a-f]{8}", text)
    unnamedDataSections = re.findall(r"(\.rodata) +([0-9a-f]{8})", text)
    return functions, unnamedDataSections


def getObjectPath(sourcePath):
    objPath = sourcePath.partition('.')[0] + '.o'
    return objPath


def assignFunctionAddresses(functions):
    for func in functions:
        for segment in codeSegments:
            if segment.canInsert(func):
                func.address = segment.currentAddress
                segment.insertFunction(func)
                break
        else:
            raise Exception(f"{func=} Couldn't fit")


def assignDataSections(data, unnamedDaatSections):
    address = dataStartAddress
    for s in unnamedDaatSections:
        address += int(s[1], 16)

    for d in data:
        d.address = address
        address += d.size
        #align by 4
        address = ((address + 3) // 4) * 4
    global codeDataSectionSize
    codeDataSectionSize = address - dataStartAddress


def makeNM(source, dest):
    os.system(f"{ppcNM} -C -S -l -v {source} > {dest}")


def makeDisassembly(source, dest):
    os.system(f"{ppcObjDump} -C -S -l -h -M broadway {source} > {dest}")


def compress(source, dest):
    os.system(f"{ppcObjCopy} -O binary -S {source} {dest}")


def getNewSectionNames(file):
    text = subprocess.check_output(f"{ppcObjDump} -h {file}", shell=True)
    text = text.decode('utf-8')
    sections = re.findall(r"((\.text|\.rodata|\.sbss|\.bss|\.data|\.sdata)\.[a-zA-Z0-9_]+) *([0-9a-f]{8})", text)
    name2NewName = {}
    for section in sections:
        name2NewName[section[0]] = (section[0].replace(section[1] + '.', ''), section[1], section[2])
    return name2NewName


def getNewDataNames(file):
    text = subprocess.check_output(f"{ppcObjDump} -h {file}", shell=True)
    text = text.decode('utf-8')
    dataSections = re.findall(r"((\.rodata|\.sbss|\.bss|\.data|\.sdata)\.[a-zA-Z0-9_]+) *([0-9a-f]{8})", text)
    name2NewName = {}
    for section in dataSections:
        name2NewName[section[0]] = (section[0].replace(section[1] + '.', ''), section[1], section[2])
    return name2NewName
