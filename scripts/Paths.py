outputDirectory = "output"
disassemblyDirectory = "disassembly"

compiledCodesDirectory = "CompiledCodes"
preLinkedInitializersNMDirectory = "PreCompiledInitializersNMs"
initializersCPPFilePath = "Initializers.cpp"
linkedInitializersPath = "linkedInitializers"
extractedDataPath = "extractedData"
extractedInitializersPath = "extractedInitializers"
finalData = f"{outputDirectory}/data.bin"
compressedInitializersPath = "compressedInitializers"
finalInitializersPath = f"{outputDirectory}/init.bin"
dataNMPath = "dataNM.txt"
dataAddressesFilePath = "dataAddresses.txt"
initializersNMPath = f"{disassemblyDirectory}/initializersNM.txt"
initializersDisassemblyPath = f"{disassemblyDirectory}/initializersDisassembly.txt"
newFunctionNamesFilePath = "newFunctionNames.txt"
editedCodesDirectory = "EditedCodes"
editedCodesNMDirectory = "EditedCodesNMs"
codesCPPFilePath = "codes.cpp"
linkedCodesPath = "linkedCodes"
linkedCodeFunctionsPath = "linkedFunctions.txt"
functionAddressesFilePath = "functionAddresses.txt"

PATHS = [
    compiledCodesDirectory,
    preLinkedInitializersNMDirectory,
    initializersCPPFilePath,
    linkedInitializersPath,
    extractedDataPath,
    extractedInitializersPath,
    finalData,
    compressedInitializersPath,
    finalInitializersPath,
    dataNMPath,
    dataAddressesFilePath,
    initializersNMPath,
    initializersDisassemblyPath,
    newFunctionNamesFilePath,
    editedCodesDirectory,
    editedCodesNMDirectory,
    codesCPPFilePath,
    linkedCodesPath,
    linkedCodeFunctionsPath,
    functionAddressesFilePath,
]


ppcObjCopy = "powerpc-eabi-objcopy.exe"
ppcNM = "powerpc-eabi-nm.exe"
ppcCompiler = "powerpc-eabi-gcc.exe"
ppcLinker = "powerpc-eabi-ld.exe"
ppcObjDump = "powerpc-eabi-objdump.exe"

PPC_PATHS = [
    ppcObjCopy,
    ppcNM,
    ppcCompiler,
    ppcLinker,
    ppcObjDump,
]


#Prepends base directoy to relative paths
#Should be called once by main code
def init(baseDirectory, ppcBinDirectory):
    global PATHS
    global PPC_PATHS
    for i, path in enumerate(PATHS):
        PATHS[i] = f"{baseDirectory}/{path}"

    for i, path in enumerate(PPC_PATHS):
        PPC_PATHS[i] = f"{ppcBinDirectory}/{path}"
