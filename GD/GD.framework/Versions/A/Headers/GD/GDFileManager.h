/*
 * (c) 2018 BlackBerry Limited. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "GDCReadStream.h"
#import "GDCWriteStream.h"
#import "GDPortability.h"

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATED_IN_NSFILEMANAGER __attribute__((deprecated("Deprecated in NSFileManager. Do not use.")))
#else
#   define DEPRECATED_IN_NSFILEMANAGER __attribute__((deprecated))
#endif

#if __has_extension(attribute_unavailable_with_message)
#   define UNAVAILABLE_IN_GDFILEMANAGER __attribute__((unavailable("Unavailable in GDFileManager. Do not use.")))
#else
#   define UNAVAILABLE_IN_GDFILEMANAGER __attribute__((deprecated))
#endif

#if (defined(__MAC_10_12) && defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= __MAC_10_12)\
 || (defined(__IPHONE_10_0) && defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0)
#   define GD_NSFileAttributeKey NSFileAttributeKey
#   define GD_NSURLResourceKey NSURLResourceKey
#else
#   define GD_NSFileAttributeKey NSString *
#   define GD_NSURLResourceKey NSString *
#endif

/**
 * \defgroup gdfilemanagererrordomain GDFileManager Error Domain
 * These constants can be used when handling errors returned by
 * \ref GDFileManager, \ref GDFileHandle, \ref GDCReadStream, and
 * \ref GDCWriteStream functions.
 *
 * \{
 */

GD_NS_ASSUME_NONNULL_BEGIN

/** Error domain for file system errors.
 */
extern NSString* const GDFileManagerErrorDomain;

typedef NS_ENUM(NSInteger, GDFileManagerErrorType)
{
    /** Specified path does not exist.
     */
    GDFileManagerErrorPathDoesNotExist = 100,
    
    /** Input or output error occurred during a low-level read or write
     *  operation.
     */
    GDFileManagerErrorIOError         = 101,
    
    /** Permissions error when attempting to access part of the filesystem.
     */
    GDFileManagerErrorPermissionError = 102,
    
    /** Attempt to delete a directory that was not empty.
     */
    GDFileManagerErrorDirectoryNotEmpty     = 103,
    
    /** There was a problem with the path.
     */
    GDFileManagerErrorPathError  = 104,
    
    /** Attempt to access "Ubiquitous" file or directory, i.e.\ one that is in
     *  "cloud" storage.
     */
    GDFileManagerErrorICloudNotSupported  = 105,
    
    /** Specified path already exists.
     */
    GDFileManagerErrorPathAlreadyExists  = 106,
    
    /** Unable to delete backup.
     */
    GDFileManagerErrorDeleteBackupError = 107,
    
    /** Unable to preserve metadata from original file.
     */
    GDFileManagerErrorPreservingMetadata = 108,
    
    /** The URL passed in was not a valid file URL.
     */
    GDFileManagerErrorInvalidFileURL = 109,
    
    /** Attempt to utilize unsupported interface.
     */
    GDFileManagerErrorNotSupported  = 110,
    
    /** An unknown error occured.
     */
    GDFileManagerErrorUnknownError    = 500
};

/** \}
 */

