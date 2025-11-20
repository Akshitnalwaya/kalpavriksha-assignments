#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLOCK_SIZE 512
#define MAX_BLOCK 1024

typedef struct FreeNode
{
    int index;
    struct FreeNode *prev;
    struct FreeNode *next;
} FreeNode;

typedef struct FileNode
{
    char name[50];
    int type;
    struct FileNode *parent;
    struct FileNode *child;
    struct FileNode *nextSibling;
    int blockPointerCount;
    int blockPointer[512];
} FileNode;

typedef struct VFS {
    int freeBlockCount;
    FreeNode *freeListHead;
    FreeNode *freeListTail;
    char disk[MAX_BLOCK][BLOCK_SIZE];
} VFS;

FreeNode *createFreeNode(int index);
FreeNode *createFreeNodeList(VFS *vfs);
FileNode *createFileNode(int type, FileNode *parent, char *name);
void addChild(FileNode *parent, FileNode *newNode);
void lsCommand(FileNode *tail);
void mkdirCommand(VFS *vfs, FileNode *PWD, char *name);
FileNode *isPresent(FileNode *tail, char *name);
void cdCommand(FileNode **PWD, char *name);
void rmdirCommand(VFS *vfs, FileNode *PWD, char *name);
void createCommand(VFS *vfs, FileNode *PWD, char *name);
void readCommand(VFS *vfs, FileNode *PWD, char *name);
void pwdCommand(FileNode *PWD, FileNode *root);
void dfCommand(VFS *vfs);
void deleteCommand(VFS *vfs, FileNode *PWD, char *name);
int allocateMemory(VFS *vfs);
void deallocateMemory(VFS *vfs, int index);
void writeCommand(VFS *vfs, FileNode *PWD, char *name, char *fileContent);
void freeAllMemory(FileNode *root);
void freeDoublyLinkedList(VFS *vfs);

int main()
{
    VFS vfs;
    vfs.freeBlockCount = MAX_BLOCK;
    vfs.freeListHead = NULL;
    vfs.freeListTail = NULL;

    vfs.freeListHead = createFreeNodeList(&vfs);

    FileNode *root = createFileNode(0, NULL, "/");
    FileNode *PWD = root;

    printf("$ ./vfs\n/ >Compact VFS - ready. Type 'exit' to quit.\n");

    char terminalInput[60];
    while (1)
    {
        printf("%s>", PWD->name);
        scanf("%[^\n]", terminalInput);
        getchar();

        char *command = strtok(terminalInput, " ");
        char *name = strtok(NULL, " ");

        if (strcmp(command, "exit") == 0)
        {
            printf("Memory released. Exiting program...\n");
            freeAllMemory(root);
            freeDoublyLinkedList(&vfs);
            free(root);
            exit(0);
        }
        else if (strcmp(command, "mkdir") == 0)
        {
            if (name == NULL)
            {
                printf("Please Enter a valid directory name.\n");
            }
            else{
                mkdirCommand(&vfs, PWD, name);
            }
        }
        else if (strcmp(command, "ls") == 0)
        {
            lsCommand(PWD->child);
        }
        else if (strcmp(command, "cd") == 0)
        {
            if (strcmp(name, "..") == 0)
            {
                if (PWD == root)
                {
                    printf("No parent Directory , you are in root.\n");
                }
                else{
                    PWD = PWD->parent;
                }
            }
            else
            {
                cdCommand(&PWD, name);
            }
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            rmdirCommand(&vfs, PWD, name);
        }
        else if (strcmp(command, "create") == 0)
        {
            if (name == NULL){
                printf("Please enter a valid file name.\n");
            }
            else{
                createCommand(&vfs, PWD, name);
            }
        }
        else if (strcmp(command, "pwd") == 0)
        {
            pwdCommand(PWD, root);
            printf("\n");
        }
        else if (strcmp(command, "write") == 0)
        {
            char *fileContent = strtok(NULL, "\n");
            writeCommand(&vfs, PWD, name, fileContent);
        }
        else if (strcmp(command, "read") == 0)
        {
            readCommand(&vfs, PWD, name);
        }
        else if (strcmp(command, "df") == 0)
        {
            dfCommand(&vfs);
        }
        else if (strcmp(command, "delete") == 0)
        {
            deleteCommand(&vfs, PWD, name);
        }
        else
        {
            printf("Enter a valid command.\n");
        }
    }
}

