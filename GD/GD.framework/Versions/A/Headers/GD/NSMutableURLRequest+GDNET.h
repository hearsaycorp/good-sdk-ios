/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#pragma once

#import <Foundation/Foundation.h>

extern NSString* const GDURLAuthenticationParameters;

/** NSMutableURLRequest category with additional features.
 * 
 * This class is a category of the Foundation <tt>NSMutableURLRequest</tt>
 * class that can be used when the BlackBerry Dynamics proxy infrastructure is
 * in use via the URL Loading System (see \ref GDURLLoadingSystem). This class
 * provides additional features to the default request class.
 *
 * The additional features enable enhanced control over authentication by
 * applications that use the <tt>UIWebView</tt> class.
 *
 * This documentation includes only additional operations that are not part
 * of the default <tt>NSMutableURLRequest</tt> API.
 *
 * @see <a
 *          HREF="https://developer.apple.com/reference/foundation/nsmutableurlrequest?language=objc"
 *          target="_blank"
 *      >NSMutableURLRequest class reference</a
 *      > on the apple.com developer website.
 * 
 * @see <a
 *          HREF="https://developer.apple.com/reference/uikit/uiwebview?language=objc"
 *          target="_blank"
 *      >UIWebView class reference</a
 *      > on the apple.com developer website.
 * 
 * @see <a
 *          HREF="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/DisplayWebContent/DisplayWebContent.html"
 *          target="_blank"
 *      >WebKit Objective-C Programming Guide</a
 *      > on the apple.com developer website.
 *
 * <h3>Authentication error-handling</h3>
 * Authentication errors may be encountered when this class is in use. For
 * example, the combination of user name and password may be rejected by the
 * server that receives the HTTP request.
 *
 * When an authentication error is encountered, a call to
 * <tt>UIWebViewDelegate::didFailLoadWithError:(NSError *)error</tt> will be
 * made. The code used to indicate that the failure condition is an
 * authentication error is <tt>NSURLErrorUserAuthenticationRequired</tt>.
 *
 * The <tt>NSError</tt> object that is passed will have an associated
 * <tt>NSDictionary</tt> object, which can be accessed via the
 * <tt>NSError::userInfo</tt> accessor. In the <tt>NSDictionary</tt>, with the
 * key <tt>GDURLAuthenticationParameters</tt>, there will be an <tt>NSArray</tt>
 * containing the authentication objects associated with the request.
 *
 * The structure of the authentication parameters <tt>NSArray</tt> will be as
 * follows.
 * \code
 * { (NSURLProtectionSpace*) protectionSpace, (NSURLCredential*) credential, nil }
 * \endcode
 * Note that <tt>credential</tt> may be nil.
 *
 * The following example shows how to access the array and credentials.
 * \code
 * - (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
 *
 *   if( NSURLErrorUserAuthenticationRequired == [error code] ) {
 *       NSDictionary* dict = [error userInfo];
 *       NSArray* credentialArray = [dict objectForKey:GDURLAuthenticationParameters];
 *       if(credentialArray){
 *           NSURLProtectionSpace* protSpace = nil;
 *           NSURLCredential* cred = nil;
 *           if([credentialArray count] > 0){
 *               protSpace = [credentialArray objectAtIndex:0];
 *               // we may also have a credential
 *               if([credentialArray count] > 1){
 *                   cred = [credentialArray objectAtIndex:1];
 *               }
 *           }
 *           // Do something with the credentials here
 *       }
 *   }
 *
 *   // Handling for other error conditions goes here
 * }
 * \endcode
 */
@interface NSMutableURLRequest (GDNET)

/** Handle authentication challenges by failing in UIWebView.
 * 
 * Call this function to force a failure in the URL request's associated
 * <tt>UIWebView</tt> object when an authentication challenge is received.
 *
 * Calling this function sets the following behavior. When the
 * associated URL request receives an HTTP 401 "Unauthorized" response, or an
 * HTTP 407 "Proxy Authentication Required" response, the <tt>UIWebView</tt>
 * will fail with <tt>UIWebViewDelegate::didFailLoadWithError</tt>.
 *
 * The application should handle the failure by taking appropriate action,
 * such as resubmitting the request with authorization credentials.
 *
 * By default, <tt>UIWebView</tt> does not fail when either of these statuses
 * is received in the related request.
 *
 * This function can only be used when the BlackBerry Dynamics proxy
 * infrastructure is in use via the URL Loading System, and a <tt>UIWebView</tt>
 * control is in use.
 */
- (BOOL) failOnAuthorizationChallenge;

/** Specify authorization credentials (UIWebView only).
 * 
 * Call this function to set the authorization credentials that will be sent if
 * the response to the URL request contains an authentication challenge. This
 * function can be used with requests that are associated with a
 * <tt>UIWebView</tt> object.
 *
 * Authorization credentials would usually be sent in response to receiving
 * <tt>NSURLConnectionDelegate didReceiveAuthenticationChallenge</tt>.
 *
 * Server and proxy credentials can both be set, in separate calls to the
 * function.
 *
 * This function can only be used when the BlackBerry Dynamics proxy
 * infrastructure is in use via the URL Loading System, and a <tt>UIWebView</tt>
 * control is in use.
 *
 * @param credentials <tt>NSURLCredential</tt> containing the username and
 *                    password.
 * @param space <tt>NSURLProtectionSpace</tt> containing the following:\n
 *              Hostname or address,\n
 *              Port number,\n
 *              Authentication method,\n
 *              Proxy indicator.
 */
- (BOOL) setAuthorizationCredentials:(NSURLCredential*)credentials withProtectionSpace:(NSURLProtectionSpace*)space;

/** Cancel persistence of a stored server trust credential.
 * 
 * Call this function to cancel the persistence of a server trust
 * credential. Persistence must have been set via the
 * <tt>NSURLCredential(GDNET)</tt> <tt>gdPersistence</tt> property, so that the
 * credential is being managed by the BlackBerry Dynamics runtime.
 *
 * Credentials managed by the BlackBerry Dynamics runtime are stored
 * persistently in the BlackBerry Dynamics secure store on the mobile device.
 * The URL of the <tt>NSMutableURLRequest</tt> object on which this method is
 * called will be used by the runtime to identify the server certificate.
 *
 * @return <tt>YES</tt> if a persistent server trust credential was found and
 *         cancelled.
 * @return <tt>NO</tt> otherwise.
 *
 * @see The class reference of the \ref NSURLCredential(GDNET) category for the
 *      programming interface and implications of setting persistence.
 */
- (BOOL)undoPriorTrustDecision;

@end
