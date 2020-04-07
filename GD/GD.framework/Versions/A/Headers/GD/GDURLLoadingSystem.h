/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

/** Manage access across the firewall via the URL Loading System.
 * 
 * BlackBerry Dynamics applications can utilize the native URL Loading System to
 * communicate with servers that are behind the enterprise firewall.
 * Communication across the enterprise firewall utilizes the BlackBerry Dynamics
 * proxy infrastructure, which is secure.
 *
 * Access across the firewall is enabled in the URL Loading System by default,
 * when authorization of the application succeeds. The application can
 * subsequently disable and enable access. If the application's or the user's
 * authorization is withdrawn, access is implicitly disabled for the duration of
 * the withdrawal.
 *
 * Access across the firewall utilizes the BlackBerry Dynamics proxy
 * infrastructure. The BlackBerry Dynamics SDK includes a class that interfaces
 * with the infrastructure. The interfacing class is also compatible to be
 * registered as a URL handler in the URL Loading System. Enabling and disabling
 * access across the firewall actually registers and de-registers the
 * interfacing class. This means that, when access is disabled, the default URL
 * Loading System handlers will service any URL requests.
 *
 * <B>Note that synchronous request calls shouldn't be made from the main
 * application thread.</B>
 *
 * \htmlonly<div class="bulletlists">\endhtmlonly
 * When access is enabled, the normal URL Loading System classes can be used to
 * communicate with servers that are behind the firewall, using standard
 * Internet protocols. For an overview, see the <a
       HREF="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/URLLoadingSystem/URLLoadingSystem.html"
       target="_blank"
   >URL Session Programming Guide</a> on the apple.com developer website.
 *
 * Note the following details.
 * - Only the HTTP and HTTPS schemes are ever handled by the BlackBerry Dynamics
 *   runtime. FTP and other schemes are never handled, and hence cannot be used
 *   to access resources that are behind the enterprise firewall.
 * - Enterprise servers that are not listed in the management console cannot be
 *   accessed, unless they are accessible from the Internet.
 * - Authorization credentials are supported with the following notes.\n
 *   <TT>NSURLAuthenticationMethodHTTPBasic</TT> is supported.\n
 *   <TT>NSURLAuthenticationMethodDefault</TT> is treated as
 *   <TT>NSURLAuthenticationMethodHTTPBasic</TT>.\n
 *   <TT>NSURLAuthenticationMethodHTTPDigest</TT> is supported.\n
 *   <TT>NSURLAuthenticationMethodNTLM</TT> is supported, specifically: NTLMv1,
 *   NTLMv2, and NTLM2 Session.\n
 *   <TT>NSURLAuthenticationMethodNegotiate</TT> is supported for Kerberos
 *   version 5.
 * - When Kerberos authentication is in use, note the following.
 *   - Credentials are initially taken in the same way as other credentials,
 *     with the same specified persistence.
 *   - The credentials will be a username in the form
 *     <EM>user</EM><TT>\@</TT><EM>realm</EM>, and a password.
 *   - The credentials are used to request Kerberos tickets, which are stored.
 *   - The stored Kerberos tickets are then used to authenticate the user on
 *     any site that supports Kerberos authentication. So long as the ticket
 *     continues to be accepted, there is no need for credentials to be supplied
 *     again, and no authentication challenge.
 *   - This continues until a site does not accept the stored ticket (e.g. the
 *     ticket has expired and cannot be renewed).
 *   - The Kerberos realm must be accessible. Usually, this means that the
 *     Kerberos realm must be listed as an Additional Server in the enterprise
 *     management console. See \ref GC.
 *   - Kerberos delegation can be allowed or disallowed. See
 *     \link GDCacheController::kerberosAllowDelegation: \endlink.
 *   .
 * - Authorization credentials are persisted as per the
 *   <TT>NSURLCredentialPersistence</TT> flag of <TT>NSURLCredential</TT>:\n
 *   <TT>NSURLCredentialPersistenceNone</TT> credential is used for this
 *   connection only\n
 *   <TT>NSURLCredentialPersistenceForSession</TT> credential is persisted in
 *   memory\n
 *   <TT>NSURLCredentialPersistencePermanent</TT> treated as
 *   <TT>NSURLCredentialPersistenceForSession</TT>\n
 * - The BlackBerry Dynamics secure store is utilized, as follows.
 *   - Browser cookies are persisted in the secure store. See HTTP Cookie
 *     Handling below.
 *   - The default store is not used to cache retrieved files. Instead, a
 *     separate secure cache is used.
 *   - Kerberos tickets are persisted in the secure store.
 *   .
 *   General access to these secure stores by the application is not
 *   supported, but see \ref GDCacheController.
 * - Data communication does not go via the proxy specified in the native
 *   settings of the device or computer, if any.
 * - The value <TT>YES</TT> is assumed for the <TT>setAllowsCellularAccess</TT>
 *   flag. See the <a
 *       HREF="https://developer.apple.com/reference/foundation/nsmutableurlrequest?language=objc"
 *       target="_blank"
 *   >NSMutableURLRequest class reference</a> on the apple.com developer website
 *   for details of this flag.
 * .
 * \htmlonly</div>\endhtmlonly
 *
 * Additional features are made available, using the
 * \ref NSMutableURLRequest(GDNET) category, and the 
 * \ref NSURLCache(GDURLCache)
 * subclass.
 *
 * @see \ref GDiOS, for BlackBerry Dynamics authorization
 * @see \ref background_execution
 * @see <a  HREF="https://docs.blackberry.com/en/endpoint-management/blackberry-uem/"  target="_blank" >Manuals page for the BlackBerry Dynamics enterprise servers</a > for the Platform Overview.
 * @see \ref GC.
 * @see \ref GDNetUtility
 * @see \ref nsurlsession_support
 *
 * <H3>Multiple Authentication Methods</H3>
 * An HTTP server may support multiple authentication methods. For example, a
 * server could support both NTLM and Kerberos authentication. By default, the
 * BlackBerry Dynamics runtime handles this by selecting the first
 * authentication method presented.
 *
 * The application can implement its own handling for multiple authentication
 * methods, as follows.
 * \deprecated The GDRejectAuthSupport key in Info.plist is deprecated and will
 *             be removed in a future release. It is no longer required.
 *             
 * -# Add a new row to the application's Info.plist fi<TT></TT>le:
 *    - Key: <tt>GDRejectAuthSupport</tt>
 *    - Type: <tt>Boolean</tt>
 *    - Value: <tt>YES</tt>
 *    .
 *    (In case there are multiple Info.plist files, check that the correct one
 *    has been edited by opening the Info tab of the application target being
 *    built. The setting just made should appear there.)
 * -# Implement a <tt>connection::willSendRequestForAuthenticationChallenge</tt>
 *    callback.
 * -# In the application code for the callback, call the
 *    <tt>rejectProtectionSpaceAndContinueWithChallenge</tt> function, where
 *    necessary.
 * .
 * The following code snippet illustrates a simple implementation of the
 * callback mentioned in the above.
 * \code
 * - (void)connection:
 *     (NSURLConnection *)connection
 * willSendRequestForAuthenticationChallenge:
 *     (NSURLAuthenticationChallenge *)challenge
 * {
 *     if(
 *         [[challenge protectionSpace] authenticationMethod] !=
 *         NSURLAuthenticationMethodNTLM
 *     ) {
 *         // Reject anything that is not NTLM, i.e. reject Negotiate.
 *         [[challenge sender]
 *             rejectProtectionSpaceAndContinueWithChallenge:challenge];
 *     }
 *     else {
 *         NSURLCredential* cred =
 *             [NSURLCredentialcredentialWithUser:@"abc"
 *                                       password:@"abc"
 *                                    persistence:NSURLCredentialPersistenceForSession];
 *
 *         [[challenge sender] useCredential:cred
 *                forAuthenticationChallenge:challenge];
 *     }
 * }
 * \endcode
 *
 * @see <a
     HREF="https://developer.apple.com/reference/foundation/nsurlconnectiondelegate?language=objc"
     target="_blank"
   >NSURLConnectionDelegate protocol reference</A> on the apple.com developer
 * website.
 *
 * 
 * <h3>HTTP Cookie Handling</h3>
 * By default, HTTP cookies received through BlackBerry Dynamics secure
 * communication are handled automatically:
 * - Set-cookie: headers that are received as part of an HTTP response are
 *   processed and then added to subsequent matching requests.
 * - Persistent cookies are written to cookie storage in the BlackBerry Dynamics
 *   secure store.
 * .
 * 
 * The BlackBerry Dynamics cookie store persists between executions of the
 * application, and if the mobile device is switched off.
 * The contents of the store can be managed with the native
 * <tt>NSHTTPCookieStorage</tt> programming interface, and so can non-persistent
 * cookies received through BlackBerry Dynamics secure communication.
 *
 * Automatic handling of HTTP cookies received through BlackBerry Dynamics
 * secure communication can be switched off, as follows.
 * - For communication that goes via the <tt>GDURLLoad</tt><tt>ingSystem</tt>,
 *   such as <tt>NSURLSession</tt> and <tt>NSURLConnection</tt>, call the
 *   <tt>setHTTPShouldHandleCookies:</tt> method of the associated
 *   <tt>NSMutableURLRequest</tt> instance.
 * .
 * @see <a
 *     HREF="https://developer.apple.com/reference/foundation/nshttpcookiestorage"
 *     target="_blank"
 * >NSHttpCookieStorage class reference</a> on the apple.com developer website.
 * @see <a
 *     HREF="https://developer.apple.com/reference/foundation/nsmutableurlrequest?language=objc"
 *     target="_blank"
 * >NSMutableURLRequest class reference</a> on the apple.com developer website.
 */
