#set path variables
set(initializerInjectionsFile ${PROJECT_BINARY_DIR}/initializerInjections.cpp)
set(codeInjectionsFile ${PROJECT_BINARY_DIR}/codeInjections.cpp)
set(scriptFolder ../scripts)
set(outputFolder ${PROJECT_BINARY_DIR}/output)
set(disassemblyFolder ${PROJECT_BINARY_DIR}/disassembly)

set(librariesDirectory ${PROJECT_SOURCE_DIR}/Libraries)
set(globalsDirectory ${PROJECT_SOURCE_DIR}/Globals)

set(makeInjectionsScript ${scriptFolder}/makeInjections.py)
#set(compressScript ${scriptFolder}/compress.py)
set(splitScript ${scriptFolder}/splitTextAndData.py)
set(insertBranchesScript ${scriptFolder}/insertBranches.py)
#set(createDisassemblyScript ${scriptFolder}/createDisassembly.py)
set(makeFunctionMapScript ${scriptFolder}/makeFunctionMap.py)

set(justInitializers ${PROJECT_BINARY_DIR}/justInitializers)
set(compressedInitializers ${PROJECT_BINARY_DIR}/compressedInitializers)
set(finalInitializers ${outputFolder}/initializers.bin)

set(justData ${PROJECT_BINARY_DIR}/justData)
set(compressedData ${PROJECT_BINARY_DIR}/compressedData)
set(finalData ${outputFolder}/data.bin)

set(justCodes ${PROJECT_BINARY_DIR}/justCodes)
set(compressedCodes ${PROJECT_BINARY_DIR}/compressedCodes)
set(finalCodes ${outputFolder}/codes.bin)

set(initializersNM ${disassemblyFolder}/initializersNM.txt)
set(codesNM ${disassemblyFolder}/codesNM.txt)
set(dataNM ${disassemblyFolder}/dataNM.txt)
set(initializersDisassembly ${disassemblyFolder}/initializersDisassembly.txt)
set(codesDisassembly ${disassemblyFolder}/codesDisassembly.txt)
set(baseFuncMap ../BrawlFuncMap.map)
set(initializersCPPFuncMap ${disassemblyFolder}/InitializersCPPFuncMap.map)
set(cppFuncMap ${disassemblyFolder}/CPPFuncMap.map)