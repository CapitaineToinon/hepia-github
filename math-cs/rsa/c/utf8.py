import sys

if __name__ == "__main__":
  for line in sys.stdin:
    print(bytearray.fromhex(line).decode("utf-8"))