@interface GDURLLoadingSystem : NSObject {
}


/** Enable access across the enterprise firewall via the URL Loading System.
 * 
 * Call this function to enable, or re-enable access across the enterprise
 * firewall via the URL Loading System.
 *
 * Access across the enterprise firewall is provided by the BlackBerry Dynamics
 * proxy infrastructure. Only servers that have been specifically listed in the
 * enterprise management console are accessible. See \ref GC.
 *
 * Access is enabled by default during authorization processing, which is
 * initiated by the \reflink GDiOS::authorize: authorize (GDiOS) \endlink function. The
 * <TT>enableSecure</TT><TT>Communication</TT> function need only be called if
 * access was disabled after authorization, see the
 * \ref disableSecureCommunication function, below.
 */
+ (void) enableSecureCommunication;

/** Disable access across the enterprise firewall via the URL Loading System.
 * 
 * Call this function to disable access across the enterprise firewall via the
 * URL Loading System.
 *
 * Access can be re-enabled using the \ref enableSecureCommunication function.
 * Access will also be re-enabled during authorization processing, which is
 * initiated by the \reflink GDiOS::authorize: authorize (GDiOS) \endlink function.
 *
 * Calling this function has no impact on access across the firewall using
 * \ref GDHttpRequest (deprecated).
 */
