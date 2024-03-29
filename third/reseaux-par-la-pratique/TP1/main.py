#!/usr/bin/python3

from random import choice, randint
from typing import List
import argparse


def gcd(a: int, b: int) -> int:
    """
    Calculate the greatest common divisor of a and b
    """
    while b != 0:
        a, b = b, a % b
    return a


def generate_primes(n: int) -> List[int]:
    """
    Generate all the primes from 2 to n using the eratosthenes algorithm
    """
    p = 2
    numbers = [i for i in range(n + 1)]

    while p * p < n:
        if numbers[p] != False:
            for i in range(p * 2, n + 1, p):
                numbers[i] = False
        p += 1

    return [n for n in numbers[2:] if n is not False]


def random_prime(digits: int) -> int:
    """
    Generate a random prime of n digits
    """
    upper = 10 ** digits
    lower = 10 ** (digits - 1)
    return choice([p for p in generate_primes(upper) if p > lower])


def generate_g(prime: int) -> int:
    """
    Generates g according to a prime
    """
    guesses = []

    while True:
        # try random guesses until we find one
        values = [False] * prime
        g = randint(2, prime - 1)

        if g in guesses:
            continue

        guesses.append(g)

        for i in range(0, prime):
            j = pow(g, i, prime)
            values[j] = True

        # if all values (ignoring 0) are True then g is generator
        if len([v for v in values[1:] if v == False]) == 0:
            return g


def generate_k(prime) -> int:
    """
    Generate a random 0 < k < p-1 that is coprime to p-1
    """
    while True:
        k = randint(1, prime - 2)
        if gcd(k, prime - 1) == 1:
            return k


def bezout_indentity(a, b):
    """
    Calculate the greatest common divisor of a and b
    """
    if b == 0:
        return a, 1, 0
    else:
        d, x, y = bezout_indentity(b, a % b)
        return d, y, x - (a // b) * y


def bezout_b(a, b) -> int:
    """
    Return only the b coefficient of bezout
    """
    _, _b, _ = bezout_indentity(a, b)
    return _b


def generate_keys():
    """
    Generate new keys
    """
    p = random_prime(digits=3)
    g = generate_g(p)
    a = randint(1, p - 2)

    A = pow(g, a, p)
    k = generate_k(p)
    Y = pow(g, k, p)
    b = bezout_b(k, p - 1)

    return p, g, A, a, k, Y, b


def sign(message, p, g, A, a, k, Y, b) -> List[int]:
    """
    Sign a message with given keys
    """
    def sign_char(char) -> int:
        return ((ord(char) - (a * Y)) * b) % (p - 1)

    return [sign_char(char) for char in message]


def verify(message, signed, p, g, A, a, k, Y, b) -> bool:
    """
    Verifies that an output match
    """
    if len(message) != len(signed):
        return False

    def verify_char(m, S) -> bool:
        left = (pow(A, Y) * pow(Y, S)) % p
        right = pow(g, ord(m), p)
        return left == right

    for i in range(0, len(signed)):
        if verify_char(message[i], signed[i]) == False:
            return False
    return True


def main():
    parser = argparse.ArgumentParser(description="El Gamal")
    parser.add_argument(
        "-g"
        "--generate",
        dest="generate",
        help="generate keys",
        action="store_true",
    )
    parser.add_argument(
        "-s",
        "--sign",
        dest="sign",
        help="sign a given file and given keys",
        nargs=2,
        metavar=("keys", "file")
    )
    parser.add_argument(
        "-v",
        "--verify",
        dest="verify",
        help="verify the signature for a given file",
        nargs=3,
        metavar=("keys", "source", "output")
    )
    args = parser.parse_args()
    if args.sign:
        keys = []
        message = ""
        with open(args.sign[0], "r") as f:
            keys = [int(x) for x in f if x != ""]
            f.close()

        with open(args.sign[1], "r") as f:
            message = f.read()
            f.close()

        output = sign(message, *keys)
        with open("output", "w+") as f:
            for c in output:
                f.write(str(c))
                f.write("\n")
            f.close()
    elif args.verify:
        keys = []
        message = ""
        output = ""

        with open(args.verify[0], "r") as f:
            keys = [int(x) for x in f if x != ""]
            f.close()

        with open(args.verify[1], "r") as f:
            message = f.read()
            f.close()

        with open(args.verify[2], "r") as f:
            output = [int(x) for x in f if x != ""]
            f.close()

        if verify(message, output, *keys):
            print("The signature matched")
        else:
            print("The signature did not match")
            exit(1)
    elif args.generate:
        keys = generate_keys()
        with open("id_gamal.keys", "w+") as f:
            for key in keys:
                f.write(str(key))
                f.write("\n")
            f.close()
        print("created keys")
    else:
        parser.print_help()
        exit(0)


if __name__ == "__main__":
    main()
