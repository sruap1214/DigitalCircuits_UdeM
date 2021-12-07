#include <stdio.h>
#define SIZE 20

size_t getSize(float *ptr); // prototype

int main(void)
{
    float array[SIZE]; // create array

    printf("The number of bytes in the array is %u"
            "\nThe number of bytes returned by getSize is %u\n",
            sizeof(array) , getSize(array) );
}
// return size of ptr
size_t getSize(float *ptr){
    return sizeof(ptr);
}