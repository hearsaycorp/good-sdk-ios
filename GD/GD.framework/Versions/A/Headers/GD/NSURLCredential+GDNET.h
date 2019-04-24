/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#pragma once

#import <Foundation/Foundation.h>

/** NSURLCredential category with additional features.
 * 
 * This class is a category of the Foundation <tt>NSURLCredential</tt> class
 * that can be used when the BlackBerry Dynamics proxy infrastructure is in use
 * via the URL Loading System (see \ref GDURLLoadingSystem). This class provides
 * the ability to set the persistence of credentials.
 *
 * @see <a
 *          HREF="https://developer.apple.com/reference/foundation/nsurlcredential"
 *          target="_blank"
 *      >NSURLCredential class reference</a> on the apple.com developer website.
 */
@interface NSURLCredential (GDNET)

/** Credential persistence.
 * 
 * Set this property of an <tt>NSURLCredential</tt> object to specify the
 * persistence for the credential that it represents. This property can be set
 * on a server trust credential that is created as part of the processing of an
 * <tt>NSURLAuthenticationMethodServerTrust</tt> authentication challenge.
 *
 * The property can be set to one of the following values:
 * - <tt>NSURLCredentialPersistenceNone</tt> Credential won't be stored
 *   persistently.
 * - <tt>NSURLCredentialPersistencePermanent</tt> Credential will be stored
 *   persistently in the application's BlackBerry Dynamics secure store.
 * .
 * 
 * If the credential is stored persistently then it will be reused automatically
 * every time an authentication challenge for the same protection space is
 * received. This means that the following callbacks won't be invoked:
 * - <tt>NSURLConnectionDelegate willSendRequestForAuthenticationChallenge</tt>
 * - <tt>GDURLRequestConnectionDelegate
 *   willSendRequestForAuthenticationChallenge</tt>
 * .
 * 
 * Automatic reuse continues until one of the following occurs:
 * - The server presents different certificate details.
 * - The application cancels persistence, by calling the
 *   <tt>undoPriorTrustDecision</tt> function.
 * .
 *
 * See the class reference of the \ref NSMutableURLRequest(GDNET) category for
 * details of the <tt>undoPriorTrustDecision</tt> function.
 */
@property (readwrite) NSUInteger gdPersistence;

/** Whether to request Kerberos tickets that can be delegated.
 * 
 * Set this property of an <tt>NSURLCredential</tt> object to specify whether to
 * request tickets that can be delegated when the credential that it represents
 * it used for Kerberos authentication.
 *
 * Even if this property is set to <tt>YES</tt>, a ticket that can be delegated
 * might not be issued. Delegation also depends on configuration in the Kerberos
 * Key Distribution Center (KDC) at the enterprise.
 */
@property (readwrite) BOOL gdKerberosAllowDelegation;

@end
