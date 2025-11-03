#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_INITIAL_PRODUCTS 1
#define MAX_INITIAL_PRODUCTS 100
#define MIN_PRODUCT_ID 1
#define MAX_PRODUCT_ID 10000
#define MAX_NAME_LENGTH 50
#define MIN_PRICE 0.0
#define MAX_PRICE 100000.0
#define MIN_QUANTITY 0
#define MAX_QUANTITY 1000000

typedef struct Product
{
    int id;
    char name[MAX_NAME_LENGTH + 1];
    float price;
    int quantity;
} Product;

int getStringLength(const char *stringInput){
    int length = 0;
    while (stringInput[length] != '\0')
    {
        length++;
    }
    return length;
}

int isAlphaNumericCustom(char character){
    return ((character >= '0' && character <= '9') ||
            (character >= 'a' && character <= 'z') ||
            (character >= 'A' && character <= 'Z'));
}

int upperToLower(int character){
    if (character >= 'A' && character <= 'Z')
    {
        return character + ('a' - 'A');
    }
    return character;
}

bool isUniqueID(Product *products, int productCount, int id){
    for (int currentIndex = 0; currentIndex < productCount; currentIndex++)
    {
        if (products[currentIndex].id == id)
        {
            return false;
        }
    }
    return true;
}

bool containsSubstring(const char *originalString, const char *substring){
    int mainLength = getStringLength(originalString);
    int subLength = getStringLength(substring);

    if (subLength == 0 || subLength > mainLength)
    {
        return false;
    }

    for (int startIndex = 0; startIndex <= mainLength - subLength; startIndex++)
    {
        int subIndex;
        for (subIndex = 0; subIndex < subLength; subIndex++)
        {
            if (upperToLower(originalString[startIndex + subIndex]) != upperToLower(substring[subIndex]))
            {
                break;
            }
        }
        if (subIndex == subLength)
        {
            return true;
        }
    }
    return false;
}

void clearInputBuffer()
{
    int character;
    while ((character = getchar()) != '\n' && character != EOF)
        ;
}

void displayProduct(Product product){
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           product.id, product.name, product.price, product.quantity);
}

bool isValidPrice(float price){
    return (price >= MIN_PRICE && price <= MAX_PRICE);
}

bool isValidQuantity(int quantity){
    return (quantity >= MIN_QUANTITY && quantity <= MAX_QUANTITY);
}

bool isValidName(const char *name){
    if (name == NULL)
        return false;

    while (*name == ' ')
        name++;

    if (*name == '\0')
        return false;

    if (getStringLength(name) > MAX_NAME_LENGTH)
    {
        return false;
    }

    for (int index = 0; name[index] != '\0'; index++)
    {
        if (!isAlphaNumericCustom(name[index]) && name[index] != ' ' && name[index] != '-' && name[index] != '_')
        {
            return false;
        }
    }

    return true;
}

bool isValidProductID(int id, Product *products, int productCount)
{
    if (id < MIN_PRODUCT_ID || id > MAX_PRODUCT_ID)
    {
        printf("ID should be between %d and %d\n", MIN_PRODUCT_ID, MAX_PRODUCT_ID);
        return false;
    }
    if (!isUniqueID(products, productCount, id))
    {
        printf("Product ID already exists. Try again.\n");
        return false;
    }
    return true;
}

