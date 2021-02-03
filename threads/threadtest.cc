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
#include <new>
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wwritable-strings"

// Begin code changes by Anthony Guarino with suggestion from M.M on StackExchange
// clear() just clears the buffer when using getchar() if multiple characters are inputted instead of one
void clear (void) {
    int c;
    while ((c = getchar() != '\n' && c != EOF)){}
}
// End code changes by Anthony Guarino

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

//----------------------------------------------------------------------
// InputChecker
// Takes in array of characters and returns the following int outputs
// 0 - char
// 1 - char array
// 2 - positive int
// 3 - negative int
// 4 - positive decimal
// 5 - negative decimal
// 6 - error
//----------------------------------------------------------------------
int InputChecker(char string[256]) {
    int type; // stores type of input
    bool dashFlag = false; // true if string[0] is '-'
    int decLoc = 0; // used to store location of decimal in array, null if non-existent
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
        else if (!isdigit(string[i])) {charFlag = true;}
    }

    if (length > 1 && decLoc == length - 1) {charFlag = true;} // if only decimal is the last pos in string, then char

    if (charFlag && length == 1) {type = 0;} // has chars, length =1 -> character
    else if (dashFlag && length == 1) {type = 0;} // only a dash -> character
    else if (charFlag && length > 1) {type = 1;} // has chars, length >1 -> character string
    else if (!charFlag && !dashFlag && decLoc == 0) {type = 2;} //no chars, no dash, no decimal -> positive int
    else if (!charFlag && dashFlag && decLoc == 0) {type = 3;} // no chars, has dash, no decimal, input
    else if (!charFlag && !dashFlag && decLoc > 0) {type = 4;} // no chars, no dash, has decimal -> positive decimal
    else if (!charFlag && dashFlag && decLoc > 0) {type = 5;} // no chars, has dash, has decimal -> negative decimal
    else {type = 6;} // error case

    return type;
}


void InputTest(int i)
{
    while (true) {
        char string[256];
        while (true) {
            memset(&string[0], 0, sizeof(string)); // clear string (fixes bug where fgets detects '\n')
            printf("Please input anything then press ENTER: ");
            fgets(string, sizeof(string), stdin);
            if (string[0] == '\n') {
                printf("That was not a valid input. Let's try this again...\n");
            }
            else {
                string[strlen(string) - 1] = '\000'; // removes trailing \n from fgets
                break;
            }
        }


        int type = InputChecker(string);

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
        while (true) {
            printf("\nNext input? (y/n): ");
            repeatInput = (char)getchar();
            clear();
            if (repeatInput == 'y' || repeatInput == 'n') {
                break;
            }
            else {
                printf("\nThat was not a valid input. Let's try this again...");
            }

        }
        if (repeatInput == 'n') {break;}
    }

    currentThread->Finish();
}


int yieldTimeGen() { // generates random number between 3-6
    // rand = (Random() % (max + 1 - min)) + min
    return (Random() % (4)) + 3;
}