/** Secure File System.
 * 
 * This class is a subclass of the native <tt>NSFileManager</tt> class, for
 * access to the BlackBerry Dynamics secure file system.
 * 
 * <h2>Secure File System</h2>
 * The secure file system is part of the BlackBerry Dynamics Secure
 * Storage feature.
 *
 *  \htmlonly <div class="bulletlists"> \endhtmlonly
 * For applications, the BlackBerry Dynamics secure file system behaves like the
 * default file system, with the following differences.
 * - All data within the secure file system is stored on the device
 *   in an encrypted form.
 * - Directory and file names are also encrypted.
 * - The secure file system cannot be accessed until BlackBerry Dynamics
 *   authorization processing is complete, see under  \reflink GDiOS::authorize: authorize (GDiOS)\endlink.
 * .
 *
 * Every operating system has a maximum supported length for the names of files
 * and directories. For example, iOS 11 supports a maximum length of 255 bytes,
 * given by the <tt>NAME_MAX</tt> constant. The encrypted form of the name of a
 * directory or file will typically be longer than the plaintext, and it is the
 * encrypted form that must be within the limits supported by the operating
 * system. This means that names in the secure file system have a shorter
 * effective maximum supported length. It isn't possible to give a simple
 * maximum but the following should be expected.
 * - Names that are 16 bytes or longer will be increased by a factor of
 *   4:3 approximately.
 * - Shorter names will be increased to a length of 20 bytes approximately.
 * .
 * 
 * Encryption and decryption is transparent to the application code:
 * - The application passes its data to a file writing interface. The BlackBerry
 *   Dynamics runtime encrypts the data and stores it on the device.
 * - When a file-reading interface is utilized, the runtime decrypts and returns
 *   the data.
 * - Path access interfaces accept plaintext parameters for directory and file
 *   names. The runtime encrypts the parameter values in order to create paths
 *   in the secure store.
 * - Directory and file names provided as return values are plaintext. The
 *   runtime decrypts paths in the secure store in order to generate the return
 *   values.
 * .
 *
 * The encryption method used by the BlackBerry Dynamics runtime generally
 * requires that the user has entered a security password, from which an
 * encryption key is derived.
 *  \htmlonly </div> \endhtmlonly
 *
 * 
 *  \htmlonly <div class="bulletlists"> \endhtmlonly
 * <h2>Usage</h2>
 * This class is a subclass of the native <tt>NSFileManager</tt> class. It
 * should be easy to replace references to <tt>NSFileManager</tt> with
 * references to <tt>GDFile</tt><tt>Manager</tt>, in order to convert
 * application code that utilizes the native file system into code that utilizes
 * the secure file system.
 *
 * The differences between this class and <tt>NSFileManager</tt> are:
 * - Write access is limited to the secure store.
 * - There is no access to "Ubiquitous" files and directories, i.e. no access to
 *   items in "cloud" storage.
 * - There is no access to native application group containers, which aren't
 *   secured by BlackBerry Dynamics. The
 *   <tt>containerURLForSecurityApplicationGroupIdentifier:</tt> function
 *   returns the same value as the base class would for an invalid group
 *   identifier.
 * - The secure file system doesn't have a designated temporary directory. The
 *   <tt>temporaryDirectory</tt> property mustn't be used to create paths.
 * - File Provider extensions aren't supported. The
 *   <tt>getFileProviderServicesForItemAtURL:completionHandler:</tt> interface
 *   always fails.
 * - Returned <tt>NSURL</tt> values for locations within the secure file system
 *   can only be utilized by BlackBerry Dynamics programming interfaces, see
 *   below.
 * - Returned <tt>NSString</tt> values for paths within the secure file system
 *   can only be utilized by BlackBerry Dynamics programming interfaces, see
 *   below.
 * - Error codes could be in the specific <tt>GDFil</tt><tt>eManager</tt> error
 *   domain, or could be general <tt>NS</tt> error codes.
 * .
 *
 * The <tt>NSURL</tt> values returned by functions in this class can only be
 * used to access files and directories by BlackBerry Dynamics programming
 * interfaces. They cannot be used by native interfaces, such as
 * <tt>NSInputStream</tt> nor <tt>stringWithContentsOfURL:</tt> for example.
 *
 * The <tt>NSString</tt> values for paths returned by functions in this
 * class can only be used to access files and directories by BlackBerry Dynamics
 * programming interfaces. They cannot be used by native interfaces, such as
 * <tt>stringByResolvingSymlinksInPath</tt>, nor
 * <tt>dataWithContentsOfFile:</tt> for example.
 *
 * To read and write files in the secure store, use functions in this class
 * or in one of the other BlackBerry Dynamics programming interfaces. For
 * example:
 * - \link GDFileManager::createFileAtPath:contents:attributes:
 *   createFileAtPath:\endlink
 *   or
 *   \link GDFileManager::getWriteStream:appendmode:error:
 *   getWriteStream:\endlink
 *   to write.
 * - \link GDFileManager::contentsAtPath: contentsAtPath:\endlink
 *   or
 *   \link GDFileManager::getReadStream:error: getReadStream:\endlink
 *   to read.
 * - The <tt>GDFileHandle</tt> class.
 * - The BlackBerry Dynamics C Language programming interface.
 * .
 * Don't use file-reading and -writing functions in the native <tt>NS</tt>
 * classes to access files in the secure store.
 *
 * Best practice for building paths for use with this class is as follows. First
 * use an <tt>NSSearchPathDirectory</tt> value to generate a path prefix, for
 * example by calling the <tt>NSSearchPathForDirectoriesInDomains:</tt>
 * function. Then append to the prefix, for example by calling the
 * <tt>NSString</tt> <tt>stringByAppendingPath:</tt> function or the
 * <tt>NSURL</tt> <tt>URLByAppendingPathComponent</tt> function.
 * 
 * The functions in this API utilize <tt>NSError</tt> in a conventional way. Function calls accept as a parameter the location of a pointer to <tt>NSError</tt>, i.e. a pointer to a pointer, with type <tt>NSError**</tt>. The location may be <tt>nil</tt>. If the location isn't <tt>nil</tt>, and an error occurs, the BlackBerry Dynamics runtime overwrites the pointer at the specified location with the address of an object that describes the error that occurred.
 *
 * The documentation of this class includes descriptions for functions and
 * other elements of the programming interface that:
 * - Have different semantics to the corresponding elements in the base class.
 * - Are not present in the base class.
 * .
 * Elements of the base class that are overriden, but have the same semantics,
 * are listed in the documentation of this class, but not described further.
 * Elements of the base class that aren't overriden are not listed. See the
 * documentation of the base class, <tt>NSFileManager</tt>, for full
 * descriptions.
 *  \htmlonly </div> \endhtmlonly
 *
 * @see <a
            href="https://developer.apple.com/reference/foundation/nsfilemanager"
            target="_blank"
        >NSFileManager class reference</a> on the apple.com developer website.
 * @see <a
            href="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/ErrorHandlingCocoa/ErrorHandling/ErrorHandling.html"
            target="_blank"
        >Error Handling Programming Guide</a
        > on the apple.com developer website.
 * @see <a
            href="https://developer.apple.com/reference/foundation/1414224-nssearchpathfordirectoriesindoma?language=objc"
            target="_blank"
        >NSSearchPathForDirectoriesInDomains reference documentation</a
        > on the apple.com developer website.
 * @see \ref gdfilemanagererrordomain
 * @see  \reflink sqlite Secure SQL Database API\endlink
 * @see \ref GDPersistentStoreCoordinator
 * @see \ref GDFileHandle 
 * @see \ref capilist
  *
 *  <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Simple conversion to secure file system</h3>
 * The following code snippets illustrate simple conversion of a function that
 * uses the native file system to use the secure files system instead.
 *
 * The second function in the snippet is the original; the third is the
 * converted version. The changed line is flagged with a comment. The first
 * function in the snippet is a utility that is used by both the original and
 * the converted function, with no changes.
 * \code
 * - (NSString *)documentsFolderPathForFileNamed:(NSString *)fileName
 * {
 *     NSArray *paths =
 *     NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
 *                                         NSUserDomainMask,
 *                                         YES);
 *     NSString *documentsDirectory = [paths objectAtIndex:0];
 *     return [documentsDirectory stringByAppendingPathComponent:fileName];
 * }
 * 
 * - (BOOL)writeMyFile
 * {
 *     NSString *text = @"Text that is not protected.";
 *     NSData *fileData = [text dataUsingEncoding:NSUTF8StringEncoding];
 *     
 *     BOOL fileCreated =
 *     [[NSFileManager defaultManager]
 *      createFileAtPath:[self
 *                        documentsFolderPathForFileNamed:@"MyFile.txt"]
 *      contents:fileData
 *      attributes:nil];
 *     return fileCreated;
 * }
 * 
 * - (BOOL)writeMyFileGD
 * {
 *     NSString *text = @"Text to be protected by BlackBerry Dynamics.";
 *     NSData *fileData = [text dataUsingEncoding:NSUTF8StringEncoding];
 *
 *     // Following assignment uses GDFileManager instead of NSFileManager.
 *     BOOL fileCreated =
 *     [[GDFileManager defaultManager]
 *      createFileAtPath:[self
 *                        documentsFolderPathForFileNamed:@"MyFile.txt"]
 *      contents:fileData
 *      attributes:nil];
 *     return fileCreated;
 * }
 * \endcode
 */