+ (void) disableSecureCommunication;

/** Check whether access across the enterprise firewall via the URL Loading
 *  System is enabled.
 * 
 * Call this function to check whether access across the enterprise firewall via
 * the URL Loading System is enabled.
 *
 * @return <tt>YES</tt> if access is enabled.
 * @return <tt>NO</tt> otherwise.
 *
 * @see \ref enableSecureCommunication
 * @see \ref disableSecureCommunication
 */
+ (BOOL) isSecureCommunicationEnabled;

/** Custom protocol class.
 *
 * This function returns an object that is suitable to be included in an
 * <tt>NSURLSessionConfiguration</tt> <tt>protocolClasses</tt> list.
 *
 * Note that isn't generally necessary for the application code to manipulate
 * the protocol classes list in a session configuration. This is because the
 * BlackBerry Dynamics runtime configures the list automatically. This
 * function is provided for completeness.
 *
 * @return <tt>Class</tt> object.
 */
+ (Class) getProtocolClass;

/** Configure WKWebView instances for BlackBerry Dynamics (class property).
 *
 * The BlackBerry Dynamics runtime checks this property when the application
 * code creates a WKWebView instance.
 * - If <tt>YES</tt>, the new instance will be configured for BlackBerry Dynamics
 *   support. HTTP requests made from the instance may utilize the BlackBerry
 *   Dynamics proxy infrastructure. The instance will use the BlackBerry
 *   Dynamics secure store to cache data.
 * - If <tt>NO</tt>, the new instance won't be configured for BlackBerry Dynamics.
 * .
 *
 * The runtime sets this property to <tt>YES</tt> when authorization processing
 * completes with success. Set this property, if required, before creating a
 * WKWebView instance.
 *
 * Setting this property doesn't change the configuration of WKWebView instances
 * that are already running.
 *
 * This is a class property.
 */
@property (class, nonatomic, assign) BOOL supportWKWebView;

@end
