#pragma once

#ifndef _INC_SUNACC40_H
#define _INC_SUNACC40_H

#include <stdio.h>

// for VS2010 bug - stdint.h/intsafe.h conflicts
#ifdef _INTSAFE_H_INCLUDED_
#  undef INT8_MIN	  
#  undef INT16_MIN	
#  undef INT32_MIN	
#  undef INT8_MAX  	
#  undef INT16_MAX	
#  undef INT32_MAX	
#  undef UINT8_MAX	
#  undef UINT16_MAX	
#  undef UINT32_MAX	
#  undef INT64_MIN
#  undef INT64_MAX
#  undef UINT64_MAX
#endif
#include <stdint.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// platform dependent symbols

//#pragma message("if compiling on 32 bit Linux/Solaris/etc. w/ gcc, use -D_FILE_OFFSET_BITS=64 compiler flag for large file access")

// set this symbol for SUN_MULTITIME64 support (if compatible with platform)
#define USE64FORMAT
//#ifdef USE64FORMAT
//#  pragma message("Compatibility symbol SUN_MULTITIME64 is set!")
//#endif

// set this symbol for use on little endian platforms (default = big endian if not set)
#define LITTLE_ENDIAN_PLATFORM
//#ifdef LITTLE_ENDIAN_PLATFORM
//#  pragma message("Compatibility symbol LITTLE_ENDIAN_PLATFORM is set!")
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// test for 64 file format (64 bit file support may not be compatible on non-Windows platforms)
#define IS64BIT(a) ( (a == SUN_MULTITIME64) || (a == SUN_MULTITIME64_SRT) )

/* MAKEDLL - set for compiling library (dll) */
#ifdef MAKEDLL 
#  define DLLEXP __declspec( dllexport ) 
#  define CBASED
#  define CALLCONV
#else
/* using .H file for application */
#  define DLLEXP
/* ANSI C program */
#  ifdef __STDC__
#    define CBASED
#    define CALLCONV
#  else
/* C++ program */
#    define CBASED extern "C"
#    define CALLCONV _cdecl
#  endif 
#endif

#define INVALID 0xffffffff

/* real-time data drop-out flag value (4 byte type) */
#define DROP_OUT 0x80000001
#define FP_DROP_OUT (-1.40130E-45F)

/* value to indicate variable block data (DT_VARIABLE) */
#define VARIABLE 0x00000001
#define FP_VARIABLE (1.40130E-45F)

/* allocation size for SUN_MULTITIME link address stack */
#define STACK_ALLOC_SIZE 32

/* time matching tolerance */
#define TIME_TOLERANCE      .000001  /* max resolution of time stamp (1 uSec)*/

/* min amount of time before extrapolation of end points SUN_MULTITIME */
#define MIN_TIME_GAP 1.0

/* time stamping modes for data averaging function average() */
#define TS_FIRSTPOINT 0
#define TS_LASTPOINT  1
#define TS_AVERAGED   2

#define RATE_VARIABLE  0.f
#define RATE_UNKNOWN  -1.f

/* macro to pack data rate with sequence number (SUN_MULTITIME only) */
/* valid rate = 0 .. 32768, valid seqn = 0 .. 65534 0xffff = invalid */
#define PACK_RATE(rate,seqn) (0x80000000 | (rate << 16) | (seqn > 0xffff ? 0xffff : (seqn & 0x0000ffff)))

#define DBL_BLOCK         500       /* file write double blocking threshold (SUN_COMPRESSED, SUN_COMMONTIME) */
   
// v4.2.6.3 - added HEADERONLY to alow simple modification of header fields that do not need to modify, move or expand parameter data.
enum CREATEMODE { CREATE = 0x00, OVERWRITE = 0x01, UPDATE = 0x02, HEADERONLY = 0x03 };
// v4.2.6.3 - added MAPxx to provide method to reduce pre-allocation (xx = percentage) of parameter nomenclature items (name, units and description)
// use for cases where files are created with extremenly large parameter counts.
// max pre-allocation for nomenclature is 1038 bytes per entry, use MAP40 to preallocate 40% of max (use only when actual size is known)
enum PREALLOC_ADJ { MAP20 = 0x0100, MAP40 = 0x0200, MAP60 = 0x0300, MAP80 = 0x0400 }; // to be ORed with CREATEMODE 

