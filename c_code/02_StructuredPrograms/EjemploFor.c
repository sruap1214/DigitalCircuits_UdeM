#include<stdio.h>

int main(void){
    int contador = 0;

    /* 
        Imprimir los numeros del 7 al 77 en pasos de 7
    */
    printf("El primer for es: ");
    for(contador=7;contador<=77;contador+=7){
        printf("%d\t",contador);
    }
    /* 
        Imprimir los numeros del 20 al 2 en pasos de 2
    */
    printf("\nEl segundo for es: ");
    for(contador=20;contador>=2;contador-=2){
        printf("%d\t",contador);
    }
    /* 
        Imprimir los numeros del 44, 33, 22, 11, 0
    */
    printf("\nEl tercer for es: ");
    for(contador=44;contador>=0;contador-=11){
        printf("%d\t",contador);
    }

}