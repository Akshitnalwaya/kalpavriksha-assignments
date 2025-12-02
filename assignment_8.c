#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CAPACITY 1000
#define MAX_VALUE_LENGTH 100
#define MAX_INPUT_LENGTH 256
#define HASH_TABLE_SIZE 1009

typedef struct HashNode
{
    int key;
    struct Node *ref;
    struct HashNode *next;
} HashNode;

typedef struct Node
{
    int key;
    char value[MAX_VALUE_LENGTH];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct LRU
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    HashNode *table[HASH_TABLE_SIZE];
} LRU;

int hashIndex(int key)
{
    if (key < 0)
    {
        key = -key;
    }
    return key % HASH_TABLE_SIZE;
}

Node *createNode(int key, char *value)
{
    Node *temp = malloc(sizeof(Node));
    if (!temp)
    {
        return NULL;
    }
    temp->key = key;
    strncpy(temp->value, value, MAX_VALUE_LENGTH - 1);
    temp->value[MAX_VALUE_LENGTH - 1] = '\0';
    temp->prev = NULL;
    temp->next = NULL;
    return temp;
}

HashNode *createHashNode(int key, Node *ref)
{
    HashNode *h = malloc(sizeof(HashNode));
    if (!h)
    {
        return NULL;
    }
    h->key = key;
    h->ref = ref;
    h->next = NULL;
    return h;
}

void detach(LRU *cache, Node *node)
{
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else
    {
        cache->head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else
    {
        cache->tail = node->prev;
    }
}

void insertAtHead(LRU *cache, Node *node)
{
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head != NULL)
    {
        cache->head->prev = node;
    }

    cache->head = node;

    if (cache->tail == NULL)
    {
        cache->tail = node;
    }
}

void insertIntoHash(LRU *cache, int key, Node *ref)
{
    int idx = hashIndex(key);
    HashNode *h = createHashNode(key, ref);
    h->next = cache->table[idx];
    cache->table[idx] = h;
}

void removeFromHash(LRU *cache, int key)
{
    int idx = hashIndex(key);
    HashNode *curr = cache->table[idx];
    HashNode *prev = NULL;

    while (curr != NULL)
    {
        if (curr->key == key)
        {
            if (prev == NULL)
            {
                cache->table[idx] = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

Node *getFromHash(LRU *cache, int key)
{
    int idx = hashIndex(key);
    HashNode *curr = cache->table[idx];

    while (curr != NULL)
    {
        if (curr->key == key)
        {
            return curr->ref;
        }
        curr = curr->next;
    }

    return NULL;
}

void removeLRU(LRU *cache)
{
    if (cache->tail == NULL)
    {
        return;
    }

    Node *temp = cache->tail;
    int key = temp->key;

    detach(cache, temp);
    removeFromHash(cache, key);

    free(temp);
    cache->size--;
}

LRU *createCache(int capacity)
{
    if (capacity < 1 || capacity > MAX_CAPACITY)
    {
        return NULL;
    }

    LRU *cache = malloc(sizeof(LRU));
    if (!cache)
    {
        return NULL;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        cache->table[i] = NULL;
    }

    return cache;
}

void put(LRU *cache, int key, char *value)
{
    Node *node = getFromHash(cache, key);

    if (node != NULL)
    {
        strncpy(node->value, value, MAX_VALUE_LENGTH - 1);
        node->value[MAX_VALUE_LENGTH - 1] = '\0';
        detach(cache, node);
        insertAtHead(cache, node);
        return;
    }

    if (cache->size == cache->capacity)
    {
        removeLRU(cache);
    }

    Node *newNode = createNode(key, value);
    insertAtHead(cache, newNode);
    insertIntoHash(cache, key, newNode);

    cache->size++;
}

char *get(LRU *cache, int key)
{
    Node *node = getFromHash(cache, key);

    if (node == NULL)
    {
        return NULL;
    }

    detach(cache, node);
    insertAtHead(cache, node);

    return node->value;
}

void freeCache(LRU *cache)
{
    Node *curr = cache->head;

    while (curr != NULL)
    {
        Node *n = curr->next;
        free(curr);
        curr = n;
    }

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode *h = cache->table[i];
        while (h != NULL)
        {
            HashNode *n = h->next;
            free(h);
            h = n;
        }
    }

    free(cache);
}

int main()
{
    LRU *cache = NULL;
    char input[MAX_INPUT_LENGTH];

    while (1)
    {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *cmd = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, "\n");

        if (strcmp(cmd, "exit") == 0)
        {
            if (cache != NULL)
            {
                freeCache(cache);
            }
            exit(0);
        }
        else if (strcmp(cmd, "createCache") == 0)
        {
            if (arg1 == NULL)
            {
                continue;
            }
            int cap = atoi(arg1);

            if (cache != NULL)
            {
                freeCache(cache);
            }

            cache = createCache(cap);
        }
        else if (strcmp(cmd, "put") == 0)
        {
            if (cache == NULL)
            {
                continue;
            }
            if (arg1 == NULL || arg2 == NULL)
            {
                continue;
            }
            int key = atoi(arg1);
            put(cache, key, arg2);
        }
        else if (strcmp(cmd, "get") == 0)
        {
            if (cache == NULL)
            {
                continue;
            }
            if (arg1 == NULL)
            {
                continue;
            }
            int key = atoi(arg1);
            char *v = get(cache, key);
            if (v == NULL)
            {
                printf("NULL\n");
            }
            else
            {
                printf("%s\n", v);
            }
        }
    }

    return 0;
}
