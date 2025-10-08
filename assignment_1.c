#include <stdio.h>
#include<stdlib.h>
#include<ctype.h> 

int operand[100];
char operator[100];

int top_od = -1;
int top_or = -1;

void computation();
int precedence(char val);
void operand_push(int val);
void operator_push(char val);
int operand_pop();
char operator_pop();
char operator_top();
int eval_exp(char *exp, int size);
int is_valid_exp(char *exp, int size); 

int main()
{
    char exp[100];
    printf("Enter your expression :");
    fgets(exp, sizeof(exp), stdin);
    int i = 0;
    while (exp[i] != '\0')
    {
        i++;
    }


    if (!is_valid_exp(exp, i - 1)) {
        printf("Invalid expression syntax\n");
        return 1;
    }

    int result = eval_exp(exp, i - 1);
    printf("%d", result);
    return 0;
}

void computation()
{
    char top_operator = operator_top();
    int val2 = operand_pop();
    int val1 = operand_pop();
    int res = 0;
    if (top_operator == '+')
    {
        res = val1 + val2;
    }
    else if (top_operator == '-')
    {
        res = val1 - val2;
    }
    else if (top_operator == '*')
    {
        res = val1 * val2;
    }
    else if (top_operator == '/')
    {
        if (val2 == 0)
        {
            printf("dividing by zero");
            exit(1);
        }
        res = val1 / val2;
    }
    operator_pop();
    operand_push(res);
}

int precedence(char val)
{
    if (val == '/' || val == '*')
        return 2;
    else if (val == '+' || val == '-')
        return 1;

    return 0;
}

void operand_push(int val)
{
    if (top_od == 100)
    {
        printf("operand Stack overflow");
        return;
    }
    operand[++top_od] = val;
}

void operator_push(char val)
{
    if (top_or == 100)
    {
        printf("operator stack overflow");
        return;
    }
    operator[++top_or] = val;
}

int operand_pop()
{
    if (top_od == -1)
    {
        printf("operand stack underflow");
        exit(0);
    }
    return operand[top_od--];
}

char operator_pop()
{
    if (top_or == -1)
    {
        printf("operator stack underflow");
        exit(0);
    }
    return operator[top_or--];
}

char operator_top()
{
    if (top_or == -1)
        return '0';
    return operator[top_or];
}

int eval_exp(char *exp, int size)
{
    int i = 0;
    while (i < size)
    {
        if (exp[i] == ' ')
        {
            i++;
            continue;
        }
        if (exp[i] >= '0' && exp[i] <= '9')
        {
            int val = 0;
            while (exp[i] >= '0' && exp[i] <= '9')
            {
                val = val * 10 + (int)(exp[i] - '0');
                i++;
            }
            operand_push(val);
        }else if(exp[i]=='('){
            operator_push(exp[i++]);
        }else if(exp[i]==')'){
            while(operator_top()!='('){
                computation();
            }
            operator_pop();
            i++;
        }
        else if (exp[i] == '*' || exp[i] == '/' || exp[i] == '+' || exp[i] == '-')
        {
            if (top_or == -1)
            {
                operator_push(exp[i++]);
                continue;
            }
            if (precedence(operator_top()) < precedence(exp[i]))
                operator_push(exp[i++]);
            else
            {
                while (precedence(operator_top()) >= precedence(exp[i]))
                {
                    computation();
                }
                operator_push(exp[i]);
                i++;
            }
        }
        else
        {
            printf("invalid expression");
            return 0;
        }
    }
    while (top_or != -1)
    {
        computation();
    }
    return operand[top_od];
}


int is_valid_exp(char *exp, int size) {
    int i = 0, parentheses = 0;
    char prev = 0;

    while (i < size) {
        char curr = exp[i];

        if (curr == ' ') {
            i++;
            continue;
        }

        if (!(isdigit(curr) || curr == '+' || curr == '-' || curr == '*' || curr == '/' || curr == '(' || curr == ')')) {
            return 0; 
        }

        if (curr == '(') {
            parentheses++;
            if (exp[i+1] == ')') return 0; 
        } else if (curr == ')') {
            parentheses--;
            if (parentheses < 0) return 0;
        }

        if ((curr == '+' || curr == '-' || curr == '*' || curr == '/') &&
            (prev == '+' || prev == '-' || prev == '*' || prev == '/')) {
            return 0;
        }

        if (i == 0 && (curr == '+' || curr == '*' || curr == '/')) return 0; 
        if (i == size - 1 && (curr == '+' || curr == '-' || curr == '*' || curr == '/')) return 0; 

        prev = curr;
        i++;
    }

    if (parentheses != 0) return 0; 

    return 1;
}




