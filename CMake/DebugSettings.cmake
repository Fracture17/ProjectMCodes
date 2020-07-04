add_compile_options(-nostdinc)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_CXX_STANDARD 17)
set(VERBOSE 1)

set(codeStartAddress 0x8057a800)
set(initializersStartAddress 0x80d00000)
set(dataStartAddress 0x8057d000)

#can use -Og to shrink code size
#-O2 and -Os should also work, but don't right now for different reasons
set(CMAKE_CXX_FLAGS "-fno-function-cse -ffunction-sections -fno-exceptions -fno-asynchronous-unwind-tables -std=c++17 -fno-builtin -ffreestanding -mcpu=750 -mmultiple -fno-inline -save-temps -fverbose-asm")
#set(CMAKE_CXX_FLAGS "-fno-function-cse -fdata-sections -ffunction-sections -fno-exceptions -fno-asynchronous-unwind-tables -std=c++17 -fno-builtin -ffreestanding -mcpu=750 -mmultiple -fno-inline -save-temps")

set(CMAKE_EXE_LINKER_FLAGS "-z common-page-size=4 -z max-page-size=4 -Wl,--relax -Wl,--section-start=.rodata=${dataStartAddress} -nostdlib")
