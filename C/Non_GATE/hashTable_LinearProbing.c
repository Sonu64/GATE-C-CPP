// Online C compiler to run C program online
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_SIZE_NAME  256
#define TABLE_SIZE 20
#define DELETED_NODE (person*)(uintptr_t)(0xFFFFFFFFFFFFFUL)

// Creating a person structure with name and age.
typedef struct {
    char name[MAX_SIZE_NAME];
    int age;
} person;

// Hash function to generate indices for positions in the array 
unsigned int hash(char *name) {
    int length = strnlen(name, MAX_SIZE_NAME);
    unsigned int hashValue = 0;
    for(int i=0; i<length; i++) {
        hashValue = hashValue + name[i];
        hashValue = hashValue * name[i];
        hashValue = hashValue % TABLE_SIZE; // To fit into table
    }
    return hashValue;
}

// Declaring hashTable -> Our actual Hash table containing Pointers to person structures. It is an array of pointers.
person *hashTable[TABLE_SIZE];

// Function to initialize has table and set every pointer to NULL
void initHashTable() {
    for(int i=0; i<TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// Function to show details of the hash table
void printTable() {
    printf("-------------------- START --------------------\n");
    for(int i=0; i<TABLE_SIZE; i++) {
        if (hashTable[i] == DELETED_NODE) {
            printf("\t%d\t---> <deleted_node>\n", i);
        }
        else if (hashTable[i] == NULL) {
            printf("\t%d\t---> NULL \n", i);
        }
        else {
            printf("\t%d\t---> %s, %d years old.\n", i, hashTable[i]->name, hashTable[i]->age);
        }
    }
    printf("-------------------- END --------------------");
}

// Function to Add Person into the Hash Table
bool addPerson(person* p) {
    if (p==NULL) return false;
    int index = hash(p->name);
    // Add person at index, if already Filled, increment by 1 position and repeat the steps
    for(int i=0; i<TABLE_SIZE; i++) {
        int indexToTry = (i + index) % TABLE_SIZE;
        if (hashTable[indexToTry] == NULL || hashTable[indexToTry] == DELETED_NODE) {
            hashTable[indexToTry] = p;
            return true;
        }
        return false;
    }
}


// Function to Reverse map the names to their indices, since the hash was generated using person->name
person *lookup(char *name) {
    int index = hash(name);
    for(int i=0; i<TABLE_SIZE; i++) {
        int indexToTry = (i + index) % TABLE_SIZE;
        if (hashTable[indexToTry] == NULL) // true NULL
            break;
        if (hashTable[indexToTry] == DELETED_NODE)
            continue;
        if (strncmp(hashTable[indexToTry]->name, name, MAX_SIZE_NAME) == 0) // Can only reach here if above 2 condition Fails. !
            return hashTable[indexToTry];
    } 
    return NULL;
}


// Function to Delete a person based on the name, since the hash was generated using person->name
person *delete(char *name) {
    int index = hash(name);
    for (int i=0; i<TABLE_SIZE; i++) {
        int indexToTry = i + index;

        // 1. If we hit NULL, the person isn't in the table.
        if (hashTable[indexToTry] == NULL)
            break;

        // 2. If it's a tombstone, keep looking (don't break the chain).
        if (hashTable[indexToTry] == DELETED_NODE)
            continue;

        // 3. If we find the name, "mark" it as deleted and return the pointer.
        if (strncmp(hashTable[indexToTry]->name, name, MAX_SIZE_NAME)==0) {
            person *temp = hashTable[indexToTry];
            hashTable[indexToTry] = DELETED_NODE;
            return temp;
        }
    }
    return NULL;
}



int main() {
    // Write C code here

    // Starting Clean, Initializing an Empty Hash Table
    initHashTable();

    // Printing Hash table details, Empty Hash Table till now...
    // printTable();

    person sonu = {.name = "Sonu", .age = 22};
    person shanu = {.name = "Shanu", .age = 22};
    person ma = {.name = "Paramita", .age = 45};
    person bunu = {.name = "Suparna", .age = 18};
    // New Test Cases (Collisions at Index 0)
    person t1 = {.name = "Bubu", .age = 10};    
    person t2 = {.name = "Abid", .age = 35};    
    person t6 = {.name = "Laltu", .age = 40};   

    // New Test Cases (Collisions at Index 16)
    person t3 = {.name = "Kaka", .age = 50};    
    person t4 = {.name = "Kiki", .age = 48};    

    // Random placement
    person t5 = {.name = "Rana", .age = 28};
    addPerson(&sonu);
    addPerson(&shanu);
    addPerson(&ma);
    addPerson(&bunu);
    addPerson(&t1);
    addPerson(&t2);
    addPerson(&t3);
    addPerson(&t4);
    addPerson(&t5);
    addPerson(&t6);

    printTable();


    printf("\n\n\n");

    printf("\n--- Testing Deletions ---\n");

    // 1. Delete someone from the middle of a collision chain
    // (Bubu, Abid, and Laltu all collided at Index 0)
    person *deleted1 = delete("Bubu");
    if (deleted1) printf("Deleted: %s\n", deleted1->name);

    // 2. Delete someone else
    person *deleted2 = delete("Kaka");
    if (deleted2) printf("Deleted: %s\n", deleted2->name);

    // 3. Try to delete someone who doesn't exist
    person *deleted3 = delete("Unknown");
    if (deleted3 == NULL) printf("Correctly handled non-existent person.\n");

    printf("\n--- Table After Deletions (Look for 'Tombstones') ---\n");
    printTable();

    // 4. CRITICAL TEST: Can we still find Abid? 
    // Even though Bubu was at Index 0 and is now DELETED, lookup must find Abid.
    person *found = lookup("Abid");
    if (found) {
        printf("\nSuccess! Found %s even after Bubu was deleted.\n", found->name);
    } else {
        printf("\nFailure! Could not find Abid (The chain is broken).\n");
    }
    
    return 0;
}