#include "helper.h"

#include <stdio.h>
#include <string.h>

void test_substring();
void pass();
void fail();

int main() {
    test_substring();
    return 0;
}

void test_substring() {
    char *str = "hello";
    char out[strlen(str)];
    substring(str, out, 1);
    // fprintf(stderr, "%s\n", out);
    if (strcmp(out, "ello") == 0) {
        pass();
    }
    else {
        fail();
    }
}

void pass() {
    fprintf(stderr, "pass\n");
}

void fail() {
    fprintf(stderr, "fail\n");
}