/* ****************************************************************** */
/* file signature words for compatible formats                        */

#ifdef LITTLE_ENDIAN_PLATFORM
#  define SUN_FORMAT          0xf0ffffffUL
#  define SUN_COMPRESSED      0xffffffffUL
#  define SUN_UNCOMPRESSED    0xf0ffffffUL
#  define SUN_CIRCULAR        0xf1ffffffUL
#  define SUN_MULTITIME       0xf2ffffffUL
#  define SUN_COMMONTIME      0xf3ffffffUL
#  define SUN_MULTITIME64     0xf4ffffffUL
#  define SUN_MULTITIME_SRT   0xf5ffffffUL
#  define SUN_MULTITIME64_SRT 0xf6ffffffUL
#  define EXTENDED_TYPE       0xff00
#else
#  define SUN_FORMAT          0xfffffff0UL
#  define SUN_COMPRESSED      0xffffffffUL
#  define SUN_UNCOMPRESSED    0xfffffff0UL
#  define SUN_CIRCULAR        0xfffffff1UL
#  define SUN_MULTITIME       0xfffffff2UL
#  define SUN_COMMONTIME      0xfffffff3UL
#  define SUN_MULTITIME64     0xfffffff4UL
#  define SUN_MULTITIME_SRT   0xfffffff5UL
#  define SUN_MULTITIME64_SRT 0xfffffff6UL
#  define EXTENDED_TYPE       0x00ff
#endif

/* ****************************************************************** */
/* data type ID strings and values  
                                  */
#define DTS_IMAGE       "  "
#define DTS_FLOAT       "f4"
#define DTS_DOUBLE      "d8"
#define DTS_SHORTINT    "i2"
#define DTS_LONGINT     "i4"
#define DTS_USHORTINT   "u2"
#define DTS_ULONGINT    "u4"
#define DTS_TIMETAG     "t8"
#define DTS_BYTE        "b1"
#define DTS_PACKEDTIME  "p4"  /* new for v4.00 */
#define DTS_VARIABLE    "v1"  /* new for v4.21 */
#define DTS_ULONGLONG   "u8"  /* new for v4.2.6 */
#define DTS_LONGLONG    "i8"  /* new for v4.2.6 */

#define DT_IMAGE        0x0000
#define DT_FLOAT        0x0001
#define DT_DOUBLE       0x0002
#define DT_SHORTINT     0x0003
#define DT_LONGINT      0x0004
#define DT_USHORTINT    0x0005
#define DT_ULONGINT     0x0006
#define DT_TIMETAG      0x0007
#define DT_BYTE         0x0008
#define DT_PACKEDTIME   0x0009  /* new for v4.00 32bit (milTOD:27 lsb, relativeDay:5 msb)*/
#define DT_VARIABLE     0x000a  /* new for v4.21 */
#define DT_ULONGLONG    0x000b  /* new for v4.2.6 */
#define DT_LONGLONG     0x000c  /* new for v4.2.6 */

#define DT_CLOCKSTR     0x000f  /* this type NOT used as a data storage type */

/* flag for SUN_MULTITIME to return GMT array with data and relative time */
#define DT_RETURNGMT 0x0100  /* to be ORed with the DT_? data types */

/* ****************************************************************** */
/* time tag structure for high resolution time stamps                 */

CBASED struct TimeTag
{  
  uint32_t MilliSeconds;      /* milliseconds since midnight */
  uint16_t MicroSeconds;      /* microseconds word           */
  int16_t  JulianDay;         /* Julian day of time stamp    */
};

