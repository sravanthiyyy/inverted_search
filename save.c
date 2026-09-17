#include "inverted.h"

int Save_database(hash_t *hash,char *f)
{
    // Validate backup filename
    if(!valid_filename(f))
    {
        return FAILURE;
    }

    // Open file in write mode to save database
    FILE *fptr=fopen(f,"w");
    if(!fptr)
    {
        printf("File is not opened\n");
        return FAILURE;
    }

    // Traverse each hash index
    for(int i=0;i<SIZE;i++)
    {
        // Skip empty hash indices
        if(hash[i].h_link==NULL)
        {
            continue;
        }

        main_t *maintemp = hash[i].h_link;
        // Traverse main nodes (words)
        while(maintemp)
        {
            // Write index, word and file count
            fprintf(fptr,"#%d;%s;%d",i,maintemp->word,maintemp->file_count);

            sub_t *subtemp=maintemp->ms_link;
            // Write file names and word counts
            while(subtemp)
            {
                fprintf(fptr,";%s;%d;",subtemp->file_name,subtemp->word_count);
                subtemp=subtemp->s_link;
            }

            // Mark end of one word entry
            fprintf(fptr,"#\n");
            maintemp=maintemp->m_link;
        }
    }

    // Check whether data is written to file
    fseek(fptr,0,SEEK_END);
    long size=ftell(fptr);
    rewind(fptr);
    fclose(fptr);

    if(size)
    {
        return SUCCESS;
    }
    return FAILURE;
}