void Shouter(int repeatTimes) {
    // Putting all of my quotes into one big array because I am lazy ¯\_(ツ)_/¯
    // Sorry for contributing to your early-onset carpal tunnel with this scrolling, TA. I owe you a drink.
    char quoteArray[50][128] = {
            "I am the eggman.",
            "You take your car to work, I take my board.",
            "All the girlies say I'm pretty fly (for a white guy).",
            "Wee ooh, I look just like Buddy Holly.",
            "Whoa, heaven let your light shine down.",
            "She's lump! She's lump! She's lump! She's in my head.",
            "I just wanna fly.",
            "I'm sorry, Ms. Jackson. (ooh!) I am for real.",
            "Choose not a life of imitation.",
            "And my time is a piece of wax falling on a termite that's choking on the splinters.",
            "I said maybeeeeeeeeeeee ya gonna be the one that saves meeeeeeeeeeeee.",
            "Ocean man, take me by the hand.",
            "G-Funk: where rhythm is life, and life is rhythm.",
            "Nothin' left to do but smile, smile, smile.",
            "I know who I want to take me home.",
            "I don't practice Santeria. I ain't got no crystal ball.",
            "Isn't it ironic? Don't ya think?",
            "I got two turntables and a microphone.",
            "Love is shaking on Shakedown Street. Used to be the heart of town.",
            "Peaches come from a can; they were put there by a man in a factory downtown.",
            "Mmmbop!",
            "With the taste of your lips, I'm on a ride.",
            "Lord, I was born a ramblin' man.",
            "This isn't who it would be if it wasn't who it is.",
            "Here's to you, Mrs. Robinson. Jesus loves you more than you would know.",
            "She can be a belly dancer, I don't need a true romancer.",
            "I shot the sheriff, but I didn't shoot the deputy.",
            "I GOT BLISTERS ON MY FINGERS!",
            "Oops, I did it again!",
            "GET UP! (Get on up.)",
            "Jerry was a racecar driver.",
            "Learn to work the saxophone; I play just what I feel.",
            "I cannot repeal the words of the golden eel.",
            "Going back home to the village of the sun. Out in back of Palmdale where the turkey farmers run.",
            "There's nothing on the top but a bucket and a mop and an illustrated book about birds.",
            "And I say, HEYYYYEYYYYYEYYYEYEY",
            "Never gonna give you up. Never gonna let you down.",
            "Bag it, tag it, sell it to the butcher at the store.",
            "Marky got with Sharon, Sharon got Sherice. She was sharin' Sharon's outlook on the topic of disease.",
            "Cool it now! You're gonna lose control.",
            "Sneaking Sally through the alley, tryin' to get away clean. Sneaking Sally through the alley, when out pops the queen.",
            "Nigel has his future in a British steel.",
            "Her left eye is lazy.",
            "I can ride my bike with no handlebars.",
            "The butter wouldn't melt so I put it in the pie.",
            "If they say, 'why, why', tell em that it's human nature.",
            "You won't let those robots eat me, Yoshimi.",
            "Bona fide hustler, making my name.",
            "I've seen footage. I stay noided.",
            "It goes, it goes, it goes, it goes, guillotineeeeeeee, YUH!"};
    // Back to your regularly-scheduled programming (pun intended)

    for (int i = 0; i < repeatTimes; i++) {
        int randomInRange = Random() % 50; // will take Random() result as a seed to choose number between 0-49
        char *threadName = currentThread->getName();
        printf("Thread %s: ", threadName);
        printf("%s", quoteArray[randomInRange]); // print random string from quoteArray
        printf("\n");
        for (int j = 0; j < yieldTimeGen(); j++) {
            currentThread->Yield();
        }
    }
    currentThread->Finish();
}


void ShoutThreads(int i) {
    char threadCountInput[16];
    char shoutCountInput[16];

    long int threadCount;
    long int shoutCount;

    while (true) {

        printf("\nEnter thread count (between 2 and 2147483646): ");
        scanf("%10s", threadCountInput);
        clear();

        printf("\nEnter shout count (between 2 and 2147483646): ");
        scanf("%10s", shoutCountInput);
        clear();


        // Checking and storing input
        if (InputChecker(threadCountInput) == 2 && InputChecker(shoutCountInput) == 2) // threadCount
        {
            threadCount = strtol(threadCountInput, nullptr, 10);
            shoutCount = strtol(shoutCountInput, nullptr, 10);

            if (threadCount > 1 && shoutCount > 1 && threadCount < 2147483647 && shoutCount < 2147483647) {
                break;
            }
            else {
                printf("\nThat was not a valid input. Let's try this again...");
            }
        }
        else {
            printf("\nThat was not a valid input. Let's try this again...");
        }
    }

    Thread *t;
    char *name;
    for (int i = 0; i < threadCount; i++) { // create threads
        name = new char[30];
        sprintf(name, "t%d", i);
        t = new Thread(name); // creates a new thread with the name created above
        t->Fork(Shouter, shoutCount);
    }


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
    char * stringTest = "String";
    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);

    // Begin code changes by Anthony Guarino
    if (aFlag == 1) {
        Thread *t2 = new Thread("InputTest");
        t2->Fork(InputTest, 0);
    }

    if (aFlag == 2) {
        Thread *t3 = new Thread("ShoutThreads");
        t3->Fork(ShoutThreads, 0);
    }

    // End code changes by Anthony Guarino

 }


#pragma clang diagnostic pop