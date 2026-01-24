#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HISTSIZE 5

void printingHistory(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    printf("%s", history[i]);
  }
}

int appendHist(char *history[], int count, char *line) {
  if (count < HISTSIZE) {
    history[count] = line;
    count++;
  } else {
    free(history[0]);
    // shift everything up by one in the array
    for (int i = 1; i < HISTSIZE; i++) {
      history[i - 1] = history[i];
    }
    history[HISTSIZE - 1] = line;
  }
  return count;
}

int main(void) {
  char *history[HISTSIZE];
  int count = 0;
  char *line = NULL;
  size_t bufferSize = 0;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    size_t charsRead = getline(&line, &bufferSize, stdin);
    if (charsRead == -1) {
      break;
    }
    char *copiedLine = strdup(line);
    if (copiedLine == NULL) {
      perror("strdup failed");
      break;
    }

    count = appendHist(history, count, copiedLine);
    if (strcmp(copiedLine, "print\n") == 0) { // check if the strings are eqaul
      printingHistory(history, count);
    }
  }

  // free up memory
  for (int i = 0; i < count; i++) {
    free(history[i]);
  }
  free(line);

  return 0;
}