@interface GDFileManager : NSFileManager <NSStreamDelegate>
{

}

+ (GDFileManager *)defaultManager;

/** Open a file that is in the secure store, for reading.
 * 
 * Call this function to open a file in the secure store for reading. Files in
 * the secure store are encrypted on the device; this function provides access
 * to decrypted data.
 *
 * @param filePath <tt>NSString</tt> of the path, within the secure store, that
 *                 represents the file to be opened.
 * 
 * @param error For returning an <tt>NSError</tt> object if an error occurs. If <tt>nil</tt>, no object will be returned.
 *
 * @return <tt>GDCReadStream</tt> object from which the file's data can be read,
 *         or a null pointer if the file could not be opened.
 */
+ (GD_NSNULLABLE GDCReadStream*) getReadStream:(NSString*)filePath error:(NSError** ) error;

/** Open a file in the secure store, for writing.
 * 
 * Call this function to create a new file in the secure store, or to open an
 * existing file for writing. Files in the secure store are encrypted on the
 * device; data written to the stream returned by this function will be
 * encrypted, transparently to the application.
 *
 * If a file already exists at the specified path, the file can either be
 * appended to, or overwritten.
 *
 * @param filePath <tt>NSString</tt> of the path, within the secure store, that
 *                 represents the file to be opened.
 * 
 * @param flag Selects the action to take if a file already exists at the path.
 *             <tt>YES</tt> to append to the file, or <tt>NO</tt> to overwrite.
 * 
 * @param error For returning an <tt>NSError</tt> object if an error occurs. If <tt>nil</tt>, no object will be returned.
 *
 * @return <tt>GDCWriteStream</tt> object to which data can be written, or a
 *         null pointer if the file could not be opened.
 */
