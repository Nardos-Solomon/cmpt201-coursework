#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(void) {
  char *line = NULL;
  size_t bufferSize = 0;

  while (1) {
    printf("Enter programs to run.\n>");
    fflush(stdout);
    getline(&line, &bufferSize, stdin);

    // to remove the \n
    line[strcspn(line, "\n")] = '\0';
    if (line[0] == '\0') { // if they just pressed entered
      continue;
    }

    pid_t pid = fork();

    if (pid != 0) {
      waitpid(pid, NULL, 0);
    } else {
      execl(line, line, (char *)NULL);
      printf("execl failed to execute\n");
      _exit(1);
    }
  }
  return 0;
}
