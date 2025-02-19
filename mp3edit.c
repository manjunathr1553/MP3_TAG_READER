/* 
Documentaion
Name : Manjunath R
Date : 05/02/2025
Discription : Edit Operation (-e)
The editor provides the ability to modify various metadata tags in an MP3 file.
Users can edit tags such as title (-t), artist (-a), album (-A), year (-y), content 
type (-c), and comment (-C).The program ensures that the necessary file and edit options are provided before proceeding. If not, an error is displayed, and the operation is stopped.*/
#include<string.h>
#include<unistd.h>
#include <stdio.h>
#include "type.h"
#include "mp3.h"

// Function to validate and open the MP3 file
Status validation_open_file(char *argv[], mp3_head *mp3)
{
    if (strstr(argv[3], ".mp3") != NULL)  // Check if the file has a ".mp3" extension
    {
        if (strcmp(strstr(argv[3], ".mp3"), ".mp3") == 0)  // Ensure the file extension is exactly ".mp3"
            mp3->file_name = argv[3];  // Assign the file name
        mp3->file_ptr = fopen(mp3->file_name, "r");  // Open the file in read mode
        if (mp3->file_ptr == NULL)  // If file cannot be opened
        {
            printf("\n>>>>>>>>>>>>>>>> ❌ERROR : Invalid file❌ <<<<<<<<<<<<<<<<\n");
            return e_failure;
        }
        char ch[5];
        fread(ch, 3, 1, mp3->file_ptr);  // Read the file signature (first 3 bytes)
        short s;
        fread(&s, 2, 1, mp3->file_ptr);  // Read version number (2 bytes)
        if (strcmp(ch, "ID3") != 0 && (s != 3 || s != 4))  // Check for valid ID3 tag
        {
            printf("\n>>>>>>>>>>>>>>>> ❌ERROR : Invalid file❌ <<<<<<<<<<<<<<<<\n");
            return e_failure;
        }
        strcpy(mp3->signeture, ch);  // Store the signature
        mp3->ver = s;  // Store the version
    }
    else
    {
        printf("\n>>>>>>>>>>>>>>>>❌ Error : Invalid file Extension ❌<<<<<<<<<<<<<<<<<<<<\n");
        return e_failure;
    }
    return e_success;
}

// Function to create a backup of the MP3 file up to a certain limit
Status first_copy(mp3_head *mp3, int limit)
{
    char str[limit];
    strcpy(mp3->backup_fname, "backup.mp3");  // Name of the backup file
    mp3->backup = fopen(mp3->backup_fname, "w");  // Open the backup file for writing
    fread(str, limit, 1, mp3->file_ptr);  // Read data from the original file
    fwrite(str, limit, 1, mp3->backup);  // Write data to the backup file
    return e_success;
}

// Function to convert little-endian to big-endian format
Status littletobig(char *p, char *q)
{
    while (p < q)
    {
        char temp = *p;
        *p = *q;
        *q = temp;
        p++;
        q--;
    }
    return e_success;
}

