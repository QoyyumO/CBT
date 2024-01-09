#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

struct User {
    string username;
    string password;
};

struct Question {
    string question;
    string options[4];
    char correctOption;
    char userAnswer; // Added to store user's choice
};

class AuthenticationSystem {
private:
    vector<User> users;

public:
    AuthenticationSystem() {
        loadUsersFromCSV("users.csv");
    }

    void loadUsersFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, password;
            getline(ss, username, ',');
            getline(ss, password, ',');

            users.push_back({username, password});
        }

        file.close();
    }

    bool authenticateUser(const string& username, const string& password) {
        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                return true; // Authentication successful
            }
        }
        return false; // Authentication failed
    }
};

class QuestionManager {
private:
    vector<Question> questions;
    map<int, char> skippedQuestions; // Map to store skipped questions
    size_t currentQuestionIndex; // Added to keep track of the current question

public:
    
    QuestionManager() : currentQuestionIndex(0) {
        loadQuestionsFromCSV("questions.csv");
    }

    void loadQuestionsFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Question q;
            getline(ss, q.question, ',');
            for (int i = 0; i < 4; ++i) {
                getline(ss, q.options[i], ',');
            }
            ss >> q.correctOption;

            q.userAnswer = ' '; // Initialize user's answer to a space
            questions.push_back(q);
        }

        file.close();
    }
   
    void displayCurrentQuestion() const {
        const Question& currentQuestion = questions[currentQuestionIndex];

        cout << "Question " << (currentQuestionIndex + 1) << ": " << currentQuestion.question << endl;
        for (char option = 'A'; option <= 'D'; ++option) {
            cout << option << ". " << currentQuestion.options[option - 'A'] << endl;
        }
    }

