import subprocess
import os
import re
import shutil

from Settings import *
from Function import Function
from makeInjections import makeInjectionsFile
from makeFunctionMap import makeMap
from makeLoadSetup import makeLoadSetupFile
from makeSetup import makeSetupFile
from insertBranches import insertBranches


def build(baseDirectory, ppcBinDirectory):
    setPaths(baseDirectory, ppcBinDirectory)

    cleanFolders()

    buildInitializers()

    buildData()

    buildCodes()

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

    resetFolder(includedCodesFunctionListsDirectory)
    resetFolder(editedCodesDirectory)
    resetFolder(editedCodesFunctionListsDirectory)


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


#extracts the initializers code from the linked file and does everythign required to finish initializers
def buildInitializers():
    #get list of all functions from included code static libraries
    makeFunctionListsFromCodes(includedCodesDirectory, includedCodesFunctionListsDirectory)

    #make c++ file that references all injections
    makeInjectionsFile(includedCodesFunctionListsDirectory, initializersCPPPath, True)

    #compile c++ file
    initializersObjectPath = getObjectPath(initializersCPPPath)
    compile(compilerSettings, initializersCPPPath, initializersObjectPath)

    #link new object file with code libraries
    #linking and compiling are seperate because it makes it more portable
    initializerLibraries = ' '.join([f"{includedCodesDirectory}/{file}" for file in os.listdir(includedCodesDirectory)])

    initializersExtraLinkerSettings = f"-Ttext={hex(initializersStartAddress)} " + "--print-gc-sections" + " --no-gc-sections"
    link(linkerSettings, initializersObjectPath, initializersExtraLinkerSettings, initializerLibraries, linkedInitializersPath)

    #extract the initializer functions from the linked file
    os.system(f"{ppcObjCopy} --only-section=.text {linkedInitializersPath} {extractedInitializersPath}")

    #gets the function address info from extracted initializers
    makeNM(extractedInitializersPath, initializersAddressesPath)

    #creates dissasembly file from initializers
    makeDisassembly(linkedInitializersPath, initializersDisassemblyPath)

    #makes map file from initializers.  Does not include data info
    #TODO: add data addresses
    makeMap(initializersAddressesPath, initializersMapPath, brawlFunctionMapPath)

    #remove all debug info and puts all code in the correct positions
    compress(extractedInitializersPath, compressedInitializersPath)

    #old code, only used because it inserts the addresses of the contructor functions, which are not availible at link time
    insertBranches(initializersAddressesPath, initializersStartAddress, compressedInitializersPath, finalInitializersPath)


#extracts the data sections from the linked file and does everything required to finish data
def buildData():
    #isolate the data sections from the linked file
    os.system(f"{ppcObjCopy} --remove-section=.text {linkedInitializersPath} {extractedDataPath}")

    #get the address info from extracted data file
    #TODO: remove garbage symbols in beggining
    makeNM(extractedDataPath, dataAddressesPath)



    #remove all debug info and puts all data in the correct positions
    compress(extractedDataPath, finalDataPath)


