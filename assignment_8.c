#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CAPACITY 1000
#define MAX_VALUE_LENGTH 100
#define HASH_TABLE_SIZE 1009

typedef struct Node
{
    int key;
    char value[MAX_VALUE_LENGTH];
    struct Node *prev;
    struct Node *next;
    struct Node *hashNext;
} Node;

typedef struct LRU
{
    int capacity;
    int size;

    Node *head;
    Node *tail;

    Node *table[HASH_TABLE_SIZE];
} LRU;

int hashIndex(int key)
{
    if (key < 0)
        key = -key;
    return key % HASH_TABLE_SIZE;
}

Node *createNode(int key, char *value)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->key = key;
    strcpy(temp->value, value);
    temp->prev = NULL;
    temp->next = NULL;
    temp->hashNext = NULL;
    return temp;
}

void detach(LRU *cache, Node *node)
{
    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        cache->head = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
}

void insertAtHead(LRU *cache, Node *node)
{
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head != NULL)
        cache->head->prev = node;

    cache->head = node;

    if (cache->tail == NULL)
        cache->tail = node;
}

void removeFromHash(LRU *cache, int key)
{
    int index = hashIndex(key);
    Node *curr = cache->table[index];
    Node *prev = NULL;

    while (curr != NULL)
    {
        if (curr->key == key)
        {
            if (prev == NULL)
                cache->table[index] = curr->hashNext;
            else
                prev->hashNext = curr->hashNext;
            return;
        }
        prev = curr;
        curr = curr->hashNext;
    }
}

Node *getFromHash(LRU *cache, int key)
{
    int index = hashIndex(key);
    Node *curr = cache->table[index];

    while (curr != NULL)
    {
        if (curr->key == key)
            return curr;
        curr = curr->hashNext;
    }
    return NULL;
}

void insertIntoHash(LRU *cache, Node *node)
{
    int index = hashIndex(node->key);
    node->hashNext = cache->table[index];
    cache->table[index] = node;
}

void removeLRU(LRU *cache)
{
    if (cache->tail == NULL)
        return;

    Node *temp = cache->tail;

    detach(cache, temp);
    removeFromHash(cache, temp->key);

    free(temp);
    cache->size--;
}

LRU *createCache(int capacity)
{
    if (capacity < 1 || capacity > MAX_CAPACITY)
        return NULL;

    LRU *cache = (LRU *)malloc(sizeof(LRU));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        cache->table[i] = NULL;

    return cache;
}

void put(LRU *cache, int key, char *value)
{
    Node *node = getFromHash(cache, key);

    if (node != NULL)
    {
        strcpy(node->value, value);
        detach(cache, node);
        insertAtHead(cache, node);
        return;
    }

    if (cache->size == cache->capacity)
        removeLRU(cache);

    Node *newNode = createNode(key, value);

    insertAtHead(cache, newNode);
    insertIntoHash(cache, newNode);

    cache->size++;
}

char *get(LRU *cache, int key)
{
    Node *node = getFromHash(cache, key);

    if (node == NULL)
        return NULL;

    detach(cache, node);
    insertAtHead(cache, node);
    return node->value;
}

void freeCache(LRU *cache)
{
    Node *curr = cache->head;

    while (curr != NULL)
    {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(cache);
}

int main()
{
    LRU *cache = NULL;

    printf("Input \n");

    char input[200];

    while (1)
    {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *command = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, "\n");

        if (strcmp(command, "exit") == 0)
        {
            printf("Memory released. Exiting program...\n");
            if (cache != NULL)
                freeCache(cache);
            exit(0);
        }

        else if (strcmp(command, "createCache") == 0)
        {
            if (arg1 == NULL)
            {
                printf("Enter valid capacity.\n");
                continue;
            }

            int capacity = atoi(arg1);

            if (cache != NULL)
            {
                freeCache(cache);
            }

            cache = createCache(capacity);

            if (cache == NULL)
                printf("Invalid cache size.\n");
        }

        else if (strcmp(command, "put") == 0)
        {
            if (cache == NULL)
            {
                printf("Cache not initialized.\n");
                continue;
            }

            if (arg1 == NULL || arg2 == NULL)
            {
                printf("put key value\n");
                continue;
            }

            int key = atoi(arg1);
            put(cache, key, arg2);
        }

        else if (strcmp(command, "get") == 0)
        {
            if (cache == NULL)
            {
                printf("Cache not initialized.\n");
                continue;
            }

            if (arg1 == NULL)
            {
                printf("get key\n");
                continue;
            }

            int key = atoi(arg1);
            char *value = get(cache, key);

            if (value == NULL)
                printf("NULL\n");
            else
                printf("%s\n", value);
        }

        else
        {
            printf("Enter a valid command.\n");
        }
    }

    return 0;
}