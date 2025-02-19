/*Documentaion
Name : Manjunath R
Date : 05/02/2025
Discription : 
View Operation (-v):
The program allows users to view the metadata of an MP3 file. 
This includes details such as the title, artist, album, year, 
content type, and comment.The metadata is extracted from the ID3 
-tags present in the MP3 file.If a file is not provided or the file
cannot be found, the program will display an error message and halt the operation.

Help Operation (--help):
The program includes a help desk that provides information 
-on how to use different commands.It lists the available oper
-ations, including viewing and editing MP3 tags, and explains 
what each option (like -v, -e, and --help) does.
*/
#include<string.h>  
#include<unistd.h>  
#include <stdio.h>  
#include "type.h"   // Custom header file for defining data types like OperationType
#include "mp3.h"    // Custom header file for MP3 operations

// Function to check operation type: view, edit, or help
OperationType check_operation_type(char *argv[]) {
   if(! strcmp(argv[1],"-v"))  // If argument is "-v" (view)
   {
        if(argv[2]==NULL)  // Check if there's no file provided after the option
        {
            printf("............... The file name is not exist in Command.................❌\n");
             printf("\n...................Viewing is stoped..............❌\n");
            return e_unsupported;  // Return unsupported operation type if no file specified
        }
        return v_view;  // Return view operation type
    } 

    else if(! strcmp(argv[1],"-e"))  // If argument is "-e" (edit)
    {
        if(argv[2]==NULL || argv[3]==NULL)  // Ensure both file and edit option are provided
        {
            printf("............... The file name is not exist in Command.................❌\n");
            printf("\n...................Editing is stoped..............❌\n");
            return e_unsupported;  // Return unsupported operation type if parameters are missing
        }
        return e_edit;  // Return edit operation type
    }

    else if(!strcmp(argv[1],"--help"))  // If argument is "--help"
    {
      help_desk();  // Display help information
      return h_help;  // Return help operation type
    }
   else  // If none of the above options are provided
   {
      printf("...............Unsupported type of Command.................❌\n");
      return e_unsupported;  // Return unsupported operation type
    }
}

// Function to display help information
Status help_desk() {
    printf("Commands for different operations :");
    printf("\n'-v'   For View\n-e'    For Editing");
    printf("\n'-t'   Modifies a Title tag\n'-a'   Modifies an Artist tag\n'-A'   Modifies an Album tag");
    printf("\n'-y'   Modifies a Year tag\n'-c'   Modifies a Content type tag\n'-C'   Modifies a Comment tag");
    printf("\n'--help'   Displays this help info\n'-v'     Displays the version\n");
}

// Function to validate and open the MP3 file
Status validation_and_open_file(char *argv[], mp3_head *mp3) {
    if(strstr(argv[2],".mp3")!=NULL)  // Check if file has .mp3 extension
    {
        if(strcmp(strstr(argv[2],".mp3"),".mp3")==0)  // Ensure the file is a valid MP3 file
        {
            mp3->file_name = argv[2];  // Store file name in the mp3 structure
            mp3->file_ptr = fopen(mp3->file_name, "r");  // Open the MP3 file in read mode
            if(mp3->file_ptr == NULL)  // Check if file opening failed
            {
                printf("\n>>>>>>>>>>>>>>>> ❌ERROR : Invalid file❌ <<<<<<<<<<<<<<<<\n");
                return e_failure;  // Return failure if the file could not be opened
            }
            char ch[5];
            fread(ch, 3, 1, mp3->file_ptr);  // Read the first 3 bytes to check for the "ID3" signature
            short s;
            fread(&s, 2, 1, mp3->file_ptr);  // Read the version of the ID3 tag
            if(strcmp(ch,"ID3")!=0 && (s!=3 || s != 4))  // Check if it's a valid ID3 file (version 3 or 4)
            {
                printf("\n>>>>>>>>>>>>>>>> ❌ERROR : Invalid file❌ <<<<<<<<<<<<<<<<\n");
                return e_failure;  // Return failure if it's not a valid ID3 file
            }
        }
    }
    else  // If the file doesn't have the .mp3 extension
    {
        printf("\n>>>>>>>>>>>>>>>>❌ Error : Invalid file Extension ❌<<<<<<<<<<<<<<<<<<<<\n");
        return e_failure;  // Return failure for invalid file extension
    }
    return e_success;  // Return success if file validation passes
}

