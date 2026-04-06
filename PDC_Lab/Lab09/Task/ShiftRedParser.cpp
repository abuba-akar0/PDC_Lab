#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

// Stack and input buffer
char stk[50][10];   // Stack holds tokens like "id", "E", "+", "*"
int top = 0;
char input[10][10]; // Tokenized input
int inputSize = 0;
int inputPos = 0;

// Print current stack
void printStack() {
    cout << "$";
    for (int i = 0; i < top; i++)
        cout << stk[i];
}

// Print remaining input
void printInput() {
    cout << "$";
    for (int i = inputPos; i < inputSize; i++)
        cout << input[i];
    cout << "$";
}

// Check and apply REDUCE rules
bool check() {
    bool reduced = false;

    // Rule: E -> id
    if (top >= 1 && strcmp(stk[top - 1], "id") == 0) {
        strcpy(stk[top - 1], "E");
        cout << "\t REDUCE E -> id";
        reduced = true;
    }

    // Rule: E -> E + E
    if (top >= 3 &&
        strcmp(stk[top - 3], "E") == 0 &&
        strcmp(stk[top - 2], "+") == 0 &&
        strcmp(stk[top - 1], "E") == 0) {
        top -= 2;
        strcpy(stk[top - 1], "E");
        cout << "\t REDUCE E -> E + E";
        reduced = true;
    }

    // Rule: E -> E * E
    if (top >= 3 &&
        strcmp(stk[top - 3], "E") == 0 &&
        strcmp(stk[top - 2], "*") == 0 &&
        strcmp(stk[top - 1], "E") == 0) {
        top -= 2;
        strcpy(stk[top - 1], "E");
        cout << "\t REDUCE E -> E * E";
        reduced = true;
    }

    return reduced;
}

int main() {
    cout << "GRAMMAR:\n";
    cout << "E -> E + E\n";
    cout << "E -> E * E\n";
    cout << "E -> id\n\n";

    // Tokenize input: id + id * id
    strcpy(input[0], "id");
    strcpy(input[1], "+");
    strcpy(input[2], "id");
    strcpy(input[3], "*");
    strcpy(input[4], "id");
    inputSize = 5;
    inputPos = 0;
    top = 0;

    cout << "Stack\t\tInput\t\tAction\n";
    cout << "$\t\t";
    printInput();
    cout << "\t\tStart\n";

    while (inputPos < inputSize) {
        // SHIFT
        strcpy(stk[top], input[inputPos]);
        top++;
        inputPos++;

        cout << "\n";
        printStack();
        cout << "\t\t";
        printInput();
        cout << "\t\t SHIFT";

        // Try REDUCE (may apply multiple times)
        while (check()) {
            cout << "\n";
            printStack();
            cout << "\t\t";
            printInput();
        }
    }

    // Final reduce attempts
    while (check()) {
        cout << "\n";
        printStack();
        cout << "\t\t$\t\t";
    }

    cout << "\n\n";
    if (top == 1 && strcmp(stk[0], "E") == 0)
        cout << "Result: ACCEPT\n";
    else
        cout << "Result: REJECT\n";

    return 0;
}