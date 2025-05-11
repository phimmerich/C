#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nilibddc.h"

#define HEADER_SIZE 4

int main(int argc, char* argv[])
{
    DDCFileHandle file = 0;
    DDCError ddcError = 0;
    FILE* fp = NULL;
    unsigned char header[HEADER_SIZE] = { 0 };

    // Check if filename was provided
    if (argc != 2)
    {
        printf("Usage: %s <path_to_tdms_file>\n", argv[0]);
        return -1;
    }

    const char* filePath = argv[1];
    printf("Attempting to open file: %s\n", filePath);

    // Binary open and read check
    if (fopen_s(&fp, filePath, "rb") != 0)
    {
        perror("Error opening file in binary mode");
        return -1;
    }

    if (fp == NULL)
    {
        perror("Error opening file in binary mode");
        return -1;
    }

    size_t bytesRead = fread(header, 1, HEADER_SIZE, fp);
    fclose(fp);

    if (bytesRead != HEADER_SIZE)
    {
        printf("Error: Could not read the first %d bytes from the file.\n", HEADER_SIZE);
        return -1;
    }

    if (memcmp(header, "TDSm", HEADER_SIZE) != 0)
    {
        printf("Error: File does not start with 'TDSm'. Not a valid TDMS file.\n");
        return -1;
    }

    printf("Binary read check passed. 'TDSm' header confirmed.\n");

    // Now use NI library to open the file properly
    ddcError = DDC_OpenFileEx(filePath, DDC_FILE_TYPE_TDM_STREAMING, 0, &file);
    if (ddcError != DDC_NoError)
    {
        printf("Error opening TDMS file with DDC_OpenFileEx! Error code: %d\n", ddcError);
        return -1;
    }
    printf("TDMS file opened successfully with DDC_OpenFileEx.\n");

    // Example: Get number of groups
    unsigned int numGroups = 0;
    ddcError = DDC_GetNumChannelGroups(file, &numGroups);
    if (ddcError != DDC_NoError)
    {
        printf("Error getting number of groups. Error code: %d\n", ddcError);
        DDC_CloseFile(file);
        return -1;
    }

    printf("Number of channel groups in file: %u\n", numGroups);

    // Always close the file when done
    DDC_CloseFile(file);
    printf("TDMS file closed successfully.\n");

    return 0;
}
