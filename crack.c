#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
// Get this function working first!
char * tryWord(char * plaintext, char * hashFilename)
{
    // Hash the plaintext
    char *hash = md5(plaintext, strlen(plaintext));
    if (hash == NULL) {
        return NULL;
    }

    // Open the hash file
    FILE *hashFile = fopen(hashFilename, "r");
    if (hashFile == NULL) {
        fprintf(stderr, "Error opening hash file: %s\n", hashFilename);
        free(hash);
        return NULL;
    }

    // Loop through the hash file, one line at a time.
    char buffer[HASH_LEN];
    char *result = NULL;

    while (fgets(buffer, HASH_LEN, hashFile) != NULL) {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = 0;

        // Attempt to match the hash from the file to the
        // hash of the plaintext.
        if (strcmp(buffer, hash) == 0){
            // If there is a match, you'll return the hash.
            result = strdup(hash);
            break;
        }
    }

    // If not, return NULL: this is handled by the default value of the return statement

    // Before returning, do any needed cleanup:
    fclose(hashFile);
    free(hash);

    // Modify this line so it returns the hash
    // that was found, or NULL if not found.
    return result;
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // These two lines exist for testing. When you have
    // tryWord working, it should display the hash for "hello",
    // which is 5d41402abc4b2a76b9719d911017c592.
    // Then you can remove these two lines and complete the rest
    // of the main function below.
    // char *found = tryWord("hello", "hashes00.txt");
    // printf("%s %s\n", found, "hello");


    // Open the dictionary file for reading.
    FILE *dictFile = fopen(argv[2], "r");
    if (dictFile == NULL) {
        fprintf(stderr, "Error opening dictionary file: %s\n", argv[2]);
        exit(1);
    }    

    // For each dictionary word, pass it to tryWord, which
    // will attempt to match it against the hashes in the hash_file.
    char word[PASS_LEN];
    int crackedCount = 0;

    while (fgets(word, PASS_LEN, dictFile) != NULL) {
        // Remove newline character if present
        word[strcspn(word, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(word) == 0) {
            continue;
        }

        // Try this word against the hash file
        char *matchedHash = tryWord(word, argv[1]);
    
        // If we got a match, display the hash and the word. For example:
        //   5d41402abc4b2a76b9719d911017c592 hello

        if (matchedHash != NULL) {
            printf("%s %s\n", matchedHash, word);
            crackedCount++;
            free(matchedHash);
        }
    }
    
    // Close the dictionary file.
    fclose(dictFile);

    // Display the number of hashes that were cracked.
    printf("%d hashes cracked!\n", crackedCount);
    
    // Free up any malloc'd memory?
    // free(matchedHash): changed it so all the memory is free'd inside of the loop itself

    return 0;
}

