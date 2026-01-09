// ACE6153 Operating Systems
// Assignment 2
// Task A
// Group 4

// input string
// 6, 3, 5, 2, 5, 6, 3, 5, 9, 0, 0, 1, 2, 4, 7

#include <stdio.h>
#include <string.h>

#define CACHE_SIZE 3
// Cache is modeled using an array,
// index 0 is the start (top) of the cache

int input[] = {6, 3, 5, 2, 5, 6, 3, 5, 9, 0, 0, 1, 2, 4, 7};

// helper functions
void shiftCacheContentsDown(int []);
void floatCacheContent(int [], int);
int findIndex(int cache[], int value);

// page replacement algorithms
void inputFIFO(int input, int cache[], int _counter[]) {};

void inputLFU(int input, int cache[], int counter[]) {};

void inputLRU(int input, int cache[], int _counter[]) {};

int main() {

  void (*processInput)(int inputValue, int cache[], int counter[]);

  // assume the cache is holding pointers instead of ints,
  // negative values represent null
  int cache[CACHE_SIZE] = {1, -2, -4};
  int counter[CACHE_SIZE] = {1, 20, 0};

  int inputSize = sizeof(input) / sizeof(input[0]);

  int algoSelection = 0;
  char algoName[5];

  printf("Select Page Replacement Algorithm: \n");
  printf("1. FIFO \n");
  printf("2. LFU \n");
  printf("3. LRU \n");

  printf("Select number: ");
  scanf("%d", &algoSelection);
  
  switch (algoSelection) {
  case 1: {
    processInput = &inputFIFO;
    strcpy(algoName, "FIFO");
    break;
  }
  case 2: {
    processInput = &inputLFU;
    strcpy(algoName, "LFU");
    break;
  }
  case 3: {
    processInput = &inputLRU;
    strcpy(algoName, "LRU");
    break;
  }
  default: {
    printf("Invalid selection, program will exit now.");
    return 1;
  }
  }

  printf("The algorithm you selected is %s\n\n", algoName);

  for (int i = 0; i < inputSize; i++) {
    // process the input
    processInput(input[i], cache, counter);

    printf("Iteration %d: input = %d\n", i, input[i]);

    if (algoSelection == 2) {
      printf(" -------------------------------\n");
      printf("|    content\t|\tcounter\t|\n");
      printf(" -------------------------------");
    } else {
      printf(" ---------------\n");
      printf("|    content\t|\n");
      printf(" ---------------");
    }

    // print cache contents
    for (int j = 0; j < CACHE_SIZE; j++) {
      printf("\n");

      if (cache[j] < 0)
        printf("|\t--\t|");
      else
        printf("|\t%d \t|", cache[j]);

      if (algoSelection != 2)
        continue;

      if (counter[j] <= 0)
        printf("       --\t|");
      else
        printf("       %d\t|", counter[j]);
    }
    printf("\n ---------------");

    if (algoSelection == 2)
      printf("----------------");

    printf("\n\n");
  }

  return 0;
}

// shifts the entire cache down, removes the last item from cache
void shiftCacheContentsDown(int cache[]) {
  for (int i = CACHE_SIZE - 1; i > 0; i--) {
    cache[i] = cache[i-1];
  }
  
  cache[0] = -1;
}

// shifts the content in cache[index] to the top of the cache
void floatCacheContent(int cache[], int index) {
  if (index >= CACHE_SIZE || index < 0){ 
    printf("Error! Invalid index %d", index);
    return;
  }
  
  int temp, i;

  for (i=index; i > 0; i--) {
    temp = cache[i-1];
    cache[i-1] = cache[i];
    cache[i] = temp;
  }
}

int findIndex(int cache[], int value) {

  for (int i = 0; i < CACHE_SIZE; i++) {
    if (cache[i] == value) return i;
  }

  return -1;
}