char getUserInput() const {
    char userInput;
    bool validChoice = false;

    do {
        cout << "Enter your answer (A, B, C, D), or 'S' to skip: ";
        cin >> userInput;

        // Convert input to uppercase
        userInput = toupper(userInput);

        // Check if the input is valid
        if ((userInput >= 'A' && userInput <= 'D') || userInput == 'S') {
            validChoice = true;
        } else {
            cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (!validChoice);

    return userInput;
}

    void setUserAnswer(char answer) {
    questions[currentQuestionIndex].userAnswer = answer;
    }

    void handleSkippedQuestions() {
        for (const auto& entry : skippedQuestions) {
            int questionNumber = entry.first;
            char userAnswer = entry.second;

            cout << "Question " << questionNumber << ": " << questions[questionNumber - 1].question << endl;
            cout << "Your Answer: " << userAnswer << endl;
            cout << "------------------------" << endl;
        }
    }

    const vector<Question>& getQuestions() const {
        return questions;
    }

    int calculateScore() const {
        int score = 0;
        for (const auto& question : questions) {
            if (question.userAnswer == question.correctOption) {
                score++;
            }
        }
        return score;
    }

    void moveNext() {
        currentQuestionIndex++;
        skippedQuestions[currentQuestionIndex + 1] = ' '; // Initialize the next question as skipped
    }

    bool isTestComplete() const {
        return currentQuestionIndex == questions.size();
    }

    const map<int, char> getSkippedQuestions() const {
    return skippedQuestions;
}


    void clearSkippedQuestions() {
        skippedQuestions.clear();
    }

    size_t getCurrentQuestionIndex() const {
        return currentQuestionIndex;
    }

    void setCurrentQuestionIndex(size_t index) {
        currentQuestionIndex = index;
    }
};

int main() {
    AuthenticationSystem authSystem;
    QuestionManager questionManager;

    cout << "Welcome to the Quiz Application!\n";

    char startQuizChoice;
    cout << "Do you want to start the quiz? (Y/N): ";
    cin >> startQuizChoice;

    // Convert input to lowercase for easier comparison
    startQuizChoice = tolower(startQuizChoice);

    if (startQuizChoice == 'n') {
        cout << "Quiz aborted. Goodbye!\n";
        return 0;
    } else if (startQuizChoice != 'y') {
        cout << "Invalid choice. Exiting.\n";
        return 1; // Exit with an error code
    }

    bool authenticated = false;
    string username, password;

    // Keep prompting until valid credentials are provided
    while (!authenticated) {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        // Authenticate user
        authenticated = authSystem.authenticateUser(username, password);

        if (!authenticated) {
            cout << "Authentication failed. Invalid username or password.\n";
            cout << "Please re-enter your login details.\n";
        }
    }

    cout << "Authentication successful. Welcome, " << username << "!\n";

    char userChoice;


do {
    // Display the current question
    questionManager.displayCurrentQuestion();

    // Allow users to input answers or skip the question
    char userAnswer = questionManager.getUserInput();

    if (userAnswer == 'S') {
        cout << "Question skipped.\n";
    } else {
        questionManager.setUserAnswer(userAnswer);
    }

    // Provide the option to change the answer multiple times
    do {
        cout << "Do you want to change your answer (C) or proceed to the next question (N)? ";
        cin >> userChoice;

        if (userChoice == 'C' || userChoice == 'c') {
            // Re-display the current question and allow the user to change their answer
            cout << "Current answer: " << questionManager.getQuestions()[questionManager.getCurrentQuestionIndex()].userAnswer << endl;
            cout << "Enter your new answer (A, B, C, D): ";
            cin >> userAnswer;

            // Convert input to uppercase
            userAnswer = toupper(userAnswer);

            // Check if the input is valid
            if (userAnswer >= 'A' && userAnswer <= 'D') {
                // Update the user's answer
                questionManager.setUserAnswer(userAnswer);
            } else {
                cout << "Invalid choice. Please enter a valid option.\n";
            }
        } else if (userChoice != 'N' && userChoice != 'n') {
            cout << "Invalid choice. Please enter either 'C' to change your answer or 'N' to proceed to the next question.\n";
        }
    } while (userChoice != 'N' && userChoice != 'n');
    // Move to the next question if the test is not complete
    if (!questionManager.isTestComplete()) {
        questionManager.moveNext();
    }

    // Check if the test is complete
    if (questionManager.isTestComplete()) {
        cout << "Test completed.\n";
        break;
    }

    cout << "Do you want to continue to the next question? (Y/N): ";
    cin >> userChoice;
} while (userChoice == 'Y' || userChoice == 'y');


// Option to view the test script
cout << "Do you want to view your test script? (Y/N): ";
cin >> userChoice;
if (userChoice == 'Y' || userChoice == 'y') {
    questionManager.handleSkippedQuestions();
}

// Allow users to return to skipped questions
cout << "Do you want to go back to skipped questions? (Y/N): ";
cin >> userChoice;



while (userChoice == 'Y' || userChoice == 'y') {
    // Get the skipped questions
    map<int, char> skippedQuestions = questionManager.getSkippedQuestions();

    // Check if there are any skipped questions
    if (!skippedQuestions.empty()) {
        // Find the first skipped question
        auto it = skippedQuestions.begin();

        // Set the current question index to the first skipped question
        questionManager.setCurrentQuestionIndex(it->first - 1);

        do {
            // Display the skipped question
            questionManager.displayCurrentQuestion();

            // Allow users to input answers or skip the question
            char userAnswer = questionManager.getUserInput();

            if (userAnswer == 'S') {
                cout << "Question skipped.\n";
            } else {
                questionManager.setUserAnswer(userAnswer);
            }

            // Remove the question from the skipped list
            skippedQuestions.erase(it);

            // Find the next skipped question
            it = skippedQuestions.begin();
            if (it == skippedQuestions.end()) {
                cout << "No more skipped questions.\n";
                break;
            }

            // Ask if the user wants to continue to the next skipped question
            cout << "Do you want to go to the next skipped question? (Y/N): ";
            cin >> userChoice;
        } 
    while (userChoice == 'Y' || userChoice == 'y');
    } else {
        cout << "No skipped questions.\n";
        break;
    }

    // Ask if the user wants to go back to skipped questions
    cout << "Do you want to go back to skipped questions? (Y/N): ";
    cin >> userChoice;
}

// Calculate and display the final score
int finalScore = questionManager.calculateScore();
cout << "Your final score: " << finalScore << " out of " << questionManager.getQuestions().size() << endl;



    return 0;
}
