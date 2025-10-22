#include <stdio.h>
#include <string.h>

#define SUBJECTS 3
#define MAX_STUDENTS 1000000
#define MAX_PERFORMANCE 10
#define MAX_NAME 100

const int gradeA = 85;
const int gradeB = 70;
const int gradeC = 50;
const int gradeD = 35;

struct Student {
    int roll;
    char name[MAX_NAME];
    int marks[SUBJECTS];
    int total;
    float average;
    char grade;
    char performance[MAX_PERFORMANCE];
};

void getGrade(float avg, char *grade, char *performance) {
    if (avg >= gradeA) {
        *grade = 'A'; 
        strcpy(performance, "*****");

    } else if (avg >= gradeB) {
        *grade = 'B';
        strcpy(performance, "****");

    } else if (avg >= gradeC) {
        *grade = 'C';
        strcpy(performance, "***");

    } else if (avg >= gradeD) {
        *grade = 'D';
        strcpy(performance, "**");

    } else {
        *grade = 'F';
        strcpy(performance, "");

    }
}

void result(struct Student std[], int index, int totalStudents) {
    if (index == totalStudents) {
        return;
    }
    printf("%d ", std[index].roll);
    result(std, index + 1, totalStudents);
}

int main() {
    long long inputStudentNo;
    
    printf("Enter number of students: ");
    
    if (scanf("%lld", &inputStudentNo) != 1 || inputStudentNo <= 0 || inputStudentNo > MAX_STUDENTS) {
        printf("Error: Invalid no of students \n");
        return 0;
    }
    int studentCount = (int)inputStudentNo;

    struct Student std[studentCount];

    // sorting 
    for (int stdIndex = 0; stdIndex < studentCount; stdIndex++) {

        scanf("%d %s %d %d %d", &std[stdIndex].roll, std[stdIndex].name, &std[stdIndex].marks[0], &std[stdIndex].marks[1], &std[stdIndex].marks[2]);
        std[stdIndex].total = std[stdIndex].marks[0] + std[stdIndex].marks[1] + std[stdIndex].marks[2];
        
        std[stdIndex].average = std[stdIndex].total / 3.0;
        getGrade(std[stdIndex].average, &std[stdIndex].grade, std[stdIndex].performance);
    }

    for (int outerIndex = 0; outerIndex < studentCount - 1; outerIndex++) {

        for (int innerIndex = 0; innerIndex < studentCount - outerIndex - 1; innerIndex++) {
        
            if (std[innerIndex].roll > std[innerIndex + 1].roll) {
                struct Student tempStd = std[innerIndex];
                std[innerIndex] = std[innerIndex + 1];
                std[innerIndex + 1] = tempStd;
            }
        }
    }

    printf("\n****** Student Results ******\n");
    for (int index = 0; index < studentCount; index++) {
        printf("Roll: %d\n", std[index].roll);
        printf("Name: %s\n", std[index].name);
        printf("Total: %d\n", std[index].total);
        printf("Average: %.2f\n", std[index].average);
        printf("Grade: %c\n", std[index].grade);
        printf("Performance: %s\n\n", std[index].performance);
    }

    printf("List of Roll Numbers (via recursion): ");
    result(std, 0 ,studentCount);
    printf("\n");

    return 0;
}
