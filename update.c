#include "inverted.h"

int update_database(hash_t *hash, Node **head, char *filename)
{
    // Open backup file in read mode
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("[ERROR] Failed to open backup file: %s\n", filename);
        return FAILURE;
    }

    printf("\n============================================================\n");
    printf("                 UPDATING DATABASE FROM BACKUP               \n");
    printf("============================================================\n");
    printf("[INFO] Reading data from backup file: %s\n", filename);

    char line[256];

    // Read backup file line by line
    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Validate line format (#...#)
        if (line[0] != '#' || line[strlen(line) - 1] != '#')
        {
            printf("[ERROR] Invalid format in backup file. Each line must start and end with '#'.\n");
            fclose(fp);
            return FAILURE;
        }

        // Remove ending '#'
        line[strlen(line) - 1] = '\0';
        char *data = line + 1;   // Skip starting '#'

        // Extract hash index
        char *token = strtok(data, ";");
        if (!token)
        {
            continue;
        }
        int index = atoi(token);

        // Extract word
        char *word = strtok(NULL, ";");
        if (!word)
        {
            continue;
        }

        // Extract file count
        char *file_count_str = strtok(NULL, ";");
        if (!file_count_str)
        {
            continue;
        }
        int file_count = atoi(file_count_str);

        // Process file entries for this word
        for (int i = 0; i < file_count; i++)
        {
            char *file_name = strtok(NULL, ";");
            char *count_str = strtok(NULL, ";");
            if (!file_name || !count_str)
            {
                break;
            }

            int count = atoi(count_str);

            // Insert word details into database
            insert_to_database(hash, index, word, file_name, count);

            // Remove file from pending file list
            remove_file(head, file_name);
        }
    }

    fclose(fp);

    return SUCCESS;
}