/* ****************************************************************************** */
/* new for v4.00 (SunAcc)                                                         */
#define PHEADSIZE 36         /* allocated size may vary due to member alignment   */
struct pidHeader             /* SUN_MULTITIME parm header/data block prefix       */
{ 
  uint32_t      points;      /* point count of data block                         */
  uint32_t      linked;      /* 32 bit address of linked data block or NULL       */
  uint32_t      TimeLzhSize; /* size of compressed time array                     */
  uint32_t      DataLzhSize; /* size of compressed data array                     */
  uint8_t       indexType;   /* type of data value for index/Time                 */
  uint8_t       nomenSize;   /* nomen block for index, default to "TIME" if zero  */
  uint8_t       compressed;  /* data LZH compressed/time delta'ed LZH compressed  */
  uint8_t       bigEndian;   /* little/big endian flag for time & data            */
  double firstTime,lastTime; /* start/end time - type cast to indexType           */
  struct TimeTag blkBaseGMT; /* base GMT of parameter data block                  */
  /* nomen block if nomenSize != 0, <= 256 bytes 3 CSV strings (name,units,descr) */
};

/* ****************************************************************************** */
/* new for v4.10 (only supported on Windows platform)                             */
#define PHEAD64SIZE 40       /* allocated size may vary due to member alignment   */
struct pidHeader64           /* SUN_MULTITIME64 parm header/data block prefix     */
{ 
  uint32_t      points;      /* point count of data block                         */
  int64_t       linked;      /* 64 bit address of linked data block or NULL       */
  uint32_t      TimeLzhSize; /* size of compressed time array                     */
  uint32_t      DataLzhSize; /* size of compressed data array                     */
  uint8_t       indexType;   /* type of data value for index/Time                 */
  uint8_t       nomenSize;   /* nomen block for index, default to TIME if NULL    */
  uint8_t       compressed;  /* data LZH compressed/time delta'ed LZH compressed  */
  uint8_t       bigEndian;   /* little/big endian flag for time & data            */
  double firstTime,lastTime; /* start/end time - type cast to indexType           */
  struct TimeTag blkBaseGMT; /* base GMT of parameter data block                  */
  /* nomen would follow here <= 256 bytes in 3 CSV strings (name,units,descr)     */
};

/* options used by addSunParm()   (combine flags by ORing desired options)        */
#define LZH_COMPRESS      0x01 /* flag to compress data when written              */
#define PACK_TIMETAG      0x04 /* flag to pack TimeTags, ignoring microseconds    */ 
#define MANUAL_UPDATE     0x08 /* flag, application will use showFileUpdated() to */
                               /*  indicate new data added to SUN_MULTITIME format*/
#define ORIENT_LITTLE_END 0x10 /* flag to store data in little endian orientation */
#define ORIENT_BIG_END    0x20 /* flag to store data in big endian                */
#define ORIENT_NATIVE     0x40 /* flag to store data in native orientation        */
/* ****************************************************************************** */

/* ****************************************************************** */
/* structure for holding file events from comment block               */

CBASED struct Event 
{ 
  int count;                           /* count of events contained   */
  char **name;                         /* name of the event           */
  struct TimeTag *GMT;                 /* time of the event           */
};

/* ****************************************************************** */
/* file structures for SUN compressed/non-compressed data files       */

CBASED struct SunHeader 
{ 
  uint32_t           fileSig,          /* file signature word         */
                     refTime[6],       /* GMT at time zero            */
                     items,            /* number of items in file     */
                     blockPoints[2];   /* points in data blocks       */
  float              cutoff;           /* engine cutoff time          */
  char               title[80],        /* file title                  */
                     comment[80],      /* header comment              */
                     serialNum[12],    /* engine serial number        */
                     testStand[12],    /* test stand name             */
                     testPosition[12], /* test position               */
                     testType[12],     /* type of test                */
                     testNumber[12];   /* number of test              */
};

CBASED struct SunMap 
{ 
  char               name1[12],        /* primary parameter name      */
                     name2[12];        /* secondary parameter name    */
  char               description[32];  /* description of parameter    */
  uint16_t           typeFlag;         /* extended data typ flag field*/
  char               typeIDS[2];       /* ID and size of data type    */
  char               units[8];         /* parameter units             */
  uint32_t           seqNum,           /* recording sequence number   */
                     blockPtr[2];      /* file offsets to data blocks */
};

