#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "users.txt"

void AddUser(){
    char _name[100];
    int _age;
    int _id = 0;

    int id;
    char name[100];
    int age;

    FILE *file = fopen(FILENAME, "a+");
    if(file == NULL){
        printf("Cannot create file!");
        return;
    }
    
    while (fscanf(file, "%d %s %d", &id, name, &age) == 3) {
        if (id > _id) _id = id;
    }

    _id += 1;
    printf("Enter name: ");
    scanf("%s", _name);
    
    printf("Enter age: ");
    scanf("%d", &_age);
    
    fprintf(file,"%d %s %d\n",_id,_name,_age);
    printf("\nUser added\n");

    fclose(file);
}

void ReadUser(){
    int id;
    char name[50];
    int age;

    FILE *file = fopen(FILENAME,"r"); 
    if(file == NULL){
        printf("Cannot open file!");
        return;
    }

    printf("\n List users \n");
    while (fscanf(file, "%d %s %d", &id, name, &age) == 3) {
        printf("Id: %d | Name: %s | Age: %d\n", id, name, age);
    }

    fclose(file);
}

void DeleteUser(){
    int id;
    char name[50];
    int age;
    
    int _id;
    int done=0; 

    FILE *file = fopen(FILENAME,"r");
    FILE *temp = fopen("temp.txt", "w");

    if(file == NULL || temp == NULL){
        printf("Cannot open file!");
        return;
    }
    
    printf("Enter id to remove user: ");
    scanf("%d", &_id);

    while (fscanf(file, "%d %s %d", &id, name, &age) == 3) {
        if(id == _id){
            done = 1;
            continue;
        }
        fprintf(temp, "%d %s %d\n", id, name, age);
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if(done == 1){
        printf("\nUser deleted\n");
    } else{
        printf("no user with this id!");
    }
}

void UpdateUser(){
    int id;
    char name[100];
    int age;

    int _id;
    char _name[100];
    int _age;

    int done=0;

    FILE *file = fopen(FILENAME,"r");
    FILE *temp = fopen("temp.txt", "w");

    if(file == NULL){
        printf("Cannot open file!");
        return;
    }

    printf("Enter id to update: ");
    scanf("%d", &_id);

    printf("Enter name: ");
    scanf("%s", _name);
    
    printf("Enter age: ");
    scanf("%d", &_age);

    while (fscanf(file, "%d %s %d", &id, name, &age) == 3) {
        if(id == _id){
            fprintf(temp,"%d %s %d\n", _id, _name, _age);
            done = 1;
        } else {
            fprintf(temp,"%d %s %d\n", id, name, age);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if(done == 1){
        printf("\nUser updated\n");
    } else{
        printf("no user with this id!");
    }
}

int main(){
    printf("-------- \t Welcome to CRUD in file \t --------\n");

    int choice;

    do {
        printf("\n1. Add User\n");
        printf("2. List Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                AddUser();
                break;
            case 2:
                ReadUser();
                break;
            case 3:
                UpdateUser();
                break;
            case 4:
                DeleteUser();
                break;
            case 5:
                printf("Exit\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 5);

    return 0;
}
