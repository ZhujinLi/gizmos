#include <stdio.h>

extern int N;
extern int arr[];

int main() {
    int pos = 0;
    for (int i = 1; i < N; i++) {
        int diff = arr[i] != arr[pos];
        arr[pos += diff] = arr[i];
    }
    return pos + 1;
}
