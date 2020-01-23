// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27


// Represents a node in a trie
typedef struct TrieNode {
    bool is_word;
    struct TrieNode *children[N];
}
        TrieNode;


// Represents a trie
typedef struct Trie {
    struct TrieNode *root;
    unsigned int size;
}
        Trie;


Trie *trie = NULL;


// Returns [0, 25] for alphabetic characters and 26 for an apostrophe
unsigned int hash(const char c) {
    if (c == '\'')
        return N-1;
    else
        return tolower(c) - 'a';
}

void init_node(TrieNode *node);
void free_node(TrieNode *node);

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary) {
    // Initialize trie
    trie = malloc(sizeof(Trie));
    if (trie == NULL)
        return false;

    trie->size = 0;

    trie->root = malloc(sizeof(TrieNode));
    if (trie->root == NULL)
        return false;

    init_node(trie->root);

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF) {
        TrieNode *node = trie->root;

        for (int i = 0, len = strlen(word); i < len; i++) {

            unsigned int h = hash(word[i]);

            if (node->children[h] == NULL) {

                node->children[h] = malloc(sizeof(TrieNode));
                if (node->children[h] == NULL)
                    return false;

                init_node(node->children[h]);
            }

            node = node->children[h];
        }

        node->is_word = true;
        trie->size++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void) {
    if (trie != NULL)
        return trie->size;
    else
        return 0;
}


// Returns true if word is in dictionary else false
bool check(const char *word) {
    if (trie != NULL && word[0] != '\0') {
        TrieNode *node = trie->root;
        for (int i = 0, len = strlen(word); i < len; i++) {
            node = node->children[hash(word[i])];

            if (node == NULL)
                return false;
        }
        return node->is_word;
    }
    return false;
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void) {
    if (trie != NULL) {
        free_node(trie->root);
        free(trie);
    }
    return true;
}


void init_node(TrieNode *node) {
    node->is_word = false;
    for (int i = 0; i < N; i++) {
        node->children[i] = NULL;
    }
}


void free_node(TrieNode *node) {
    for (int i = 0; i < N; i++)
        if (node->children[i] != NULL)
            free_node(node->children[i]);
    free(node);
}
