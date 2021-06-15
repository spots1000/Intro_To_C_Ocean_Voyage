/*
*   Anthony Dalia
*   COP3223C
*   Section 0003
*   Assignment #7 - Ocean Voyage
*   Nov 16th, 2017
*/

//Included Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Constants for Arrays
#define STRLENGTH 30
#define NUMCREW 5
#define NUMSUPPLIES 4

//Constants for Distances (measured in miles)
#define CANARY 1261
#define GRENADA 3110
#define FINAL 500
#define DISTANCE 4871

//Function Signatures - do not change these
void setup(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int *captaintype, int *funds, int *distanceperday);
int countcrew(int crewstatus[NUMCREW]);
void printstatus(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW]);
void getsupplies(char supplytypes[NUMSUPPLIES][STRLENGTH], int supplies[NUMSUPPLIES], int *funds);
void dailyreport(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int funds, int traveled);
void rest(int supplies[NUMSUPPLIES], char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days);
int fish();
int max(int a, int b);
int min(int a, int b);
void event(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days, int supplies[NUMSUPPLIES]);

//Main function - This is the final version of main.  Any changes you make while
//creating the functions should be removed prior to submission.
int main(void) {
    //crewnames and supplytypes are arrays of strings to store the names of the crew members
    //and the types of supplies that can be purchased and taken on the voyage
    char crewnames[NUMCREW][STRLENGTH];
    char supplytypes[NUMSUPPLIES][STRLENGTH] = {"Food", "Clothes", "Ship Parts", "Shovels"};
    //stop indicates whether or not the user would like to stop at a port
    //crewstatus indicates the status of each crew member, corresponding to the order of names
    //supplies has a total for each type of supply, corresponding to the order of supplies
    char stop;
    int crewstatus[NUMCREW], supplies[NUMSUPPLIES];
    //the distanceperday and funds depends on the captaintype the user selects
    //day is the current day, traveled is the total miles traveled, i is a loop counter
    //and action stores the intended action of the user for the day
    int distanceperday, captaintype, funds, traveled=0, day=1;
    int i, action;

    //seed the pseudorandom number generator
    srand(time(0));

    //initialize each variable with information from the user
    setup(crewnames, crewstatus, supplies, &captaintype, &funds, &distanceperday);

    //begin the game by purchasing initial supplies
    printf("\nBefore leaving Port Marin, you should purchase some supplies.\n");
    getsupplies(supplytypes, supplies, &funds);

    //continue the voyage until the ship reaches the intended destination
    //if all crew members perish, the journey cannot continui
    while (traveled < DISTANCE && countcrew(crewstatus) > 0) {
        printf("\n\n--It is day #%d.--\n", day);

        //check to see if the ship has reached the next port
        if(traveled >= (GRENADA+CANARY) && traveled < (GRENADA + CANARY + distanceperday) ) {
            printf("You have arrived at Grenada, at the edge of the Carribbean Sea.\n");
            printf("Would you like to make port? (Y/N)\n");
            scanf(" %c", &stop);

            if (stop == 'Y' || stop == 'y')
                getsupplies(supplytypes, supplies, &funds);

            traveled = (GRENADA+CANARY) + distanceperday;
        }
        else if (traveled >= CANARY && traveled < (CANARY + distanceperday) ) {
            printf("You have arrived at the Canary Islands.\n");
            printf("Would you like to make port? (Y/N)\n");
            scanf(" %c", &stop);

            if (stop == 'Y' || stop == 'y')
                getsupplies(supplytypes, supplies, &funds);

            traveled = CANARY + distanceperday;
        }
        //if between destinations: print the daily report and process the user's action for the day
        else {
            dailyreport(crewnames, crewstatus, supplies, funds, traveled);

            printf("\nWhat would you like to do?\n");
            printf("1 - Fish\n");
            printf("2 - Rest\n");
            printf("3 - Continue\n");
            scanf("%d", &action);

            if(action == 1) {
                supplies[0] += fish();
            }
            else if(action == 2) {
                day--;
                rest(supplies, crewnames, crewstatus, &day);
            }
            else {
                traveled += distanceperday;
                supplies[0] = max(supplies[0] - countcrew(crewstatus) * 2, 0);
                event(crewnames, crewstatus, &day, supplies);
            }
        }
        day++;
    }

    printf("\n\n");

    //The final printout changes based on which condition broke the while loop
    if (countcrew(crewstatus) == 0) {
        printf("Your crew has perished in the search for treasure. :(\n");

        printstatus(crewnames, crewstatus);
    }
    else {
        printf("Your crew has made it safely to the island.\n");

        printstatus(crewnames, crewstatus);

        if(supplies[3] >= countcrew(crewstatus))
            printf("You have enough shovels to dig up the treasure!\n");
        else
            printf("Unfortuantely, you will not be able to dig up the treasure.\n");
    }

    return 0;
}

