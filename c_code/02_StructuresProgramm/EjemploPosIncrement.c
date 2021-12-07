#include<stdio.h>

int main(void){
    int a=1; //storing a number in a
    int x=a++; //using the post-increment operator

    printf("El valor de a es %d y el valor de x es %d",a,x);

    return 0;
}