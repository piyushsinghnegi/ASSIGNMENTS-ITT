#include <stdio.h>
#include <ctype.h>

#define MAX_LEN 200

int precedence(char op) {
    if (op == '+' || op == '-'){
        return 1;
    }
    if (op == '*' || op == '/'){
        return 2;
    } 
    return 0;
}

int calculate(int a, int b, char op, int* error) {
    if (op == '+') {
        return a + b;
    }
    if (op == '-'){
        return a - b;
    }
    if (op == '*') {
        return a * b;
    }
    if (op == '/') {
        if (b == 0) {
            *error = 1;
            return 0;
        }
        return a / b;
    }
    *error = 2;
    return 0;
}
int result1(char* expr, int* result) {
    int values[MAX_LEN], val_top = -1;
    char ops[MAX_LEN];
    int op_top = -1;
    int i = 0, error = 0;

    while (expr[i]) {
        if (isdigit(expr[i])) {
            int num = 0;
            while (isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            values[++val_top] = num;
        } else {
            char curr_op = expr[i];
            if (curr_op != '+' && curr_op != '-' && curr_op != '*' && curr_op != '/') {
                return 2;
            }
            while (op_top >= 0 && precedence(ops[op_top]) >= precedence(curr_op)) {
                int b = values[val_top--];
                int a = values[val_top--];
                char op = ops[op_top--];
                int res = calculate(a, b, op, &error);
                if (error == 1){
                    return 1;
                }
                if (error == 2){
                    return 2;
                }
                values[++val_top] = res;
            }
            ops[++op_top] = curr_op;
            i++;
        }
    }

    while (op_top >= 0) {
        int b = values[val_top--];
        int a = values[val_top--];
        char op = ops[op_top--];
        int res = calculate(a, b, op, &error);
        if (error == 1) {
            return 1;
        }
        if (error == 2) {
            return 2;
        }
        values[++val_top] = res;
    }

    *result = values[val_top];
    return 0;
}

void Input(){
    char input[200];
    char ch = 0;
    int count = 0;
    int result;

    printf("Enter equation: ");

    while ((ch = getchar()) != '\n') {
        if (ch == ' ') continue;
        input[count++] = ch;
    }
    input[count] = '\0';

    int status = result1(input, &result);

    if (status == 1) {
        printf("Error: division by zero\n");
    } else if (status == 2) {
        printf("Error: invalid statement\n");
    } else {
        printf("Result: %d\n", result);
    }
}

int main() {
    printf("-------- \t Welcome to calculator \t --------\n");
    
    Input();
    
    return 0;
}