//Pre-conditions:  none
//Post-conditions: each input parameter should be assigned an initial value
/* SETUP COMPLETED
    What to do in this function: Provide the starting message for the user and ask how they plan to travel.
    Based on their response initialize captaintype, funds, and distanceperday
    -captaintype 1 should get 1000 funds and 80 distanceperday
    -captaintype 2 should get 900 funds and 90 distanceperday
    -captaintype 3 should get 800 funds and 100 distanceperday

    Ask the user for their name.  Place this in the first row of crewnames, representing the captain.  Treat
    crewnames as a 1-Dimensional array of strings. As an example: printf("%s", crewnames[0]); would print the
    first string or the captains name to the screen.  Then ask for the names of the other 4 crew members in a loop.

    Set the crew status to 2 for each crew member, representing healthy
    Set the initial amount of supplies to be 0 for each supply

*/
void setup(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int *captaintype, int *funds, int *distanceperday)
{

    //Loop indexer
    int i;


    printf("You may now take your ship and crew from Port Marlin, Spain to the hidden island in the Caribbean on the old pirate's map. \n\n");
    printf("How will you travel? \n");
    printf("1. - As a merchant\n");
    printf("2. - As a privateer\n");
    printf("3. - As a pirate\n");

    //Pull the ship type into the var
    scanf("%d", captaintype);

    //Initialize the ship type and run relevant code
    if (*captaintype == 1)
    {

        *funds = 1000;
        *distanceperday = 80;

        printf("As a merchant, you begin your trip with 1000 gold pieces. \nYou will be sailing your Carrack, with an average speed of 80 miles per day.");
    }
    else if (*captaintype == 2)
    {

        *funds = 900;
        *distanceperday = 90;

        printf("As a privateer, you begin your trip with 900 gold pieces. \nYou will be sailing your Galleon, with an average speed of 90 miles per day.");
    }
    else if (*captaintype == 3)
    {

        *funds = 800;
        *distanceperday = 100;

        printf("As a pirate, you begin your trip with 800 gold pieces. \nYou will be sailing your Clipper, with an average speed of 100 miles per day.");
    }

    //Name of the captain
    printf("What is your name, Captain?\n");
    scanf("%s", crewnames[0]);

    //Other crew members
    printf("Who are the other members of your crew?\n");
    for (i = 1; i < NUMCREW; i++)
    {

        printf("Please enter the name of a crew member: ");
        scanf("%s", crewnames[i]);
    }

    //Make all the crew healthy
    for (i = 0; i < NUMCREW; i++)
    {
        crewstatus[i] = 2;
    }

    //Intialise the supplies
    for (i = 0; i < NUMSUPPLIES; i++)
    {
        supplies[i] = 0;
    }
}

//Pre-conditions:  crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//Post-conditions: returns the number of crew members that are alive
/* COUNTCREW COMPLETED

//What to do in this function: Traverse the crew status array and count how many crew members
//  have a status that is not 0.  Return this count.
*/
int countcrew(int crewstatus[NUMCREW]) {

    int i, cnt = 0;

    for (i = 0; i < NUMCREW; i++)
    {

        if (crewstatus[i] != 0)
            cnt++;
    }

    return cnt;
}


//Pre-conditions:  crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//Post-conditions: none

