cmake_minimum_required(VERSION 3.15)

project(ProjectMCodes)

set(CMAKE_CXX_COMPILER C:\\devkitPro\\devkitPPC\\bin\\powerpc-eabi-g++.exe)
set(CMAKE_LINKER C:\\devkitPro\\devkitPPC\\bin\\powerpc-eabi-ld.exe)
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(CMAKE_CROSSCOMPILING 1)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CMake/DebugSettings.cmake)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

include(CMake/Paths.cmake)
include(CMake/Functions.cmake)


create_folder(${PROJECT_BINARY_DIR}/IncludedCodes)


#used to ensure all nm files have been written for each code
add_custom_target(codeBarrier)


#Build individual codes
#Every individual library runs this function
function(add_code codeName)
    #Create static library for each code group
    add_library(${codeName} STATIC "${ARGN}")
    set_target_properties(${codeName} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/IncludedCodes)
    SET_TARGET_PROPERTIES(${codeName} PROPERTIES SUFFIX "")
    SET_TARGET_PROPERTIES(${codeName} PROPERTIES PREFIX "")

    add_dependencies(codeBarrier ${codeName})
endfunction()


include_directories(${librariesDirectory})
include_directories(${globalsDirectory})


#code subdirectories
add_subdirectory(Codes)
add_subdirectory(Libraries)
add_subdirectory(Globals)


add_custom_target(build ALL
        COMMAND python ${buildSystemDirectory}/BuildFromCMake.py ${PROJECT_BINARY_DIR} ${PPC_BIN_DIRECTORY}
        DEPENDS codeBarrier)

