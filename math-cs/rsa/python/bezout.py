'''
Return the gcd and bezout coeffs for 2 numbers
'''
def get_gcd_bezout(a, b):
  # Important to keep track if we swapped or not
  # since it changes which coef we care about.
  # a needs to be bigger than b so we swap if it's not the case
  swapped = False
  if (b > a):
    a, b = b, a
    swapped = True

  # Setup
  remainders = [
    a, # R0
    b, # R1
  ]
  xy = [
    [1, 0], # x0, y0
    [0, 1], # x1, y1
  ]

  while True:
    coeff = remainders[-2] // remainders[-1]
    remainder = remainders[-2] % remainders[-1]

    # We're done!
    if (remainder == 0):
      # swap xy back if needed
      x, y = xy[-1]
      if swapped:
        x, y = y, x
      # gcd, x, y
      return [remainders[-1], x, y]

    # We're not done, going to the next step
    x = xy[-2][0] - coeff * xy[-1][0]
    y = xy[-2][1] - coeff * xy[-1][1]

    remainders.append(remainder)
    xy.append([x, y])