/* PRINTSTATUS COMPLETED
//What to do in this function: print each crew members name and their status.
//  You may use a status array to shorten this process: char status[3][STRLENGTH] = {"Deceased", "Ill", "Healthy"};
*/
void printstatus(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW]) {

    int i;
    char status[3][STRLENGTH] = {"Deceased", "Ill", "Healthy"};

    printf("Crew Status: \n");

    for (i = 0; i < NUMCREW; i++)
    {
        //Print the status of the crew member
        printf("%s  :  %s\n", crewnames[i], status[crewstatus[i]]);
    }
}

//Pre-conditions:  supplytypes in an array of strings and gives the name of each supply type
//                 supplies is an array of integers representing how many of each type the crew has
//                 funds represents how many gold pieces the crew has to spend
//Post-conditions: the user may choose to buy supplies: incrementing values in supplies and
//                 decrementing funds
/* GETSUPPLIES COMPLETED
//What to do in this function: First print out the amount of gold the crew has.
//  Then print each supply type and the amount it costs.  You may use a cost array to shorten
//  this process: int supplycosts[NUMSUPPLIES] = {1, 2, 20, 10};
//  Based on the user's selection, ask the user how many of that supply they would like to buy.
//  Verify that the user has enough gold for their purchase and update the correct index of supplies.
//  Deduct the corresponding amount from the user's funds.
*/
void getsupplies(char supplytypes[NUMSUPPLIES][STRLENGTH], int supplies[NUMSUPPLIES], int *funds) {

    int supplycosts[NUMSUPPLIES] = {1, 2, 20, 10}, i, sel, numItems, itemPrice;

    //print gold
    printf("You have %d gold pieces. \n", *funds);

    //Supply processing code
    printf("Available Supplies: \n");

    for (i = 0; i < NUMSUPPLIES; i++)
    {
        printf("%d.)    %s  - %d gold pieces \n", (i+1), supplytypes[i], supplycosts[i]);
    }

    printf("5.)   Leave Store \n");

    //Input verification
    scanf("%d", &sel);

    //Run the supply buying loop
    while (sel != 5)
    {

        if (sel == 1)
        {
            //Format Text
            printf("Food \n");
            printf("How many pounds of food do you want to buy? \n:");

            //Scan in val
            scanf("%d", &numItems);

            //Set the price
            itemPrice = supplycosts[0];

            //Do some quick math
            if ( (itemPrice * numItems) > *funds )
            {
                printf("Sorry, you cannot afford that much food.");
            }
            else
            {
                supplies[0] += numItems;

                *funds -= (itemPrice * numItems);
            }
        }
        else if (sel == 2)
        {
            //Format Text
            printf("Clothes \n");
            printf("How many sets of clothes do you want to buy? \n:");

            //Scan in val
            scanf("%d", &numItems);

            //Set the price
            itemPrice = supplycosts[1];

            //Do some quick math
            if ( (itemPrice * numItems) > *funds )
            {
                printf("Sorry, you cannot afford that many sets of clothes.");
            }
            else
            {
                supplies[1] += numItems;

                *funds -= (itemPrice * numItems);
            }
        }
        else if (sel == 3)
        {
            //Format Text
            printf("Ship Parts \n");
            printf("How many extra ship parts do you want to buy? \n:");

            //Scan in val
            scanf("%d", &numItems);

            //Set the price
            itemPrice = supplycosts[2];

            //Do some quick math
            if ( (itemPrice * numItems) > *funds )
            {
                printf("Sorry, you cannot afford that many ship parts.");
            }
            else
            {
                supplies[2] += numItems;

                *funds -= (itemPrice * numItems);
            }
        }
        else if (sel == 4)
        {
            //Format Text
            printf("Shovels \n");
            printf("How many shovels do you want to buy? \n:");

            //Scan in val
            scanf("%d", &numItems);

            //Set the price
            itemPrice = supplycosts[3];

            //Do some quick math
            if ( (itemPrice * numItems) > *funds )
            {
                printf("Sorry, you cannot afford that many shovels.");
            }
            else
            {
                supplies[3] += numItems;

                *funds -= (itemPrice * numItems);
            }
        }

        //Replay the asking loop

        //print gold
        printf("You have %d gold pieces. \n", *funds);

        //Supply processing code
        printf("Available Supplies: \n");

        for (i = 0; i < NUMSUPPLIES; i++)
        {
            printf("%d.)    %s  - %d gold pieces \n", (i+1), supplytypes[i], supplycosts[i]);
        }

        printf("5.)   Leave Store \n");

        //Input verification
        scanf("%d", &sel);

    }
}