+ (GD_NSNULLABLE GDCWriteStream*) getWriteStream:(NSString*)filePath appendmode:(BOOL) flag error:(NSError**) error;

/** Get the absolute encrypted path of a file within the secure store.
 * 
 * This function returns the encrypted path for a file or directory within the
 * secure store. The principal usage for this function is to provide a path that
 * is compatible with the SQL ATTACH command.
 *
 * @param filePath <tt>NSString</tt> of the path, within the secure store, that
 *                 represents the item for which the encrypted path is required.
 *
 * @return <tt>NSString</tt> containing the encrypted path.
 */
+ (GD_NSNULLABLE NSString*)getAbsoluteEncryptedPath:(NSString*)filePath;

/** Convert a path for use with the BlackBerry Dynamics POSIX-level C language
 *  programming interface.
 *
 * Call this function to convert a path so that it can be used with the
 * BlackBerry Dynamics POSIX-level C language programming interface.
 *
 * Paths used with this class, <tt>GDFile</tt><tt>Manager</tt>, have a different
 * root to the file access functions in the POSIX-level C programming interface.
 * This is because this class is an <tt>NSFileManager</tt> subclass. This
 * function maps a path with the <tt>GDFile</tt><tt>Manager</tt> root to a
 * path with the POSIX-level root.
 *
 * The <tt>pathForManagerFromCPath:</tt> and <tt>pathForCFromManagerPath:</tt>,
 * functions facilitate inter-operation of the programming interfaces. For
 * example, a file could be created with POSIX-level C functions, then read back
 * with functions in this class.
 * 
 * @param filePath <tt>NSString</tt> containing a path for use with this class.
 *
 * @return <tt>NSString</tt> containing the converted path.
 * @see \ref capi.
 */
+ (GD_NSNULLABLE NSString*)pathForCFromManagerPath: (NSString*) filePath;

