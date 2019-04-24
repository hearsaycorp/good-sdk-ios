/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 *
 */

#ifndef GDDiagnostic_h
#define GDDiagnostic_h

/** BlackBerry Dynamics diagnostic checks and information.
 *
 * This class contains a number of programming interfaces for running
 * checks and retrieving information for diagnostic purposes.
 *
 * Use the application programming interface (API) of this class if the
 * application encounters an error that might involve the BlackBerry Dynamics
 * infrastructure or configuration in some way. The API includes functions
 * to:
 * - Check that an application server can be reached by BlackBerry Dynamics
 *   secure communication.
 * - Check that the enterprise BlackBerry Dynamics management console can be
 *   reached.
 * - Retrieve current application configuration settings.
 * .
 *
 * The return values are generally structured text that is intended to be logged
 * by the application and then made available to technical support staff. The
 * structure and content aren't documented.
 */
@interface GDDiagnostic : NSObject

/** \defgroup gdreachabilityerrors BlackBerry Dynamics Reachability Errors
 * \{
 */

/** Error codes for reachability checks.
 * 
 * These constants can be used for handling errors returned by BlackBerry
 * Dynamics reachability diagnostic checks.
 *
 * @see \ref GDDiagnostic class reference.
 */
typedef NS_ENUM(NSInteger, GDDiagnosticReachabilityErrorType)
{
    /** No error.
     */
    GDDiagnosticNoError=0,

    /** No network connection.
     */
    GDDiagnosticNoNetwork,

    /** Network changed during reachability check.
     */
    GDDiagnosticNetworkChange,

    /** Time out expired before reachability results could be generated.
     */
    GDDiagnosticTimedOut
};

/** \}
 */

#pragma mark Singleton

/** Get a reference to the BlackBerry Dynamics diagnostic object.
 * 
 * This function returns a reference to the BlackBerry Dynamics diagnostic
 * object, which is a "singleton class".
 *
 * @return Reference that can be used to call, for example, the
 *         \ref currentSettings function.
 */
+ (GDDiagnostic *)sharedInstance;


#pragma mark Reachability

/** \defgroup gddiagnosticreachabilityresultsblock Type for check reachability results callback
 * \{
 */

/** Type for check reachability results callback.
 * 
 * Pass a code block of this type as the <tt>block</tt> parameter to any of the
 * reachability checking functions in the \ref GDDiagnostic class.
 *
 * The block receives the following parameters.
 * @param results <tt>NSString</tt> containing results of the reachability check, which
 *                can be logged by the application for diagnostic purposes.
 * @param requestID <tt>NSInteger</tt> value that was returned by the original
 *                  check reachability function call.
 */
typedef void (^GDDiagnosticReachabilityResultsBlock)(NSString* results,
                                                     NSInteger requestID);
/** \}
 */

/** Check that an application server can be reached.
 * 
 * Call this function to check the reachability of an application
 * server by BlackBerry Dynamics communication. The application server endpoint
 * is specified as a host name and port number.
 *
 * When this function is called, the BlackBerry Dynamics runtime will make a
 * number of attempts to reach the specified server. One or more routes may be
 * used, depending on enterprise configuration. For example, the runtime could
 * attempt to connect through the BlackBerry Dynamics proxy infrastructure via
 * an endpoint behind the enterprise firewall. The results of all attempts are
 * returned in a single text string that can be logged by the application for
 * diagnostic purposes.
 *
 * This function is asynchronous. Results are returned by invocation of a
 * code block.
 * 
 * Every call to this function is issued a unique request identifier. The
 * identifier is returned synchronously, as the return value of this
 * function, and asynchronously, with the results. The identifier can be
 * used to match results with calls, in the case that the application initiates
 * multiple concurrent reachability checks.
 *
 * A time out can be specified. If it is, and checking hasn't completed when the
 * time out expires, then the results up to that point are returned.
 *
 * @param host <tt>NSString</tt> containing the hostname of the server to check.
 * @param port <tt>NSInteger</tt> for the port
 *             number to check.
 * @param ssl <tt>YES</tt> to check reachability using a Secure Socket Layer
 *            connection or Transport Layer Security (SSL/TLS).\n
 *            <tt>NO</tt> to use a plain socket connection without SSL/TLS.
 * @param validateCertificate If using SSL/TLS:\n
 *                            <tt>YES</tt> to require a valid certificate, and treat
 *                            an invalid certificate as a connection error.\n
 *                            <tt>NO</tt> to switch off certificate checking.\n
 *                            If the <tt>ssl</tt> parameter is <tt>NO</tt>, this
 *                            flag is ignored.
 * @param timeOut <tt>NSTimeInterval</tt>
 *                representing the time out, in seconds.
 * @param block \link gddiagnosticreachabilityresultsblock
 *              GDDiagnosticReachabilityResultsBlock\endlink for the results
 *              callback.
 * 
 * @return <tt>NSInteger</tt> value that
 *         identifies this call.
 */
- (NSInteger)checkApplicationServerReachability:(NSString*)host
                                       withPort:(NSInteger)port
                                         useSSL:(BOOL)ssl
                            validateCertificate:(BOOL)validateCertificate
                                    withTimeOut:(NSTimeInterval)timeOut
                                  callbackBlock:(GDDiagnosticReachabilityResultsBlock)block;

/** Check that the BlackBerry Dynamics management console can be reached.
 * 
 * Call this function to check the reachability of the BlackBerry
 * Dynamics management console. The management console is a service within
 * every BlackBerry Dynamics deployment, with which the end user of the
 * application is associated by BlackBerry Dynamics activation processing. The
 * service can run on a single server, or on a cluster of servers operating as a
 * node.
 *
 * When this function is called, the BlackBerry Dynamics runtime will make a
 * number of attempts to reach each of the servers that provide the management
 * console service. One or more routes may be used for each server, depending on
 * enterprise configuration. For example, the runtime could attempt to connect
 * through the BlackBerry Dynamics proxy infrastructure via an endpoint behind
 * the enterprise firewall. The results of all attempts are returned in a number
 * of text strings, one for each server. The text strings can be logged by the
 * application for diagnostic purposes.
 *
 * This function is asynchronous. Results are returned by multiple
 * invocations of a code block.
 *
 * Every call to this function is issued an array of unique request
 * identifiers. The array is returned synchronously, as the return value of this
 * function. Individual values within the array are also returned
 * asynchronously, with each of the results. The identifier can be used to match
 * results with calls, in the case that the application initiates multiple
 * concurrent reachability checks.
 *
 * A time out can be specified. If it is, and checking hasn't completed when the
 * time out expires, then the results up to that point are returned.
 *
 * @param timeOut <tt>NSTimeInterval</tt>
 *                representing the time out, in seconds.
 * @param block \link gddiagnosticreachabilityresultsblock
 *              GDDiagnosticReachabilityResultsBlock\endlink for the results
 *              callback.
 * @return <tt>NSArray</tt> of <tt>NSInteger</tt>
 *         containing identifier values.
 */
- (NSArray*)checkManagementConsoleReachability:(NSTimeInterval)timeOut
                                 callbackBlock:(GDDiagnosticReachabilityResultsBlock)block;

#pragma mark Configs

/** Current application configuration settings.
 * 
 * 
  * A number of current
  * application configuration settings, which can be logged by the application
  * for diagnostic purposes.
 */
@property (nonatomic, readonly) NSString* currentSettings;

@end

#endif /* GDDiagnostic_h */
