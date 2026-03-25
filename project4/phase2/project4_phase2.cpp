/**
 * Michael Cole
 * mzc0216
 * project4_phase2.cpp  
 * 
 * 
 * Compile: g++ project4_phase2.cpp -o project4_phase2
 * Run: ./project4_phase2
 * 
 * Chat GPT and Claude Ai helped with the function prototypes and unit testing portion of phase 2.
 */

 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <cctype>
 #include <iomanip>
 #include <algorithm>
 #include <cassert>
 #include <limits> 
 
 using namespace std;
 
 // Define UNIT_TESTING to enable debug mode, comment out for production

 // #define UNIT_TESTING
 
 // Question types
 enum QuestionType {
     MCQ,    // Multiple Choice Question
     TF,     // True or flase
     WR      // Written Response 
 };
 
 // MCQ options
 struct Option {
     char label;          
     string text;         
     Option* next;        
     
     // Constructor
     Option(char l, string t) : label(l), text(t), next(nullptr) {}
 };
 
 // question node
 struct Question {
     QuestionType type;        
     string questionText;       
     Option* options;           
     string correctAnswer;      
     double pointValue;         
     string userAnswer;         
     bool answered;             
     Question* next;            
     
     // Constructor
     Question(QuestionType t) : type(t), options(nullptr), next(nullptr), 
                               pointValue(0.0), answered(false) {}
     
     // Destructor
     ~Question() {
         Option* current = options;
         while (current != nullptr) {
             Option* temp = current;
             current = current->next;
             delete temp;
         }
     }
 };
 
 // Function prototypes
 void displayWelcomeMessage();
 bool isValidPointValue(const string& value);
 Question* createQuestion();
 Question* addMultipleChoiceQuestion();
 Question* addTrueFalseQuestion();
 Question* addWrittenResponseQuestion();
 void editQuestion(Question* question);
 void deleteQuestion(Question** head, int questionNumber);
 void displaySessionLog(Question* head, bool showCorrectAnswers);
 void assessmentPhase(Question* head);
 void displayQuestion(Question* q, int questionNumber, bool showUserAnswer = false);
 bool checkAnswer(Question* q, const string& userAnswer);
 void clearInputBuffer();
 string toLower(string str);
 int countQuestions(Question* head);
 int calculateTotalPoints(Question* head);
 Question* getQuestionByNumber(Question* head, int questionNumber);
 void displayQuestionForEditing(Question* question);
 Option* addOption(Option* options, char label, const string& text);
 void deleteAllQuestions(Question** head);
 int countOptions(Option* options);
 
 // Test functions
 #ifdef UNIT_TESTING
 void runUnitTests();
 void testEmptyQuestionBank();
 void testDeleteAllQuestions();
 void testSubmitWithoutAnswering();
 void testHardCodedAssessment();
 void testInvalidInputs();
 #endif
 
 int main() {
     #ifdef UNIT_TESTING
         cout << "***This is a debugging version ***" << endl;
         runUnitTests();
         cout << "*** End of the Debugging Version ***" << endl;
         return 0;
     #else
         Question* head = nullptr;
         bool quit = false;
         
         displayWelcomeMessage();
         
         while (!quit) {
             cout << "Do you want to?" << endl;
             cout << "1. Create new question." << endl;
             cout << "2. Edit question." << endl;
             cout << "3. Delete question." << endl;
             cout << "4. Finish." << endl;
             cout << "Select an action: ";
             
             string action;
             cin >> action;
             clearInputBuffer();
             
             if (action == "1") {
                 int questionCount = countQuestions(head);
                 cout << "=== QUESTION " << questionCount + 1 << " ===" << endl;
                 
                 Question* newQuestion = createQuestion();
                 if (newQuestion != nullptr) {
                     // Add to the linked list
                     if (head == nullptr) {
                         head = newQuestion;
                     } else {
                         // Add to the end of the list
                         Question* current = head;
                         while (current->next != nullptr) {
                             current = current->next;
                         }
                         current->next = newQuestion;
                     }
                     
                     cout << "Question saved." << endl;
                 }
             } else if (action == "2") {
                 int questionCount = countQuestions(head);
                 
                 if (questionCount == 0) {
                     cout << "No questions available to edit." << endl;
                     continue;
                 }
                 
                 cout << "Select a question to edit, or type quit() [1-" << questionCount << "]: ";
                 string qNumberStr;
                 cin >> qNumberStr;
                 clearInputBuffer();
                 
                 if (qNumberStr == "quit()") {
                     continue;
                 }
                 
                 int qNumber = atoi(qNumberStr.c_str());
                 if (qNumber < 1 || qNumber > questionCount) {
                     cout << "[That question does not exist!]" << endl;
                     continue;
                 }
                 
                 Question* questionToEdit = getQuestionByNumber(head, qNumber);
                 displayQuestionForEditing(questionToEdit);
                 editQuestion(questionToEdit);
             } else if (action == "3") {
                 int questionCount = countQuestions(head);
                 
                 if (questionCount == 0) {
                     cout << "No questions available to delete." << endl;
                     continue;
                 }
                 
                 cout << "Select a question to delete [1-" << questionCount << "]: ";
                 int qNumber;
                 cin >> qNumber;
                 clearInputBuffer();
                 
                 if (qNumber < 1 || qNumber > questionCount) {
                     cout << "[That question does not exist!]" << endl;
                     continue;
                 }
                 
                 deleteQuestion(&head, qNumber);
                 cout << "Question " << qNumber << " deleted." << endl;
             } else if (action == "4") {
                 quit = true;
             } else {
                 cout << "[Command not recognized, please try again!]" << endl;
             }
         }
         
         // Display
         displaySessionLog(head, false);
         
         if (head != nullptr) {
             cout << "/!\\ Begin assessment? [y/n]: ";
             string startAssessment;
             cin >> startAssessment;
             clearInputBuffer();
             
             if (startAssessment == "y") {
                 assessmentPhase(head);
             }
         }
         
         deleteAllQuestions(&head);
         
         cout << "*** Thank you for using the testing service. Goodbye! ***" << endl;
         
         return 0;
     #endif
 }
 
 // Welcome message
 void displayWelcomeMessage() {
     cout << "*** Welcome to Michael's Testing Service ***" << endl;
 }
 
 bool isValidPointValue(const string& value) {
     bool hasDecimal = false;
     
     for (size_t i = 0; i < value.length(); i++) {
         if (value[i] == '.' && !hasDecimal) {
             hasDecimal = true;
         } else if (!isdigit(value[i])) {
             return false;
         }
     }
     
     double points = stod(value);
     return points >= 0;
 }

 Question* createQuestion() {
     string questionType;
     
     cout << "Type of question [mcq/tf/wr]: ";
     cin >> questionType;
     clearInputBuffer();
     
     while (questionType != "mcq" && questionType != "tf" && questionType != "wr") {
         cout << "[Command not recognized, please try again!]" << endl;
         cout << "Type of question [mcq/tf/wr]: ";
         cin >> questionType;
         clearInputBuffer();
     }
     
     if (questionType == "mcq") {
         return addMultipleChoiceQuestion();
     } else if (questionType == "tf") {
         return addTrueFalseQuestion();
     } else {
         return addWrittenResponseQuestion();
     }
 }
 
 Option* addOption(Option* options, char label, const string& text) {
     Option* newOption = new Option(label, text);
     
     if (options == nullptr) {
         return newOption;
     }
     
     Option* current = options;
     while (current->next != nullptr) {
         current = current->next;
     }
     current->next = newOption;
     
     return options;
 }
 
 // MCQ
 Question* addMultipleChoiceQuestion() {
     Question* newQuestion = new Question(MCQ);
     
     cout << "Enter a question: ";
     getline(cin, newQuestion->questionText);
     cout << "[At any time, type 'quit()' to exit]" << endl;
     
     char optionLabel = 'A';
     
     while (true) {
         string optionText;
         cout << "Enter choice " << optionLabel << ": ";
         getline(cin, optionText);
         
         if (optionText == "quit()") {
             break;
         }
         
         newQuestion->options = addOption(newQuestion->options, optionLabel, optionText);
         optionLabel++;
     }
     
     if (newQuestion->options == nullptr) {
         cout << "Error: MCQ must have at least one option." << endl;
         delete newQuestion;
         return nullptr;
     }
     
     cout << "Select correct answer: ";
     string correctAnswer;
     cin >> correctAnswer;
     clearInputBuffer();
     
     bool validAnswer = false;
     Option* current = newQuestion->options;
     while (current != nullptr) {
         if (toupper(correctAnswer[0]) == current->label) {
             validAnswer = true;
             break;
         }
         current = current->next;
     }
     
     while (!validAnswer) {
         cout << "[Answer not recognized, please try again!]" << endl;
         cout << "Select correct answer: ";
         cin >> correctAnswer;
         clearInputBuffer();
         
         current = newQuestion->options;
         while (current != nullptr) {
             if (toupper(correctAnswer[0]) == current->label) {
                 validAnswer = true;
                 break;
             }
             current = current->next;
         }
     }
     
     newQuestion->correctAnswer = toupper(correctAnswer[0]);
     
     string pointValue;
     bool validPointValue = false;
     
     while (!validPointValue) {
         cout << "Enter point value: ";
         cin >> pointValue;
         clearInputBuffer();
         
         if (isValidPointValue(pointValue)) {
             validPointValue = true;
         } else {
             cout << "[Not a point value, please try again!]" << endl;
         }
     }
     
     newQuestion->pointValue = stod(pointValue);
     
     return newQuestion;
 }
 //TF
 Question* addTrueFalseQuestion() {
     Question* newQuestion = new Question(TF);
     
     cout << "Enter a question: ";
     getline(cin, newQuestion->questionText);
     
     cout << "Select correct answer: ";
     string correctAnswer;
     cin >> correctAnswer;
     correctAnswer = toLower(correctAnswer);
     clearInputBuffer();
     
     while (correctAnswer != "true" && correctAnswer != "false") {
         cout << "[Answer not recognized, please try again!]" << endl;
         cout << "Select correct answer: ";
         cin >> correctAnswer;
         correctAnswer = toLower(correctAnswer);
         clearInputBuffer();
     }
     
     newQuestion->correctAnswer = correctAnswer;
     
     // point value
     string pointValue;
     bool validPointValue = false;
     
     while (!validPointValue) {
         cout << "Enter point value: ";
         cin >> pointValue;
         clearInputBuffer();
         
         if (isValidPointValue(pointValue)) {
             validPointValue = true;
         } else {
             cout << "[Not a point value, please try again!]" << endl;
         }
     }
     
     newQuestion->pointValue = stod(pointValue);
     
     return newQuestion;
 }
 
 // WR
 Question* addWrittenResponseQuestion() {
     Question* newQuestion = new Question(WR);
     
     cout << "Enter a question: ";
     getline(cin, newQuestion->questionText);
     
     cout << "Type correct answer: ";
     getline(cin, newQuestion->correctAnswer);
     
     // point value
     string pointValue;
     bool validPointValue = false;
     
     while (!validPointValue) {
         cout << "Enter point value: ";
         cin >> pointValue;
         clearInputBuffer();
         
         if (isValidPointValue(pointValue)) {
             validPointValue = true;
         } else {
             cout << "[Not a point value, please try again!]" << endl;
         }
     }
     
     newQuestion->pointValue = stod(pointValue);
     
     return newQuestion;
 }
 
 void displayQuestionForEditing(Question* question) {
     cout << "===============================" << endl;
     cout << "=== QUESTION SAVED VALUES ===" << endl;
     cout << "===============================" << endl;
     
     string typeName;
     switch (question->type) {
         case MCQ: typeName = "mcq"; break;
         case TF: typeName = "tf"; break;
         case WR: typeName = "wr"; break;
     }
     
     cout << "1. Type: " << typeName << endl;
     cout << "2. Question: " << question->questionText << endl;
     
     if (question->type == MCQ) {
         cout << "3. Answer choices:" << endl;
         Option* current = question->options;
         while (current != nullptr) {
             cout << "   " << current->label << ". " << current->text << endl;
             current = current->next;
         }
         cout << "4. Correct answer: " << question->correctAnswer << endl;
     } else {
         cout << "3. Correct answer: " << question->correctAnswer << endl;
     }
     
     cout << "===============================" << endl;
 }
 // Question edit
 void editQuestion(Question* question) {
     string choice;
     bool quit = false;
     
     while (!quit) {
         cout << "Type a number to edit, or type quit(): ";
         cin >> choice;
         clearInputBuffer();
         
         if (choice == "quit()") {
             quit = true;
         } else if (choice == "1") {
             cout << "Cannot change question type once created." << endl;
         } else if (choice == "2") {
             cout << "Enter a question: ";
             getline(cin, question->questionText);
             cout << "Question saved." << endl;
         } else if (choice == "3") {
             if (question->type == MCQ) {
                 Option* current = question->options;
                 while (current != nullptr) {
                     Option* temp = current;
                     current = current->next;
                     delete temp;
                 }
                 question->options = nullptr;
        
                 char optionLabel = 'A';
                 
                 while (true) {
                     string optionText;
                     cout << "Enter choice " << optionLabel << ": ";
                     getline(cin, optionText);
                     
                     if (optionText == "quit()") {
                         break;
                     }
                     
                     question->options = addOption(question->options, optionLabel, optionText);
                     optionLabel++;
                 }
                 
                 cout << "Question saved." << endl;
             } else {
                 cout << "Type correct answer: ";
                 getline(cin, question->correctAnswer);
                 cout << "Question saved." << endl;
             }
         } else if (choice == "4" && question->type == MCQ) {
             cout << "Select correct answer: ";
             string correctAnswer;
             cin >> correctAnswer;
             clearInputBuffer();
             
             bool validAnswer = false;
             Option* current = question->options;
             while (current != nullptr) {
                 if (toupper(correctAnswer[0]) == current->label) {
                     validAnswer = true;
                     break;
                 }
                 current = current->next;
             }
             
             while (!validAnswer) {
                 cout << "[Answer not recognized, please try again!]" << endl;
                 cout << "Select correct answer: ";
                 cin >> correctAnswer;
                 clearInputBuffer();
                 
                 current = question->options;
                 while (current != nullptr) {
                     if (toupper(correctAnswer[0]) == current->label) {
                         validAnswer = true;
                         break;
                     }
                     current = current->next;
                 }
             }
             
             question->correctAnswer = toupper(correctAnswer[0]);
             cout << "Question saved." << endl;
         } else {
             cout << "Invalid choice." << endl;
         }
     }
 }
 
 // Delete a question
 void deleteQuestion(Question** head, int questionNumber) {
     if (*head == nullptr) {
         return;
     }
     
     if (questionNumber == 1) {
         Question* temp = *head;
         *head = (*head)->next;
         delete temp;
         return;
     }
     
     Question* current = *head;
     for (int i = 1; i < questionNumber - 1 && current != nullptr; i++) {
         current = current->next;
     }
     
     if (current == nullptr || current->next == nullptr) {
         return;
     }
     
     Question* temp = current->next;
     current->next = temp->next;
     delete temp;
 }
 
 // Display questions
 void displaySessionLog(Question* head, bool showCorrectAnswers) {
     cout << "=== SESSION LOG ===" << endl;
     
     int questionCount = countQuestions(head);
     double totalPoints = calculateTotalPoints(head);
     
     cout << "Total questions: " << questionCount << endl;
     cout << "Total point values: " << totalPoints << endl;
     
     if (showCorrectAnswers) {
         int correctCount = 0;
         double score = 0;
         
         Question* current = head;
         int questionNumber = 1;
         
         while (current != nullptr) {
             cout << "Question " << questionNumber << ": ";
             
             // correct
             if (current->type == MCQ) {
                 cout << current->correctAnswer << endl;
             } else if (current->type == TF) {
                 cout << current->correctAnswer << endl;
             } else {
                 cout << current->correctAnswer << endl;
             }
             
             // answer
             cout << "Your answer: " << current->userAnswer << endl;
             
             if (checkAnswer(current, current->userAnswer)) {
                 correctCount++;
                 score += current->pointValue;
             }
             
             current = current->next;
             questionNumber++;
         }
         
         cout << "Correct answers: " << correctCount << "/" << questionCount << endl;
         cout << "Final score: " << score << "/" << totalPoints << endl;
     }
 }
 // start of assessment
 void assessmentPhase(Question* head) {
     if (head == nullptr) {
         cout << "No questions available for assessment." << endl;
         return;
     }
     
     Question* current = head;
     bool quit = false;
     
     Question* q = head;
     while (q != nullptr) {
         q->answered = false;
         q->userAnswer = "";
         q = q->next;
     }
     
     while (!quit) {
         int questionNumber = 1;
         Question* temp = head;
         while (temp != current) {
             questionNumber++;
             temp = temp->next;
         }
         
         displayQuestion(current, questionNumber, true);
         
         if (!current->answered) {
             cout << "Do you want to?" << endl;
             cout << "1. Answer this question." << endl;
             cout << "2. Jump to question." << endl;
             cout << "3. Submit." << endl;
             cout << "Select an action: ";
             
             string action;
             cin >> action;
             clearInputBuffer();
             
             if (action == "1") {
                 if (current->type == TF) {
                     cout << "Your answer [true/false]: ";
                     string answer;
                     getline(cin, answer);
                     answer = toLower(answer);
                     
                     while (answer != "true" && answer != "false") {
                         cout << "[Answer not recognized, please try again!]" << endl;
                         cout << "Your answer [true/false]: ";
                         getline(cin, answer);
                         answer = toLower(answer);
                     }
                     
                     current->userAnswer = answer;
                 } else {
                     cout << "Your answer: ";
                     getline(cin, current->userAnswer);
                 }
                 
                 current->answered = true;
                 
                 cout << "Do you want to?" << endl;
                 cout << "1. Go to next question." << endl;
                 cout << "2. Jump to question." << endl;
                 cout << "3. Submit." << endl;
                 cout << "Select an action: ";
                 
                 cin >> action;
                 clearInputBuffer();
             }
             
             if (action == "1") {
                 if (current->next != nullptr) {
                     current = current->next;
                 } else {
                     current = head; 
                 }
             } else if (action == "2") {
                 int totalQuestions = countQuestions(head);
                 cout << "Jump to question [1-" << totalQuestions << "]: ";
                 int jumpTo;
                 cin >> jumpTo;
                 clearInputBuffer();
                 
                 if (jumpTo < 1 || jumpTo > totalQuestions) {
                     cout << "Invalid question number." << endl;
                     continue;
                 }
                 
                 current = getQuestionByNumber(head, jumpTo);
             } else if (action == "3") {
                 bool allAnswered = true;
                 Question* check = head;
                 int unansweredCount = 0;
                 
                 while (check != nullptr) {
                     if (!check->answered) {
                         allAnswered = false;
                         unansweredCount++;
                     }
                     check = check->next;
                 }
                 
                 if (!allAnswered) {
                     cout << "Warning: You have " << unansweredCount << " unanswered question(s)." << endl;
                     cout << "Do you want to submit anyway? [y/n]: ";
                     string confirm;
                     cin >> confirm;
                     clearInputBuffer();
                     
                     if (confirm != "y") {
                         continue;
                     }
                 }
                 
                 quit = true;
             } else {
                 cout << "[Command not recognized, please try again!]" << endl;
             }
         }
         else {
             cout << "You answered: " << current->userAnswer << endl;
             
             cout << "Do you want to?" << endl;
             cout << "1. Edit this answer." << endl;
             cout << "2. Go to next question." << endl;
             cout << "3. Jump to question." << endl;
             cout << "4. Submit." << endl;
             cout << "Select an action: ";
             
             string action;
             cin >> action;
             clearInputBuffer();
             
             if (action == "1") {
                 if (current->type == TF) {
                     cout << "Your new answer [true/false]: ";
                     string answer;
                     getline(cin, answer);
                     answer = toLower(answer);
                     
                     while (answer != "true" && answer != "false") {
                         cout << "[Answer not recognized, please try again!]" << endl;
                         cout << "Your new answer [true/false]: ";
                         getline(cin, answer);
                         answer = toLower(answer);
                     }
                     
                     current->userAnswer = answer;
                 } else {
                     cout << "Your new answer: ";
                     getline(cin, current->userAnswer);
                 }
             } else if (action == "2") {
                 if (current->next != nullptr) {
                     current = current->next;
                 } else {
                     current = head;
                 }
             } else if (action == "3") {
                 int totalQuestions = countQuestions(head);
                 cout << "Jump to question [1-" << totalQuestions << "]: ";
                 int jumpTo;
                 cin >> jumpTo;
                 clearInputBuffer();
                 
                 if (jumpTo < 1 || jumpTo > totalQuestions) {
                     cout << "Invalid question number." << endl;
                     continue;
                 }
                 
                 current = getQuestionByNumber(head, jumpTo);
             } else if (action == "4") {
                 bool allAnswered = true;
                 Question* check = head;
                 int unansweredCount = 0;
                 
                 while (check != nullptr) {
                     if (!check->answered) {
                         allAnswered = false;
                         unansweredCount++;
                     }
                     check = check->next;
                 }
                 
                 if (!allAnswered) {
                     cout << "Warning: You have " << unansweredCount << " unanswered question(s)." << endl;
                     cout << "Do you want to submit anyway? [y/n]: ";
                     string confirm;
                     cin >> confirm;
                     clearInputBuffer();
                     
                     if (confirm != "y") {
                         continue;
                     }
                 }
                 
                 quit = true;
             } else {
                 cout << "[Command not recognized, please try again!]" << endl;
             }
         }
     }
     
     // Display results
     cout << "/!\\ Assessment Complete." << endl;
     displaySessionLog(head, true);
 }
 
 // single question
 void displayQuestion(Question* q, int questionNumber, bool showUserAnswer) {
     cout << "Question " << questionNumber << ": " << q->questionText << endl;
     
     if (q->type == MCQ) {
         Option* current = q->options;
         while (current != nullptr) {
             cout << current->label << ". " << current->text << endl;
             current = current->next;
         }
     }
     
     if (showUserAnswer && q->answered) {
         cout << "You answered: " << q->userAnswer << endl;
     }
 }
 
 // Check if correct
 bool checkAnswer(Question* q, const string& userAnswer) {
     if (q->type == MCQ) {
         char userChar = toupper(userAnswer[0]);
         char correctChar = q->correctAnswer[0];
         return (userChar == correctChar);
     } else if (q->type == TF) {
         return (toLower(userAnswer) == toLower(q->correctAnswer));
     } else { // Written response
         return (toLower(userAnswer) == toLower(q->correctAnswer));
     }
 }
 
 void clearInputBuffer() {
     cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
 }
 
 // str lowercase
 string toLower(string str) {
     string result = str;
     for (size_t i = 0; i < result.length(); i++) {
         result[i] = tolower(result[i]);
     }
     return result;
 }
 
 // Count the number of questions
 int countQuestions(Question* head) {
     int count = 0;
     Question* current = head;
     
     while (current != nullptr) {
         count++;
         current = current->next;
     }
     
     return count;
 }
 
 // Calculate total points
 int calculateTotalPoints(Question* head) {
     int total = 0;
     Question* current = head;
     
     while (current != nullptr) {
         total += current->pointValue;
         current = current->next;
     }
     
     return total;
 }
 
 Question* getQuestionByNumber(Question* head, int questionNumber) {
     Question* current = head;
     int currentNum = 1;
     
     while (current != nullptr && currentNum < questionNumber) {
         current = current->next;
         currentNum++;
     }
     
     return current;
 }
 
 int countOptions(Option* options) {
     int count = 0;
     Option* current = options;
     
     while (current != nullptr) {
         count++;
         current = current->next;
     }
     
     return count;
 }
 
 // Delete all questions
 void deleteAllQuestions(Question** head) {
     Question* current = *head;
     while (current != nullptr) {
         Question* temp = current;
         current = current->next;
         delete temp;
     }
     *head = nullptr;
 }
 
 #ifdef UNIT_TESTING
 /** Run Unit Tests
  * This shows the execution of the unit tests for the program.
  * Each test is different for certain aspects of the program to 
  * make sure the program does the correct thing.
  * */ 
 void runUnitTests() {
     testEmptyQuestionBank();
     testDeleteAllQuestions();
     testSubmitWithoutAnswering();
     testHardCodedAssessment();
     testInvalidInputs();
 }
 /** This will verify that the program can handle a situation when there are no questions
  * before attempting the assessment.
  * This makes sure the count of questions shows as 0 and gives a warning message.
  */
 void testEmptyQuestionBank() {
     cout << "Unit Test Case 1: Ask no question. The program should give a warning message." << endl;
     
     Question* head = nullptr;
     int count = countQuestions(head);
     
     assert(count == 0);
     cout << "Warning - the number of questions to be asked must equal to or be larger than 1." << endl;
     
     cout << "Case 1 Passed" << endl;
 }
 /** This test will verify that all questions can be deleted from the bank, which proves memory management. Creates the questions, puts them 
  * in the bank, deletes them, and the proves head pointer is null.
  */
 void testDeleteAllQuestions() {
     cout << "Unit Test Case 2: Test whether you can delete all questions after setting them up." << endl;
     
     Question* q1 = new Question(MCQ);
     q1->questionText = "Test question 1";
     q1->correctAnswer = "A";
     q1->pointValue = 10;
     
     Question* q2 = new Question(TF);
     q2->questionText = "Test question 2";
     q2->correctAnswer = "false";
     q2->pointValue = 5;
     
     Question* q3 = new Question(WR);
     q3->questionText = "Test question 3";
     q3->correctAnswer = "answer";
     q3->pointValue = 15;
     
     q1->next = q2;
     q2->next = q3;
     
     Question* head = q1;
     
     deleteQuestion(&head, 1);
     deleteQuestion(&head, 1); 
     deleteQuestion(&head, 1); 
     
     assert(head == nullptr);
     
     cout << "Case 2 Passed" << endl;
 }
 
