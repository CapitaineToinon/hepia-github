#!/usr/bin/python3

from random import choice, randint
from typing import List


def gcd(a: int, b: int):
    """
    Calculate the greatest common divisor of a and b
    """
    while b != 0:
        a, b = b, a % b
    return a


def generate_primes(n: int) -> List[int]:
    """
    Generate all the primes from 2 to n using the eratosthene algorythm
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


def generate_g(prime: int):
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

        # if all values (ingoring 0) are True then g is generator
        if len([v for v in values[1:] if v == False]) == 0:
            return g


def generate_keys():
    """
    Generate a public and private key pair
    """
    p = random_prime(digits=3)
    g = generate_g(prime=p)
    a = randint(1, p - 2)
    A = pow(g, a, p)
    return (p, g, A), a


def generate_k(prime):
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


def bezout_b(a, b):
    """
    Return only the b coeficient of bezout
    """
    _, _b, _ = bezout_indentity(a, b)
    return _b


def sign(m, public_key, private_key):
    a = private_key
    (p, g, A) = public_key
    k = generate_k(prime=p)

    Y = pow(g, k, p)

    gcd, x, y = bezout_indentity(k, p - 1)
    S = (m - a * Y) * x
    # Y = (g ** k) % p
    # S = ((m - (a * Y)) % (p - 1)) * ((k ** -1) % (p - 1))
    return Y, S


if __name__ == "__main__":

    message = "Bonjour"

    # public_key, private_key = generate_keys()

    p = random_prime(digits=2)
    # p = 13
    g = generate_g(p)
    # g = 7
    a = randint(1, p - 2)
    # a = 9

    A = pow(g, a, p)
    key = (p, g, A)

    k = generate_k(p)
    # k = 5

    Y = pow(g, k, p)
    b = bezout_b(k, p - 1)
    print(a, Y, b)

    def sign_char(char) -> int:
        return ((ord(char) - (a * Y)) * b) % (p - 1)

    def verify_char(m, S) -> bool:
        left = (pow(A, Y) * pow(Y, S)) % p
        right = pow(g, ord(m), p)
        return left == right

    def sign(message) -> List[int]:
        return [sign_char(char) for char in message]

    def verify(message, signed) -> bool:
        for i in range(0, len(signed)):
            if verify_char(message[i], signed[i]) == False:
                return False
        return True
    
    # signed = sign_char("B")
    # print("B", signed)
    # print(verify_char("B", signed))

    signed = sign(message)
    print(message, signed)
    print(verify(message, signed))
