#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INITIAL_PRODS 100
#define MAX_PROD_ID 10000
#define MAX_PROD_NAME_LEN 50
#define MAX_PROD_PRICE 100000
#define MAX_PROD_QTY 1000000

struct ProductInfo{
    int prodId;
    char prodName[MAX_PROD_NAME_LEN];
    float prodPrice;
    int prodQty;
    int error;
};

void addProd(struct ProductInfo **prodArr, int *prodLen){
    *prodLen += 1;

    *prodArr = realloc(*prodArr, (*prodLen) * sizeof(struct ProductInfo));

    if(*prodArr == NULL){
        printf("Allocation failed!");
        return;
    }


    printf("Enter new product details:-\n");
    printf("Product ID: ");
    scanf("%d",&(*prodArr)[*prodLen - 1].prodId);
    printf("Product Name: ");
    scanf("%s",(*prodArr)[*prodLen - 1].prodName);
    printf("Product Price: ");
    scanf("%f",&(*prodArr)[*prodLen -1].prodPrice);
    printf("Product Quantity: ");
    scanf("%d",&(*prodArr)[*prodLen -1].prodQty);  
    (*prodArr)[*prodLen -1].error = 0;

    if((*prodArr)[*prodLen -1].prodId > MAX_PROD_ID || (*prodArr)[*prodLen -1].prodPrice > MAX_PROD_PRICE || (*prodArr)[*prodLen -1].prodQty > MAX_PROD_QTY ){
        (*prodArr)[*prodLen -1].error = 1;
    }
    printf("Product added successfully!\n");
}

void viewProd(struct ProductInfo *prodArr,int prodLen){
    printf("\n========= PRODUCT LIST ========= \n");
    for(int i =0;i < prodLen;i++){
        if(prodArr[i].error != 1){
            printf(" Product ID: %d | Name: %s | Price: %f | Quantity: %d \n",prodArr[i].prodId,prodArr[i].prodName,prodArr[i].prodPrice,prodArr[i].prodQty);
        }else{
            printf("Invalid Product!\n");
        }
    }

}