CBASED struct FileInfo          /* new for v4.00               */ 
{ 
  uint32_t           fileSig,          /* file signature word         */
                     refTime[6];       /* GMT at time zero            */
  struct TimeTag     refGMT;           /* GMT at time zero (TimeTag)  */
  uint32_t           items,            /* number of items in file     */
                     points,           /* points in data blocks       */
                     isDynamic;        /* file is realtime capable    */
  float              cutoff;           /* engine cutoff time          */
  char              *title,            /* file title                  */
                    *comment,          /* header comment              */
                    *serialNum,        /* engine serial number        */
                    *testStand,        /* test stand name             */
                    *testPosition,     /* test position               */
                    *testType,         /* type of test                */
                    *testNumber;       /* number of test              */
   union            /* for v4.25 backwards compatibility with codes compiled/linked with <= v4.24 */
   { 
     char           *certStatus;       /* used only for PCG notation  */
     FILE           **filePtr;         /* share position to maintain file pointer of real-time files */
   };                                  /* v4.3x, should create dedicated member for forward versions.   */
};
#define FILE_PTR(a) (a ? a[1] : NULL)  // first element used to hold null for codes that access certStatus (creates empty string)

CBASED struct ParmInfo          /* new for v4.00               */
{ 
  char              *name[3];          /* pri/sec/ext parameter names */
  char              *units;            /* parameter units             */
  char              *description[2];   /* description of parameter    */
  char               typeIDS[2];       /* ID and size of data type    */
  char               extendedType;     /* extended data typ flag field*/
  float              rate;             /* data rate (samples/second)  */
  uint32_t           seqNum;           /* recording sequence number   */
  uint32_t           blockPtr[2];      /* file offsets to data blocks */
  uint32_t           blockSize[2];     /* calculated LZH block sizes  */
};

//   this is the format of the variably sized map entries (for reference only)  
//  CBASED DLLEXP struct VarNomenMap  (map entry for SUN_ COMMONTIME/MULTITIME  
//  {                                                                           
//    char               n1Len,name1[n1Len],      // primary parameter name       
//                       n2Len,*name2[n2Len],     // secondary parameter name    
//                       dLen,*description[dLen], // description of parameter    
//                       uLen,*units[uLen];       // parameter units             
//    char               typeIDS[2];              // ID and size of data type    
//    uint32_t           seqNum,                  // recording sequence number   
//                       blockPtr[2];             // file offsets to data blocks 
//  };                                                                          

/* MultiTimeLookup struct is used only for access to SUN_MULTITIME    */
/*   it maintains lookup tables for multi-blocked files (real-time)   */
CBASED struct MultiTimeLookup
{ 
  uint32_t           addrStackSz;      /* size of block address stack */
  uint32_t           addrStackPtr;     /* block address stack pointer */
  char               indexNomen[256];  /* buffer for nomen override   */
  union 
  { 
    struct pidHeader   *p32;           /* standard 32 bit version     */
    struct pidHeader64 *p64;           /* 64 bit address version      */
  } pHead;                             /* parameter block info struct */
};

/* ****************************************************************** */
/* pre-typed pointers to parameter data buffer                        */

union ParmData
{ 
  uint8_t            *c;        /* generic byte pointer 1 byte        */
  float              *f;        /* generic float pointer 4 byte       */
  double             *d;        /* generic double pointer 8 byte      */
  uint16_t           *u2;       /* generic uint16_t  2 byte           */
  uint32_t           *u4;       /* generic int32_t 4 bytes            */
  int16_t            *i2;       /* generic int16_t 2 bytes            */
  int32_t            *i4;       /* generic int32_t 4 bytes            */
  int64_t            *i8;       /* generic int64_t 8 bytes            */
  uint64_t           *u8;       /* generic uint64_t 8 bytes           */
  struct TimeTag     *TimeTag;  /* time tag data structure            */
};    

/* ****************************************************************** */
/* return structures of file access functions                         */

