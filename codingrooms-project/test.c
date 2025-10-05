#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;
    
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    // Test case 1
    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return false;
    }
    // Test case 2
    printf(" Checking totalEVs() for 6 States:\n");
    res = totalEVs(aStates,6);
    if (res != 48) {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n",res);
        return false;
    }

    //--------------------------------------------------------   
    // TODO: Task 1 - perform additional testing to make this 
    //                test case for totalEVs() more robust
    //--------------------------------------------------------   
    // Test case 3
    printf(" Checking totalEVs() for 1 States:\n");
    res = totalEVs(aStates,1);
    if (res != 5) {
        printf("  individual state EVs are 5\n");
        printf("  expected total EVs = 5, actual total EVs = %d\n",res);
        return false;
    }

    // Test case 4
    printf(" Checking totalEVs() for 6 States:\n");
    res = totalEVs(aStates,3);
    if (res != 25) {
        printf("  individual state EVs are 5, 8, 12\n");
        printf("  expected total EVs = 25, actual total EVs = %d\n",res);
        return false;
    }

    return true;
}

bool test_totalPVs() {
    //--------------------------------------------------------   
    // TODO: Task 1 - write your own test case for totalPVs();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //--------------------------------------------------------
    State aStates[10];
    int res;
    
    aStates[0].popularVotes = 100000;
    aStates[1].popularVotes = 200000;
    aStates[2].popularVotes = 300000;
    aStates[3].popularVotes = 150000;
    aStates[4].popularVotes = 250000;
    aStates[5].popularVotes = 350000;

    // Test case 1
    printf(" Checking totalPVs() for 5 States:\n");
    res = totalPVs(aStates,5);
    if (res != 1000000) {
        printf("  individual state PVs are 100000, 200000, 300000, 150000, 250000\n");
        printf("  expected total PVs = 1000000, actual total PVs = %d\n",res);
        return false;
    }

    // Test case 2
    printf(" Checking totalPVs() for 3 States:\n");
    res = totalPVs(aStates,3);
    if (res != 600000) {
        printf("  individual state PVs are 100000, 200000, 300000\n");
        printf("  expected total PVs = 600000, actual total PVs = %d\n",res);
        return false;
    }
    return true; //modify this
}

bool test_setSettings() {
    //-----------------------------------------------------------   
    // TODO: Task 2 - write your own test case for setSettings();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------
    int year;
    bool fastMode, quietMode;

    // Test case 1: No arguments
      printf(" Checking setSettings() for no arguments:\n");
    char *argv1[] = {"./test.exe"};
    if (!setSettings(1, argv1, &year, &fastMode, &quietMode)) {
        printf("  Test case 1 FAILED: No arguments should return true.\n");
        return false;
    }
    if (year != -1 || fastMode != false || quietMode != false) {
        printf("  Test case 1 FAILED: Default values not set correctly.\n");
        return false;
    }

    // Test case 2: Valid arguments
      printf(" Checking setSettings() for valid arguments:\n");
    char *argv2[] = {"./test.exe", "-y", "2020", "-f", "-q"};
    if (!setSettings(5, argv2, &year, &fastMode, &quietMode)) {
        printf("  Test case 2 FAILED: Valid arguments should return true.\n");
        return false;
    }
    if (year != 2020 || fastMode != true || quietMode != true) {
        printf("  Test case 2 FAILED: Arguments not parsed correctly.\n");
        return false;
    }

    // Test case 3: Invalid year
      printf(" Checking setSettings() for invalid year:\n");
    char *argv3[] = {"./test.exe", "-y", "abc"};
    if (setSettings(3, argv3, &year, &fastMode, &quietMode)) {
        printf("  Test case 3 FAILED: Invalid year should return false.\n");
        return false;
    }

    return true; //modify this
}

bool test_inFilename() {
    //----------------------------------------------------------   
    // TODO: Task 3 - write your own test case for inFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //----------------------------------------------------------
    char filename[100];

    // Test case 1
    printf("Checking inFilename() for 2020:\n");
    inFilename(filename, 2020);
    if (strcmp(filename, "data/2020.csv") != 0)
    {
        printf(" Test case 1 FAILED: inFileName() for 2020 did not produce the expected filename.\n");
        printf(" Expected: toWin/2020_win.csv\n");
        printf(" Actual: %s\n", filename);
        return false;
    }

    // Test case 2
    printf("Checking inFilename() for 2000:\n");
    inFilename(filename, 2000);
    if (strcmp(filename, "data/2000.csv") != 0)
    {
        printf(" Test case 2 FAILED: inFileName() for 2000 did not produce the expected filename.\n");
        printf(" Expected: toWin/2000_win.csv\n");
        printf(" Actual: %s\n", filename);
        return false;
    }
    return true; //modify this
}

