/* Error code/text message table **************************** */
/* valid for version SunAcc 4.2.6.1 */

#ifndef ERRCODES_H

#define ERRCODES_H

/* *************************************************************************** */
/* ERROR CODES *************************************************************** */

enum ERROR_CODES
{ /* READ codes */
  WRTERRFREE=0,     /* no errors                                               */
  ERR_SUCCESS=0,    /* no errors                                               */
  ERR_FILEOPEN,     /* file open error                                         */
  ERR_UNKNOWN_FMT,  /* file has unrecognized format ID                         */
  ERR_MEMALLOC1,    /* alloc fail for file control struct                      */
  ERR_MEMALLOC2,    /* alloc fail for file map struct                          */
  ERR_FILESIG,      /* invalid file signature                                  */
  ERR_MEMALLOC3,    /* alloc fail for for data type conversion                 */
  ERR_MEMALLOC4,    /* alloc fail for time data buffer                         */
  ERR_MEMALLOC5,    /* alloc fail for file header struct                       */
  ERR_MEMALLOC6,    /* alloc fail for file map struct                          */
  ERR_MEMALLOC7,    /* alloc fail for block size array                         */
  ERR_MEMALLOC8,    /* alloc fail for file map storage                         */
  ERR_PIDNOTFOUND,  /* name not found in file map                              */
  ERR_TIMEERROR,    /* time array error                                        */
  ERR_CONVPTR,      /* failed to get pointer to type conversion buffer         */
  ERR_MEMALLOC9,    /* couldn't allocate memory for pid struct                 */
  ERR_MEMALLOC14,   /* failed to allocate memory for data buffer               */
  ERR_NULLFILE,     /* null sun file structure used                            */
  ERR_TIMEREF,      /* parameter retrieval by GMT when GMT not available       */
  ERR_MEMALLOC10,   /* couldn't allocate memory for access reference GMT       */
  ERR_BADREF,       /* attempt to access time using GMT reference, use TIME    */
  ERR_COMREAD,      /* failed to open Sun file to retrieve comment block       */
  ERR_MEMALLOC11,   /* couldn't allocate memory to retrieve comment block      */
  ERR_COMREADFAIL,  /* fread failed reading comment block data                 */
  ERR_MEMALLOC12,   /* couldn't allocate memory for time stamps GMT->TIME      */
  ERR_TIMESLICE,    /* data not available for selected time slice              */
  ERR_REFRESHTIME,  /* time load/re-load attempt during file update            */
  ERR_MEMALLOC15,   /* alloc fail for nomenclature pointers                    */
  /* lzh_decode module error codes */
  ERR_LZWDICT,      /* corrupt dictionary table (currupted file)               */
  ERR_LZWREADFAIL,  /* general file read error                                 */
  /* pcgacc error codes */
  ERR_FILTOPEN,     /* failed to open filter file                              */
  ERR_STREAMFIND,   /* failed to find stream                                   */
  ERR_PARMFIND,     /* failed to find parm in stream                           */
  ERR_MEMALLOC13,   /* memory allocation failed                                */
  ERR_PARMFILT,     /* failed to find parm in file map (filter)                */
  ERR_GMTMEMALLOC,  /* failed to allocate memory for GMT array (SUN_MULTITIME) */
  ERR_FMTNOTSUPP,   /* SUN_MULTITIME64 not supported on this platform          */
  ERR_GMTNOTAVAIL,  /* request for GMT array when data is not available        */
  /* WRITE codes */
  WRTSTRALLOC,      /* structure allocation failed                             */
  WRTFILEXISTS,     /* output file exists                                      */
  WRTOPENERR,       /* failed to open output file                              */
  WRTFWRCOMBLK,     /* failure writing comment block                           */
  WRTFSKCOMBLK,     /* failure seeking to comment block                        */ 
  WRTCLOSERR,       /* failure closing output file                             */
  WRTNOTENOPARMS,   /* not enough parameters written                           */
  WRTINVALIDIDX,    /* invalid file index parameter                            */
  WRTPARMFILRST,    /* failure seeking to EOF/end of data                      */
  WRTHEADERWRT,     /* failure writing header item                             */
  WRTHEADERFSK,     /* failure seeking to header item                          */
  WRTFILEFULL,      /* too many paramter writes attempted                      */
  WRTINVALIDFORMAT, /* invalid file type requested                             */
  WRTMISSINGIDX,    /* UNIVERSAL write missing timestamp                       */
  WRTBUFFERALLOC,   /* failure allocating data buffer                          */
  WRTINVALIDARGS,   /* invalid arguments to function                           */
  WRTSEEKFAILED,    /* file seek() failed                                      */
  WRTNOTVALID,      /* function not valid for file format or mode              */
  WRTZEROPOINTS,    /* attempt to write with zero point count                  */
  ERR_FILETYPE,     /* unrecognized file type                                  */ 
  ERR_NOARCHIVE,    /* archive source not selected -not returned by SunAcc     */
  ERR_FMT_NO_SUP,   /* format not supported on platform                        */
  WRT_READONLY,     /* file is read-only                                       */
  WRT_REVNOMATCH,   /* no matching parameter name for revision                 */
  WRT_NOT_REVISING, /* file is not in revise mode (MULTITIME only)             */
  ERR_LINK_HDR_RD,  /* error reading MULTITIME parameter link header           */
  ERR_UNDEFINED     /* invalid error code (** MUST BE LAST OF THIS SET **)     */

};