CBASED struct SunParm
{
  struct SunFile      *parent;         /* parent file of parameter    */
  char                *name,           /* parameter name              */
                      *units,          /* parameter units             */
                      *descr;          /* parameter description       */
  struct TimeTag      *refGMT;         /* access reference GMT        */
  double              *refTIME;        /* access reference Time value */
  union  ParmData      data;           /* pointers to data buffer     */
  float               *time;           /* time points                 */
  char                 typeIDS[2];     /* buffer type/size ID string  */
  uint16_t             typeID;         /* type ID of data buffer      */
  uint32_t             points,         /* number of points retrieved  */
                       seqNum;         /* recording sequence number   */
  struct SunParm      *timeStamp;      /* data timestamps (file index)*/
  uint32_t             tStart,tStop;   /* point offsets timeStamp     */
  uint32_t             oMin,oMax;      /* point offsets to min/max    */
  double               vMin,vMax;      /* values of parameter min/max */
  float                vTshift;        /* offset value of time shift  */
  void                *reserved;       /* reserved, not for end user  */
  uint32_t             mapPos;         /* position of parm in file map*/
  uint32_t             allocSz;        /* actual time/data alloc size */
  float                rate;           /* data rate (samples/second)  */
  void                *ptr;            /* user application defineable */
  uint32_t             flag1,flag2;    /* user application defineable */
  uint32_t             flag3,flag4;    /* user application defineable */
  uint32_t             flag5,flag6;    /* user application defineable */
};

/* ****************************************************************** */

CBASED struct SunFile
{ 
  char                *name;           /* name and path of data file  */
  struct SunHeader    *header;         /* file header data            */
  struct FileInfo     *fileInfo;       /* generic file infomation     */
  struct ParmInfo     *parmInfo;       /* parameter mapping and nomen */
  struct SunParm      *TIMEindex;      /* master time data for file   */
  struct SunParm      *GMTindex;       /* master GMT data for file    */
  float                rate;           /* data rate of file (@ tzero) */
  uint32_t             commentSZ;      /* size of comment block       */
  uint32_t             commentOffset;  /* byte offset of comment block*/
  struct Event         events;         /* pointers to file events     */
  void                *reserved;       /* reserved, not for end user  */
  void                *ptr;            /* user application defineable */
  uint32_t             flag1,flag2;    /* user application defineable */
  uint32_t             flag3,flag4;    /* user application defineable */
  uint32_t             flag5,flag6;    /* user application defineable */
};

/* ****************************************************************** */

struct Revision
{
  struct TimeTag refGMT;     /* existing file header reference time       */
  double *appendAfter;       /* last time stamp of existing data in file  */
  uint16_t *indexType; /* existing timestamp type of parameter */
  uint16_t *dataType;  /* existing data type of parameter      */
};

CBASED struct SunWrt 
{ 
  uint32_t fileSig;
  FILE *filePtr;
  int32_t writeMode;
  char name[256];
  uint32_t nPoints,nItems,currentMap,mapEntrySize;
  uint32_t blockPoints[2],commentSize;
  uint32_t nextMapEntryPos;
  uint32_t commentOffset;
  union { uint32_t *p32; int64_t *p64; } currentBlockAddr;
  union { uint32_t p32; int64_t p64;   } nextParmHeader;
  char **currentParmSet;
  struct Revision revision; /* this member only valid in revise mode */
};

/* macro to test for MULTITIME format files */
#define IS_MULTITIME_FILE(a) ((a->fileInfo->fileSig == SUN_MULTITIME) || (a->fileInfo->fileSig == SUN_MULTITIME64) || (a->fileInfo->fileSig == SUN_MULTITIME_SRT) || (a->fileInfo->fileSig == SUN_MULTITIME64_SRT))
#define IS_MULTITIME_SIG(a) ((a == SUN_MULTITIME) || (a == SUN_MULTITIME64) || (a == SUN_MULTITIME_SRT) || (a == SUN_MULTITIME64_SRT))

/* ****************************************************************** */
/* read lib function prototypes                                       */

/* open and close files  */
CBASED DLLEXP struct SunFile * CALLCONV openSunFile(char *SunFileName);
CBASED DLLEXP int CALLCONV freeSunFile(struct SunFile *);
CBASED DLLEXP int CALLCONV refreshSunFile(struct SunFile *SunFile);

