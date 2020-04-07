/*
 * (c) 2019 BlackBerry Limited. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>


/** NSNotification name for <tt>GDAuthenticationManager</tt> notifications.
 *
 *  The name of the notification for which to register an observer.
 *
 *  <tt>UserInfo</tt> dictionary from <tt>GDReAuthenticationResultNotification</tt> contains the following:\n
 *  - token    - specific request token, enables the caller to associate an event with a specific call to the interface (as interface will synchronously return with a token for that call)
 *  - result   - denotes whether user was successfully authenticated and provides details of any error. See \ref GDReAuthResult.
 *  - authType - indicates how user was authenticated. See \ref GDReAuthType.
 *
 *  To get the reauthenticate results from the notification, use these convenient methods from GDAuthenticationManager:
 *     - getReauthenticationToken
 *     - getReauthenticationResult
 *     - getReauthenticationType
 */
extern NSString * _Nonnull const GDReAuthenticationResultNotification;

/// BlackBerry reauthentication result type
/// \copydetails ssReauthResult
typedef NS_ENUM(NSInteger, GDReAuthResult)
{
    /// \copydetails ssReauthResultSuccess
    GDReAuthResultSuccess,

    /// \copydetails ssReauthResultErrorFailed
    GDReAuthResultErrorFailedAuth,

    /// \copydetails ssReauthResultErrorUserCancelled
    GDReAuthResultErrorUserCancelled,

    /// \copydetails ssReauthResultErrorExpired
    GDReAuthResultErrorExpired,

    /// \copydetails ssReauthResultErrorInProgress
    GDReAuthResultErrorInProgress,

    /// \copydetails ssReauthResultErrorNotSupported
    GDReAuthResultErrorNotSupported,

    /// \copydetails ssReauthResultInvalidRequest
    GDReAuthResultErrorInvalidRequest,

    /// \copydetails ssReauthResultErrorUnknown
    GDReAuthResultErrorUnknown
};

/// BlackBerry reauthentication result type
/// \copydetails ssReauthType
typedef NS_ENUM(NSInteger, GDReAuthType)
{
    /// \copydetails ssReauthTypeNone
    GDReAuthTypeNone,

    /// \copydetails ssReauthTypeNoPassword
    GDReAuthTypeNoPassword,

    /// \copydetails ssReauthTypePassword
    GDReAuthTypePassword,

    /// \copydetails ssReauthTypeBiometric
    GDReAuthTypeBiometric,

    /// \copydetails ssReauthTypeGracePeriod
    GDReAuthTypeGracePeriod,
    
    /// \copydetails ssReauthTypeTrusted
    GDReAuthTypeTrustedAuthenticator
    
    
};

NS_ASSUME_NONNULL_BEGIN



