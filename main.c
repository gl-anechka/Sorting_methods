#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int b_comp = 0, h_comp = 0;  // счетчики кол-ва сравнений в сортировках
int b_mov = 0, h_mov = 0;    // счетчики кол-ва обменов в сортировках


/* Сортировка_1 "пузырек".
   Числа упорядочиваются по невозрастанию.
   Функция принимает на вход кол-во чисел массива и указатель на массив. */
void bubble_sort(int n, int *a);


/* Сортировка_2 пирамидальная.
   Числа упорядочиваются по невозрастанию.
   Функция принимает на вход кол-во чисел массива и указатель на массив. */
void heap_sort(int n, int *a);


/* Функция меняет местами два элемента массива.
   Принимает на вход указатель на начало массива и индексы элементов. */
void swap(int *a, int i, int j);


/* Функция просеивания элемента через кучу.
   Получает на вход указатель на начало массива, индекс обрабатываемого
   элемента и индекс последнего элемента (последнего ряда). */
void sift(int *a, int root, int end);


/* Генерация массива случайных чисел заданной длины в зависимости от
   параметра, передаваемого на вход:
   1 - элементы уже упорядочены (по невозрастанию);
   2 - элементы упорядочены в обратном порядке (по неубыванию);
   3 - расстановка элементов случайна.
   Функция возвращает указатель на сгенерированный массив. Память
   выделяется динамически. */
int* gener(int n, int flag);


/* Функция сравнения для qsort. Используется при генерации
   упорядоченного по невозрастанию массива. */
int comp1(const void* el1, const void* el2);


/* Функция сравнения для qsort. Используется при генерации
   упорядоченного по неубыванию массива. */
int comp2(const void* el1, const void* el2);


/* Функция копирования элементов массива a в новый массив.
   Принимает на вход указатель на массив a и его длину.
   Возвращает указатель на скопированный массив. */
int* copy(int *a, int n);


int main(void)
{
  int n;
  printf("Enter the number of elements.\n");
  scanf("%d", &n);

  int f;
  printf("Enter the number of the generating method.\n");
  scanf("%d", &f);

  int *arr1 = gener(n, f);    // генерация
  int *arr2 = copy(arr1, n);  // копирование массива

  printf("\nBUBBLE SORT\n\n");

  // "пузырек"
  for (int i = 0; i < n; i++)
  {
    printf("%d ", arr1[i]);
  }
  printf("\n");
  bubble_sort(n, arr1);  // собственно сортировка
  for (int i = 0; i < n; i++)
  {
    printf("%d ", arr1[i]);
  }
  printf("\n");
  printf("Number of comparisons: %d\n", b_comp);
  printf("Number of exchanges: %d\n", b_mov);

  printf("\nHEAP SORT\n\n");

  // пирамидальная
  for (int i = 0; i < n; i++)
  {
    printf("%d ", arr2[i]);
  }
  printf("\n");
  heap_sort(n, arr2);  // собственно сортировка
  for (int i = 0; i < n; i++)
  {
    printf("%d ", arr2[i]);
  }
  printf("\n");
  printf("Number of comparisons: %d\n", h_comp);
  printf("Number of exchanges: %d\n", h_mov);

  free(arr1);
  free(arr2);
  return 0;
}





void bubble_sort(int n, int *a)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = n - 2; j >= i; j--)
    {
      b_comp++;
      if (a[j] < a[j+1])
      {
        swap(a, j, j+1);
        b_mov++;
      }
    }
  }
}


void heap_sort(int n, int *a)
{
  // начинаем с нижнего ряда пирамиды
  for (int i = (n - 2) / 2; i >= 0; i--)
  {
    sift(a, i, n - 1);
  }

  // просеиваем остальные элементы
  for (int i = n - 1; i > 0; i--)
  {
    swap(a, 0, i);
    h_mov++;
    sift(a, 0, i - 1);
  }
}


void swap(int *a, int i, int j)
{
  int temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}


void sift(int *a, int root, int end)
{
  int min_child;  // индекс минимального дочернего элемента
  int flag = 0;   // флаг: элемент просеян или не просеян

  // пока не последний ряд и элемент не просеян
  while ((root * 2 + 1 <= end) && (!flag))
  {
    // последний ряд
    if (root * 2 + 1 == end)
    {
      min_child = root * 2 + 1;
    }
    // иначе меньший из дочерних элементов
    else if (a[root * 2 + 1] < a[root * 2 + 2])
    {
      min_child = root * 2 + 1;
      h_comp++;
    }
    else
    {
      min_child = root * 2 + 2;
      h_comp++;
    }

    // дочерний элемент меньше родителя, тогда меняем их
    if (a[root] > a[min_child])
    {
      swap(a, root, min_child);
      h_mov++;
      h_comp++;
      root = min_child;
    }
    // иначе элемент просеян
    else
    {
      flag = 1;
    }
  }
}


int* gener(int n, int flag)
{
  // инициализация генератора
  srand(time(NULL));

  int *arr = (int*) malloc(sizeof(int) * n);

  // генерация случайного массива
  for (int i = 0; i < n; i++)
  {
    if (rand() % 10 == 1)
    {
      arr[i] = -1 * rand() * rand();
    }
    else
    {
      arr[i] = rand() * rand();
    }
  }

  // по невозрастанию
  if (flag == 1)
  {
    qsort(arr, n, sizeof(int), comp1);
  }
  // по неубыванию
  else if (flag == 2)
  {
    qsort(arr, n, sizeof(int), comp2);
  }

  return arr;
}


int comp1(const void* el1, const void* el2)
{
  int a = *((const int*) el1);
  int b = *((const int*) el2);
  return b - a;
}


int comp2(const void* el1, const void* el2)
{
  int a = *((const int*) el1);
  int b = *((const int*) el2);
  return a - b;
}


int* copy(int *a, int n)
{
  int *b = (int*) malloc(sizeof(int)*n);
  for (int i = 0; i < n; i++)
  {
    b[i] = a[i];
  }
  return b;
}

