/*Michael Cole
mzc0216
project2_Cole_mzc0216.cpp
How to compile: g++ project2_Cole_mzc0216 -o ./project2_Cole_mzc0216.exe
Why Strategy 2 is better:
Strategy 2 forces Bob and Charlie (the stronger opponents) to focus on eachother in the first round. This increases Aaron's
chances of victory due to him not being a target against more accurate oppenents.
This file contains the test drivers. 
Sources: ChatGpt, youtube, deepseek for help with Git and for/while loops.*/

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std;

// Required Functions
bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive);
void Aaron_shoots1(bool& B_alive, bool& C_alive);
void Bob_shoots(bool& A_alive, bool& C_alive);
void Charlie_shoots(bool& A_alive, bool& B_alive);
void Aaron_shoots2(bool& B_alive, bool& C_alive, bool& first_shot);

// Test functions
void test_at_least_two_alive();
void test_Aaron_shoots1();
void test_Bob_shoots();
void test_Charlie_shoots();
void test_Aaron_shoots2();

// Constants
const int TOTAL_SIM = 10000;
const int RAND_SEED = 50;
const double PERC_CONST = 100.0 / TOTAL_SIM;

int main() {
    srand(time(0)); // randomizer

    cout << "*** Welcome to Michael's Truel of the Fates Simulator ***" << endl;

    // Unit Tests
    test_at_least_two_alive();
    test_Aaron_shoots1();
    test_Bob_shoots();
    test_Charlie_shoots();
    test_Aaron_shoots2();

    // strat 1 
    cout << "Ready to test strategy 1 (run " << TOTAL_SIM << " times):" << endl;
    cout << "Press any key to continue..." << endl;
    cin.get();

    // wins
    int Aaron_wins_strat1 = 0;
    int Bob_wins_strat1 = 0;
    int Charlie_wins_strat1 = 0;

    int Aaron_wins_strat2 = 0;
    int Bob_wins_strat2 = 0;
    int Charlie_wins_strat2 = 0;

    for (int i = 0; i < TOTAL_SIM; i++) {
        bool Aaron_alive = true;
        bool Bob_alive = true;
        bool Charlie_alive = true;

        // until last alive
        while (at_least_two_alive(Aaron_alive, Bob_alive, Charlie_alive)) {
            if (Aaron_alive) {
                Aaron_shoots1(Bob_alive, Charlie_alive);
            }
            if (Bob_alive) {
                Bob_shoots(Aaron_alive, Charlie_alive);
            }
            if (Charlie_alive) {
                Charlie_shoots(Aaron_alive, Bob_alive);
            }
        }

        // winner
        if (Aaron_alive) {
            ++Aaron_wins_strat1;
        } else if (Bob_alive) {
            ++Bob_wins_strat1;
        } else {
            ++Charlie_wins_strat1;
        }
    }

    // output strat 1
    cout << "Aaron won " << Aaron_wins_strat1 << "/" << TOTAL_SIM << " truels or "
         << (Aaron_wins_strat1 * PERC_CONST) << "%" << endl;
    cout << "Bob won " << Bob_wins_strat1 << "/" << TOTAL_SIM << " truels or "
         << (Bob_wins_strat1 * PERC_CONST) << "%" << endl;
    cout << "Charlie won " << Charlie_wins_strat1 << "/" << TOTAL_SIM << " truels or "
         << (Charlie_wins_strat1 * PERC_CONST) << "%" << endl;

    // strat 2 
    cout << "\nReady to test strategy 2 (run " << TOTAL_SIM << " times):" << endl;
    cout << "Press any key to continue..." << endl;
    cin.get();

    for (int i = 0; i < TOTAL_SIM; ++i) {
        bool Aaron_alive = true;
        bool Bob_alive = true;
        bool Charlie_alive = true;
        bool first_shot = true; 

        // until last alive
        while (at_least_two_alive(Aaron_alive, Bob_alive, Charlie_alive)) {
            if (Aaron_alive) {
                Aaron_shoots2(Bob_alive, Charlie_alive, first_shot);
            }
            if (Bob_alive) {
                Bob_shoots(Aaron_alive, Charlie_alive);
            }
            if (Charlie_alive) {
                Charlie_shoots(Aaron_alive, Bob_alive);
            }
        }

        // winner
        if (Aaron_alive) {
            ++Aaron_wins_strat2;
        } else if (Bob_alive) {
            ++Bob_wins_strat2;
        } else {
            ++Charlie_wins_strat2;
        }
    }

    // output strat 2
    cout << "Aaron won " << Aaron_wins_strat2 << "/" << TOTAL_SIM << " truels or "
         << (Aaron_wins_strat2 * PERC_CONST) << "%" << endl;
    cout << "Bob won " << Bob_wins_strat2 << "/" << TOTAL_SIM << " truels or "
         << (Bob_wins_strat2 * PERC_CONST) << "%" << endl;
    cout << "Charlie won " << Charlie_wins_strat2 << "/" << TOTAL_SIM << " truels or "
         << (Charlie_wins_strat2 * PERC_CONST) << "%" << endl;

    // strats?
    if((Aaron_wins_strat2 * PERC_CONST) > (Aaron_wins_strat1 * PERC_CONST)){
        cout << "\nStrategy 2 is better than strategy 1.";
    } else {
        cout << "\nStrategy 1 is better than strategy 2.";
    }

    return 0;
}