void updateQty(struct ProductInfo *prodArr,int prodLen){
    int tempId;
    int found = 0;
    
    printf("Enter Product ID to update quantity: ");
    scanf("%d",&tempId);

    for (int i = 0; i < prodLen; i++) {
        if (prodArr[i].prodId == tempId && prodArr[i].error != 1) {
            printf("Enter new Quantity: ");
            scanf("%d", &prodArr[i].prodQty);
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (found == 0){
        printf("No product found!\n");
    }
}

void searchById(struct ProductInfo *prodArr,int prodLen){
    int tempId;
    int found = 0;

    printf("Enter Product ID to search: ");
    scanf("%d",&tempId);

    for (int i = 0; i < prodLen; i++) {
        if (prodArr[i].prodId == tempId && prodArr[i].error != 1) {
            printf(" Product ID: %d | Name: %s | Price: %f | Quantity: %d \n",prodArr[i].prodId,prodArr[i].prodName,prodArr[i].prodPrice,prodArr[i].prodQty);
            found++;
            break;
        }
    }
    if (found == 0){
        printf("Product not found.\n");
    }
}

void searchByName(struct ProductInfo *prodArr,int prodLen){
    char tempName[MAX_PROD_NAME_LEN];
    int isFound = 0;

    printf("Enter name to search (partial allowed): ");
    scanf("%s",tempName);

    for(int i=0;i<prodLen;i++){
        if(strstr(prodArr[i].prodName, tempName) && prodArr[i].error != 1){
            isFound =1;
            printf(" Product ID: %d | Name: %s | Price: %f | Quantity: %d \n",prodArr[i].prodId,prodArr[i].prodName,prodArr[i].prodPrice,prodArr[i].prodQty);
        }
    }

    if(isFound == 0){
        printf("No Product with this name!\n");
    }
}


void searchByRange(struct ProductInfo *prodArr,int prodLen){
    float maxPriceTemp;
    float minPriceTemp;
    int found = 0;
    
    printf("Enter minimum price: ");
    scanf("%f",&minPriceTemp);

    printf("Enter max price: ");
    scanf("%f",&maxPriceTemp);

    for(int i=0;i < prodLen;i++){
        if(prodArr[i].prodPrice <= maxPriceTemp && prodArr[i].prodPrice >= minPriceTemp){
            if(prodArr[i].error == 0){
                printf("ID: %d | Name: %s | Price: %.2f | Qty: %d\n", prodArr[i].prodId, prodArr[i].prodName, prodArr[i].prodPrice, prodArr[i].prodQty);
                found++;
            }
        }
    }

    if(found == 0){
        printf("No product found in given price range!");
    }
}   

void deleteProd(struct ProductInfo **prodArr,int *prodLen){
    int tempId;
    int found=0;
    int tempArrIndex = 0;

    struct ProductInfo *tempProdArr = (struct ProductInfo*) calloc((*prodLen)-1 ,sizeof(struct ProductInfo));
    
    if(tempProdArr == NULL){
        printf("Allocation failed!");
        return;
    }

    printf("Enter Product ID to delete: ");
    scanf("%d",&tempId);

    for(int i=0;i<*prodLen;i++){
        if((*prodArr)[i].prodId == tempId && (*prodArr)[i].error!=1){
            found++;
            continue;
        } else if (tempArrIndex < (*prodLen) - 1){
            tempProdArr[tempArrIndex] = (*prodArr)[i];
            tempArrIndex++;
        }
    }
    
    if(found == 0){
        printf("No product found for given id!\n");
        free(tempProdArr);
    } else {
        free(*prodArr); 
        *prodArr = tempProdArr;
        *prodLen = *prodLen - 1;
        printf("Deleted Successfully\n");
    }
}

void getInitialProd(int productsLen,struct ProductInfo *prodArr){
    for(int itr = 0;itr<productsLen;itr++){
        printf("\nEnter details for product %d :-",itr+1);
        printf("\nProduct ID: ");
        scanf("%d",&prodArr[itr].prodId);
        printf("Product Name: ");
        scanf("%s",prodArr[itr].prodName);
        printf("Product Price: ");
        scanf("%f",&prodArr[itr].prodPrice);
        printf("Product Quantity: ");
        scanf("%d",&prodArr[itr].prodQty);
        prodArr[itr].error = 0;
        
        if(prodArr[itr].prodId > MAX_PROD_ID || prodArr[itr].prodPrice > MAX_PROD_PRICE || prodArr[itr].prodQty > MAX_PROD_QTY ){
            prodArr[itr].error = 1;
        }
    }
}


int main(){
    int productsLen;
    int choice;

    printf("\n========= INVENTORY MENU =========\n");
    printf("Enter the initial number of products: ");
    
    if(scanf("%d",&productsLen) != 1 || productsLen < 1 || productsLen > MAX_INITIAL_PRODS){
        printf("Should be in range [1,100]!\n");
        return 1;
    }
    
    struct ProductInfo *prodArr = (struct ProductInfo*) calloc(productsLen ,sizeof(struct ProductInfo));

    if(prodArr == NULL){
        printf("Allocation failed!\n");
        return 1;
    }

    getInitialProd(productsLen,prodArr);

    do{
        printf("\n========= INVENTORY MENU ========= \n");
        printf("1. Add New Product\n2. View All Products \n3. Update Quantity \n4. Search Product by ID \n5. Search Product by Name \n6. Search Product by Price Range \n7. Delete Product \n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                addProd(&prodArr,&productsLen);
                break;
            case 2:
                viewProd(prodArr,productsLen);
                break;
            case 3:
                updateQty(prodArr,productsLen);
                break;
            case 4:
                searchById(prodArr,productsLen);
                break;
            case 5:
                searchByName(prodArr,productsLen);
                break;
            case 6:
                searchByRange(prodArr,productsLen);
                break;
            case 7:
                deleteProd(&prodArr,&productsLen);
                break;
            case 8:
                printf("\nExiting...");
                break;
            default:
                printf("Invalid choice!");
        };
    } while (choice != 8);

    free(prodArr);
    printf("\nMemory released successfully.");

    return 0;
}