import socket
import argparse
import pickle
from config import Config, Mode
from threading import Thread, Lock
from time import sleep


class Server(object):
    port: int
    config: Config
    mode: Mode
    initialized: bool = False
    ready: bool = False
    dist: dict[str, int] = {}
    first: dict[str, tuple[str, int]] = {}

    lock: Lock = Lock()
    count: int = 0
    messages: dict[str, dict] = {}
    done: set[str] = set()

    @staticmethod
    def from_args(port: int, path: str, mode: Mode) -> 'Server':
        config = Config.from_path(path)
        return Server(port, config, mode)

    def __init__(self, port: int, config: Config, mode: Mode) -> None:
        self.port = port
        self.config = config
        self.mode = mode
        self.dist[self.config.address] = 0
        self.first[self.config.address] = self.config.address

    def start(self) -> None:
        if self.mode == Mode.init:
            print(f"Started in init mode")
            self.init_broadcast(delay=5)
        else:
            print(f"Started in wait mode")

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            try:
                s.bind((self.config.address, self.port))
                s.listen()
                print(f"listening on {self.config.address}:{self.port}")

                while not self.ready:
                    conn, addr = s.accept()
                    t = Thread(target=self.handler, args=(
                        conn, addr), name="handler")
                    t.start()
                    t.join()
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

        if self.mode == Mode.wait and not self.initialized:
            self.init_broadcast(delay=0)

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
                    self.ready = True
                    self.broadcast_all({
                        "stop": True,
                    })

                    self.print_state()
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
        self.initialized = True

        def run():
            print(f"broadcasting t0 to neighbours in {delay}s")
            sleep(delay)
            self.broadcast_all({
                "dist": 0,
                "nodes": [self.config.address]
            })

        t = Thread(target=run, name="init_broadcast_run")
        t.start()

    def broadcast_all(self, message: dict):
        print("broadcasting t0 to neighbours now")
        threads = [Thread(target=self.broadcast_to,
                          name="broadcast_to",
                          args=(neighbour, message,))
                   for neighbour in self.config.neighbours
                   if neighbour.address not in self.done]

        for t in threads:
            t.start()

        for t in threads:
            t.join()

    def broadcast_to(self, to: Config,  message: dict):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.config.address, 0))
            s.connect((to.address, self.port))
            bytes = pickle.dumps(message)
            s.send(bytes)

    def print_state(self):
        print("Table of distances:")
        for (ip, dist) in self.dist.items():
            print([ip, dist])

        print("Table of firsts:")
        for (ip, next) in self.first.items():
            print([ip, next])


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog='distributed',
        description='What the program does',
        epilog='Text at the bottom of help')

    parser.add_argument('port', type=int)
    parser.add_argument('config', type=str)
    parser.add_argument('mode', type=Mode, choices=list(Mode))
    args = parser.parse_args()

    server = Server.from_args(args.port, args.config, args.mode)
    server.start()
