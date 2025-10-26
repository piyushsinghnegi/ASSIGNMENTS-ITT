#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

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

long long calculate(long long a, long long b, char op, int* error) {
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

long long result(char* expr, long long* output) {
    long long values[MAX_LEN];
    char ops[MAX_LEN];
    int op_top = -1;
    int val_top = -1;
    int i = 0, error = 0;

    if (!isdigit(expr[0]) && expr[0] != '-' && expr[0] != '+'){
        return 2;
    }
    if (!isdigit(expr[strlen(expr) - 1])){
        return 2;
    }

    while (expr[i]) {
        if (expr[i] == '+' && (i == 0 || expr[i - 1] == '*' || expr[i - 1] == '/')) {
            i++;
            continue;
        }
        if (expr[i] == '-' && (i == 0 || expr[i - 1] == '*' || expr[i - 1] == '/')) {
            i++;
            long long num = 0;

            if (!isdigit(expr[i])) {
                return 2;
            }
            while (isdigit(expr[i])) {
                int digit = expr[i] - '0';
                if (num > (LLONG_MAX - digit) / 10) { 
                    return 3;
                }
                num = num * 10 + digit;
                i++;
            }
            values[++val_top] = -num;
            continue;
        }

        if (isdigit(expr[i])) {
            long long num = 0;
            
            while (isdigit(expr[i])) {
                int digit = expr[i] - '0';
                if (num > (LLONG_MAX - digit) / 10){ 
                    return 3;
                }
                num = num * 10 + digit;
                i++;
            }
            values[++val_top] = num;

        } else {
            char curr_op = expr[i];
            
            if (curr_op != '+' && curr_op != '-' && curr_op != '*' && curr_op != '/') {
                return 2;
            }
            while (op_top >= 0 && precedence(ops[op_top]) >= precedence(curr_op)) {
                if (val_top < 1) return 2;
                long long b = values[val_top--];
                long long a = values[val_top--];
                char op = ops[op_top--];
                long long res = calculate(a, b, op, &error);
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
        if (val_top < 1){
            return 2;
        }

        long long b = values[val_top--];
        long long a = values[val_top--];
        char op = ops[op_top--];
        long long res = calculate(a, b, op, &error);
        if (error == 1) return 1;
        if (error == 2) return 2;
        values[++val_top] = res;
    }

    *output = values[val_top];
    return 0;
}

void input(){
    char input[MAX_LEN];
    char ch;
    int count = 0;
    long long answer;

    printf("Enter equation: ");

    while ((ch = getchar()) != '\n' && count < MAX_LEN - 1) {
        if (ch == ' ') continue;
        input[count++] = ch;
    }
    input[count] = '\0';

    if (count == 0) {
        printf("Error: invalid statement\n");
        return;
    }

    long long status = result(input, &answer);

    if (status == 1){
        printf("Error: division by zero\n");

    } else if (status == 2){
        printf("Error: invalid statement\n");

    }  else if (status == 3) {
        printf("War: overflow\n");

    } else {
        printf("Result: %lld\n", answer);
    }
}

int main() {
    printf("-------- \t Welcome to calculator \t --------\n");
    
    input();
    
    return 0;
} 