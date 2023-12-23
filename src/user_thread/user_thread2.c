#include <stdio.h>
#include <unistd.h>

int main() {
    int a = 0;
    while (1) {
        printf("Program running...%d\n", a);
		a++;
		long long s = 1;
		while(1){
			int l=1;
			int r=s;
			l=l+r;
			s++;
			if(s>100000000) break;
		}
    }
    return 0;
}
