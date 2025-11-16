#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tests.hpp"


int parse_arguments(int argc, char **argv) {
    if (argc < 2) {
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "correctness") == 0) {
            return 0;
        } else if (strcmp(argv[i], "performance") == 0) {
            return 1;
        } else if (strcmp(argv[i], "all") == 0) {
            return 2;
        } else {
            return -1;
        }
    }
    return -1;
}

int run_tests(int argc, char **argv) {
    int res = parse_arguments(argc, argv);
    switch (res) {
        case 0: {
            run_correctness();
            break;
        };
        case 1: {
            run_performance();
            break;
        };
        case 2: {
            if (run_correctness() == 0) {
                run_performance();
            };
            break;
        }
        default: {
            printf("Using: %s correctness|performance|all\n", argv[0]);
        }
    }
    return 0;
}
