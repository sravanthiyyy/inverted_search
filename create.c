#include "inverted.h"

int create_database(hash_t *hash, Node **head)
{
    // If no files are available, database creation is not possible
    if (*head == NULL)
    {
        printf("[ERROR] No files available for processing.\n");
        return FAILURE;
    }

    Node *temp = *head;
    Node *prev_temp = NULL;
    FILE *fptr;
    char word[100];   // Buffer to store each word from file

    while (temp != NULL)
    {
        // Open the current file in read mode
        fptr = fopen(temp->file_name, "r");
        if (!fptr)
        {
            printf("[ERROR] Could not open file: %s\n", temp->file_name);
            temp = temp->link;
            continue;
        }

        printf("[INFO] Reading data from file: %s\n", temp->file_name);

        // Read words one by one from file
        while (fscanf(fptr, "%99s", word) != EOF)
        {
            int index;
            char ch = word[0];   // First character used for hashing

            // Calculate hash index based on first character
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
                index = (ch & ~32) - 'A';
            else if (ch >= '0' && ch <= '9')
                index = 26;
            else
                index = 27;

            main_t *mainTemp = hash[index].h_link;
            main_t *prev = NULL;

            // Search for the word in main list
            while (mainTemp && strcmp(mainTemp->word, word) != 0)
            {
                prev = mainTemp;
                mainTemp = mainTemp->m_link;
            }

            // If word is not present, create a new main node
            if (mainTemp == NULL)
            {
                main_t *new_main = malloc(sizeof(main_t));
                if (!new_main)
                {
                    printf("[ERROR] Memory allocation failed for main node.\n");
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_main->word, word);
                new_main->file_count = 1;
                new_main->m_link = NULL;
                new_main->ms_link = NULL;

                // Create sub node for the current file
                sub_t *new_sub = malloc(sizeof(sub_t));
                if (!new_sub)
                {
                    printf("[ERROR] Memory allocation failed for sub node.\n");
                    free(new_main);
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_sub->file_name, temp->file_name);
                new_sub->word_count = 1;
                new_sub->s_link = NULL;

                new_main->ms_link = new_sub;

                // Insert main node into hash table
                if (hash[index].h_link == NULL)
                    hash[index].h_link = new_main;
                else
                    prev->m_link = new_main;
            }
            else
            {
                sub_t *subTemp = mainTemp->ms_link;
                sub_t *subPrev = NULL;

                // Search for file in sub list
                while (subTemp && strcmp(subTemp->file_name, temp->file_name) != 0)
                {
                    subPrev = subTemp;
                    subTemp = subTemp->s_link;
                }

                // If file is not present, create a new sub node
                if (subTemp == NULL)
                {
                    sub_t *new_sub = malloc(sizeof(sub_t));
                    if (!new_sub)
                    {
                        printf("[ERROR] Memory allocation failed for sub node.\n");
                        fclose(fptr);
                        return FAILURE;
                    }

                    strcpy(new_sub->file_name, temp->file_name);
                    new_sub->word_count = 1;
                    new_sub->s_link = NULL;
                    subPrev->s_link = new_sub;
                    mainTemp->file_count++;
                } 
                else
                {
                    // If word already exists in same file, increment count
                    subTemp->word_count++;
                }
            }
        }

        fclose(fptr);

        // Remove processed file from file list
        prev_temp = temp;
        temp = temp->link;
        remove_file(head, prev_temp->file_name);
    }

    printf("\n[INFO] All files processed successfully. Database creation complete.\n");
    return SUCCESS;
}