/* retrieve and free parameter data  */
/* v3.10 mod, parameter added, use _ for old version */
CBASED DLLEXP struct SunParm * CALLCONV readSunParm(struct SunFile *,char *ParmName, double startTime,double stopTime, double *refTIME,
                                                    struct TimeTag *refGMT, uint16_t convertType);
CBASED DLLEXP int CALLCONV appendSunParm(struct SunParm *base, struct SunParm *append, int32_t retainScale);
CBASED DLLEXP int CALLCONV freeSunParm(struct SunParm *);
CBASED DLLEXP struct SunParm * CALLCONV dupeSunParm(struct SunParm *);

/* library menu functions */
CBASED DLLEXP int CALLCONV installLibMenu(void *pMenu, uint32_t nBaseId);
CBASED DLLEXP int CALLCONV doLibFunction(void *pMenu, uint32_t nFuncId);
CBASED DLLEXP int CALLCONV doLibCommand(const char *strCommand);

/* access information items  */
CBASED DLLEXP uint32_t CALLCONV isSupportedType(const char *fileName);
CBASED DLLEXP const char * CALLCONV accFileTypeDescription(uint32_t fileSig);
CBASED DLLEXP int CALLCONV accStatus(void);
CBASED DLLEXP char * CALLCONV accVersion(void);

/* returns the map index of the given parameter name */
CBASED DLLEXP int CALLCONV findParm(char *parmName,struct SunFile *,int32_t *altname);

/* returns the position of the min/max values from a SunParm object */
CBASED DLLEXP uint32_t CALLCONV dataMin(struct SunParm *sunParm);
CBASED DLLEXP uint32_t CALLCONV dataMax(struct SunParm *sunParm);

/* time conversion functions */
CBASED DLLEXP struct TimeTag * CALLCONV GMT(double *time,struct TimeTag *refGMT,struct TimeTag *timeGMT,int32_t setMicro);
CBASED DLLEXP double * CALLCONV TIME(struct TimeTag *timeGMT, struct TimeTag *refGMT,double *time);

/* allocate a buffer and read file comment block data + free function */
CBASED DLLEXP char * CALLCONV getCommentBlock(struct SunFile *sunFile);
CBASED DLLEXP void CALLCONV freeCommentBlock(char *commentBlock);

/* reverse byte order of data items (ints, floats, doubles, etc) */
CBASED DLLEXP void * CALLCONV flip(void *bytestring,int32_t length);

/* find the offset of a given time stamp  */
CBASED DLLEXP uint32_t CALLCONV TIME_location(double time, float *timearry,uint32_t total_points,int32_t mode);
CBASED DLLEXP uint32_t CALLCONV GMT_location(double tpoint,struct TimeTag *GMT,uint32_t np,struct TimeTag *refGMT,int32_t mode);

/* reset parameter nomenclature items */
CBASED DLLEXP void CALLCONV resetNomen(struct SunParm *parm,char *name,char *units,char *descr);

/* time based parameter data reduction functions */
CBASED DLLEXP uint32_t CALLCONV average(struct SunParm *pid,float timeDelta,float avgTime,int32_t timeStamp);
CBASED DLLEXP uint32_t CALLCONV quantize(struct SunParm *pParm, double qRate, int bSample);

/* memory allocation/deallocation functions for SunParm modification */
CBASED DLLEXP void * CALLCONV mallocSunParmData(uint32_t nBytes);
CBASED DLLEXP void CALLCONV freeSunParmData(void * dStorage);

/* utility funtions **************************************************** */
/* type ID value and ID string lookup functions */
CBASED DLLEXP uint16_t CALLCONV getDT(char *idString);
CBASED DLLEXP char * CALLCONV getDTS(uint16_t typeID);

