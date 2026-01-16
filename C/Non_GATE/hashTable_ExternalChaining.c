#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE_NAME  256
#define TABLE_SIZE 20

typedef struct {
    char name[MAX_SIZE_NAME];
    int age;
} person;

// Forward declaration to avoid pointer type warnings
typedef struct NODE NODE;

struct NODE {
    person* p;
    NODE* next; 
}; 

// Array of Linked List Heads
NODE *hashTable[TABLE_SIZE];

unsigned int hash(char *name) {
    int length = strnlen(name, MAX_SIZE_NAME);
    unsigned int hashValue = 0;
    for(int i=0; i<length; i++) {
        hashValue = (hashValue + name[i]) * name[i] % TABLE_SIZE;
    }
    return hashValue;
}

void initHashTable() {
    for(int i=0; i<TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

void destroyTable() {
    for(int i=0; i<TABLE_SIZE; i++) {
        NODE *current = hashTable[i];
        while(current != NULL) {
            NODE *temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
    printf("\n[System] Hash Table memory cleared successfully.\n");
}

void printTable() {
    printf("\n==================== HASH TABLE LAYOUT ====================\n");
    printf("Index | Chain (Linked List)\n");
    printf("-----------------------------------------------------------\n");
    for(int i=0; i<TABLE_SIZE; i++) {
        printf(" [%2d] | ", i);
        if (hashTable[i] == NULL) {
            printf("<empty>");
        } else {
            NODE *temp = hashTable[i];
            while (temp != NULL) {
                printf("(%s, %d) ---> ", temp->p->name, temp->p->age);
                temp = temp->next;
            }
            printf("NULL");
        }
        printf("\n");
    }
    printf("===========================================================\n");
}

bool insertToHashTable(person* p) {
    if (p == NULL) return false;
    int index = hash(p->name);
    
    NODE* newNode = malloc(sizeof(NODE));
    if (!newNode) return false;

    newNode->p = p;
    newNode->next = hashTable[index]; // Insert at Head (O(1))
    hashTable[index] = newNode;
    return true;
}

person *lookup(char *name) {
    int index = hash(name);
    NODE* temp = hashTable[index];
    while(temp != NULL) {
        if(strncmp(temp->p->name, name, MAX_SIZE_NAME) == 0)
            return temp->p;
        temp = temp->next;
    }
    return NULL;
}

person *delete(char *name) {
    int index = hash(name);
    NODE *temp = hashTable[index];
    NODE *prev = NULL;

    while(temp != NULL) {
        if (strncmp(temp->p->name, name, MAX_SIZE_NAME) == 0) {
            if (prev == NULL) {
                hashTable[index] = temp->next;
            } else {
                prev->next = temp->next;
            }
            person *savedPerson = temp->p;
            free(temp); 
            return savedPerson;
        }
        prev = temp;
        temp = temp->next;
    }
    return NULL;
}

int main() {
    initHashTable();

    // Data Setup
    person people[] = {
        {"Sonu", 22}, {"Shanu", 22}, {"Paramita", 45}, {"Suparna", 18},
        {"Bubu", 10}, {"Abid", 35}, {"Laltu", 40}, {"Kaka", 50},
        {"Kiki", 48}, {"Rana", 28}
    };

    int numPeople = sizeof(people) / sizeof(people[0]);
    for(int i=0; i < numPeople; i++) {
        insertToHashTable(&people[i]);
    }

    printTable();

    printf("\n--- Action: Deleting 'Bubu' and 'Kaka' ---\n");
    delete("Bubu");
    delete("Kaka");

    printTable();

    printf("\n--- Action: Looking up 'Abid' ---\n");
    person *found = lookup("Abid");
    if (found) {
        printf("RESULT: Found %s, Age: %d\n", found->name, found->age);
    } else {
        printf("RESULT: Person not found.\n");
    }

    destroyTable();
    return 0;
}