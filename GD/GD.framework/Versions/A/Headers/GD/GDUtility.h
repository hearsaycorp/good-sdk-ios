/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 *
 */

#pragma once

#import <Foundation/Foundation.h>
#import "GDPortability.h"


GD_NS_ASSUME_NONNULL_BEGIN

/** Delegate for handling the results of BlackBerry Dynamics authentication
 *  token requests.
 * 
 * The results of BlackBerry Dynamics authentication token requests are
 * handled by creating a class that implements this protocol.
 * 
 * BlackBerry Dynamics authentication tokens can be requested by utilizing a
 * function in the <tt>GDUti</tt><tt>lity</tt> class.
 *
 * For the token request programming interface, and general information about
 * the mechanism, see the \reflink GDUtility GDUtility\endlink class reference.
 */
@protocol GDAuthTokenDelegate

/** Successful token request callback.
 * 
 * This callback will be invoked when a new BlackBerry Dynamics
 * authentication token has been generated. Token generation is always in
 * response to a call to the
 * \reflink GDUtility::getGDAuthToken:serverName: getGDAuthToken:serverName\endlink
 * function.
 *
 * The function that is invoked could initiate sending of the token to the
 * application server, out of band. The application server will then be able to
 * utilize the token to authenticate the end user.
 * 
 * @param gdAuthToken <tt>NSString</tt> containing the token.
 */
- (void)onGDAuthTokenSuccess:(NSString*)gdAuthToken;

/** Failed token request callback.
 * 
 * This callback will be invoked when a request for a BlackBerry
 * Dynamics authentication token has failed. Information about the error
 * condition is passed in a parameter.
 * 
 * The request will have been made by calling the
 * \reflink GDUtility::getGDAuthToken:serverName: getGDAuthToken:serverName\endlink
 * function.
 *
 * Invocation of this callback notifies the application that a token wasn't
 * issued in response to the request.
 *
 * Depending on the reason for failure, the function that is invoked could:
 * - Retry, by requesting a token again.
 * - Notify the user that this authentication mechanism isn't available.
 * .
 *
 * @param authTokenError <tt>NSError</tt> describing the error condition that
 *                       led to the failure of the token request.
 *
 * @see \ref gdauthtokendomain
 */
- (void)onGDAuthTokenFailure:(NSError*) authTokenError;

@end
/** BlackBerry Dynamics authentication token request.
 * 
 * The BlackBerry Dynamics authentication token mechanism enables
 * applications to utilize the user identification that takes place during
 * BlackBerry Dynamics authorization processing. This enables an application
 * server at the enterprise to authenticate the user without the need for entry
 * of any additional credentials at the device or desktop.
 * 
 * See below for an overall description of the BlackBerry Dynamics
 * authentication token mechanism. This class includes the programming interface
 * for requesting tokens.
 * 
 * @see  \reflink GDiOS::authorize: authorize (GDiOS)\endlink for more details of BlackBerry Dynamics authorization
 *      processing.
 * @see \ref ServerAPIGDAuthToken
 * 
 * <h3>BlackBerry Dynamics Authentication Token Mechanism</h3>
 * The BlackBerry Dynamics platform includes rigorous authentication of the end
 * user. This is used when, for example, identifying whether the user is
 * entitled to run the current application, and when applying security policies.
 * 
 * The BlackBerry Dynamics Authentication Token (BlackBerry Dynamics auth)
 * mechanism enables applications to take advantage of the authentication
 * processes of the platform.
 *
 * BlackBerry Dynamics auth tokens can be requested by the application on the
 * device or desktop. A token will only be issued if authorization processing
 * has completed, and the end user's identity has been authenticated.
 *
 * Once a token has been issued, the application can send the token to an
 * application server at the back end. The token can then be checked by the
 * application server, using a verification service provided by the BlackBerry
 * Dynamics infrastructure.
 *
 * The sequence of programming interfaces used in BlackBerry Dynamics auth is as
 * follows.
 * -# The mobile application calls \reflink GDUtility::getGDAuthToken:serverName: getGDAuthToken:serverName\endlink to
 *    request a token.
 * -# All being well, a token is issued and the
 *     \reflink GDAuthTokenDelegate::onGDAuthTokenSuccess: onGDAuthTokenSuccess\endlink callback is
 *    invoked and passed the new token.
 * -# The application sends the token to the application server, using an HTTP
 *    request, socket, or other method. In the same communication, the
 *    application sends the email address or other end user identifier for which
 *    it is activated as a BlackBerry Dynamics application.
 * -# The application server checks that the token is valid by calling the
 *    verification service in the \ref ServerAPIGDAuthToken, hosted by an
 *    enterprise BlackBerry Dynamics server. (The server could be a BlackBerry
 *    Proxy or Good Proxy instance.)
 * -# The response from the verification service includes the end user
 *    identifier, if the token is valid. The application server can check that
 *    the value from the verification service is the same as that originally
 *    sent by the mobile application.
 * .
 * This sequence validates the end user's identity, and the application server
 * can therefore grant access to resources and other permissions.
 *
 * Calling the verification service doesn't cause the token to be consumed.
 *
 * Note that a BlackBerry Dynamics application can obtain the identifier of the
 * end user from the <tt>GDAppConfigKeyUserId</tt> value in the collection
 * returned by the \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS)\endlink function.
 * 
 * <h3>Challenge Strings</h3>
 * A <em>challenge string </em>can be included in a BlackBerry Dynamics auth
 * token request. The same challenge string will then be provided to the
 * application server, in the response from the verification service.
 *
 * The challenge string could have a number of uses for application developers.
 * A typical usage could be to tie an instance of authentication to a previous
 * access request, as follows:
 * -# A mobile application attempts to access a resource on an application
 *    server.
 * -# The application server generates a random challenge string.
 * -# The application server responds to the mobile application with a denial of
 *    access message that includes the random challenge string.
 * -# The mobile application requests a BlackBerry Dynamics auth token, and
 *    passes the value from the denial of access message as the challenge
 *    string.
 * -# The mobile application again attempts to access the resource on the
 *    application server, but this time includes the BlackBerry Dynamics auth
 *    token in the request.
 * -# The application server sends the token to the verification service, which
 *    responds that the token is valid. The response includes the challenge
 *    string.
 * -# The application server checks that the challenge string from the
 *    verification service is the same as the random challenge string initially
 *    sent to the mobile application in the denial of access message.
 * .
 * In the above, a new random challenge string is generated on every resource
 * access attempt. This has the effect of making the BlackBerry Dynamics auth
 * tokens one-use. A more advanced algorithm might be to store the token and
 * challenge string in the server, as a session identifier. To end a session,
 * the server could delete the stored token and challenge string, effectively
 * forcing the application to generate a new token, based on a new challenge
 * string, when it next connected.
 * 
 * The verification service provides the challenge string to the application
 * server in an HTTP header, which limits the character set that can be utilized
 * safely. All letters and numerals in the ASCII range 32 to 126 can be utilized
 * safely. Use of other characters isn't supported.
 */
