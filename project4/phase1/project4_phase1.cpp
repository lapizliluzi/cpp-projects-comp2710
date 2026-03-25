/**
 * Michael Cole
 * mzc0216
 * project4_phase1.cpp 
 * 
 * Compile: g++ project4_phase1.cpp -o project4_phase1
 * Run: ./project4_phase1
 * 
 * Claude Ai helped with functions protypes and structure of question types. Chat GPT helped with the display.
 */

 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <cctype>
 #include <iomanip>
 #include <limits> 
 
 using namespace std;
 
 // Question types
 enum QuestionType {
     MCQ,    // Multiple Choice Question
     TF,     // TF
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
     Question* next;            
     
     // Constructor
     Question(QuestionType t) : type(t), options(nullptr), next(nullptr), pointValue(0.0) {}
     
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
 void displayQuestionSession(Question* head);
 void assessmentPhase(Question* head);
 void displaySessionLog(Question* head, bool showCorrectAnswers);
 void displayQuestion(Question* q, int questionNumber);
 bool checkAnswer(Question* q, const string& userAnswer);
 void clearInputBuffer();
 string toLower(string str);
 
 int main() {
     Question* head = nullptr;
     Question* tail = nullptr;
     int questionCount = 0;
     
     displayWelcomeMessage();
     
     bool continueAdding = true;
     while (continueAdding) {
         cout << "=== QUESTION " << questionCount + 1 << " ===" << endl;
         
         Question* newQuestion = createQuestion();
         if (newQuestion != nullptr) {
             questionCount++;
             
             // Add to the linked list
             if (head == nullptr) {
                 head = newQuestion;
                 tail = newQuestion;
             } else {
                 tail->next = newQuestion;
                 tail = newQuestion;
             }
             
             cout << "Question saved. Continue? [y/n]: ";
             string response;
             cin >> response;
             clearInputBuffer();
             
             while (response != "y" && response != "n") {
                 cout << "[Command not recognized, please try again!]" << endl;
                 cout << "Question saved. Continue? [y/n]: ";
                 cin >> response;
                 clearInputBuffer();
             }
             
             continueAdding = (response == "y");
         }
     }
     
     // Display 
     displaySessionLog(head, false);
     
     cout << "/!\\ Begin assessment? [y/n]: ";
     string startAssessment;
     cin >> startAssessment;
     clearInputBuffer();
     
     if (startAssessment == "y") {
         assessmentPhase(head);
     }
     
     Question* current = head;
     while (current != nullptr) {
         Question* temp = current;
         current = current->next;
         delete temp;
     }
     
     cout << "*** Thank you for using the testing service. Goodbye! ***" << endl;
     
     return 0;
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
 // MCQ
 Question* addMultipleChoiceQuestion() {
     Question* newQuestion = new Question(MCQ);
     
     cout << "Enter a question: ";
     getline(cin, newQuestion->questionText);
     cout << "[At any time, type 'quit()' to exit]" << endl;
     
     char optionLabel = 'A';
     Option* lastOption = nullptr;
     
     while (true) {
         string optionText;
         cout << "Enter choice " << optionLabel << ": ";
         getline(cin, optionText);
         
         if (optionText == "quit()") {
             break;
         }
         
         Option* newOption = new Option(optionLabel, optionText);
         
         if (newQuestion->options == nullptr) {
             newQuestion->options = newOption;
         } else {
             lastOption->next = newOption;
         }
         
         lastOption = newOption;
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
 // Display questions
 void displaySessionLog(Question* head, bool showCorrectAnswers) {
     cout << "=== SESSION LOG ===" << endl;
     
     int questionCount = 0;
     double totalPoints = 0;
     
     Question* current = head;
     while (current != nullptr) {
         questionCount++;
         totalPoints += current->pointValue;
         current = current->next;
     }
     
     cout << "Total questions: " << questionCount << endl;
     cout << "Total point values: " << totalPoints << endl;
 }
 
 void assessmentPhase(Question* head) {
     Question* current = head;
     int questionNumber = 1;
     double userScore = 0;
     double totalPoints = 0;
     int correctAnswers = 0;
     int totalQuestions = 0;
     
     while (current != nullptr) {
         totalQuestions++;
         displayQuestion(current, questionNumber);
         
         string userAnswer;
         if (current->type == TF) {
             cout << "Your answer [true/false]: ";
         } else {
             cout << "Your answer: ";
         }
         
         getline(cin, userAnswer);
         
         if (current->type == TF) {
             userAnswer = toLower(userAnswer);
             while (userAnswer != "true" && userAnswer != "false") {
                 cout << "[Answer not recognized, please try again!]" << endl;
                 cout << "Your answer [true/false]: ";
                 getline(cin, userAnswer);
                 userAnswer = toLower(userAnswer);
             }
         }
         
         bool isCorrect = checkAnswer(current, userAnswer);
         
         if (isCorrect) {
             cout << "[Your answer is correct!]" << endl;
             userScore += current->pointValue;
             correctAnswers++;
         } else {
             cout << "[Your answer is incorrect. The correct answer is " << current->correctAnswer << ".]" << endl;
         }
         
         totalPoints += current->pointValue;
         current = current->next;
         questionNumber++;
     }
     
     cout << "/!\\ Assessment Complete." << endl;
     cout << "=== SESSION LOG ===" << endl;
     cout << "Correct answers: " << correctAnswers << "/" << totalQuestions << endl;
     cout << "Final score: " << userScore << "/" << totalPoints << endl;
 }
 
 // single question
 void displayQuestion(Question* q, int questionNumber) {
     cout << "Question " << questionNumber << ": " << q->questionText << endl;
     
     if (q->type == MCQ) {
         Option* current = q->options;
         while (current != nullptr) {
             cout << current->label << ". " << current->text << endl;
             current = current->next;
         }
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