#include <stdio.h>

int main(void)
{
    unsigned int counter = 1;
    
    do{
        printf("% u ", counter);
    }while(++counter<=10);
}