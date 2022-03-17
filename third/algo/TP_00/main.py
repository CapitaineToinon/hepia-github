import numpy as np

debug = False


def main():
    W = np.matrix([
        # what we need to solve
        [0, 3, 8, np.inf, -4],
        [np.inf, 0, np.inf, 1, 7],
        [np.inf, 4, 0, np.inf, np.inf],
        [2, np.inf, -5, 0, np.inf],
        [np.inf, np.inf, np.inf, 6, 0]

        # example for pdf
        # [0, 2, 4, np.inf, 3],
        # [2, 0, 8, np.inf, 1],
        # [6, 2, 0, 4, 3],
        # [1, np.inf, np.inf, 0, 5],
        # [np.inf, np.inf, np.inf, 1, 0],

        # example from https://www.pairform.fr/doc/1/32/180/web/co/Floyd.html
        # [0, 3, np.inf, 3],
        # [2, 0, 2, 2],
        # [-2, np.inf, 0, 1],
        # [np.inf, 4, 4, 0],
    ])

    print(f"initial matrix")
    print(f"{W=}")

    W, P = floyd_warshall(W, print_k=True)

    print(f"resulting matrices")
    print(f"{W=}")
    print(f"{P=}")


def create_P(W: np.matrix):
    P = np.full_like(W, -1, dtype=np.int32)

    for index, x in np.ndenumerate(W):
        i, j = index
        if i == j or W.item(index) == np.inf or x == 0:
            continue
        P.itemset(index, i + 1)  # NOT 0 INDEXED!

    return P


def floyd_warshall(W: np.matrix, print_k: bool = False):
    W = np.copy(W)
    P = create_P(W)
    n = len(W)

    print("starting P")
    print(f"{P=}")

    for k in range(1, n + 1):
        for i in range(1, n + 1):
            for j in range(1, n + 1):
                current = W.item((i - 1, j - 1))
                candidate = W.item((i - 1, k - 1)) + W.item((k - 1, j - 1))
                if current > candidate:
                    debug and print(
                        f"Currently doing {i} -> {j} " +
                        f"for a cost of {current} " +
                        f"but we can do better going through {k} " +
                        f"for a cost of {candidate} instead. "
                        f"current value at {P.item((k - 1, j - 1))=}"
                    )
                    W.itemset((i - 1, j - 1), candidate)
                    P.itemset((i - 1, j - 1), P.item((k - 1, j - 1)))

        if print_k:
            print(f"for {k=}")
            print(f"{W=}")
            print(f"{P=}")


    return W, P


if __name__ == '__main__':
    main()
