/*Documentaion
Name : Manjunath R
Date : 05/02/2025
Discription : The MP3 Metadata Editor is a command-line tool that enables users
to edit metadata within MP3 files, such as the title, artist, album, year, cont
-ent type, and comments. It reads and modifies the ID3 tags of an MP3 file, whi
-ch store information about the audio track (like the song title, artist name, e
-tc.). The tool allows users to easily update or view specific metadata fields wi
-thin an MP3 file without modifying the actual audio content
*/
#include <stdio.h>   
#include <unistd.h>  
#include "mp3.h"   

int main(int argc, char *argv[])  // Main function with command-line arguments
{
    // Check if the number of arguments is less than 2
    if(argc < 2)
    {
        printf("\n................Invalid Number of arguments please enter valid arguments ❌.............\n");
        return 1;  // Exit with error code if arguments are invalid
    }

    mp3_head mp3;  // Declare an mp3_head structure to hold MP3 information

    // Check the type of operation (view or edit)
    int ret = check_operation_type(argv);  // Function to determine if the operation is view or edit
    if(ret == v_view)  // If the operation is view
    {
        // Validate and open the file
        if(validation_and_open_file(argv, &mp3) != e_success)
        {
            printf("\n...................Viewing is stoped..............❌\n");
            return 1;  // Exit if there is an issue with file validation or opening
        }
      if(view(&mp3)!=e_success)// Call the view function to display MP3 information
      {
            printf("\n...................Viewing is stoped..............❌\n");
            return 1;
      }  
    }

    if(ret == e_edit)  // If the operation is edit
    {
        // Validate and open the file for editing
        if(validation_open_file(argv, &mp3) != e_success)
        {
            printf("\n...................Editing is stoped..............❌\n");
            return 1;  // Exit if there is an issue with file validation or opening for editing
        }

        // Check for position ( for seeking edit location)
        if(pos(&mp3) != e_success)
        {
            printf("\n...................Editing is stoped..............❌\n");
            return 1;  // Exit if position handling fails
        }

        // Perform the edit operation
        if(edit(&mp3, argv) != e_success)
        {
            printf("\n...................Editing is stoped..............❌\n");
            return 1;  // Exit if the edit operation fails
        }
    }

    return 0;  // Exit the program successfully
}