//Pre-conditions:  crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//                 supplies is an array of integers representing how many of each type the crew has
//                 funds represents how many gold pieces the crew has to spend
//                 traveled represents the total number of miles the ship has traveled from the beginning
//Post-conditions: none
/* DAILYREPORT COMPLETED
//What to do in this function: Print a daily status report by telling the user how many miles have been
//  traveled.  Then, print the status of the crew by calling printstatus.  Print the funds and amount
//  of food that the ship has.  Then, let the user know how far they are from their next destination.
*/
void dailyreport(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int supplies[NUMSUPPLIES], int funds, int traveled) {

    //Distance left
    int distLeft;
    if (traveled < CANARY)
    {
        distLeft = CANARY - traveled;
    }
    else if ( traveled < GRENADA)
    {
        distLeft = GRENADA - traveled;
    }
    else if ( (traveled - (GRENADA + CANARY) ) < FINAL)
    {
        distLeft = FINAL - ( traveled - (GRENADA + CANARY));
    }

    //Print out the daily report as asked
    printf("Daily Report: \n");
    printf("You have traveled %d miles. \n", traveled);
    printstatus(crewnames, crewstatus);
    printf("You have %d gold pieces. \n", funds);
    printf("You have %d pounds of food. \n", supplies[0]);
    printf("You are %d miles from your next destination. \n", distLeft);

}

//Pre-conditions:  a and b are both integers
//Post-conditions: the larger value will be returned
/* MAX COMPLETED */
int max(int a, int b) {

    if (a > b)
    {
        return a;
    }
    else if ( b > a)
    {
        return b;
    }
}

//Pre-conditions:  a and b are both integers
//Post-conditions: the smaller value will be returned
/* MIN COMPLETED */
int min(int a, int b) {

    if (a < b)
    {
        return a;
    }
    else if ( b < a)
    {
        return b;
    }
}

//Pre-conditions:  supplies is an array of integers representing how many of each type the crew has
//                 crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//                 days represents the current day
//Post-conditions: the user will select a number of days to rest for. update days to reflect this number
//                 there is a small chance an ill crew member will recover during rest days

/* REST COMPLETED
//What to do in this function: ask the user how many days they would like to rest for.
//  Updates days to indicate that that many days has now passed.  Deduct 2 pounds of food
//  for each crew member for each day rested.

//  Generate a pseudorandom value that will be either 0 or 1.  Generate a second value
//  between 0 and the number of original crew members.  If the first number is a 1,
//  the crew member in the index of the second number may recover if they are sick.
//  If they are healthy or deceased, nothing happens.  If the first number is a 0,
//  nothing happens.
*/
void rest(int supplies[NUMSUPPLIES], char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days) {

    int daysOfRest, i, recovery, crewRecovered;

    srand(time(NULL));

    printf("Resting: \n");
    printf("How many days would you like to rest for? \n:");

    scanf("%d", &daysOfRest);

    //Update Days accordingly
    *days += daysOfRest;

    //Reduce the amount of food
    supplies[0] -= (2 * (daysOfRest * countcrew(crewstatus)));

    //Generate two rands
    recovery = rand() %2;
    crewRecovered = rand() % NUMCREW;

    //Determine who recovers
    if (recovery == 1)
    {

        if ( crewstatus[crewRecovered] == 1)
        {

            //Recover
            crewstatus[crewRecovered] = 2;
        }
    }
}

//Pre-conditions:  none
//Post-conditions: returns the number of pounds of fish gained by the ship

