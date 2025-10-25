#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_PRODUCTS 1
#define MAX_PRODUCTS 100

typedef struct catalog
{
    int productID;
    char productName[100];
    float productPrice;
    int productQuantity;

} catalog;

int upperToLower(int character)
{
    if (character >= 'A' && character <= 'Z')
    {
        return character + ('a' - 'A');
    }
    return character;
}

bool isUniqueID(catalog *productDetails, int totalProducts, int id)
{
    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        if (productDetails[currentIndex].productID == id)
        {
            return false;
        }
    }
    return true;
}

bool compare(const char *originalString, const char *stringToBeCheck)
{
    int lenghtMain = 0;
    int index = 0;
    while (originalString[index] != '\0')
    {
        lenghtMain++;
        index++;
    }
    index = 0;
    int lenghtSubString = 0;
    while (stringToBeCheck[index] != '\0')
    {
        lenghtSubString++;
        index++;
    }

    if (lenghtSubString == 0 || lenghtSubString > lenghtMain)
        return false;

    for (int startIndex = 0; startIndex <= lenghtMain - lenghtSubString; startIndex++)
    {
        int matchCount = 0;
        for (int subIndex = 0; subIndex < lenghtSubString; subIndex++)
        {
            if (upperToLower(originalString[startIndex + subIndex]) == upperToLower(stringToBeCheck[subIndex]))
            {
                matchCount++;
            }
            else
            {
                break;
            }
        }
        if (matchCount == lenghtSubString)
            return true;
    }
    return false;
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void addInitialProducts(int totalProducts, catalog *productDetails, int *countProducts)
{
    for (int currentIndex = 0; currentIndex < totalProducts; currentIndex++)
    {
        int id;
        do
        {
            printf("Enter product ID: ");
            if (scanf("%d", &id) != 1)
            {
                printf("Invalid input. Please enter a number.\n");
                clearInputBuffer();
                continue;
            }
            if (id < MIN_PRODUCTS)
            {
                printf("ID should be greater than or equal to %d\n", MIN_PRODUCTS);
                continue;
            }
            if (!isUniqueID(productDetails, currentIndex, id))
            {
                printf("Product ID already exists.Try again.\n");
                clearInputBuffer();
                continue;
            }
            productDetails[currentIndex].productID = id;
            clearInputBuffer();
            break;
        } while (1);

        printf("Enter product name: ");
        fgets(productDetails[currentIndex].productName, sizeof(productDetails[currentIndex].productName), stdin);
        productDetails[currentIndex].productName[strcspn(productDetails[currentIndex].productName, "\n")] = 0;

        printf("Enter price: ");
        while (scanf("%f", &productDetails[currentIndex].productPrice) != 1)
        {
            printf("Invalid input. Enter a valid number: ");
            clearInputBuffer();
        }
        clearInputBuffer();

        printf("Enter quantity: ");
        while (scanf("%d", &productDetails[currentIndex].productQuantity) != 1)
        {
            printf("Invalid input. Enter an integer: ");
            clearInputBuffer();
        }
        clearInputBuffer();
        (*countProducts)++;
    }
}

void addNewProduct(int *totalProducts, catalog **productsDetailed, int *countProducts)
{
    catalog *temp = realloc(*productsDetailed, (*totalProducts + 1) * sizeof(catalog));

    if (temp == NULL)
    {
        printf("Memory reallocation failed!\n");
        exit(1);
    }
    *productsDetailed = temp;

    catalog *products = *productsDetailed;

    int id;
    do
    {
        printf("Enter unique product ID: ");
        if (scanf("%d", &id) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        if (!isUniqueID(products, *countProducts, id))
        {
            printf("Error: Product ID already exists. Try again.\n");
            clearInputBuffer();
            continue;
        }
        products[*countProducts].productID = id;
        clearInputBuffer();
        break;
    } while (1);

    printf("Enter product Name: ");
    fgets(products[*countProducts].productName, sizeof(products[*countProducts].productName), stdin);
    products[*countProducts].productName[strcspn(products[*countProducts].productName, "\n")] = 0;

    printf("Enter Price: ");
    while (scanf("%f", &products[*countProducts].productPrice) != 1)
    {
        printf("Invalid input. Enter a valid number: ");
        clearInputBuffer();
    }

    printf("Enter Quantity: ");
    while (scanf("%d", &products[*countProducts].productQuantity) != 1)
    {
        printf("Invalid input. Enter an integer: ");
        clearInputBuffer();
    }

    (*countProducts)++;
    (*totalProducts)++;
}

void displayProducts(int totalProducts, catalog *productDetails)
{
    if (totalProducts == 0)
    {
        printf("No products available.\n");
        return;
    }

    for (int starting = 0; starting < totalProducts; starting++)
    {
        printf("--------------------------------------\n");
        printf("ID: %d\n", productDetails[starting].productID);
        printf("Name: %s\n", productDetails[starting].productName);
        printf("Price: %.2f\n", productDetails[starting].productPrice);
        printf("Quantity: %d\n", productDetails[starting].productQuantity);
    }
}

void updateProducts(int totalProducts, catalog *productDetails)
{
    printf("Enter the ID for updating quantity of item: ");
    int temp;
    scanf("%d", &temp);
    clearInputBuffer();

    int found = 0;

    for (int starting = 0; starting < totalProducts; starting++)
    {
        if (productDetails[starting].productID == temp)
        {
            found = 1;
            printf("Enter new quantity: ");
            while (scanf("%d", &productDetails[starting].productQuantity) != 1)
            {
                printf("Invalid input. Enter an integer: ");
                clearInputBuffer();
            }
            break;
        }
    }

    if (!found)
    {
        printf("No product found with ID %d\n", temp);
    }
}

void searchByID(int totalProducts, catalog *productDetails)
{
    int tempID;
    printf("Enter the ID : ");
    scanf("%d", &tempID);
    int found = 0;
    for (int starting = 0; starting < totalProducts; starting++)
    {
        if (productDetails[starting].productID == tempID)
        {
            found = 1;
            printf("Product found!\n");
            printf("Name: %s\n", productDetails[starting].productName);
            printf("Price: %.2f\n", productDetails[starting].productPrice);
            printf("Quantity: %d\n", productDetails[starting].productQuantity);
            break;
        }
    }
    if (!found)
    {
        printf("No product found with ID %d\n", tempID);
    }
}

void searchByPriceRange(int totalProducts, catalog *productDetails)
{
    float min = 0, max = 0;
    printf("Enter the price range (min max): ");
    scanf("%f %f", &min, &max);
    clearInputBuffer();
    int found = 0;
    for (int starting = 0; starting < totalProducts; starting++)
    {
        if ((productDetails[starting].productPrice >= min) && (productDetails[starting].productPrice <= max))
        {
            printf("Product found: %s (ID: %d, Price: %.2f)\n",
                   productDetails[starting].productName,
                   productDetails[starting].productID,
                   productDetails[starting].productPrice),
                   productDetails[starting].productQuantity;
            found = 1;
        }
    }
    if (!found)
    {
        printf("No products found in price range %.2f - %.2f\n", min, max);
    }
}

void searchByName(int totalProducts, catalog *productDetails)
{
    char temp[100];
    printf("Enter name (or substring) to search: ");
    clearInputBuffer();
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;

    bool found = false;
    for (int starting = 0; starting < totalProducts; starting++)
    {
        if (compare(productDetails[starting].productName, temp))
        {
            printf("\nProduct found:\n");
            printf("Product found: %s (ID: %d, Price: %.2f, Quantity: %d)\n",
                   productDetails[starting].productName,
                   productDetails[starting].productID,
                   productDetails[starting].productPrice,
                   productDetails[starting].productQuantity);
            found = true;
        }
    }

    if (!found)
        printf("No product found matching \"%s\"\n", temp);
}

void deleteProduct(int *totalProducts, catalog *productsDetailed)
{
    int id;
    printf("Enter product ID to delete: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }

    for (int starting = 0; starting < *totalProducts; starting++)
    {
        if (productsDetailed[starting].productID == id)
        {
            for (int shiftIndex = starting; shiftIndex < *totalProducts - 1; shiftIndex++)
            {
                productsDetailed[shiftIndex] = productsDetailed[shiftIndex + 1];
            }
            (*totalProducts)--;
            printf("Product deleted successfully\n");
            return;
        }
    }
    printf("No product found with ID %d\n", id);
}

int main()
{
    int totalProducts;
    int countProducts = 0;
    do
    {
        printf("Enter the initial number of products.");
        scanf("%d", &totalProducts);
        clearInputBuffer();
        if (totalProducts < MIN_PRODUCTS || totalProducts > MAX_PRODUCTS)
        {
            printf("Error: Number of products is not valid.");
        }
    } while (totalProducts < MIN_PRODUCTS || totalProducts > MAX_PRODUCTS);
    catalog *productDetails = (catalog *)calloc(totalProducts, sizeof(catalog));
    if (productDetails == NULL)
    {
        printf("Memory is Full");
        exit(1);
    }
    addInitialProducts(totalProducts, productDetails, &countProducts);

    int userChoice;
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

        if (scanf("%d", &userChoice) != 1)
        {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }

        switch (userChoice)
        {
        case 1:
            addNewProduct(&totalProducts, &productDetails, &countProducts);
            break;
        case 2:
            displayProducts(totalProducts, productDetails);
            break;
        case 3:
            updateProducts(totalProducts, productDetails);
            break;
        case 4:
            searchByID(totalProducts, productDetails);
            break;
        case 5:
            searchByName(totalProducts, productDetails);
            break;
        case 6:
            searchByPriceRange(totalProducts, productDetails);
            break;
        case 7:
            deleteProduct(&totalProducts, productDetails);
            break;
        case 8:
            printf("END\n");
            break;
        default:
            printf("Invalid option.\n");
            break;
        }
    } while (userChoice != 8);
    free(productDetails);
}