/** Convert a path from the BlackBerry Dynamics POSIX-level C language
 *  programming interface.
 * 
 * Call this function to convert a path from the BlackBerry Dynamics
 * POSIX-level C programming interface.
 * 
 * Paths used with this class, <tt>GDFile</tt><tt>Manager</tt>, have a different
 * root to the file access functions in the POSIX-level C programming interface.
 * This is because this class is an <tt>NSFileManager</tt> subclass. This
 * function maps a path with the POSIX-level root to a path with the
 * <tt>GDFile</tt><tt>Manager</tt> root.
 *
 * The <tt>pathForManagerFromCPath:</tt> and <tt>pathForCFromManagerPath:</tt>,
 * functions facilitate inter-operation of the programming interfaces. For
 * example, a file could be created with POSIX-level C functions, then read back
 * with functions in this class.
 * 
 * @param filePath <tt>NSString</tt> containing a path for use with the POSIX-level C
 *                 programming interface.
 *
 * @return <tt>NSString</tt> containing the converted path.
 *
 * @see \ref capi.
 */
+ (GD_NSNULLABLE NSString*)pathForManagerFromCPath: (NSString*) filePath;

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_EXP_LOGFILE_TO_DOC_FOLDER __attribute__((deprecated("Will be removed.")))
#else
#   define DEPRECATE_EXP_LOGFILE_TO_DOC_FOLDER __attribute__((deprecated))
#endif

/** Dump BlackBerry Dynamics logs out to an accessible folder (deprecated).
 *
 * @deprecated This function is deprecated and will be removed in a future
 * release. Use the programming interface in the GDLogManager class to upload
 * log files instead.
 *
 * Call this function to create a dump of BlackBerry Dynamics activity
 * logs. The logs will be dumped to a file that is outside the secure store, in
 * the
 * Documents folder.
 * The file will not be encrypted.
 * @param error For returning an <tt>NSError</tt> object if an error occurs. If <tt>nil</tt>, no object will be returned.
 *
 * The log file can be copied from the device in the normal way, and sent to
 * BlackBerry to assist in support analysis.
 */

+ (BOOL)exportLogFileToDocumentsFolder:(NSError**)error DEPRECATE_EXP_LOGFILE_TO_DOC_FOLDER;

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_UPLOAD_LOGS __attribute__((deprecated("Use startUpload in GDLogManager instead.")))
#else
#   define DEPRECATE_UPLOAD_LOGS __attribute__((deprecated))
#endif

/** Upload BlackBerry Dynamics logs to BlackBerry (deprecated).
 * 
 * @deprecated This function is deprecated and will be removed in a
 * future release. Use \reflink GDLogManager::startUpload GDLogManager::startUpload\endlink instead.
 *
 * Call this function to upload BlackBerry Dynamics activity logs
 * for support purposes. The logs will be uploaded to a server in the BlackBerry
 * Network Operation Center (NOC).
 * 
 * Upload takes place in background and is retried as necessary.
 * This function returns immediately.
 * @param error For returning an <tt>NSError</tt> object if an error occurs. If <tt>nil</tt>, no object will be returned.
 *
 * BlackBerry support staff have access to the server to which log files are
 * uploaded, and can use the data for support analysis. This function can be
 * used to upload logs even if authorization has failed. The end user's
 * enterprise email address will be needed by support staff, to identify
 * uploaded files. If authorization has failed or been cancelled without an
 * email address being successfully entered no logs will be uploaded.
 */

+ (BOOL)uploadLogs:(NSError**) error DEPRECATE_UPLOAD_LOGS;

// Working with directories.

- (GD_NSNULLABLE NSURL *)URLForDirectory:(NSSearchPathDirectory)directory inDomain:(NSSearchPathDomainMask)domain appropriateForURL:(GD_NSNULLABLE NSURL *)url create:(BOOL)shouldCreate error:(NSError **)error;

- (GD_NSArray(NSURL *) *)URLsForDirectory:(NSSearchPathDirectory)directory inDomains:(NSSearchPathDomainMask)domainMask;

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @return <tt>nil</tt> always.
 */
- (NSURL *)containerURLForSecurityApplicationGroupIdentifier:(NSString *)groupIdentifier;