bool test_outFilename() {
    //-----------------------------------------------------------   
    // TODO: Task 3 - write your own test case for outFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------
    char filename[100];

    // Test case 1
    printf("Checking outFilename() for 2020:\n");   
    outFilename(filename, 2020);
    if (strcmp(filename, "toWin/2020_win.csv") != 0) 
    {
        printf(" Test case 1 FAILED: inFileName() for 2020 did not produce the expected filename.\n");
        printf(" Expected: toWin/2020_win.csv\n");
        printf(" Actual: %s\n", filename);
        return false;
    }

    // Test case 2
    printf("Checking outFilename() for 2000:\n");
    outFilename(filename, 2000);
    if (strcmp(filename, "toWin/2000_win.csv") != 0)
    {
        printf(" Test case 2 FAILED: inFileName() for 2000 did not produce the expected filename.\n");
        printf(" Expected: toWin/2000_win.csv\n");
        printf(" Actual: %s\n", filename);
        return false;
    }
    return true; //modify this
}

bool test_parseLine() {
    //---------------------------------------------------------   
    // TODO: Task 5 - write your own test case for parseLine();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //---------------------------------------------------------
    State myState;
    char line1[] = "California,CA,55,13202158";
    char line2[] = "Texas,TX,38,7993851";

    // Test case 1
    printf("Checking parseLine() for valid line:\n");
    if (!parseLine(line1, &myState))
    {
        printf(" Test case 1 FAILED: Valid line should return true.\n");
        return false;
    }
    if (strcmp(myState.name, "California") != 0 || strcmp(myState.postalCode, "CA") != 0 ||
    myState.electoralVotes != 55 || myState.popularVotes != 13202158)
    {
        printf(" Test case 1 FAILED: Parsed data incorrect.\n");
        return false;
    }

    // Test case 2
    printf("Checking parseLine() for valid line:\n");
    if (!parseLine(line2, &myState))
    {
        printf(" Test case 1 FAILED: Valid line should return true.\n");
        return false;
    }
    if (strcmp(myState.name, "Texas") != 0 || strcmp(myState.postalCode, "TX") != 0 ||
    myState.electoralVotes != 38 || myState.popularVotes != 7993851)
    {
        printf(" Test case 1 FAILED: Parsed data incorrect.\n");
        return false;
    }
    return true; //modify this
}

bool test_readElectionData() {
    //----------------------------------------------------------------   
    // TODO: Task 6 - write your own test case for readElectionData();
    //                make sure to test all components
    //----------------------------------------------------------------
    State allStates[51];
    int nStates;
    // Test case 1: Valid file
    printf("Checking inFilename() for valid file:\n");
    if (!readElectionData("data/2012.csv", allStates, &nStates)) 
    {
        printf(" Test case 1 FAILED: Valid file should return true.\n");
    }
    if (nStates <= 0)
    {
        printf(" Test case 1 FAILED: No states read from file.\n");
    }
    // Test case 2: Invalid file
    printf("Checking inFilename() for valid file:\n");
    if (readElectionData("data/nonexistent.csv", allStates, &nStates))
    {
        printf(" Test case 2 FAILED: Invalid file should return false.\n");
        return false;
    }
    return true; //modify this
}

bool test_minPVsSlow() {
    //----------------------------------------------------------------- 
    // TODO: Task 7 - write your own test case for minPopVoteAtLeast();
    //                make sure to test all components
    //-----------------------------------------------------------------
    printf(" Checking minPopVoteAtLeast():\n");
    return true; //modify this
}

bool test_minPVsFast() {
    //--------------------------------------------------------------------- 
    // TODO: Task 8 - write your own test case for minPopVoteAtLeastFast();
    //                make sure to test all components
    //---------------------------------------------------------------------
    printf(" Checking minPopVoteAtLeastFast():\n");
    return true; //modify this
}

int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
