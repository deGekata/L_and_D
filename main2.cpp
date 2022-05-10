#include <stdio.h>

int fn(int a) {
    {
        int a = 10;
    }   
    return a;
}


int main() {
    unsigned int main = 10;
    printf("%d", fn(15));
    return 0;
}