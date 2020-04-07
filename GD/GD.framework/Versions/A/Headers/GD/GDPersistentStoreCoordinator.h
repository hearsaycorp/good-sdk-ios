/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#pragma once

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

/** NSPersistentStoreCoordinator subclass that supports an encrypted binary
 *  store type in Core Data.
 * 
 * BlackBerry Dynamics applications can store Core Data objects in the secure
 * store.
 *
 * Using this class instead of the default <tt>NSPersistentStoreCoordinator</tt>
 * allows the use of the following additional Core Data store types.<dl>
 *     <dt
 *         ><tt>GDEncryptedBinaryStoreType</tt></dt>
 *     <dd
 *         >Encrypted binary store that is stored in the BlackBerry Dynamics
 *         secure store.\n
 *         Use this in place of <tt>NSBinaryStoreType</tt>.</dd>
 *     <dt
 *         ><tt>GDEncryptedIncrementalStoreType</tt></dt>
 *     <dd
 *         >Encrypted incremental store that is stored in the BlackBerry
 *         Dynamics secure store.\n
 *         Use this in place of <tt>NSSQLiteStoreType</tt>.</dd>
 *     </dl>
 *
 * \htmlonly<div class="bulletlists">\endhtmlonly
 * Note the following.
 * - When these store types are in use, the <tt>URL</tt> parameter will be an
 *   absolute path within the BlackBerry Dynamics secure file system.
 * - Use of this class with store types other than the above results in
 *   identical behavior to using the default class. The above additional store
 *   types cannot be used with the default class.
 * - Data can be migrated from an <tt>NSSQLiteStoreType</tt> store to a
 *   <tt>GDEncryptedIncrementalStoreType</tt> store. Use the Core Data migration
 *   API to do this.\n
 *   For an example, see the CoreData sample application supplied with the
 *   BlackBerry Dynamics Software Development Kit for iOS.\n
 *   It isn't possible to import an <tt>NSSQLiteStoreType</tt> store file
 *   directly into the secure file system, and then use it as a
 *   <tt>GDEncryptedIncrementalStoreType</tt> store.
 * - Core Data stores of the above types cannot be accessed until BlackBerry
 *   Dynamics authorization processing has completed. This means that
 *   construction of the Managed Object Context, and the population of views,
 *   must be deferred until after authorization.\n
 *   For an example of deferred construction and population, see the CoreData
 *   sample application supplied with the BlackBerry Dynamics Software
 *   Development Kit for iOS.
 * .
 * \htmlonly</div>\endhtmlonly
 *
 * @see <a
            href="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/CoreData/"
            target="_blank"
        >Core Data Programming Guide</a> on the apple.com developer website.
 * @see \reflink  GDFileManager \endlink
 * @see \reflink  sqlite Secure SQL Database API \endlink
 * @see \reflink GDiOS \endlink, for BlackBerry Dynamics authorization.
 *
 * <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Utilize BlackBerry Dynamics Persistent Store Coordinator</h3>
 * \code
 * - (NSPersistentStoreCoordinator *)persistentStoreCoordinator
 * {
 *      if (__persistentStoreCoordinator != nil) {
 *          return __persistentStoreCoordinator;
 *      }
 *
 *      // The URL will be a path in the secure container
 *      NSURL *storeURL = [NSURL URLWithString:@"/example.bin"];
 *
 *      NSError *error = nil;
 *      __persistentStoreCoordinator =
 *          [[GDPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
 *      if (![__persistentStoreCoordinator
 *              addPersistentStoreWithType: GDEncryptedBinaryStoreType
 *                           configuration: nil
 *                                     URL: storeURL
 *                                 options: nil
 *                                   error: &error]
 *      ) {
 *          abort();
 *      }
 *
 *      return __persistentStoreCoordinator;
 * }
 *
 * \endcode
 *
 */

@interface GDPersistentStoreCoordinator : NSPersistentStoreCoordinator {
    
}

@end

/** Specify the encrypted binary store type.
 */
extern NSString * const GDEncryptedBinaryStoreType;

/** Specify the encrypted binary store type error domain.
 */
extern NSString* const GDEncryptedBinaryStoreErrorDomain;

/** Specify the encrypted incremental store type.
 */
extern NSString* const GDEncryptedIncrementalStoreType;

/** Specify the encrypted incremental store type error domain.
 */
extern NSString* const GDEncryptedIncrementalStoreErrorDomain;

