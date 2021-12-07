#include <stdio.h>
#include <stdlib.h>
#include <time.h> // contains prototype for function time

// enumeration constants represent game status
enum Status { CONTINUE, WON, LOST };

int rollDice(void); // function prototype

int main(void)
{
// randomize random number generator using current time
    srand(time(NULL));

    int myPoint; // player must make this point to win
    enum Status gameStatus;// can contain CONTINUE, WON, or LOST
    int sum = rollDice(); // first roll of the dice

    switch(sum) {
        case 7: case 11: // 7 and 11 is a winner
            gameStatus = WON;
            break;
        case 2:  case 3: case 12:// 2, 3 and 12 is a loser
            gameStatus = LOST;
            break;
        default:
            gameStatus = CONTINUE; // player should keep rolling
            myPoint = sum; // remember the point
            printf("Point is %d\n", myPoint);
            break; // optional
    }
    while (CONTINUE == gameStatus) { // player should keep rolling
        sum = rollDice(); // roll dice again
        if (sum == myPoint) { // win by making point
            gameStatus = WON;
        }
        else {
            if (7 == sum) { // lose by rolling 7
                gameStatus = LOST;
            }
        }
    }
    if (WON == gameStatus) { // did player win?
        puts("Player wins");
    }
    else { // player lost
        puts("Player loses");
    }
}

int rollDice(void){
    int die1 = 1 + (rand() % 6); // pick random die1 value
    int die2 = 1 + (rand() % 6); // pick random die2 value

    printf("Player rolled %d + %d = %d\n", die1, die2, die1 + die2);
    return die1 + die2; // return sum of dice
}

