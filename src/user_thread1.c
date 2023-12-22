#include <stdio.h>
#include <unistd.h>

int main() {
    int a = 0;
    while (1) {
        printf("Program running...%d\n", a);
	    a++;
        sleep(1); 
    }
    return 0;
}
