/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#ifndef __GD_FILE_SYSTEM_H__
#define __GD_FILE_SYSTEM_H__

#import <Foundation/Foundation.h>
#import "GDCReadStream.h"
#import "GDCWriteStream.h"
#import "GDFileStat.h"
#import "GDPersistentStoreCoordinator.h"

/* \cond DOXYGEN_IGNORE */
#ifndef __has_extension
#define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif
/* \endcond */

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_GDFILESYSTEMERROR __attribute__((deprecated("Use GDFileManagerErrorDomain instead.")))
#else
#   define DEPRECATE_GDFILESYSTEMERROR __attribute__((deprecated))
#endif
/* \endcond */

/** @deprecated
 * \defgroup gdfilesystemerrordomain GDFileSystem Error Domain
 * These constants can be used when handling errors returned by
 * \ref GDCReadStream and \ref GDCWriteStream
 * functions.\ The constant values in the GDFileSystemErr enumeration also
 * occur in the \link gdfilemanagererrordomain GDFileManagerErrorType\endlink
 * enumeration.\ The labels in the GDFileManagerErrorType enumeration are
 * similar but follow a different convention.\ The semantics are the same.
 *
 * \{
 */

/** Error domain for file system errors.
 */
extern NSString* const GDFileSystemErrorDomain DEPRECATE_GDFILESYSTEMERROR;

typedef NS_ENUM(NSInteger, GDFileSystemErr)
{
    /** Specified path does not exist.
     */
    GDFileSystemErrPathDoesntExist __deprecated_enum_msg("Use GDFileManagerErrorPathDoesNotExist instead.") = 100,

    /** IO error occurred during a low-level read or write operation.
     */
    GDFileSystemErrIOError __deprecated_enum_msg("Use GDFileManagerErrorIOError instead.") = 101,

    /** Permissions error when attempting to access part of the filesystem.
     */
    GDFileSystemErrPermissionError __deprecated_enum_msg("Use GDFileManagerErrorPermissionError instead.") = 102,

    /** Attempt to delete a directory that was not empty.
     */
    GDFileSystemErrDirNotEmpty __deprecated_enum_msg("Use GDFileManagerErrorDirectoryNotEmpty instead.")   = 103,

    /** An unknown error occured.
     */
    GDFileSystemErrUnknownError __deprecated_enum_msg("Use GDFileManagerErrorUnknownError instead.")  = 500
};

/** \}
 */

#undef DEPRECATE_GDFILESYSTEMERROR
#endif /* __GD_FILE_SYSTEM_H__ */