/**
 * Manage user authentication and require the user to authenticate.
 *
 * <h2>Authentication Manager Usage</h2>
 *
 * The Authentication Manager Interface enables an application to request a user to reauthenticate.
 * When called a user will be prompted to authenticate using whatever authentication 
 * method is specified by policy. For example, biometrics or security password. If the policy doesn't
 * require a password, then a prompt will be displayed with an 'Authorize' button for the user to acknowledge.
 *
 * There are two scenarios for using the interface.
 * 
 * <b>Authorize an action</b>
 *
 * Before permitting a destructive or sensitive action, it may be necessary to confirm such a request is 
 * being initiated by an authorized user of the application. For example, during a request 
 * to delete all documents or share a file with an external recipient the application may want to first 
 * prompt the user to enter their security password. Alternatively, if a user is taking an action on a 
 * different device or website, it may be necessary to confirm the authenticity of the request by prompting
 * the user to authorize the action on their BlackBerry Dynamics application, essentially a two-factor
 * authentication (2FA). In this scenario the user is able to cancel the request and continue using the 
 * application.
 *
 * <b>Force reauthentication</b>
 *
 * The application may want to trigger the runtime state to become locked, perhaps due to a threat which 
 * has been detected or an anomaly in user behaviour. To continue using the application the user must authenticate.
 *
 * <h3>Programming Interface and Notifications</h3>
 * To notify the application, the runtime posts an <tt>NSNotification</tt>.
 *
 * <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Request reauthentication</h3>
 * \code
 *
 * NSNotificationCenter * __weak center = [NSNotificationCenter defaultCenter];
 * id __block observerToken = [center addObserverForName:GDReAuthenticationResultNotification
 *                                                object:nil
 *                                                 queue:[NSOperationQueue mainQueue]
 *                                            usingBlock:^(NSNotification *notification) {
 *
 *                                               NSLog(@"Received the reauthentication notification!");
 *
 *                                               NSString *receivedToken = [GDAuthenticationManager getReauthenticationToken:notification];
 *                                               GDReAuthResult result = [GDAuthenticationManager getReauthenticationResult:notification];
 *                                               GDReAuthType type = [GDAuthenticationManager getReauthenticationType:notification];
 *
 *                                               if (result == GDReAuthResultSuccess)
 *                                               {
 *                                                   NSLog(@"Reauthentication successful, proceed doing your work.");
 *                                                   // do some user action
 *                                               }
 *                                               else
 *                                               {
 *                                                   NSLog(@"Reauthentication Rejected");
 *                                               }
 *
 *                                               [center removeObserver:observerToken];
 *                               }];
 *
 * NSString *token = [GDAuthenticationManager reauthenticate:@"Title" message:@"Message" timeoutInSeconds:20 gracePeriodInSeconds:120 enforce:YES requirePassword:YES];
 * \endcode
 *
 */

@interface GDAuthenticationManager : NSObject

/// Requests user reauthentication
///
///\copydetails ssReauthenticate
+ (NSString *)reauthenticate:(NSString *)title
                     message:(NSString *)message
            timeoutInSeconds:(NSUInteger)timeout
        gracePeriodInSeconds:(NSUInteger)gracePeriod
                     enforce:(BOOL)enforce
             requirePassword:(BOOL)requirePassword;

/// Requests user reauthentication
///\overload
+ (NSString *)reauthenticate:(NSString *)title
                     message:(NSString *)message
            timeoutInSeconds:(NSUInteger)timeout;

/// Requests user reauthentication
///\overload
+ (NSString *)reauthenticate:(NSString *)title
                     message:(NSString *)message
            timeoutInSeconds:(NSUInteger)timeout
        gracePeriodInSeconds:(NSUInteger)gracePeriod;

/**
 * Gets token from reauthentication notification.
 *
 * This function is a helper that returns the reuthentication token from <tt>NSNotification</tt>.
 *
 * @param notification              <tt>NSNotification</tt> instance which has been received after subscribing on
 *                                  notification with name GDReAuthenticationResultNotification.
 *
 * @return <tt>NSString</tt> token value for the type of event being notified.
 */
+ (NSString *)getReauthenticationToken:(NSNotification *)notification;


/**
 * Gets authentication result from reauthentication notification.
 *
 * This function is a helper that returns the reauthentication result from <tt>NSNotification</tt>.
 *
 * @param notification              <tt>NSNotification</tt> instance which has been received after subscribing to
 *                                  notification with name GDReAuthenticationResultNotification.
 *
 * @return <tt>GDReAuthResult</tt> value for the type of event being notified.
 */
+ (GDReAuthResult)getReauthenticationResult:(NSNotification *)notification;


/**
 * Gets authentication type from reauthentication notification.
 *
 * This function is a helper that returns the reauthentication type from <tt>NSNotification</tt>.
 *
 * @param notification              <tt>NSNotification</tt> instance which has been received after subscribing to
 *                                  notification with name GDReAuthenticationResultNotification.
 *
 * @return <tt>GDReAuthType</tt> value for the type of event being notified.
 */
+ (GDReAuthType)getReauthenticationType:(NSNotification *)notification;

@end

NS_ASSUME_NONNULL_END
