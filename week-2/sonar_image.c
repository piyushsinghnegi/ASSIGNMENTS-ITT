#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void smoothingArr(int **arr,int size){
    const int SHIFT_BITS = 10;
    const int VALUE = (1 << SHIFT_BITS) - 1;

    for (int row = 0; row < size; row++) {
        
        for (int col = 0; col < size; col++) {
            int sum = 0;
            int neighborCount = 0;

            for (int windowRow = row-1; windowRow <= row+1; windowRow++) {

                for (int windowCol = col-1; windowCol <= col+1; windowCol++) {

                    if (windowCol < 0 || windowCol >= size || windowRow < 0 || windowRow >= size){
                        continue;
                    }

                    int originalValue = *(*(arr + windowRow) + windowCol) & VALUE;
                    sum += originalValue;
                    neighborCount++;
                }
            }

            int newValue = sum / neighborCount;

            *(*(arr + row) + col) = *(*(arr + row) + col) | (newValue << SHIFT_BITS);
        }
    }

    for (int row = 0; row < size; row++) {
        
        for (int col = 0; col < size; col++) {

            *(*(arr + row) + col) = *(*(arr + row) + col) >> SHIFT_BITS;
        }
    }
}

void rotateArr(int **arr,int size){

    for (int row = 0; row < size; row++) {
        
        for (int col = row + 1; col < size; col++) {
            int temp = *(*(arr + row) + col);
            *(*(arr + row) + col) = *(*(arr + col) + row);
            *(*(arr + col) + row) = temp;
        }
    }

    
    for (int row = 0; row < size; row++) {
        
        for (int col = 0; col < size / 2; col++) {
            int temp = *(*(arr + row) + col);
            *(*(arr + row) + col) = *(*(arr + row) + (size - 1 - col));
            *(*(arr + row) + (size - 1 - col)) = temp;
        }
    }
}

void showArr(int **arr,int size){
    for(int row=0;row<size;row++){

        for(int col=0;col<size;col++){
           printf("%d ",*(*(arr + row) + col));
        }
        printf("\n");
    }
}

void generateArr(int **arr,int size){
    for(int row=0; row<size ;row++){
        
        for(int col=0; col<size; col++){
           *(*(arr + row) + col) = rand() % 255;
        }
    }
}

int main(){
    int size;
    scanf("%d",&size);

    srand(time(NULL));
    
    if(size<2 || size>10){
        printf("size should be in range [2,10]!");
        return 0;
    }
    
    int **arr = (int **) malloc(size * sizeof(int *));
    
    for (int row = 0; row < size; row++){
        *(arr + row) = (int *) malloc(size * sizeof(int));
    }
    
    generateArr(arr,size);
    
    printf("Generated array:-\n");
    showArr(arr,size);
    
    rotateArr(arr,size);
    
    printf("\nAfter 90deg rotation:-\n");
    showArr(arr,size);
    
    smoothingArr(arr,size);
    
    printf("\nAfter smoothing arr:-\n");
    showArr(arr,size);
    
    for (int i = 0; i < size; i++) {
        free(*(arr + i));
    }
    free(arr);

    return 0;
}