//What to do in this function: Generate a pseudorandom value between 0 and 3, inclusive.
//  Multiply this number by 50 and tell the user how many fish were caught.  Return
//  this value.
/* FISH COMPLETED */
int fish() {

    int fishCaught;

    srand(time(NULL));

    fishCaught = rand() % 4;

    return (50 * fishCaught);
}

//Pre-conditions:  crew names is an array of strings for the crew members
//                 crewstatus is an array of numerical indicators for the status of each crew member
//                 0 - deceased, 1 - ill, 2 - healthy
//                 days represents the current day
//                 supplies is an array of integers representing how many of each type the crew has
//Post-conditions: the status of a crew member or supplies may be affected by a random event,
//                 some events cause the ship to be delayed a certain number of days

/* EVENT COMPLETED

//What to do in this function: Generate a pseudorandom number between 0 and 9, inclusive.  If the user
//  has run out of food, tell the user they have no food and increase your number by 2.  This number cannot
//  exceed 9.

//  Numbers 0, 1, and 2 do not correspond with any events.
//  Numbers 3 and 4 correspond with "positive events"
//      on a 3 the user will gain between 1 and 4 ship parts determined randomly
//      on a 4 the user will gain between 10 and 60 pounds of food determined randomly
//      both events take 1 day
//  Number 5 means the user will lose between 5 and 55 pounds of food
//      this event takes 1 day
//  Number 6 means the ship is stuck in fog, which takes 1 day
//  Number 7 means the ship is in a storm, which takes 2 days
//  Number 8 means something on the ship has broken
//      check the number of extra ship parts the crew has
//      if they have at least one ship part, they can replace it and continue
//      this takes 1 day
//      if they have no extra parts, they must stop and repair
//      this takes 3 days
//  Number 9 indicates sickness.  Generate a random number between 0 and the original
//      number of crew members.  Check the status of that crew member: if they are healthy,
//      they now fall ill.  If they are already ill, they now perish.  If they are already
//      deceased, nothing happens.

//  For any days spent in this way, deduct 2 pounds of food per crew member per day.
*/

void event(char crewnames[NUMCREW][STRLENGTH], int crewstatus[NUMCREW], int *days, int supplies[NUMSUPPLIES]) {

    int daysPast;

    srand(time(NULL));

    int eventNum = rand() % 10;

    if (eventNum == 3)
    {
        int parts = (rand() % 4) + 1;

        printf("You have gained %d ship parts!\n", parts);
        supplies[2] += parts;
        daysPast ++;
    }
    else if ( eventNum == 4)
    {
        int food = 10 * ((rand() % 6) + 1);

        printf("You have gained %d pounds of food from an attacking pirate ship. \n", food);
        supplies[0] += food;
        daysPast ++;
    }
    else if ( eventNum == 5)
    {
        int food = (11 * ((rand() % 5 ) + 1));

        printf("You have lost %d pounds of food taken by a pirate ship. \n", food);
        supplies[0] -= food;
        daysPast ++;
    }
    else if ( eventNum == 6)
    {

        printf("You are stuck in fog for one day.\n");
        daysPast ++;
    }
    else if ( eventNum == 7)
    {

        printf("The ship is stuck in a storm for two days. \n");
        daysPast ++;
    }
    else if ( eventNum == 8)
    {

        if ( supplies[2] >= 1)
        {

            printf("The ship breaks down but you have the parts needed to repair it. Loose one day.");
            supplies[2] -= 1;
            daysPast ++;
        }
        else
        {
            printf("The ship breaks down but you have no spare parts left to repair it with. Loose 3 days.");
            daysPast += 3;
        }
    }
    else if ( eventNum == 9)
    {

        srand(time(NULL));

        int sickCrew = rand() % NUMCREW;

        if ( crewstatus[sickCrew] == 2)
            crewstatus[sickCrew] = 1;
        else if ( crewstatus[sickCrew] == 1)
            crewstatus[sickCrew] = 0;

    }

    supplies[0] -= (2 * daysPast);

    *days += daysPast;

}
