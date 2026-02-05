#define _DEFAULT_SOURCE
#include <inttypes.h> // PRIu64
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>  // snprintf
#include <string.h> // memset
#include <unistd.h> // sbrk, write

#define EXTRA 256
#define BLOCK_SIZE 128
#define BUFFER_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

static void print_out(const char *format, void *data, size_t data_size) {
  char buf[BUFFER_SIZE];

  int len = snprintf(buf, BUFFER_SIZE, format,
                     (data_size == sizeof(uint64_t)) ? *(uint64_t *)data
                                                     : *(void **)data);

  if (len > 0) {
    (void)write(STDOUT_FILENO, buf, (size_t)len);
  }
}

int main(void) {
  void *base = sbrk(EXTRA);
  if (base == (void *)-1) {
    return 1;
  }

  struct header *block1 = (struct header *)base;
  struct header *block2 = (struct header *)((unsigned char *)base + BLOCK_SIZE);

  block1->size = BLOCK_SIZE;
  block1->next = NULL;

  block2->size = BLOCK_SIZE;
  block2->next = block1;

  unsigned char *data1 = (unsigned char *)block1 + sizeof(struct header);
  unsigned char *data2 = (unsigned char *)block2 + sizeof(struct header);

  size_t data_size = BLOCK_SIZE - sizeof(struct header);

  memset(data1, 0, data_size);
  memset(data2, 1, data_size);

  print_out("first block:        %p\n", &block1, sizeof(block1));
  print_out("second block:       %p\n", &block2, sizeof(block2));

  print_out("first block size:   %" PRIu64 "\n", &block1->size,
            sizeof(block1->size));
  print_out("first block next:   %p\n", &block1->next, sizeof(block1->next));

  print_out("second block size:  %" PRIu64 "\n", &block2->size,
            sizeof(block2->size));
  print_out("second block next:  %p\n", &block2->next, sizeof(block2->next));

  for (size_t i = 0; i < data_size; i++) {
    char buf[BUFFER_SIZE];
    int len = snprintf(buf, BUFFER_SIZE, "%u\n", (unsigned)data1[i]);
    if (len > 0)
      (void)write(STDOUT_FILENO, buf, (size_t)len);
  }

  for (size_t i = 0; i < data_size; i++) {
    char buf[BUFFER_SIZE];
    int len = snprintf(buf, BUFFER_SIZE, "%u\n", (unsigned)data2[i]);
    if (len > 0)
      (void)write(STDOUT_FILENO, buf, (size_t)len);
  }

  return 0;
}
