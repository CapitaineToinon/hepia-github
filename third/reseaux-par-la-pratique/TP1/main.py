#!/usr/bin/python3

from random import choice, randint


def gcd(a, b):
    """
    Calculate the greatest common divisor of a and b
    """
    while b != 0:
        a, b = b, a % b
    return a


def modexp(base, exp, modulus):
    """
    function that does a modular exponentiation
    """
    return pow(base, exp, modulus)


def generate_primes(n):
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


def random_prime(digits):
    """
    Generate a random prime of n digits
    """
    upper = 10 ** digits
    lower = 10 ** (digits - 1)
    return choice([p for p in generate_primes(upper) if p > lower])


def generate_g(prime):
    """
    Generates g according to a prime
    """
    candidates = []

    while True:
        # try random guesses until we find one
        values = [False] * prime
        g = randint(2, prime - 1)

        if g in candidates:
            continue

        candidates.append(g)

        for i in range(0, prime):
            j = pow(g, i, prime)

            if values[j] == True:
                break

            values[j] = True

        # if all values are True then g is generator
        if len([v for v in values[1:] if v == False]) == 0:
            return g


def generate_keys():
    """
    Generate a public and private key pair
    """
    p = random_prime(digits=3)
    g = generate_g(prime=p)
    a = randint(1, p - 1)
    A = modexp(g, a, p)
    return (p, g, A), a


def generate_k(prime):
    """
    Generate a random 0 < k < p-1 that is coprime to p-1
    """
    while True:
        k = randint(1, prime - 1)
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


def sign(m, public_key, private_key):
    a = private_key
    (p, g, A) = public_key
    k = generate_k(prime=p)

    Y = modexp(g, k, p)

    gcd, x, y = bezout_indentity(k, p - 1)
    S = (m - a * Y) * x
    # Y = (g ** k) % p
    # S = ((m - (a * Y)) % (p - 1)) * ((k ** -1) % (p - 1))
    return Y, S


if __name__ == "__main__":
    (p, g, A), private_key = generate_keys()
    Y, S = sign(89, (p, g, A), private_key)

    print(f"(p, g, A)={(p, g, A)}, a={private_key}")
    print(f"(Y, S)={(Y, S)}")
    # print(verify(15, public_key, Y, S))

    print(gcd(20, 10))
    print(bezout_indentity(20, 10))
