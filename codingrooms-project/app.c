//-----------------------------------------
// Popular Vote Minimizer Application
// Course: CS 211 - Programming Practicuum
// Author: Prof. Scott Reckinger
//-----------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

// printing State data members, all items on one line 
// Ex: Illinois (IL): 20 EVs, 6033744 PVs
void printState(State myState) {
    printf("  %s (%s): %d EVs, %d PVs\n",myState.name,myState.postalCode,myState.electoralVotes,myState.popularVotes);
}

// printing State data members, all items on one line 
//  PVs printed here are minimum number of popular votes  
//  needed to win the state's electoral votes
// Ex: Illinois (IL): 20 EVs, 3016873 PVs
void printStatePVsToWin(State myState) {
    printf("  %s (%s): %d EVs, %d PVs\n",myState.name,myState.postalCode,myState.electoralVotes,myState.popularVotes/2+1);
}

// helpful function for printing the status of a bool input
void printONorOFF(bool flag) {
    if (flag) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}

int main(int argc, char* argv[]) {
    
    bool fastMode = false;
    bool quietMode = false;
    int year = -1;

    // command-line argument settings
    if (!setSettings(argc, argv, &year, &fastMode, &quietMode)) {
        printf("Invalid command-line argument.\n");
        printf("  Valid command-line arguments:\n");
        printf("    [-y year] = sets the election year, where valid [year]\n");
        printf("                values are perfect multiples of 4, between\n"); 
        printf("                1828 and 2024, inclusively, OR 9999 to use\n");
        printf("                a basic 4-state data file for testing; \n");
        printf("                user interactively sets the year if absent.\n");
        printf("    [-q] = quiet mode; default is OFF; if ON, full State\n");
        printf("           list read-in from file is NOT printed AND\n");
        printf("           subset of States needed to win election with\n");
        printf("           the minimum popular vote total is NOT printed.\n");
        printf("    [-f] = fast mode; default is OFF; if ON, the \"fast\"\n");
        printf("           algorithm is used to determine the minimum\n");
        printf("           number of popular votes to win the election.\n");
        printf("  All command-line arguments are optional.\n");
        printf("Terminating program...\n");
        return -1;
    }

    printf("Welcome to the Popular Vote Minimizer Application!\n\n");

    // if year is not set by command-line arguments, 
    //  then set it using interactive user-input;
    //  year must be between 1828 and 2024, inclusive
    //  OR 9999 for debugging/testing purposes
    if (year == -1) {
        printf("Year NOT set by command-line arguments.\nEnter the year: ");
        int res = scanf("%d",&year);
        while (res <= 0 || year < 1828 || (year != 9999 && (year > 2024 || year % 4 != 0))) {
            if ( res <= 0 ) {
                char garbage[100];
                fgets(garbage,100,stdin);
                printf("  Input must be an integer.\n");            
            } else if (year == 1789 || ((year % 4 == 0) && (year >= 1792) && (year <= 1824))) {
                printf("  Insufficient election data for the U.S. Presidential Election held in %d.\n",year);            
            } else {
                printf("  There was no U.S. Presidential Election held in %d.\n",year);
            }
            printf("Enter the year: \n");
            res = scanf("%d",&year);
        }
    }
    
    // generate filepath for the input election data file for reading
    //  format for filename = "data/[year].csv"
    char filename[100] = "";
    inFilename(filename,year); 

    // generate filepath for the output election data file for writing
    //  format for filename = "toWin/[year]_win.csv"
    char filenameW[100] = "";
    outFilename(filenameW,year); 
    
    // display the program settings   
    printf("\n");
    printf("Settings:\n");
    printf("  year = %d\n", year);
    printf("  quiet mode = ");
    printONorOFF(quietMode);
    printf("  fast mode = ");
    printONorOFF(fastMode);
    printf("  input data file = %s\n", filename);
    printf("  output data file = %s\n", filenameW);

    // read election data file to create [allStates] array, 
    //  which is a static array with capacity 51, but the
    //  actual size of the array is [nStates] 
    State allStates[51];
    int nStates;
    if(!readElectionData(filename,allStates,&nStates)) {
        printf("Invalid filename %s. \nNo file found. \nTerminating program...\n",filename);
        return 0;
    } 
        
    // display full list of States (only if not in quiet mode)
    if (!quietMode) {
        printf("\n");
        printf("Election data for all states:\n");
        for (int i = 0; i < nStates; ++i) {
            printState(allStates[i]);
        }
    }

    // solve the "central question" by calculating the minimum
    //  number of popular votes to win the election
    printf("\n");
    printf("Calculating Minimum Popular Votes to Win...\n");
    MinInfo toWin;
    if (fastMode) {
        toWin = minPopVoteToWinFast(allStates, nStates);
    } else {
        toWin = minPopVoteToWin(allStates, nStates);
    }

    // display the list of States in the subset that produces
    //  the minimum total popular votes, but still wins election.
    //  Note: PVs printed here are the required number of popular
    //  votes needed in each state to win its electoral votes.
    if (!quietMode) {
        printf("\n");
        printf("States in the minimum-PV, sufficient-EV subset:\n");
        for (int i = 0; i < toWin.szSomeStates; ++i) {
            printStatePVsToWin(toWin.someStates[toWin.szSomeStates-i-1]);
        }
    }   
    int totEVs = totalEVs(allStates,nStates);
    int wonEVs = totalEVs(toWin.someStates,toWin.szSomeStates);
    int reqEVs = totEVs/2 + 1;
    int totPVs = totalPVs(allStates,nStates);
    
    // display statistics winning the election 
    //  with minimum number of popular votes
    printf("\n");
    printf("Statistical Summary: \n");
    printf("  Total EVs = %d\n",totEVs);
    printf("  Required EVs = %d\n",totEVs/2+1);
    printf("  EVs won = %d\n",wonEVs);
    printf("  Total PVs = %d\n",totPVs);
    printf("  PVs Won = %d\n",toWin.subsetPVs);
    printf("  Minimum Percentage of Popular Vote to Win Election = %0.2lf%%\n\n",100.0*toWin.subsetPVs/totPVs);

    printf("\n");
    printf("Writing the minimum-PV winning subset of States data to %s...\n",filenameW);
    
    // write the minimum-PV winning subset of States file, 
    //  format is one State per line: 
    //      [name],[postalCode],[EVs],[minPVsToWin]
    //  Note that minPVsToWin is the minimum popular votes
    //  needed to win the state's electoral votes
    writeSubsetData(filenameW, totEVs, totPVs, wonEVs, toWin);

    printf("\n");

    return 0;
}