/* data type conversions */
CBASED DLLEXP void           CALLCONV newType    (void *oPtr, uint16_t oType, void *nPtr, uint16_t nType);
CBASED DLLEXP float          CALLCONV toFloat    (void *vPtr, uint16_t vType);
CBASED DLLEXP double         CALLCONV toDouble   (void *oPtr, uint16_t oType);
CBASED DLLEXP int16_t        CALLCONV toShortInt (void *vPtr, uint16_t vType);
CBASED DLLEXP int32_t        CALLCONV toLongInt  (void *vPtr, uint16_t vType);
CBASED DLLEXP uint16_t       CALLCONV toUShortInt(void *vPtr, uint16_t vType);
CBASED DLLEXP uint32_t       CALLCONV toULongInt (void *vPtr, uint16_t vType);
CBASED DLLEXP struct TimeTag CALLCONV toTimeTag  (void *vPtr, uint16_t vType);
CBASED DLLEXP uint8_t        CALLCONV toByte     (void *vPtr, uint16_t vType);
CBASED DLLEXP uint64_t       CALLCONV toULongLong(void *vPtr, uint16_t vType);
CBASED DLLEXP int64_t        CALLCONV toLongLong (void *vPtr, uint16_t vType);


/* *** internal functions *********************************************  */

/* retrieval of SUN_MULTITIME parameter data */
struct SunParm *readMultiTimeParm( struct SunFile *pSunFile,
                                   char *parmName,
                                   double startTime,double stopTime,
                                   double *refTIME,
                                   struct TimeTag *pRefGMT,
											             uint16_t returnType);

struct SunParm *readMultiTimeParm64( struct SunFile *pSunFile,
                                   char *parmName,
                                   double startTime,double stopTime,
                                   double *refTIME,
                                   struct TimeTag *pRefGMT,
											             uint16_t returnType);

struct SunParm *readMultiTimeBlock(FILE *filePtr,
                                   struct SunFile *pSunFile,
                                   int32_t mapPos,int32_t bAltName,
                                   double startTime,double stopTime,
                                   double *refTIME,
                                   struct TimeTag *pRefGMT,
                                   struct TimeTag *baseReference,
                                   struct pidHeader *pHead,
                                   uint16_t returnType,
                                   uint32_t preAllocSz);

struct SunParm *readMultiTimeBlock64(FILE *filePtr,
                                     struct SunFile *pSunFile,
                                     int32_t mapPos,int32_t bAltName,
                                     double startTime,double stopTime,
                                     double *refTIME,
                                     struct TimeTag *pRefGMT,
                                     struct TimeTag *baseReference,
                                     struct pidHeader64 *pHead,
											               uint16_t returnType,
                                     uint32_t preAllocSz);

int readMultiTimeParmHeader(FILE *filePtr, struct pidHeader *pHead, struct TimeTag *baseGMT, char *nomenBuf);

int readMultiTimeParmHeader64(FILE *filePtr, struct pidHeader64 *pHead, struct TimeTag *baseGMT, char *nomenBuf);

/* rebuilds change-only data rate to full rate */
struct SunParm *makeFullRate(struct SunParm *pParm);

/* retrieve file events from comment block */
int getEvents(struct SunFile *sunFile);

/* retrieve extended nomenclature from comment block */
int getExtendedNomen(struct SunFile *sunFile);

/* retrieve file map and build nomenclature structure */
int retrieveFileMap(FILE *filePtr,struct SunFile *SunFile);

/* convert time stamp data from delta values to running time */
void delt2lin(float *,uint32_t,uint32_t);

/* determine byte positions of data withing file data blocks */
void getBytePos(struct SunFile *SunFile,uint32_t startPos, uint32_t stopPos,uint32_t *strb, uint32_t *stpb,char *dblk, uint32_t DataTypeSize);

/* calculate the compressed block sizes of data */
int calc_blksz(struct ParmInfo *,uint32_t,FILE *);

/* blank out flip function big-endian machines */
#ifdef LITTLE_ENDIAN_PLATFORM
#  define FLIP(b,l) flip(b,l)
#else
#  define FLIP(b,l) b
#endif


/* ****************************************************************** */
/* WRITE library specific functions                                   */
/* ****************************************************************** */

/* retrieve pointer to version string */
CBASED DLLEXP char * CALLCONV wrtVersion(void);

/* retrieve error code of last action */
CBASED DLLEXP int CALLCONV wrtStatus(void);

/* create output file with empty header */
CBASED DLLEXP struct SunWrt * CALLCONV createSunFile(char *fileName,int32_t writeMode,uint32_t commentBlockSZ,uint32_t nItems,uint32_t nPoints,uint32_t formatType);

