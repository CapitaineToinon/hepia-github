import math
import random
import hashlib
import json
from warnings import resetwarnings

# Créer dans un TP "RSA". Réalisé avec Damian Boquette et Nicola Antonijevic
#inverse modulaire
                     # a, mod
def extendedEuclidean(phi, e):
    if e > phi:
        tmp = e
        e = phi
        phi = tmp

    # INIT
    r = []
    q = []
    x = []
    y = []
    cpt = 2

    r.insert(0, phi)
    r.insert(1, e)

    q.insert(0, 0)
    q.insert(1, 0)

    x.insert(0, 1)
    x.insert(1, 0)

    y.insert(0, 0)
    y.insert(1, 1)
    # END INIT

    while r[cpt-1] != 0:
        q.insert(cpt, int(r[cpt-2] / r[cpt-1]))
        r.insert(cpt, int(r[cpt-2]-(q[cpt] * r[cpt-1])))
        x.insert(cpt, int(x[cpt-2]-(q[cpt] * x[cpt-1])))
        y.insert(cpt, int(y[cpt-2]-(q[cpt] * y[cpt-1])))
        cpt += 1

    if y[cpt-2] < 0:
        return phi+(y[cpt-2])

    return y[cpt-2]

# https://fr.wikipedia.org/wiki/Exponentiation_modulaire
def exponentiation_modulaire(n,exp,mod):
    res = 1
    
    while exp > 0:
        if ((exp & 1) > 0): 
            res = modulo((res*n), mod)

        n = modulo((n*n),mod)
        exp = exp >> 1

    return res

def pgcd(a,b):
    while a!=b: 
        d=abs(b-a) 
        b=a 
        a=d

    return d

# 13 mod 3 = 1
# 13/3 = 4, reste 1
# 4*3 = 12 => 13-12 = 1
def modulo(a, mod):
    q = a // mod        # division entière 13/3 = 4
    return a - mod * q  # 13 - 3*4

# Permet de créer tous les nombres générateur modulo p
def generateur_etendu(p):
    potential_g = []
    actual_result = []
    actual_g = 2

    while actual_g < p-1:
        is_potential_g = 1
        actual_exponent = 0
        while actual_exponent < p-1:
            res = modulo(pow(actual_g,actual_exponent), p)

            if res in actual_result:
                is_potential_g = 0
                break
            else:
                actual_result.append(res)

            actual_exponent = actual_exponent+1

        if is_potential_g == 1:
            potential_g.append(actual_g)

        actual_result = []
        actual_g = actual_g+1

    return potential_g

def peek_a(p):
    list = range(1,p-1)
    return random.choice(list)

# Récupère un g générateur modulo p
def peek_g(p):
    list = generateur_etendu(p)
    return random.choice(list)

# k doit être pcgd de p-1
def peek_k(p):
    list = range(1,p-1)
    k = random.choice(list)
    
    while not pgcd(k, p-1) == 1:
        k = random.choice(list)

    return k

def calculate_S(m,a,Y,k,p):
    nba = (m-a*Y)
    nbb = extendedEuclidean(k,p-1)
    return modulo(nba*nbb, p-1)

def checkFile(filename):
    try:
        open(filename, "r")
    except IOError:
        print("Cannot open file...")
        exit()
    return 1

def getHashMd5File(filename):
    return int(hashlib.md5(open(filename, "rb").read()).hexdigest(),16)

def sign(filename):
    # Génération des modules
    p = 661 # Nombre premier (pas trop grand pour l'exercice)
    a = peek_a(p) # Clé privée, aléatoire entre 1 < a < p-1
    g = peek_g(p) # Sélection d'un g générateur modulo p aléatoire
    k = peek_k(p) # Sélection d'un k aléatoire entre 0 < k < p-1 
    m = getHashMd5File(filename)

    A = exponentiation_modulaire(g,a,p)
    Y = exponentiation_modulaire(g,k,p)
    S = calculate_S(m,a,Y,k,p)

    return [m,[p,g,A],[Y,S]]

def verif(m, public_key, sign):
    p, g, A = public_key
    Y, S = sign

    val1 = modulo(exponentiation_modulaire(A,Y,p) * exponentiation_modulaire(Y,S,p), p)
    val2 = exponentiation_modulaire(g,m,p)

    print("A^Y*Y^S = ", val1)
    print("module(g^m, p) = ", val2)

    if val1 == val2:
        print("Vérification réussie")
    else:
        print("Erreur dans la vérification")


if __name__ == "__main__":
    fichier = input("Indiquez le fichier à signer: ")
    checkFile(fichier)

    print("--------------- Signature ------------------")
    M, public_key, signature = sign(fichier)
    print("Clé public [p,g,A]: ", public_key)
    print("Signature [Y,S]: ", signature)

    print("---------------- Vérification -----------------")
    verif(M, public_key, signature)