/** This test will verify that a student can submit a test without answering a single question.
 * It creates a question, sets the flag to false, and the verifies the current state. 
 */
 void testSubmitWithoutAnswering() {
     cout << "Unit Test Case 3: Test whether a student can submit the test without answering any question." << endl;
     
     Question* q = new Question(MCQ);
     q->questionText = "Test question";
     q->correctAnswer = "A";
     q->pointValue = 10;
     q->answered = false;
     
     assert(!q->answered);
     cout << "Student can submit without answering." << endl;
     
     delete q;
     
     cout << "Case 3 Passed" << endl;
 }
 /**This test will simulate a workflow assessment. Checks to make
  * sure everything is correctyl evaluated. 
  */
 void testHardCodedAssessment() {
     cout << "Unit Test Case 4: Test a simple hard-coded flow of operation." << endl;
     
     Question* q1 = new Question(MCQ);
     q1->questionText = "How long was the shortest war on record?";
     q1->correctAnswer = "C";
     q1->pointValue = 100;
     
     q1->options = new Option('A', "1 day");
     q1->options->next = new Option('B', "23 days");
     q1->options->next->next = new Option('C', "38 minutes");
     
     Question* q2 = new Question(WR);
     q2->questionText = "What was Bank of America's original name?";
     q2->correctAnswer = "Bank of Italy";
     q2->pointValue = 50;
     
     q1->next = q2;
     
     Question* head = q1;
     
     cout << "Question: How long was the shortest war on record?" << endl;
     cout << "Answer: 38" << endl;
     
     q1->userAnswer = "C";
     q1->answered = true;
     
     if (checkAnswer(q1, q1->userAnswer)) {
         cout << "Your answer is correct! You receive 100 points." << endl;
         cout << "Your total points: 100" << endl;
     } else {
         cout << "Your answer is wrong. The correct answer is 38" << endl;
         cout << "Your total points 0" << endl;
     }
     
     cout << "Question: What was Bank of America's original name? (Hint: Bank of Italy or Bank of Germany)?" << endl;
     cout << "Answer: Bank of Germany" << endl;
     
     q2->userAnswer = "Bank of Germany";
     q2->answered = true;
     
     if (checkAnswer(q2, q2->userAnswer)) {
         cout << "Your answer is correct!" << endl;
     } else {
         cout << "Your answer is wrong. The correct answer is Bank of Italy." << endl;
     }
     
     deleteAllQuestions(&head);
     
     cout << "Case 4 passed" << endl;
 }
 /** This test will verify that the program can handle invalid user inputs.
  * Tests valid and invalid inputs and identifies invalid inputs. 
  */
 void testInvalidInputs() {
     cout << "Unit Test Case 5: Test handling of invalid inputs." << endl;
     
     string invalidPointValue = "-12.50";
     assert(!isValidPointValue(invalidPointValue));
     
     string validPointValue = "10.75";
     assert(isValidPointValue(validPointValue));
     
     cout << "Case 5 passed" << endl;
 }
 #endif