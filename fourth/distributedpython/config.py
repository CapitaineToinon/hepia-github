import yaml
from enum import Enum


class Mode(Enum):
    init: str = "INIT"
    wait: str = "WAIT"

    def __str__(self):
        return self.value


class Config:
    id: int
    address: str
    neighbours: list['Config']

    @staticmethod
    def from_path(path: str) -> 'Config':
        with open(path, "r") as stream:
            content = yaml.safe_load(stream)
            return Config(content)

    def __init__(self, yaml: dict) -> None:
        self.id = yaml['id']
        self.address = yaml['address']
        self.neighbours = []

        if yaml.get('neighbours'):
            for neighbour in yaml['neighbours']:
                self.neighbours.append(Config(neighbour))

    def is_neighbour(self, addr: tuple[str, int]) -> bool:
        return False if self.get_neighbour_from_addr(addr=addr) is None else True

    def get_neighbour_from_addr(self, addr: tuple[str, int]) -> 'Config':
        address, _ = addr
        for n in self.neighbours:
            if n.address == address:
                return n
        return None
