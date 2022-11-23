import yaml
import socket
import argparse
from threading import Thread, Lock
from time import sleep

PORT: int = 65432


class Config(object):
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
        address, _ = addr
        for n in self.neighbours:
            if n.address == address:
                return True
        return False


class Server(object):
    config: Config
    dist: dict[str, int] = {}
    first: dict[str, tuple[str, int]] = {}
    nodes: int = 2  # TODO dynamic that

    lock: Lock = Lock()
    msg_count: int = 0

    @staticmethod
    def from_path(path: str) -> 'Server':
        config = Config.from_path(path)
        return Server(config)

    def __init__(self, config: Config) -> None:
        self.config = config

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

    def handler(self, conn: socket.socket, addr: tuple[str, int]) -> None:
        """
        Main handler that processes messages recieved on the socket
        """
        is_neighbour = self.config.is_neighbour(addr)
        who = 'neighbour' if is_neighbour else 'client'

        with conn:
            print(f"Connected by {who} ({addr})")

            while True:
                data = conn.recv(1024)
                if not data:
                    break
                conn.sendall(data)

            with self.lock:
                self.msg_count += 1

                if self.msg_count == len(self.config.neighbours):
                    print("receive message from all my neighbours!")

    def init_broadcast(self, delay: int = 5):
        print(f"broadcasting in {delay}s")
        sleep(delay)
        print("broadcasting")
        for neighbour in self.config.neighbours:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                print(f"sending to {neighbour.address}:{PORT}")
                s.bind((self.config.address, 0))
                s.connect((neighbour.address, PORT))
                s.sendall(b"Hello, world")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog='distributed',
        description='What the program does',
        epilog='Text at the bottom of help')

    # parser.add_argument('port')
    parser.add_argument('config')
    parser.add_argument('-n', '--nodes')
    args = parser.parse_args()

    server = Server.from_path(args.config)
    server.start()
