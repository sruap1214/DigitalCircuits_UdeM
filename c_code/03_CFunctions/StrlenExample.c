// C program to demonstrate difference 
// between strlen() and sizeof()
#include<stdio.h>
#include<string.h>
  
int main()
{
    char str[] = "November";
    printf("Length of String is %lu\n", strlen(str));
    printf("Size of String is %lu\n", sizeof(str));
}