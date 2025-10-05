/*-------------------------------------------
Project 3: Win 80% of Votes but Lose Presidency???
This project aims to minimize the number of popular votes required
to win a U.S presidential election. It involves reading election data,
calculating optimal subsets, and writing the results to a file. The
focus is to utilize recursion and memoization and compare their use in efficiency
Course: CS 211, Spring 2025, UIC
Author: Kalvin Pham
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

int totalEVs(State* states, int szStates) {
    //----------------------------------------------   
    // TODO: Task 1 - write the totalEVs() function;
    //                note test_totalEVs() is 
    //                provided in test.c to test
    //                the functionality of totalEVs();
    //                update makefile to build and run
    //                testing suite as follows:
    //                   -> make build_test
    //                   -> make run_test 
    //----------------------------------------------
    int total = 0;
    for(int i = 0; i < szStates; i++)
    {
       total += states[i].electoralVotes; // Adding each state's electoral votes to the total
    }
    return total; // modify or replace this line
}

int totalPVs(State* states, int szStates) {
    //--------------------------------------------------   
    // TODO: Task 1 - write the totalPVs() function;
    //                then, write your own test function 
    //                test_totalPVs() in test.c;
    //                update makefile to build and run
    //                testing suite as follows:
    //                   -> make build_test
    //                   -> make run_test 
    //--------------------------------------------------
    int total = 0;
    for(int i = 0; i < szStates; i++)
    {
        total += states[i].popularVotes; // Adding each state's popular votes to the total
    }
    return total; // modify or replace this line
}

bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    //------------------------------------------------   
    // TODO: Task 2 - write the setSettings() function
    //------------------------------------------------
    *year = -1; // Default values are false
    *quietMode = false; 
    *fastMode = false; 

    if (argc == 1) // If no arguments are provided
    {
        return true; 
    }

    for (int i = 1; i < argc; i++) // Loop through all of arg count
    {
        if (strstr(argv[i], "-f")) // Checking substring of arg variable element and if it is -f
        {
            if (*fastMode) // If already true, then make it false
            {
                return false;
            }
            *fastMode = true; // Otherwise, make it true, activate fast mode
        }
        else if (strstr(argv[i], "-q")) // Checking substring of arg variable element and if it is -q
        {
             if (*quietMode) // If already true, then make it false
            {
                return false;
            }
            *quietMode = true; // Otherwise, make it true, activate quiet mode
        }
        else if (strstr(argv[i], "-y")) // Checking if the argument is -y (year)
        {
             if (i + 1 < argc) // Checking if there is an element after -y
            {
                *year = atoi(argv[i+1]); // Converting the element after y to an integer
                if (*year == 9999 || *year % 4 == 0 && *year >= 1828 && *year <= 2024) // Checking the bounds
                {
                  *year = atoi(argv[i+1]); // Converting the element after y to an integer
                  i++;  //Increment to the next element
                }
            }
            else return false; // Otherwise no element after y, then exit and y is invalid
        }
        else // Otherwise no y at all, then exit and y is invalid
        {
            return false;
        }
    }
    
    return true; // If everything is valid, return true
}

void inFilename(char* filename, int year) {
    //------------------------------------------------   
    // TODO: Task 3 - write the inFilename() function
    //------------------------------------------------
    sprintf(filename, "data/%d.csv", year); // Sending data/[year].csv into the file name
    return;
}

void outFilename(char* filename, int year) {
    //------------------------------------------------   
    // TODO: Task 3 - write the outFilename() function
    //------------------------------------------------
    sprintf(filename, "toWin/%d_win.csv", year); // Sending toWin/[year]_win.csv into file name
    return;
}

bool parseLine(char* line, State* myState) {
    //------------------------------------------------   
    // TODO: Task 5 - write the parseLine() function
    //------------------------------------------------
    int length = strlen(line); // Getting the length of the input line
    int fieldCount = 0; // Initializing fieldcounter
    char* token; // This is the token for parsing
    char* whole = line; // Copy of the input line


    // Here we will remove newline if it is present in the string
    if (line [length - 1] == '\n') // Check if last character is a newline
    {
        line[length - 1] = '\0'; // If it is a newline then we will replace it with a null terminator and end that line
        length--; // Updating the length to exclude the newline
    }

    // Initializing state struct values
    myState->name[0] = '\0';
    myState->postalCode[0] = '\0';
    myState->electoralVotes = 0;
    myState->popularVotes = 0;

    // Tokenize the line, which is by separating them and categorizing them using the commas as delimiters
    // They are like breakpoints for separation
    while ((token = strtok_r(whole, ",", &whole))) // Extracting each token
    {
        fieldCount++; // Incrementing the field count

        if (fieldCount == 1)
        {
            strncpy(myState->name, token, 50); // Copy the  token to the state name
        }
        else if (fieldCount == 2)
        {
            strncpy(myState->postalCode, token, 3); // Copying the token to the postal code
        }
        else if (fieldCount == 3)
        {
            myState->electoralVotes = atoi(token); // Converting the token to an int and storing in electoralVotes
        }
        else if (fieldCount == 4)
        {
            myState->popularVotes = atoi(token); // Converting the token to an int and storing in popularVotes
        }
        else return false;
    }
    if (fieldCount == 4) return true; // If exactly 4 fields were parsed, then the line is valid

    return false; // Otherwise return false to show line is invalid
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    *nStates = 0; //required initialization prior to incrementing. 
    //-----------------------------------------------------   
    // TODO: Task 6 - write the readElectionData() function
    //-----------------------------------------------------
    FILE* file = fopen(filename, "r"); // Opening the file for reading
    if (!file) return false; // If the file can't be opened, then exit and return false

    char line[500]; // A buffer to store each line of the file

    while (fgets(line, sizeof(line), file)) // Reading the line into the buffer
    {
        if (parseLine(line, &allStates[*nStates])) // Parsing the line into a state struct
        {
            (*nStates)++; // Incrementing the number of states
            if (*nStates >= 51) break; // Stop if max number of states reached
        }
    }

    fclose(file); // Close the file
    return nStates > 0; // Checking if at least one state was checked
}

MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    //----------------------------------------------   
    // TODO: Task 7 - write minPopVoteAtLeast();
    //                a recursive helper function;
    //                returns MinInfo for the subset
    //                of [states] from index [start]
    //                to the end with the minimum
    //                popular vote total that has 
    //                sufficient [EVs] to win
    //---------------------------------------------- 
    if (EVs <= 0) // Base case 1, if no more electoral votes are needed
    {
        MinInfo res = {{0},0, 0, true}; // If the electoral votes are satisfied, return with no more popular votes needed
        return res;
    }

    if (start == szStates) // Base case 2, if no more states are available
    {
        if (EVs <= 0) // If no more electoral votes are needed
        {
            MinInfo res = {{0},0,0,true}; // If the electoral votes are satisfied, return with no more popular votes needed
        }
        MinInfo res = {{0},0, 0, false}; // invalid subset, cannot get more EVs
        return res;
    }

    State currentState = states[start]; // Getting the current state
    
    // Recursively including the current state
    MinInfo includeState = minPopVoteAtLeast(states, szStates, start + 1, EVs - currentState.electoralVotes);
    if (includeState.sufficientEVs) // If including the states gives sufficient EVs
    {
        includeState.subsetPVs += currentState.popularVotes/2 + 1; // Add the minimum required popular votes for the state
        includeState.someStates[includeState.szSomeStates++] = currentState; // Then add the state to the subset
    }

    // Recursively excluding the current state
    MinInfo excludeState = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    // Choose the result with the minimum popular votes
    MinInfo res; // modify or replace this line
    if (includeState.sufficientEVs && excludeState.sufficientEVs) // If both options are valid
    {
        if (includeState.subsetPVs < excludeState.subsetPVs) // Choose the option with fewer popular votes
        {
            res = includeState;
        }
        else
        {
            res = excludeState;
        }
    }
    else if (includeState.sufficientEVs) // If only including the state is valid
    {
        res = includeState;
    }
    else // If only excluding the state is valid
    {
        res = excludeState;
    }

    // res.szSomeStates = 0; // modify or replace this line
    return res; // modify or replace this line
}

MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    //----------------------------------------------   
    // TODO: Task 8 - write minPopVoteAtLeastFast();
    //                start by copying in fully
    //                functioning code from 
    //                minPopVoteAtLeast() and make
    //                additions for memoization
    //---------------------------------------------- 
        if (EVs <= 0) // Base case 1, if no more electoral votes are needed
    {
        MinInfo res = {{0},0, 0, true}; // If the electoral votes are satisfied, return with no more popular votes needed
        return res;
    }

    if (start == szStates) // Base case 2, if no more states are available
    {
        if (EVs <= 0) // If no more electoral votes are needed
        {
            MinInfo res = {{0},0,0,true}; // If the electoral votes are satisfied, return with no more popular votes needed
        }
        MinInfo res = {{0},0, 0, false}; // invalid subset, cannot get more EVs
        return res;
    }

    // If the result is already memoized, return the memoized result
    if (EVs >= 0 && memo[start][EVs].subsetPVs != -1) return memo[start][EVs]; // Using the memo here

    State currentState = states[start]; // Getting the current state

    // Recursively including the current state
    MinInfo includeState = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - currentState.electoralVotes, memo);
    if (includeState.sufficientEVs)
    {
        includeState.subsetPVs += currentState.popularVotes/2 + 1; // Adding the minimum required popular votes for the state
        includeState.someStates[includeState.szSomeStates++] = currentState; // Adding the state to the subset
    }

    // Recursively excluding the current state
    MinInfo excludeState = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    MinInfo res; // modify or replace this line
    if (includeState.sufficientEVs && excludeState.sufficientEVs) // If both are valid
    {
        if (includeState.subsetPVs < excludeState.subsetPVs) // Choosing the one with fewer popular votes
        {
            res = includeState;
        }
        else
        {
            res = excludeState;
        }
    }
    else if (includeState.sufficientEVs) // If only including the state is valid
    {
        res = includeState;
    }
    else // If only excluding the state is valid
    {
        res = excludeState;
    }
    memo[start][EVs] = res; // Updating the memo, memoizing the result
    // res.szSomeStates = 0; // modify or replace this line
    return res; // modify or replace this line
}

MinInfo minPopVoteToWinFast(State* states, int szStates) { // Middleman
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo); // Helper recursion function
    
    //----------------------------------------------   
    // TODO: Task 8 - [memo] will go out of scope 
    //                upon return, so free all
    //                heap-allocated memory for 
    //                [memo] before return 
    //---------------------------------------------- 
    for (int i = 0; i < szStates+1; i++) // Free the elements of memo first
    {
        free(memo[i]); // Free the elements which are arrays in the array memo
    }
    free(memo); // Then free the memo itself which is an array

    return res;
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    //-----------------------------------------------------   
    // TODO: Task 9 - write the writeSubsetData() function
    //-----------------------------------------------------
    FILE* file = fopen(filenameW, "w"); // Opening the file for writing
    if (file == NULL) // Checking if the file can't be opened, then exit and return false
    {
        return false;
    }

    // Writing the header line: totalEVs, totalPVs, wonEVs, and the minimumPVs
    fprintf(file, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs); //toWin.subsetPVs is the minPVs for all won states
    for(int i = toWin.szSomeStates-1; i >= 0; i--) // Reverse looping
    {
        fprintf(file, "%s,%s,%d,%d\n",
            toWin.someStates[i].name, // State name
            toWin.someStates[i].postalCode, // Postal code
            toWin.someStates[i].electoralVotes, // Electoral Votes
            toWin.someStates[i].popularVotes/2 + 1 // Minimum number of popular votes for a single state  
        );
    }

    fclose(file); // Closing the file
    return true; // Return true for validity
}
