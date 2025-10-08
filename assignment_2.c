#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[100];
    int age;
};

void createUser();
void readUsers();
void updateUser();
void deleteUser();
int isUserExists(int id);

int main() {
    FILE *filePointer = fopen("users.txt", "a");
    if (!filePointer) {
        printf("Error creating file.\n");
        return 1;
    }
    fclose(filePointer);

    int choice;
    while (1) {
        printf("\n-----------------------\n");
        printf("1. Create a new user\n");
        printf("2. Display all users\n");
        printf("3. Update user by ID\n");
        printf("4. Delete user by ID\n");
        printf("5. Exit\n");
        printf("-----------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                createUser();
                break;
            }
            case 2: {
                readUsers();
                break;
            }
            case 3: {
                updateUser();
                break;
            }
            case 4: {
                deleteUser();
                break;
            }
            case 5: {
                exit(0);
            }
            default: {
                printf("Invalid choice. Try again.\n");
            }
        }
    }
    return 0;
}

int isUserExists(int id) {
    struct User user;
    FILE *userFilePointer = fopen("users.txt", "r");
    if (!userFilePointer) {
        return 0;
    }

    while (fscanf(userFilePointer, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            fclose(userFilePointer);
            return 1;
        }
    }
    fclose(userFilePointer);
    return 0;
}

void createUser() {
    struct User user;
    FILE *userFilePointer = fopen("users.txt", "a");
    if (!userFilePointer) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter User ID: ");
    scanf("%d", &user.id);
    getchar();

    if (isUserExists(user.id)) {
        printf("User with ID %d already exists.\n\n", user.id);
        fclose(userFilePointer);
        return;
    }

    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &user.age);

    fprintf(userFilePointer, "%d,%s,%d\n", user.id, user.name, user.age);
    fclose(userFilePointer);

    printf("User added successfully.\n\n");
}

void readUsers() {
    struct User user;
    FILE *userFilePointer = fopen("users.txt", "r");
    if (!userFilePointer) {
        perror("Error opening file");
        return;
    }

    printf("\n--- User Records ---\n");
    while (fscanf(userFilePointer, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        printf("ID: %d  |  Name: %s  |  Age: %d\n", user.id, user.name, user.age);
    }
    printf("--------------------\n\n");
    fclose(userFilePointer);
}

void updateUser() {
    struct User user;
    int targetId, isFound = 0;
    FILE *userFilePointer = fopen("users.txt", "r");
    FILE *tempFilePointer = fopen("temp.txt", "w");

    if (!userFilePointer || !tempFilePointer) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter ID of user to update: ");
    scanf("%d", &targetId);
    getchar();

    while (fscanf(userFilePointer, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == targetId) {
            isFound = 1;
            printf("Enter new Name: ");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';
            printf("Enter new Age: ");
            scanf("%d", &user.age);
        }
        fprintf(tempFilePointer, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(userFilePointer);
    fclose(tempFilePointer);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (isFound) {
        printf("User updated successfully.\n\n");
    } else {
        printf("User with ID %d not found.\n\n", targetId);
    }
}

void deleteUser() {
    struct User user;
    int targetId, isFound = 0;
    FILE *userFilePointer = fopen("users.txt", "r");
    FILE *tempFilePointer = fopen("temp.txt", "w");

    if (!userFilePointer || !tempFilePointer) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter ID of user to delete: ");
    scanf("%d", &targetId);

    while (fscanf(userFilePointer, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == targetId) {
            isFound = 1;
            continue;
        }
        fprintf(tempFilePointer, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(userFilePointer);
    fclose(tempFilePointer);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (isFound) {
        printf("User deleted successfully.\n\n");
    } else {
        printf("User with ID %d not found.\n\n", targetId);
    }
}
