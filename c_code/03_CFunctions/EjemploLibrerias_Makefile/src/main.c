#include <stdio.h>
#include "LibreriaPropia.h"

int main(void)
{
    // loop 10 times and calculate and output square of x each time
    for (int x = 1; x <= 10; ++x) {
        printf("%d ",squareP(x)); // function call
    }
    puts("");
}
