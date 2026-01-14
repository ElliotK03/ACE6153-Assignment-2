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
// int input[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

// helper functions
void shiftArrayContentsDown(int[]);
void floatArrayContent(int[], int);
int findIndex(int cache[], int value);
int findLowestCountIndex(int counter[], int *tie);
void updateLastWrittenIndex(int index);

// global variable
int lastWrittenIndices[CACHE_SIZE] = {-1, -1, -1};

// page replacement algorithms
int processInputFIFO(int input, int cache[], int _counter[]) {
  int index = findIndex(cache, input);

  // find hit
  if (index >= 0) {
    return index;
  }

  // handle miss
  index = findIndex(cache, -1);
  if (index >= 0) {
    cache[index] = input;
    updateLastWrittenIndex(index);

    return -1;
  } else {
    index = lastWrittenIndices[CACHE_SIZE - 1];

    cache[index] = input;
    updateLastWrittenIndex(index);

    return -1;
  }

  return -1;
}

int processInputLFU(int input, int cache[], int counter[]) {
  int index, tie;

  // find hit
  index = findIndex(cache, input);
  if (index >= 0) {
    counter[index] += 1;
    return index;
  }

  // find empty slot
  index = findIndex(cache, -1);
  if (index >= 0) {
    cache[index] = input;
    counter[index] = 1;

    updateLastWrittenIndex(index);
    return -1;
  }
  // vacate page with lowest count, and handle ties
  index = findLowestCountIndex(counter, &tie);

  if (tie == 0) {
    cache[index] = input;
    counter[index] = 1;
    updateLastWrittenIndex(index);
  } else {
    int countToVacate = counter[index];

    for (int i = (CACHE_SIZE - 1); i > 0; i--) {
      if (counter[lastWrittenIndices[i]] == countToVacate) {
        cache[lastWrittenIndices[i]] = input;
        counter[lastWrittenIndices[i]] = 1;
        floatArrayContent(lastWrittenIndices, i);
        break;
      } else continue;
    }
  }
  return -1;
}

int processInputLRU(int input, int cache[], int _counter[]) {
  // check for hit
  int index = findIndex(cache, input);
  if (index >= 0) {
    floatArrayContent(cache, index);
    return 0;
  }

  // handle miss
  if (cache[0] < 0) {
    cache[0] = input;
  } else {
    shiftArrayContentsDown(cache);
    cache[0] = input;
  }
  return -1;
}

int main() {

  int (*processInput)(int inputValue, int cache[], int counter[]);

  // assume the cache is holding pointers instead of ints,
  // value of -1 represent null
  int cache[CACHE_SIZE] = {-1, -1, -1};
  int counter[CACHE_SIZE] = {0, 0, 0};

  int inputSize = sizeof(input) / sizeof(input[0]);

  int algoSelection = 0;
  char algoName[5];

  int hitCount = 0;

  printf("Select Page Replacement Algorithm: \n");
  printf("1. FIFO \n");
  printf("2. LFU \n");
  printf("3. LRU \n");

  printf("Select number: ");
  scanf("%d", &algoSelection);

  switch (algoSelection) {
  case 1: {
    processInput = &processInputFIFO;
    strcpy(algoName, "FIFO");
    break;
  }
  case 2: {
    processInput = &processInputLFU;
    strcpy(algoName, "LFU");
    break;
  }
  case 3: {
    processInput = &processInputLRU;
    strcpy(algoName, "LRU");
    break;
  }
  default: {
    printf("Invalid selection, program will exit now.");
    return 1;
  }
  }

  printf("The algorithm you selected is %s\n\n", algoName);

  int hitIndex = -1;

  for (int i = 0; i < inputSize; i++) {
    // process the input
    hitIndex = processInput(input[i], cache, counter);

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

      if (algoSelection == 2) {
        if (counter[j] <= 0)
          printf("       --\t|");
        else
          printf("       %d\t|", counter[j]);
      }

      if (hitIndex == j) {
        printf(" HIT!");
        hitCount++;
      }
    }
    printf("\n ---------------");

    if (algoSelection == 2)
      printf("----------------");

    printf("\n\n");
  }

  printf("\nHit count is %d", hitCount);

  return 0;
}

// shifts the entire cache down, removes the last item from cache
void shiftArrayContentsDown(int cache[]) {
  for (int i = CACHE_SIZE - 1; i > 0; i--) {
    cache[i] = cache[i - 1];
  }

  cache[0] = -1;
}

// shifts the content in cache[index] to the top of the cache
void floatArrayContent(int cache[], int index) {
  if (index >= CACHE_SIZE || index < 0) {
    printf("Error! Invalid index %d", index);
    return;
  }

  int temp, i;

  for (i = index; i > 0; i--) {
    temp = cache[i - 1];
    cache[i - 1] = cache[i];
    cache[i] = temp;
  }
}

int findIndex(int cache[], int value) {

  for (int i = 0; i < CACHE_SIZE; i++) {
    if (cache[i] == value)
      return i;
  }

  return -1;
}

int findLowestCountIndex(int counter[], int *tie) {
  int index = 0, lowest = counter[0], i;
  *tie = 0;

  for (i = 1; i < CACHE_SIZE; i++) {
    if (counter[i] < lowest) {
      lowest = counter[i];
      index = i;
    }
  }

  for (i -= 1; i > 0; i--) {
    if (i == index)
      continue;

    if (counter[i] == lowest) {
      *tie = 1;
    }
  }

  return index;
}

void updateLastWrittenIndex(int index) {

  shiftArrayContentsDown(lastWrittenIndices);

  lastWrittenIndices[0] = index;
}