/* open an existing file for revision/update (MULTITIME only) */
CBASED DLLEXP struct SunWrt * CALLCONV reviseSunFile(char *fileName, int writeMode);

/* get last time written to existing file (valid for revision of MULTITIME only) */
CBASED DLLEXP double * CALLCONV getReviseAppendTime(struct SunWrt *pSunWrt,char *parmName,double *timeBuf);

/* data data type of existing parameter (valid for revision of MULTITIME only) */
CBASED DLLEXP uint16_t * CALLCONV getReviseDataType(struct SunWrt *pSunWrt,char *parmName,uint16_t *iDT);
CBASED DLLEXP uint16_t * CALLCONV getReviseIndexType(struct SunWrt *pSunWrt,char *parmName,uint16_t *iDT);

/* find element offset for parameter in file being revised (valid for revision of MULTITIME only) */
CBASED DLLEXP uint32_t * CALLCONV getReviseParmIndex(struct SunWrt *pSunWrt,char *parmName,uint32_t *parmIndex);

/* insert parameters into file */
CBASED DLLEXP int CALLCONV addSunParm(struct SunWrt *pSunWrt,char *n1,char *n2,
                                      char *u, char *d,int32_t sn, char *dataID,void *dataBuf,
                                      char *indexID, void *indexBuf,uint32_t pCount,
                                      uint8_t options,char *indexNomen);

/* indicate that file (SUN_MULITIME only) has new data added */
CBASED DLLEXP uint32_t CALLCONV showFileUpdated(struct SunWrt *pSunWrt);

/* finish header and close file */
CBASED DLLEXP int CALLCONV closeSunFile(struct SunWrt*);
CBASED DLLEXP int CALLCONV closeSrtFile(struct SunWrt*);

/* comment block text */
CBASED DLLEXP int CALLCONV setCommentText(struct SunWrt*,char *commentText);

/* header info items */
CBASED DLLEXP int CALLCONV h_setCutoff(struct SunWrt*,float cutoffTime);
CBASED DLLEXP int CALLCONV h_setTitle(struct SunWrt*,char *titleText);
CBASED DLLEXP int CALLCONV h_setComment(struct SunWrt*,char *commentText);
CBASED DLLEXP int CALLCONV h_setSerialNum(struct SunWrt*,char *sernumText);
CBASED DLLEXP int CALLCONV h_setTestPos(struct SunWrt*,char *testposText);
CBASED DLLEXP int CALLCONV h_setTestType(struct SunWrt*,char *testtypeText);
CBASED DLLEXP int CALLCONV h_setTestNumb(struct SunWrt*,char *testnumbText);
CBASED DLLEXP int CALLCONV h_setTestStand(struct SunWrt *,char *teststandText); 
CBASED DLLEXP int CALLCONV h_setRefTime(struct SunWrt*,uint32_t yr,uint32_t day,uint32_t hr,uint32_t min,uint32_t sec,uint32_t mil);

/* internal */
/* get parameter block pointer position and value from file map */
int32_t getParmMapBlockPtr(uint32_t fileSig, FILE *filePtr, uint32_t iParm, uint32_t *blockPtr /*[2]*/);

/* convert linear time values to deltas */
void lin2delt(float *indx,uint32_t nPoints);

/* get or set parameter time and data value at specific point offset */

int getValue(struct SunParm *pid, uint32_t point, union ParmData pVal); /* retrieve selected point value as native type */
int setValue(struct SunParm *pid, uint32_t point, float time, union ParmData pVal); /* value must be same type as pid  */

// set an array value directly from double using union parmData/type ID for determining data type
int setValueDirect(void *pDest, uint32_t pos, double val, uint16_t typeID);
// set parameter data array element of any type from any data type 
int setValueDirectX(uint32_t nPos, union ParmData pDestArry, uint16_t destType, void *pSrcValue, uint16_t srcType);

/* data compression function */
unsigned long int lzh_encode(unsigned char *buf,unsigned long int bsize,FILE *out);
/* data de-compression function */
int lzh_decode(unsigned char *buf, unsigned long strb, unsigned long stpb, unsigned long blksize, FILE *filePtr);

#endif

