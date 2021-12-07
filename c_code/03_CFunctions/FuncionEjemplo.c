#include <stdio.h>

int square(int y); // function prototype

int main(void)
{
    // loop 10 times and calculate and output square of x each time
    for (int x = 1; x <= 10; ++x) {
        printf("%d ",square(x)); // function call
    }
    puts("");
}

int square(int y) // y is a copy of the argument to the function
{
    return y * y; // returns the square of y as an int
}