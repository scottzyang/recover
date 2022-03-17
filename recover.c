#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE; // generates a BYTE as an 8-bit integer
const int BLOCK_SIZE = 512; // generates size of block as 512 BYTEs
// BYTE buffer[BLOCK_SIZE]; // creates an array that stores 512 BYTES

int main(int argc, char *argv[])
{
    // checks if the command line arguments are valid
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    // opens the user inputted file and stores it at pointer file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) // verifies there is valid space to read file
    {
        printf("Could not open file\n");
        return 2;
    }

    BYTE buffer[BLOCK_SIZE];
    FILE *img = NULL;
    char filename[8];
    //char *filename = malloc(sizeof(char) * 8);// Dynamically allocates memory for storing new JPEG names
    int file_number = 0;
    bool first_image_found = false;

    while (fread(buffer, BLOCK_SIZE, 1, file) == 1) // while it can read 512 items
    {
        // verify if a JPEG signature is found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // verify if JPEG signature is the first one identified
            if (file_number == 0)
            {
                first_image_found = true;
                sprintf(filename, "%03i.jpg", file_number); // creates new files with ###.jpg format and stores it at the filename address
                img = fopen(filename, "w"); // opens up the filename created with writing capabilities
                fwrite(buffer, BLOCK_SIZE, 1, img); // write data from buffer into img file that was created
                file_number++;
            }
            // if JPEG signature is not first one, close previous and write into new file
            else
            {
                fclose(img); // close previous JPEG file
                sprintf(filename, "%03i.jpg", file_number); // creates new files with ###.jpg format and stores it at the filename address
                img = fopen(filename, "w"); // opens up the filename created with writing capabilities
                fwrite(buffer, BLOCK_SIZE, 1, img); // write data from buffer into img file that was created
                file_number++;
            }
        }
        // continue writing if no JPEG signature is found
        else if (first_image_found)
        {
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }

    fclose(img);
    fclose(file);
}
