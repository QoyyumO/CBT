# Quiz Application

Welcome to the Quiz Application! This application allows users to take a quiz with multiple-choice questions, navigate between questions, and view their final score. 

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Files](#files)
- [Running the Application](#running-the-application)
- [Instructions](#instructions)

## Installation

1. **Clone the repository:**
    ```sh
    git clone https://github.com/your-repo/quiz-application.git
    ```
2. **Navigate to the project directory:**
    ```sh
    cd quiz-application
    ```

## Usage

1. **Open the project in VS Code:**
    ```sh
    code .
    ```
2. **Ensure you have a C++ compiler installed (e.g., g++, clang++).**

3. **Compile the application:**
    ```sh
    g++ -o quiz_app main.cpp
    ```
Each line in `questions.csv` should follow this format:
```
Question,OptionA,OptionB,OptionC,OptionD,CorrectOption
```
- Example:
```
 What is the capital of France?,Berlin,Madrid,Paris,Rome,C
```

### `users.csv` Format

Each line in `users.csv` should follow this format:
```
username,password
```
- Example:
```
JohnDoe,password123
```
## Running the Application

1. **Run the compiled application:**
    ```sh
    ./quiz_app
    ```

## Instructions

1. **Starting the Quiz:**
    - Upon launching the application, you will be prompted to start the quiz. Enter `Y` to start or `N` to exit.

2. **Authentication:**
    - Enter your username and password to log in.

3. **Answering Questions:**
    - For each question, choose your answer by entering `A`, `B`, `C`, or `D`.
    - You can navigate between questions:
      - `P` for the previous question
      - `Q` for the next question
      - `E` to end the quiz

4. **Viewing Skipped Questions:**
    - After completing the quiz, you can review your answers and see the correct answers.

5. **Final Score:**
    - Your score will be displayed at the end of the quiz.

## Sample CSV Files

### `questions.csv`
```
What is the capital of France?,Berlin,Madrid,Paris,Rome,C
Which planet is known as the "Red Planet"?,Venus,Mars,Jupiter,Saturn,B
Who wrote "Romeo and Juliet"?,Charles Dickens,William Shakespeare,Jane Austen,Mark Twain,B
What is the largest mammal on Earth?,Elephant,Giraffe,Gorilla,Blue Whale,D
In which year did Christopher Columbus reach the Americas?,1492,1607,1776,1850,A
```

### `users.csv`
```
JohnDoe,password123
JaneDoe,password456
Alice,pass789
Bob,pass101
```
## Files

- **main.cpp**: Contains the main code for the quiz application.
- **questions.csv**: Stores the questions and answers for the quiz.
- **users.csv**: Stores the user credentials for authentication.

### `questions.csv` Format



## License

- This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contributing

- Contributions are welcome! Please open an issue or submit a pull request.

```
