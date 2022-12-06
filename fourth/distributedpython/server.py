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
                    self.handler(conn, addr)

                # wait before stopping
                sleep(5)
            except KeyboardInterrupt:
                s.close()

    def handler(self, conn: socket.socket, addr: tuple[str, int]) -> None:
        """
        Main handler that processes messages recieved on the socket
        """
        is_neighbour = self.config.is_neighbour(addr)

        with conn:
            data = conn.recv(4096)  # TODO while True for more?

            if is_neighbour:
                neighbour = self.config.get_neighbour_from_addr(addr)
                message = pickle.loads(data)
                self.on_message(neighbour, message)
            else:
                print("TODO handle client messages")

    def on_message(self, neighbour: Config, message: dict):
        print(f"Connected by {neighbour.address}: {message}")

        if self.mode == Mode.wait and not self.initialized:
            self.init_broadcast(delay=0)
            sleep(5)

        with self.lock:
            self.count += 1

            if "stop" in message:
                # one of our neighbour is done
                print(f"neighbour {neighbour.address} is done")
                self.done.add(neighbour.address)
            else:
                # otherwise save its message for later
                self.messages[neighbour.address] = message

            # all our neighbours answered us with either message or stop
            if self.count == len(self.config.neighbours):
                # reset for next step
                self.count = 0

                print("Received all messages...")
                next_nodes: set[str] = set()
                for (address, message) in self.messages.items():
                    for node in message["nodes"]:
                        d = (message["dist"] + 1)
                        # we found a better match!
                        if node not in self.dist or self.dist[node] > d:
                            current_d = self.dist[node] if node in self.dist else 'None'
                            print(
                                f"address {node} current is {current_d} but will now be {d}")
                            self.dist[node] = d
                            self.first[node] = address
                            next_nodes.add(node)

                # reset for next step
                self.messages.clear()

                if len(next_nodes) == 0:
                    print("READY")

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

                # We need to sleep before sending the next one
                if message["dist"] == 0:
                    sleep(5)

                print("NEXT", next)
                self.broadcast_all(next)

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
        to = [neighbour for neighbour in self.config.neighbours
              if neighbour.address not in self.done]

        print(f"broadcasting to {[n.address for n in to]}")

        if len(to) == 0:
            return

        threads = [Thread(target=self.broadcast_to,
                          name="broadcast_to",
                          args=(neighbour, message,))
                   for neighbour in to]

        for t in threads:
            t.start()

    def broadcast_to(self, to: Config,  message: dict):
        bytes = pickle.dumps(message)

        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.bind((self.config.address, 0))
                s.connect((to.address, self.port))
                s.send(bytes)
        except ConnectionRefusedError:
            print(f"ConnectionRefusedError {to.address}:{self.port}")

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