// at least two alive
bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive) {
    return (A_alive + B_alive + C_alive) >= 2;
}

// Aaron strat 1
void Aaron_shoots1(bool& B_alive, bool& C_alive) {
    if (C_alive) {
        int shoot_result = rand() % 3;
        if (shoot_result == 0) {
            C_alive = false;
        }
    } else if (B_alive) {
        int shoot_result = rand() % 3;
        if (shoot_result == 0) {
            B_alive = false;
        }
    }
}

// Bob
void Bob_shoots(bool& A_alive, bool& C_alive) {
    if (C_alive) {
        int shoot_result = rand() % 2;
        if (shoot_result == 0) {
            C_alive = false;
        }
    } else if (A_alive) {
        int shoot_result = rand() % 2;
        if (shoot_result == 0) {
            A_alive = false;
        }
    }
}

// Charlie
void Charlie_shoots(bool& A_alive, bool& B_alive) {
    if (B_alive) {
        B_alive = false;
    } else if (A_alive) {
        A_alive = false;
    }
}

// Aaron strat 2 then strat 1
void Aaron_shoots2(bool& B_alive, bool& C_alive, bool& first_shot) {
    if (first_shot) {
        first_shot = false; 
    } else {
        
        Aaron_shoots1(B_alive, C_alive);
    }
}

// unit tests
void test_at_least_two_alive() {
    cout << "Unit Testing 1: Function - at_least_two_alive()\n";
    
    cout << "Case 1: Aaron alive, Bob alive, Charlie alive\n";
    assert(at_least_two_alive(true, true, true) == true);
    cout << "Case passed ...\n";

    cout << "Case 2: Aaron dead, Bob alive, Charlie alive\n";
    assert(at_least_two_alive(false, true, true) == true);
    cout << "Case passed ...\n";

    cout << "Case 3: Aaron alive, Bob dead, Charlie alive\n";
    assert(at_least_two_alive(true, false, true) == true);
    cout << "Case passed ...\n";

    cout << "Case 4: Aaron alive, Bob alive, Charlie dead\n";
    assert(at_least_two_alive(true, true, false) == true);
    cout << "Case passed ...\n";

    cout << "Case 5: Aaron dead, Bob dead, Charlie alive\n";
    assert(at_least_two_alive(false, false, true) == false);
    cout << "Case passed ...\n";

    cout << "Case 6: Aaron dead, Bob alive, Charlie dead\n";
    assert(at_least_two_alive(false, true, false) == false);
    cout << "Case passed ...\n";

    cout << "Case 7: Aaron alive, Bob dead, Charlie dead\n";
    assert(at_least_two_alive(true, false, false) == false);
    cout << "Case passed ...\n";

    cout << "Case 8: Aaron dead, Bob dead, Charlie dead\n";
    assert(at_least_two_alive(false, false, false) == false);
    cout << "Case passed ...\n";

    cout << "Press any key to continue...\n";
    cin.get();
}

