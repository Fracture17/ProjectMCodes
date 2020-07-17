from dataclasses import dataclass


@dataclass
class Function:
    name: str
    address: int
    size: int