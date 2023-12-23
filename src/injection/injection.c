#include <stdlib.h>

extern void print();

extern void injection() {
    print();
    system("date");
}