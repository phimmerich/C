#define CALLCONV __cdecl

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "nilibddc.h"
#include "SunAcc42.h"
#include "time.h"
#include "ErrorCodes.h"

typedef struct SunWrt SunWrt;
typedef struct SunParm SunParm;

void setRefTimeNow(SunWrt *sunFile)
{
    time_t rawTime;
    struct tm timeInfo;
    time(&rawTime);
    gmtime_s(&timeInfo, &rawTime);

    unsigned long year = (unsigned long)(timeInfo.tm_year + 1900);
    unsigned long day = (unsigned long)(timeInfo.tm_yday + 1);
    unsigned long hour = (unsigned long)(timeInfo.tm_hour);
    unsigned long minute = (unsigned long)(timeInfo.tm_min);
    unsigned long second = (unsigned long)(timeInfo.tm_sec);
    unsigned long millisecond = 0;

    int err = h_setRefTime(sunFile, year, day, hour, minute, second, millisecond);
    if (err != 0)
    {
        printf("Warning: h_setRefTime failed with error code %d\n", err);
    }
}

int createParametersFromTDMS(DDCFileHandle tdmsFile, SunWrt *sunFile)
{
    unsigned int numGroups = 0;
    int ddcError = DDC_GetNumChannelGroups(tdmsFile, &numGroups);
    if (ddcError != DDC_NoError || numGroups == 0)
    {
        printf("TDMS group error or empty. Code: %d\n", ddcError);
        return -1;
    }

    DDCChannelGroupHandle *groups = calloc(numGroups, sizeof(DDCChannelGroupHandle));
    ddcError = DDC_GetChannelGroups(tdmsFile, groups, numGroups);
    if (ddcError != DDC_NoError)
    {
        free(groups);
        return -1;
    }
    //double dat1[15] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 9., 8., 7., 4., 1., 0.};

    //if (addSunParm(sunFile, "DTIMExxx", "dat1", "8bytes", "Double Precision Float", 8, DTS_DOUBLE, dat1, NULL, NULL, 0, 0, NULL))
    //    printf("failure in addSunParm() code=%d\n", wrtStatus());




    for (unsigned int i = 0; i < numGroups; ++i)
    {
       unsigned int numChannels = 0;
       ddcError = DDC_GetNumChannels(groups[i], &numChannels);
       if (ddcError != DDC_NoError || numChannels == 0)
           continue;

       DDCChannelHandle *channels = calloc(numChannels, sizeof(DDCChannelHandle));
       DDC_GetChannels(groups[i], channels, numChannels);

        for (unsigned int j = 0; j < numChannels; ++j)
        {
           unsigned int nameLength;
           DDC_GetChannelStringPropertyLength(channels[j], DDC_CHANNEL_NAME, &nameLength);
           char *channelName = malloc(nameLength + 1);
           DDC_GetChannelProperty(channels[j], DDC_CHANNEL_NAME, channelName, nameLength + 1);

           if (_stricmp(channelName, "Time_calc") == 0 ||
               _stricmp(channelName, "Timestamp") == 0 ||
               _stricmp(channelName, "t") == 0) {
               strcpy_s(channelName, nameLength + 1, "TIME");
               printf("Found time channel: %s\n", channelName);
           }
           //printf("Found time channel: %s\n", channelName);
           for (char *p = channelName; *p; ++p)
               if (*p == '/' || *p == '\\')
                   *p = '_';

           unsigned __int64 numDataValues = 0;
           DDC_GetNumDataValues(channels[j], &numDataValues);

           // float* data = calloc(numDataValues, sizeof(float));
           // float* index = calloc(numDataValues, sizeof(float));
           // for (unsigned int k = 0; k < numDataValues; ++k) index[k] = (float)k;

           DDCDataType type;
           if (DDC_GetDataType(channels[j], &type) != 0 || type != DDC_Float)
           {
               //printf("breakout loop number %i with type %d\n", j, type);
               // continue;
           }
           // printf("%d",type);
       

           double *data = calloc(numDataValues, sizeof(double));
           float *index = calloc(numDataValues, sizeof(float));
           if (!data || !index)
               exit(1); // or handle error

           for (unsigned int k = 0; k < numDataValues; ++k)
               index[k] = (float)k;

           //DDC_GetDataValues(channels[j], 0, numDataValues, data);

           DDC_GetDataValues(channels[j], 0, (unsigned int)numDataValues, data);

           channelName[nameLength] = '\0';

           //printf("pre addSun %Ii\n", numDataValues);

           if (_stricmp(channelName, "TIME") == 0){

               float* floatData = malloc(numDataValues * sizeof(float));
               if (!floatData) exit(1);

               for (unsigned int i = 0; i < numDataValues; ++i) {
                   floatData[i] = (float)data[i];  // safe conversion
               }

               // Then call:
               addSunParm(sunFile, "TIME", "TIME", "SECONDS", "TIME IN SECONDS", 1, DTS_FLOAT, floatData, NULL, NULL, 0, 0, NULL);

               // Clean up:
               free(floatData);

           }
           else {

               int sunErr = addSunParm(sunFile,
                   channelName,
                   channelName,
                   "unitless",
                   "TDMS imported",
                   j,
                   DTS_DOUBLE,
                   data,
                   NULL,
                   NULL,
                   0,
                   0,
                   NULL);
           }


           // int sunErr = addSunParm(sunFile,
           //     "channelName",
           //     "channelName",
           //     "unitless",
           //     "TDMS imported",
           //     j,
           //     DTS_DOUBLE,
           //     dat1,
           //     NULL,
           //     NULL,
           //     NULL,
           //     NULL,
           // NULL);

           // if (addSunParm(sunFile, "DTIME", "dat1", "8bytes", "Double Precision Float", j, DTS_DOUBLE, dat1, NULL, NULL, NULL, NULL, NULL))
           //     printf("failure in addSunParm() code=%s\n", GetAccessErrorText(wrtStatus()));

           // if (sunErr != 0) {

           //    printf("addSunParm failed: %s\n", GetAccessErrorText(sunErr));
           //}
           //printf("post addSun %u\n", j);
           free(channelName);
           free(data);
           free(index);
       }
       free(channels);
    }
    free(groups);
    showFileUpdated(sunFile);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <path_to_tdms_file>\n", argv[0]);
        return -1;
    }

    const char *filePath = argv[1];
    printf("Opening TDMS file: %s\n", filePath);

    DDCFileHandle tdmsFile = NULL;
    if (DDC_OpenFileEx(filePath, DDC_FILE_TYPE_TDM_STREAMING, 0, &tdmsFile) != DDC_NoError)
    {
        printf("Failed to open TDMS file.\n");
        return -1;
    }

    const char *sunFileName = "output.sun";
    long unsigned i, commentSize = 128;
    long unsigned nPoints = 20;
    long unsigned nItems = 99;

    double dat1[15] = { 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 4., 1., 0. };
    double dat2[15] = { 1., 2., 3., 4., 5., 6., 7., 8., 9., 9., 8., 7., 4., 1., 0. };
    long dat3[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 4, 1, 0 };
    short dat4[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 4, 1, 0 };
    char* dat5 = { "abcdefghijklmno" };
    float time[15];
    struct TimeTag ts[15];
    struct TimeTag refT;
    double starttime;
    refT.JulianDay = 31;
    refT.MilliSeconds = 43200000L; /* Noon in milliseconds*/
    refT.MicroSeconds = 0;
    /* generate TimeStamp data */
    starttime = -.120;

    SunWrt *sunFile = createSunFile((char *)sunFileName, OVERWRITE, commentSize, nItems, 1000, SUN_COMPRESSED);
    if (!sunFile)
    {
        printf("Failed to create SUN file.\n");
        DDC_CloseFile(tdmsFile);
        return -1;
    }

    h_setTitle(sunFile, "Example SUN File Title");
    h_setTestStand(sunFile, "My Test Stand Name");
    h_setTestType(sunFile, "Functional Test");
    h_setComment(sunFile, "Generated from TDMS");
    setRefTimeNow(sunFile);

    for ( i = 0; i < nPoints; i++)
    {
        dat1[i] = starttime + ((double)i * .040);
        time[i] = (float)dat1[i];
        GMT(dat1 + i, &refT, ts + i, 0); /* create a TimeTag from a reference TimeTag and offset */
        TIME(ts + i, &refT, dat2 + i);   /* calculate relative time from reference TimeTag and delta TimeTag */
    }


    //double dat1[15] = { 1., 2., 3., 4., 5., 6., 7., 8., 9., 9., 8., 7., 4., 1., 0. };
    //if (addSunParm(sunFile, "TIME", "TIME", "SECONDS", "TIME IN SECONDS", 1, DTS_FLOAT, time, NULL, NULL, 0, 0, NULL))
        //printf("failure in addSunParm() code=%d\n", wrtStatus());
    //if (addSunParm(sunFile, "GMT", "TIME2", "uSEC.", "TIME IN MILLISECONDS", 2, DTS_TIMETAG, ts, NULL, NULL, 0, 0, NULL))
    //    printf("failure in addSunParm() code=%d\n", wrtStatus());
    //if (addSunParm(sunFile, "DTIME", "dat1", "8bytes", "Double Precision Float", 3, DTS_DOUBLE, dat1, NULL, NULL, 0, 0, NULL))
    //    printf("failure in addSunParm() code=%d\n", wrtStatus());
    //if (addSunParm(sunFile, "DOUBLE", "dat2", "8bytes", "Double Precision Float", 4, DTS_DOUBLE, dat2, NULL, NULL, 0, 0, NULL))

    //    printf("failure in addSunParm() code=%d\n", wrtStatus());
    //if (addSunParm(sunFile, "LONGINT", "dat3", "4bytes", "Long Integer", 5, DTS_LONGINT, dat3, NULL, NULL, 0, 0, NULL))
    //    printf("failure in addSunParm() code=%d\n", wrtStatus());
    //if (addSunParm(sunFile, "SHORTINT", "dat4", "2bytes", "Short Integer", 6, DTS_SHORTINT, dat4, NULL, NULL, 0, 0, NULL))
    //    printf("failure in addSunParm() code=%d\n", wrtStatus());
    //if (addSunParm(sunFile, "BYTE", "dat5", "1byte", "Character", 7, DTS_BYTE, dat5, NULL, NULL, 0, 0, NULL))
    //    printf("failure in addSunParm() code=%d\n", wrtStatus());

    createParametersFromTDMS(tdmsFile, sunFile);

    printf("Finalizing SUN file...\n");
    //showFileUpdated(sunFile);
    if (closeSunFile(sunFile) != 0)
        printf("Warning: closeSunFile failed.\n");
    else
        printf("SUN file closed successfully.\n");

    DDC_CloseFile(tdmsFile);
    return 0;
}
