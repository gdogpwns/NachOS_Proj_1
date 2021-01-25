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
#include <cctype>
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
    bool run = true;

    while (run) {
        char string[256];
        printf("Please input anything then press ENTER: ");
        scanf("%s", string);

        int type; // stores type of input

        /* Code block, you were too good for this world. You will be missed.
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
       */

        bool dashFlag = false; // true if string[0] is '-'
        int decLoc = 0; // used to store location of decimal in array, null if non-existent
        bool postFlag = false; // true if numbers proceed decimal
        bool charFlag = false; // true if a character or character string is detected
        int length = strlen(string); // length of string

        if (string[0] == '-') {dashFlag = true;} // check if string starts with '-'
        else if (!isdigit(string[0])) {charFlag = true;} // if first item is NOT '-' but is char, set charFlag to true

        for (int i = 1; i < length; i++) { // starting counter at 1 since we've already checked what that is
            if (string[i] == '.' && decLoc == 0) { // check for one AND ONLY one decimal
                decLoc = i;
            }
            else if (string[i] == '.' && decLoc != 0) { // if there's >1 decimal, set charFlag and break
                charFlag = true;
                break;
            }
        }

        if (length > 1 && decLoc == length - 1) {charFlag = true;} // if only decimal is the last pos in string, then char

        if (charFlag && length == 1) {type = 0;} // has chars, length =1 -> character
        else if (charFlag && length > 1) {type = 1;} // has chars, length >1 -> character string
        else if (!charFlag && !dashFlag && decLoc == 0) {type = 2;} //no chars, no dash, no decimal -> positive int
        else if (!charFlag && dashFlag && decLoc == 0) {type = 3;} // no chars, has dash, no decimal -> negative int
        else if (!charFlag && !dashFlag && decLoc > 0) {type = 4;} // no chars, no dash, has decimal -> positive decimal
        else if (!charFlag && dashFlag && decLoc > 0) {type = 5;} // no chars, has dash, has decimal -> negative decimal
        else {type = 6;} // error case


        switch(type) {
            case 0: // character
                printf("\nThe input is a character.\n");
                break;
            case 1: // string
                printf("\nThe input is a character array.\n");
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
            case 5: // negative decimal
                printf("\nThe input is a negative decimal.\n");
                break;
            case 6: // error :(
                printf("\nWell, you caught me slipping. I hadn't considered this case :(\n");
                break;
        }

        // repeats section if wanted
        char repeatInput;
        printf("Next input (y/n): ");
        scanf(" %c", &repeatInput);
        if (repeatInput == 'y') {run = true;}
        else {run = false;}
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


