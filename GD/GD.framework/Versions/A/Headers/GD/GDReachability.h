/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

/* \cond DOXYGEN_IGNORE */
#ifndef __has_extension
#define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif
/* \endcond */

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATED_IN_GDREACHABILITY __attribute__((deprecated("Deprecated in GDReachability. Use isNetworkAvailable property instead.")))
#else
#   define DEPRECATED_IN_GDREACHABILITY __attribute__((deprecated))
#endif
/* \endcond */

/**
 * Constants for GDReachability network status.
 * 
 * This enumeration represents the different possible statuses of the connection
 * to the BlackBerry Dynamics infrastructure. The <tt>status</tt> property of a
 * \ref GDReachability object always takes one of these values.
 */
typedef NS_ENUM(NSInteger, GDReachabilityStatus) {
    /** The infrastructure isn't reachable.
     */
    GDReachabilityNotReachable = 0,
    
    /** The infrastructure is reachable via a Wi-Fi connection.
     */
    GDReachabilityViaWiFi,

    /** The infrastructure is reachable via a mobile data (cellular) connection.
     */
    GDReachabilityViaCellular
};

#endif

/** Notification name for changes to BlackBerry Dynamics infrastructure
 *  reachability.
 * 
 * Use this value to add an observer of changes to BlackBerry Dynamics
 * infrastructure reachability. See the \ref GDReachability class reference for
 * details.
 */
extern NSString *GDReachabilityChangedNotification;

/** BlackBerry Dynamics infrastructure connection status.
 * 
 * This class represents the status of the connection from the
 * application to the BlackBerry Dynamics infrastructure.
 * 
 * Every BlackBerry Dynamics application connects to the BlackBerry Dynamics
 * infrastructure whenever possible. The connection is maintained by the
 * BlackBerry Dynamics runtime instance that is embedded in the application. The
 * infrastructure includes the BlackBerry Dynamics Network Operation Center
 * (NOC) as well as a number of other components that can be installed by the
 * enterprise.
 * <p class="bulletlists">
 * The current status can be obtained synchronously. It is also possible to
 * receive notifications whenever the status changes. Use the native
 * <tt>NSNotificationCenter</tt> programming interface, with the following
 * parameters.
 * - Set the notification name to the \ref GDReachabilityChangedNotification
 *   value.
 * - Set the notification object to the BlackBerry Dynamics runtime interface
 *   object, obtained by
 *   <tt>[G<tt></tt>DiOS sharedInstance]</tt>.
 * </p>
 *
 * @see \ref GDConnectivityManager class reference.
 * @see <a href="https://developer.apple.com/reference/foundation/nsnotificationcenter"
          target="_blank">NSNotificationCenter class reference</a>
          on the apple.com developer website.
 *
 * <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Register for notification</h3>
 * \code
 * - (void)addChangeConnectionStatusObserver {
 *  [[NSNotificationCenter defaultCenter] addObserver:self
 *                                           selector:@selector(reachabilityChanged:)
 *                                               name:GDReachabilityChangedNotification
 *                                             object:[GDiOS sharedInstance]];
 * }
 * \endcode
 *
 * \code
 * - (void)reachabilityChanged:(NSNotification *)notification {
 *  GDReachability *reachability = [GDReachability sharedInstance];
 *
 *  if (reachability.status == GDReachabilityNotReachable) {
 *
 *      UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Error"
 *                                                      message:@"Network NOT Available"
 *                                                     delegate:nil
 *                                            cancelButtonTitle:@"OK"
 *                                            otherButtonTitles:nil, nil];
 *      [alert show];
 *
 *  } else {
 *
 *      UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Success"
 *                                                      message:@"Network Available"
 *                                                     delegate:nil
 *                                            cancelButtonTitle:@"OK"
 *                                            otherButtonTitles:nil, nil];
 *      [alert show];
 *
 *  }
 * \endcode
 * <p class="bulletlists">
 * The above snippet shows:
 * - Registration for notification of changes in connection status. The observer
 *   code is specified by a selector.
 * - Implementation of the selector, which displays a message to the user.
 * </p>
 */
@interface GDReachability : NSObject

/** Get a reference to the BlackBerry Dynamics infrastructure connection status
 *  object.
 * 
 * This function returns a reference to the BlackBerry Dynamics infrastructure
 * connection status object, which is a "singleton class".
 *
 * @return Reference that can be used, for example, to access the
 *         <tt>status</tt> property.
 */
+ (instancetype) sharedInstance;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/**
 * Connection status and medium.
 * 
 * The value of this property represents the type of connection through which
 * the BlackBerry Dynamics infrastructure is reachable. It always takes a value
 * from the \ref GDReachabilityStatus enumeration:
 * - <tt>GDReachabilityViaCellular</tt> if the infrastructure is reachable via a
 *   mobile data (cellular) connection.
 * - <tt>GDReachabilityViaWiFi</tt> if the infrastructure is reachable via a
 *   Wi-Fi connection.
 * - <tt>GDReachabilityNotReachable</tt> if the infrastructure isn't reachable.
 * .
 */
@property (nonatomic, readonly) GDReachabilityStatus status;

#endif

/** Connection availability.
 *
 * The value of this property represents the availability of the connection
 * to the BlackBerry Dynamics infrastructure.
 * 
 * A change to the availability of the connection will trigger an infrastructure
 * status change notification to the application. See the class description, 
 * above, for details of how to register for these notifications. In addition, 
 * key-value observing (KVO) can be applied to this property.
 */
@property (nonatomic, readonly) BOOL isNetworkAvailable;

/** Push Channel availability.
 *
 * The value of this property represents the availability of the BlackBerry
 * Dynamics Push Channel service. For an overall description of how to use the
 * service, see under \reflink GDPushChannel GDPushChannel \endlink.
 *
 * A change to the availability of the Push Channel service will trigger an
 * infrastructure status change notification to the application. See the class
 * description, above, for details of how to register for these notifications.
 * In addition, key-value observing (KVO) can be applied to this property.
 */
@property (nonatomic, readonly) BOOL isPushChannelAvailable;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/** 
 * Connection availability (deprecated).
 * \deprecated This function is deprecated and will be removed in a future
 * release. Use the <tt>isNetworkAvailable</tt> property instead.
 * 
 * @return <tt>YES</tt> if there is a current connection to the infrastructure.
 * @return <tt>NO</tt> otherwise.
 */
+ (BOOL) isNetworkAvailable DEPRECATED_IN_GDREACHABILITY;
#endif

@end
