void insertsort(int *t, int n)
{
  int i, p, j, x;

  for (i = 1; i < n; i++)
  {
    p = 0;

    while (t[p] < t[i])
      p++;

    x = t[i];

    for (j = i - 1; j >= p; j--)
      t[j + 1] = t[j];

    t[p] = x; 
  }
}