void test_Aaron_shoots1() {
    cout << "Unit Testing 2: Function Aaron_shoots1(Bob_alive, Charlie_alive)\n";

    bool Bob_alive = true, Charlie_alive = true;
    cout << "Case 1: Bob alive, Charlie alive\n";
    Aaron_shoots1(Bob_alive, Charlie_alive);
    cout << "Aaron is shooting at Charlie\n";

    cout << "Case 2: Bob dead, Charlie alive\n";
    Bob_alive = false;
    Aaron_shoots1(Bob_alive, Charlie_alive);
    cout << "Aaron is shooting at Charlie\n";

    cout << "Case 3: Bob alive, Charlie dead\n";
    Bob_alive = true, Charlie_alive = false;
    Aaron_shoots1(Bob_alive, Charlie_alive);
    cout << "Aaron is shooting at Bob\n";

    cout << "Press any key to continue...\n";
    cin.get();
}

void test_Bob_shoots() {
    cout << "Unit Testing 3: Function Bob_shoots(Aaron_alive, Charlie_alive)\n";

    bool Aaron_alive = true, Charlie_alive = true;
    cout << "Case 1: Aaron alive, Charlie alive\n";
    Bob_shoots(Aaron_alive, Charlie_alive);
    cout << "Bob is shooting at Charlie\n";

    cout << "Case 2: Aaron dead, Charlie alive\n";
    Aaron_alive = false;
    Bob_shoots(Aaron_alive, Charlie_alive);
    cout << "Bob is shooting at Charlie\n";

    cout << "Case 3: Aaron alive, Charlie dead\n";
    Aaron_alive = true, Charlie_alive = false;
    Bob_shoots(Aaron_alive, Charlie_alive);
    cout << "Bob is shooting at Aaron\n";

    cout << "Press any key to continue...\n";
    cin.get();
}

void test_Charlie_shoots() {
    cout << "Unit Testing 4: Function Charlie_shoots(Aaron_alive, Bob_alive)\n";

    bool Aaron_alive = true, Bob_alive = true;
    cout << "Case 1: Aaron alive, Bob alive\n";
    Charlie_shoots(Aaron_alive, Bob_alive);
    cout << "Charlie is shooting at Bob\n";

    cout << "Case 2: Aaron dead, Bob alive\n";
    Aaron_alive = false;
    Charlie_shoots(Aaron_alive, Bob_alive);
    cout << "Charlie is shooting at Bob\n";

    cout << "Case 3: Aaron alive, Bob dead\n";
    Aaron_alive = true, Bob_alive = false;
    Charlie_shoots(Aaron_alive, Bob_alive);
    cout << "Charlie is shooting at Aaron\n";

    cout << "Press any key to continue...\n";
    cin.get();
}

void test_Aaron_shoots2() {
    cout << "Unit Testing 5: Function Aaron_shoots2(Bob_alive, Charlie_alive)\n";

    bool Bob_alive = true, Charlie_alive = true, first_shot = true;
    cout << "Case 1: Bob alive, Charlie alive\n";
    Aaron_shoots2(Bob_alive, Charlie_alive, first_shot);
    cout << "Aaron intentionally misses his first shot\n";
    cout << "Both Bob and Charlie are alive.\n";

    cout << "Case 2: Bob dead, Charlie alive\n";
    Bob_alive = false, Charlie_alive = true, first_shot = false;
    Aaron_shoots2(Bob_alive, Charlie_alive, first_shot);
    cout << "Aaron is shooting at Charlie\n";

    cout << "Case 3: Bob alive, Charlie dead\n";
    Bob_alive = true, Charlie_alive = false, first_shot = false;
    Aaron_shoots2(Bob_alive, Charlie_alive, first_shot);
    cout << "Aaron is shooting at Bob\n";

    cout << "Press any key to continue...\n";
    cin.get();
}