/*Michael Cole
mzc0216
project2_Cole_mzc0216.cpp
How to compile: g++ project2_Cole_mzc0216 -o ./project2_Cole_mzc0216.exe
Why Strategy 2 is better:
Strategy 2 forces Bob and Charlie (the stronger opponents) to focus on eachother in the first round. This increases Aaron's
chances of victory due to him not being a target against more accurate oppenents.
This file does not contain the test drivers.
Sources: ChatGpt, youtube, deepseek for help with Git and for/while loops.*/

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Required Functions
bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive);
void Aaron_shoots1(bool& B_alive, bool& C_alive);
void Bob_shoots(bool& A_alive, bool& C_alive);
void Charlie_shoots(bool& A_alive, bool& B_alive);
void Aaron_shoots2(bool& B_alive, bool& C_alive, bool& first_shot);

// Constants
const int TOTAL_SIM = 10000;
const int RAND_SEED = 50;
const double PERC_CONST = 100.0 / TOTAL_SIM;

int main() {
    srand(time(0));

    cout << "*** Welcome to Michael's Truel of the Fates Simulator ***\n" << endl;

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