FreeNode *createFreeNode(int index)
{
    FreeNode *temp = (FreeNode *)malloc(sizeof(FreeNode));
    temp->index = index;
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}

FreeNode *createFreeNodeList(VFS *vfs)
{
    FreeNode *headFN = NULL;
    FreeNode *prev = NULL;

    for (int index = 0; index < MAX_BLOCK; index++)
    {
        FreeNode *temp = createFreeNode(index);
        if (headFN == NULL)
        {
            headFN = temp;
            prev = temp;
        }
        else
        {
            prev->next = temp;
            temp->prev = prev;
            prev = prev->next;
        }
    }
    vfs->freeListTail = prev;
    return headFN;
}

FileNode *createFileNode(int type, FileNode *parent, char *name)
{
    FileNode *temp = (FileNode *)malloc(sizeof(FileNode));
    strcpy(temp->name, name);
    temp->type = type;
    temp->parent = parent;
    temp->child = NULL;
    temp->nextSibling = NULL;
    temp->blockPointerCount = 0;
    return temp;
}

void addChild(FileNode *parent, FileNode *newNode)
{
    if (parent->child == NULL)
    {
        parent->child = newNode;
        newNode->nextSibling = newNode;
    }
    else
    {
        newNode->nextSibling = parent->child->nextSibling;
        parent->child->nextSibling = newNode;
    }
}

void lsCommand(FileNode *tail)
{
    if (tail == NULL)
    {
        printf("(empty)\n");
        return;
    }
    FileNode *temp = tail;
    do
    {
        printf("%s\t", temp->name);
        temp = temp->nextSibling;
    } while (temp != tail);
    printf("\n");
}

void mkdirCommand(VFS *vfs, FileNode *PWD, char *name)
{
    if (isPresent(PWD->child, name) == NULL)
    {
        FileNode *newNode = createFileNode(0, PWD, name);
        addChild(PWD, newNode);
        printf("Directory %s created successfully\n", name);
    }
    else
    {
        printf("Directory with %s already exists.\n", name);
    }
}

FileNode *isPresent(FileNode *tail, char *name)
{
    if (tail == NULL)
        return NULL;

    FileNode *temp = tail;
    do
    {
        if (strcmp(temp->name, name) == 0){
            return temp;
        }
        temp = temp->nextSibling;
    } while (temp != tail);
    return NULL;
}

void cdCommand(FileNode **PWD, char *name)
{
    if ((*PWD)->child == NULL)
    {
        printf("NO %s directory exists", name);
        return;
    }

    FileNode *temp = (*PWD)->child;
    do
    {
        if (strcmp(temp->name, name) == 0)
        {
            if (temp->type == 1)
            {
                printf("%s is not a directory.\n", name);
                return;
            }
            *PWD = temp;
            printf("Moved to /%s\n", name);
            return;
        }
        temp = temp->nextSibling;
    } while (temp != *PWD);
    return;
}

FileNode *deleteFileNode(FileNode *tail, FileNode *target)
{
    FileNode *temp = tail;
    do
    {
        if (temp->nextSibling == target)
        {
            if (temp->nextSibling == temp)
            {
                free(temp);
                return NULL;
            }
            else
            {
                FileNode *toDelete = temp->nextSibling;
                temp->nextSibling = temp->nextSibling->nextSibling;
                free(toDelete);
                break;
            }
        }
        temp = temp->nextSibling;
    } while (temp != tail);
    return temp;
}

void rmdirCommand(VFS *vfs, FileNode *PWD, char *name)
{
    if (PWD->child == NULL)
    {
        printf("No %s directory exists\n", name);
        return;
    }
    FileNode *toDelete = isPresent(PWD->child, name);
    if (toDelete == NULL || toDelete->type == 1)
    {
        printf("No %s directory exists\n", name);
        return;
    }

    if (toDelete->child != NULL)
    {
        printf("Directory is not empty.\n");
        return;
    }
    PWD->child = deleteFileNode(PWD->child, toDelete);
    printf("Directory %s deleted successfully\n", name);
}

void createCommand(VFS *vfs, FileNode *PWD, char *name)
{
    if (isPresent(PWD->child, name) == NULL)
    {
        FileNode *newNode = createFileNode(1, PWD, name);
        addChild(PWD, newNode);
        printf("File %s created successfully\n", name);
    }
    else
    {
        printf("File with %s already exists.\n", name);
    }
}

