import subprocess
import os
import re
import shutil

from Settings import *
from Function import Function
from makeInjections import makeInjectionsFile
from convertAddressesToLinkerCommands import convertAddressesToLinkerCommands
from makeFunctionMap import makeMap
from makeLoadSetup import makeLoadSetupFile
from makeSetup import makeSetupFile
from insertBranches import insertBranches



def makeFunctionListsFromCodes(codesDirectory, functionListsDirectory):
    for file in os.listdir(codesDirectory):
        makeNM(f"{codesDirectory}/{file}", f"{functionListsDirectory}/{file}.txt")


def compile(compilerArgs, cppFilePath, objPath):
    print(f"{ppcCompiler} -c {compilerArgs} {cppFilePath} -o {objPath} -save-temps=obj")
    os.system(f"{ppcCompiler} -c {compilerArgs} {cppFilePath} -o {objPath} -save-temps=obj")


def link(linkerArgs, objectFilePath, extraArgs, libraries, outputPath):
    print(f"{ppcLinker} {linkerArgs} {objectFilePath} {extraArgs} {libraries} {libraries} {libraries} -o {outputPath}")
    os.system(f"{ppcLinker} {linkerArgs} {objectFilePath} {extraArgs} {libraries} {libraries} {libraries} -o {outputPath}")


#extracts the data sections from the linked file and does everything required to finish data
def doDataStuff():
    #isolate the data sections from the linked file
    os.system(f"{ppcObjCopy} --remove-section=.text {linkedInitializersPath} {extractedDataPath}")

    #get the address info from extracted data file
    makeNM(extractedDataPath, dataAddressesPath)

    #make file to enforce data addresses for when the codes are linked
    convertAddressesToLinkerCommands(dataAddressesPath, dataAddressesFileForLinkerPath)

    #remove all debug info and puts all data in the correct positions
    compress(extractedDataPath, finalDataPath)


#extracts the initializers code from the linked file and does everythign required to finish initializers
def doInitializersStuff():
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


def renameFunctionsAndRemoveConstructorsInCodeFile(codeName):
    name2NewName = getNewFunctionNames(f"{includedCodesDirectory}/{codeName}")

    renames = []
    for name, newName in name2NewName.items():
        renames.append(f"--rename-section {name}={newName}")
    renames = ' '.join(renames)
    os.system(f"{ppcObjCopy} {renames} --remove-section=.ctors --remove-section=.dtors {includedCodesDirectory}/{codeName} {editedCodesDirectory}/{codeName}")


#gets all functions in file, assuming that they are all their own section
def getFunctionText(file):
    data = subprocess.check_output(f"{ppcObjDump} -h {file}", shell=True)
    text = data.decode('utf-8')
    functions = re.findall(r"[0-9] ([a-zA-Z_][a-zA-Z0-9_]+) +([0-9a-f]{8})", text)
    print(functions)
    return functions


def getObjectPath(sourcePath):
    objPath = sourcePath.partition('.')[0] + '.o'
    print(objPath)
    return objPath


def init(baseDirectory, ppcBinDirectory):
    os.chdir(baseDirectory)
    print(os.getcwd())

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


