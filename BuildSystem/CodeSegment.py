from dataclasses import dataclass

from Function import Function


@dataclass
class CodeSegment:
    startAddress: int
    endAddress: int
    functions: list = None
    size: int = None
    remainingSize: int = None
    currentAddress: int = None
    name: str = None
    filename: str = None

    def __post_init__(self):
        self.functions = []
        self.size = self.endAddress - self.startAddress
        self.remainingSize = self.size
        self.currentAddress = self.startAddress

    def canInsert(self, func: Function):
        return func.size <= self.size

    def insertFunction(self, func: Function):
        assert self.canInsert(func)
        self.functions.append(func)
        self.size -= func.size
        self.remainingSize -= func.size
        self.currentAddress += func.size

    def setName(self, name):
        self.name = name
        self.filename = name + '.bin'
