add_compile_options(-nostdinc)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_CXX_STANDARD 20)
set(VERBOSE 1)

#can use -Og to shrink code size
#-O2 and -Os should also work, but don't right now for different reasons
# set(CMAKE_CXX_FLAGS "-g -nostartfiles -fomit-frame-pointer -fno-unwind-tables -fno-stack-check -fno-function-cse -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -std=c++17 -fno-builtin -ffreestanding -mcpu=750 -mmultiple -fno-inline -save-temps=obj -fverbose-asm -fno-threadsafe-statics")

# Replicates -Os with broken flags excluded
set(CMAKE_CXX_FLAGS 
  "-O1 -ggdb3 \
    -nostartfiles \
    -fomit-frame-pointer \
    -fno-unwind-tables \
    -fno-stack-check \
    -fno-function-cse \
    -ffunction-sections \
    -fdata-sections \
    -fno-exceptions \
    -fno-rtti \
    -fno-asynchronous-unwind-tables \
    -std=c++17 \
    -fno-builtin \
    -ffreestanding \
    -mcpu=750 \
    -mmultiple \
    -fno-inline \
    -save-temps=obj \
    -fverbose-asm \
    -fno-threadsafe-statics \
    \
    -fcode-hoisting \
    -fcrossjumping \
    -fcse-follow-jumps  -fcse-skip-blocks \
    -fdelete-null-pointer-checks \
    -fdevirtualize  -fdevirtualize-speculatively \
    -fexpensive-optimizations \
    -fgcse  -fgcse-lm  \
    -fhoist-adjacent-loads \
    -finline-functions \
    -finline-small-functions \
    -findirect-inlining \
    -fisolate-erroneous-paths-dereference \
    -flra-remat \
    -foptimize-sibling-calls \
    -foptimize-strlen \
    -fpartial-inlining \
    -fpeephole2 \
    -frerun-cse-after-loop  \
    -fsched-interblock  -fsched-spec \
    \
    -fstore-merging \
    -fstrict-aliasing \
    -fthread-jumps \
    -ftree-builtin-call-dce \
    -ftree-slp-vectorize \
    -ftree-switch-conversion  -ftree-tail-merge \
    -ftree-vrp \
    -finline-functions \
    \
    -fschedule-insns2 \
    -fvect-cost-model=cheap \
    \
    -fgcse-after-reload \
    -floop-interchange \
    -floop-unroll-and-jam \
    -fpredictive-commoning \
    -fsplit-loops \
    -ftree-loop-distribution \
  ")

  # enbiggens size
  # -ftree-loop-vectorize \
  # -ftree-pre \


  # -fipa-bit-cp  -fipa-cp  -fipa-icf \
  # -fipa-cp-clone \
  # -fipa-ra \
  # -fipa-sra \
  # -fipa-vrp \

## UNCOMMENT FOR DEBUGGING
# set(CMAKE_CXX_FLAGS "-Og -ggdb3 -nostartfiles -fomit-frame-pointer -fno-unwind-tables -fno-stack-check -fno-function-cse -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -std=c++17 -fno-builtin -ffreestanding -mcpu=750 -mmultiple -fno-inline -save-temps=obj -fverbose-asm -fno-threadsafe-statics")


#set(CMAKE_CXX_FLAGS "-ggdb3 -fno-function-cse -ffunction-sections -fdata-sections -fno-exceptions -fno-asynchronous-unwind-tables -std=c++17 -fno-builtin -ffreestanding -mcpu=750 -mmultiple -fno-inline -save-temps -fverbose-asm -fdump-lang-class")

# optimizations and what they do:
# https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html

# -ggdb3 \
# -nostartfiles \
# -fomit-frame-pointer \
# -fno-unwind-tables \
# -fno-stack-check \
# -fno-function-cse \
# -ffunction-sections \
# -fdata-sections \
# -fno-exceptions \
# -fno-rtti \
# -fno-asynchronous-unwind-tables \
# -std=c++17 \
# -fno-builtin \
# -ffreestanding \
# -mcpu=750 \
# -mmultiple \
### POSSIBLY CAN GO WITHOUT
### -fno-inline \
# -save-temps=obj \
# -fverbose-asm \
# -fno-threadsafe-statics \
# \

# FULL Os OPTIMIZATIONS:
# -fcaller-saves \
# -fcode-hoisting \
# -fcrossjumping \
# -fcse-follow-jumps  -fcse-skip-blocks \
# -fdelete-null-pointer-checks \
# -fdevirtualize  -fdevirtualize-speculatively \
# -fexpensive-optimizations \
### seems to break with this compiler
### -ffinite-loops \
# -fgcse  -fgcse-lm  \
# -fhoist-adjacent-loads \
# -finline-functions \
# -finline-small-functions \
# -findirect-inlining \
### breaks when working with lambdas in certain cases (see: qsort)
### -fipa-bit-cp  -fipa-cp  -fipa-icf \
### -fipa-ra \
### breaks under unknown circumstances
### -fipa-sra \
### -fipa-vrp \
# -fisolate-erroneous-paths-dereference \
# -flra-remat \
# -foptimize-sibling-calls \
# -foptimize-strlen \
# -fpartial-inlining \
# -fpeephole2 \
#### MAKES BUILD SIZES SIGNIFICANTLY LARGER THAN NECESSARY
#### removing this brought size from 88508kb to 86860
#### -freorder-blocks-algorithm=stc \
#### THIS IS THE CULPRIT OF THE BREAKING -O2/Os
#### -freorder-blocks-and-partition  -freorder-functions \
# -frerun-cse-after-loop  \
# -fschedule-insns  \
####  APPEARS TO CRASH WHEN USING DYNAMIC_CAST
#### -fschedule-insns2 \
# -fsched-interblock  -fsched-spec \
# -fstore-merging \
# -fstrict-aliasing \
# -fthread-jumps \
# -ftree-builtin-call-dce \
# -ftree-loop-vectorize \
# -ftree-pre \
# -ftree-slp-vectorize \
# -ftree-switch-conversion  -ftree-tail-merge \
# -ftree-vrp \
### seems to break with this compiler
### -fvect-cost-model=very-cheap \
# -finline-functions \