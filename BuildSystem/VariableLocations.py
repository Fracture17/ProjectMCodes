import os
import re
from dataclasses import dataclass


@dataclass
class VariableLayout:
    name: str
    type: str
    size: int
    address: int
    indent: int
    members: list = None
    topLevel: bool = True

    def __post_init__(self):
        if self.members is None:
            self.members = []

    def toFileFormat(self):
        if self.members:
            members = []
            for m in self.members:
                members.append(m.toFileFormat())
            members = ',\n'.join(members)
            return groupFormat.format(variables=members, name=self.name)
        return variableFormat.format(address=hex(self.address), label=self.name)

    def addMember(self, layout):
        self.members.append(layout)
        layout.topLevel = False


dmeFileFormat = """
{{
    "watchList": [
        {variables}
    ]
}}
""".strip()


groupFormat = """
{{
    "groupEntries": [
        {variables}
    ],
    "groupName": "{name}"
}}
""".strip()


variableFormat = """
{{
    "address": "{address}",
    "baseIndex": 0,
    "label": "{label}",
    "typeIndex": 2,
    "unsigned": false
}}
""".strip()




def makeVariableLocationMap(dataAddressesFilePath, linkedInitializersPath, gdbBatchCommandsFilePath, variableLayoutsFilePath, dmeVariableLocationsFilePath, ppcGDB):
    with open(dataAddressesFilePath, 'r') as file:
        text = file.read()
        variableText = re.findall(r'([0-9a-f]{8}) ([0-9a-f]{8}) . ([a-zA-Z0-9_]+)', text)
        variables = []
        for address, size, name in variableText:
            address = int(address, 16)
            members = makeVariableLayout(name, address, linkedInitializersPath, gdbBatchCommandsFilePath, variableLayoutsFilePath, ppcGDB)
            for m in members:
                m.address += address
            topMembers = [m for m in members if m.topLevel]
            v = VariableLayout(name, None, size, address, 0, topMembers)
            variables.append(v)

    data = []

    print(variables[1])

    for v in variables:
        data.append(v.toFileFormat())

    data = ',\n'.join(data)
    text = dmeFileFormat.format(variables=data)
    with open(dmeVariableLocationsFilePath, 'w') as file:
        file.write(text)


def makeVariableLayout(varName, address, linkedInitializersPath, gdbBatchCommandsFilePath, variableLayoutsFilePath, ppcGDB):
    makeGDBBatchFile(varName, linkedInitializersPath, gdbBatchCommandsFilePath, variableLayoutsFilePath)
    makeVariableLayoutsFile(gdbBatchCommandsFilePath, ppcGDB)
    return makeVariableLayoutFromGDBFile(variableLayoutsFilePath)







def makeVariableLayoutFromGDBFile(variableLayoutsFilePath):
    with open(variableLayoutsFilePath, 'r') as file:
        text = file.read()

    memberVarRegex = r"(\/\* +([0-9]+) +\| +([0-9]+) \*\/ +)(struct|class|union|enum)? ?([a-zA-Z0-9_]+) (\**)([a-zA-Z0-9_]+(\[[0-9]+\])*);"
    memberVars = re.findall(memberVarRegex, text)

    classRegex = r"(\/\* +([0-9]+) +\| +([0-9]+) \*\/ +)(struct|class|union|enum) (.+?) .*?\{"
    memberClasses = re.findall(classRegex, text)

    classEndRegex = r"( +)\} ([a-zA-Z0-9_]+);"
    memberClassEnds = re.findall(classEndRegex, text)

    classes = []
    members = []

    for classStart, classEnd in zip(memberClasses, memberClassEnds):
        print(classStart)
        print(classEnd)
        c = VariableLayout(classEnd[1], classStart[4], int(classStart[2]), int(classStart[1]), len(classStart[0]))
        classes.append(c)

    for m in memberVars:
        print(m)
        member = VariableLayout(m[6], m[4] + m[5], int(m[2]), int(m[1]), len(m[0]))
        members.append(member)
        for c in classes:
            if member.address >= c.address and member.address + member.size <= c.address + c.size:
                if member.indent == c.indent + 4:
                    c.addMember(member)
                    break

    for member in classes:
        for c in classes:
            if member.address >= c.address and member.address + member.size <= c.address + c.size:
                if member.indent == c.indent + 4:
                    c.addMember(member)
                    break

    print(classes)
    print(members)
    members.extend(classes)
    return members

    i = 0
    for classStart, classEnd in zip(memberClasses, memberClassEnds):
        endOfClassOffset = int(classStart[0]) + int(classStart[1])
        classMembers = []
        while i < len(memberVars) and int(memberVars[i] < endOfClassOffset):
            pass



def makeGDBBatchFile(varName, linkedInitializersPath, gdbBatchCommandsFilePath, variableLayoutsFilePath):
    with open(gdbBatchCommandsFilePath, 'w') as file:
        file.write(f"file {linkedInitializersPath}\n")
        file.write(f"set logging file {variableLayoutsFilePath}\n")
        file.write(f"set logging overwrite on\n")
        file.write(f"set logging redirect on\n")
        file.write(f"set logging on\n")
        file.write(f"ptype /o {varName}\n")


def makeVariableLayoutsFile(gdbBatchCommandsFilePath, ppcGDB):
    os.system(f"{ppcGDB} --batch -x {gdbBatchCommandsFilePath}")


def makeDMEVariableLocationsFile(variableLayoutsFilePath, dmeVariableLocationsFilePath):
    with open(variableLayoutsFilePath, 'r') as file:
        text = file.read()

    memberVarRegex = r"\/\* +([0-9]+) +\| +([0-9]+) \*\/( +)(struct|class|union|enum)? ?([a-zA-Z0-9_]+) (\**)([a-zA-Z0-9_]+(\[[0-9]+\])*);"
    memberVars = re.findall(memberVarRegex, text)
