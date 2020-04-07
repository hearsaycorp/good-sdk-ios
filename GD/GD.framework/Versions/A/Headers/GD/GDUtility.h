/*
 * (c) 2019 BlackBerry Limited. All rights reserved.
 *
 */

#pragma once

#import <Foundation/Foundation.h>

#import "GDPortability.h"

/** \defgroup jwtcallback BlackBerry Enterprise Identity JWT request callback
 * \{
 */

/** Status codes from BlackBerry Enterprise Identity JWT request.
 * 
 * These constants can be used for handling errors returned from JWT retrieval
 *
 * @see \ref GDUtility class reference.
 */

GD_NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, BBDJWTStatusCode) {
    /** Request successful
     */
    BBDJWTStatusOK = 0,
    
    /** A general error occurred not covered by other status codes.
     */
    BBDJWTStatusUnknownError,
    
    /** MTD is not enabled (only for MTD related requests)
     */
    BBDJWTMTDNotEnabled,
    
    /** Server responded with an error.
     */
    BBDJWTServerError,
    
    /** Client is not authorised to make this request. For Zero Sign On (ZSO), a valid certificate is not enrolled.
     */
    BBDJWTAuthFailure,
    
    /** Server has not yet updated to accept a recently enrolled authorisation. Eg a ZSO is enrolled but not yet activated on the
     * server. This is temporary and can be retried.
     */
    BBDJWTPendingActivation
};
 
/** Completion Block definition for JWT retrieval
 *
 * @param JWT If successful an NSString object pointer containing the JWT. Empty otherwise.
 *
 * @param status A BBDJWTStatusCode value indicating the status of the request.
 *
 * @see BBDJWTStatusCode
 *
 * @param error An additional error parameter which may contain the HTTP status code of a server related failure, 0 on success.
 *
 */
typedef void (^BBDJWTCompletion)(NSString* JWT, BBDJWTStatusCode status, int error);
/** \}
 */

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
 * the mechanism, see the \reflink GDUtility GDUtility \endlink class reference.
 */
@protocol GDAuthTokenDelegate

/** Successful token request callback.
 * 
 * This callback will be invoked when a new BlackBerry Dynamics
 * authentication token has been generated. Token generation is always in
 * response to a call to the
 * \reflink GDUtility::getGDAuthToken:serverName: getGDAuthToken:serverName \endlink
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
 * \reflink GDUtility::getGDAuthToken:serverName: getGDAuthToken:serverName \endlink
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

/** BlackBerry Dynamics utility for managing authentication tokens and identity.
 * 
 * 
 * <h3>BlackBerry Dynamics Authentication Token Mechanism</h3>
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
 * @see \reflink GDiOS::authorize: authorize (GDiOS) \endlink for more details of BlackBerry Dynamics authorization
 *      processing.
 * @see \ref ServerAPIGDAuthToken
 *
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
 * -# The mobile application calls \reflink GDUtility::getGDAuthToken:serverName: getGDAuthToken:serverName \endlink to
 *    request a token.
 * -# All being well, a token is issued and the
 *    \reflink GDAuthTokenDelegate::onGDAuthTokenSuccess: onGDAuthTokenSuccess \endlink callback is
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
 * returned by the \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink function. 
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
 * 

 * <h3>BlackBerry Dynamics Shared User Identifier</h3>
 * The BlackBerry Dynamics Shared User ID is a unique identifier for all containers which are provisioned for the same user on the same device.
 *
 * <h4>Identifier characteristics</h4>
 *
 * - The identifier is available once a user has authorized / unlocked the container.
 * - The identifier is the same for applications activated against the same user, on the same UEM domain and on the same device, even if authentication delegation (SSO) is enabled.
 * - The identifier doesn't persist once the last Dynamics application is uninstalled from a device or the device is reset.
 * - The identifier is only available for applications activated against a UEM. (Not standalone GC)
 *
 * <h4>Shared Identifier limitations</h4>
 * Shared identifier will not be shared among applications from different keychain groups
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
 * passed to the \reflink GDAuthTokenDelegate::onGDAuthTokenSuccess: onGDAuthTokenSuccess \endlink callback in the
 * delegate. If the attempt fails, an error will
 * be passed to the \reflink GDAuthTokenDelegate::onGDAuthTokenFailure: onGDAuthTokenFailure \endlink callback in
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

/** Get User Dynamics Shared identifier.
 *
 * 
 * Call this method to get a BlackBerry Dynamics shared user identifier.
 *
 * See under BlackBerry Dynamics Shared User Identifier, above, for
 * background on shared id usage and characteristics.
 *
 * If calculation of shared id is not possible, empty string will be returned
 *
 * \return Shared Id String
 */
- (NSString *)getDynamicsSharedUserIDWithError:(NSError **)error;

/** Get JWT from BlackBerry Enterprise Identity (EID) server
 *
 * Call this function to request a BlackBerry Enterprise Identity
 * JWT Bearer Token for authentication purposes. Pass a registered client ID string, scope
 * string and closure reference (for calling back) as parameters. Tokens are cached
 * until expired, but a new token from the server may be specified by setting the refresh flag true.
 *
 * This function requests a BlackBerry Dynamics JWT from
 * the BlackBerry Dynamics runtime. The runtime might connect to the BlackBerry
 * Dynamics infrastructure installed at the enterprise in order to service the
 * request.
 *
 * The request is asynchronous.
 *
 * @param clientID <tt>NSString</tt> containing the client ID.
 *
 * @param scope <tt>NSString</tt> the scope for the token.
 *
 * @param resourceServer <tt>NSString</tt> optional resource server specifier (not currently used)
 *
 *
 * @param refresh A flag to force a new token to be fetched from the server.
 *
 * @param callback block of type \ref BBDJWTCompletion, void (^BBDJWTCompletion)(NSString* JWT, BBDJWTStatusCode status, int error);
 *
 * @param error Optional pointer to NSError object which may contain further information.
 
 *
 */
- (BOOL) getEIDToken:(NSString *) clientID withScope:(NSString*) scope withResourceServer:(NSString*) resourceServer withRefresh:(BOOL) refresh withCompletion:(BBDJWTCompletion) callback withError:(NSError**) error;

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
 * token request errors, in a \reflink GDAuthTokenDelegate GDAuthTokenDelegate \endlink
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
