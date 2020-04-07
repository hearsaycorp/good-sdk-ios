/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#ifndef ios_GDNetUtility_h
#define ios_GDNetUtility_h

#import <Foundation/Foundation.h>

/* \cond DOXYGEN_IGNORE */
// See: http://clang.llvm.org/docs/LanguageExtensions.html
#ifndef __has_extension
#define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif
/* \endcond */

/** Enumerated constants for use with the BlackBerry Dynamics name service.
 * 
 * Use these constants to specify the type of results required for a BlackBerry
 * Dynamics name service lookup. The type parameter of the
 * \link GDNetUtility::nslookup:type:completion: nslookup\endlink function
 * always takes one of these values.
 */
typedef NS_ENUM(NSInteger, GDNslookupType)
{
    /** Specify a CNAME lookup.
     */
    GDNslookupCNAME			 = 0,

    /** Specify an A record lookup.
     */
    GDNslookupARECORD,
};

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_ERRORHOSTNOTLISTED __attribute__((deprecated("No longer required")))
#else
#   define DEPRECATE_ERRORHOSTNOTLISTED __attribute__((deprecated))
#endif
/* \endcond */

/**
 * \defgroup gdnslookupdomain BlackBerry Dynamics Name Service Error Domain
 * These constants can be used when handling BlackBerry Dynamics name service
 * lookup errors, in the results handling block of an
 * \link GDNetUtility::nslookup:type:completion: nslookup\endlink call.
 *
 * \{
 */

/** The error domain for BlackBerry Dynamics proxy infrastructure utility
 *  errors.
 */
extern NSString* const GDNetUtilityErrorDomain;

typedef NS_ENUM(NSInteger, GDNslookupErr)
{
    /** A time out occurred.
     */
    GDNslookupErrTimeout                = 100,
    
    /** @deprecated This code was for the following condition: The host looked
     * up isn't listed for client connection in the enterprise management
     * console.\ Unlisted hosts can now be looked up so this code is no longer
     * required.
     */
    GDNslookupErrHostNotListed DEPRECATE_ERRORHOSTNOTLISTED,
    
    /** A network error occurred, for example the enterprise deployment couldn't
     *  be reached or the mobile data or Wi-Fi connection was lost.
     */
    GDNslookupErrNetworkError           = 102,
    
    /** The name service response couldn't be parsed.
     */
    GDNslookupErrParsingResponseError   = 103,

    /** An internal error occured.
     */
    GDNslookupErrInternalError          = 104,

    /** One or more parameters to the lookup was invalid.
     */
    GDNslookupErrParameterError         = 105,

    /** A general error occurred.
     */
    GDNetUtilityErrCouldNotPerformService = 500
};

/** \}
 */

/** BlackBerry Dynamics name service lookup completion block.
 * 
 * Pass a code block of this type as the <tt>completion</tt> parameter to the
 * \link GDNetUtility::nslookup:type:completion: nslookup\endlink function.
 *
 * The block receives two parameters.
 * @param response <tt>NSDictionary</tt> containing the results if the lookup
 *                 succeeded, or <tt>nil</tt> otherwise.
 * 
 * @param error <tt>NSError</tt> containing a description of the error condition
 *              if the lookup failed, or <tt>nil</tt> otherwise.
 */
typedef void (^GDNslookupCompletion)(NSDictionary *response, NSError *error);


/** BlackBerry Dynamics proxy infrastructure network utilities.
 * 
 * This class contains the programming interface for a network utility provided
 * by the BlackBerry Dynamics proxy infrastructure.
 *
 * The utility is accessed as a static method on a "singleton class" instance.
 */
@interface GDNetUtility : NSObject

/** Execute a BlackBerry Dynamics name service lookup.
 * 
 * Call this function to execute a BlackBerry Dynamics name service
 * lookup. The lookup can be for canonical name (CNAME) or address record
 * (A record).
 * 
 * The lookup will be executed from an enterprise end point of the BlackBerry
 * Dynamics deployment to which the application is connected. The end point could
 * be a BlackBerry Proxy server located behind the enterprise firewall or in its
 * perimeter network (also known as DMZ, demilitarized zone, and screened
 * subnet). Details for enterprise application servers could be in scope of the
 * lookup, depending on the network configuration at the enterprise. 
 * 
 * The lookup is asynchronous. When the lookup completes, a completion block
 * will be executed. The block will be passed the results, if the lookup was
 * successful, or an error code otherwise.
 *
 * @param host <tt>NSString</tt> containing the name to look up.
 *
 * @param type <tt>GDNslookupType</tt> specifying the type of result required,
 *             either CNAME or A record.
 *
 * @param completion Block to execute when the lookup completes. The block
 *                   receives two parameters:\n
 *                   <tt>NSDictionary</tt> containing the results if the lookup
 *                   succeeded, or <tt>nil</tt> otherwise.\n
 *                   <tt>NSError</tt> containing a description of the error
 *                   condition if the lookup failed, see \ref gdnslookupdomain,
 *                   or <tt>nil</tt> otherwise.
 */
+ (void)nslookup:(NSString*)host type:(GDNslookupType)type completion:(GDNslookupCompletion)completion;

/** Configure usage of a web proxy server for connections through the BlackBerry Dynamics proxy infrastructure.
 * The proxy configuration may be specified within the BlackBerry Dynamics management console using either a
 * Proxy Auto-Configuration (PAC) file or manually.
 *
 * - If <tt>YES</tt>, the application should specify its own web proxy server to which web requests (HTTP, HTTPS) should be forwarded.
 * - If <tt>NO</tt>, all web requests through the BlackBerry Dynamics proxy infrastructure will use the web proxy
 * server configuration defined in the BlackBerry Dynamics management console (if any).
 
 * The runtime sets this property to <tt>NO</tt> by default.
 *
 * This is a class property.
 */
@property (class, nonatomic, assign) BOOL webProxyAppliedByApp;

@end


#endif
