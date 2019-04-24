/*
 * (c) 2017 BlackBerry Limited. All Right Reserved.
 * 
 */

#ifndef __GD_URL_REQUEST_CONNECTION_DELEGATE_H__
#define __GD_URL_REQUEST_CONNECTION_DELEGATE_H__

#import <Foundation/Foundation.h>
#import "GDPortability.h"

GD_NS_ASSUME_NONNULL_BEGIN

/** \defgroup gdurlconnectionmanagmentconstants BlackBerry Dynamics URL Connection Management Constants
 * Use these constants in a BlackBerry Dynamics URL connection management
 * delegate. See the \link GDURLRequestConnectionDelegate
 * GDURLRequestConnectionDelegate class reference\endlink for details.
 * 
 * \{
 */

/** Routing specifier for direct connection to the Internet.
 * 
 * Routing specifier for URL requests that will be, or have been, sent over a
 * direct Internet connection and <em>not </em>via the BlackBerry Dynamics proxy
 * infrastructure.
 */
extern NSString* GDRouteInternet;

/** Routing specifier for connection via the BlackBerry Dynamics proxy
 *  infrastructure.
 * 
 * Routing specifier for URL requests that will be, or have been, sent via the
 * BlackBerry Dynamics proxy infrastructure.
 */
extern NSString* GDRouteGoodProxy;

/** \}
 */

/** Delegate for managing the URL connections associated with a UIWebView
 *  control.
 * 
 * Connections for URL requests issued by a <tt>UIWebView</tt> control can be
 * managed by creating a class that implements this protocol.
 *
 * The callbacks in this protocol enable the application to:
 * - Respond to a certificate authentication challenge.
 * - Block a connection.
 * - Specify whether a connection will be routed direct to the Internet or via
 *   the BlackBerry Dynamics proxy infrastructure.
 * - Confirm that a connection was made, and confirm its routing.
 * .
 *
 * Implement this protocol if any of the following is required by the
 * application:
 * - Custom verification of electronic certificates for Secure Socket Layer or
 *   Transport Layer Security (SSL/TLS) connections.
 * - Mutual authentication of electronic certificates for SSL/TLS connections.
 * - Finer control over routing and filtering than is offered by the BlackBerry
 *   Dynamics management console client connection configuration.
 * .
 * But note that this protocol doesn't enable routing to an application server
 * that is behind the enterprise firewall and that hasn't been configured in
 * the enterprise management console.
 *
 * Call the <tt>GDSetRequestConnectionDelegate:</tt> function in the
 * \link UIWebView(GDNET)\endlink category to set the delegate for a particular
 * <tt>UIWebView</tt> instance. The delegate callbacks will be executed on the
 * same thread in which the delegate was set.
 *
 * The callbacks in this protocol utilize an <tt>NSURLRequest</tt> object to
 * represent the request to which the callback invocation relates. For details
 * of how to access its attributes, see the <a
 *     HREF="https://developer.apple.com/reference/foundation/nsurlrequest?language=objc"
 *     target="_blank"
 * >NSURLRequest class reference</a> on the apple.com developer website.
 *
 * @see \ref GC.
 */
@protocol GDURLRequestConnectionDelegate <NSObject>

@optional

/** Invoked before a request is sent.
 * 
 * This callback is invoked when a request is about to be sent or a connection
 * is about to be established. A specifier for the routing that is going to be
 * used for the connection is passed as a parameter.
 *
 * The specifier is passed as the location of a pointer to <tt>NSString</tt>, i.e. a
 * pointer to a pointer. The function that is invoked can check the routing
 * specified by comparing the pointed-to value with the
 * \ref gdurlconnectionmanagmentconstants.
 *
 * The function that is invoked can specify a different route by overwriting the
 * pointer with the address of an <tt>NSString</tt> of its own. The value of the
 * <tt>NSString</tt> must be the same as one of the connection management constants.
 *
 * The function that is invoked can also block the connection from being
 * established by doing either of the following:
 * - Overwriting the pointer with <tt>nil</tt>.
 * - Returning <tt>NO</tt>.
 * .
 *
 * @param request <tt>NSURLRequest</tt> representing the request that will be
 *                sent.
 * 
 * @param route location of a pointer to <tt>NSString</tt> specifying the
 *              routing of the request. The value will be one of the
 *              \ref gdurlconnectionmanagmentconstants.
 * 
 * @return <tt>YES</tt> to allow the request to be sent, via the original or changed
 *         route.
 * @return <tt>NO</tt> to block the request.
 */
