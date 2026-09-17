#include "inverted.h"

/* Validate filename to ensure it ends with .txt */
int valid_filename(char *s)
{
    char *pos = strstr(s, ".txt");   // Find ".txt" extension
    if (!pos || pos == s || *(pos + 4) != '\0')
        return FAILURE;
    return SUCCESS;
}

/* Check whether a file is empty */
int is_file_empty(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);   // Move file pointer to end
    long size = ftell(fptr);    // Get file size
    rewind(fptr);               // Reset file pointer
    return (size == 0);
}

/* Check if filename already exists in the list */
int is_duplicate(Node *head, char *filename)
{
    Node *temp = head;
    while (temp)
    {
        if(strcmp(temp->file_name, filename) == 0)
            return SUCCESS;     // Duplicate found
        temp = temp->link;
    }
    return FAILURE;
}

/* Insert a new filename into linked list */
void insert_file(Node **head, char *filename)
{
    Node *new = malloc(sizeof(Node));   // Allocate node
    strcpy(new->file_name, filename);
    new->link = NULL;

    if (*head == NULL)
    {
        *head = new;    // First node insertion
    }
    else
    {
        Node *temp = *head;
        while (temp->link != NULL)
            temp = temp->link;
        temp->link = new;   // Insert at end
    }
}

/* Read command-line files and validate them */
int read_and_validate(int argc, char *argv[], Node **head)
{
    printf("\n========== VALIDATING INPUT FILES ==========\n");

    for (int i = 1; i < argc; i++)
    {
        // Validate file extension
        if (!valid_filename(argv[i]))
        {
            printf(" Invalid file name: '%s' (must end with .txt)\n", argv[i]);
            continue;
        }

        // Check for duplicate filenames
        if (is_duplicate(*head, argv[i]))
        {
            printf(" Duplicate file skipped: '%s'\n", argv[i]);
            continue;
        }

        // Try opening the file
        FILE *fptr = fopen(argv[i], "r");
        if (!fptr)
        {
            printf(" Unable to open file: '%s'\n", argv[i]);
            continue;
        }

        // Check if file is empty
        if (is_file_empty(fptr))
        {
            printf(" File is empty: '%s'\n", argv[i]);
            fclose(fptr);
            continue;
        }

        fclose(fptr);
        insert_file(head, argv[i]);   // Add valid file to list
        printf("[SUCCESS] File added: '%s'\n", argv[i]);
    }

    // No valid files found
    if (*head == NULL)
    {
        printf("\nNo valid input files found.\n");
        return FAILURE;
    }

    printf("\nAll valid files have been processed successfully.\n");
    return SUCCESS;
}

/* Check whether hash table is empty */
int is_hash_empty(hash_t *hash)
{
    int flag = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (hash[i].h_link != NULL)
            flag = 1;   // At least one index is occupied
    }

    if (flag)
        return FAILURE;
    else
        return SUCCESS;
}

/* Insert word data into inverted index */
void insert_to_database(hash_t *hash, int index, char *word, char *file_name, int word_count)
{
    main_t *mainTemp = hash[index].h_link;
    main_t *prev = NULL;

    // Search for word in main list
    while (mainTemp && strcmp(mainTemp->word, word) != 0)
    {
        prev = mainTemp;
        mainTemp = mainTemp->m_link;
    }

    // Word not found, create new main node
    if (mainTemp == NULL)
    {
        main_t *new_main = malloc(sizeof(main_t));
        if (!new_main)
        {
            printf("[ERROR] Memory allocation failed for main node.\n");
            return;
        }

        strcpy(new_main->word, word);
        new_main->file_count = 1;
        new_main->m_link = NULL;
        new_main->ms_link = NULL;

        // Create sub node for file
        sub_t *new_sub = malloc(sizeof(sub_t));
        if (!new_sub)
        {
            printf("[ERROR] Memory allocation failed for sub node.\n");
            free(new_main);
            return;
        }

        strcpy(new_sub->file_name, file_name);
        new_sub->word_count = word_count;
        new_sub->s_link = NULL;
        new_main->ms_link = new_sub;

        // Link main node into hash table
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
        while (subTemp && strcmp(subTemp->file_name, file_name) != 0)
        {
            subPrev = subTemp;
            subTemp = subTemp->s_link;
        }

        // File not found, create new sub node
        if (subTemp == NULL)
        {
            sub_t *new_sub = malloc(sizeof(sub_t));
            if (!new_sub)
            {
                printf("[ERROR] Memory allocation failed for sub node.\n");
                return;
            }

            strcpy(new_sub->file_name, file_name);
            new_sub->word_count = word_count;
            new_sub->s_link = NULL;
            subPrev->s_link = new_sub;
            mainTemp->file_count++;   // Increment file count
        }
        else
        {
            subTemp->word_count++;   // Increment word count
        }
    }
}

/* Remove processed file from file list */
int remove_file(Node **head, char *filename)
{
    if (*head == NULL)
    {
        return FAILURE;   // List is empty
    }

    Node *temp = *head;
    Node *prev = NULL;

    while (temp)
    {
        if (strcmp(temp->file_name, filename) == 0)
        {
            if (temp == *head)
            {
                *head = temp->link;   // Remove head node
                free(temp);
                return SUCCESS;
            }
            else
            {
                prev->link = temp->link;  // Remove middle/end node
                free(temp);
                return SUCCESS;
            }
        }
        prev = temp;
        temp = temp->link;
    }
    return SUCCESS;
}

