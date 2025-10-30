#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_INITIAL_PRODUCTS 100
#define MAX_PRODUCT_ID 10000
#define MAX_PRODUCT_NAME_LENGTH 50
#define MAX_PRODUCT_PRICE 100000
#define MAX_PRODUCT_QTY 1000000

#define MIN_INITIAL_PRODUCTS 1
#define MIN_PRODUCT_ID 1
#define MIN_PRODUCT_NAME_LENGTH 1
#define MIN_PRODUCT_PRICE 0
#define MIN_PRODUCT_QTY 0

struct ProductDetails{
    int id;
    char name[MAX_PRODUCT_NAME_LENGTH];
    float price;
    int qty;
};

bool isValidName(const char *name) {
    int nameLength = strlen(name);
    if (nameLength < MIN_PRODUCT_NAME_LENGTH || nameLength >= MAX_PRODUCT_NAME_LENGTH) {
        return false;
    }
    for (int i = 0; i < nameLength; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return false; // only letters and spaces allowed
        }
    }
    return true;
}

bool haveDuplicateID(struct ProductDetails *products, int numberOfProducts, int id) {
    for(int product=0; product<numberOfProducts; product++) {
        if(products[product].id == id) {
            return true;
        }
    }
    return false;
}

void addProduct(struct ProductDetails **products, int *numberOfProducts){
    *numberOfProducts += 1;

    *products = realloc(*products, (*numberOfProducts) * sizeof(struct ProductDetails));

    if(*products == NULL){
        printf("Allocation failed!");
        return;
    }

    printf("Enter new product details:-\n");
    do {
        printf("Product ID: ");
        if(scanf("%d",&(*products)[*numberOfProducts - 1].id) != 1 || (*products)[*numberOfProducts - 1].id <MIN_PRODUCT_ID || (*products)[*numberOfProducts - 1].id > MAX_PRODUCT_ID){
            printf("should be in range [%d, %d].\n", MIN_PRODUCT_ID, MAX_PRODUCT_ID);
            while (getchar() != '\n');
            continue;
        }
        if (haveDuplicateID(*products, *numberOfProducts - 1, (*products)[*numberOfProducts - 1].id)) {
            printf("cannot have duplicate id!.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    do {
        printf("Product Name: ");
        scanf(" %[^\n]", (*products)[*numberOfProducts - 1].name);
        if (!isValidName((*products)[*numberOfProducts - 1].name)) {
            printf("Invalid name! Use only letters and spaces, length %d-%d.\n", MIN_PRODUCT_NAME_LENGTH, MAX_PRODUCT_NAME_LENGTH-1);
            while(getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    do {
        printf("Product Price: ");
        if(scanf("%f",&(*products)[*numberOfProducts -1].price) != 1 || (*products)[*numberOfProducts -1].price < MIN_PRODUCT_PRICE || (*products)[*numberOfProducts -1].price > MAX_PRODUCT_PRICE ){
            printf("should be in range [%d, %d].\n", MIN_PRODUCT_PRICE, MAX_PRODUCT_PRICE);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    do {
        printf("Product Quantity: ");
        if(scanf("%d",&(*products)[*numberOfProducts -1].qty) != 1 || (*products)[*numberOfProducts -1].qty < MIN_PRODUCT_QTY || (*products)[*numberOfProducts -1].qty > MAX_PRODUCT_QTY ){
            printf("should be in range [%d, %d].\n", MIN_PRODUCT_QTY, MAX_PRODUCT_QTY);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    printf("Product added successfully!\n");
}

void listProducts(struct ProductDetails *products,int numberOfProducts){
    printf("\n========= PRODUCT LIST ========= \n");
    for(int product =0 ; product < numberOfProducts ; product++){
        printf(" Product ID: %d | Name: %s | Price: %f | Quantity: %d \n",products[product].id,products[product].name,products[product].price,products[product].qty);
    }

}

void updateProductQuantity(struct ProductDetails *products,int numberOfProducts){
    int searchProductId;
    int found = 0;
    
    do{
        printf("Enter Product ID to update quantity: ");
        if(scanf("%d",&searchProductId) != 1 || searchProductId < MIN_PRODUCT_ID || searchProductId > MAX_PRODUCT_ID){
            printf("should be in range [%d,%d]",MIN_PRODUCT_ID,MAX_PRODUCT_ID);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while(1);

    for (int product = 0; product < numberOfProducts; product++) {
        if (products[product].id == searchProductId) {
            do{
                printf("Enter new Quantity: ");
                if(scanf("%d", &products[product].qty) != 1 || products[product].qty < MIN_PRODUCT_QTY || products[product].qty > MAX_PRODUCT_QTY){
                    printf("should be in range [%d,%d]",MIN_PRODUCT_QTY,MAX_PRODUCT_QTY);
                    while (getchar() != '\n');
                    continue;
                }
                break;
            } while (1);
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (found == 0){
        printf("No product found!\n");
    }
}

void searchProductById(struct ProductDetails *products,int numberOfProducts){
    int searchId;
    int found = 0;

    do{
        printf("Enter Product ID to search: ");
        if(scanf("%d",&searchId) != 1 || searchId < MIN_PRODUCT_ID || searchId > MAX_PRODUCT_ID){
            printf("should be in range [%d,%d]",MIN_PRODUCT_ID,MAX_PRODUCT_ID);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    for (int product = 0; product < numberOfProducts; product++) {
        if (products[product].id == searchId) {
            printf(" Product ID: %d | Name: %s | Price: %f | Quantity: %d \n",products[product].id,products[product].name,products[product].price,products[product].qty);
            found++;
            break;
        }
    }
    if (found == 0){
        printf("Product not found.\n");
    }
}

void searchProductByName(struct ProductDetails *products,int numberOfProducts){
    char searchName[MAX_PRODUCT_NAME_LENGTH];
    int isFound = 0;

    do{
        printf("Enter name to search (partial allowed): ");
        if(scanf("%49s",searchName) != 1){
            printf("Invalid Input");
            while (getchar() != '\n');
            continue;
        }
        for (int i = 0; searchName[i]; i++) {
            if (isdigit(searchName[i])) {
                printf("Enter valid name!");
                
                continue;
            }
        }
        break;
    } while (1);
    

    for(int product=0 ; product<numberOfProducts ; product++){
        if(strstr(products[product].name, searchName) != NULL){
            isFound =1;
            printf(" Product ID: %d | Name: %s | Price: %f | Quantity: %d \n",products[product].id,products[product].name,products[product].price,products[product].qty);
        }
    }

    if(isFound == 0){
        printf("No Product with this name!\n");
    }
}


void searchProductByPriceRange(struct ProductDetails *products,int numberOfProducts){
    float maxPrice;
    float minPrice;
    int found = 0;
    
    printf("Enter minimum price: ");
    do{
        if(scanf("%f",&minPrice) != 1 || minPrice < MIN_PRODUCT_PRICE || minPrice > MAX_PRODUCT_PRICE){
            printf("Price should be in range [%d,%d]\n",MIN_PRODUCT_PRICE,MAX_PRODUCT_PRICE);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    do{
        printf("Enter max price: ");
        if(scanf("%f",&maxPrice) != 1 || maxPrice < MIN_PRODUCT_PRICE || maxPrice > MAX_PRODUCT_PRICE){
            printf("Price should be in range [%d,%d]\n",MIN_PRODUCT_PRICE,MAX_PRODUCT_PRICE);
            while (getchar() != '\n');
            continue;
        }
        break;
    }while(1);

    for(int product=0; product < numberOfProducts ; product++){
        if(products[product].price >= minPrice && products[product].price <= maxPrice){
            printf("ID: %d | Name: %s | Price: %.2f | Qty: %d\n", products[product].id, products[product].name, products[product].price, products[product].qty);
            found++;
        }
    }

    if(found == 0){
        printf("No product found in given price range!");
    }
}   

void deleteProductById(struct ProductDetails **products,int *numberOfProducts){
    int IdToDelete;
    int found=0;
    int tempProductLength = 0;

    struct ProductDetails *tempProduct = (struct ProductDetails*) calloc((*numberOfProducts)-1 ,sizeof(struct ProductDetails));
    
    if(tempProduct == NULL){
        printf("Allocation failed!");
        return;
    }

    do{
        printf("Enter Product ID to delete: ");
        if (scanf("%d",&IdToDelete) != 1 || IdToDelete < MIN_PRODUCT_ID || IdToDelete > MAX_PRODUCT_ID){
            printf("should be in range [%d,%d]",MIN_PRODUCT_ID,MAX_PRODUCT_ID);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while(1);

    for(int product=0;product<*numberOfProducts;product++){
        if((*products)[product].id == IdToDelete){
            found++;
            continue;
        } else if (tempProductLength < (*numberOfProducts) - 1){
            tempProduct[tempProductLength] = (*products)[product];
            tempProductLength++;
        }
    }
    
    if(found == 0){
        printf("No product found for given id!\n");
        free(tempProduct);
        tempProduct = NULL;
    } else {
        free(*products); 
        *products = tempProduct;
        *numberOfProducts = *numberOfProducts - 1;
        printf("Deleted Successfully\n");
    }
}

void getInitialProducts(int numberOfProducts,struct ProductDetails *products){
    for(int product = 0; product < numberOfProducts ; product++){
        printf("\nEnter details for product %d :-",product + 1);
        
        do{
            printf("\nProduct ID: ");
            if(scanf("%d",&products[product].id) != 1 || products[product].id < MIN_PRODUCT_ID || products[product].id > MAX_PRODUCT_ID){
                printf("should be in range [%d,%d]",MIN_PRODUCT_ID,MAX_PRODUCT_ID);
                while (getchar() != '\n');
                continue;
            }
            if(haveDuplicateID(products,product, products[product].id)){
                printf("Duplicate Id not allowed");
                continue;
            }
            break;
        } while(1);
        
        
        do{
            printf("Product Name: ");
            scanf(" %[^\n]", products[product].name);
            if (!isValidName(products[product].name)) {
                printf("Invalid name! Use only letters and spaces, length %d-%d.\n", MIN_PRODUCT_NAME_LENGTH, MAX_PRODUCT_NAME_LENGTH-1);
                while(getchar() != '\n');
                continue;
            }
            break;
        }while(1);

        do{
            printf("Product Price: ");
            if (scanf("%f",&products[product].price) != 1 || products[product].price < MIN_PRODUCT_PRICE || products[product].price > MAX_PRODUCT_PRICE){
                printf("should be in range [%d,%d]",MIN_PRODUCT_PRICE,MAX_PRODUCT_PRICE);
                while (getchar() != '\n');
                continue;
            }
            break;
        }while(1);

        do{
            printf("Product Quantity: ");
            if (scanf("%d",&products[product].qty) != 1 || products[product].qty < MIN_PRODUCT_QTY || products[product].qty > MAX_PRODUCT_QTY){
                printf("should be in range [%d,%d]",MIN_PRODUCT_QTY,MAX_PRODUCT_QTY);
                while (getchar() != '\n');
                continue;
            }
            break;
        }while(1);
    }
}


int main(){
    int numberOfProducts;
    int choice;

    printf("\n========= INVENTORY MENU =========\n");
    
    do{
        printf("Enter the initial number of products: ");
        if(scanf("%d",&numberOfProducts) != 1 || numberOfProducts < MIN_INITIAL_PRODUCTS || numberOfProducts > MAX_INITIAL_PRODUCTS){
            printf("Should be in range [%d,%d]!\n",MIN_INITIAL_PRODUCTS,MAX_INITIAL_PRODUCTS);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while(1);
    
    struct ProductDetails *products = (struct ProductDetails*) calloc(numberOfProducts ,sizeof(struct ProductDetails));

    if(products == NULL){
        printf("Allocation failed!\n");
        return 1;
    }

    getInitialProducts(numberOfProducts,products);

    do{
        printf("\n========= INVENTORY MENU ========= \n");
        printf("1. Add New Product\n2. View All Products \n3. Update Quantity \n4. Search Product by ID \n5. Search Product by Name \n6. Search Product by Price Range \n7. Delete Product \n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                addProduct(&products,&numberOfProducts);
                break;
            case 2:
                listProducts(products,numberOfProducts);
                break;
            case 3:
                updateProductQuantity(products,numberOfProducts);
                break;
            case 4:
                searchProductById(products,numberOfProducts);
                break;
            case 5:
                searchProductByName(products,numberOfProducts);
                break;
            case 6:
                searchProductByPriceRange(products,numberOfProducts);
                break;
            case 7:
                deleteProductById(&products,&numberOfProducts);
                break;
            case 8:
                printf("\nExiting...");
                break;
            default:
                printf("Invalid choice!");
        };
    } while (choice != 8);

    free(products);
    printf("\nMemory released successfully.");

    return 0;
}