- (GD_NSNULLABLE GD_NSArray(NSURL*)*)contentsOfDirectoryAtURL:(NSURL *)url includingPropertiesForKeys:(GD_NSNULLABLE GD_NSArray(GD_NSURLResourceKey)*)keys options:(NSDirectoryEnumerationOptions)mask error:(NSError **)error;

- (GD_NSNULLABLE GD_NSArray(NSString*)*)contentsOfDirectoryAtPath:(NSString *)path error:(NSError **)error;

- (GD_NSNULLABLE GD_NSDirectoryEnumerator(NSURL *) *)enumeratorAtURL:(NSURL *)url includingPropertiesForKeys:(GD_NSNULLABLE GD_NSArray(GD_NSURLResourceKey) *)keys options:(NSDirectoryEnumerationOptions)mask errorHandler:(GD_NSNULLABLE BOOL (^)(NSURL * url, NSError * error))handler;

- (GD_NSNULLABLE GD_NSDirectoryEnumerator(NSString *) *)enumeratorAtPath:(NSString *)path;

- (GD_NSNULLABLE GD_NSArray(NSString*) *)subpathsOfDirectoryAtPath:( NSString *)pathRaw error:(NSError **)error;

- (GD_NSNULLABLE GD_NSArray(NSString*) *)subpathsAtPath:( NSString *)path;

// Creating and deleting items.

- (BOOL)createDirectoryAtURL:( NSURL *)url withIntermediateDirectories:(BOOL)createIntermediates attributes:(GD_NSNULLABLE GD_NSDictionary(NSString*, id)*)attributes error:(NSError **)error;

- (BOOL)createDirectoryAtPath:( NSString*)path withIntermediateDirectories:(BOOL) createIntermediates attributes:(GD_NSNULLABLE GD_NSDictionary(NSString*, id)*)attributes error:(NSError **)error;

- (BOOL)createFileAtPath:( NSString *)path contents:(GD_NSNULLABLE NSData *)contents attributes:(GD_NSNULLABLE GD_NSDictionary(NSString*, id)*)attributes;

- (BOOL)removeItemAtURL:( NSURL *)URL error:(NSError **)error;

- (BOOL)removeItemAtPath:( NSString*)filePath error:(NSError **)error;

- (BOOL)replaceItemAtURL:( NSURL *)originalItemURL withItemAtURL:( NSURL *)newItemURL backupItemName:(GD_NSNULLABLE NSString *)backupItemName options:(NSFileManagerItemReplacementOptions)options resultingItemURL:(NSURL * GD_NSNULLABLE_POINTER * GD_NSNULLABLE_POINTER)resultingURL error:(NSError **)error;

// Moving and copying items.

- (BOOL)copyItemAtURL:( NSURL *)srcURL toURL:( NSURL *)dstURL error:(NSError **)error;

- (BOOL)copyItemAtPath:( NSString *)srcPath toPath:( NSString *)dstPath error:(NSError **)error;

- (BOOL)moveItemAtURL:( NSURL *)srcURL toURL:( NSURL *)dstURL error:(NSError **)error;

- (BOOL)moveItemAtPath:( NSString*)srcPath toPath:( NSString*)dstPath error:(NSError **)error;


#if !TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR
//Getting home directory details.

/** Returns the home directory for the specified user.
 * 
 * This function is only supported for use with the current user.
 *
 * @return <tt>NSURL</tt> object containing the location of the specified user’s 
 *         home directory if the current user, or <tt>nil</tt> if not the current
 *         user, no such user exists, or the user’s home directory isn't
 *         available.
 */
#if defined(__MAC_10_12)
- (GD_NSNULLABLE NSURL *)homeDirectoryForUser:(NSString *)userName GD_API_AVAILABLE(macosx(10.12)) GD_API_UNAVAILABLE(ios, watchos, tvos);
#endif
#endif

// Unavailable API elements.
// iCloud - unsupported

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns <tt>nil</tt> always.
 */
@property(GD_NSNULLABLE_PROP readonly, copy, atomic) id<NSObject, NSCopying, NSCoding> ubiquityIdentityToken;

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns <tt>nil</tt> always.
 */
