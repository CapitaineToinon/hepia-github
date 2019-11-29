void bubblesort(int *t, int n)
{
  int x, i, p;

  do
  {
    p = 1;
    for (i = 0; i < n - 1; i++)
    {
      if (t[i] > t[i + 1])
      {
        x = t[i + 1];
        t[i + 1] = t[i];
        t[i] = x;
        p = 0;
      }
    }
  } while (p == 0);
}