/* error strings */
static const char *ERR_STRING[] = 
{   
  "OK",
  "file open error",
  "file format ID unknown",
  "alloc failed for file control struct",
  "alloc failed for file map struct",
  "invalid file signature",
  "alloc failed for for data type conversion",
  "alloc failed for time data buffer",
  "alloc failed for file header struct",
  "alloc failed for file map struct",
  "alloc failed for block size array",
  "alloc failed for file map storage",
  "name not found in file map",
  "time array error",
  "failed to get pointer to type conversion buffer",
  "couldn't allocate memory for pid struct",
  "couldn't allocate memory for data buffer",
  "null sun file structure used",
  "parameter retrieval by GMT when GMT not available",
  "couldn't allocate memory for access reference GMT",
  "attempt to access time using GMT reference, use TIME",
  "failed to open Sun file to retrieve comment block",
  "couldn't allocate memory to retrieve comment block",
  "fread failed reading comment block data",
  "couldn't allocate memory for time stamps GMT->TIME",
  "data not available for selected time slice",
  "time load/re-load attempt during file update",
  "alloc failed for Nomen pointer structure",
  "corrupt dictionary table (currupted file)",
  "general file read error",
  "failed to open filter file",
  "failed to find stream",
  "failed to find parm in stream",
  "memory allocation failed",
  "failed to find parm in file map (filter)",
  "failed to allocate memory for GMT array",
  "SUN_MULTITIME64 not supported on this platform",
  "GMT array requested but not available",
  "structure allocation failed",
  "output file exists",
  "failed to open output file",        
  "failure writing comment block",     
  "failure seeking to comment block",   
  "failure closing output file",
  "not enough parameters written",
  "invalid file index parameter",
  "failure seeking to EOF/end of data",
  "failure writing header item",
  "failure seeking to header item",
  "too many parameter writes attempted",
  "invalid file type requested",
  "MULTI_TIME write missing timestamp",
  "failure allocating data buffer",
  "invalid arguments to function",
  "file seek() failed",
  "function not valid for file format or mode",
  "attempt to write with zero point count",
  "unrecognized file type",
  "no archive source selected",
  "file format not supported on platform",
  "file is read only",
  "no matching parameter name for revision",
  "file not in revise mode",
  "error reading MULTITIME parameter link header",
  "undefined error code"
};

/* a macro to retrieve error text from error code */
#define GetAccessErrorText(a) ERR_STRING[abs(a) < ERR_UNDEFINED ? abs(a) : ERR_UNDEFINED]

#endif