@interface GDUtility : NSObject

/** BlackBerry Dynamics authentication token request.
 * 
 * Call this function to request a new BlackBerry Dynamics
 * authentication token. Pass the challenge string and server name as
 * parameters.
 *
 * See under BlackBerry Dynamics Authentication Token Mechanism, above, for
 * background on tokens and the challenge string.
 * 
 * This function requests a BlackBerry Dynamics authentication token from
 * the BlackBerry Dynamics runtime. The runtime might connect to the BlackBerry
 * Dynamics infrastructure installed at the enterprise in order to service the
 * request.
 * 
 * The request is asynchronous. If the request succeeds, the token will be
 * passed to the  \reflink GDAuthTokenDelegate::onGDAuthTokenSuccess: onGDAuthTokenSuccess\endlink callback in the
 * delegate. If the attempt fails, an error will
 * be passed to the  \reflink GDAuthTokenDelegate::onGDAuthTokenFailure: onGDAuthTokenFailure\endlink callback in
 * the delegate instead.
 *
 * The <tt>delegate</tt> property must be set before this function is
 * called.
 *
 * @param challenge <tt>NSString</tt> containing the challenge string for the token.
 *                  The challenge string can be empty.
 *
 * @param serverName <tt>NSString</tt> containing additional identification, by
 *                   convention the fully qualified domain name of the
 *                   application server for which a token is being requested.
 *                   Whatever value is passed here will also be returned to the
 *                   server as part of the token validation response.
 *
 */
- (void)getGDAuthToken: (NSString*) challenge serverName:(GD_NSNULLABLE NSString*) serverName;

/** Delegated event-handling.
 * 
 * BlackBerry Dynamics authentication token requests are asynchronous. When a
 * request succeeds or fails, a callback in the application code is invoked by
 * the BlackBerry Dynamics runtime.
 * 
 * Set this property to an instance of a class in the application that contains
 * the code for the required callbacks, i.e. a class that implements
 * the GDAuthTokenDelegate protocol.
 */
@property (GD_NSNULLABLE_PROP weak, getter = getGDAuthDelegate, setter = setGDAuthDelegate:) id<GDAuthTokenDelegate> gdAuthDelegate;


@end

/**
 * \defgroup gdauthtokendomain BlackBerry Dynamics authentication token error domain
 * These constants can be used when handling BlackBerry Dynamics authentication
 * token request errors, in a \reflink GDAuthTokenDelegate GDAuthTokenDelegate\endlink
 * implementation.
 *
 * \{
 */

/** Error domain for the BlackBerry Dynamics authentication token mechanism.
 */
extern NSString* const GDAuthTokenDomain;


typedef NS_ENUM(NSInteger, GDAuthTokenError)
{
    /** The BlackBerry Dynamics server software installed at the enterprise
     * doesn't support the authentication token mechanism.
     */
    GDAuthTokenErrNotSupported = -2,
    
    /** An error occurred during token generation or communication.
     * 
     * Sending the same request later mightn't encounter the same condition, and
     * could succeed.
     */
    GDAuthTokenErrRetry         = -1,
    
};

/** \}
 */

GD_NS_ASSUME_NONNULL_END
