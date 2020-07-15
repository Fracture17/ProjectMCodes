import subprocess
import os
import sys
import re
from dataclasses import dataclass

from Paths import *
from makeInjections import makeInjectionsFile
from convertAddressesToLinkerCommands import convertAddressesToLinkerCommands
from makeFunctionMap import makeMap
from makeLoadSetup import makeLoadSetupFile
from makeSetup import makeSetupFile
from insertBranches import insertBranches


@dataclass
class Function:
    name: str
    address: int
    size: int



def main():
    baseDirectory = sys.argv[1]
    compilerArgs = sys.argv[2]
    linkerArgs = sys.argv[3]
    linkerArgs = linkerArgs.replace('-Wl,', '')
    ppcBinDirectory = sys.argv[4]
    initializersStartAddress = sys.argv[5]
    openCodeSegments = sys.argv[6]
    openCodeSegments = openCodeSegments.split(',')
    openCodeSegments = [segment.split() for segment in openCodeSegments]
    openCodeSegments = [[int(segment[0], 16), int(segment[1], 16)] for segment in openCodeSegments]
    startOfSections = [x[0] for x in openCodeSegments]






    init(baseDirectory, ppcBinDirectory)


    resetFolder(preLinkedInitializersNMDirectory)
    resetFolder(editedCodesNMDirectory)
    resetFolder(editedCodesDirectory)

    #makePreLinkedNMs(compiledCodesDirectory, preLinkedInitializersNMDirectory)
    for file in os.listdir(compiledCodesDirectory):
        makeNM(f"{compiledCodesDirectory}/{file}", f"{preLinkedInitializersNMDirectory}/{file}.txt")
        #os.system(f"{ppcNM} -C -S -l -v {compiledCodesDirectory}/{file} {preLinkedInitializersNMDirectory}/{file}.txt")



    makeInjectionsFile(preLinkedInitializersNMDirectory, initializersCPPFilePath, True)




    #compileInitializers(initializersCPPFilePath, compiledCodesDirectory, compilerArgs, linkerArgs, linkedInitializersPath)
    #compiles
    os.system(f"{ppcCompiler} -c {compilerArgs} {initializersCPPFilePath}")
    #links
    libraries = ' '.join([f"{compiledCodesDirectory}/{file}" for file in os.listdir(compiledCodesDirectory)])
    print(libraries)
    os.system(f"{ppcLinker} {linkerArgs} Initializers.o -Ttext={initializersStartAddress} --print-gc-sections {libraries} {libraries} -o {linkedInitializersPath}")



    #extractData(linkedInitializersPath, extractedDataPath)
    os.system(f"{ppcObjCopy} --remove-section=.text {linkedInitializersPath} {extractedDataPath}")



    makeNM(extractedDataPath, dataNMPath)





    convertAddressesToLinkerCommands(dataNMPath, dataAddressesFilePath)


    compress(extractedDataPath, finalData)









    #extractInitializers(linkedInitializersPath, extractedInitializersPath)
    os.system(f"{ppcObjCopy} --only-section=.text {linkedInitializersPath} {extractedInitializersPath}")



    makeNM(extractedInitializersPath, initializersNMPath)



    makeDisassembly(linkedInitializersPath, initializersDisassemblyPath)
