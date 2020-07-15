import sys
import re
from dataclasses import dataclass


@dataclass
class Function:
    address: int
    size: int
    name: str

    def __post_init__(self):
        if isinstance(self.address, str):
            self.address = int(self.address, 16)
        if isinstance(self.size, str):
            self.size = int(self.size, 16)


def makeMap(nmFilePath, mapFilePath, existingMapPath=''):
    mapFormat = "{address} {size} {address} 0 {name}\n"

    if existingMapPath != '':
        with open(existingMapPath, 'r') as file:
            map = file.read()
    else:
        map = '.text\n'

    funcs = getAllFunctions(nmFilePath)
    for func in funcs:
        editFuncName(func)

        map += mapFormat.format(address=f"{func.address:08x}", size=f"{func.size:08x}", name=func.name)

    with open(mapFilePath, 'w+') as file:
        file.write(map)


def getAllFunctions(nmFilePath):
    with open(nmFilePath, 'r') as file:
        #8 char address, 8 char size, name
        funcs = re.findall(r"([0-9a-zA-Z]{8}) ([0-9a-zA-Z]{8}) . (.*)", file.read())
        funcs = [Function(*f) for f in funcs]
        return funcs


def editFuncName(func: Function):
    if hasCppSource(func):
        newName = combineNameAndSource(func)
        func.name = newName


def hasCppSource(func: Function):
    return len(func.name.split('\t')) == 2


def combineNameAndSource(func: Function):
    name, loc = func.name.split('\t')
    p = loc.rfind('/')
    loc = loc[p+1:]
    funcName = name + '@' + loc
    return funcName
