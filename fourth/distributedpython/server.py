import yaml
import socket
import argparse
import pickle
from threading import Thread, Lock
from functools import reduce
from time import sleep

PORT: int = 65432


class Message:
    dist: int
    nodes: list[str]

    @staticmethod
    def new(dist: int, nodes: list[str]) -> 'Message':
        m = Message
        m.dist = dist
        m.nodes = nodes
        return m

    @staticmethod
    def loads(bytes: bytes) -> 'Message':
        return pickle.loads(bytes)

    def dumps(self) -> str:
        return pickle.dumps(self)


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


class Server(object):
    config: Config
    dist: dict[str, int] = {}
    first: dict[str, tuple[str, int]] = {}

    lock: Lock = Lock()
    count: int = 0
    messages: dict[str, dict] = {}
    done: set[str] = set()

    @staticmethod
    def from_path(path: str) -> 'Server':
        config = Config.from_path(path)
        return Server(config)

    def __init__(self, config: Config) -> None:
        self.config = config
        self.dist[self.config.address] = 0
        self.first[self.config.address] = self.config.address

    def start(self) -> None:
        t = Thread(target=self.init_broadcast, name="init_broadcast")
        t.start()

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            try:
                s.bind((self.config.address, PORT))
                s.listen()
                print(f"listening on {self.config.address}:{PORT}")

                while True:
                    conn, addr = s.accept()
                    t = Thread(target=self.handler, args=(
                        conn, addr), name="handler")
                    t.start()
            except KeyboardInterrupt:
                s.close()

        t.join()

    def handler(self, conn: socket.socket, addr: tuple[str, int]) -> None:
        """
        Main handler that processes messages recieved on the socket
        """
        is_neighbour = self.config.is_neighbour(addr)
        who = 'neighbour' if is_neighbour else 'client'
        with conn:
            address, _ = addr
            print(f"Connected by {who} ({address})")
            data = conn.recv(4096)  # TODO while True for more?

            if is_neighbour:
                neighbour = self.config.get_neighbour_from_addr(addr)
                message = pickle.loads(data)
                self.on_message(neighbour, message)
            else:
                print("TODO handle client messages")

    def on_message(self, neighbour: Config, message: dict):
        print(f"Message from {neighbour.address} is {message}")
        with self.lock:
            self.count += 1

            if "stop" in message:
                # one of our neighbour is done
                self.done.add(neighbour.address)
            else:
                # otherwise save its message for later
                self.messages[neighbour.address] = message

            # all our neighbours answered us with either message or stop
            if self.count == len(self.config.neighbours):
                next_nodes: set[str] = set()
                for (address, message) in self.messages.items():
                    for node in message["nodes"]:
                        # we found a better match!
                        if node not in self.dist or self.dist[node] > message["dist"]:
                            d = (message["dist"] + 1)
                            current_d = self.dist[node] if node in self.dist else 'None'
                            print(
                                f"address {node} current is {current_d} but will now be {d}")
                            self.dist[node] = d
                            self.first[node] = address
                            next_nodes.add(node)

                if len(next_nodes) == 0:
                    self.broadcast_all({
                        "stop": True,
                    })

                    print("DONE")
                    print("dist", self.dist)
                    print("first", self.first)
                    return

                # otherwise keep going broadcast
                next = {
                    "dist": message["dist"] + 1,
                    "nodes": [*next_nodes]
                }

                print("NEXT", next)

                if message["dist"] == 0:
                    sleep(5)

                self.broadcast_all(next)
                self.count = 0
                self.messages.clear()

    def init_broadcast(self, delay: int = 10):
        sleep(delay)
        self.broadcast_all({
            "dist": 0,
            "nodes": [self.config.address]
        })

    def broadcast_all(self, message: dict):
        threads = [Thread(target=self.broadcast_to,
                          name="broadcast_to",
                          args=(neighbour, message,))
                   for neighbour in self.config.neighbours if neighbour not in self.done]

        for t in threads:
            t.start()

        for t in threads:
            t.join()

    def broadcast_to(self, to: Config,  message: dict):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.config.address, 0))
            s.connect((to.address, PORT))
            bytes = pickle.dumps(message)
            s.send(bytes)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog='distributed',
        description='What the program does',
        epilog='Text at the bottom of help')

    # parser.add_argument('port')
    parser.add_argument('config')
    args = parser.parse_args()

    server = Server.from_path(args.config)
    server.start()
