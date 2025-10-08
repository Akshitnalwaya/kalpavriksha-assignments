#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[100];
    int age;
};

void create_user();
void read_users();
void update_user();
void delete_user();

int user_exists(int id) {
    struct User existingUser;
    FILE *filePointer = fopen("users.txt", "r");

    if (filePointer == NULL) {
        return 0;
    }

    while (fscanf(filePointer, "%d,%99[^,],%d\n", &existingUser.id, existingUser.name, &existingUser.age) == 3) {
        if (existingUser.id == id) {
            fclose(filePointer);
            return 1;
        }
    }

    fclose(filePointer);
    return 0;
}

int main() {
    int choice;

    while (1) {
        printf("-----------------------\n");
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
                create_user();
                break;
            }
            case 2: {
                read_users();
                break;
            }
            case 3: {
                update_user();
                break;
            }
            case 4: {
                delete_user();
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

void create_user() {
    struct User newUser;
    FILE *filePointer = fopen("users.txt", "a");

    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter User ID: ");
    scanf("%d", &newUser.id);
    getchar();

    if (user_exists(newUser.id)) {
        printf("User with ID %d already exists.\n", newUser.id);
        fclose(filePointer);
        return;
    }

    printf("Enter Name: ");
    fgets(newUser.name, sizeof(newUser.name), stdin);
    newUser.name[strcspn(newUser.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &newUser.age);

    fprintf(filePointer, "%d,%s,%d\n", newUser.id, newUser.name, newUser.age);
    fclose(filePointer);

    printf("User added successfully.\n");
}

void read_users() {
    struct User user;
    FILE *filePointer = fopen("users.txt", "r");

    if (filePointer == NULL) {
        perror("Error opening file");
        return;
    }

    printf("\n--- User Records ---\n");
    while (fscanf(filePointer, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        printf("ID: %d  Name: %s  Age: %d\n", user.id, user.name, user.age);
    }

    fclose(filePointer);
}

void update_user() {
    struct User user;
    int id, found = 0;
    FILE *filePointer = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (filePointer == NULL || tempFile == NULL) {
        perror("Error opening file");
        return;
    }

    printf("Enter ID of user to update: ");
    scanf("%d", &id);
    getchar();

    while (fscanf(filePointer, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            printf("Enter new Name: ");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';
            printf("Enter new Age: ");
            scanf("%d", &user.age);
        }
        fprintf(tempFile, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(filePointer);
    fclose(tempFile);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found) {
        printf("User updated successfully.\n");
    } else {
        printf("User with ID %d not found.\n", id);
    }
}

void delete_user() {
    struct User user;
    int id, found = 0;
    FILE *filePointer = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (filePointer == NULL || tempFile == NULL) {
        perror("Error opening file");
        return;
    }

    printf("Enter ID of user to delete: ");
    scanf("%d", &id);

    while (fscanf(filePointer, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            continue;
        }
        fprintf(tempFile, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(filePointer);
    fclose(tempFile);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found) {
        printf("User deleted successfully.\n");
    } else {
        printf("User with ID %d not found.\n", id);
    }
}
