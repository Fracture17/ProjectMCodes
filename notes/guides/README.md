# Writing your Own Mods for Brawl / P+ with C++

Heyooo - fudge here! Just so this project isn't lost to time, I've decided to write a guide so that anyone else that desires to write codes in C++ can do so if they desire!

## Things you'll need:

- A computer running Windows
- [CLion](https://www.jetbrains.com/clion/)
  - until someone figures out how to make this compile using vscode or something free :(

- [MinGW](https://osdn.net/projects/mingw/releases/)
  - install the `msys64` version

- [devkitpro powerpc](https://github.com/devkitPro/installer/releases/download/v3.0.3/devkitProUpdater-3.0.3.exe)
  - necessary to actually compile stuff

## Setting Up:

Once you have the stuff linked above installed, the setup *should* be relatively simple.

1. Open CLion

2. Clone this repository in CLion 

   1. make a new project
   2. go to VCS (at the top bar) ==> Git ==> pull

   <img src="C:\Users\dareb\OneDrive\Documents\Gekko-ppc-asm\fracture_pmcodes\notes\guides\README_Resources\VCS_Pull_Path.png" alt="VCS Pull Path"  />

3. Go to File ==> Settings
4. Setup the toolchain
   1. go to "Toolchains"
   2. Add a new toolchain and name it MinGW
      1. technically the name doesn't matter it just makes things consistent
   3. under "environment," browse for and select the MinGW installation:
      1. for me, it's `C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32`
   4. under "C Compiler," browse for and select the devkitpro powerpc C Compiler:
      1. for me, it's `C:\devkitPro\devkitPPC\bin\powerpc-eabi-gcc.exe`
   5. under "C++ Compiler," do the same as step 4 but for the C++ compiler
      1. for me, it's `C:\devkitPro\devkitPPC\bin\powerpc-eabi-g++.exe`
   6. in the end, it should look something like this:
5. Configure CMake
   1. if it doesn't already exist, make a new profile entitled "Debug-MinGW"
   2. Set the build type to "Debug"
   
   
   
   1. for toolchains, Select your newly-made MinGW toolchain 
   2. under CMake options, copy and paste the following:
      1. `-DCMAKE_SYSTEM_NAME=Generic -DCMAKE_C_COMPILER_WORKS=1 -DCMAKE_CXX_COMPILER_WORKS=1`
      2. this will just tell CMake that, yes, the compiler *does* work and *no* there shouldn't be any problems with it

You should now have everything you need setup to start developing the codes. But how exactly do you do that? Continue reading to find out!

## Developing Codes

There are two main folders in the root directory that you'll be interested in:

1. Codes
   - this is where codes are created
2. Libraries
   -  this is where the libraries necessary to write codes are created
   -  if you're contributing to libraries, you'll also want to check out `BrawlFuncMap.map` at the root of the project
      -  this contains a list of all the known function names as well as their offsets.

### Defining Native Brawl Functions

To define brawl functions that are in known locations, we need to tell the program exactly where they are in memory. To do this, we can define macros that act as function definitions. Take a look at `ftManager.h` at `Libraries/Brawl/ftManager.h`:

You can see some various macros created with `#define` statements, such as the following:

```c++
//gets the number of fighters in a match
#define _getEntryCount_ftManager ((int (*)(ftManager * self)) 0x80815be4)
```

This tells the compiler that at the address `0x80815be4` there is a function (which we'll name`_getEntryCount_ftManager`) which will return an `int` that takes a `ftManager` as an argument. If we look at the `BrawlFuncMap` and search for `80815be4` we'll find `80815be4 00000014 80815be4 0 getEntryCount/[ftManager]/ft_manager.o`, which consists of the offset, size, and name of the function.

If you want more information about the function, you'll need to either reverse-engineer it in ghidra or take a look at the asm at that address when you have dolphin paused with P+ running.

### Creating New Codes

#### CMake

in the `Codes` folder, there are a bunch of codes that we've already created. To tell the compiler to add a code to the output, we use `CMakeLists.txt` and use the `add_subdirectory` command. Then when creating a new code, you'll need another `CMakeLists.txt` containing an `add_code` command that has the code name followed by all the files it depends on.

#### INJECTION and BASIC_INJECT

When in a code's cpp file, to inject code into Brawl we'll need to use the `BASIC_INJECT` and `INJECTION` macros that fracture has defined for us. For arguments they take:

1. the function name
2. the offset
3. the asm which we wish to inject at that location

The main difference between `BASIC_INJECT` and `INJECTION` is that `BASIC_INJECT` will automatically surround the asm with the `SAVE_REGS` and `RESTORE_REGS` macros, which will save and restore the registers, respectively. It will also automatically create a branch to the function name that was given as an argument. For cases where you'll just be modifying the game's state, you'll often use `BASIC_INJECT`. For when you want to mess with specific registers or output particular things, you'll use `INJECTION`.

It's important to know what line of code you're overwriting at that particular location so you can add it to the `asm` injection.

#### Injecting with C++

To actually write and execute C++ code you'll need to either preface the function definition with `extern "C"` or write it within an `extern "C" {...}` block. Once you do this, use a `bl` statement with the function name as the argument within your injected ASM to reference and execute it. 

`BASIC_INECT` example:

```c++
BASIC_INJECT("renderInjection", 0x8001792c, "addi r3, r30, 280");

extern "C" void renderInjection() {
    ...
}
```

`INJECTION` example:

```c++
INJECTION("renderInjection", 0x8001792c, R"(
	addi r3, r30, 280
	SAVE_REGS
	bl renderInjection
	RESTORE_REGS
)");

extern "C" void renderInjection() {
    ...
}
```

#### Arguments and Return Values

there's an excellent entry on the CustomMarioKart wiki here:

http://wiki.tockdom.com/wiki/Compiler#Register_Convention

the important takeaway here is the `Register Convention` section, which details exactly how registers normally work when using functions. Any function arguments called from an injected function ( called upon with `bl`) will follow those conventions. They'll start with the floating point-based arguments (registers `f1` through `f8`) followed by the integer-based arguments (registers `r3` through `r10`).

To access the registers you want in a C++ injection, you'll sometimes need to move registers around with `mr` and `fmr`. This is why it's important to use `SAVE_REGS` and `RESTORE_REGS` - because without them you may mess with the order of things that Brawl expects which could lead to unexpected behavior (such as crashes). If I'm not mistaken, defining local variables may also mess with registers, which could *also* lead to unexpected behavior (such as crashes) - so the takeaway here is that `SAVE_REGS` and `RESTORE_REGS` are extremely important to use unless you know what you're doing when using the `INJECTION` macro to define a code.

#### Moving Registers to Use as Arguments in Injected Functions

here's a snippet from one of my slightly more complex injections:

```c++
INJECTION("CUSTOM_AI_COMMANDS", 0x80917450, R"(
    lbz r4, 0x00(r30)
    SAVE_REGS
    mr r3, r26
    mr r4, r30
    bl aiCommandHandlers
    RESTORE_REGS
)");
// ...
extern "C" void aiCommandHandlers(aiAct* aiActInst, const int* args) { ... }
```

the `lbz r4, 0x00(r30)` is the code that was there before I injected it. The exclusion of this would cause `r30` to have an unexpected value. The two `mr` statements then move some data from registers `r26` and `r30` to `r3` and `r4`, respectively. In this case they're pointers to an `aiAct` instance and the "args" of the custom AI command. This allows us to access them in our custom `aiCommandHandlers` function as the first and second arguments, because they're in `r3` and `r4`.

## Current Known Limitations:

- struct inheritance involving **functions** that we create does *not* work.

## That's All Folks

Thank you for reading! We can't wait to see the amazing things you'll create with this new and powerful framework!

\- Fracture & Fudgepop01