def build(baseDirectory, ppcBinDirectory):
    print(ppcObjCopy)
    init(baseDirectory, ppcBinDirectory)
    print(ppcObjCopy)

    resetFolder(intermediateFilesDirectory)
    resetFolder(outputDirectory)
    resetFolder(disassemblyDirectory)

    resetFolder(includedCodesFunctionListsDirectory)
    resetFolder(editedCodesDirectory)
    resetFolder(editedCodesFunctionListsDirectory)



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
    print(initializerLibraries)
    #changes files type to .o

    initializersExtraArgs = f"-Ttext={hex(initializersStartAddress)} " + "--print-gc-sections"
    print(initializersExtraArgs)
    link(linkerSettings, initializersObjectPath, initializersExtraArgs, initializerLibraries, linkedInitializersPath)



    doDataStuff()


    doInitializersStuff()




    #codes start


    #each function's name begins with .text
    #to keep them seperate after linking, remove the .text
    #also remove the .ctors and .dtors so unesassarry code can be removed
    #do this for each code file
    for file in os.listdir(includedCodesDirectory):
        renameFunctionsAndRemoveConstructorsInCodeFile(file)

    #get function names from edited codes
    makeFunctionListsFromCodes(editedCodesDirectory, editedCodesFunctionListsDirectory)

    #make cpp file for codes
    makeInjectionsFile(editedCodesFunctionListsDirectory, codesCPPPath, False)

    #compile and link edited codes
    #this is the first time they are linked, which is purely to see which functions remain after being garbage collected
    #another link will be required to put the functions in specific addresses
    codesObjectPath = getObjectPath(codesCPPPath)
    compile(compilerSettings, codesCPPPath, codesObjectPath)
    codeLibraries = ' '.join([f"{editedCodesDirectory}/{file}" for file in os.listdir(editedCodesDirectory)])
    print(codeLibraries)
    codesExtraArgs = f"@{dataAddressesFileForLinkerPath} -Ttext=0x81234568 " + "--print-gc-sections"
    link(linkerSettings, codesObjectPath, codesExtraArgs, codeLibraries, linkedCodesPath)


    #get list of remaining functions
    functionsText = getFunctionText(linkedCodesPath)

    #computeFunctionAddresses(linkedCodeFunctions, functionAddressesFilePath)

    #assign each function a place in the list of availible memory segments
    functions = [Function(name, -1, int(size, 16)) for name, size in functionsText]
    for func in functions:
        for segment in codeSegments:
            if segment.canInsert(func):
                func.address = segment.currentAddress
                segment.insertFunction(func)
                break
        else:
            raise Exception(f"{func=} Couldn't fit")

    """
    functionList = []
    functionsInSegment = [[] for _ in range(len(openCodeSegments))]
    for func in functions:
        for i, segment in enumerate(openCodeSegments):
            segmentSize = segment[1] - segment[0]
            functionSize = int(func[1], 16)
            if functionSize <= segmentSize:
                newFunc = Function(func[0], segment[0], functionSize)
                functionList.append(newFunc)
                segment[0] = segment[0] + functionSize
                functionsInSegment[i].append(newFunc)
                break
        else:
            raise Exception(f"{func=} Couldn't fit")

"""


    #create a list of linker commands that force each function to be in the assigned address
    functionAddressLinkerCommands = []
    for func in functions:
        command = f"--section-start={func.name}={hex(func.address)}"
        functionAddressLinkerCommands.append(command)
    functionAddressLinkerCommands = ' '.join(functionAddressLinkerCommands)


    #link the codes again but this time force each function to go to its assigned address
    codesExtraArgs = f"{functionAddressLinkerCommands} @{dataAddressesFileForLinkerPath} -Ttext=0x81234568 " + "--print-gc-sections"
    link(linkerSettings, codesObjectPath, codesExtraArgs, codeLibraries, linkedCodesPath)

    #make list of addresses, includes data addresses
    makeNM(linkedCodesPath, codesAddressesPath)

    #make dissasembly file
    makeDisassembly(linkedCodesPath, f"{disassemblyDirectory}/codesDis.txt")

    #make map file, includes data addresses
    makeMap(codesAddressesPath, codesMapPath, brawlFunctionMapPath)


    #start making setup file


    #for each segment with functions in it, give it a unique name and extract all the functions to a file
    for i, segment in enumerate(codeSegments):
        if segment.functions:
            segment.setName(chr(ord('A') + i))
            sections = []
            for func in segment.functions:
                sections.append(f"--only-section={func.name}")
            sections = ' '.join(sections)
            os.system(f"{ppcObjCopy} {sections} {linkedCodesPath} {segment.name}")
            compress(segment.name, f"{outputDirectory}/{segment.filename}")


    """    
    segmentInfo = []
    for i, functions in enumerate(functionsInSegment):
        if functions:
            name = chr(ord('a') + i)
            segmentInfo.append((name + '.bin', hex(startOfSections[i])))
            sections = []
            for func in functions:
                sections.append(f"--only-section={func.name}")
            sections = ' '.join(sections)
            os.system(f"{ppcObjCopy} {sections} {linkedCodesPath} {name}")
            compress(name, f"{outputDirectory}/{name}.bin")
"""



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




    with open(initializersAddressesPath, 'r') as file:
        text = file.read()
        startupFunctionInfo = re.findall(r"([0-9a-f]{8}) [0-9a-f]{8} [tT] __STARTUP_FUNCTIONS__", text)[0]
        print(startupFunctionInfo)
        startupFunctionAddress = hex(int(startupFunctionInfo, 16))


    initSegment = CodeSegment(initializersStartAddress, initializersStartAddress)
    initSegment.setName("Init")
    codeSegments.append(initSegment)

    dataSegment = CodeSegment(dataStartAddress, dataStartAddress)
    dataSegment.setName("Data")
    codeSegments.append(dataSegment)

    print(codeSegments)


    #segmentInfo.append(("init.bin", hex(0x80d00000)))
    #segmentInfo.append(("data.bin", hex(0x8057d000)))

    #print(segmentInfo, injectionInfo, startupFunctionAddress)
    makeSetupFile(codeSegments, injectionInfo, startupFunctionAddress, setupCPPPath, setupHeaderPath)



    setupObjectPath = getObjectPath(setupCPPPath)
    compile(compilerSettings, setupCPPPath, setupObjectPath)
    #os.system(f"{ppcCompiler} -c {compilerArgs} -g Setup.cpp")
    #links
    setupLinkerSettings = linkerSettings.replace(f"--section-start=.rodata=={hex(dataStartAddress)}", "")
    print(setupLinkerSettings)

    setupExtraSettings = f"--no-gc-sections -Ttext={hex(setupStartAddress)}"
    link(setupLinkerSettings, setupObjectPath, setupExtraSettings, "", linkedSetupPath)




    compress(linkedSetupPath, finalSetupPath)


    setupFileSize = os.path.getsize(finalSetupPath)
    print(setupFileSize)

    makeNM(linkedSetupPath, setupAddressesPath)
    with open(setupAddressesPath, "r") as file:
        text = file.read()
        setupFunctionAddress = re.findall(r"([0-9a-f]{8}) [0-9a-f]{8} T setup", text)[0]

    makeLoadSetupFile(loadSetupFilePath, hex(setupStartAddress))



    loadSetupObjectPath = getObjectPath(loadSetupFilePath)
    compile(compilerSettings, loadSetupFilePath, loadSetupObjectPath)
    #os.system(f"{ppcCompiler} -c {compilerArgs} LoadSetup.s")
    #links
    os.system(f"{ppcLinker} {linkerSettings} --no-gc-sections LoadSetup.o -o LoadSetup")
    loadSetupExtraSettings = "--no-gc-sections"
    link(linkerSettings, loadSetupObjectPath, loadSetupExtraSettings, "", linkedLoadSetupFilePath)

    os.system(f"{ppcObjCopy} -O binary -S --only-section=.text {linkedLoadSetupFilePath} {loadSetupBinaryPath}")


    with open(loadSetupBinaryPath, 'rb') as file:
        data = file.read()
        text = data.hex()

    with open(loadSetupASMPath, "w") as file:
        for i in range(0, len(text), 16):
            word1 = text[i: i + 8]
            word2 = text[i + 8: i + 16]
            file.write(f"* {word1.upper()} {word2.upper()}\n")



    makeDisassembly(linkedSetupPath, setupDisassemblyPath)
    makeMap(setupAddressesPath, setupMapPath, brawlFunctionMapPath)




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
