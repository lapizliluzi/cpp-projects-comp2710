//Michael Cole
//mzc0216
//project1_Cole_mzc0216.cpp
//This program will tell you how many months it will take you to pay off aspecific loan. Enter your the inputs (loan Amount, Annual interet rate, and monthly payments) and it will tell you how many months it will take to pay off your loan.
//I used google and chatgpt to help learn the basic c++ functions, like cout and cin.
//I also used ChatGPT to help me how to make print functions in c++.
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    //header
    cout << "**************************************************" << endl;
    cout << "*               Amortization Program             *" << endl;
    cout << "**************************************************" << endl;

    // inputs
    double loanAmount, annualInterestRate, monthlyPayment;
    
    cout << "Loan Amount: ";
    cin >> loanAmount;

    cout << "Interest Rate (% per year): ";
    cin >> annualInterestRate;

    cout << "Monthly Payments: ";
    cin >> monthlyPayment;

    // Convert annual interest rate to monthly interest rate 
    double monthlyInterestRate = (annualInterestRate / 100) / 12;

    // validation
    if (loanAmount <= 0 || annualInterestRate < 0 || monthlyPayment <= 0) {
        cout << "Error: Loan amount and monthly payment must be positive, and interest rate must be non-negative." << endl;
        return 1;
    }

    if (monthlyPayment <= loanAmount * monthlyInterestRate) {
        cout << "Error: Monthly payment must be greater than the first month's interest." << endl;
        return 1;
    }

    // top of the table portion
    cout << "**********************************************************" << endl;
    cout << " Amortization Table " << endl;
    cout << "**********************************************************" << endl;
    cout << setw(6) << "Month" 
         << setw(12) << "Balance" 
         << setw(10) << "Payment" 
         << setw(6) << "Rate" 
         << setw(10) << "Interest" 
         << setw(12) << "Principal" << endl;
    
    // Print the first balance
    cout << setw(6) << "0" 
         << setw(12) << fixed << setprecision(2) << loanAmount 
         << setw(10) << "N/A" 
         << setw(6) << "N/A" 
         << setw(10) << "N/A" 
         << setw(12) << "N/A" << endl;

    int month = 0;
    double totalInterestPaid = 0.0;
    //while
    while (loanAmount > 0) {
        month++;
        double interest = loanAmount * monthlyInterestRate;
        double principal = monthlyPayment - interest;

        if (loanAmount - principal < 0) {
            principal = loanAmount;
            monthlyPayment = interest + principal;  
        }
        
        loanAmount -= principal;
        totalInterestPaid += interest;

        // Print the monthly details
        cout << setw(6) << month 
             << setw(12) << fixed << setprecision(2) << loanAmount 
             << setw(10) << monthlyPayment 
             << setw(6) << (monthlyInterestRate * 100) 
             << setw(10) << interest 
             << setw(12) << principal << endl;
    }
        // print line
    cout << "**********************************************************" << endl;
    cout << "It takes " << month << " months to pay off the loan." << endl;
    cout << "Total interest paid is: $" << fixed << setprecision(2) << totalInterestPaid << endl;

    return 0;
}