// Function to handle editing of different MP3 tags (e.g., title, artist, album)
Status edit(mp3_head *mp3, char *argv[])
{
    // Edit title tag
    if ((strcmp("-t", argv[2]) == 0) && argv[4] != NULL)
    {
        rewind(mp3->file_ptr);  // Rewind the file pointer
        int ret = first_copy(mp3, mp3->TIT2_pos);  // Copy data up to TIT2 position
        ret = editing(argv, mp3);  // Edit the title
        fseek(mp3->file_ptr, mp3->TIT2_size - 1, SEEK_CUR);  // Move the file pointer
        copy_remaining_img_data(mp3->file_ptr, mp3->backup);  // Copy remaining data to backup
        printf(" \n>>>>>>> TITLE OF SONG IS EDITED SUCCESSFULLY <<<<<<<<<<<✅\n");
        sleep(5);
    }

    // Edit artist tag
    else if ((strcmp("-a", argv[2]) == 0) && argv[4] != NULL)
    {
        rewind(mp3->file_ptr);
        int ret = first_copy(mp3, mp3->TPE1_pos);
        ret = editing(argv, mp3);
        fseek(mp3->file_ptr, mp3->TPE1_size - 1, SEEK_CUR);
        copy_remaining_img_data(mp3->file_ptr, mp3->backup);
        printf(" \n>>>>>>> ARTIST NAME OF SONG IS EDITED SUCCESSFULLY <<<<<<<<<<<✅\n");
        sleep(5);
    }
    // Edit album tag
    else if ((strcmp("-A", argv[2]) == 0) && argv[4] != NULL)
    {
        rewind(mp3->file_ptr);
        int ret = first_copy(mp3, mp3->TALB_pos);
        ret = editing(argv, mp3);
        fseek(mp3->file_ptr, mp3->TALB_size - 1, SEEK_CUR);
        copy_remaining_img_data(mp3->file_ptr, mp3->backup);
        printf(" \n>>>>>>> ALBUM OF SONG IS EDITED SUCCESSFULLY <<<<<<<<<<<✅\n");
        sleep(5);
    }
    // Edit year tag
    else if ((strcmp("-y", argv[2]) == 0) && argv[4] != NULL)
    {
        rewind(mp3->file_ptr);
        int ret = first_copy(mp3, mp3->TYER_pos);
        ret = editing(argv, mp3);
        fseek(mp3->file_ptr, mp3->TYER_size - 1, SEEK_CUR);
        copy_remaining_img_data(mp3->file_ptr, mp3->backup);
        printf(" \n>>>>>>> YEAR OF SONG IS EDITED SUCCESSFULLY <<<<<<<<<<<✅\n");
        sleep(5);
    }
    // Edit content type tag
    else if ((strcmp("-c", argv[2]) == 0) && argv[4] != NULL)
    {
        rewind(mp3->file_ptr);
        int ret = first_copy(mp3, mp3->TCON_pos);
        ret = editing(argv, mp3);
        fseek(mp3->file_ptr, mp3->TCON_size - 1, SEEK_CUR);
        copy_remaining_img_data(mp3->file_ptr, mp3->backup);
        printf(" \n>>>>>>> CONTENT TYPE OF SONG IS EDITED SUCCESSFULLY <<<<<<<<<<<✅\n");
        sleep(5);
    }
    // Edit comment tag
    else if ((strcmp("-C", argv[2]) == 0) && argv[4] != NULL)
    {
        rewind(mp3->file_ptr);
        int ret = first_copy(mp3, mp3->COMM_pos);
        ret = editing(argv, mp3);
        fseek(mp3->file_ptr, mp3->COMM_size - 1, SEEK_CUR);
        copy_remaining_img_data(mp3->file_ptr, mp3->backup);
        printf(" \n>>>>>>> COMMENT OF SONG IS EDITED SUCCESSFULLY <<<<<<<<<<<✅\n");
        sleep(5);
    }
    // Display version info
    else if ((strcmp("-v", argv[2]) == 0) && argv[4] == NULL)
    {
        printf(" \n Version is : %sv2.%hd \n", mp3->signeture, mp3->ver);
        return e_success;
    }
    else 
    {
        printf("\n.....Invalid command please refer '-h' command for reference...........❌\n");
        return e_failure;
    }

    // Close the original and backup files
    fclose(mp3->file_ptr);
    fclose(mp3->backup);

    // Reopen files and copy changes back to the original file
    mp3->file_ptr = fopen(mp3->file_name, "w");
    mp3->backup = fopen(mp3->backup_fname, "r");
    copy_img_data(mp3->backup, mp3->file_ptr);
    fclose(mp3->file_ptr);
    fclose(mp3->backup);

    printf("\n>>>>>>>>>>>>>>>>> Editing is successful✅\n\n");
    return e_success;
}

// Function to write the edited text (e.g., new title, artist) into the backup file
Status editing(char *argv[], mp3_head *mp3)
{
    int len = strlen(argv[4]) + 1;  // Calculate length of the new text
    char *ptr = (char*)&len;
    int m = littletobig(ptr, ptr + 4 - 1);  // Convert to big-endian format
    fwrite(&len, sizeof(int), 1, mp3->backup);  // Write length to backup file
    fseek(mp3->file_ptr, 4, SEEK_CUR);  // Skip 4 bytes in the original file
    int pos1, pos2;
    pos1 = ftell(mp3->file_ptr);  // Get current position in the original file
    pos2 = ftell(mp3->backup);  // Get current position in the backup file
    if (pos1 != pos1)
        printf("\n>>>>>>>>>>> ERROR : positions are mismatching >>>>>>>>>>>>>>>❌\n");
    
    char str[3];
    fread(str, 3, 1, mp3->file_ptr);  // Read 3 bytes (e.g., "TIT2")
    fwrite(str, 3, 1, mp3->backup);  // Write the same 3 bytes to backup file
    pos1 = ftell(mp3->file_ptr);
    pos2 = ftell(mp3->backup);
    if (pos1 != pos1)
        printf("\n>>>>>>>>>>> ERROR : positions are mismatching >>>>>>>>>>>>>>>❌\n");
    fwrite(argv[4], strlen(argv[4]), 1, mp3->backup);  // Write the new text (title, artist, etc.)
    return e_success;
}

