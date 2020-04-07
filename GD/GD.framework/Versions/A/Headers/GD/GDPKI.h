/*
 * (c) 2018 BlackBerry Limited. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

/** Certificate addition notification identifier.
 * 
 * Use this value to add an observer of additions to the BlackBerry Dynamics
 * X.509 certificate store.
 * 
 * @see \ref GDPKICertificate.
 */
extern NSString* const GDPKINotificationCertificateAdded;

/** Certificate removal notification identifier.
 * 
 * Use this value to add an observer of removals from the BlackBerry Dynamics
 * X.509 certificate store.
 * 
 * @see \ref GDPKICertificate.
 */
extern NSString* const GDPKINotificationCertificateRemoved;

/** X.509 Public Key Certificate.
 * 
 * Objects of this class are used to represent X.509 public key
 * certificates in the BlackBerry Dynamics secure store. Certificates in the
 * store could be used as part of integration with an enterprise public key
 * infrastructure.
 * 
 * The properties of this class correspond to the standard fields of an X.509
 * public key certificate.
 * 
 * @see <a
            href="https://tools.ietf.org/html/rfc3280"
            target="_blank"
        >RFC 3280</a> and <a
            href="https://tools.ietf.org/html/rfc5280"
            target="_blank"
        >RFC 5280</a> on the ietf.org website.
 *
 * <h2>Public Key Infrastructure Integration</h2>
 * BlackBerry Dynamics can be integrated into a public key
 * infrastructure (PKI) implementation. BlackBerry Dynamics has a number of
 * capabilities for handling the X.509 public key certificates that would be
 * associated with an end user within an enterprise PKI implementation.
 *
 * <h2>Certificate Store Notifications</h2>
 * The BlackBerry Dynamics runtime maintains a secure certificate store on the
 * device. The application code can be notified when certificates are added to,
 * and removed from, the secure certificate store.
 * 
 * The typical usage of the notification interface is as follows.
 * -# The application code
 *    implements and adds a notification observer, using the native
 *    <tt>NSNotificationCenter</tt> programming interface.
 * -# When the BlackBerry Dynamics runtime adds an X.509 certificate to its
 *    store, a notification is dispatched to the
 *    observer. The notification includes a
 *    reference to an object that represents the certificate.
 * -# The application code in the observer extracts
 *    the certificate object from the notification.
 * -# The application code can read the object properties to determine the
 *    characteristics of the certificate.
 * .
 * The available notifications are:
 * - \ref GDPKINotificationCertificateAdded for when a certificate is added to
 *   the BlackBerry Dynamics certificate store.
 * - \ref GDPKINotificationCertificateRemoved for when a certificate is removed
 *   from the BlackBerry Dynamics certificate store.
 * .
 * 
 * In all cases, the object of the notification will be an instance of this
 * class that represents the certificate.
 *
 * @see <a
            HREF="https://developer.apple.com/library/ios/documentation/Cocoa/Reference/Foundation/Classes/NSNotificationCenter_Class/"
            target="_blank"
        >NSNotificationCenter class reference</a
        > on the apple.com developer website.
 * @see \ref clientcertificateimport documentation for a related interface.
 * 
 * <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Certificate Notification Observer</h3>
 * \code
 * - (void)addCertificateObservers {
 *     [[NSNotificationCenter defaultCenter] addObserver:self
 *                                              selector:@selector(certificateAdded:)
 *                                                  name:GDPKINotificationCertificateAdded
 *                                                object:nil];
 *     [[NSNotificationCenter defaultCenter] addObserver:self
 *                                              selector:@selector(certificateRemoved:)
 *                                                  name:GDPKINotificationCertificateRemoved
 *                                                object:nil];
 * }
 *
 * - (void)certificateAdded:(NSNotification *)nsNotification {
 *     GDPKICertificate *gdPKICertificate = nsNotification.object;
 *     SecCertificateRef secCertificate =
 *         SecCertificateCreateWithData(NULL, gdPKICertificate.binaryX509DER);
 *     CFStringRef secCertificateSummary =
 *         SecCertificateCopySubjectSummary(secCertificate);
 *     NSLog(@"Certificate added. Serial Number: \"%s\", Summary:\"%s\".\n",
 *           gdPKICertificate.serialNumber, (NSString*)secCertificateSummary);
 *     CFRelease(secCertificateSummary);
 *     CFRelease(secCertificate);
 * }
 *
 * - (void)certificateRemoved:(NSNotification *)nsNotification {
 *     GDPKICertificate *gdPKICertificate = nsNotification.object;
 *   
 *     NSLog(@"Certificate removed. Serial Number: \"%@\".\n",
 *           gdPKICertificate.serialNumber );
 * }
 * \endcode
 * \htmlonly<div class="bulletlists">\endhtmlonly
 * The above snippet shows:
 * - Registration for notification of certificate addition and removal. The
 *   observer code is specified by selector.
 * - Dummy implementation of the certificate added listener that:
 *   - Logs one field directly from the notification object, which is a
 *     BlackBerry Dynamics representation of a certificate.
 *   - Creates a native representation of the same certificate, and logs another
 *     field from it.
 *   .
 * - Dummy implementation of the certificate removed listener that logs one
 *   X.509 field.
 * .
 * \htmlonly</div>\endhtmlonly
 * @see <a
 *          href="https://developer.apple.com/documentation/security/certificate_key_and_trust_services/certificates?language=objc"
 *          target="_blank"
 *      >Certificates reference documentation</a> on the apple.com developer
 *      website.
 */
@interface GDPKICertificate : NSObject

/** Initialize from binary DER encoded X.509 certificate data.
 * 
 * Call this function to initialize a new object from binary DER encoded X.509
 * certificate data.
 *
 * @param x509 <tt>NSData</tt> containing the binary DER encoded X.509 data.
 */
- (instancetype)initWithData:(NSData*)x509;

/** Binary DER encoded certificate data.
 * 
 * Binary DER encoded representation of the X.509 certificate data.
 */
@property (atomic, readonly) NSData *binaryX509DER;

/** X.509 version.
 * 
 * The X.509 version of the certificate.
 */
@property (atomic, readonly) NSInteger version;

/** X.509 Serial Number field.
 * 
 * Value of the X.509 Serial Number field of the certificate.
 */
@property (atomic, readonly) NSString *serialNumber;

/** X.509 Subject field.
 * 
 * Value of the X.509 Subject field of the certificate.
 */
@property (atomic, readonly) NSString *subjectName;

/** X.509 Subject Alternative Name field.
 * 
 * Value of the X.509 Subject Alternative Name field of the certificate.
 */
@property (atomic, readonly) NSString *subjectAlternativeName;

/** X.509 Issuer field.
 * 
 * Value of the X.509 Issuer field of the certificate.
 */
@property (atomic, readonly) NSString *issuer;

/** X.509 Validity: Not Before date and time.
 * 
 * Value of the X.509 Validity: Not Before date and time of the certificate.
 */
@property (atomic, readonly) NSDate *notBeforeDate;

/** X.509 Validity: Not After date and time.
 * 
 * Value of the X.509 Validity: Not After date and time of the certificate.
 */
@property (atomic, readonly) NSDate *notAfterDate;

/** X.509 Key Usage field.
 * 
 * Value of the X.509 Key Usage field of the certificate.
 */
@property (atomic, readonly) NSString *keyUsage;

@end
