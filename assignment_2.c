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

int main() {
    int ch;
    while (1) {
        printf("-----------------------\n");
        printf("1. Create a  new user  \n");
        printf("2. Display all users \n");
        printf("3. Update user by ID \n");
        printf("4. Delete user by ID \n");
        printf("5. Exit\n");
         printf("-----------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        getchar(); 
        switch (ch) {
            case 1: create_user(); break;
            case 2: read_users(); break;
            case 3: update_user(); break;
            case 4: delete_user(); break;
            case 5: exit(0);
            default: printf("Invalid choice Try again.\n");
        }
    }

    return 0;
}

void create_user() {
    struct User u;
    FILE *fp = fopen("users.txt", "a");
    if (fp == NULL) {
        printf("Error opening file");
        return;
    }

    printf("Enter User ID: ");
    scanf("%d", &u.id);
    getchar();
    printf("Enter Name: ");
    fgets(u.name, sizeof(u.name), stdin);
    u.name[strcspn(u.name, "\n")] = '\0'; 
    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(fp, "%d,%s,%d\n", u.id, u.name, u.age);
    fclose(fp);
    printf("User added successfully\n");
}


void read_users() {
    struct User u;
    FILE *fp = fopen("users.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    printf("\n--- User Records ---\n");
    while (fscanf(fp, "%d,%99[^,],%d\n", &u.id, u.name, &u.age) == 3) {
        printf("ID: %d  Name: %s  Age: %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
}


void update_user() {
    struct User u;
    int id, find = 0;
    FILE *fp = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        perror("Error opening file");
        return;
    }

    printf("Enter ID of user to update: ");
    scanf("%d", &id);
    getchar();

    while (fscanf(fp, "%d,%99[^,],%d\n", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            find = 1;
            printf("Enter new Name: ");
            fgets(u.name, sizeof(u.name), stdin);
            u.name[strcspn(u.name, "\n")] = '\0';
            printf("Enter new Age: ");
            scanf("%d", &u.age);
        }
        fprintf(temp, "%d,%s,%d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (find)
        printf("User updated successfully\n");
    else
        printf("User with ID %d not Found.\n", id);
}


void delete_user() {
    struct User u;
    int id, find = 0;
    FILE *fp = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        perror("Error opening file");
        return;
    }

    printf("Enter ID of user to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d,%99[^,],%d\n", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            find = 1;
            continue; 
        }
        fprintf(temp, "%d,%s,%d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (find)
        printf("User deleted successfully\n");
    else
        printf("User with ID %d not Found.\n", id);
}
 
