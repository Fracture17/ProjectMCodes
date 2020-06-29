#1. Build individual code
#2. For each code, use nm binutil to get file with list of functions
#3. After everything is built, call python script to use nm files to identify injections
#python file makes a cpp file that calls the functions to inject and has their injection addresses
#4. Compile and link injection cpp file and all compiled code libraries
#5. Compress executable by removing debug info and other nonessential stuff
#6. Edit compressed file to include injection addresses.  This is the final code file
#7. Create a disassembly file that interlaces assembly with c++
#8. Create function map to be used with Dolphin


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
#target_link_options(initializers PRIVATE "-Wl,--print-gc-sections")

include_directories(initializers Common)


#15. Compile and link the code injection cpp file and the code libraries with their ctors and dtors removed
#Contains all code that is needed after initialization
#Second in file, but actually builds 15th
add_executable(code ${codeInjectionsFile})
target_link_options(code PRIVATE "-Wl,-Ttext=${codeStartAddress}")
target_link_options(code PRIVATE "-Wl,--gc-sections")
#target_link_options(code PRIVATE "-Wl,--print-gc-sections")
target_link_options(code PRIVATE "-Wl,@dataAddresses.txt")






#1. Build individual code
#Every individual library runs this function
function(add_code codeName)
    add_library(${codeName} STATIC "${ARGN}")

    #15. Create new library file without ctors and dtors
    #This will be used to construct a version of code without any one-time initialization code
    #This occurs once initializers is built, since it modifies the library in place
    #Modifying in place is needed for proper linking
    add_custom_target(EXTRACTED_${codeName} ALL
            COMMAND ppc-objcopy --remove-section=.ctors --remove-section=.dtors $<TARGET_FILE:${codeName}> ${codeName}
            DEPENDS initializers)

    #3. For each code, use nm binutil to get file with list of functions
    add_custom_target("NM_${codeName}" ALL
            COMMAND ppc-nm -C $<TARGET_FILE:${codeName}> > "NM_${codeName}.txt"
            DEPENDS ${codeName})

    add_dependencies(nmBarrier "NM_${codeName}")
    target_link_libraries(initializers ${codeName})
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



#4. After everything is built, call python script to use nm files to identify injections
#Creates cpp file used as entry for the initializer build
get_property(NMFiles GLOBAL PROPERTY source_list_property)
add_custom_target(initializerInjections
        COMMAND python ${makeInjectionsScript} ${initializerInjectionsFile} ${NMFiles} true
        BYPRODUCTS ${initializerInjectionsFile}
        OUTPUT ${initializerInjectionsFile}
        DEPENDS nmBarrier)
add_dependencies(initializers initializerInjections)

#Creates cpp file used as entry for the code build
add_custom_target(codeInjections
        COMMAND python ${makeInjectionsScript} ${codeInjectionsFile} ${NMFiles} false
        BYPRODUCTS ${codeInjectionsFile}
        OUTPUT ${codeInjectionsFile}
        DEPENDS nmBarrier)


#6. Split the initializer text and data sections into different files
add_custom_target(splitInitializersAndData ALL
        COMMAND ppc-objcopy --only-section=.text $<TARGET_FILE:initializers> ${justInitializers}
        COMMAND ppc-objcopy --remove-section=.text $<TARGET_FILE:initializers> ${justData}
        BYPRODUCTS ${justInitializers} ${justData}
        OUTPUT ${justInitializers} ${justData}
        DEPENDS initializers)



#7. Make a list of all symbols in the initializer data file and their addresses
#Since the initializer build contains everything, its data file is a superset of the code build
#To prevent the initializer from having different data offsets from the code build, the code build will have these addresses forcibly set
add_custom_target(makeDataNM ALL
        COMMAND ppc-nm -C -S -l -v ${justData} > ${dataNM}
        BYPRODUCTS ${dataNM}
        OUTPUT ${dataNM}
        DEPENDS splitInitializersAndData)


#8. Collect the relevent symbols and put them into a file for the linker
add_custom_target(getDataAddresses ALL
        COMMAND python ${scriptFolder}/convertAddressesToLinkerCommands.py ${dataNM} dataAddresses.txt
        DEPENDS makeDataNM)
add_dependencies(code getDataAddresses)


#9. Compress initializer code by removing debug info, symbols, ect.
add_custom_target(compressInitializers ALL
        COMMAND ppc-objcopy -O binary -S ${justInitializers} ${compressedInitializers}
        BYPRODUCTS ${compressedInitializers}
        OUTPUT ${compressedInitializers}
        DEPENDS splitInitializersAndData)
