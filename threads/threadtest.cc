// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993,2021 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include <cstdlib>
#include "copyright.h"
#include "system.h"

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}


// Begin code changes by Anthony Guarino
void InputTest(int i)
{
    char string[256];
    printf("Please input anything then press ENTER: ");
    scanf("%s", string);

    int type;

    // check if input is "0"
    if (strcmp(string, "0") == 0) { // checks if user input is 0 because atoi returns 0 if not an int
        type = 2; // just saying zero is a positive int. let the math nerd war commence.
    }
    else { // input is NOT 0
        double doubleAttempt = atof(string); // attempt to convert string to double

        if (doubleAttempt == 0) { // string could not be converted to double
            if (strlen(string) == 1) { // checking length of character (string)
                type = 0; // input is a character
            }
            else {
                type = 1; // input is a character string
            }
        }
        else { // string could be converted to long
            int intAttempt = atoi(string); // try to convert string to int

            if (intAttempt == 0) { // could not convert string to int, but is decimal
                if (doubleAttempt >= 0) {
                    type = 4; // mark type as positive decimal
                }
                else {
                    type = 5; // mark type as negative decimal
                }
            }
            else { // successfully converted string to int
                if (intAttempt >= 0) { // check sign of input
                    type = 2; // input is a positive int
                }
                else {
                    type = 3; // input is a negative int
                }
            }
        }
    }


    switch(type) {
        case 0: // character
            printf("\nThe input is a character.\n");
            break;
        case 1: // string
            printf("\nThe input is a character string.\n");
            break;
        case 2: // positive int
            printf("\nThe input is a positive integer.\n");
            break;
        case 3: // negative int
            printf("\nThe input is a negative integer.\n");
            break;
        case 4: // positive decimal
            printf("\nThe input is a positive decimal.\n");
            break;
        case 5: // negativedecimal
            printf("\nThe input is a negative decimal.\n");
    }

    currentThread->Finish();
}
// End code changes by Anthony Guarino


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering ThreadTest");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);

    // Begin code changes by Anthony Guarino

    Thread *t2 = new Thread("InputTest");

    t2->Fork(InputTest, 2);

    // End code changes by Anthony Guarino

 }


