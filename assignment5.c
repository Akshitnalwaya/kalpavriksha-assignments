#include <stdio.h>
#include <stdlib.h>
typedef struct catalog
{
    int productID;
    char productName[100];
    float price;
    int quantity;
} catalog;

void addNewProduct(int totalNumberOfproducts, catalog*productsDetailed, int *counterPtr)
{
    if (*counterPtr == totalNumberOfproducts)
    {
        productsDetailed = (catalog *)realloc(productsDetailed, 2 * sizeof(catalog));

        // productsDetailed=(catalog*)realloc(productsDetailed,sizeof(catalog)*2);
        // printf("Array is full\n");
    }
    for (int starting = 0; starting < totalNumberOfproducts; starting++)
    {
        printf("Enter the productID :");
        scanf("%d", &(productsDetailed + starting)->productID);
        getchar();
        // when i didnt used it then when i try to run the fgets then it directly take the enter in th einput buffer
        // scanf("%d", &(productsDetailed + starting).productID);

        // scanf("%d",productsDetailed->productID);
        /* Or which one is coeewct
                scanf("%d",productsDetailed+starting->productID);
                scanf("%d",productsDetailed(starting)->productID);
        */
        printf("Enter the product Name :");
        //    fgets("%s",&(productsDetailed+starting)->productName);
        fgets((productsDetailed + starting)->productName, sizeof((productsDetailed + starting)->productName), stdin);

        printf("Enter Price :");
        // scanf("%lf",&(productsDetailed + starting)->price);
        scanf("%f", &(productsDetailed + starting)->price);

        getchar();
        printf("Enter Quantity :");
        scanf("%d", &(productsDetailed + starting)->quantity);
        getchar();
        /*          EARLIER USING THIS
                    scanf("%f", (productsDetailed + starting)->price);
                    scanf("%d", (productsDetailed + starting)->quantity);
        */
        printf("\n");
        *counterPtr += 1;
    }
    return;
}

void viewAllProducts(int totalNumberOfproducts, catalog *productsDetailed)
{
    for (int starting = 0; starting < totalNumberOfproducts; starting++)
    {
        printf("--------------------------------------\n");
        // printf("Entered productID is :%d\n",(productsDetailed->productID));
        // scanf("%d",productsDetailed->productID);
        printf("ID: %d\n", productsDetailed[starting].productID);
        printf("Name: %s", (productsDetailed + starting)->productName);
        printf("Price: %f\n", productsDetailed[starting].price);
        printf("Quantity: %d\n", productsDetailed[starting].quantity);
    }
}

