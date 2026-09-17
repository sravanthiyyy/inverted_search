#include "inverted.h"

int main(int argc, char *argv[])
{
    Node *head = NULL;   // Head pointer for input file linked list

    printf("\n============================================================\n");
    printf("               WELCOME TO INVERTED SEARCH ENGINE             \n");
    printf("============================================================\n");

    // Check for minimum command-line arguments
    if (argc < 2)
    {
       printf("Enter the required no.of arguments\n");
       printf("Usage : ./a.out filename1 filename2 ....\n");
       return FAILURE;
    }

    // Validate input files and populate file list
    if (read_and_validate(argc, argv, &head) == 0)
    {
        printf("\n[ERROR] Input file validation failed!\n");
        printf("        One or more input files are invalid or unreadable.\n");
        printf("        Please verify filenames and access permissions.\n");
        return FAILURE;
    }

    printf("\n[INFO] Input files validated successfully.\n");
    printf("------------------------------------------------------------\n");
    printf(" Files to be processed:\n");

    Node *temp = head;
    int i = 1;
    // Display validated input files
    while (temp != NULL)
    {
        printf("   [%02d] %s\n", i, temp->file_name);
        i++;
        temp = temp->link;
    }

    printf("------------------------------------------------------------\n");

    hash_t hash[SIZE];   // Hash table initialization
    for (int i = 0; i < SIZE; i++)
    {
        hash[i].index = i;
        hash[i].h_link = NULL;
    }

    char buffer[100];   // Buffer for user input
    int option;         // Menu choice variable

    int db_initialized= 0;   // (Unused) database state flag
    int files_processed = 0; // (Unused) file processing flag

    while (1)
    {
        // Main menu display
        printf("\n----------------------- MAIN MENU ---------------------------\n");
        printf("   [1] Create Database\n");
        printf("   [2] Display Database\n");
        printf("   [3] Search Database\n");
        printf("   [4] Update Database\n");
        printf("   [5] Save Database\n");
        printf("   [6] Exit Program\n");
        printf("------------------------------------------------------------\n");
        printf("Enter your choice [1-6]: ");

        // Validate numeric menu input
        if (scanf("%d", &option) != 1)
        {
            printf("\n[ERROR] Invalid input. Please enter a numeric value (1–6).\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option)
        {
            case 1:
                // Create database only if files are available
                if(!head)
                {
                    printf("No new files available for database creation\n");
                    break;
                }
                if(create_database(hash,&head) == SUCCESS)
                {
                    printf("Database created successfully");
                }
                else
                {
                    printf("Database creation failed");
                }
                break;

            case 2:
                // Display database if it is not empty
                if (is_hash_empty(hash) == FAILURE)
                {
                    if (display_database(hash) == SUCCESS)
                        printf("[SUCCESS] Database displayed successfully.\n");
                    else
                        printf("[FAILURE] Error displaying database.\n");
                }
                else
                    printf("[INFO] Database is empty. Nothing to display.\n");
                break;

            case 3:
                // Search operation allowed only if database exists
                if (is_hash_empty(hash) == FAILURE)
                {
                    printf("Enter the word to search: ");
                    scanf("%99s", buffer);
                    if (search_database(hash, buffer) == SUCCESS)
                        printf("[SUCCESS] Word '%s' found in the database.\n", buffer);
                    else
                        printf("[INFO] Word '%s' not found in the database.\n", buffer);
                }
                else
                    printf("[INFO] Database is empty. Please create it first.\n");
                break;

            case 4:
                // Update database only when database is empty
                if(is_hash_empty(hash) == SUCCESS)
                {
                    printf("Enter the backup filename to update from: ");
                    scanf("%99s", buffer);
                    if (update_database(hash, &head, buffer) == SUCCESS)
                    {
                        printf("Database updated successfully.\n");
                    }
                    else
                        printf("Database update failed.\n");
                }
                else
                    printf("Database already exists. Update not possible.\n");
                break;

            case 5:
                // Save database only if data exists
                if (is_hash_empty(hash) == FAILURE)
                {
                    printf("Enter the filename to save the database: ");
                    scanf("%99s", buffer);
                    if (Save_database(hash, buffer) == SUCCESS)
                        printf("[SUCCESS] Database saved successfully.\n");
                    else
                        printf("[FAILURE] Error saving database.\n");
                }
                else
                    printf("[INFO] Database is empty. Nothing to save.\n");
                break;

            case 6:
                // Exit program safely
                printf("\n[EXIT] Exiting program... Goodbye!\n");
                printf("============================================================\n");
                return 0;

            default:
                printf("[ERROR] Invalid choice. Please select option 1–6.\n");
        }
    }

    return 0;
}

