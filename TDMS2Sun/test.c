#define CALLCONV __cdecl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <share.h>
#include <limits.h>
#include "ErrorCodes.h"
/* must have this symbol set if we are on little endian platform */
#define LITTLE_ENDIAN_PLATFORM
#include "SunAcc42.h"
/* sample program */
/* ****************************************************************** */
void main(void)
{
    struct SunWrt *mySunWrt;
    long unsigned nPoints = 15;
    long unsigned nItems = 7;
    long unsigned i, commentSize = 128;
    float time[15];
    double starttime;
    struct TimeTag ts[15];
    struct TimeTag refT;
    double dat1[15] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 9., 8., 7., 4., 1., 0.};
    double dat2[15] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 9., 8., 7., 4., 1., 0.};
    long dat3[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 4, 1, 0};
    short dat4[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 4, 1, 0};
    char *dat5 = {"abcdefghijklmno"};
    char *fileName = {"sample.dat"};
    printf("test app for read/write library: %s\n", wrtVersion());
    /* set referenct time (12:00:00.000 Jan 31/day 31) */
    refT.JulianDay = 31;
    refT.MilliSeconds = 43200000L; /* Noon in milliseconds*/
    refT.MicroSeconds = 0;
    /* generate TimeStamp data */
    starttime = -.120;
    for (i = 0; i < nPoints; i++)
    {
        dat1[i] = starttime + ((double)i * .040);
        time[i] = (float)dat1[i];
        GMT(dat1 + i, &refT, ts + i, 0); /* create a TimeTag from a reference TimeTag and offset */
        TIME(ts + i, &refT, dat2 + i);   /* calculate relative time from reference TimeTag and delta TimeTag */
    }

    /* to write a SUN_COMMONTIME format, simply replace SUN_COMPRESSED with SUN_COMMONTIME */
    if ((mySunWrt = createSunFile(fileName, OVERWRITE, commentSize, nItems, nPoints, SUN_COMPRESSED)))
    {
        /* call some of the h_set functions for header data */
        if (setCommentText(mySunWrt,
                           "[events]\nDATASTART = 31/43199880.000\nSTART = 31/43200000.000\n[history]\nSample program data\n"))
            printf("failure in setCommentText() code=%d\n", wrtStatus());
        if (h_setRefTime(mySunWrt, 2002L, 31L, 12L, 00L, 00L, 0L)) /* ref: 2002/12:00:00.000 */
            printf("failure in h_setRefTime() code=%d\n", wrtStatus());
        if (h_setCutoff(mySunWrt, (float)2.0))
            printf("failure in h_setCutoff() code=%d\n", wrtStatus());
        if (h_setTitle(mySunWrt, wrtVersion()))
            printf("failure in h_setTitle() code=%d\n", wrtStatus());
        if (h_setComment(mySunWrt, "Header comment"))
            printf("failure in h_setComment() code=%d\n", wrtStatus());
        if (h_setSerialNum(mySunWrt, "Serial #"))
            printf("failure in h_setSerialNum() code=%d\n", wrtStatus());
        if (h_setTestPos(mySunWrt, "Test Position"))
            printf("failure in h_setTestPos() code=%d\n", wrtStatus());
        if (h_setTestStand(mySunWrt, "Test Stand"))
            printf("failure in h_setTestStand() code=%d\n", wrtStatus());
        if (h_setTestType(mySunWrt, "Test Type"))
            printf("failure in h_setTestType() code=%d\n", wrtStatus());
        if (h_setTestNumb(mySunWrt, "Test Number"))
            printf("failure in h_setTestNumb() code=%d\n", wrtStatus());
        /* write the parameter data */
        if (addSunParm(mySunWrt, "TIME", "TIME", "SECONDS", "TIME IN SECONDS", 1, DTS_FLOAT, time, NULL, NULL, 0, 0, NULL))
            printf("failure in addSunParm() code=%d\n", wrtStatus());
        if (addSunParm(mySunWrt, "GMT", "TIME2", "uSEC.", "TIME IN MILLISECONDS", 2, DTS_TIMETAG, ts, NULL, NULL, 0, 0, NULL))
            printf("failure in addSunParm() code=%d\n", wrtStatus());
        if (addSunParm(mySunWrt, "DTIME", "dat1", "8bytes", "Double Precision Float", 3, DTS_DOUBLE, dat1, NULL, NULL, 0, 0, NULL))
            printf("failure in addSunParm() code=%d\n", wrtStatus());
        if (addSunParm(mySunWrt, "DOUBLE", "dat2", "8bytes", "Double Precision Float", 4, DTS_DOUBLE, dat2, NULL, NULL, 0, 0, NULL))

            printf("failure in addSunParm() code=%d\n", wrtStatus());
        if (addSunParm(mySunWrt, "LONGINT", "dat3", "4bytes", "Long Integer", 5, DTS_LONGINT, dat3, NULL, NULL, 0, 0, NULL))
            printf("failure in addSunParm() code=%d\n", wrtStatus());
        if (addSunParm(mySunWrt, "SHORTINT", "dat4", "2bytes", "Short Integer", 6, DTS_SHORTINT, dat4, NULL, NULL, 0, 0, NULL))
            printf("failure in addSunParm() code=%d\n", wrtStatus());
        if (addSunParm(mySunWrt, "BYTE", "dat5", "1byte", "Character", 7, DTS_BYTE, dat5, NULL, NULL, 0, 0, NULL))
            printf("failure in addSunParm() code=%d\n", wrtStatus());
        /* finish the file */
        if (closeSunFile(mySunWrt))
            printf("failure in closeSunFile() code=%d\n", wrtStatus());
    }
    else
        printf("failure in createSunWrt() code=%s\n", GetAccessErrorText(wrtStatus()));
    printf("done.\n");
}