// Function to swap bytes from big-endian to little-endian or vice versa
Status Bigtolittle(char* p, char* q) {
    while(p < q) {  // Swap the values byte by byte
        char temp = *p;
        *p = *q;
        *q = temp;
        p++;
        q--;
    }
    return e_success;  // Return success after byte swap
}

// Function to view MP3 file's metadata (e.g., title, artist, album, year)
Status view(mp3_head *mp3) {
    fseek(mp3->file_ptr, 10, SEEK_SET);  // Set file pointer to the ID3 tag position
    int count = 0;  // Counter for found tags
    printf(".................................................................................................\n");
    printf("  MP3 Tag Reader and Editor for IDV3\n");
    printf(".................................................................................................\n");
    do {
        char ch[4];
        fread(&ch, 4, 1, mp3->file_ptr);  // Read the next 4 bytes to check the tag identifier
        ch[4] = '\0';
        // If the tag is a specific type (e.g., TIT2 for title), process the data
        if(strcmp("TIT2", ch) == 0) {
            char des[100];
            mp3->TIT2_pos = ftell(mp3->file_ptr);  // Store the position of the tag
            print(mp3, mp3->TIT2_pos, des);  // Read and print the tag's value
            printf("TITLE        : %s\n", des);
            count++;
        }
        // Similarly process other tags like TPE1 (artist), TALB (album), etc.
        else if(strcmp("TPE1", ch) == 0) {
            char des[100];
            mp3->TPE1_pos = ftell(mp3->file_ptr);
            print(mp3, mp3->TPE1_pos, des);
            printf("ARTIST NAME  : %s\n", des);
            count++;
        }
        else if(strcmp("TALB", ch) == 0) {
            char des[100];
            mp3->TALB_pos = ftell(mp3->file_ptr);
            print(mp3, mp3->TALB_pos, des);
            printf("ALBUM        : %s\n", des);
            count++;
        }
        else if(strcmp("TYER", ch) == 0) {
            char des[100];
            mp3->TYER_pos = ftell(mp3->file_ptr);
            print(mp3, mp3->TYER_pos, des);
            printf("YEAR         : %s\n", des);
            count++;
        }
        else if(strcmp("TCON", ch) == 0) {
            char des[100];
            mp3->TCON_pos = ftell(mp3->file_ptr);
            print(mp3, mp3->TCON_pos, des);
            printf("CONTENT TYPE : %s\n", des);
            count++;
        }
        else if(strcmp("COMM", ch) == 0) {
            char des[100];
            mp3->TCON_pos = ftell(mp3->file_ptr);
            print(mp3, mp3->TCON_pos, des);
            printf("COMMENT      : %s\n", des);
            count++;
        }
        else {
            int size;
            fread(&size, 4, 1, mp3->file_ptr);  // Read the size of the unknown frame
            char *ptr = (char*)&size;
            int m = Bigtolittle(ptr, ptr + 4 - 1);  // Convert size to little-endian if necessary
            fseek(mp3->file_ptr, size - 1 + 3, SEEK_CUR);  // Skip the unknown frame
            if(feof(mp3->file_ptr)) {  // If end of file is reached, break
                break;
            }
        }
    } while(count < 6);  // Repeat until 6 tags are found
    printf(".................................................................................................\n");
    return e_success;  // Return success after viewing all metadata
}

// Function to read a specific tag and store its value in the provided string
Status print(mp3_head *mp3, int pos, char str[]) {
    int size;
    fseek(mp3->file_ptr, pos,SEEK_SET);  // Set file pointer to the position of the tag
    fread(&size, 4, 1, mp3->file_ptr);  // Read the size of the tag
    char *ptr = (char*)&size;
    int m =Bigtolittle(ptr, ptr + 4 - 1);  // Convert size to little-endian if necessary
    fseek(mp3->file_ptr, 3, SEEK_CUR);  // Skip the 3 bytes after the size field
    fread(str, size - 1, 1, mp3->file_ptr);  // Read the tag's value
    str[size - 1] = '\0';  // Null-terminate the string
    return e_success;  // Return success after reading the tag
}