- (GD_NSNULLABLE NSURL *)URLForUbiquityContainerIdentifier:(GD_NSNULLABLE NSString *)containerIdentifier;

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns <tt>NO</tt> always.
 */
- (BOOL)isUbiquitousItemAtURL:( NSURL *)url;

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns <tt>NO</tt> always. Sets <tt>GDFileManagerErrorICloudNotSupported</tt>
 *          if an <tt>error</tt> location is specified.
 */
- (BOOL)setUbiquitous:(BOOL)flag itemAtURL:( NSURL *)url destinationURL:( NSURL *)destinationURL error:(NSError **)error;

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns <tt>NO</tt> always. Sets <tt>GDFileManagerErrorICloudNotSupported</tt>
 *          if an <tt>error</tt> location is specified.
 */
- (BOOL)startDownloadingUbiquitousItemAtURL:( NSURL *)url error:(NSError **)error;

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns <tt>NO</tt> always. Sets <tt>GDFileManagerErrorICloudNotSupported</tt>
 *          if an <tt>error</tt> location is specified.
 */
- (BOOL)evictUbiquitousItemAtURL:( NSURL *)url error:(NSError **)error;

/** Not available.
 * 
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns <tt>nil</tt> always. Sets <tt>GDFileManagerErrorICloudNotSupported</tt>
 *          if an <tt>error</tt> location is specified.
 */
- (GD_NSNULLABLE NSURL *)URLForPublishingUbiquitousItemAtURL:( NSURL *)url expirationDate:(NSDate * GD_NSNULLABLE_POINTER * GD_NSNULLABLE_POINTER)outDate error:(NSError **)error;

/** Not available.
 *
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * Calling this function in the subclass causes the specified completion
 * handler to be invoked as follows. The <tt>services</tt> parameter will
 * receive <tt>nil</tt>. The <tt>error</tt> parameter will receive an
 * <tt>NSError</tt> object with the <tt>GDFileManagerErrorNotSupported</tt>
 * code. The invocation will be on a background thread.
 */
#if defined(__MAC_10_13) || defined(__IPHONE_11_0)
- (void)getFileProviderServicesForItemAtURL:(NSURL *)url completionHandler:(void (^)(GD_NSDictionary(NSFileProviderServiceName,NSFileProviderService *) *services, NSError *error))completionHandler UNAVAILABLE_IN_GDFILEMANAGER;

#endif

/** Not available.
 *
 * This element of the base class interface isn't available in this subclass.
 * See under Usage, above.
 *
 * @returns Instance of NSURL class with no path always.
 */
#if defined(__MAC_10_12) || defined(__IPHONE_10_0)
@property(readonly, copy, atomic) NSURL *temporaryDirectory UNAVAILABLE_IN_GDFILEMANAGER;
#endif

// Linking.

- (BOOL)createSymbolicLinkAtURL:( NSURL *)url withDestinationURL:( NSURL *)destURL error:(NSError **)error;

- (BOOL)createSymbolicLinkAtPath:( NSString *)path withDestinationPath:( NSString *)destPath error:(NSError **)error;

- (BOOL)linkItemAtURL:( NSURL *)srcURL toURL:( NSURL *)dstURL error:(NSError **)error;

- (BOOL)linkItemAtPath:( NSString *)srcPath toPath:( NSString *)dstPath error:(NSError **)error;

- (GD_NSNULLABLE NSString *)destinationOfSymbolicLinkAtPath:( NSString *)path error:(NSError **)error;

// Determining access.

- (BOOL)fileExistsAtPath:( NSString *)path;

- (BOOL)fileExistsAtPath:( NSString*)path isDirectory:(GD_NSNULLABLE BOOL*)isDirectory;

- (BOOL)isReadableFileAtPath:( NSString *)path;

- (BOOL)isWritableFileAtPath:( NSString *)path;

- (BOOL)isExecutableFileAtPath:( NSString *)path;

- (BOOL)isDeletableFileAtPath:( NSString *)path;

- (GD_NSNULLABLE GD_NSDictionary(GD_NSFileAttributeKey, id) *)attributesOfItemAtPath:( NSString *)path error:(NSError **)error;

