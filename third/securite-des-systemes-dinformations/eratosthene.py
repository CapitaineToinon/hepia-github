def get_primes(n):
    p = 2
    numbers = [i for i in range(n + 1)]

    while p * p < n:
        if numbers[p] != False:
            for i in range(p * 2, n + 1, p):
                numbers[i] = False
        p += 1

    return [n for n in numbers[2:] if n is not False]


if __name__ == "__main__":
    print(get_primes(100))
