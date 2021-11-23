from urllib import request
from urllib.error import URLError, HTTPError
from threading import Thread
from timeit import default_timer as timer
from time import sleep

THREAD_COUNT = 20_000
SERVER_IP = "127.0.0.1"
SERVER_PORT = "8080"
IMAGE = "Fronalpstock_big.jpg"
TARGET = f"http://{SERVER_IP}:{SERVER_PORT}/{IMAGE}"


def normal_client():
    """
    Simulating a normal client browsing the server regularly
    This should fail after a while once the other threads are running
    """
    while True:
        try:
            start = timer()
            response = request.urlopen(TARGET)
            end = timer()
            ms = (end - start) * 1000
            print("Regular client downloaded image in: {:.2f}ms.".format(ms))
            sleep(1)
        except HTTPError as e:
            print(f"Regular client error code: {e.code}")
        except URLError as e:
            print(f"Regular client error reason: {e.reason}")


def thread_function(number):
    while True:
        try:
            bytes = request.urlopen(TARGET).read()
            # do nothing with the bytes
        except HTTPError as e:
            pass  # do nothing
        except URLError as e:
            pass  # do nothing


if __name__ == "__main__":
    print("Starting normal client that will download the image once every second")
    regular_client = Thread(target=normal_client)
    regular_client.start()

    sleep(5)
    print(f"Creating {THREAD_COUNT} threads to kill the server...")

    threads = []
    for i in range(THREAD_COUNT):
        t = Thread(target=thread_function, args=(i,))
        t.start()
        threads.append(t)

    print(f"{THREAD_COUNT} threads are running...")
