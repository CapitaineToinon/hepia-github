# Sécurité

Par Antoine Sutter

# El Gamal

Ce travail pratique a pour but de signer et vérifer des signature sur des document à l'aide de l’algorithme de [El Gamal](https://fr.wikipedia.org/wiki/Cryptosyst%C3%A8me_de_ElGamal). Le langage de programation Python 3 a été choisit car celui-ci est facile d'utilisation surtout dans le domaine des mathématiques.

## 0 - Utilisation

```
$ python3 main.py
usage: main.py [-h] [-g--generate] [-s keys file] [-v keys source output]

El Gamal

optional arguments:
  -h, --help            show this help message and exit
  -g--generate          generate keys
  -s keys file, --sign keys file
                        sign a given file and given keys
  -v keys source output, --verify keys source output
                        verify the signature for a given file
```

## 1 - Generation des clés

La première étape de générer une clé publique et privée à l'aide de l'option `-g`:

```
$ python3 main.py -g
created keys
$ cat id_gamal.keys
313
244
247
158
257
77
17
```

Afin de générer les clés, il faut générer les valeurs suivante:

- un nombre premier `p` (dans notre cas, un nombre premier à 3 chiffre généré à l'aide du crible d'eratosthène);
- un entier `a` tel que `0 <= a < p-1` qui sera généré aléatoirement;
- un entier `g` appelé générateur. Sa particularité est qu'il est capable de générer tout les nombres entre 1 et `p` à l'aide de la formule `g ** n % p for n from 0 to p`;
- `A` qui est généré à l'aide de la formule `A ≡ g ** a % p`.
- `k` tel que `0 < k < p-2` et que `k` soit co-prime avec `p`. Nous utilisons une formule pour trouver le plus grand dénominateur commun (pgcd). Cette valeur est générée aléatoirement tant que le pgcd de `k` et `p` est `1`.
- `Y` qui est généré à l'aide de la formule `g ** k % p`
- `b` qui correspond au coefficient b du [Théorème de Bachet-Bézout](https://fr.wikipedia.org/wiki/Th%C3%A9or%C3%A8me_de_Bachet-B%C3%A9zout)

Une fois toutes les valeurs générées, la publique est `(p, g, A)` et la clé privée est `a`. Les valeurs `k`, `Y` et `b` sont utilisés dans les étapes suivantes.

Actuellement pour des soucis de simplification, toutes les valeurs cités ci dessus sont sauvés dans un unique fichier. Idéalement, il faudrait séparer la clé publique de la clé privée, comme le fait ssh par exemple:

```
$ ls -alp ~/.ssh/
...
-rw-------+  1 antoine  wheel  3434 Dec 11  2019 id_rsa
-rw-------+  1 antoine  wheel   749 Dec 11  2019 id_rsa.pub
...
```

## 2 - Signature d'un message

Ensuite, il est possible de signer un message à l'aide de l'option `-s`:

```
$ echo "Bonjour" > message.txt
$ python3 main.py -s id_gamal.keys message.txt
$ cat output
220
49
32
276
49
151
100
204
```

Le ficher `output` contient la signature qui peut ensuite être utilisée pour valider le message.

Afin de pouvoir signer un message, celui-ci est signé caractère par caractère à l'aide de la formule `(char - (a * Y)) * b) % (p - 1)`.

## 3 - Validation d'une signature

Finalement, il est possible de valider la signature d'un message à l'aide de l'option `-v`:

```
$ python3 main.py -v id_gamal.keys message.txt output
The signature matched
```

La vérification est effectuée à l'aide de la formule `(A ** Y) * (Y ** S)) % p == g ** char % p` ou `S` est le caractère signé et `char` est le caractère original. Si ces deux expression ont la même valeur et ce pour chaque caractères alors a signature est valide.

## 4 - Code source

```python
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

        # if all values (ingoring 0) are True then g is generator
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
    Return only the b coeficient of bezout
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

```

## Firewall