void getProductDetails(Product *product, Product *products, int productCount)
{
    int id;
    do
    {
        printf("Product ID: ");
        if (scanf("%d", &id) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        if (isValidProductID(id, products, productCount))
        {
            product->id = id;
            clearInputBuffer();
            break;
        }
        clearInputBuffer();
    } while (1);

    do
    {
        printf("Product Name: ");
        fgets(product->name, sizeof(product->name), stdin);
        product->name[strcspn(product->name, "\n")] = 0;

        if (!isValidName(product->name))
        {
            printf("Invalid name. Only alphanumeric characters, spaces, hyphens, and underscores are allowed.\n");
            continue;
        }
        break;
    } while (1);

    printf("Product Price: ");
    while (scanf("%f", &product->price) != 1 || !isValidPrice(product->price))
    {
        printf("Invalid input. Enter a valid price (%f-%f): ", MIN_PRICE, MAX_PRICE);
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Product Quantity: ");
    while (scanf("%d", &product->quantity) != 1 || !isValidQuantity(product->quantity))
    {
        printf("Invalid input. Enter a valid quantity (%d-%d): ", MIN_QUANTITY, MAX_QUANTITY);
        clearInputBuffer();
    }
    clearInputBuffer();
}

void addInitialProducts(int initialCount, Product *products, int *productCount)
{
    for (int currentIndex = 0; currentIndex < initialCount; currentIndex++)
    {
        printf("\nEnter details for product %d:\n", currentIndex + 1);
        getProductDetails(&products[currentIndex], products, currentIndex);
        (*productCount)++;
    }
}

void addNewProduct(int *totalProducts, Product **products, int *productCount)
{
    Product *temp = realloc(*products, (*totalProducts + 1) * sizeof(Product));
    if (temp == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(1);
    }
    *products = temp;

    printf("\nEnter new product details:\n");
    getProductDetails(&(*products)[*productCount], *products, *productCount);

    (*productCount)++;
    (*totalProducts)++;
    printf("Product added successfully\n");
}

void displayAllProducts(int productCount, Product *products)
{
    if (productCount == 0)
    {
        printf("No products available.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int index = 0; index < productCount; index++)
    {
        displayProduct(products[index]);
    }
}

void updateProductQuantity(int productCount, Product *products)
{
    int id;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int index = 0; index < productCount; index++)
    {
        if (products[index].id == id)
        {
            printf("Enter new Quantity: ");
            while (scanf("%d", &products[index].quantity) != 1 || !isValidQuantity(products[index].quantity))
            {
                printf("Invalid input. Enter a valid quantity (%d-%d): ", MIN_QUANTITY, MAX_QUANTITY);
                clearInputBuffer();
            }
            clearInputBuffer();
            printf("Quantity updated successfully\n");
            return;
        }
    }
    printf("No product found with ID %d\n", id);
}

Product *searchProductByID(int productCount, Product *products, int id)
{
    for (int index = 0; index < productCount; index++)
    {
        if (products[index].id == id)
        {
            return &products[index];
        }
    }
    return NULL;
}

void searchByID(int productCount, Product *products)
{
    int id;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();

    Product *foundProduct = searchProductByID(productCount, products, id);
    if (foundProduct != NULL)
    {
        printf("Product Found: ");
        displayProduct(*foundProduct);
    }
    else
    {
        printf("No product found with ID %d\n", id);
    }
}

void searchProductByName(int productCount, Product *products)
{
    char searchName[100];
    printf("Enter name to search : ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    bool found = false;
    printf("Products Found:\n");
    for (int index = 0; index < productCount; index++)
    {
        if (containsSubstring(products[index].name, searchName))
        {
            displayProduct(products[index]);
            found = true;
        }
    }

    if (!found)
    {
        printf("No product found matching \"%s\"\n", searchName);
    }
}

void searchProductByPriceRange(int productCount, Product *products)
{
    printf("Enter the price range between %.2f - %.2f: \n",MIN_PRICE,MAX_PRICE);
    float minPrice, maxPrice;
    printf("Enter price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    clearInputBuffer();

    if (minPrice < MIN_PRICE || maxPrice < MIN_PRICE || minPrice > maxPrice)
    {
        printf("Invalid price range.\n");
        return;
    }

    bool found = false;
    printf("Products in price range:\n");
    for (int index = 0; index < productCount; index++)
    {
        if (products[index].price >= minPrice && products[index].price <= maxPrice)
        {
            displayProduct(products[index]);
            found = true;
        }
    }

    if (!found)
    {
        printf("No products found in price range %.2f - %.2f\n", minPrice, maxPrice);
    }
}

void deleteProduct(int *totalProducts, Product **products, int *productCount)
{
    int id;
    printf("Enter Product ID to delete: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (int index = 0; index < *productCount; index++)
    {
        if ((*products)[index].id == id)
        {
            for (int shiftIndex = index; shiftIndex < *productCount - 1; shiftIndex++)
            {
                (*products)[shiftIndex] = (*products)[shiftIndex + 1];
            }
            (*productCount)--;

            if (*totalProducts > *productCount)
            {
                Product *temp = realloc(*products, (*productCount) * sizeof(Product));
                if (temp != NULL || *productCount == 0)
                {
                    *products = temp;
                    *totalProducts = *productCount;
                }
            }

            printf("Product deleted successfully\n");
            return;
        }
    }
    printf("No product found with ID %d\n", id);
}

int main()
{
    int totalProducts;
    int productCount = 0;

    do
    {
        printf("Enter initial number of products: ");
        if (scanf("%d", &totalProducts) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        if (totalProducts < MIN_INITIAL_PRODUCTS || totalProducts > MAX_INITIAL_PRODUCTS)
        {
            printf("Number must be between %d and %d.\n", MIN_INITIAL_PRODUCTS, MAX_INITIAL_PRODUCTS);
            clearInputBuffer();
            continue;
        }
        break;
    } while (1);
    clearInputBuffer();

    Product *products = (Product *)calloc(totalProducts, sizeof(Product));
    if (products == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    addInitialProducts(totalProducts, products, &productCount);

    int choice;
    do
    {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("==================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addNewProduct(&totalProducts, &products, &productCount);
            break;
        case 2:
            displayAllProducts(productCount, products);
            break;
        case 3:
            updateProductQuantity(productCount, products);
            break;
        case 4:
            searchByID(productCount, products);
            break;
        case 5:
            searchByName(productCount, products);
            break;
        case 6:
            searchByPriceRange(productCount, products);
            break;
        case 7:
            deleteProduct(&totalProducts, &products, &productCount);
            break;
        case 8:
            printf("Memory released successfully. Exiting program.\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    } while (choice != 8);

    free(products);
    return 0;
}
