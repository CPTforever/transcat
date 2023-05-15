#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <sys/ioctl.h>

#define BLUE "\033[38;2;91;206;205m"
#define PINK "\033[38;2;245;169;184m"
#define WHITE "\033[38;2;255;255;255m"
#define RESET "\e[0m"

#define BUFFER_LEN 4096
#define SEQUENCE_LENGTH 5

#define OPTIONS "f:"
const static char *sequence[5] = {BLUE, PINK, WHITE, PINK, BLUE};
static char position = 0;

int main(int argc, char **argv) {
    int opt;
    int off = 1; 
    size_t FLAG_WIDTH = 2;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch(opt) {
            case 'f':
                FLAG_WIDTH = strtoul(optarg, NULL, 10);
                if (FLAG_WIDTH < 1 || FLAG_WIDTH > 65565) FLAG_WIDTH = 1;
                off = 3;
        }
    }

    char buffer[BUFFER_LEN];

    for (int i = off; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror(argv[0]);
            continue;
        }
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while((read = getline(&line, &len, fp)) != -1) {
            printf("%s", sequence[position++ / FLAG_WIDTH]);
            printf("%s", line);
            printf(RESET);
            position %= (FLAG_WIDTH * SEQUENCE_LENGTH);
        }
    }
    if (argc == off) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        while((read = getline(&line, &len, stdin)) != -1) {
            printf("%s", sequence[position++ / FLAG_WIDTH]);
            printf("%s", line);
            printf(RESET);
            position %= (FLAG_WIDTH * SEQUENCE_LENGTH);
        }

    }
}