// Function to copy remaining image data (album artwork, etc.) from one file to another
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while ((fread(&ch, 1, 1, fptr_src)))  // Read byte by byte from the source file
    { 
        fwrite(&ch, 1, 1, fptr_dest);  // Write byte to the destination file
    }
    return e_success;
}

// Function to find the positions of various tags in the MP3 file (e.g., TIT2, TPE1, etc.)
Status pos(mp3_head *mp3)
{
    fseek(mp3->file_ptr, 10, SEEK_SET);  // Set the file pointer to the start of the ID3 tag (skipping header)

    int count = 0;  // Counter to track the number of tags found
    while (count < 6)  // Process up to 6 tags
    {
        char ch[4];  // Array to store tag identifiers (e.g., "TIT2", "TPE1", etc.)
        fread(&ch, 4, 1, mp3->file_ptr);  // Read the 4-character tag identifier
        ch[4] = '\0';  // Null-terminate the string

        // Check if the tag is TIT2 (title tag)
        if (strcmp("TIT2", ch) == 0)
        {
            mp3->TIT2_pos = ftell(mp3->file_ptr);  // Save the current file position as TIT2 position
            mp3->TIT2_size = size_des(mp3, mp3->TIT2_pos);  // Get the size of the TIT2 tag
            count++;
        }
        // Check if the tag is TPE1 (artist tag)
        else if (strcmp("TPE1", ch) == 0)
        {
            mp3->TPE1_pos = ftell(mp3->file_ptr);  // Save the position of TPE1 tag
            mp3->TPE1_size = size_des(mp3, mp3->TPE1_pos);  // Get the size of the TPE1 tag
            count++;
        }
        // Check if the tag is TALB (album tag)
        else if (strcmp("TALB", ch) == 0)
        {
            mp3->TALB_pos = ftell(mp3->file_ptr);  // Save the position of TALB tag
            mp3->TALB_size = size_des(mp3, mp3->TALB_pos);  // Get the size of the TALB tag
            count++;
        }
        // Check if the tag is TYER (year tag)
        else if (strcmp("TYER", ch) == 0)
        {
            mp3->TYER_pos = ftell(mp3->file_ptr);  // Save the position of TYER tag
            mp3->TYER_size = size_des(mp3, mp3->TYER_pos);  // Get the size of the TYER tag
            count++;
        }
        // Check if the tag is TCON (content type tag)
        else if (strcmp("TCON", ch) == 0)
        {
            mp3->TCON_pos = ftell(mp3->file_ptr);  // Save the position of TCON tag
            mp3->TCON_size = size_des(mp3, mp3->TCON_pos);  // Get the size of the TCON tag
            count++;
        }
        // Check if the tag is COMM (comment tag)
        else if (strcmp("COMM", ch) == 0)
        {
            mp3->COMM_pos = ftell(mp3->file_ptr);  // Save the position of COMM tag
            mp3->COMM_size = size_des(mp3, mp3->COMM_pos);  // Get the size of the COMM tag
            count++;
        }
        else
        {
            // If the tag doesn't match any of the known tags, skip the current tag
            int size;
            fread(&size, 4, 1, mp3->file_ptr);  // Read the size of the current tag
            char *ptr = (char*)&size;
            int m = Bigtolittle(ptr, ptr + 4 - 1);  // Convert size to little-endian
            fseek(mp3->file_ptr, size - 1 + 3, SEEK_CUR);  // Skip over the current tag data
        }
    }
    return e_success;
}


// Function to read the size of a tag from the MP3 file
int size_des(mp3_head *mp3, int pos)
{
    int size;
    fread(&size, 4, 1, mp3->file_ptr);  // Read the size of the tag
    fseek(mp3->file_ptr, 3, SEEK_CUR);  // Skip 3 bytes
    char *ptr = (char*)&size;
    int m = Bigtolittle(ptr, ptr + 4 - 1);  // Convert to little-endian format
    fseek(mp3->file_ptr, size - 1, SEEK_CUR);  // Skip over the tag data
    return size;
} 

// Function to copy the image data (album artwork) from one file to another
Status copy_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while ((fread(&ch, 1, 1, fptr_src)))  // Read byte by byte from the source file
    { 
        fwrite(&ch, 1, 1, fptr_dest);  // Write byte to the destination file
    }
    return e_success;
}
