#include <stdio.h>
#include <string.h>

struct Student {
    int roll;
    char name[100];
    int marks[3];
    int total;
    float average;
    char grade;
    char performance[10];
};

void getGrade(float avg, char *grade, char *performance) {
    if (avg >= 85) {
        *grade = 'A'; 
        strcpy(performance, "*****");

    } else if (avg >= 70) {
        *grade = 'B';
        strcpy(performance, "****");

    } else if (avg >= 50) {
        *grade = 'C';
        strcpy(performance, "***");

    } else if (avg >= 35) {
        *grade = 'D';
        strcpy(performance, "**");

    } else {
        *grade = 'F';
        strcpy(performance, "");

    }
}

void result(struct Student std[], int i, int no) {
    if (i == no) {
        return;
    }
    printf("%d ", std[i].roll);
    result(std, i + 1, no);
}

int main() {
    long long temp;
    
    printf("Enter number of students: ");
    
    if (scanf("%lld", &temp) != 1 || temp <= 0 || temp > 1000000) {
        printf("Error: Invalid no of students \n");
        return 0;
    }
    int no = (int)temp;

    struct Student std[no];

    // sorting 
    for (int i = 0; i < no; i++) {
        scanf("%d %s %d %d %d", &std[i].roll, std[i].name, &std[i].marks[0], &std[i].marks[1], &std[i].marks[2]);
        std[i].total = std[i].marks[0] + std[i].marks[1] + std[i].marks[2];
        
        std[i].average = std[i].total / 3.0;
        getGrade(std[i].average, &std[i].grade, std[i].performance);
    }

    for (int i = 0; i < no - 1; i++) {
        for (int j = 0; j < no - i - 1; j++) {
            if (std[j].roll > std[j + 1].roll) {
                struct Student temp = std[j];
                std[j] = std[j + 1];
                std[j + 1] = temp;
            }
        }
    }

    printf("\n****** Student Results ******\n");
    for (int i = 0; i < no; i++) {
        printf("Roll: %d\n", std[i].roll);
        printf("Name: %s\n", std[i].name);
        printf("Total: %d\n", std[i].total);
        printf("Average: %.2f\n", std[i].average);
        printf("Grade: %c\n", std[i].grade);
        printf("Performance: %s\n\n", std[i].performance);
    }

    printf("List of Roll Numbers (via recursion): ");
    result(std, 0 ,no);
    printf("\n");

    return 0;
}
