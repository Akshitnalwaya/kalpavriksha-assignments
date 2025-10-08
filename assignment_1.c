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

int main()
{
    char expression[100];
    printf("Enter your expression: ");
    fgets(expression, sizeof(expression), stdin);
    int length = 0;
    while (expression[length] != '\0')
        length++;

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

    if (currentOperator == '+') result = firstOperand + secondOperand;
    else if (currentOperator == '-') result = firstOperand - secondOperand;
    else if (currentOperator == '*') result = firstOperand * secondOperand;
    else if (currentOperator == '/') {
        if (secondOperand == 0) {
            printf("dividing by zero\n");
            exit(1);
        }
        result = firstOperand / secondOperand;
    }

    popOperator();
    pushOperand(result);
}

int getPrecedence(char symbol)
{
    if (symbol == '*' || symbol == '/') return 2;
    if (symbol == '+' || symbol == '-') return 1;
    return 0;
}

void pushOperand(double value)
{
    if (topOperand == 99) {
        printf("operand stack overflow\n");
        return;
    }
    operandStack[++topOperand] = value;
}

void pushOperator(char symbol)
{
    if (topOperator == 99) {
        printf("operator stack overflow\n");
        return;
    }
    operatorStack[++topOperator] = symbol;
}

double popOperand()
{
    if (topOperand == -1) {
        printf("operand stack underflow\n");
        exit(0);
    }
    return operandStack[topOperand--];
}

char popOperator()
{
    if (topOperator == -1) {
        printf("operator stack underflow\n");
        exit(0);
    }
    return operatorStack[topOperator--];
}

char peekOperator()
{
    if (topOperator == -1) return '0';
    return operatorStack[topOperator];
}

double evaluateExpression(char *expression, int length)
{
    int index = 0;
    while (index < length)
    {
        if (expression[index] == ' ') { index++; continue; }

        if (isdigit(expression[index]))
        {
            double number = 0;
            while (index < length && isdigit(expression[index]))
            {
                number = number * 10 + (expression[index] - '0');
                index++;
            }
            pushOperand(number);
        }
        else if (expression[index] == '(') pushOperator(expression[index++]);
        else if (expression[index] == ')') {
            while (peekOperator() != '(') performComputation();
            popOperator();
            index++;
        }
        else if (expression[index] == '+' || expression[index] == '-' || expression[index] == '*' || expression[index] == '/')
        {
            if (topOperator == -1) { pushOperator(expression[index++]); continue; }
            if (getPrecedence(peekOperator()) < getPrecedence(expression[index])) pushOperator(expression[index++]);
            else {
                while (getPrecedence(peekOperator()) >= getPrecedence(expression[index])) performComputation();
                pushOperator(expression[index++]);
            }
        }
        else {
            printf("invalid expression\n");
            return 0;
        }
    }
    while (topOperator != -1) performComputation();
    return operandStack[topOperand];
}

int isValidExpression(char *expression, int length)
{
    int index = 0, parenthesesCount = 0;
    char previousChar = 0;

    while (index < length)
    {
        char currentChar = expression[index];
        if (currentChar == ' ') { index++; continue; }
        if (!(isdigit(currentChar) || currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' || currentChar == '(' || currentChar == ')')) return 0;

        if (currentChar == '(') { parenthesesCount++; if (expression[index + 1] == ')') return 0; }
        else if (currentChar == ')') { parenthesesCount--; if (parenthesesCount < 0) return 0; }

        if ((currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') && 
            (previousChar == '+' || previousChar == '-' || previousChar == '*' || previousChar == '/')) return 0;

        if (index == 0 && (currentChar == '+' || currentChar == '*' || currentChar == '/')) return 0;
        if (index == length - 1 && (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/')) return 0;

        previousChar = currentChar;
        index++;
    }

    if (parenthesesCount != 0) return 0;
    return 1;
}
