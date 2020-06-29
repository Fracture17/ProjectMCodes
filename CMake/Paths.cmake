#set path variables
set(initializerInjectionsFile ${PROJECT_BINARY_DIR}/initializerInjections.cpp)
set(codeInjectionsFile ${PROJECT_BINARY_DIR}/codeInjections.cpp)
set(scriptFolder ../scripts)
set(outputFolder ${PROJECT_BINARY_DIR}/output)
set(disassemblyFolder ${PROJECT_BINARY_DIR}/disassembly)

#path to ppc executables
set(PPC_BIN_DIRECTORY "C:\\devkitPro\\devkitPPC\\bin")
#prefix in ppc executable file names
set(PPC_BIN_PREFIX "powerpc-eabi-")
#use this like: ${PPC_PATH_PREFIX}nm.exe
set(PPC_PATH_PREFIX ${PPC_BIN_DIRECTORY}/${PPC_BIN_PREFIX})
set(PPC_OBJCOPY ${PPC_PATH_PREFIX}objcopy.exe)
set(PPC_NM ${PPC_PATH_PREFIX}nm.exe)
set(PPC_OBJDUMP ${PPC_PATH_PREFIX}objdump.exe)

set(librariesDirectory ${PROJECT_SOURCE_DIR}/Libraries)
set(globalsDirectory ${PROJECT_SOURCE_DIR}/Globals)

set(makeInjectionsScript ${scriptFolder}/makeInjections.py)
set(insertBranchesScript ${scriptFolder}/insertBranches.py)
set(makeFunctionMapScript ${scriptFolder}/makeFunctionMap.py)
set(convertAddressesToLinkerCommandsScript ${scriptFolder}/convertAddressesToLinkerCommands.py)

set(justInitializersFile ${PROJECT_BINARY_DIR}/justInitializers)
set(compressedInitializersFile ${PROJECT_BINARY_DIR}/compressedInitializers)
set(finalInitializersFile ${outputFolder}/initializers.bin)

set(justDataFile ${PROJECT_BINARY_DIR}/justData)
set(finalDataFile ${outputFolder}/data.bin)

set(justCodesFile ${PROJECT_BINARY_DIR}/justCodes)
set(compressedCodesFile ${PROJECT_BINARY_DIR}/compressedCodes)
set(finalCodesFile ${outputFolder}/codes.bin)

set(initializersNMFile ${disassemblyFolder}/initializersNM.txt)
set(codesNMFile ${disassemblyFolder}/codesNM.txt)
set(dataNMFile ${disassemblyFolder}/dataNM.txt)
set(dataAddressesFile ${PROJECT_BINARY_DIR}/dataAddresses.txt)
set(initializersDisassembly ${disassemblyFolder}/initializersDisassembly.txt)
set(codesDisassemblyFile ${disassemblyFolder}/codesDisassembly.txt)
set(baseFuncMap ../BrawlFuncMap.map)
set(initializersCPPFuncMap ${disassemblyFolder}/InitializersCPPFuncMap.map)
set(cppFuncMap ${disassemblyFolder}/CPPFuncMap.map)