def buildCodes():
    #each function's name begins with .text
    #to keep them seperate after linking, remove the .text
    #also remove the .ctors and .dtors so unesassarry code can be removed
    #do this for each code file
    for file in os.listdir(includedCodesDirectory):
        renameFunctionsAndRemoveConstructorsInCodeFile(file)

    #get function names from edited codes
    makeFunctionListsFromCodes(editedCodesDirectory, editedCodesFunctionListsDirectory, True)

    #make cpp file for codes
    makeInjectionsFile(editedCodesFunctionListsDirectory, codesCPPPath, False)

    #compile and link edited codes
    #this is the first time they are linked, which is purely to see which functions remain after being garbage collected
    #another link will be required to put the functions in specific addresses
    codesObjectPath = getObjectPath(codesCPPPath)
    compile(compilerSettings, codesCPPPath, codesObjectPath)

    codeLibraries = ' '.join([f"{editedCodesDirectory}/{file}" for file in os.listdir(editedCodesDirectory)])
    #Ttext can be pretty much anything, since it only affects some symbols in the map files in practice
    #Needs to be in the 0x80000000 area though, or it will make the linker use bctrl
    codesExtraLinkerSettings = f"-Ttext=0x81234568 " + "--print-gc-sections"
    link(linkerSettings, codesObjectPath, codesExtraLinkerSettings, codeLibraries, linkedCodesPath)

    #get list of remaining functions
    functionsText = getFunctionText(linkedCodesPath)

    #assign each function a place in the list of availible memory segments
    functions = [Function(name, -1, int(size, 16)) for name, size in functionsText]
    assignFunctionAddresses(functions)

    #create a list of linker commands that force each function to be in the assigned address
    functionAddressLinkerCommands = []
    for func in functions:
        command = f"--section-start={func.name}={hex(func.address)}"
        functionAddressLinkerCommands.append(command)
    functionAddressLinkerCommands = ' '.join(functionAddressLinkerCommands)

    #get the addresses of each data section
    text = subprocess.check_output(f"{ppcObjDump} -h {extractedDataPath}", shell=True)
    text = text.decode('utf-8')
    sectionsToForce = ['.rodata', '.data', '.bss', '.sbss']
    sectionsRegex = '(' + '|'.join(sectionsToForce) + ')' + " +[0-9a-f]{8}  ([0-9a-f]{8})"
    sections = re.findall(sectionsRegex, text)

    #turn the data section addresses into a linker command
    dataSectionsLinkerCommand = ''
    for name, address in sections:
        dataSectionsLinkerCommand += f"--section-start={name}={address} "

    #link the codes again but this time force each function to go to its assigned address
    codesExtraLinkerSettings = f"{functionAddressLinkerCommands} {dataSectionsLinkerCommand} -Ttext=0x81234568 --no-gc-sections"
    link(linkerSettings, codesObjectPath, codesExtraLinkerSettings, codeLibraries, linkedCodesPath)

    #make list of addresses, includes data addresses
    #TODO: Use sections instead because this is inaccurate
    makeNM(linkedCodesPath, codesAddressesPath)

    #make dissasembly file
    makeDisassembly(linkedCodesPath, f"{disassemblyDirectory}/codesDis.txt")

    #make map file, includes data addresses
    makeMap(codesAddressesPath, codesMapPath, brawlFunctionMapPath)


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

    dataSegment = CodeSegment(dataStartAddress, dataStartAddress)
    dataSegment.setName("Data")
    codeSegments.append(dataSegment)

    fileSegments = [s for s in codeSegments if s.name is not None]
    makeSetupFile(fileSegments, injectionInfo, startupFunctionAddress, setupCPPPath, setupHeaderPath)

    setupObjectPath = getObjectPath(setupCPPPath)
    compile(compilerSettings, setupCPPPath, setupObjectPath)
    #.rodata section should immediatly after .text section
    setupLinkerSettings = linkerSettings.replace(f"--section-start=.rodata={hex(dataStartAddress)}", "")

    setupExtraLinkerSettings = f"--no-gc-sections -Ttext={hex(setupStartAddress)}"
    link(setupLinkerSettings, setupObjectPath, setupExtraLinkerSettings, "", linkedSetupPath)

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


def makeFunctionListsFromCodes(codesDirectory, functionListsDirectory, x = False):
    for file in os.listdir(codesDirectory):
        makeNM(f"{codesDirectory}/{file}", f"{functionListsDirectory}/{file}.txt")


def compile(compilerArgs, cppFilePath, objPath):
    print(f"{ppcCompiler} -c {compilerArgs} {cppFilePath} -o {objPath} -save-temps=obj")
    os.system(f"{ppcCompiler} -c {compilerArgs} {cppFilePath} -o {objPath} -save-temps=obj")


def link(linkerArgs, objectFilePath, extraArgs, libraries, outputPath):
    print(f"{ppcLinker} {linkerArgs} {objectFilePath} {extraArgs} {libraries} {libraries} {libraries} -o {outputPath}")
    os.system(f"{ppcLinker} {linkerArgs} {objectFilePath} {extraArgs} {libraries} {libraries} {libraries} -o {outputPath}")


def renameFunctionsAndRemoveConstructorsInCodeFile(codeName):
    name2NewName = getNewFunctionNames(f"{includedCodesDirectory}/{codeName}")

    renames = []
    for name, newName in name2NewName.items():
        renames.append(f"--rename-section {name}={newName}")
    renames = ' '.join(renames)
    with open(renameFunctionsCommandFilePath, 'w') as file:
        file.write(renames)
    os.system(f"{ppcObjCopy} @{renameFunctionsCommandFilePath} --remove-section=.ctors --remove-section=.dtors {includedCodesDirectory}/{codeName} {editedCodesDirectory}/{codeName}")


#gets all functions in file, assuming that they are all their own section
def getFunctionText(file):
    data = subprocess.check_output(f"{ppcObjDump} -h {file}", shell=True)
    text = data.decode('utf-8')
    functions = re.findall(r"[0-9] ([a-zA-Z_][a-zA-Z0-9_]+) +([0-9a-f]{8})", text)
    return functions


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


def makeNM(source, dest):
    os.system(f"{ppcNM} -C -S -l -v {source} > {dest}")


def makeDisassembly(source, dest):
    os.system(f"{ppcObjDump} -C -S -l -h -M broadway {source} > {dest}")


def compress(source, dest):
    os.system(f"{ppcObjCopy} -O binary -S {source} {dest}")


def getNewFunctionNames(file):
    text = subprocess.check_output(f"{ppcObjDump} -h {file}", shell=True)
    text = text.decode('utf-8')
    textSegmentFunctions = re.findall(r"(\.text\.[a-zA-Z0-9_]+) *([0-9a-f]{8})", text)
    name2NewName = {}
    for func in textSegmentFunctions:
        name2NewName[func[0]] = func[0].replace('.text.', '')

    return name2NewName