#    os.system(f"{ppcObjDump} -C -S -l -h -M broadway {linkedInitializersPath} > {initializersDisassemblyPath}")

    makeMap(initializersNMPath, f"{disassemblyDirectory}/initializers.map", "../BrawlFuncMap.map")


    #makeFunctionMap(extractedInitializersPath, baseFunctionMapPAth, initializersFunctionMapPath)



    compress(extractedInitializersPath, compressedInitializersPath)

    insertBranches(initializersNMPath, int(initializersStartAddress, 16), compressedInitializersPath, finalInitializersPath)



    #codes start



    for file in os.listdir(compiledCodesDirectory):
        name2NewName = getNewFunctionNames(f"{compiledCodesDirectory}/{file}")


        #removeConstructorsAndRenameFunctions(compiledCodesDirectory, newFunctionNamesFilePath, editedCodesDirectory)
        renames = []
        for name, newName in name2NewName.items():
            renames.append(f"--rename-section {name}={newName}")
        renames = ' '.join(renames)
        os.system(f"{ppcObjCopy} {renames} --remove-section=.ctors --remove-section=.dtors {compiledCodesDirectory}/{file} {editedCodesDirectory}/{file}")



    #makePreLinkedNMs(editedCodesDirectory, editedCodesNMDirectory)
    for file in os.listdir(editedCodesDirectory):
        makeNM(f"{editedCodesDirectory}/{file}", f"{editedCodesNMDirectory}/{file}.txt")


    #createCodesCPPFile(editedCodesNMDirectory, codesCPPFilePath)
    makeInjectionsFile(editedCodesNMDirectory, codesCPPFilePath, False)




    #compileRelocatableCodes(codesCPPFilePath, editedCodesDirectory, compilerArgs, linkerArgs, dataAddressesFilePath, linkedCodesPath)
    #compiles
    os.system(f"{ppcCompiler} -c {compilerArgs} {codesCPPFilePath}")
    #links
    libraries = ' '.join([f"{editedCodesDirectory}/{file}" for file in os.listdir(editedCodesDirectory)])
    #libraries = "EditedCodes/TEST_PRINT " + libraries
    print(libraries)
    os.system(f"{ppcLinker} {linkerArgs} codes.o {libraries} {libraries} {libraries} @{dataAddressesFilePath} --print-gc-sections -Ttext=0x80000000 -o {linkedCodesPath}")




    #getFunctions(linkedCodesPath, linkedCodeFunctionsPath)
    os.system(f"{ppcObjDump} -h {linkedCodesPath} > test.txt")
    with open('test.txt', 'r') as file:
        text = file.read()
        functions = re.findall(r"[0-9] ([a-zA-Z_][a-zA-Z0-9_]+) +([0-9a-f]{8})", text)
        print(functions)
    
    print(openCodeSegments)


    #computeFunctionAddresses(linkedCodeFunctions, functionAddressesFilePath)
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




    functionAddressLinkerCommands = []
    for func in functionList:
        command = f"--section-start={func.name}={hex(func.address)}"
        functionAddressLinkerCommands.append(command)
    functionAddressLinkerCommands = ' '.join(functionAddressLinkerCommands)


    #relink(linkedCodesPath, linkerArgs, functionAddressesFilePath, linkedCodesPath2)
    #compiles
    #Dont need to compile again, only linking is differenmt
    #os.system(f"{ppcCompiler} -c {compilerArgs} {codesCPPFilePath}")
    #links
    libraries = ' '.join([f"{editedCodesDirectory}/{file}" for file in os.listdir(editedCodesDirectory)])
    os.system(f"{ppcLinker} {linkerArgs} codes.o {libraries} {libraries} {libraries} {functionAddressLinkerCommands} @{dataAddressesFilePath} --print-gc-sections  -Ttext=0x80000000 -o {linkedCodesPath}")




    makeNM(linkedCodesPath, "CodesNM.txt")
    makeDisassembly(linkedCodesPath, f"{disassemblyDirectory}/codesDis.txt")

    makeMap("CodesNM.txt", f"{disassemblyDirectory}/codeMap.map", "../BrawlFuncMap.map")


    #makeDisassembly(linkedCodesPath2, codesDissasemblyPath)
#
    #makeFunctionMap(linkedCodesPath2, baseFunctionMapPath, codesMap)
    #makeMap("CodesNM.txt", f"{disassemblyDirectory}/CPPFuncMap.map", "../BrawlFuncMap.map")


    #extract a bunch of stuff
    #extractCodes(linkedCodesPath2, functionAddressesFilePath, codesPath)
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




    with open("CodesNM.txt", 'r') as file:
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




    with open(initializersNMPath, 'r') as file:
        text = file.read()
        startupFunctionInfo = re.findall(r"([0-9a-f]{8}) [0-9a-f]{8} [tT] __STARTUP_FUNCTIONS__", text)[0]
        print(startupFunctionInfo)
        startupFunctionAddress = hex(int(startupFunctionInfo, 16))



    segmentInfo.append(("init.bin", hex(0x80d00000)))
    segmentInfo.append(("data.bin", hex(0x8057d000)))

    print(segmentInfo, injectionInfo, startupFunctionAddress)
    makeSetupFile(segmentInfo, injectionInfo, startupFunctionAddress)


    os.system(f"{ppcCompiler} -c {compilerArgs} -g Setup.cpp")
    #links
    args = linkerArgs.replace("--section-start=.rodata=0x8057d000", "")
    print(args)
    os.system(f"{ppcLinker} {args} --no-gc-sections Setup.o -Ttext=804E2000 -o Setup")


    compress("Setup", "Setup.bin")
    os.system(f"{ppcObjCopy} -O binary -S  Setup {outputDirectory}/Setup.bin")


    setupFileSize = os.path.getsize("Setup.bin")
    print(setupFileSize)

    makeNM("Setup", "SetupNM.txt")
    with open("SetupNM.txt", "r") as file:
        text = file.read()
        setupFunctionAddress = re.findall(r"([0-9a-f]{8}) [0-9a-f]{8} T setup", text)[0]

    makeLoadSetupFile("LoadSetup.s", hex(0x804E2000))



    os.system(f"{ppcCompiler} -c {compilerArgs} LoadSetup.s")
    #links
    os.system(f"{ppcLinker} {linkerArgs} --no-gc-sections LoadSetup.o -o LoadSetup")

    os.system(f"{ppcObjCopy} -O binary -S --only-section=.text LoadSetup LoadSetup.bin")


    with open("LoadSetup.bin", 'rb') as file:
        data = file.read()
        text = data.hex()

    with open("LoadSetupFile.asm", "w") as file:
        for i in range(0, len(text), 16):
            word1 = text[i: i + 8]
            word2 = text[i + 8: i + 16]
            file.write(f"* {word1.upper()} {word2.upper()}\n")



    makeMap("SetupNM.txt", "Setup.map", "../BrawlFuncMap.map")
    makeDisassembly("Setup", "SetupDis.txt")



def resetFolder(directory):
    try:
        clearFolder(directory)
    except:
        os.mkdir(directory)


def clearFolder(directory):
    for file in os.listdir(directory):
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





main()
