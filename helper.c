#include "helper.h"

void substring(char *in, char *out, int begIdx) {
    int inIdx = begIdx;
    int outIdx = 0;
    char c = in[inIdx];
    while(c != '\0') {
        out[outIdx] = c;
        inIdx++;
        outIdx++;
        c = in[inIdx];
    }
    out[outIdx] = '\0';
}