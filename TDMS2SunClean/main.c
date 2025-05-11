#define CALLCONV __cdecl

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "nilibddc.h"
#include "SunAcc42.h"

// -------------------------------------
// Error code lookup helper
const char* DDC_GetErrorString(DDCError error)
{
    switch (error)
    {
    case DDC_NoError: return "No Error";
    case DDC_OutOfMemory: return "Out of Memory";
    case DDC_InvalidArgument: return "Invalid Argument";
    case DDC_UnexpectedError: return "Unexpected Error (often invalid TDMS file)";
    case DDC_InvalidFileHandle: return "Invalid File Handle";
    case DDC_FileDoesNotExist: return "File Does Not Exist";
    case DDC_InvalidChannelGroupHandle: return "Invalid Channel Group Handle";
    case DDC_InvalidChannelHandle: return "Invalid Channel Handle";
    //case DDC_InvalidPropertyHandle: return "Invalid Property Handle";
    case DDC_PropertyDoesNotExist: return "Property Does Not Exist";
    //case DDC_PropertyTypeMismatch: return "Property Type Mismatch";
    case DDC_FileAlreadyExists: return "File Already Exists";
    default: return "Unknown Error";
    }
}

// -------------------------------------
// Safe error checker
#define CHECK_DDC_ERROR(x) do { \
    DDCError err = (x); \
    if (err != DDC_NoError) { \
        printf("❌ DDC Error %d: %s\n", err, DDC_GetErrorString(err)); \
        return 1; \
    } \
} while(0)

// -------------------------------------
// Try to open file normally
DDCError TryOpenTDMSFile(const char* filePath, DDCFileHandle* tdmsFile)
{
    printf("🔎 Trying DDC_OpenFile()...\n");
    DDCError error = DDC_OpenFile(filePath, DDC_FILE_TYPE_TDM_STREAMING, tdmsFile);
    return error;
}

// -------------------------------------
// Try to open file with Ex version
DDCError TryOpenTDMSFileEx(const char* filePath, DDCFileHandle* tdmsFile)
{
    const char* pluginPath = "./"; // Current directory, adjust if needed
    int32_t openFlags = 0; // Default flags

    printf("🔎 Trying DDC_OpenFileEx() with plugin path: %s\n", pluginPath);
    DDCError error = DDC_OpenFileEx(filePath, pluginPath, openFlags, &tdmsFile);
    return error;
}

// -------------------------------------
// Main program
int main()
{
    DDCFileHandle tdmsFile = NULL;
    const char* tdmsFilePath = "example.tdms";

    printf("📂 Attempting to open TDMS file: %s\n", tdmsFilePath);

    DDCError error = TryOpenTDMSFile(tdmsFilePath, &tdmsFile);

    if (error != DDC_NoError)
    {
        printf("⚡ Initial open failed: %s\n", DDC_GetErrorString(error));
        printf("➡️  Attempting fallback using DDC_OpenFileEx()...\n");

        error = TryOpenTDMSFileEx(tdmsFilePath, &tdmsFile);

        if (error != DDC_NoError)
        {
            printf("❌ Fallback also failed: %s\n", DDC_GetErrorString(error));
            return 1;
        }
    }

    printf("✅ TDMS file opened successfully!\n");

    CHECK_DDC_ERROR(DDC_CloseFile(tdmsFile));

    printf("✅ TDMS file closed successfully!\n");

    return 0;
}
