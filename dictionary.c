/****************************************************************************
 * Wahaj Chaudhry
 * wchaudhry@college.harvard.edu 
 *
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include "dictionary.h"

// hash table size 
node* hash_table[MAX];

// djb2 hash function adopted from: http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(const char* word)
{
    unsigned long hash = MAGIC;
    int count_up;
    
    while ((count_up = *word++))
    {
        hash = ((hash << 5) + hash) + (count_up);
    }
    return hash % MAX;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{   
    // set-up temporary storage 
    char temp_store[LENGTH + 1] = {'\0'};
    
    // String copied to storage 
    strcpy(temp_store, word);
    
    // all letters set to lowercase 
    for (int i = 0; word[i] != '\0'; i++)
    {
        temp_store[i] = tolower(word[i]);
    }
    
    // caste returned hash value as int   
    int i_array = hash(temp_store);
    
    // pointer to hash table 
    node* storage = hash_table[i_array];
    
    // when table is being filled 
    while(storage != NULL)
    {   
        // check equivalency of word in string to stored value
        if (strcmp(temp_store, storage->word) == 0)
        {
            return true;
        }
        
        // move cursor to next node 
        storage = storage->next;
    }
    
    // if failed 
    return false;
}


/**m
 * Loads dictionary into memory.  Returns true if successful else false.
 */

// initialize for size function later 
int node_counter = 0;

bool load(const char* dictionary)
{
    // open file 
    FILE* text = fopen(dictionary, "r");
    
    // check if file is populated  
    if (text == NULL)
    {
        return false; 
    }
    
    // assign space for new word node 
    char* new_string = malloc(sizeof(char) * (LENGTH + 1));
    
    // loop through words until end-of-file
    while (fscanf(text, "%s", new_string) != EOF)
    {
        // alloc space to node 
        node* newnode = malloc(sizeof(node));
        
        // copy new string into array 
        strcpy(newnode->word, new_string);
        
        // grab hash value as int 
        int htoi = hash(new_string);
        
        // point to where table is pointing with hash function 
        newnode->next = hash_table[htoi];
        
        // set table value to new node 
        hash_table[htoi] = newnode;
        
        // word checked 
        node_counter++;
        
    }   
    
    // free memory
    free(new_string);
    
    // close file 
    fclose(text);
    
    // Success 
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // when loaded return words 
    if (node_counter > 0)
    {
        return node_counter;
    }
    
    // return 0 is not loaded 
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // counter for each item freed 
    unsigned int counter = 0;
    
    // iterates through table and frees 
    while(counter <= MAX)
    {
    
        // shifts index if bucket is empty 
        if (hash_table[counter] == NULL)
        {
            counter++;
        }
        
        // moves through nodes and frees
        else
        {
            while(hash_table[counter] != NULL)
            {
                node* cursor = hash_table[counter];
                hash_table[counter] = cursor->next;
                free(cursor);
            }
            // next index
            counter++;
        }
    }
    // Success 
    return true;
}
