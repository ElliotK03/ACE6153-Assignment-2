// ACE6153 Operating Systems
// Assignment 2
// Task A
// Group 4

// input string
// 6, 3, 5, 2, 5, 6, 3, 5, 9, 0, 0, 1, 2, 4, 7

#include <stdio.h>

#define CACHE_SIZE 3

int input[] = {6, 3, 5, 2, 5, 6, 3, 5, 9, 0, 0, 1, 2, 4, 7};

void inputFIFO(int input, int cache[], int _counter[]) {};

void inputLFU(int input, int cache[], int counter[]) {};

void inputLRU(int input, int cache[], int _counter[]) {};

int main() {

  void (*processInput)(int inputValue, int cache[], int counter[]);

  int cache[CACHE_SIZE] = {1, -2, -4};
  int counter[CACHE_SIZE] = {1, 20, 0};

  int inputSize = sizeof(input) / sizeof(input[0]);

  int algoSelection = 0;

  printf("Select Page Replacement Algorithm: \n");
  printf("1. FIFO \n");
  printf("2. LFU \n");
  printf("3. LRU \n");

  printf("Select number: ");
  scanf("%d", &algoSelection);

  printf("The algorithm you selected is");

  switch (algoSelection) {
  case 1: {
    processInput = &inputFIFO;
    printf(" FIFO\n\n");
    break;
  }
  case 2: {
    processInput = &inputLFU;
    printf(" LFU\n\n");
    break;
  }
  case 3: {
    processInput = &inputLRU;
    printf(" LRU\n\n");
    break;
  }
  default:
    return 1;
  }

  // printf("%d", algoSelection);

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
