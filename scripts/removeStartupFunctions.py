import os
import sys
import re


def main():
    directory = sys.argv[1]
    cppArgs = sys.argv[2]
    linkerFlags = sys.argv[3]
    output = sys.argv[4]
    x = sys.argv[5]

    print(output)

    #os.system(f"ppc-objcopy --only-section=.rodata {output} {output}")
    #os.system(f"ppc-objcopy --remove-section=.rodata {output} {output}")
    os.system(f"ppc-objcopy --remove-section=.ctors --remove-section=.dtors {output} {x}")
    #os.system(f"ppc-objcopy --remove-section=.dtors {x} {x}")
    #os.system(f"ppc-objcopy --rename-section .rodata=.bss -g {output} {output}")
    #os.system(f"ppc-objcopy --add-symbol __RODATA__=.rodata:0,.readonly {output} {output}")
    return


    files = [file for file in os.listdir() if file.endswith('.s')]

    for file in files:
        removeInitializers(file)
        removeDestructors(file)

    files = [f"{directory}/{file}" for file in files]
    files = ' '.join(files)

    print(output)

    print(f'ppc-gcc {files} {cppArgs} Wl-,"{linkerFlags} --gc-sections" -o {output}')
    os.system(f'ppc-gcc {files} {cppArgs} {linkerFlags} -o {output}')


def removeInitializers(path):
    with open(path, 'r') as file:
        removeConstructorsRegex = re.compile(r"\.section\t\.ctors.*?$.+?$.+?$", re.S | re.M)
        text = file.read()
        newText = re.sub(removeConstructorsRegex, '', text)
        #print(newText)
        print(re.findall(r"\.section\t\.ctors.*?$.+?$.+?$", text, re.S | re.M))

    with open(path, 'w') as file:
        file.write(newText)

def removeDestructors(path):
    with open(path, 'r') as file:
        removeDestructorsRegex = re.compile(r"\.section\t\.dtors.*?$.+?$.+?$", re.S | re.M)
        text = file.read()
        newText = re.sub(removeDestructorsRegex, '', text)

    with open(path, 'w') as file:
        file.write(newText)


main()