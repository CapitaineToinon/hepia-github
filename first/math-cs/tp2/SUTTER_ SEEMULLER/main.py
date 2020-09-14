import numpy as np
from math import pow
from enum import Enum
from functools import reduce

class Direction(Enum):
    Upper = 1
    Lower = 2

def truncateMatrix(source, colToDelete):
    '''Removes the first line and a given column, returns the updated matrix'''

    matrix = np.delete(source, 0, 0)
    matrix = np.delete(matrix, colToDelete, 1)
    return matrix

def det(matrix):
    '''Finds and returns the determinant of a given n*n matrix.'''
    
    # Use a nested function in order to treat the absolute value only at the end of our recursive function
    def recusive(matrix):
        if (len(matrix) <= 2):
            return (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1])
        else:
            return sum([
                matrix[0][x] * pow(-1, 1+x) * recusive(truncateMatrix(matrix, x))
                for x, item in enumerate(matrix[0])
            ])
    
    return abs(recusive(matrix))

def detFromU(matrix):
    '''compute the determinant using U instead'''
    return abs(np.prod([
        matrix[i][i]
        for i in range(0, len(matrix))
    ]))

def computeLU(matrix):
    '''Compute L and U for a matrix'''
    N = len(matrix[0])
    
    # Start by initializing our output arrays
    # L = I(N*N) and U = A (where A is our input array)
    L = np.eye(N, dtype=float)
    U = np.copy(matrix)

    for i in range(0, N):
        # ! Warning check if U[i,i] = 0, we have an issue
        for j in range(i+1, N):
            R = -U[j][i]/U[i][i]
            L[j][i] = -R

            # Adding U's lines
            for k in range(0, N):
                U[j][k] = U[j][k] + R * U[i][k]

    return [L, U]

def solveSystem(matrix, result, order):
    '''Solves an equation system stored in a U or L matrix'''
    answers = []
    m = matrix.copy();
    b = result.copy();

    if (order == Direction.Upper):
        # If we have to read an "upper" matrix we translate it to treat it like a lower
        # we also make sure to invert our "b" matrix, to read results from top to bottom
        m = np.fliplr(np.flipud(m))
        b = np.flipud(b)
        
    for i in range(0, len(m)):
        values = sum([answers[j] * m[i][j] for j in range(0, i)])
        answers.append((b[i] - values) / m[i][i])

    if (order == Direction.Upper):
        answers = np.flipud(answers)

    return np.array(answers, dtype=float)

if __name__ == "__main__": 
    A = np.array([
        77, 339, 324, 271, 816, 347, 614, 594, 388, 773,
        95, 415, 397, 332, 1000, 426, 752, 727, 475, 947,
        57, 250, 239, 200, 602, 256, 453, 438, 286, 571,
        80, 350, 335, 280, 843, 359, 634, 613, 400, 799,
        89, 391, 374, 312, 941, 400, 707, 684, 447, 891,
        28, 123, 117, 98, 296, 126, 222, 215, 140, 280,
        62, 274, 262, 219, 659, 281, 496, 479, 313, 625,
        36, 160, 153, 128, 386, 164, 290, 281, 183, 365,
        20, 89, 85, 71, 215, 91, 162, 156, 102, 204,
        6, 27, 26, 22, 66, 27, 49, 47, 31, 62,
    ], dtype=float)

    B = np.array([         
        61,
        -74,
        -28,
        -73,
        -3,
        27,
        -94,
        -11,
        -89,
        76,
    ], dtype=float)

    # Convert our input 1D array to 2D square array (3x3, 10x10, etc...)
    A = np.reshape(A, (-1, len(B)))

    print("Find the determinant of our array to see if it is inversible. This may take a while...")
    detOfA = det(A)
    
    if (detOfA == 0):
        raise ValueError("The matrix is not inversible, det(A) = 0")
    
    print("The matrix is inversible, det(A) = ", detOfA)
    print("If the matrix is inversible, we can use 'Gaussian Elimination'")
    
    # Compute our Lower "L" and Upper "U" matrices
    [L, U] = computeLU(A)
    print("\nL = \n", L)
    print("\nU = \n", U)

    print("detOfA but using U this time: ", detFromU(U))

    print("Now we just have to find x")
    '''
    A*x = b
    A = L*U
    
    THUS :  
    L*U*x = b
    
    THUS :
    L*(U*x) = b
    L*y = b
    
    We know that U*x = y

    Thus, we find "x" using U*x = y
    '''

    # Find y first using (L*y = b)
    y = solveSystem(L, B, order=Direction.Lower)
    print("y: ", y)

    # We can now find x using (U*x = y)
    x = solveSystem(U, y, order=Direction.Upper)
    print("x: ", x)
