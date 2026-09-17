#include "inverted.h"

int display_database(hash_t *hash)
{
    printf("\nIndex  Word                 FileCnt    File Name       WordCnt\n");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < SIZE; i++)
    {
        main_t *mtemp = hash[i].h_link;

        // Traverse ALL main nodes at this index
        while (mtemp)
        {
            sub_t *stemp = mtemp->ms_link;
            int first = 1;

            // Traverse sub nodes
            while (stemp)
            {
                if (first)
                {
                    printf("%-6d %-20s %-10d %-15s %-10d\n",i,mtemp->word, mtemp->file_count,stemp->file_name,stemp->word_count);
                    first = 0;
                }
                else
                {
                    printf("%-6s %-20s %-10s %-15s %-10d\n", "", "", "",stemp->file_name, stemp->word_count);
                }

                stemp = stemp->s_link;
            }

            mtemp = mtemp->m_link;
        }
    }

    return SUCCESS;
}

