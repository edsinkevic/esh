#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define IO_BUFFER_SIZE 256
#define EXIT_REPR "exit"
#define BG_REPR "bg"
#define WHITESPACE ' '
#define NEWLINE '\n'

char IO_BUFFER[IO_BUFFER_SIZE] = {0};
char IO_DOUBLE_BUFFER[IO_BUFFER_SIZE] = {0};

typedef enum {
  ESH_EXIT,
  ESH_UNKNOWN,
  ESH_BG
} ESH_CMD_TYPE;

typedef union {
  ESH_CMD_TYPE type;
} esh_cmd;

void clear_buffer(char *buffer) {
  for (size_t i = 0; i < IO_BUFFER_SIZE; ++i) {
    buffer[i] = 0;
  }
}

void cp_buffer(char *from_buffer, char *to_buffer) {
  for (size_t i = 0; i < IO_BUFFER_SIZE; ++i) {
    to_buffer[i] = from_buffer[i];
  }
}

void trim_buffer() {
  cp_buffer(IO_BUFFER, IO_DOUBLE_BUFFER);
  size_t j = 0;
  for (size_t i = 0; i < IO_BUFFER_SIZE; ++i) {
    char c = IO_DOUBLE_BUFFER[i];
    if (c == WHITESPACE) {
      continue;
    }

    if (c == NEWLINE) {
      continue;
    }

    IO_BUFFER[j] = IO_DOUBLE_BUFFER[i];
    ++j;
  }
}

int repr_eq(char *repr) {
  if (!repr) {
    return 0;
  }

  return strcmp(IO_BUFFER, repr) == 0;
}

esh_cmd parse_cmd() {
  trim_buffer();

  if (repr_eq(EXIT_REPR)) {
    esh_cmd cmd = {
      .type = ESH_EXIT,
    };

    return cmd;
  }

  if (repr_eq(BG_REPR)) {
    esh_cmd cmd = {
      .type = ESH_BG,
    };

    return cmd;
  }

  esh_cmd cmd = {
    .type = ESH_UNKNOWN,
  };

  return cmd;
}

void esh_exit() {
  printf("Exiting...\n");
  exit(EXIT_SUCCESS);
}

int main() {
  printf("Entering esh shell...\n\n");

  while (1) {
    printf("$/ ");
    
    errno = 0;
    if (!fgets(IO_BUFFER, IO_BUFFER_SIZE, stdin)) {
      if (errno) {
	fprintf(stderr, "Could not read input: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
      }

      esh_exit();
    };

    esh_cmd cmd = parse_cmd();

    switch (cmd.type) {
    case ESH_EXIT:
      esh_exit();
      break;
    case ESH_BG:
      printf("hello, world");
      break;
    case ESH_UNKNOWN:
      fprintf(stderr, "Unknown command\n\n");
      break;
    default:
      exit(EXIT_FAILURE);
    }

    printf("DEBUG: Buffers: \n[%s]\n[%s]\n", IO_BUFFER, IO_DOUBLE_BUFFER);

  }
  
  return EXIT_SUCCESS;
}
