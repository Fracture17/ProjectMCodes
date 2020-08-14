add_compile_options(-nostdinc)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_CXX_STANDARD 17)
set(VERBOSE 1)

#can use -Og to shrink code size
#-O2 and -Os should also work, but don't right now for different reasons
set(CMAKE_CXX_FLAGS "-g -fno-function-cse -ffunction-sections -fdata-sections -fno-exceptions -fno-asynchronous-unwind-tables -std=c++17 -fno-builtin -ffreestanding -mcpu=750 -mmultiple -fno-inline -save-temps -fverbose-asm -fdump-lang-class")