void updateQuantity(int totalNumberOfproducts, catalog *productsDetailed)
{
    printf("Enter the ID for updating quantity of item: ");
    int temp;
    scanf("%d", &temp);
    getchar();

    int found = 0;

    for (int i = 0; i < totalNumberOfproducts; i++)
    {
        if (productsDetailed[i].productID == temp)
        {
            printf("Enter the updated quantity of item: ");
            scanf("%d", &productsDetailed[i].quantity);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Doesn't found any ID for %d\n", temp);
    }
}

void searchByID(int totalNumberOfproducts, catalog *productsDetailed)
{
    int tempID;
    printf("Enter the ID : ");
    scanf("%d", &tempID);
    int found = 0;
    for (int i = 0; i < totalNumberOfproducts; i++)
    {
        if (productsDetailed[i].productID == tempID)
        {
            found = 1;
            printf("Product found \n");
            break;
        }
    }
    if (!found)
    {
        printf("Element did found");
    }
}

void searchByPrice(int totalNumberOfproducts, catalog *productsDetailed)
{
    int min = 0, max = 0;
    printf("Enter the range (min to max):");
    scanf("%d %d", &min, &max);
    int found = 0;
    for (int starting = 0; starting < totalNumberOfproducts; starting++)
    {
        if ((productsDetailed + starting)->price >= min && (productsDetailed + starting)->price <= max)
        {
            printf("Product found and id is %d\n", productsDetailed[starting].productID);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("Not found in the price range %d %d \n", min, max);
    }
}

int main()
{
    int totalNumberOfproducts;
    int userChoice;
    do
    {
        printf("Enter the total number of products : ");
        scanf("%d", &totalNumberOfproducts);
        if (totalNumberOfproducts > 10 || totalNumberOfproducts < 0)
        {
            printf("Error invalid input \n");
            continue;
        }
        getchar();
        // -->
    } while (totalNumberOfproducts > 10 || totalNumberOfproducts <= 0);

    catalog *productsDetailed = (catalog *)calloc(totalNumberOfproducts, sizeof(catalog));
    int counter = 0;
    int *counterPtr = &counter;
    do
    {

        printf("========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("\nPlease enter the choice : ");
        scanf("%d", &userChoice);
        getchar();
        // scanf("%d",userChoice);
        switch (userChoice)
        {
        case 1:
            printf("totalNumberOfproducts\n");
            addNewProduct(totalNumberOfproducts, productsDetailed, counterPtr);
            break;
        case 2:
            printf("Products detailed ares: \n");
            viewAllProducts(totalNumberOfproducts, productsDetailed);
            break;
        case 3:
            printf("Update Quantity\n");
            updateQuantity(totalNumberOfproducts, productsDetailed);
            break;
        case 4:
            printf("Search by id\n");
            searchByID(totalNumberOfproducts, productsDetailed);
            break;

        case 5:
            printf("Search by name\n");
            break;

        case 6:
            printf("search by price\n");
            searchByPrice(totalNumberOfproducts, productsDetailed);
            break;

        case 7:
            printf("delete\n");
            break;

        case 8:
            printf("exit\n");
            break;
        default:
            printf("Invalid input");
            break;
        }
    } while (userChoice != 8);
    printf("After the do while");
}

/*
Q1 kya ham dynamic wale me bina poinyers ke nhi kar sakte kya??
*/




// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct catalog
// {
//     int productID;
//     char productName[100];
//     float price;
//     int quantity;
// } catalog;

// /* ✅ CHANGED:
//    - Function now takes `catalog **productsDetailed` and `int *capacity`
//      so realloc() can modify the pointer and capacity safely.
// */
// void addNewProduct(int *capacity, catalog **productsDetailed, int *counterPtr)
// {
//     // ✅ Check if array is full
//     if (*counterPtr >= *capacity)
//     {
//         printf("Array is full. Increasing capacity...\n");

//         *capacity *= 2; // ✅ Double the capacity

//         // ✅ Correct use of realloc()
//         catalog *temp = realloc(*productsDetailed, (*capacity) * sizeof(catalog));
//         if (temp == NULL)
//         {
//             printf("Memory reallocation failed!\n");
//             exit(1);
//         }

//         *productsDetailed = temp; // ✅ Update original pointer
//         printf("New capacity = %d\n", *capacity);
//     }

//     // ✅ Simplify access
//     catalog *p = *productsDetailed;

//     printf("Enter the productID: ");
//     scanf("%d", &p[*counterPtr].productID);
//     getchar();

//     printf("Enter the product Name: ");
//     fgets(p[*counterPtr].productName, sizeof(p[*counterPtr].productName), stdin);
//     p[*counterPtr].productName[strcspn(p[*counterPtr].productName, "\n")] = 0; // remove newline

//     printf("Enter Price: ");
//     scanf("%f", &p[*counterPtr].price);

//     printf("Enter Quantity: ");
//     scanf("%d", &p[*counterPtr].quantity);

//     (*counterPtr)++; // ✅ Increase count
//     printf("Product added successfully Total products: %d\n\n", *counterPtr);
// }

// void viewAllProducts(int totalNumberOfproducts, catalog *productsDetailed)
// {
//     if (totalNumberOfproducts == 0)
//     {
//         printf("No products to show.\n");
//         return;
//     }

//     for (int i = 0; i < totalNumberOfproducts; i++)
//     {
//         printf("--------------------------------------\n");
//         printf("ID: %d\n", productsDetailed[i].productID);
//         printf("Name: %s\n", productsDetailed[i].productName);
//         printf("Price: %.2f\n", productsDetailed[i].price);
//         printf("Quantity: %d\n", productsDetailed[i].quantity);
//     }
// }

// void updateQuantity(int totalNumberOfproducts, catalog *productsDetailed)
// {
//     printf("Enter the ID for updating quantity of item: ");
//     int temp;
//     scanf("%d", &temp);
//     getchar();

//     int found = 0;

//     for (int i = 0; i < totalNumberOfproducts; i++)
//     {
//         if (productsDetailed[i].productID == temp)
//         {
//             printf("Enter the updated quantity of item: ");
//             scanf("%d", &productsDetailed[i].quantity);
//             found = 1;
//             break;
//         }
//     }

//     if (!found)
//     {
//         printf("No product found with ID %d\n", temp);
//     }
// }

// void searchByID(int totalNumberOfproducts, catalog *productsDetailed)
// {
//     int tempID;
//     printf("Enter the ID : ");
//     scanf("%d", &tempID);
//     int found = 0;
//     for (int i = 0; i < totalNumberOfproducts; i++)
//     {
//         if (productsDetailed[i].productID == tempID)
//         {
//             found = 1;
//             printf("Product found!\n");
//             printf("Name: %s\n", productsDetailed[i].productName);
//             printf("Price: %.2f\n", productsDetailed[i].price);
//             printf("Quantity: %d\n", productsDetailed[i].quantity);
//             break;
//         }
//     }
//     if (!found)
//     {
//         printf("No product found with ID %d\n", tempID);
//     }
// }

// void searchByPrice(int totalNumberOfproducts, catalog *productsDetailed)
// {
//     float min = 0, max = 0;
//     printf("Enter the price range (min max): ");
//     scanf("%f %f", &min, &max);
//     int found = 0;
//     for (int i = 0; i < totalNumberOfproducts; i++)
//     {
//         if (productsDetailed[i].price >= min && productsDetailed[i].price <= max)
//         {
//             printf("Product in range: %s (ID: %d)\n", productsDetailed[i].productName, productsDetailed[i].productID);
//             found = 1;
//         }
//     }
//     if (!found)
//     {
//         printf("No products found between %.2f and %.2f\n", min, max);
//     }
// }

// int main()
// {
//     int capacity;
//     int userChoice;

//     do
//     {
//         printf("Enter the initial number of products (max 10): ");
//         scanf("%d", &capacity);
//         if (capacity > 10 || capacity <= 0)
//         {
//             printf("Error: invalid input.\n");
//             continue;
//         }
//         getchar();
//     } while (capacity > 10 || capacity <= 0);

//     /* ✅ CHANGED:
//        - Dynamic memory allocation for initial capacity.
//        - Must check allocation success.
//     */
//     catalog *productsDetailed = calloc(capacity, sizeof(catalog));
//     if (productsDetailed == NULL)
//     {
//         printf("Memory allocation failed!\n");
//         return 1;
//     }

//     int counter = 0; // ✅ Tracks how many products currently stored

//     do
//     {
//         printf("\n========= INVENTORY MENU =========\n");
//         printf("1. Add New Product\n");
//         printf("2. View All Products\n");
//         printf("3. Update Quantity\n");
//         printf("4. Search Product by ID\n");
//         printf("5. Search Product by Price Range\n");
//         printf("6. Exit\n");
//         printf("==================================\n");
//         printf("Please enter your choice: ");
//         scanf("%d", &userChoice);
//         getchar();

//         switch (userChoice)
//         {
//         case 1:
//             addNewProduct(&capacity, &productsDetailed, &counter); // ✅ Changed parameters
//             break;
//         case 2:
//             viewAllProducts(counter, productsDetailed); // ✅ Uses counter, not capacity
//             break;
//         case 3:
//             updateQuantity(counter, productsDetailed);
//             break;
//         case 4:
//             searchByID(counter, productsDetailed);
//             break;
//         case 5:
//             searchByPrice(counter, productsDetailed);
//             break;
//         case 6:
//             printf("Exiting program...\n");
//             break;
//         default:
//             printf("Invalid choice. Please try again.\n");
//             break;
//         }

//     } while (userChoice != 6);

//     printf("After the do while\n");

//     /* ✅ Free the allocated memory before exiting */
//     free(productsDetailed);

//     return 0;
// }
