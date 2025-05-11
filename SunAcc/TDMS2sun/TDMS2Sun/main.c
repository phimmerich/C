#define CBASED
#define CALLCONV __cdecl

#include <stdint.h>
#include <stdlib.h>
#include "SunAcc42.h"
#include "nilibddc.h"

#define NUM_POINTS_MAX 10000  // max number of data points to allocate

int main() {
    DDCFileHandle tdmsFile = NULL;
    DDCChannelGroupHandle group = NULL;
    DDCChannelHandle channel = NULL;

    float* data = NULL;
    unsigned __int64 numVals = 0;

    // --- 1. Open the TDMS File
    if (DDC_OpenFile("input.tdms", DDC_FILE_TYPE_TDM_STREAMING, &tdmsFile) != 0) {
        printf("Failed to open TDMS file.\n");
        return 1;
    }

    // --- 2. Get the first channel group
    unsigned int groupCount = 0;
    DDC_GetNumChannelGroups(tdmsFile, &groupCount);
    if (groupCount == 0) {
        printf("No channel groups found.\n");
        return 1;
    }

    DDCChannelGroupHandle groups[1];
    DDC_GetChannelGroups(tdmsFile, groups, 1);
    group = groups[0];

    // --- 3. Get the first channel
    unsigned int channelCount = 0;
    DDC_GetNumChannels(group, &channelCount);
    if (channelCount == 0) {
        printf("No channels found.\n");
        return 1;
    }

    DDCChannelHandle channels[1];
    DDC_GetChannels(group, channels, 1);
    channel = channels[0];

    // --- 4. Read the data
    DDC_GetNumDataValues(channel, &numVals);
    if (numVals > NUM_POINTS_MAX) numVals = NUM_POINTS_MAX;

    data = (float*)malloc(numVals * sizeof(float));
    if (!data) {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    DDC_GetDataValuesFloat(channel, 0, (size_t)numVals, data);

    // --- 5. Generate TIME data (assuming uniform spacing)
    float* time = (float*)malloc(numVals * sizeof(float));
    for (size_t i = 0; i < numVals; i++) {
        time[i] = (float)i * 0.01f;  // example: 100 Hz
    }

    // --- 6. Create a SUN_COMPRESSED file
    struct SunWrt* dbf = createSunFile("output.sun", 1, 64, 2, (unsigned long)numVals, SUN_COMPRESSED);
    if (!dbf) {
        printf("Failed to create SUN file. Error: %d\n", wrtStatus());
        return 1;
    }

    setCommentText(dbf, "[comment]\nGenerated from TDMS\n");

    // --- 7. Add TIME and DATA channels
    addSunParm(dbf, "TIME", "TIME", "s", "Time Axis", 0, DTS_FLOAT, time, NULL, NULL, 0, 0, NULL);
    addSunParm(dbf, "DATA1", "data1", "unit", "Converted data", 1, DTS_FLOAT, data, NULL, NULL, 0, 0, NULL);

    closeSunFile(dbf);

    // --- 8. Cleanup
    free(data);
    free(time);
    DDC_CloseFile(tdmsFile);

    printf("Conversion complete.\n");
    return 0;
}