void pwdCommand(FileNode *PWD, FileNode *root)
{
    if (PWD == root){
        printf("/");
        return;
    }

    pwdCommand(PWD->parent, root);
    printf("%s/", PWD->name);
}

void writeCommand(VFS *vfs, FileNode *PWD, char *name, char *fileContent)
{
    FileNode *file = isPresent(PWD->child, name);
    if (file == NULL || file->type == 0)
    {
        printf("No such File exists.\n");
    }
    else
    {
        int sizeOfContent = strlen(fileContent);
        int blocksNeeded = ceil((double)sizeOfContent / BLOCK_SIZE);

        if (blocksNeeded > vfs->freeBlockCount)
        {
            printf("Insufficient Memory.\n");
            return;
        }
        file->blockPointerCount = blocksNeeded;

        for (int block = 0; block < blocksNeeded; block++)
        {
            file->blockPointer[block] = allocateMemory(&vfs[0]);
        }

        for (int block = 0; block < blocksNeeded; block++)
        {
            for (int i = 0; fileContent[i] != '\0' && i < BLOCK_SIZE; i++)
            {
                vfs->disk[file->blockPointer[block]][i] = fileContent[i];
            }
        }

        printf("Data written successfully (size = %d bytes)\n", sizeOfContent);
    }
}

void readCommand(VFS *vfs, FileNode *PWD, char *name)
{
    FileNode *file = isPresent(PWD->child, name);
    if (file == NULL || file->type == 0)
    {
        printf("No such File exists.\n");
    }
    else
    {
        for (int block = 0; block < file->blockPointerCount; block++)
        {
            for (int i = 0; i < BLOCK_SIZE && vfs->disk[file->blockPointer[block]][i] != '\0'; i++)
            {
                printf("%c", vfs->disk[file->blockPointer[block]][i]);
            }
        }
        printf("\n");
    }
}

void dfCommand(VFS *vfs)
{
    printf("Total Blocks : %d\n", MAX_BLOCK);
    printf("Used Blocks : %d\n", MAX_BLOCK - vfs->freeBlockCount);
    printf("Free Block : %d\n", vfs->freeBlockCount);
    printf("Disk Usage : %.2lf%%\n", ((double)(MAX_BLOCK - vfs->freeBlockCount) / MAX_BLOCK) * 100);
}

int allocateMemory(VFS *vfs)
{
    int index = vfs->freeListHead->index;
    FreeNode* toDelete = vfs->freeListHead;
    vfs->freeListHead = vfs->freeListHead->next;

    if (vfs->freeListHead != NULL)
        vfs->freeListHead->prev = NULL;

    free(toDelete);
    vfs->freeBlockCount--;

    return index;
}

void deleteCommand(VFS *vfs, FileNode *PWD, char *name)
{
    FileNode *file = isPresent(PWD->child, name);
    if (file == NULL || file->type == 0)
    {
        printf("No such File exists.\n");
    }
    else
    {
        for (int block = 0; block < file->blockPointerCount; block++)
        {
            deallocateMemory(vfs, file->blockPointer[block]);
        }

        PWD->child = deleteFileNode(PWD->child, file);
        printf("File %s deleted successfully\n", name);
    }
}

void deallocateMemory(VFS *vfs, int index)
{
    FreeNode *toADD = createFreeNode(index);
    vfs->freeListTail->next = toADD;
    toADD->prev = vfs->freeListTail;
    vfs->freeListTail = vfs->freeListTail->next;
    vfs->freeBlockCount++;
}

void freeAllMemory(FileNode *root)
{
    if (root == NULL || root->child == NULL)
        return;

    FileNode *temp = root->child;
    do
    {
        if (temp->child != NULL)
        {
            freeAllMemory(temp);
        }
        FileNode *toDelete = temp;
        temp = temp->nextSibling;
        free(toDelete);
    } while (temp != root->child);
}

void freeDoublyLinkedList(VFS *vfs)
{
    while (vfs->freeListHead != NULL)
    {
        FreeNode *toDelete = vfs->freeListHead;
        vfs->freeListHead = vfs->freeListHead->next;
        if (vfs->freeListHead != NULL){
            vfs->freeListHead->prev = NULL;
        }
        free(toDelete);
    }
}
