#ifndef VIEW_H
#define VIEW_H
#include "type.h"

// Structure to store MP3 file data and related file pointers
typedef struct mp3
{
    FILE *file_ptr;          // Pointer to the MP3 file
    char *file_name;         // Name of the MP3 file

    int TIT2_pos;            // Position of the Title (TIT2) tag in the file
    int TIT2_size;           // Size of the Title (TIT2) tag

    int TPE1_pos;            // Position of the Artist (TPE1) tag in the file
    int TPE1_size;           // Size of the Artist (TPE1) tag

    int TALB_pos;            // Position of the Album (TALB) tag in the file
    int TALB_size;           // Size of the Album (TALB) tag

    int TYER_pos;            // Position of the Year (TYER) tag in the file
    int TYER_size;           // Size of the Year (TYER) tag

    int TCON_pos;            // Position of the Genre (TCON) tag in the file
    int TCON_size;           // Size of the Genre (TCON) tag

    int COMM_pos;            // Position of the Comment (COMM) tag in the file
    int COMM_size;           // Size of the Comment (COMM) tag

    char signeture[3];       // Signature of the MP3 file (e.g., "ID3")
    short ver;               // Version of the ID3 tag

    FILE *backup;            // Pointer to the backup file
    char backup_fname[20];   // Name of the backup file
} mp3_head;

// Function to check the type of operation (view, edit, help...) based on command-line arguments
OperationType check_operation_type(char *argv[]);

// Function to view MP3 file metadata
Status view(mp3_head *mp3);

// Function to convert big-endian to little-endian format
Status Bigtolittle(char* p, char* q);

// Function to validate and open the MP3 file
Status validation_and_open_file(char *argv[], mp3_head *mp3);

// Function to print metadata from the MP3 file
Status print(mp3_head *mp3, int pos, char str[]);

// Function to validate and open the MP3 file (alternative implementation)
Status validation_open_file(char *argv[], mp3_head *mp3);

// Function to edit MP3 file metadata
Status edit(mp3_head *mp3, char *argv[]);

// Function to convert little-endian to big-endian format
Status littletobig(char* p, char* q);

// Function to create the first copy of the MP3 file (backup)
Status first_copy(mp3_head *mp3, int limit);

// Function to copy remaining image data from source to destination file
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

// Function to handle editing operations
Status editing(char *argv[], mp3_head *mp3);

// Function to calculate positions of metadata tags in the MP3 file
Status pos(mp3_head *mp3);

// Function to calculate the size of a descriptor at a given position
int size_des(mp3_head *mp3, int pos);

// Function to display help information
Status help_desk();

// Function to copy image data from source to destination file
Status copy_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif