#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

double operandStack[100];
char operatorStack[100];

int topOperand = -1;
int topOperator = -1;

void performComputation();
int getPrecedence(char symbol);
void pushOperand(double value);
void pushOperator(char symbol);
double popOperand();
char popOperator();
char peekOperator();
double evaluateExpression(char *expression, int length);
int isValidExpression(char *expression, int length);
int isOperator(char operatorSymbol);

int main()
{
    char expression[100];
    printf("Enter your expression: ");
    fgets(expression, sizeof(expression), stdin);
    int length = 0;
    while (expression[length] != '\0') {
        length++;
    }

    if (!isValidExpression(expression, length - 1)) {
        printf("Invalid expression syntax\n");
        return 1;
    }

    double result = evaluateExpression(expression, length - 1);
    printf("%lf\n", result);
    return 0;
}

void performComputation()
{
    char currentOperator = peekOperator();
    double secondOperand = popOperand();
    double firstOperand = popOperand();
    double result = 0;

    switch (currentOperator) {
        case '+':
            result = firstOperand + secondOperand;
            break;
        case '-':
            result = firstOperand - secondOperand;
            break;
        case '*':
            result = firstOperand * secondOperand;
            break;
        case '/':
            if (secondOperand == 0) {
                printf("Dividing by zero\n");
                exit(1);
            }
            result = firstOperand / secondOperand;
            break;
        default:
            printf("Invalid operator encountered\n");
            exit(1);
    }

    popOperator();
    pushOperand(result);
}

int getPrecedence(char symbol)
{

    switch (symbol) {
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

void pushOperand(double value)
{
    if (topOperand >= 99) {
        printf("Operand stack overflow\n");
        return;
    }
    operandStack[++topOperand] = value;
}

void pushOperator(char symbol)
{
    if (topOperator >= 99) {
        printf("Operator stack overflow\n");
        return;
    }
    operatorStack[++topOperator] = symbol;
}

double popOperand()
{
    if (topOperand == -1) {
        printf("Operand stack underflow\n");
        exit(0);
    }
    return operandStack[topOperand--];
}

char popOperator()
{
    if (topOperator == -1) {
        printf("Operator stack underflow\n");
        exit(0);
    }
    return operatorStack[topOperator--];
}

char peekOperator()
{
    if (topOperator == -1) {
        return '0';
    }
    return operatorStack[topOperator];
}

int isOperator(char operatorSymbol)
{
    return (operatorSymbol == '+' || operatorSymbol == '-' ||
            operatorSymbol == '*' || operatorSymbol == '/');
}

double evaluateExpression(char *expression, int length)
{
    int index = 0;
    while (index < length) {
        if (expression[index] == ' ') {
            index++;
            continue;
        }

        if (isdigit(expression[index])) {
            double number = 0;
            while (index < length && isdigit(expression[index])) {
                number = number * 10 + (expression[index] - '0');
                index++;
            }
            pushOperand(number);
        } else if (expression[index] == '(') {
            pushOperator(expression[index++]);
        } else if (expression[index] == ')') {
            while (peekOperator() != '(') {
                performComputation();
            }
            popOperator();
            index++;
        } else if (isOperator(expression[index])) {
            if (topOperator == -1) {
                pushOperator(expression[index++]);
                continue;
            }
            if (getPrecedence(peekOperator()) < getPrecedence(expression[index])) {
                pushOperator(expression[index++]);
            } else {
                while (getPrecedence(peekOperator()) >= getPrecedence(expression[index])) {
                    performComputation();
                }
                pushOperator(expression[index++]);
            }
        } else {
            printf("Invalid expression\n");
            return 0;
        }
    }

    while (topOperator != -1) {
        performComputation();
    }

    return operandStack[topOperand];
}

int isValidExpression(char *expression, int length)
{
    int index = 0, parenthesesCount = 0;
    char previousChar = 0;

    while (index < length) {
        char currentChar = expression[index];
        if (currentChar == ' ') {
            index++;
            continue;
        }

        if (!(isdigit(currentChar) || isOperator(currentChar) ||
              currentChar == '(' || currentChar == ')')) {
            return 0;
        }


        switch (currentChar) {
            case '(':
                parenthesesCount++;
                if (expression[index + 1] == ')') {
                    return 0;
                }
                break;
            case ')':
                parenthesesCount--;
                if (parenthesesCount < 0) {
                    return 0;
                }
                break;
        }

        if (isOperator(currentChar) && isOperator(previousChar)) {
            return 0;
        }

        if (index == 0 && (currentChar == '+' || currentChar == '*' || currentChar == '/')) {
            return 0;
        }
        if (index == length - 1 && isOperator(currentChar)) {
            return 0;
        }

        previousChar = currentChar;
        index++;
    }

    if (parenthesesCount != 0) {
        return 0;
    }
    return 1;
}