- (BOOL)GDRequest:(NSURLRequest*)request willConnectVia:(NSString * GD_NSNULLABLE_POINTER * GD_NSNULLABLE_POINTER)route;

/** Invoked after a request has been sent.
 * 
 * This callback is invoked after a URL request has been sent or a new
 * connection was established. A specifier for the routing that was used for the
 * request is passed as a parameter.
 *
 * @param request <tt>NSURLRequest</tt> representing the request that was sent.
 * 
 * @param route <tt>NSString</tt> specifying the route of the connection. The
 *              value will be one of the \ref gdurlconnectionmanagmentconstants.
 */
- (void)GDRequest:(NSURLRequest*)request didConnectVia:(NSString*)route;

/** Invoked when authentication is requested.
 * 
 * This callback is invoked during SSL/TLS connection negotiation and when HTTP
 * resource authentication is required, once for each authentication challenge.
 * There are six types of challenge, all of which could occur during loading of
 * a document:
 * - An <tt>NSURLAuthenticationMethodServerTrust</tt> challenge occurs when a
 *   host certificate is received and requires verification. Note that
 *   verification isn't required from the application code if the BlackBerry
 *   Dynamics runtime was able to verify the host certificate itself. In that
 *   case, this callback isn't invoked.
 * - An <tt>NSURLAuthenticationMethodClientCertificate</tt> challenge occurs
 *   when the host requests a client certificate for mutual certificate
 *   authentication.
 * - An <tt>NSURLAuthenticationMethodNegotiate</tt> challenge occurs when access
 *   to the requested resource requires Kerberos or NTLM authentication.
 * - An <tt>NSURLAuthenticationMethodNTLM</tt> challenge occurs when access to
 *   the requested resource requires NTLM authentication.
 * - An <tt>NSURLAuthenticationMethodHTTPDigest</tt> challenge occurs when
 *   access to the requested resource requires HTTP Digest authentication.
 * - An <tt>NSURLAuthenticationMethodHTTPBasic</tt> challenge occurs when access
 *   to the requested resource requires HTTP Basic authentication.
 * .
 * The type of the authentication challenge can be determined from the
 * <tt>challenge</tt> parameter. See the <a
 *     HREF="https://developer.apple.com/reference/foundation/nsurlauthenticationchallengesender?language=objc"
 *     target="_blank"
 * >NSURLAuthenticationChallenge class reference</a> on the apple.com developer
 * website.
 *
 * The function that is invoked can respond to the challenge by calling the
 * <tt>NSURLAuthenticationChallengeSender</tt> methods on the object in the
 * <tt>challenge</tt> parameter. For details of the programming interface, see
 * the <a
 *     HREF="https://developer.apple.com/reference/foundation/nsurlauthenticationchallengesender?language=objc"
 *     target="_blank"
 * >NSURLAuthenticationChallengeSender protocol reference</a> on the apple.com
 * developer website. The <tt>NSURLAuthenticationChallengeSender</tt> methods
 * must be used within one second of the invocation or no response will be
 * assumed.
 *
 * The handling of a challenge can include the creation of a credential object,
 * which could utilize the \ref NSURLCredential(GDNET) category programming
 * interface.
 *
 * If host certificate verification is required and the application doesn't
 * respond, then the connection won't be established. If a client certificate is
 * requested and the application doesn't respond, then the connection might or
 * might not be established, depending only on the configuration of the host
 * server. If an HTTP resource requires authentication and the application
 * doesn't respond, the request will eventually fail with a time out.
 *
 * @param request <tt>NSURLRequest</tt> representing the request that will be
 *                sent if SSL/TLS negotiation succeeds.
 * 
 * @param challenge <tt>NSURLAuthenticationChallenge</tt> object that:
 *                  - Contains the challenge details.
 *                  - Implements the <tt>NSURLAuthenticationChallengeSender</tt>
 *                    protocol.
 *                  .
 */
- (void)GDRequest:(NSURLRequest*)request willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge*)challenge;

@end

GD_NS_ASSUME_NONNULL_END

#endif
