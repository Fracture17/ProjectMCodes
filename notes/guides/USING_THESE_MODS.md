# Installing These Mods

These mods have been created using this build pipeline for developing mods
using c++ as opposed to building them by-hand via typical ASM. The way they
get loaded is also a bit different than *normal* gecko codes in that 
they're placed at the root of the SD card and get loaded using a single 
gecko code.

**for now**, I have placed a working build of the AI debugging code I
developed in the "codes" folder. All of its files will be in the "AIDebug"
folder.

### Warning:

doing this with the netplay `sd.raw` will cause it to desync - therefor
it is recommended to make a copy of your sd.raw before doing this to ensure
you won't need to re-download the full build whenever you wish to play netplay
again.

---

Here are the steps to actually install this code for Project+:

1. move everything except `LoadSetupFile.asm` into the root of the SD card.
2. move `LoadSetupFile.asm` into the `Project+/Source` directory
3. find `RSBE01.txt` (or `PPLUSNETPLAY.txt`), open it, and add
    the following line after the second-to-last line:
    - `.include source/LoadSetupFile.asm` 
4. locate `GCTRealMate.exe`
5. click and drag the text file you modified onto `GCTRealMate.exe`. This
    will generate a `.GCT` file of the same name. Take the generated `.GCT` 
    file and put it where you found its text file. You can overwrite
    whatever GCT was there before.
6. as long as everything is where it should be (all the `.bin` files at the root
    along and the updated `.GCT` in-place), you should be able to launch the
    game and have it work
    
Now that you have that `LoadSetupFile.asm` injected into the GCT file, if you wish
to use other codes that have been built with Fracture's compiler, you only need
to replace all the `.bin` files at the root of the sd card with whatever
`.bin` files come with the new code.

Thank you for following this setup guide!

\- fudgepop01