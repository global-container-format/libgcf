#ifndef GCF_API_H
#define GCF_API_H

#include <gcf/defs.h>

#ifdef GCF_BUILD_DLL
    #ifdef GCF_SOURCE
        #define GCFAPI __attribute__((dllexport))
    #else /* GCF_SOURCE */
        #define GCFAPI __attribute__((dllimport))
    #endif /* GCF_SOURCE */
#else /* GCF_BUILD_DLL */
    #define GCFAPI
#endif /* GCF_BUILD_DLL */

#define GCFATTR

#endif /* !GCF_API_H */
