import os
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

    def asLinkerCommand(self):
        return f"--defsym={self.name}={hex(self.address)}"


def convertAddressesToLinkerCommands(dataNMPath, outputFilePath):
    funcs = getAllFunctions(dataNMPath)
    with open(outputFilePath, 'w') as file:
        commands = [func.asLinkerCommand() for func in funcs]
        file.write(' '.join(commands))





def getAllFunctions(nmFilePath):
    with open(nmFilePath, 'r') as file:
        #8 char address, 8 char size, name
        funcs = re.findall(r"([0-9a-zA-Z]{8}) ([0-9a-zA-Z]{8}) . (.*?)[^\S]", file.read())
        funcs = [Function(*f) for f in funcs]
        return funcs
