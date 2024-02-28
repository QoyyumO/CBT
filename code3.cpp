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
            cout << "Enter your answer (A, B, C, D), 'P' for previous, 'Q' for next, or 'E' to end: ";
            cin >> userInput;

            // Convert input to uppercase
            userInput = toupper(userInput);

            // Check if the input is valid
            if ((userInput >= 'A' && userInput <= 'D') ||
                (userInput == 'P' && currentQuestionIndex > 0) || 
                (userInput == 'Q' && currentQuestionIndex < questions.size() - 1) || 
                userInput == 'E') {
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
        cout << "Options:\n";
        for (char option = 'A'; option <= 'D'; ++option) {
            cout << option << ". " << questions[questionNumber - 1].options[option - 'A'] << endl;
        }
        cout << "Your Answer: " << (userAnswer == 'S' ? "Skipped" : string(1, userAnswer)) << endl;
        cout << "Correct Answer: " << questions[questionNumber - 1].correctOption << endl;
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

    void movePrevious() {
        if (currentQuestionIndex > 0) {
            currentQuestionIndex--;
            // Remove the skipped status when moving back
            skippedQuestions.erase(currentQuestionIndex + 2);
        }
    }
void moveNext() {
    currentQuestionIndex++;
    skippedQuestions[currentQuestionIndex] = questions[currentQuestionIndex - 1].userAnswer; // Store the user's answer
}

    void endQuiz() {
        cout << "Quiz ended. Thank you for participating!\n";
        // Calculate and display the final score
        int finalScore = calculateScore();
        cout << "Your final score: " << finalScore << " out of " << questions.size() << endl;
        exit(0); // End the application
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

    cout << ">>>>>>>>>>>>>>>>Welcome to the Quiz Application!<<<<<<<<<<<<<<<<<<<<<<<<\n";
    cout << "These are the rules\n";
    cout << "Click A B C or D to answer each question\n";
    cout << "Click P for previous question, N for next question and E to end the quiz.\n";
    cout << "Good Luck!!\n";

    char startQuizChoice;
    while (true) {
        cout << "Do you want to start quiz? (Y/N): ";
        cin >> startQuizChoice;

        startQuizChoice = tolower(startQuizChoice);

        if (startQuizChoice == 'n') {
            cout << "Aborted. Goodbye!!\n";
            return 0;  // End the application
        } else if (startQuizChoice == 'y') {
            break;
        } else {
            cout << "Invalid choice. Please enter 'Y' or 'N'.\n";
        }
    }

    // Get username and password from the user
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    // Authenticate user
    if (authSystem.authenticateUser(username, password)) {
        cout << "Authentication successful. Welcome, " << username << "!\n";
        // Add code for the rest of your application here
    } else {
        cout << "Authentication failed. Invalid username or password.\n";
        return 1; // Exit the application if authentication fails
    }

    do {
        // Display the current question
        questionManager.displayCurrentQuestion();

        // Allow users to input answers or skip/navigate the questions
        char userChoice = questionManager.getUserInput();

        if (userChoice == 'P') {
            questionManager.movePrevious();
        } else if (userChoice == 'Q') {
            questionManager.moveNext();
        } else if (userChoice == 'E') {
            questionManager.endQuiz();
        } else {
            questionManager.setUserAnswer(userChoice);
            questionManager.moveNext();
        }

    } while (!questionManager.isTestComplete());

    // Option to view the test script
    cout << "Do you want to view your test script? (Y/N): ";
    cin >> startQuizChoice;
    if (startQuizChoice == 'Y' || startQuizChoice == 'y') {
        questionManager.handleSkippedQuestions();
    }

    // Calculate and display the final score
    int finalScore = questionManager.calculateScore();
    cout << "Your final score: " << finalScore << " out of " << questionManager.getQuestions().size() << endl;

    return 0;
}
