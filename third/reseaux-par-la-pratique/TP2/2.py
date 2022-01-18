import socket

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8080
IMAGE = "Fronalpstock_big.jpg"
GET = f"GET /{IMAGE} HTTP/1.1\r\n\r\n"

if __name__ == "__main__":
    response = b''
    print(f"Downloading {IMAGE} from {SERVER_IP}:{SERVER_PORT}...")

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

    print(f"Recived {len(response)} bytes")