- (GD_NSNULLABLE GD_NSDictionary(GD_NSFileAttributeKey, id) *)attributesOfFileSystemForPath:( NSString *)path error:(NSError **)error;

- (BOOL)setAttributes:( GD_NSDictionary(GD_NSFileAttributeKey, id) *)attributes ofItemAtPath:( NSString *)path error:(NSError **)error;

// Getting file contents.

- (GD_NSNULLABLE NSData *)contentsAtPath:( NSString *)path;

- (BOOL)contentsEqualAtPath:( NSString *)path1 andPath:( NSString *)path2;

// Converting paths to strings.

- ( const char *)fileSystemRepresentationWithPath:( NSString *)path NS_RETURNS_INNER_POINTER;

- ( NSString *)stringWithFileSystemRepresentation:( const char *)string length:(NSUInteger)len;

// Get and set the current working directory of the program.

- (BOOL)changeCurrentDirectoryPath:( NSString *)path;

- ( NSString *)currentDirectoryPath;

// Deprecated methods

/** Deprecated in base class.
 * 
 * \deprecated This function is deprecated in the base class. Use
 * \reflink GDFileManager::setAttributes:ofItemAtPath:error: setAttributes:ofItemAtPath:error:\endlink
 * instead.
 */
- (BOOL)changeFileAttributes:( NSDictionary *)attributes atPath:( NSString *)path DEPRECATED_IN_NSFILEMANAGER;

/** Deprecated in base class.
 * 
 * \deprecated This function is deprecated in the base class. Use
 * \reflink GDFileManager::attributesOfItemAtPath:error: attributesOfItemAtPath:error:\endlink
 * instead.
 */
- (GD_NSNULLABLE NSDictionary *)fileAttributesAtPath:( NSString *)path traverseLink:(BOOL)flag DEPRECATED_IN_NSFILEMANAGER;

/** Deprecated in base class.
 * 
 * \deprecated This function is deprecated in the base class. Use
 * \reflink GDFileManager::attributesOfFileSystemForPath:error: attributesOfFileSystemForPath:error:\endlink
 * instead.
 */
- (GD_NSNULLABLE NSDictionary *)fileSystemAttributesAtPath:( NSString *)path  DEPRECATED_IN_NSFILEMANAGER;

/** Deprecated in base class.
 * 
 * \deprecated This function is deprecated in the base class. Use
 * \reflink GDFileManager::contentsOfDirectoryAtPath:error: contentsOfDirectoryAtPath:error:\endlink
 * instead.
 */
- (GD_NSNULLABLE NSArray *)directoryContentsAtPath:( NSString *)path DEPRECATED_IN_NSFILEMANAGER;

/** Deprecated in base class.
 * 
 * \deprecated This function is deprecated in the base class. Use
 * \reflink GDFileManager::createDirectoryAtPath:withIntermediateDirectories:attributes:error: createDirectoryAtPath:withIntermediateDirectories:attributes:error:\endlink
 * instead.
 */
- (BOOL)createDirectoryAtPath:( NSString *)path attributes:( NSDictionary *)attributes DEPRECATED_IN_NSFILEMANAGER;

/** Deprecated in base class.
 * 
 * \deprecated This function is deprecated in the base class. Use
 * \reflink GDFileManager::createSymbolicLinkAtPath:withDestinationPath:error: createSymbolicLinkAtPath:withDestinationPath:error:\endlink
 * instead.
 */
- (BOOL)createSymbolicLinkAtPath:( NSString *)path pathContent:( NSString *)otherPath DEPRECATED_IN_NSFILEMANAGER;

/** Deprecated in base class.
 * 
 * \deprecated This function is deprecated in the base class. Use
 * \reflink GDFileManager::destinationOfSymbolicLinkAtPath:error: destinationOfSymbolicLinkAtPath:error:\endlink
 * instead.
 */
- (GD_NSNULLABLE NSString *)pathContentOfSymbolicLinkAtPath:( NSString *)path  DEPRECATED_IN_NSFILEMANAGER;

@end

GD_NS_ASSUME_NONNULL_END
