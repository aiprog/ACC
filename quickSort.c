extern int time (int t);
extern void srand (int seed);
extern int rand ();
extern void printf (char *format, ...);
  
void quickSort (int *arr, int beg, int end)
{
  int i, j, temp;

  if (beg >= end)
    return;

  i = beg;
  j = end;
  
  temp = arr[beg];
  while (i < j)
  {
    while (i < j && arr[j] >= temp)
      j = j - 1;
    arr[i] = arr[j];


    while (i < j && arr[i] <= temp)
      i = i + 1;
    arr[j] = arr[i];
  }
  arr[i] = temp;

  quickSort (arr, beg, i - 1);
  quickSort (arr, i + 1, end);
}

int main ()
{
  int i;
  int arr[10];

  srand (time (0));
  for (i = 0; i < 10; i = i + 1)
    arr[i] = rand () % 100;

  quickSort (arr, 0, 9);

  for (i = 0; i < 10; i = i + 1)
    printf ("%d ", arr[i]);
  printf ("\n");

  return 0;
}

