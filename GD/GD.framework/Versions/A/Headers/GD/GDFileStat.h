/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

//  GDFileStat.h

#ifndef GDMac_GDFileStat_h
#define GDMac_GDFileStat_h

/* \cond DOXYGEN_IGNORE */
#ifndef __has_extension
#define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif
/* \endcond */

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_GDFILESTAT __attribute__((deprecated("Use NSDictionary collection with NSFileAttributeKey instead.")))
#else
#   define DEPRECATE_GDFILESTAT __attribute__((deprecated))
#endif
/* \endcond */

/** \struct GDFileStat
 *
 * @deprecated
 * This structure is deprecated and will be removed in a future release.
 * It is no longer required. Current BlackBerry Dynamics represents information about
 * files and directories in an NSDictionary collection instead of using this structure.
 * See \reflink GDFileManager::attributesOfItemAtPath:error: GDFileManager::attributesOfItemAtPath:error: \endlink for example.
 *
 * Information about a file or directory in the BlackBerry Dynamics secure
 * store.
 *
 * This structure is used to return information about a file or directory in the
 * BlackBerry Dynamics secure store.
 *
 */
typedef struct
{
    /* @deprecated
     * This attribute is deprecated and will be removed in a future release.
     * It is in a deprecated structure, GDFileStat.
     *
     * File size in bytes.
     */
    unsigned long long fileLen __deprecated_msg("Use NSFileSize instead.");
    
    /* @deprecated
     * This attribute is deprecated and will be removed in a future release.
     * It is in a deprecated structure, GDFileStat.
     *
     * Timestamp for when file was last modified.
     */
    unsigned long lastModifiedTime __deprecated_msg("Use NSFileModificationDate instead.");
    
    /* @deprecated
     * This attribute is deprecated and will be removed in a future release.
     * It is in a deprecated structure, GDFileStat.
     *
     * \ss_true for directories, \ss_false for plain files.
     */
    bool isFolder __deprecated_msg("Use NSFileTypeDirectory instead.");
} GDFileStat DEPRECATE_GDFILESTAT;


#endif
