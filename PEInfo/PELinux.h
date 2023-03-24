#ifndef __PE_LINUX__H__
#define __PE_LINUX__H__

#ifndef __linux__
    #error "non linux env error"
#else
    #include <iostream>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>

    #ifndef VOID
    #define VOID void
    typedef char CHAR;
    typedef short SHORT;
    typedef long LONG;
    typedef wchar_t WCHAR;
    #endif

    typedef unsigned short WORD;
    typedef unsigned int DWORD;
    typedef unsigned int DWORD;
    typedef const WCHAR *LPCTSTR;
    typedef struct _IMAGE_IMPORT_BY_NAME {
        WORD    Hint;
        CHAR   Name[1];
    } IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

    typedef struct _IMAGE_EXPORT_DIRECTORY {
        DWORD   Characteristics;
        DWORD   TimeDateStamp;
        WORD    MajorVersion;
        WORD    MinorVersion;
        DWORD   Name;
        DWORD   Base;
        DWORD   NumberOfFunctions;
        DWORD   NumberOfNames;
        DWORD   AddressOfFunctions;     // RVA from base of image
        DWORD   AddressOfNames;         // RVA from base of image
        DWORD   AddressOfNameOrdinals;  // RVA from base of image
    } IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

    typedef struct _IMAGE_IMPORT_DESCRIPTOR {
        union {
            DWORD   Characteristics;            // 0 for terminating null import descriptor
            DWORD   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
        } DUMMYUNIONNAME;
        DWORD   TimeDateStamp;                  // 0 if not bound,
                                                // -1 if bound, and real date\time stamp
                                                //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
                                                // O.W. date/time stamp of DLL bound to (Old BIND)

        DWORD   ForwarderChain;                 // -1 if no forwarders
        DWORD   Name;
        DWORD   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
    } IMAGE_IMPORT_DESCRIPTOR;

#endif


#endif
