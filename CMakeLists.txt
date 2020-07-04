#I have numbered each "step" of the script, because it is difficult to keep track of otherwise
#The numbered order isn't exactly accurate, since multiple things can happen at once
#The general idea of this script is it builds a bunch of seperate code libraries and links them together
#Since injections are not part of normal c++, they have to be handled differently
#Injections are given special symbol names which are used to encode the location of the injection, and where the function ends up
#The initializer build contains everything, and is ran once when the game is still loading
#This lets it initialize the global variables and anything else needed for the codes to function
#Since it only needs to work when the game is still loading, there is plenty of room
#The code build removes everything that is not referenced by an injection
#It is muc smaller, but has different data offsets than the initializer
#To fix this, the linker forcefully sets the data addresses to the initializer build's values


cmake_minimum_required(VERSION 3.15)

project(ProjectMCodes)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CMake/DebugSettings.cmake)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

include(CMake/Paths.cmake)
include(CMake/Functions.cmake)


create_folder(${outputFolder})
create_folder(${disassemblyFolder})



#used to ensure all nm files have been written for each code
add_custom_target(nmBarrier)


#5. Compile and link initializers injection cpp file and all compiled code libraries
#First in file, but actually builds fifth
add_executable(initializers ${initializerInjectionsFile})
target_link_options(initializers PRIVATE "-Wl,-Ttext=${initializersStartAddress}")
target_link_options(initializers PRIVATE "-Wl,--gc-sections")
target_link_options(initializers PRIVATE "-Wl,--print-gc-sections")


#15. Compile and link the code injection cpp file and the code libraries with their ctors and dtors removed
#Contains all code that is needed after initialization
#Second in file, but actually builds 15th
add_executable(code ${codeInjectionsFile})
target_link_options(code PRIVATE "-Wl,-Ttext=${codeStartAddress}")
target_link_options(code PRIVATE "-Wl,--gc-sections")
target_link_options(code PRIVATE "-Wl,--print-gc-sections")
target_link_options(code PRIVATE "-Wl,@${dataAddressesFile}")
target_link_options(code PRIVATE "-Wl,--section-start=.text.setMenuOpenFlag=0x88000000")
#target_link_options(code PRIVATE "testLinkerScript")



#Build individual codes
#Every individual library runs this function
function(add_code codeName)
    #1. Create static library for each code group
    add_library(${codeName} STATIC "${ARGN}")

    #15. Create new library file without ctors and dtors
    #This will be used to construct a version of code without any one-time initialization code
    #This occurs once initializers is built, since it modifies the library in place
    #Modifying in place is needed for proper linking
    add_custom_target(EXTRACTED_${codeName} ALL
            COMMAND ${PPC_OBJCOPY} --remove-section=.ctors --remove-section=.dtors $<TARGET_FILE:${codeName}> $<TARGET_FILE:${codeName}>
            DEPENDS initializers)
    #add_custom_target(DIS____${codeName} ALL
    #        COMMAND ${PPC_OBJDUMP} -h $<TARGET_FILE:${codeName}> dump.txt
    #        DEPENDS ${codeName})
    #add_custom_target(EXTRACTED_${codeName} ALL
    #        COMMAND python ${scriptsFolder}/renameFunctions.py dump.txt $<TARGET_FILE:${codeName}>
    #        DEPENDS DIS____${codeName})
    add_custom_target(NOT_EXTRACTED_${codeName} ALL
            COMMAND ${PPC_OBJCOPY} $<TARGET_FILE:${codeName}> ${codeName}
            DEPENDS ${codeName})

    #2. For each code group, use nm binutil to get file with list of functions
    add_custom_target("NM_${codeName}" ALL
            COMMAND ${PPC_NM} -C -S -l -v $<TARGET_FILE:${codeName}> > "NM_${codeName}.txt"
            DEPENDS ${codeName})

    add_dependencies(nmBarrier "NM_${codeName}")
    target_link_libraries(initializers ${codeName})
    #link with the same library names as initializers, but they have been altered by the time it builds
    target_link_libraries(code ${codeName})

    #NMFiles property contains list of all built files
    #Only changes when reloading cmake, so there won't be any issues with losing track
    get_property(NMFiles GLOBAL PROPERTY source_list_property)
    set_property(GLOBAL PROPERTY source_list_property "${NMFiles}, $<TARGET_FILE_DIR:${codeName}>/NM_${codeName}.txt")
