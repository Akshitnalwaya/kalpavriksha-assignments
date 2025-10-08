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
    FILE *fp = fopen("users.txt", "a");
    if (!fp) {
        printf("Error creating file.\n");
        return 1;
    }
    fclose(fp);

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
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

int isUserExists(int id) {
    struct User user;
    FILE *file = fopen("users.txt", "r");
    if (!file) return 0;

    while (fscanf(file, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void createUser() {
    struct User user;
    FILE *file = fopen("users.txt", "a");
    if (!file) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter User ID: ");
    scanf("%d", &user.id);
    getchar();

    if (isUserExists(user.id)) {
        printf("User with ID %d already exists.\n\n", user.id);
        fclose(file);
        return;
    }

    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &user.age);

    fprintf(file, "%d,%s,%d\n", user.id, user.name, user.age);
    fclose(file);

    printf("User added successfully.\n\n");
}

void readUsers() {
    struct User user;
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("\n--- User Records ---\n");
    while (fscanf(file, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        printf("ID: %d  |  Name: %s  |  Age: %d\n", user.id, user.name, user.age);
    }
    printf("--------------------\n\n");
    fclose(file);
}

void updateUser() {
    struct User user;
    int id, found = 0;
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter ID of user to update: ");
    scanf("%d", &id);
    getchar();

    while (fscanf(file, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            printf("Enter new Name: ");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';
            printf("Enter new Age: ");
            scanf("%d", &user.age);
        }
        fprintf(temp, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User updated successfully.\n\n");
    else
        printf("User with ID %d not found.\n\n", id);
}

void deleteUser() {
    struct User user;
    int id, found = 0;
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter ID of user to delete: ");
    scanf("%d", &id);

    while (fscanf(file, "%d,%99[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User deleted successfully.\n\n");
    else
        printf("User with ID %d not found.\n\n", id);
}