#10. Compress initializer data by removing debug info, symbols, ect.
#The data file doesn't need any more processing, so this is its final version
add_custom_target(compressData ALL
        COMMAND ppc-objcopy -O binary -S ${justData} ${finalData}
        BYPRODUCTS ${finalData}
        OUTPUT ${finalData}
        DEPENDS splitInitializersAndData)


#11. Create symbol file for initializer file
#Includes code and data because it's more useful together
add_custom_target(makeInitializersNM ALL
        COMMAND ppc-nm -C -S -l -v $<TARGET_FILE:initializers> > ${initializersNM}
        BYPRODUCTS ${initializersNM}
        OUTPUT ${initializersNM}
        DEPENDS initializers)


#12. Edit compressed initializer code file to include injection addresses.  This is the final initializer code file
add_custom_target(initializerInsertBranches ALL
        COMMAND python ${insertBranchesScript} ${initializersNM} ${initializersStartAddress} ${compressedInitializers} ${finalInitializers}
        BYPRODUCTS ${finalInitializers}
        OUTPUT ${finalInitializers}
        DEPENDS compressInitializers)


#13. Create a disassembly file that interlaces assembly with c++
add_custom_target(createInitializersDisassembly ALL
        COMMAND ppc-objdump -C -S -l -h -M broadway $<TARGET_FILE:initializers> > ${initializersDisassembly}
        BYPRODUCTS ${initializersDisassembly}
        OUTPUT ${initializersDisassembly}
        DEPENDS initializers)

#14. Create function map of initializers to be used with Dolphin
#There are two seperate function maps because they largely contain the same functions
#Having both in one would cause a bunch of duplicates and confusion
#Additionally, the initializer functions are lost after they are run once, so they would only be useful in specific circumstances
add_custom_target(makeInitializersFunctionMap ALL
        COMMAND python ${makeFunctionMapScript} ${initializersNM} ${initializersCPPFuncMap} ${baseFuncMap}
        BYPRODUCTS ${initializersCPPFuncMap}
        OUTPUT ${initializersCPPFuncMap}
        DEPENDS makeInitializersNM)




#16. Extract the text section from the code build
#The data section is not necessary, and would probably be inaccurate
add_custom_target(getJustCodes ALL
        COMMAND ppc-objcopy --only-section=.text $<TARGET_FILE:code> ${justCodes}
        BYPRODUCTS ${justCodes}
        OUTPUT ${justCodes}
        DEPENDS code)

#17. Compress code by removing debug info, symbols, ect.
add_custom_target(compressCodes ALL
        COMMAND ppc-objcopy -O binary -S ${justCodes} ${compressedCodes}
        BYPRODUCTS ${compressedCodes}
        OUTPUT ${compressedCodes}
        DEPENDS getJustCodes)


#18. Create symbol file for code file
#Includes code and data because it's more useful together
add_custom_target(makeCodesNM ALL
        COMMAND ppc-nm -C -S -l -v $<TARGET_FILE:code> > ${codesNM}
        BYPRODUCTS ${codesNM}
        OUTPUT ${codesNM}
        DEPENDS code)


#19. Edit compressed code file to include injection addresses.  This is the final code file
add_custom_target(codeInsertBranches ALL
        COMMAND python ${insertBranchesScript} ${codesNM} ${codeStartAddress} ${compressedCodes} ${finalCodes}
        BYPRODUCTS ${codesNM} ${finalCodes}
        OUTPUT ${codesNM} ${finalCodes}
        DEPENDS compressCodes)


#20. Create a disassembly file that interlaces assembly with c++
add_custom_target(createCodesDisassembly ALL
        COMMAND ppc-objdump -C -S -l -h -M broadway $<TARGET_FILE:code> > ${codesDisassembly}
        BYPRODUCTS ${codesDisassembly}
        OUTPUT ${codesDisassembly}
        DEPENDS code)


#21. Create function map of the code build to be used with Dolphin
#This is intended to be the main map used in most cases
add_custom_target(makeCodesFunctionMap ALL
        COMMAND python ${makeFunctionMapScript} ${codesNM} ${cppFuncMap} ${baseFuncMap}
        BYPRODUCTS ${cppFuncMap}
        OUTPUT ${cppFuncMap}
        DEPENDS makeCodesNM)