endfunction()


include_directories(${librariesDirectory})
include_directories(${globalsDirectory})


#code subdirectories
add_subdirectory(Codes)
add_subdirectory(Libraries)
add_subdirectory(Globals)


#This section deals with building the initializer and data stuff

#3. After all codes are built, call python script to use nm files to identify injections
#Creates cpp file used as entry for the initializer build
get_property(NMFiles GLOBAL PROPERTY source_list_property)
add_custom_target(initializerInjections
        COMMAND python ${makeInjectionsScript} ${initializerInjectionsFile} ${NMFiles} true
        BYPRODUCTS ${initializerInjectionsFile}
        OUTPUT ${initializerInjectionsFile}
        DEPENDS nmBarrier)
add_dependencies(initializers initializerInjections)


#4. Initializer is built here


#5. Split the initializer text and data sections into different files
add_custom_target(splitInitializersAndData ALL
        COMMAND ${PPC_OBJCOPY} --only-section=.text $<TARGET_FILE:initializers> ${justInitializersFile}
        COMMAND ${PPC_OBJCOPY} --remove-section=.text $<TARGET_FILE:initializers> ${justDataFile}
        BYPRODUCTS ${justInitializersFile} ${justDataFile}
        OUTPUT ${justInitializersFile} ${justDataFile}
        DEPENDS initializers)


#6. Make a list of all symbols in the initializer data file and their addresses
#Since the initializer build contains everything, its data file is a superset of the code build
#To prevent the initializer from having different data offsets from the code build, the code build will have these addresses forcibly set
add_custom_target(makeDataNM ALL
        COMMAND ${PPC_NM} -C -S -l -v ${justDataFile} > ${dataNMFile}
        BYPRODUCTS ${dataNMFile}
        OUTPUT ${dataNMFile}
        DEPENDS splitInitializersAndData)


#7. Collect the relevent symbols and put them into a file for the linker
add_custom_target(getDataAddresses ALL
        COMMAND python ${convertAddressesToLinkerCommandsScript} ${dataNMFile} ${dataAddressesFile}
        BYPRODUCTS ${dataAddressesFile}
        OUTPUT ${dataAddressesFile}
        DEPENDS makeDataNM)
add_dependencies(code getDataAddresses)


#8. Compress initializer code by removing debug info, symbols, ect.
add_custom_target(compressInitializers ALL
        COMMAND ${PPC_OBJCOPY} -O binary -S ${justInitializersFile} ${compressedInitializersFile}
        BYPRODUCTS ${compressedInitializersFile}
        OUTPUT ${compressedInitializersFile}
        DEPENDS splitInitializersAndData)


#9. Compress initializer data by removing debug info, symbols, ect.
#The data file doesn't need any more processing, so this is its final version
add_custom_target(compressData ALL
        COMMAND ${PPC_OBJCOPY} -O binary -S ${justDataFile} ${finalDataFile}
        BYPRODUCTS ${finalDataFile}
        OUTPUT ${finalDataFile}
        DEPENDS splitInitializersAndData)


#10. Create nm symbol file for initializer file
#Includes code and data because it's more useful together
add_custom_target(makeInitializersNM ALL
        COMMAND ${PPC_NM} -C -S -l -v $<TARGET_FILE:initializers> > ${initializersNMFile}
        BYPRODUCTS ${initializersNMFile}
        OUTPUT ${initializersNMFile}
        DEPENDS initializers)


#11. Edit compressed initializer code file to include injection addresses.  This is the final initializer code file
add_custom_target(initializerInsertBranches ALL
        COMMAND python ${insertBranchesScript} ${initializersNMFile} ${initializersStartAddress} ${compressedInitializersFile} ${finalInitializersFile}
        BYPRODUCTS ${finalInitializersFile}
        OUTPUT ${finalInitializersFile}
        DEPENDS compressInitializers makeInitializersNM)


