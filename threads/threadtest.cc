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
#include "synch.h"

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

// Begin Project 1

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
    else if (!charFlag && dashFlag && decLoc == 0) {type = 3;} // no chars, has dash, no decimal -> negative int
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
            printf("Please input anything (<256 characters) then press ENTER: ");
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

// Should deprecate this function
int yieldTimeGen() { // generates random number between 3-6
    // rand = (Random() % (max + 1 - min)) + min
    return (Random() % (4)) + 3;
}

void YieldThreadRandom() {
    int yieldTime = (Random() % (4)) + 3;
    for (int i = 0; i < yieldTime; i++) {
        currentThread->Yield();
    }
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

// End Project 1

// Begin Project 2

// Function to make getting string inputs easier
char* GetStringInput(char toPrint[256]) {
    //char string[256];
    char *string = (char*)malloc(256);
    while (true) {
        memset(&string[0], 0, sizeof(string)); // clear string (fixes bug where fgets detects '\n')
        printf(toPrint);
        fgets(string, 256, stdin);
        if (string[0] == '\n') {
            printf("That was not a valid input. Let's try this again...\n");
        }
        else {
            string[strlen(string) - 1] = '\000'; // removes trailing \n from fgets
            break;
        }
    }
    return string;
}

int chopsticksAmt;
bool * chopsticks;
int meals;

void ReleaseChopsticksBusyWaiting(char *name, bool *chopsticksHeld) {
    int philosopherInt = atoi(name);
    if (chopsticksHeld[0]) {
        printf("\nPhilosopher %s is releasing the chopstick on their left.", name);
        chopsticks[philosopherInt] = true;
        // Commenting out this print because it is unnecessary in my opinion
        //printf("\nPhilosopher %s has released the chopstick on their left.", name);
    }

    if (chopsticksHeld[1]) {
        printf("\nPhilosopher %s is releasing the chopstick on their right.", name);
        chopsticks[(philosopherInt + 1) % chopsticksAmt] = true;
        // Commenting out this print because it is unnecessary in my opinion
        //printf("\nPhilosopher %s has released the chopstick on their right.", name);
    }

}

void BeginThinking(char *name) {
    printf("\nPhilosopher %s is thinking.", name);
    for(int i = 0; i < yieldTimeGen(); i++) {
        currentThread->Yield();
    }
}

void LeaveTable(char *name) {
    printf("\nPhilosopher %s is leaving the table.\n", name); // I don't like printing the blank line like this
    currentThread->Finish();
}

void RunBusyWaitingPhilosopher(int which) {
    char *name = currentThread->getName();
    int philosopherInt = atoi(name);
    bool chopsticksHeld[2] = {false, false}; // indicates if philosopher is holding left and/or right chopstick

    printf("\nPhilosopher %s has sat at the table.", name);
    currentThread->Yield();

    /*
     * OK, here's the move:
     * I will check before every pickup if the last meal has been eaten
     * If all the meals are eaten, I'm gonna send them to release each chopstick they're holding
     */
    while (meals >= 1) {
        printf("\nPhilosopher %s is hungry and trying to pick up the chopstick on their left.", name);
        while(!chopsticks[philosopherInt]) {currentThread->Yield();}
        if (meals >= 1) {
            chopsticks[philosopherInt] = false;
            chopsticksHeld[0] = true;
            printf("\nPhilosopher %s has successfully picked up the chopstick on their left.", name);
        }
        else {break;}


        printf("\nPhilosopher %s is hungry and trying to pick up the chopstick on their right.", name);
        while(!chopsticks[(philosopherInt + 1) % chopsticksAmt]) {currentThread->Yield();}
        if (meals >= 1) {
            chopsticks[(philosopherInt + 1) % chopsticksAmt] = false;
            chopsticksHeld[1] = true;
            printf("\nPhilosopher %s has successfully picked up the chopstick on their right.", name);
        }
        else {break;}

        printf("\nPhilosopher %s is now eating.", name);
        meals--;
        printf("\n%d meal(s) are remaining.", meals);
        for (int i = 0; i < yieldTimeGen(); i++) {
            currentThread->Yield();
        }
        printf("\nPhilosopher %s is done eating.", name);

        ReleaseChopsticksBusyWaiting(name, chopsticksHeld);
        chopsticksHeld[0] = false;
        chopsticksHeld[1] = false;
        BeginThinking(name);
    }
    ReleaseChopsticksBusyWaiting(name, chopsticksHeld);
    chopsticksHeld[0] = false;
    chopsticksHeld[1] = false;


    // checking to see if all chopsticks have been released
    for (int i = 0; i < chopsticksAmt; i++) {
        if (!chopsticks[i]) {
            i--;
            currentThread->Yield();
        }
    }
    LeaveTable(name);

}


Semaphore **semChopstick;
Semaphore *chopstickMutex = new Semaphore("chopstickMutex", 1);
int readyToLeave = 0;
Semaphore *readyToLeaveSem = new Semaphore("readyToLeave", 0);
// keeps track of WHO holds WHICH chopstick
// NULL is indicated by chopsticksAmt + 1
int * holdingChopstick;

void ReleaseChopsticksSemaphore(char *name) {
    int philosopherInt = atoi(name);

    if (holdingChopstick[philosopherInt] == philosopherInt) {
        printf("\nPhilosopher %s is releasing the chopstick on their left.", name);
        semChopstick[philosopherInt]->V();
        // Commenting out this print because it is unnecessary in my opinion
        //printf("\nPhilosopher %s has released the chopstick on their left.", name);
    }

    if (holdingChopstick[(philosopherInt + 1) % chopsticksAmt] == philosopherInt) {
        printf("\nPhilosopher %s is releasing the chopstick on their right.", name);
        semChopstick[(philosopherInt + 1) % chopsticksAmt]->V();
        // Commenting out this print because it is unnecessary in my opinion
        //printf("\nPhilosopher %s has released the chopstick on their right.", name);
    }
}

void RunSemaphorePhilosopher(int which) {
    char *name = currentThread->getName();
    int philosopherInt = atoi(name);

    printf("\nPhilosopher %s has sat at the table.", name);
    currentThread->Yield();


    while (meals >= 1) {
        printf("\nPhilosopher %s is hungry and trying to pick up the chopstick on their left.", name);
        if (meals >= 1) {
            semChopstick[philosopherInt]->P();
            chopstickMutex->P();
            holdingChopstick[philosopherInt] = philosopherInt;
            chopstickMutex->V();
            printf("\nPhilosopher %s has successfully picked up the chopstick on their left.", name);
        }
        else {break;}


        printf("\nPhilosopher %s is hungry and trying to pick up the chopstick on their right.", name);
        if (meals >= 1) {
            semChopstick[(philosopherInt + 1) % chopsticksAmt]->P();
            chopstickMutex->P();
            holdingChopstick[(philosopherInt + 1) % chopsticksAmt] = philosopherInt;
            chopstickMutex->V();
            printf("\nPhilosopher %s has successfully picked up the chopstick on their right.", name);
        }
        else {break;}


        printf("\nPhilosopher %s is now eating.", name);
        chopstickMutex->P();
        meals--;
        chopstickMutex->V();
        printf("\n%d meal(s) are remaining.", meals);
        for (int i = 0; i < yieldTimeGen(); i++) {
            currentThread->Yield();
        }
        printf("\nPhilosopher %s is done eating.", name);

        ReleaseChopsticksSemaphore(name);

        BeginThinking(name);
    }
    ReleaseChopsticksSemaphore(name);

    chopstickMutex->P();
    readyToLeave++;
    chopstickMutex->V();
    if (readyToLeave < chopsticksAmt) {
        readyToLeaveSem->P();
    }
    readyToLeaveSem->V();
    LeaveTable(name);
}

void MakePhilosopher(int which) {
    // Get amount of philosophers
    char pInputString[256];
    int pAmt;
    while(true) {
        strcpy(pInputString, GetStringInput("\nHow many philosophers will be dining?: "));
        if (InputChecker(pInputString) == 2) {
            pAmt = atoi(pInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }

    // Get amount of meals
    char mInputString[256];
    while(true) {
        strcpy(mInputString, GetStringInput("\nHow many meals will they be eating?: "));
        if (InputChecker(mInputString) == 2) {
            meals = atoi(mInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }

    // Guilt trip the end-user if they cook food for an empty table
    if (pAmt == 0) {
        printf("\nThe table has been set for nobody. The food is going bad! Remember: only YOU can end food waste!\n");
        currentThread->Finish();
    }

    // Guilt trip the end-user if they didn't invite any of the philosopher's friends
    else if (pAmt == 1) {
        printf("\nA lone philosopher ponders how to eat with only one chopstick. Too hungry to ponder further, they order a pizza instead.\n");
        currentThread->Finish();
    }

    // Guilt trip the end-user if they were too cheap to make any food
    if (meals == 0) {
        printf("\nI can't believe you would invite %d friend(s) over then let them starve! You're an awful host.\n", pAmt);
        currentThread->Finish();
    }

    chopsticksAmt = pAmt;
    if (which == 0) {
        // populate chopsticks list
        chopsticks = new bool[pAmt];
        for (int i = 0; i < (pAmt); i++) {
            chopsticks[i] = true;
        }

        // Create and fork threads
        Thread *t;
        char *name;
        for (int i = 0; i < pAmt; i++) { // create threads
            name = new char[30];
            sprintf(name, "%d", i);
            t = new Thread(name); // creates a new thread with the name created above
            t->Fork(RunBusyWaitingPhilosopher, 0);
        }
    }
    else if (which == 1) {
        // populate semChopstick list and holdingChopstick list
        semChopstick = new Semaphore * [pAmt];
        holdingChopstick = new int [pAmt];


        Thread *t;
        char *name;
        for (int i = 0; i < pAmt; i++) {
            name = new char[30];
            sprintf(name, "%d", i);

            semChopstick[i] = new Semaphore(name, 1);
            holdingChopstick[i] = pAmt; // NULL (see note at declaration)

            t = new Thread(name);
            t->Fork(RunSemaphorePhilosopher, 0);
        }

    }

    currentThread->Finish();
}

/*
 * Holds the message text and a bool if there is a message in that slot or not
 */
struct Message {
    char* message = new char[256];
    bool holdsMessage = false;
    Semaphore *sem = new Semaphore("mailbox", 1);
};

/*
 * Creates a struct to hold mail in a mailbox. Contains a Semaphore and an int to hold a message.
 * Using an int instead of a string because I am too lazy to think about strings in C right now.
 */
struct Mailbox {
    Message **messages; // to hold list of Messages, amount S
};

char* GetRandomQuote() {
    char *quoteArray[50] = {
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

    int randomInRange = Random() % 50; // will take Random() result as a seed to choose number between 0-49
    return quoteArray[randomInRange];
}


Mailbox **boxArr;
int amtCustomers;
int amtMessages;
int amtToSend;

void RunPostOffice(int which) {
    char *name = currentThread->getName();
    int custInt = atoi(name);
    int mailTo = 0;

    while (true) {
        printf("\nCustomer %s has entered the post office.", name);

        for (int i = 0; i < amtMessages; i++) {
            if (i == amtMessages - 1 && !boxArr[custInt]->messages[i]->holdsMessage) {
                printf("\nCustomer %s's mailbox is empty.", name);
            }
            else if (boxArr[custInt]->messages[i]->holdsMessage) { // if a message is found, read it
                printf("\nCustomer %s has mail!", name);
                printf("\nMessage reads: %s\n", boxArr[custInt]->messages[i]->message); // print message
                boxArr[custInt]->messages[i]->message = nullptr; // erase message
                boxArr[custInt]->messages[i]->holdsMessage = false; // mark space as empty
                boxArr[custInt]->messages[i]->sem->V();
                currentThread->Yield();
            }
        }

        if (amtToSend > 0) {
            mailTo = (Random() % (amtCustomers));
            while (mailTo == custInt) {mailTo = (Random() % (amtCustomers));} // makes sure that mailTo != custInt

            printf("\nSending message to Customer %d...", mailTo);
            for (int i = 0; i < amtMessages; i++) {
                if (!boxArr[mailTo]->messages[i]->holdsMessage) {
                    boxArr[mailTo]->messages[i]->sem->P();
                    //boxArr[mailTo]->messages[i]->message = GetRandomQuote();
                    boxArr[mailTo]->messages[i]->message = GetRandomQuote();
                    boxArr[mailTo]->messages[i]->holdsMessage = true;
                    printf("\nMessage sent successfully!");
                    amtToSend--;
                    printf("\n%d message(s) left to send.", amtToSend);
                    break;
                }
                if (i == amtMessages - 1) {
                    printf("\nMessage send failed: not enough space.");
                }
            }

            printf("\nCustomer %s is leaving the post office.\n", name);
            YieldThreadRandom();
        }
        else if (amtToSend == 0) {
            printf("\nCustomer %s is leaving the post office.\n", name);
            currentThread->Finish();
            break; // this break is just to stop an Endless Loop warning
        }
    }
}

void MakePostOffice(int which) {
    // Get amount of people
    char pInputString[256];
    int pAmt;
    while(true) {
        strcpy(pInputString, GetStringInput("\nHow many people have mailboxes?: "));
        //pInputString = GetStringInput("\nHow many people have mailboxes?: ");
        if (InputChecker(pInputString) == 2) {
            pAmt = atoi(pInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }
    amtCustomers = pAmt;

    // Get mailbox capacity
    char sInputString[256];
    int sAmt;
    while(true) {
        strcpy(sInputString, GetStringInput("\nHow many letters can a mailbox hold?: "));
        if (InputChecker(sInputString) == 2) {
            sAmt = atoi(sInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }
    amtMessages = sAmt;

    // Get number of messages to be sent
    char mInputString[256];
    int mAmt;
    while(true) {
        strcpy(mInputString, GetStringInput("\nHow many messages will be sent?: "));
        if (InputChecker(mInputString) == 2) {
            mAmt = atoi(mInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }
    amtToSend = mAmt;

    boxArr = new Mailbox * [pAmt];

    // Create and fork threads
    Thread *t;
    char *name;
    for (int i = 0; i < pAmt; i++) { // create threads and mailboxes
        boxArr[i] = new Mailbox; // create new mailbox at boxArr[i]
        boxArr[i]->messages = new Message * [sAmt]; // creates sAmt of Message holders in boxArr[i]
        for (int j = 0; j < sAmt; j++) {
            boxArr[i]->messages[j] = new Message;
        }

        name = new char[30];
        sprintf(name, "%d", i);
        t = new Thread(name); // creates a new thread with the name created above
        t->Fork(RunPostOffice, 0);
    }

}

Semaphore *area = new Semaphore("rw shared semaphore", 1);
Semaphore *mutex = new Semaphore("readCount mutex", 1);
Semaphore *readerCap;
int maxReaders = 0;
int remainingWriters = 0;
int readCount = 0;
int totalReads = 0;
void RunReader(int which) {
    char *name = currentThread->getName();
    int threadInt = atoi(name);


    //printf("\nReader %s is trying to enter the buffer area.", name);
    if (remainingWriters > 0) {
        readerCap->P();
    }


    mutex->P();

    // Begin reader entry
    readCount++;
    if (readCount == 1) {area->P();}
    mutex->V();

    printf("\nReader %s is reading in the buffer area.", name);
    currentThread->Yield();

    mutex->P();
    totalReads++;
    printf("\nReader %s finished reading.", name);
    printf(" Total reads: %d", totalReads);
    // Begin reader exit
    readCount--;
    if (readCount == 0) {area->V();}
    mutex->V();

    currentThread->Finish();
}

void RunWriter(int which) {
    char *name = currentThread->getName();
    int threadInt = atoi(name);

    //printf("\nWriter %s is trying to enter the buffer area.", name);
    area->P();

    mutex->P();
    totalReads = 0;
    remainingWriters--;
    mutex->V();

    printf("\nWriter %s is writing in the buffer area.", name);
    currentThread->Yield();
    printf("\nWriter %s is done writing and leaving the area.\n", name);

    for (int i = 0; i < maxReaders; i++) {
        readerCap->V();
    }

    area->V();
    currentThread->Finish();
}

void MakeReaderWriter(int which) {
    // Get amount of readers (R)
    char rInputString[256];
    int rAmt;
    while(true) {
        strcpy(rInputString, GetStringInput("\nHow many readers?: "));
        if (InputChecker(rInputString) == 2) {
            rAmt = atoi(rInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }

    // Get amount of writers (W)
    char wInputString[256];
    int wAmt;
    while(true) {
        strcpy(wInputString, GetStringInput("\nHow many writers?: "));
        if (InputChecker(wInputString) == 2) {
            wAmt = atoi(wInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }

    // Get max number of readers that can read the file at once (N)
    char nInputString[256];
    int nAmt;
    while(true) {
        strcpy(nInputString, GetStringInput("\nHow many readers can read at once?: "));
        if (InputChecker(nInputString) == 2) {
            nAmt = atoi(nInputString);
            break;
        }
        else {printf("\nThat was not a valid input. let's try this again...");}
    }

    readerCap = new Semaphore("readerCap", nAmt);
    maxReaders = nAmt;
    remainingWriters = wAmt;

    // Create and fork threads
    Thread *t;
    char *name;
    for (int i = 0; i < rAmt; i++) {
        name = new char[30];
        sprintf(name, "%d", i);
        t = new Thread(name);
        t->Fork(RunReader, nAmt);
    }

    for (int i = 0; i < wAmt; i++) {
        name = new char[30];
        sprintf(name, "%d", i);
        t = new Thread(name);
        t->Fork(RunWriter, 0);
    }
}
// End Project 2

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
    if (aFlag == 3) {
        Thread *t4 = new Thread("MakeBusyWaitingPhilosopher");
        t4->Fork(MakePhilosopher, 0);
    }
    if (aFlag == 4) {
        Thread *t5 = new Thread("MakeSemaphorePhilosopher");
        t5->Fork(MakePhilosopher, 1);
    }
    if (aFlag == 5) {
        Thread *t6 = new Thread("MakePostOffice");
        t6->Fork(MakePostOffice, 0);
    }
    if (aFlag == 6) {
        Thread *t7 = new Thread("MakeReaderWriter");
        t7->Fork(MakeReaderWriter, 0);
    }
    // End code changes by Anthony Guarino

 }


#pragma clang diagnostic pop