#12. Create a disassembly file that interlaces assembly with c++
add_custom_target(createInitializersDisassembly ALL
        COMMAND ${PPC_OBJDUMP} -C -S -l -h -M broadway $<TARGET_FILE:initializers> > ${initializersDisassembly}
        BYPRODUCTS ${initializersDisassembly}
        OUTPUT ${initializersDisassembly}
        DEPENDS initializers)

#13. Create function map of initializers to be used with Dolphin
#There are two seperate function maps because they largely contain the same functions
#Having both in one would cause a bunch of duplicates and confusion
#Additionally, the initializer functions are lost after the loading stage, so they would only be accurate in specific circumstances
add_custom_target(makeInitializersFunctionMap ALL
        COMMAND python ${makeFunctionMapScript} ${initializersNMFile} ${initializersCPPFuncMap} ${baseFuncMap}
        BYPRODUCTS ${initializersCPPFuncMap}
        OUTPUT ${initializersCPPFuncMap}
        DEPENDS makeInitializersNM)




#This sections deals with building the codes stuff


#14. Creates cpp file used as entry for the code build
add_custom_target(codeInjections
        COMMAND python ${makeInjectionsScript} ${codeInjectionsFile} ${NMFiles} false
        BYPRODUCTS ${codeInjectionsFile}
        OUTPUT ${codeInjectionsFile}
        DEPENDS nmBarrier)


#15. Codes file is built here


#16. Extract the text section from the code build
#The data section is not necessary, and would likely be inaccurate
add_custom_target(getJustCodes ALL
        COMMAND ${PPC_OBJCOPY} --only-section=.text $<TARGET_FILE:code> ${justCodesFile}
        BYPRODUCTS ${justCodesFile}
        OUTPUT ${justCodesFile}
        DEPENDS code)


#17. Compress code by removing debug info, symbols, ect.
add_custom_target(compressCodes ALL
        COMMAND ${PPC_OBJCOPY} -O binary -S ${justCodesFile} ${compressedCodesFile}
        BYPRODUCTS ${compressedCodesFile}
        OUTPUT ${compressedCodesFile}
        DEPENDS getJustCodes)


#18. Create symbol file for code file
#Includes code and data because it's more useful together
#TODO: symbol data is currently incomplete, since most data is lost
#TODO: need to use initializer data addresses instead
add_custom_target(makeCodesNM ALL
        COMMAND ${PPC_NM} -C -S -l -v $<TARGET_FILE:code> > ${codesNMFile}
        BYPRODUCTS ${codesNMFile}
        OUTPUT ${codesNMFile}
        DEPENDS code)


#19. Edit compressed code file to include injection addresses.  This is the final code file
add_custom_target(codeInsertBranches ALL
        COMMAND python ${insertBranchesScript} ${codesNMFile} ${codeStartAddress} ${compressedCodesFile} ${finalCodesFile}
        BYPRODUCTS ${finalCodesFile}
        OUTPUT ${finalCodesFile}
        DEPENDS compressCodes)


#20. Create a disassembly file that interlaces assembly with c++
add_custom_target(createCodesDisassembly ALL
        COMMAND ${PPC_OBJDUMP} -C -S -l -h -M broadway $<TARGET_FILE:code> > ${codesDisassemblyFile}
        BYPRODUCTS ${codesDisassemblyFile}
        OUTPUT ${codesDisassemblyFile}
        DEPENDS code)


#21. Create function map of the code build to be used with Dolphin
#This is intended to be the main map used in most cases
add_custom_target(makeCodesFunctionMap ALL
        COMMAND python ${makeFunctionMapScript} ${codesNMFile} ${cppFuncMap} ${baseFuncMap}
        BYPRODUCTS ${cppFuncMap}
        OUTPUT ${cppFuncMap}
        DEPENDS makeCodesNM)
