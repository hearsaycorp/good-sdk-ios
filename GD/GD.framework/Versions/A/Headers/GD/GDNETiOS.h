/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 *
 */

#pragma once

#import <Foundation/Foundation.h>
#import "GDURLLoadingSystem.h"
#import "GDPortability.h"
#import "GDNET.h"

/** Constants for GDSocket errors.
 * 
 * This enumeration represents the type of a GDSocket error that is being
 * notified. The <tt>error</tt> parameter of the
 * \link GDSocketDelegate::onErr:inSocket: GDSocketDelegate::onErr:\endlink
 * callback always takes one of these values.
 */
typedef NS_ENUM(NSInteger, GDSocketErrorType)
{
    /** No error.
     * 
     * This value is a placeholder for when the socket operation succeeded. The
     * error parameter never takes this value.
     */
    GDSocketErrorNone=0,
    
    /** Destination network not available.
     * 
     * This value indicates that the socket operation failed because the
     * destination network couldn't be reached.
     */
    GDSocketErrorNetworkUnvailable,

    /** Socket operation timed out.
     * 
     * This value indicates that a socket operation timed out and didn't
     * complete.
     */
    GDSocketErrorServiceTimeOut
};

GD_NS_ASSUME_NONNULL_BEGIN

/** Delegate for handling GDSocket state transitions and received data.
 * 
 * Errors and state changes that occur when using GDSocket are handled by
 * creating a class that implements this protocol. The callback for handling
 * received data is also part of this protocol.
 */
@protocol GDSocketDelegate

@required
/** Socket opened callback.
 * 
 * This callback is invoked when the corresponding socket opens for
 * communication with the remote server, see GDSocket::connect.
 *
 * Invocation of this callback also notifies the application on the device
 * that data can be written to the socket, using \ref GDSocket::write.
 *
 * @param socket <tt>GD</tt><tt>Socket</tt> object that issued the callback.
 */
- (void)onOpen:(id) socket;

@required
/** Socket data received callback.
 * 
 * This callback is invoked when data has been received from the remote server,
 * and is ready to read. The function that is invoked should consume the
 * received data.
 *
 * The received data will be available in the corresponding socket's
 * \ref GDSocket::readStream "readStream" buffer, which can be accessed using
 * the GDDirectByteBuffer interface.
 *
 * @param socket <tt>GD</tt><tt>Socket</tt> object that issued the callback.
 */
- (void)onRead:(id) socket;

@required
/** Socket closed callback.
 * 
 * This callback is invoked when the corresponding socket is closed. This means
 * closed by the remote end, or by the client end (see GDSocket::disconnect).
 *
 * Invocation of this callback notifies the application on the device that:
 * - The socket cannot now be used for writing.
 * - No more data will be received on the socket.
 * .
 *
 * @param socket <tt>GD</tt><tt>Socket</tt> object that issued the callback.
 */
- (void)onClose:(id) socket;

@required
/** Socket error callback.
 * 
 * This callback is invoked when a socket error occurs on the corresponding
 * socket.
 *
 * @param error \ref GDSocketErrorType value for the socket error that occurred.
 * 
 * @param socket <tt>GD</tt><tt>Socket</tt> object that issued the callback.
 */
- (void)onErr:(int)error inSocket:(id) socket;

@end


/** Buffer for accessing GDSocket and GDHttpRequest data.
 * 
 * The BlackBerry Dynamics direct byte buffer interface is used to access
 * a number of in-memory byte buffers within the BlackBerry Dynamics secure
 * communication features.
 * - Use it with \ref GDSocket for reading and writing data.
 * - Use it with the deprecated \ref GDHttpRequest class for reading response
 *   data.
 * .
 */
@interface GDDirectByteBuffer : NSObject {
    @private
    void* m_dbbInternal;
}

/** Constructor that prepares a new buffer.
 * 
 * Call this function to construct a stand-alone byte buffer. Where a buffer is
 * associated with a GDSocket or GDHttpRequest (deprecated) object, this
 * function will have been called by the BlackBerry Dynamics runtime, and need
 * not be called by the application.
 */
- (instancetype)init;

/** Append null-terminated string.
 * 
 * Call this function to append data to the buffer. This would be used on the
 * outbound buffer of a GDSocket.
 *
 * Calling this function doesn't cause data to be sent. See GDSocket::write.
 * The buffer allocates itself more memory as necessary to accomodate unsent
 * data.
 *
 * @param data Null-terminated string, containing the data to be appended.
 *             The terminating null is not written to the buffer.
 */
- (void)write:(const char*)data;

/** Append NSData data.
 * 
 * Call this function to append data to the buffer. This would be used on the
 * outbound buffer of a GDSocket.
 *
 * Calling this function doesn't cause data to be sent. See GDSocket::write.
 * The buffer allocates itself more memory as necessary to accomodate unsent
 * data.
 *
 * @param data <tt>NSData</tt> object containing the data to be appended.
 */
- (void)writeData:(NSData*)data;

/** Append a number of bytes from a string.
 * 
 * Call this function to append data to the buffer. This would be used on the
 * outbound buffer of a GDSocket.
 *
 * Calling this function doesn't cause data to be sent. See GDSocket::write.
 * The buffer allocates itself more memory as necessary to accomodate unsent
 * data.
 *
 * @param data Buffer containing the data to be appended.
 * @param length Number of bytes to be written from the buffer.
 */
- (void)write:(const char*)data withLength:(int)length;

/** Number of bytes available.
 * 
 * @return Number of bytes available to read from the buffer.
 */
- (int)bytesUnread;

/** Consume data into a <tt>char*</tt> buffer.
 * 
 * Call this function to read and consume a specified amount of raw data from
 * the buffer. This would be used on the inbound buffer of a GDSocket, or on the
 * response data of a GDHttpRequest (deprecated).
 *
 * This function would typically be called in a delegated event handler, such as
 * one of the following.
 * - \ref GDHttpRequestDelegate::onStatusChange: (deprecated).
 * - \ref GDHttpRequest::getReceiveBuffer (deprecated).
 * - \ref GDSocketDelegate::onRead:
 * .
 *
 * Calling this function causes data to be written to a <tt>char</tt> buffer
 * supplied by the caller. The caller specifies the maximum amount of data to be
 * written to the buffer, as a number of bytes.
 *
 * The function returns the number of bytes actually written. This will be the
 * lesser of the specified maximum, and the amount of unread data available.
 * 
 * The data written to the caller's buffer is, in effect, deleted from the byte
 * buffer, and will not be returned by future calls to any reading functions.
 *
 * @param data Pointer to a <tt>char</tt> buffer to which data is
 *             to be written.
 *             
 * @param maxLength Maximum number of bytes to be written to the <tt>char</tt>
 *                  buffer.
 * 
 * @return Number of bytes actually written to the <tt>char</tt> buffer.
 */
- (int)read:(char*)data toMaxLength:(int)maxLength;

/** Consume data into a new <tt>NSString</tt> object.
 * 
 * Call this function to create an <tt>NSString</tt> object, populated with
 * data consumed from the buffer. This would be used on the inbound buffer of a
 * GDSocket, or on the response data of a GDHttpRequest (deprecated).
 *
 * This function would typically be called in a delegated event handler, such as
 * one of the following.
 * - \ref GDHttpRequestDelegate::onStatusChange: (deprecated).
 * - \ref GDHttpRequest::getReceiveBuffer (deprecated).
 * - \ref GDSocketDelegate::onRead:
 * .
 *
 * Calling this function first causes a UTF-8 <tt>NSString</tt> object to be
 * allocated, by the BlackBerry Dynamics runtime. All unread data from the
 * buffer is then written into the new object. The data written is, in effect,
 * deleted from the buffer, and will not be returned by future calls to any
 * reading functions.
 *
 * @return New <tt>NSString</tt> object, populated with data consumed from the
 *         buffer.
 */
- (GD_NSNULLABLE NSMutableString*)unreadDataAsString;

/** Consume data into a new <tt>NSData</tt> object.
 * 
 * Call this function to create an <tt>NSData</tt> object, populated with data
 * consumed from the buffer. This would be used on the inbound buffer of a
 * GDSocket, or on the response data of a GDHttpRequest (deprecated).
 *
 * This function would typically be called in a delegated event handler, such as
 * one of the following.
 * - \ref GDHttpRequestDelegate::onStatusChange: (deprecated).
 * - \ref GDHttpRequest::getReceiveBuffer (deprecated).
 * - \ref GDSocketDelegate::onRead:
 * .
 *
 * Calling this function first causes an <tt>NSData</tt> object to be allocated,
 * by the BlackBerry Dynamics runtime. All unread data from the buffer is then
 * written into the new object. The data written is, in effect, deleted from the
 * buffer, and will not be returned by future calls to any reading functions.
 *
 * @return New <tt>NSData</tt> object, populated with data consumed from the
 *         buffer.
 */
- (GD_NSNULLABLE NSMutableData*)unreadData;

@end

/** \page st02gdsocket GDSocket state transition diagram
 *  \image html "st02 GD Socket.png" "GDSocket state transition diagram" \image rtf "st02 GD Socket.png" "GDSocket state transition diagram"
 * @see GDSocket
 */

/** TCP socket, supporting SSL/TLS and communication across the firewall.
 * 
 * 
 * The BlackBerry Dynamics socket is for bi-directional data
 * communication between the mobile application on the device and an application
 * server. The application server can be on the Internet, or behind the
 * enterprise firewall. Use of a Secure Socket Layer and Transport Layer
 * Security (SSL/TLS) are supported.
 * 
 * The functions in this class cannot be called until BlackBerry Dynamics
 * authorization processing is complete.
 * 
 * 
 * BlackBerry Dynamics secure communication does not go via the proxy specified
 * in the device's native settings, if any.
 * @see \reflink GDiOS\endlink, for BlackBerry Dynamics authorization.
 * @see \ref threads
 * @see \ref background_execution
 * @see GDHttpRequest
 * @see GDNetUtility
 * @see <a  HREF="http://help.blackberry.com/en/good-control-good-proxy/current/"  target="_blank" >Manuals page for the BlackBerry Dynamics enterprise servers</a > for the Platform Overview.
 *
 *
 * <h3>Overview</h3>
 * The BlackBerry Dynamics socket programming interface is asynchronous and
 * state-based. The application attaches its own event-handler callbacks to the
 * socket object. The callback functions are invoked when socket events occur,
 * or when the socket changes state. Which functions can be called by the
 * application at any time depend on the socket's state.
 *
 * Callbacks are attached through a delegate class. The states in which each
 * callback may be expected to be invoked are detailed in the delegate class's
 * documentation, see \ref GDSocketDelegate.
 *
 * The availability of functions, and what actions take place, are detailed
 * below, and summarized in the following table. The table also summarizes which
 * callbacks may expect to be invoked in each state.
 * <table>
 *     <tr><th>State</th><th>Functions / Actions</th><th>Expected callbacks</th>
 *
 *     </tr><tr><td>
 *         Prepared</td>
 *     <td>
 *         Application can call <tt>connect</tt>: state becomes Connecting\n
 *         Application can call <tt>disableHostVerification</tt>:
 *         no state change\n
 *         Application can call <tt>disablePeerVerification</tt>:
 *         no state change</td>
 *     <td>
 *         None</td>
 *
 *     </tr><tr><td>
 *         Connecting</td>
 *     <td>
 *         Open socket connection to server</td>
 *     <td>
 *         <tt>onErr</tt>: no state change\n
 *         <tt>onOpen</tt>: new state is Open</td>
 *
 *     </tr><tr><td>
 *         Open</td>
 *     <td>
 *         Application can call <tt>disconnect</tt>: state becomes
 *         Disconnecting\n
 *         Application can call <tt>write</tt>: No state change</td>
 *     <td>
 *         <tt>onRead</tt>: no state change\n
 *         <tt>onErr</tt>: no state change\n
 *         <tt>onClose</tt>: new state is Disconnected</td>
 *
 *     </tr><tr><td>
 *         Disconnecting</td>
 *     <td>
 *         Close socket connection</td>
 *     <td>
 *         <tt>onRead</tt>: no state change\n
 *         <tt>onErr</tt>: no state change\n
 *         <tt>onClose</tt>: new state is Disconnected</td>
 *
 *     </tr><tr><td>
 *         Disconnected</td>
 *     <td>
 *         Application can call <tt>connect</tt>: state becomes Connecting</td>
 *     <td>
 *         None</td>
 *
 *     </tr>
 * </table>
 * The transitions in the above table are also shown in the
 * \ref st02gdsocket
 *
 * <h3>Sending and Receiving Data</h3>
 * Sending data through a BlackBerry Dynamics socket is a two-stage operation.
 * The first stage is to add the data to the socket's outbound buffer. The
 * socket's outbound buffer is represented by the <tt>writestream</tt> property,
 * and is accessed using the BlackBerry Dynamics direct byte buffer interface.
 * The second stage is to send the contents of the buffer through the socket
 * connection. To send the buffer, call the <tt>write</tt> function.
 *
 * Reading data from a BlackBerry Dynamics socket is asynchronous. When data is
 * received at the device, the data is stored in the socket's inbound buffer.
 * The application is then notified that data is available to read, by
 * invocation of the delegate <tt>onRead</tt> callback. In the callback, the
 * application consumes the received data from the inbound buffer. The inbound
 * buffer is represented by the <tt>readStream</tt> property, and is
 * accessed using the BlackBerry Dynamics direct byte buffer interface.
 *
 * <h3>SSL/TLS Security</h3>
 * The BlackBerry Dynamics socket interface supports use of a Secure Socket
 * Layer or Transport Layer Security (SSL/TLS) to send and receive data.
 *
 * Using SSL/TLS requires that the remote end has a suitable certificate, and
 * that the certificate is valid.
 * A number of checks for validity are made by the BlackBerry Dynamics runtime,
 * some of which can be switched off by the application.

 * A typical secure socket connection sequence would be as follows. The
 * application makes a first connection attempt. In the first attempt, all
 * checking is switched on. If a security error is encountered, this attempt
 * will fail. The application can then switch off some checking, and make a
 * second connection attempt to the same address as the first connection
 * attempt. With less rigorous checking, the second attempt may succeed where
 * the first failed.
 *
 * The relevant parts of the API are:
 * - Use of SSL/TLS is specified by including
 *   <tt>andUseSSL:</tt>&nbsp;<tt>YES</tt> in the call to the <tt>init</tt>
 *   function.
 * - Security errors are treated the same as connection failures.
 * - The functions <tt>disableHostVerification</tt> and
 *   <tt>disablePeerVerification</tt> are used to reduce the level
 *   of security checking. Setting <tt>disablePeerVerification</tt> implicitly
 *   sets <tt>disableHostVerification</tt>.
 * .
 * <h4>Secure Protocol Selection</h4>
 * Establishing an SSL/TLS connection can involve negotiating and retrying, in
 * order to select a secure protocol that is supported by both client and
 * server. The BlackBerry Dynamics runtime handles client-side negotiation and
 * retrying, if secure communication is in use.
 *
 * By default, the runtime won't offer the TLSv1.1 or TLSv1.2 protocols for
 * SSL/TLS connections with an application server.
 *
 * The runtime can be configured to offer these protocols, as follows.
 * -# Add a new row to the application's Info.plist fi<tt></tt>le*:
 *     - Key: <tt>GDControlTLSVersions</tt>
 *     - Type: <tt>String</tt> (the default)
 *     .
 * -# Set the value to:
 *     - <tt>GDEnableTLS1.1</tt> to enable the TLSv1.1 protocol.
 *     - <tt>GDEnableTLS1.2</tt> to enable both TLSv1.1 and TLSv1.2 protocols.
 *     .
 *     As an alternative, the value can instead be an array containing one or
 *     both of the above strings as separate items.
 * .
 * (*In case there are multiple Info.plist files, check that the correct one has
 * been edited by opening the Info tab of the application target being built.
 * The setting just made should appear there.)
 * 
 * The setting only affects connections to application servers, not the
 * connection with the BlackBerry Dynamics infrastructure itself. The protocols
 * aren't offered by default because there are many installed web servers with
 * which a connection cannot be established after one of these protocols has
 * been offered.
 *
 * In addition to the above, the following protocol versions can be blocked by
 * enterprise management console configuration.
 * <div class="bulletlists">
 * - SSLv3
 * - TLSv1.0
 * - TLSv1.1
 * - TLSv1.2
 * .
 * </div>
 *
 * The configuration of blocked protocol versions can be done in the enterprise
 * management console user interface. The configuration applies at the
 * deployment level and isn't specific to user, policy group, or application
 * server. The block only affects communication with application servers that is
 * routed via the BlackBerry Dynamics proxy infrastructure.
 *
 * <h3>Enterprise server connection notes</h3>
 * BlackBerry Dynamics secure communication can be used to connect to servers
 * that are behind the enterprise firewall. This applies to socket connections
 * and HTTP requests. Note the following when using this capability.
 *
 * The addresses of any application servers to which connection is being made
 * must be configured in the enterprise management console. The address could be
 * registered as the application's server, or as an additional server, for
 * example.
 *
 * Note. The application server configuration set in the management console can
 * be obtained in the application code by using the
 * \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS)\endlink function.
 *
 * The connection to the application server will be made through the BlackBerry
 * Dynamics proxy infrastructure. The status of the application's connection to
 * the proxy infrastructure can be checked and monitored by using the
 * GDReachability interface.
 *
 * @see \ref GC
 * @see \reflink GDReachability GDReachability\endlink class reference.
 */
@interface GDSocket : NSObject {
    /** \privatesection */
    __weak id <GDSocketDelegate> GD_NSNULLABLE_POINTER delegate;
    @private
    void* m_socketInternal;
    GDDirectByteBuffer* writeStream;
    GDDirectByteBuffer* readStream;
}

/** Constructor that prepares a new socket.
 * 
 * Call this function when constructing a new GDSocket object. This function
 * doesn't initiate data communication, compare \ref connect.
 *
 * @param url Null-terminated string containing the address of the server.
 *        Can be either an Internet Protocol address (IP address, for example
 *        <tt>"192.168.1.10"</tt>), or a fully qualified domain name (for
 *        example <tt>"www.example.com"</tt>).
 *
 * @param port Number of the server port to which the socket will connect.
 *
 * @param ssl <tt>NO</tt> to use no security, <tt>YES</tt> to use SSL/TLS
 *            security.
 *            
 * @see SSL/TLS Security, above
 */
- (instancetype)init:(const char*)url onPort:(int)port andUseSSL:(BOOL)ssl;

/** Security option: Switch off SSL/TLS host name verification.
 *
 * This function switches off host name verification, when making
 * an SSL/TLS connection. Host name verification is an SSL/TLS security option.
 *
 * \par Host Name Verification
 * When negotiating an SSL/TLS connection, the server sends a certificate
 * indicating its identity. The certificate includes a number of host names. By
 * default, one of the host names in the certificate must match with the host
 * name specified by the client, or the connection fails.\n
 * If host name verification is switched off, the connection succeeds regardless
 * of whether there is a matching host name in the certificate.\n
 * If switched on, the BlackBerry Dynamics runtime checks server identity in a
 * way that conforms with the relevant RFC.
 * See under section 3.1 Server Identity, in <a
 *     href="http://www.rfc-editor.org/rfc/rfc2818.txt"
 *     target="_blank"
 * >RFC2818</a>.
 *
 * This function must be called before <tt>connect</tt>.
 *
 * Switching off host name verification doesn't switch off authenticity
 * verification, see \ref disablePeerVerification.
 * 
 * @return <tt>YES</tt> if the security option was successfully disabled.
 * @return <tt>NO</tt> if the security option could not be disabled.
 *
 * @see SSL/TLS Security, above
 */
- (BOOL)disableHostVerification;

/** Security option: Disable SSL/TLS authenticity verification.
 *
 * This function switches off certificate authenticity
 * verification, when making an SSL/TLS connection. Authenticity verification is
 * an SSL/TLS security option.
 *
 * \par Certificate Authenticity Verification
 * When negotiating an SSL/TLS connection, the server sends a certificate
 * indicating its identity. By default, the certificate must be verified as
 * trustworthy, or the connection fails. In this context, trustworthiness
 * derives from a chain of digital signatures, rooted in a certification
 * authority.\n
 * If authenticity verification is switched off, the connection succeeds
 * regardless of the certificate's trustworthiness.\n
 * If authenticity verification is switched on, the BlackBerry Dynamics runtime
 * checks certificate trustworthiness using operating system services.
 * See the <a
       HREF="https://developer.apple.com/reference/security/certificate_key_and_trust_services?language=objc"
       target="_blank"
   >Certificate, Key, and Trust Services API Reference</a> on the apple.com
   developer website.
 *
 * This function must be called before <tt>connect</tt>.
 *
 * Switching off authenticity verification implicitly also switches off host
 * name verification, see \ref disableHostVerification.
 *
 * @return <tt>YES</tt> if the security option was successfully disabled.
 * @return <tt>NO</tt> if the security option could not be disabled.
 *
 * @see SSL/TLS Security, above
 */
- (BOOL)disablePeerVerification;

/** Connect the socket.
 * 
 * Call this function to open the socket connection.
 *
 * The connection attempt is asynchronous. If the attempt succeeds, the delegate
 * \ref GDSocketDelegate::onOpen: "onOpen" callback is invoked.
 */
- (void)connect;

/** Send data from the writeStream buffer.
 * 
 * Call this function to send data through the socket connection. The data must
 * previously have been added to the socket's outbound buffer, represented by
 * the \ref GDSocket::writeStream "writeStream" property.
 */
- (void)write;

/** Terminate the socket connection.
 * 
 * Call this function to terminate the socket connection.
 *
 * Disconnection is asynchronous. When disconnection completes, the delegate
 * \ref GDSocketDelegate::onClose: "onClose" callback is invoked.
 */
- (void)disconnect;

/** Handler for state changes and received data.
 * 
 * The BlackBerry Dynamics socket interface is asynchronous. When socket state
 * changes, or data is received, a callback in the application code is invoked
 * by the BlackBerry Dynamics runtime.
 *
 * Set this property to an instance of a class that contains the code for the
 * required callback functions, i.e. a class that implements the
 * GDSocketDelegate protocol.
 */
@property (GD_NSNULLABLE_PROP nonatomic, weak) id<GDSocketDelegate> delegate;

/** Outbound buffer.
 * 
 * This property represents the outbound buffer of the socket. Data to be sent
 * through the socket is first written to this buffer, using the
 * GDDirectByteBuffer interface, then sent by calling \ref write.
 *
 * The outbound buffer can be accessed whenever this property isn't null.
 */
@property (GD_NSNULLABLE_PROP nonatomic, strong) GDDirectByteBuffer* writeStream;

/** Inbound buffer.
 * 
 * This property represents the inbound buffer of the socket. When data is
 * received through the socket, the following takes place:
 * - The data is stored in the inbound buffer,
 * - The delegate \ref GDSocketDelegate::onRead: "onRead" callback is invoked.
 * .
 * Received data should then be consumed from the buffer, using the
 * GDDirectByteBuffer interface.
 *
 * The inbound buffer can be accessed whenever this property isn't null.
 */
@property (GD_NSNULLABLE_PROP nonatomic, strong) GDDirectByteBuffer* readStream;

@end

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_GDHTTPREQUESTDELEGATE __attribute__((deprecated("GDHttpRequestDelegate interface has been deprecated")))
#else
#   define DEPRECATE_GDHTTPREQUESTDELEGATE __attribute__((deprecated))
#endif

/** Delegate for handling GDHttpRequest state transitions (deprecated).
 * 
 * @deprecated
 * This class is deprecated and will be removed in a future release.
 * It is associated with a deprecated class, \ref GDHttpRequest class.
 *
 * Errors and state changes that occur when using GDHttpRequest are handled by
 * creating a class that implements this protocol.
 */
DEPRECATE_GDHTTPREQUESTDELEGATE
@protocol GDHttpRequestDelegate

@required
/** Callback for state changes and received data (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is associated with a deprecated class, \ref GDHttpRequest.
 *
 * This callback is invoked whenever the corresponding GDHttpRequest changes
 * state, or when more response data is received.
 *
 * The function that is invoked should initially call
 * \ref GDHttpRequest::getState "getState" to determine the ready state.
 *
 * Depending on the ready state, other member functions may then be called by
 * the invoked function. See GDHttpRequest for details.
 *
 * @param httpRequest <tt>GDHttp</tt><tt>Request</tt> object that issued the
 *                    callback.
 */
- (void)onStatusChange:(id) httpRequest DEPRECATE_GDHTTPREQUESTDELEGATE;

@end

/** Constants for HTTP Request ready states (deprecated).
 * 
 * @deprecated
 * This enumeration is deprecated and will be removed in a future release.
 * It is associated with a deprecated class, \ref GDHttpRequest.
 *
 * This enumeration represents the possible states of an HTTP request.
 *
 * @see http://www.w3.org/TR/XMLHttpRequest/#states
 *
 * Compare the value returned by \ref GDHttpRequest::getState "getState" to
 * these constants to check the ready state of the GDHttpRequest object. (The
 * XHR state names have been prefixed with <tt>GDHttpRequest_</tt> and the
 * standard values used.)
 */
typedef NS_ENUM(NSInteger, GDHttpRequest_state_t)
{
    /** Prior to the request being opened (deprecated).
     */
    GDHttpRequest_UNSENT = 0,
    /** The request is ready to have headers added, and be sent (deprecated).
     */
    GDHttpRequest_OPENED = 1,
    /** The request has been sent (deprecated).
     */
    GDHttpRequest_SENT = 2,
    /** The request has been sent, and response headers have been received
     *  (deprecated).
     */
    GDHttpRequest_HEADERS_RECEIVED = 3,
    /** Headers and some data have been received in response to the request
     *  (deprecated).
     */
    GDHttpRequest_LOADING = 4,
    /** All data has been received, or a permanent error has been
     *  encountered (deprecated).
     */
    GDHttpRequest_DONE = 5
} DEPRECATE_GDHTTPREQUESTDELEGATE;

/** \page st03gdhttprequest GDHttpRequest state transition diagram (deprecated)
 *  \image html "st03 GD HTTP Request.png" "GDHttpRequest state transition diagram" \image rtf "st03 GD HTTP Request.png" "GDHttpRequest state transition diagram"
 * @see GDHttpRequest (deprecated).
 */

// See: http://clang.llvm.org/docs/LanguageExtensions.html
#ifndef __has_extension
#define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_CLEARCOOKIES __attribute__((deprecated("No longer supported")))
#else
#   define DEPRECATE_CLEARCOOKIES __attribute__((deprecated))
#endif
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_GDHTTPREQUEST __attribute__((deprecated("GDHttpRequest interface has been deprecated")))
#else
#   define DEPRECATE_GDHTTPREQUEST __attribute__((deprecated))
#endif
/** Standards-based HTTP request, also supporting HTTPS and communication across
 *  the firewall (deprecated).
 *
 * @deprecated
 * This class is deprecated and will be removed in a future release. BlackBerry
 * Dynamics secures the native NSURLSession and NSURLConnection programming
 * interfaces, which should be used instead. For reference, see
 * \ref nsurlsession_support and \ref GDURLLoadingSystem. For example code, see
 * the RSSReader sample application that comes with the BlackBerry Dynamics SDK
 * for iOS.
 *
 * This class is for sending Hypertext Transfer Protocol (HTTP) requests, such
 * as GET and POST, from a front-end application on a mobile device to a
 * back-end application server. The application server can be on the Internet,
 * or behind the enterprise firewall. The programming interface is based on the
 * XML Http Request (XHR) standard. HTTPS security is supported.
 *
 * This programming interface cannot be used until BlackBerry Dynamics
 * authorization processing is complete.
 *
 * <b>Note that synchronous request calls should not be made from the main
 * application thread.</b>
 *
 * @see GDiOS, for BlackBerry Dynamics authorization
 * @see <a  HREF="http://help.blackberry.com/en/good-control-good-proxy/current/"  target="_blank" >Manuals page for the BlackBerry Dynamics enterprise servers</a > for the Platform Overview.
 * @see \ref threads
 * @see \ref background_execution
 * @see GDSocket
 * @see GDNetUtility
 * @see <a
            href="http://www.w3.org/TR/XMLHttpRequest/"
            target="_blank"
        >XML HTTP Request (XHR) specification on w3.org</a>
 * @see \ref GDURLLoadingSystem and \ref nsurlsession_support for alternative
 *      approaches.
 *
 * <h3>Overview</h3>
 * An instance of this class represents an HTTP request. The programming
 * interface is state-based. The availability of functions to be called by the
 * application at any given time depend on the current state of the request.
 *
 * Requests can be processed synchronously or asynchronously, at the option of
 * the application. For asychronous operation, the application attaches its own
 * event-handler callback to the request object. The callback function is
 * invoked when events occur, or when the request changes state.
 *
 * The callback is attached through a delegate class. Invocation of the callback
 * is detailed in the delegate class's documentation, see GDHttpRequestDelegate.
 *
 * The availability of interface functions, and what actions take place, are
 * detailed below and summarized in the following table. (States in all-capitals
 * are standard XHR ready states.)<table>
 *     <tr><th>Ready State</th><th>Functions / Actions</th>
 *     </tr><tr><td>
 *         UNSENT</td>
 *     <td>
 *         The application can call <tt>open</tt>: state becomes OPENED\n
 *
 *         The application can also call any of the following
 *         <em>pre-send </em>functions:\n
 *         <tt>disableHostVerification</tt> (deprecated),\n
 *         <tt>disablePeerVerification</tt> (deprecated),\n
 *         <tt>disableFollowLocation</tt> (deprecated),\n
 *         <tt>disableCookieHandling</tt>(deprecated),\n
 *         <tt>clearCookies</tt> (deprecated),\n
 *         <tt>enableHttpProxy</tt> (deprecated),\n
 *         <tt>disableHttpProxy</tt> (deprecated)\n
 *         Calling a pre-send function doesn't cause a state change.
 *         The action of a pre-send function will take effect when the request
 *         is sent.</td>
 *     </tr><tr><td>
 *         OPENED</td>
 *     <td>
 *         The application can call <tt>send</tt>, <tt>sendData</tt> or
 *         <tt>sendWithFile</tt>: state becomes Sending\n
 *
 *         The application can also call any of the following, which don't cause
 *         a state change:\n
 *         Any pre-send function that can be called in the UNSENT state,\n
 *         <tt>setRequestHeader</tt> (deprecated),\n
 *         <tt>setPostValue</tt> (deprecated),\n
 *         <tt>clearPostValues</tt> (deprecated)</td>
 *     </tr><tr><td>
 *         SENT</td>
 *     <td>
 *         The request has been sent to the server\n
 *
 *         If a response is received, state becomes HEADERS_RECEIVED\n
 *
 *         If an error occurs, state becomes DONE</td>
 *     </tr><tr><td>
 *         HEADERS_RECEIVED</td>
 *     <td>
 *         All the headers have been received
 *
 *         The application can call the following, which don't cause a state
 *         change:\n
 *         <tt>getResponseHeader</tt> (deprecated),\n
 *         <tt>getAllResponseHeaders</tt> (deprecated),\n
 *         <tt>getStatus</tt> (deprecated),\n
 *         <tt>getStatusText</tt> (deprecated)\n
 *
 *         When the first response data is received, state becomes LOADING</td>
 *     </tr><tr><td>
 *         LOADING</td>
 *     <td>
 *         The body is being received\n
 *
 *         The application can call the following, which don't cause a state
 *         change:\n
 *         <tt>getResponseHeader</tt> (deprecated),\n
 *         <tt>getAllResponseHeaders</tt> (deprecated),\n
 *         <tt>getStatus</tt> (deprecated),\n
 *         <tt>getStatusText</tt> (deprecated),\n
 *         <tt>getReceiveBuffer</tt> (deprecated)\n
 *
 *         Note: <tt>getReceiveBuffer</tt> is used to access the body of the
 *         HTTP response
 *
 *         When the last response data is received, state becomes DONE</td>
 *     </tr><tr><td>
 *         DONE</td>
 *     <td>
 *         The complete body has been received or an error has occured
 *
 *         The application can call the same functions as when in the UNSENT
 *         state\n
 *
 *         The application can also call the following, which don't cause a
 *         state change:\n
 *         <tt>getResponseHeader</tt> (deprecated),\n
 *         <tt>getAllResponseHeaders</tt> (deprecated),\n
 *         <tt>getStatus</tt> (deprecated),\n
 *         <tt>getStatusText</tt> (deprecated),\n
 *         <tt>getReceiveBuffer</tt> (deprecated)</td>
 *     </tr>
 *  </table>
 * The transitions in the above table are also shown in the
 * \ref st03gdhttprequest.
 *
 * <h3>XHR differences</h3>
 * Differences between this programming interface and the XmlHttpRequest
 * standard are detailed below, and summarized in the following table:<table>
 *     <tr><th>XmlHttpRequest</th><th>GDHttpRequest</th>
 *     </tr><tr><td>
 *         readyState attribute</td>
 *     <td>
 *         <tt>getState</tt> function</td>
 *     </tr><tr><td>
 *         responseText and responseXML attributes,\n
 *         which provide all data received so far.</td>
 *     <td>
 *         <tt>getReceiveBuffer</tt> function,\n
 *         which provides data received since last called</td>
 *     </tr><tr><td>
 *         status attribute</td>
 *     <td>
 *         <tt>getStatus</tt> function</td>
 *     </tr><tr><td>
 *         statusText attribute</td>
 *     <td>
 *         <tt>getStatusText</tt> function</td>
 *     </tr>
 * </table>
 * <h3>Enterprise server connection notes</h3>
 * BlackBerry Dynamics secure communication can be used to connect to servers
 * that are behind the enterprise firewall. This applies to socket connections
 * and HTTP requests. Note the following when using this capability.
 *
 * The addresses of any application servers to which connection is being made
 * must be configured in the enterprise management console. The address could be
 * registered as the application's server, or as an additional server, for
 * example.
 *
 * Note. The application server configuration set in the management console can
 * be obtained in the application code by using the
 * \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS)\endlink function.
 *
 * The connection to the application server will be made through the BlackBerry
 * Dynamics proxy infrastructure. The status of the application's connection to
 * the proxy infrastructure can be checked and monitored by using the
 * GDReachability interface.
 *
 * @see \ref GC
 * @see \reflink GDReachability GDReachability\endlink class reference.
 * 
<h3>HTTPS Security</h3>
 * BlackBerry Dynamics secure communication supports HTTPS, using a Secure
 * Socket Layer connection or Transport Layer Security (SSL/TLS) to send an HTTP
 * request and receive a response.
 *
 * Using SSL/TLS requires that the remote end has a suitable certificate, and
 * that the certificate is valid.
 * A number of checks for validity are made by the BlackBerry Dynamics runtime,
 * some of which can be switched off by the application.
 *
 * A typical HTTPS request sequence would be as follows. The application makes a
 * first attempt to send the HTTPS request. In the first attempt, all checking
 * is switched on. If a security error is encountered, this request will fail.
 * The application can then switch off some checking, and attempt to send a
 * second request to the same address as the first request. With less rigorous
 * checking, the second attempt may succeed where the first failed.
 *
 * The relevant parts of the programming interface are:
 * - Use of HTTPS is specified by addressing the request to a URL with "https"
 *   as its scheme, as in "https://www.example.com".
 * - Security errors are treated the same as connection failures. The
 *   <tt>getStatusText</tt> return value will begin with <tt>"SSL"</tt>.
 * - The functions <tt>disableHostVerification</tt> and
 *   <tt>disablePeerVerification</tt> can be used to reduce the level of
 *   security checking. Setting <tt>disablePeerVerification</tt> implicitly sets
 *   <tt>disableHostVerification</tt>.
 * .
 * <h4>Secure Protocol Selection</h4>
 * Establishing an SSL/TLS connection can involve negotiating and retrying, in
 * order to select a secure protocol that is supported by both client and
 * server. The BlackBerry Dynamics runtime handles client-side negotiation and
 * retrying, if secure communication is in use.
 *
 * By default, the runtime won't offer the TLSv1.1 or TLSv1.2 protocols for
 * SSL/TLS connections with an application server.
 *
 * The runtime can be configured to offer these protocols, as follows.
 * -# Add a new row to the application's Info.plist fi<tt></tt>le*:
 *     - Key: <tt>GDControlTLSVersions</tt>
 *     - Type: <tt>String</tt> (the default)
 *     .
 * -# Set the value to:
 *     - <tt>GDEnableTLS1.1</tt> to enable the TLSv1.1 protocol.
 *     - <tt>GDEnableTLS1.2</tt> to enable both TLSv1.1 and TLSv1.2 protocols.
 *     .
 *     As an alternative, the value can instead be an array containing one or
 *     both of the above strings as separate items.
 * .
 * (*In case there are multiple Info.plist files, check that the correct one has
 * been edited by opening the Info tab of the application target being built.
 * The setting just made should appear there.)
 * 
 * The setting only affects connections to application servers, not the
 * connection with the BlackBerry Dynamics infrastructure itself. The protocols
 * aren't offered by default because there are many installed web servers with
 * which a connection cannot be established after one of these protocols has
 * been offered.
 *
 * In addition to the above, the following protocol versions can be blocked by
 * enterprise management console configuration.
 * <div class="bulletlists">
 * - SSLv3
 * - TLSv1.0
 * - TLSv1.1
 * - TLSv1.2
 * .
 * </div>
 *
 * The configuration of blocked protocol versions can be done in the enterprise
 * management console user interface. The configuration applies at the
 * deployment level and isn't specific to user, policy group, or application
 * server. The block only affects communication with application servers that is
 * routed via the BlackBerry Dynamics proxy infrastructure.
 * 
<h3>HTTP proxy support</h3>
 * HTTP and HTTPS requests can be relayed by an HTTP
 * or HTTPS 
 * proxy that resides on the Internet or behind the enterprise firewall.
 * Authentication with the proxy is supported.
 *
 * See the \link
 * GDHttpRequest::enableHttpProxy:withPort:withUser:withPass:withAuth:
 * enableHttpProxy:\endlink and \link
 * GDHttpRequest::disableHttpProxy disableHttpProxy\endlink reference
 * documentation for details.
 *
 * When making HTTPS requests through an HTTP proxy, SSL/TLS certificate
 * verification must be switched off. Certificate verification while using an
 * HTTP proxy isn't supported. See under HTTPS Security, above.
 * BlackBerry Dynamics HTTP data communication doesn't go via the proxy
 * specified in the device's native settings, if any.
 * 
<h3>HTTP authentication</h3>
 * The BlackBerry Dynamics runtime supports the following mechanisms for
 * authentication with HTTP servers: Basic Access, Digest Access, NTLM, and
 * Kerberos. Except for Kerberos, all these mechanisms are also supported for
 * authentication with HTTP proxies.
 *
 * <h4>Kerberos Authentication</h4>
 *  \htmlonly <div class="bulletlists"> \endhtmlonly
 * The BlackBerry Dynamics runtime supports Kerberos version 5 authentication.
 * If using Kerberos authentication:
 * - Supply username and password credentials
 * as documented in the relevant function references.
 * - The username must be in the <em>user</em><tt>\@</tt><em>realm</em> long
 *   form. The short form <em>shortrealm</em><tt>\\</tt><em>user</em> isn't
 *   supported.
 * - The BlackBerry Dynamics runtime will use these credentials to request
 *   Kerberos tickets. The tickets are persisted on the device in the BlackBerry
 *   Dynamics secure store. (The ticket store isn't generally accessible to the
 *   application, but see  \reflink GDCacheController::clearCredentialsForMethod:  clearCredentialsForMethod:\endlink.)
 * - The stored Kerberos tickets are then used to authenticate the user on any
 *   site that supports Kerberos authentication. So long as the ticket continues
 *   to be accepted, there is no need for credentials to be supplied again, and
 *   no authentication challenge.
 * - This continues until a site doesn't accept the stored ticket (e.g. the
 *   ticket has expired and cannot be renewed).
 * - The Kerberos realm must be accessible. One way is to configure the Kerberos
 *   realm as an Additional Server in the enterprise management console.
 * - Kerberos realms are treated differently to server addresses. An unqualified
 *   server address may be resolved according to configuration in the enterprise
 *   management console, but the short form of a Kerberos realm cannot be
 *   resolved in this way. Note that the short form of the Kerberos realm will
 *   typically be the form used when logging in to the enterprise LAN at the
 *   desktop. The long form is typically the short form with the domain
 *   appended.
 * - Kerberos delegation can be allowed or disallowed. See
 *   \link GDCacheController::kerberosAllowDelegation:
 *   GDCacheController::kerberosAllowDelegation:\endlink.
 * .
 *  \htmlonly </div> \endhtmlonly
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
DEPRECATE_GDHTTPREQUEST
@interface GDHttpRequest : NSObject {
    __weak id <GDHttpRequestDelegate> GD_NSNULLABLE_POINTER delegate;
    @private
    void* m_httpRequestInternal;
}

/** Initializer that prepares a new request (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function when constructing a new request. This function doesn't
 * initiate data communication. Compare
 * \ref open:withUrl:withAsync:withUser:withPass:withAuth: "open" and
 * \ref send:withLength:withTimeout: "send".
 *
 * The new request's ready state will be <tt>GDHttpRequest_UNSENT</tt>.
 */
- (instancetype)init DEPRECATE_GDHTTPREQUEST;

/** Open the request with all parameters (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to open the request, and set the main parameters.
 *
 * This is generally the first function called, after <tt>init</tt>, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt>. However, in principle
 * this function can be called at any time, regardless of the ready state. If
 * the ready state isn't <tt>GDHttpRequest_UNSENT</tt> then an effective
 * <tt>abort</tt> is executed, before the <tt>open</tt> call is processed.
 *
 * This section documents all the parameters that the function supports. It is
 * valid to use all the parameters, or to use particular subsets. The subsequent
 * <tt>open</tt> sections document the valid subsets of parameters.
 *
 * \par Kerberos authentication
 * To utilize Kerberos authentication, supply the username and password
 * credentials to the initial call to this function. See also under HTTP
 * Authentication, above.
 * 
 * @param method Null-terminated case-sensitive string containing the HTTP
 *               method that will be sent to the server. Typical values are:
 *               "GET", "POST", "HEAD", "OPTIONS", "TRACE", "PUT", "CONNECT".
 *               Any other value is sent as a custom method.
 * 
 * @param url Null-terminated string containing the Uniform Resource Locator
 *            (URL) that will be requested. The URL must be fully qualified,
 *            including a scheme, domain, and path. For example:
 *            "http://www.example.com/index.html".
 *
 * @param isAsync <tt>NO</tt> to use synchronous fetching, <tt>YES</tt> to use
 *                asynchronous fetching.\n
 *                See under <tt>send</tt> and <tt>abort</tt> for details of the
 *                difference.
 * 
 * @param user Null-terminated string containing authentication username. For
 *             Kerberos, this is in the <em>user</em><tt>\@</tt><em>realm</em>
 *             format.
 *
 * @param password Null-terminated string containing authentication password.
 * 
 * @param auth Null-terminated string specifying the authentication scheme:\n
 *             <tt>"BASIC"</tt> or a null pointer to use Basic Access
 *             authentication.\n
 *             <tt>"DIGEST"</tt> to use Digest Access authentication.\n
 *             <tt>"NEGOTIATE"</tt> to use negotiated Kerberos authentication,
 *             see note above.\n
 *             <tt>"NTLM"</tt> to use NTLM authentication.\n
 *             The following forms of the NTLM authentication protocol are
 *             supported: NTLMv1, NTLMv2, and NTLM2 Session.
 *
 * @return <tt>YES</tt> if the request was opened.
 * @return <tt>NO</tt> if the parameters were not valid or the request couldn't
 *         be opened.
 *
 * If the request was opened then the ready state changes, to
 * <tt>GDHttpRequest_OPENED</tt>. If asynchronous fetching was specified, then
 * the delegate <tt>onStateChange</tt> callback is invoked.
 */
- (BOOL) open:(const char*)method withUrl:(const char*)url withAsync:(BOOL) isAsync withUser:(GD_NSNULLABLE const char*)user withPass:(GD_NSNULLABLE const char*)password withAuth:(GD_NSNULLABLE const char*)auth DEPRECATE_GDHTTPREQUEST;

/** Open the HTTP request with synchronous fetching (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call with these parameters to open the request with authentication and
 * synchronous fetching. See
 * \ref open:withUrl:withAsync:withUser:withPass:withAuth: "open" for full
 * details.
 */
- (BOOL) open:(const char*)method withUrl:(const char*)url withUser:(GD_NSNULLABLE const char*)user withPass:(GD_NSNULLABLE const char*)password withAuth:(GD_NSNULLABLE const char*)auth DEPRECATE_GDHTTPREQUEST;

/** Open the HTTP request with specified fetching (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call with these parameters to open the HTTP request specifying synchronous or
 * asynchronous fetching. The request will not use any new authentication.
 *
 * If Kerberos authentication is in use then existing Kerberos tickets will be
 * attempted if the server returns an HTTP 401 response requesting to Negotiate
 * Authentication.
 *
 * See \ref open:withUrl:withAsync:withUser:withPass:withAuth: "open" for full
 * details.
 */
- (BOOL) open:(const char*)method withUrl:(const char*)url withAsync:(BOOL)isAsync DEPRECATE_GDHTTPREQUEST;

/** Open the HTTP request with synchronous fetching (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call with these parameters to open the HTTP request with synchronous
 * fetching. The request will not use any new authentication.
 *
 * If Kerberos authentication is in use then existing Kerberos tickets will be
 * attempted if the server returns an HTTP 401 response requesting to Negotiate
 * Authentication.
 *
 * See \ref open:withUrl:withAsync:withUser:withPass:withAuth: "open" for full
 * details.
 */
- (BOOL) open:(const char*)method withUrl:(const char*)url DEPRECATE_GDHTTPREQUEST;

/** Security option: Switch off SSL/TLS host name verification (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to switch off host name verification, when
 * making an HTTPS request. Host name verification is an SSL/TLS security
 * option.
 * \par Host Name Verification
 * When negotiating an SSL/TLS connection, the server sends a certificate
 * indicating its identity. The certificate includes a number of host names. By
 * default, when using HTTPS, one of the host names in the certificate must
 * match with the host name in the URL being opened, or the connection fails.\n
 * If host name verification is switched off, the connection succeeds
 * regardless of whether there is a matching host name in the certificate.\n
 * If switched on, the BlackBerry Dynamics runtime checks server identity in a
 * way that conforms with the relevant RFC. See under section 3.1 Server
 * Identity, in <a
     href="http://www.rfc-editor.org/rfc/rfc2818.txt"
     target="_blank">RFC 2818</a>.
 *
 * Note that connections may remain open after the HTTP transaction completes.
 * Even if another instance of GDHttpRequest is constructed, the connection may
 * still be reused. If subsequent transactions to the server require host
 * verification, <tt>close</tt> must be called immediately after the last
 * unverified transaction completes.
 *
 * @see HTTPS Security, above.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * Switching off host name verification doesn't switch off authenticity
 * verification, see \ref disablePeerVerification.
 *
 * @return <tt>YES</tt> if the security option was switched off. The check won't
 *         be made when the request is sent.
 * @return <tt>NO</tt> if the security option couldn't be switched off.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) disableHostVerification DEPRECATE_GDHTTPREQUEST;

/** Security option: Switch off SSL/TLS authenticity verification (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to switch off certificate authenticity
 * verification, when making an HTTPS request. Authenticity verification is an
 * SSL/TLS security option.
 *
 * \par Certificate Authenticity Verification
 * When negotiating an SSL/TLS connection, the server sends a certificate
 * indicating its identity. By default, when using HTTPS, the certificate must
 * be verified as trustworthy, or the connection fails. In this context,
 * trustworthiness derives from a chain of digital signatures, rooted in a
 * certification authority.\n
 * If authenticity verification is switched off, the connection succeeds
 * regardless of the certificate's trustworthiness.\n
 * If authenticity verification is switched on, the BlackBerry Dynamics runtime
 * checks certificate trustworthiness using operating system services.
 * See the <a
       HREF="https://developer.apple.com/reference/security/certificate_key_and_trust_services?language=objc"
       target="_blank"
   >Certificate, Key, and Trust Services API Reference</a> on the apple.com
   developer website.
 *
 * Note that connections may remain open after the HTTP transaction completes.
 * Even if another instance of GDHttpRequest is constructed, the connection may
 * still be reused. If subsequent transactions to the server require peer
 * verification, <tt>close</tt> must be called immediately after the last
 * unverified transaction completes.
 *
 * @see HTTPS Security, above.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * Switching off authenticity verification implicitly also switches off host
 * name verification, see \ref disableHostVerification.
 *
 * @return <tt>YES</tt> if the security option was switched off. The check won't
 *         be made when the request is sent.
 * @return <tt>NO</tt> if the security option couldn't be switched off.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) disablePeerVerification DEPRECATE_GDHTTPREQUEST;

/** Switch off automatic redirection (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to switch off automatic redirection.
 *
 * If automatic redirection is switched off, the application must handle
 * redirection itself, including handling Location: headers, and HTTP statuses
 * in the 30x range.
 *
 * If automatic redirection is switched on, any Location: header that the server
 * sends as part of an HTTP header will be automatically followed. This means
 * that the same request will be re-sent to the new location. The re-sent
 * request may itself be redirected, receiving a new Location: header. Automatic
 * redirection continues until a request receives no location headers.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * @return <tt>YES</tt> if the option was switched off and any Location: headers
 *         won't be followed.
 * @return <tt>NO</tt> if the option couldn't be switched off.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) disableFollowLocation DEPRECATE_GDHTTPREQUEST;

/** Switch off automatic handling of cookies (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to switch off automatic cookie handling.
 * 
 * If automatic handling is switched off, the application must store and process
 * cookies itself.
 *
 * When automatic handling is switched on, the BlackBerry Dynamics runtime
 * processes and stores HTTP cookies automatically, as described under HTTP
 * Cookie Handling, above.
 * 
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * @return <tt>YES</tt> if the option was switched off.
 * @return <tt>NO</tt> if the option couldn't be switched off.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) disableCookieHandling DEPRECATE_GDHTTPREQUEST;

/** Delete automatically stored cookies (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest. Cookies can be directly
 * cleared by using the native <tt>NSHTTPCookieStorage</tt> programming
 * interface. See under HTTP Cookie Handling, above.
 *
 * Call this function to clear cookies that were automatically stored. Cookies
 * can be cleared from memory only, or from the persistent cookie store too. If
 * cleared from memory only, cookies will still be reloaded from the persistent
 * cookie store when the application is next launched.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * This function is most useful if automatic cookie handling is swiched on. See
 * the \ref disableCookieHandling function, above.
 *
 * This function doesn't affect the ready state.
 * 
 * @param includePersistentStore <tt>YES</tt> to clear cookies from memory and
 *                               from persistent BlackBerry Dynamics cookie
 *                               storage.\n
 *                               <tt>NO</tt> to clear cookies from memory only.
 */
- (void) clearCookies:(BOOL) includePersistentStore DEPRECATE_CLEARCOOKIES;

/** Configure sending through an HTTP proxy with all parameters (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to set up usage of an HTTP proxy at a specified address,
 * and using specified credentials. The request will be sent through the proxy.
 *
 * The proxy server can be located behind the enterprise firewall. In this case
 * its address must be registered in the enterprise management console.
 * Registration would usually be as an additional server. See \ref GC.
 *
 * Certificate authenticity verification while using an HTTP proxy isn't
 * supported. When making HTTPS requests through a proxy, SSL/TLS certificate
 * verification must be switched off, see the <tt>disablePeerVerification</tt>
 * function.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * @param host Null-terminated string containing the address of the proxy. Can
 *             be either an Internet Protocol address (IP address, for example
 *             <tt>"192.168.1.10"</tt>), or a fully qualified domain name (for
 *             example <tt>"www.example.com"</tt>).
 * 
 * @param port Number of the port on the proxy to which connection will be made.
 * 
 * @param user Null-terminated string containing the proxy authentication
 *             username.
 * 
 * @param password Null-terminated string containing the proxy authentication
 *                 password.
 * 
 * @param auth Null-terminated string specifying the proxy authentication
 *             scheme:\n
 *             <tt>"BASIC"</tt> or any other value to use Basic Access
 *             authentication.\n
 *             <tt>"DIGEST"</tt> to use Digest Access authentication.\n
 *             <tt>"NTLM"</tt> to use NTLM authentication.\n
 *             The following forms of the NTLM authentication protocol are
 *             supported: NTLMv1, NTLMv2, and NTLM2 Session.
 *
 * @return <tt>YES</tt> if sending through an HTTP proxy was configured OK.
 * @return <tt>NO</tt> if configuration failed.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) enableHttpProxy:(const char*)host withPort:(int)port withUser:(const char*)user withPass:(const char*)password withAuth:(const char*)auth DEPRECATE_GDHTTPREQUEST;

/** Configure sending through an HTTP proxy without authentication (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to set up usage of an HTTP proxy at a specified address.
 * No authentication will be used when connecting to the proxy.
 *
 * See
 * \ref enableHttpProxy:withPort:withUser:withPass:withAuth: "enableHttpProxy"
 * for full details.
 */
- (BOOL) enableHttpProxy:(const char*)host withPort:(int)port DEPRECATE_GDHTTPREQUEST;

/** Remove HTTP proxy configuration (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to remove an HTTP proxy configuration that was previously
 * set, using an
 * \ref enableHttpProxy:withPort:withUser:withPass:withAuth: "enableHttpProxy"
 * function. The request then won't be sent through an HTTP proxy.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * @return <tt>YES</tt> if the HTTP proxy configuration was removed.
 * @return <tt>NO</tt> if removal failed.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) disableHttpProxy DEPRECATE_GDHTTPREQUEST;

/** Add an HTTP header field (deprecated).
 * 
 *@deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to add a header field to the HTTP request. This is for
 * standard HTTP header fields such as "Authorization". Add headers after the
 * request is open, but before sending.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_OPENED</tt>.
 *
 * This function can be called zero or more times, because not all HTTP requests
 * will require headers to be added by the application.
 *
 * Parameter data is copied and stored internally. The application doesn't need
 * to keep the data after calling the function.
 *
 * @param header Null-terminated string containing the name of the header field
 *               to add.
 * 
 * @param value Null-terminated string containing the value to put in the header
 *              field.
 *
 * @return <tt>YES</tt> if the header was added OK.
 * @return <tt>NO</tt> if the header couldn't be added.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) setRequestHeader:(const char*)header withValue:(const char*)value DEPRECATE_GDHTTPREQUEST;

/** Add a name-value pair for a "POST" request (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to add a name-value pair to the HTTP request. The request
 * method must be "POST". Multiple name-value pairs can be added, by calling
 * this function multiple times.
 *
 * When the request is sent, name-value pairs will be encoded in the request
 * body in a way that is compatible with HTML form submission. No other body
 * data can be passed in the send call.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_OPENED</tt>.
 *
 * @param value Null-terminated string containing the value to be set.
 * 
 * @param key Null-terminated string containing the name associated with the
 *            value.
 *
 * This function doesn't affect the ready state.
 */
- (void) setPostValue:(const char*)value forKey:(const char*)key DEPRECATE_GDHTTPREQUEST;

/** Clear all name-value pairs (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to remove all name-value pairs from the HTTP request.
 * Name-value pairs would have been added with the <tt>setPostValue</tt>
 * function, see above.
 *
 * Note that all name-value pairs will be cleared anyway when the request is
 * re-opened. This function need only be called if it is required to clear
 * name-value pairs before sending.
 *
 * This function should be called before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_OPENED</tt>.
 *
 * This function doesn't affect the ready state.
 */
- (void) clearPostValues DEPRECATE_GDHTTPREQUEST;

/** Send the HTTP request with all parameters (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to send the HTTP request to the server.
 *
 * This section documents all the parameters that the function supports. It is
 * valid to use all the parameters, or to use particular subsets. The subsequent
 * <tt>send</tt> sections document the valid subsets of parameters.
 *
 * This function can only be called after <tt>open</tt> has succeeded, when the
 * ready state is <tt>GDHttpRequest_OPENED</tt>. The subsequent behavior of
 * <tt>send</tt> depends on the kind of fetching that was specified in the
 * <tt>open</tt> call. See the <tt>isAsync</tt> parameter to <tt>open</tt>,
 * above.
 *
 * If synchronous fetching was specified, then the <tt>send</tt> call returns
 * when HTTP response data is received from the server.
 *
 * If asynchronous fetching was specified, then the <tt>send</tt> call returns
 * immediately. The following state transitions then take place as the request
 * progresses.
 * -# <tt>GDHttpRequest_SENT</tt> when the HTTP request has been sent.
 * -# <tt>GDHttpRequest_HEADERS_RECEIVED</tt> when the HTTP response headers
 *    have been received.
 * -# <tt>GDHttpRequest_LOADING</tt> when the first HTTP response data is
 *    received.
 * -# <tt>GDHttpRequest_DONE</tt> when all HTTP response data has been received.
 * .
 * If an error is encountered, in any state, then the request makes an immediate
 * transition to the <tt>GDHttpRequest_DONE</tt> state. This includes connection
 * errors, security errors, and time out expiry. (See also the Ready State table
 * in the class documentation, above.)
 *
 * @param data Pointer to the HTTP request body. This would be used in, for
 *             example, a "POST" method request. Parameter data isn't copied.
 *             The application must ensure that the data remains available until
 *             the request is in the <tt>GDHttpRequest_DONE</tt> state.
 *
 * @param len Numeric value for the number of bytes in the request body, i.e.
 *            what is pointed to by the data parameter.
 *
 * @param timeout_s Length of time out in seconds, or 0 (zero) for never. If the
 *                  function is called without this parameter, see below, zero
 *                  is assumed.
 *
 * @return <tt>YES</tt> if the request was accepted.
 * @return <tt>NO</tt> if the parameters were invalid.
 *
 * If the request was sent, and asynchronous fetching was specified, then a
 * state transition should be expected. The next state would be
 * <tt>GDHttpRequest_HEADERS_RECEIVED</tt> if the request is proceeding, or
 * <tt>GDHttpRequest_DONE</tt> if there is a connection failure.
 *
 * @see \ref sendWithFile:withTimeout: "sendWithFile"
 * @see \ref sendData:withTimeout: "sendData"
 */
- (BOOL) send:(GD_NSNULLABLE const char*)data withLength:(unsigned int)len withTimeout:(int)timeout_s DEPRECATE_GDHTTPREQUEST;

/** Send the HTTP request with null-terminated body and specified time out
 *  (deprecated).
 *  
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to send an HTTP request with body, and specified time out.
 * The body must be null-terminated.
 *
 * See \ref send:withLength:withTimeout: "send" for details.
 */
- (BOOL) send:(GD_NSNULLABLE const char*)data withTimeout:(int)timeout_s DEPRECATE_GDHTTPREQUEST;


/** Send the HTTP request with null-terminated body (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to send an HTTP request with body, and the default time
 * out setting. The body must be null-terminated.
 *
 * See \ref send:withLength:withTimeout: "send" for details.
 */
- (BOOL) send:(GD_NSNULLABLE const char*)data DEPRECATE_GDHTTPREQUEST;

/** Send the HTTP request without body (e.g.\ "GET" method) (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to send an HTTP request that has no body, for example a
 * "GET" method request, using the default time out setting.
 *
 * To send an HTTP request with no body, and override the time out setting, use
 * the full form of <tt>send</tt> but pass a null pointer as the data parameter.
 *
 * See \ref send:withLength:withTimeout: "send" for details.
 */
- (BOOL) send DEPRECATE_GDHTTPREQUEST;

/** Send the HTTP request with NSData body and specified time out (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to send an HTTP request with body, and specified time out.
 * The body will be the contents of an <tt>NSData</tt> object.
 *
 * See \ref send:withLength:withTimeout: "send" for details.
 */
- (BOOL) sendData:(GD_NSNULLABLE NSData*)data withTimeout:(int)timeout_s DEPRECATE_GDHTTPREQUEST;

/** Send the HTTP request with NSData body (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to send an HTTP request with body, and the default time
 * out setting. The body will be the contents of an <tt>NSData</tt> object.
 *
 * See \ref send:withLength:withTimeout: "send" for details.
 */
- (BOOL) sendData:(GD_NSNULLABLE NSData*)data DEPRECATE_GDHTTPREQUEST;

/** Send the HTTP request with file contents as body, with specified time out
 *  (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to use the open HTTP request to upload a file. The HTTP
 * request's method will be overridden to "PUT" unless it is a custom method. A
 * time out can be specified.
 *
 * This function causes the HTTP request to be sent, similar to the
 * <tt>send</tt> function, above. The body of the request will be the contents
 * of the specified file.
 *
 * The file will not be deleted after it is uploaded. Uploading directly from
 * the BlackBerry Dynamics secure file system isn't supported.
 *
 * @param pathAndFileName <tt>NSString</tt> containing the path (optional) and
 *                        filename of the file to upload. If path is omitted,
 *                        the file is read from the current working directory.
 * 
 * @param timeoutSeconds Length of time out in seconds, or 0 (zero) for never.
 *                       If the function is called without this parameter, see
 *                       below, zero is assumed.
 *
 * @return <tt>YES</tt> if the request was accepted.
 * @return <tt>NO</tt> if the parameters were invalid.
 *
 * @see \ref send:withLength:withTimeout: "send" for other details of sending.
 * @see \ref open:withUrl:withAsync:withUser:withPass:withAuth: "open" for
 *      how to set the request method.
 */
- (BOOL) sendWithFile:(NSString*)pathAndFileName withTimeout:(NSTimeInterval)timeoutSeconds DEPRECATE_GDHTTPREQUEST;

/** Send the HTTP request with file contents as body, with default time out
 *  (deprecated).
 *  
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to send the HTTP request, reading the body of the request
 * from a file, with the default time out setting.
 *
 * See \ref sendWithFile:withTimeout: "sendWithFile" for details.
 */
- (BOOL) sendWithFile:(NSString*)pathAndFileName DEPRECATE_GDHTTPREQUEST;

/** Get the ready state of the HTTP request (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * This function returns the ready state of the HTTP Request. See the
 * <tt>GDHttpRequest_state_t</tt> documentation for a list of values and ready
 * states. This function is generally the first function called in the delegated
 * event handler, see GDHttpRequestDelegate.
 *
 * This function corresponds to the standard XHR read-only attribute,
 * <tt>readyState</tt>.
 *
 * @return Numeric value that can be compared to the \ref GDHttpRequest_state_t
 *         enumerated constants.
 */
- (GDHttpRequest_state_t) getState DEPRECATE_GDHTTPREQUEST;

/** Get a specified HTTP response header (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to obtain a specific HTTP response header. (Compare
 * \ref getAllResponseHeaders.) HTTP response headers will be sent by the server
 * as part of its response to the HTTP request. Response headers are sent before
 * the content part of the response. (Compare \ref getReceiveBuffer.)
 *
 * If asynchronous fetching is in use, this function can be used after the
 * <tt>send</tt> call has been made and the request's ready state has progressed
 * to <tt>GDHttpRequest_HEADERS_RECEIVED</tt>.\n
 * If synchronous fetching is in use, this function can be used after
 * <tt>send</tt> has returned. (By that time, the ready state will already have
 * progressed to <tt>GDHttpRequest_DONE</tt>.)
 *
 * @param header Null-terminated string containing the name of the required HTTP
 *               response header field.
 *
 * @return Null-terminated string containing the value of the specified header,
 *         if present.
 * @return Empty string if the server did not send the specified header, or if
 *         there was an error and the request never reached the server.
 */
- (GD_NSNULLABLE const char*) getResponseHeader:(const char*)header DEPRECATE_GDHTTPREQUEST;

/** Get all HTTP response headers (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to obtain all HTTP response headers. (Compare
 * \ref getResponseHeader:.) HTTP response headers will be sent by the server as
 * part of its response to the HTTP request. Response headers are sent before
 * the content part of the response. (Compare \ref getReceiveBuffer.)
 *
 * This function can be used at the same point in the HTTP request cycle as
 * <tt>getResponseHeader</tt>, see above.
 *
 * @return Null-terminated string containing all HTTP response header fields,
 *         and their values.
 *         - Different headers will be separated by newline characters.
 *         - On each line, field and value will be separated by a colon (:)
 *           character.
 *         .
 *
 * This function doesn't affect the ready state.
 */
- (GD_NSNULLABLE const char*) getAllResponseHeaders DEPRECATE_GDHTTPREQUEST;

/** Get the numeric HTTP response status, or 0 (zero) if an error occurred
 *  (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to determine the success or failure of the HTTP request.
 * If the request was sent OK, this function returns the status code received
 * from the HTTP server, which could be a success code or an error code.
 * Otherwise, if the request was not sent, or there was a connection failure,
 * this function returns zero.
 *
 * In normal HTTP request processing, the status code is sent before the content
 * of the response. (Compare \ref getReceiveBuffer.)
 *
 * This function can be used at the same point in the HTTP request cycle as
 * <tt>getResponseHeader</tt> see above.
 *
 * This function corresponds to the standard XHR read-only attribute,
 * <tt>status</tt>.
 *
 * @return Numeric value for the final request status, interpreted as follows:
 *   <dl>
 *     <dt>
 *         0 (zero)</dt>
 *     <dd>
 *         Server connection failed.\n
 *         This includes:
 *         - DNS errors, or other problems where a connection to the server
 *           couldn't even be established.
 *         - Certificate verification failures, when HTTPS is in use.
 *         - Connection failure during receipt of the HTTP response.
 *         - Time out expiry while waiting for the server.
 *         - Connection closure initiated by the application,
 *           see \reflink GDHttpRequest::abort abort\endlink.
 *     <dt>
 *         200 to 299</dt>
 *     <dd>
 *         HTTP request successful at server.\n
 *         The number is the success code returned by the server.
 *     </dd>
 *     <dt>
 *         Other values.
 *     </dt>
 *     <dd>
 *         HTTP request failed at server.\n
 *         This the includes all the standard HTTP errors, such as:
 *         404&nbsp;'Not&nbsp;found' or 403&nbsp;'Forbidden'.
 *     </dd>
 *  </dl>
 * 
 * Note that, when asynchronous fetching is in use, it is possible that this
 * function returns different values at different points in the request cycle.
 * For example, suppose there is a network failure during receipt of a long
 * response. When the <tt>GDHttpRequest_LOADING</tt> state is entered, 200 might
 * be returned. But, when the request later enters the
 * <tt>GDHttpRequest_DONE</tt> state after the connection failure, 0 would be
 * returned.
 *
 * @see \ref getStatusText
 */
- (int) getStatus DEPRECATE_GDHTTPREQUEST;

/** Get the textual HTTP response status, as sent by the server, or details of
 *  error if \ref getStatus returns 0 (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * This function returns the status message received from the HTTP server, if
 * the request was sent OK. If the request was not sent, or there was a
 * connection failure, this function will return a description of the condition
 * that caused the failure.
 *
 * The status message is sent at the same time as the status code, see
 * <tt>getStatus</tt> above.
 *
 * This function can be used at the same point in the HTTP request cycle as
 * <tt>getResponseHeader</tt>, see above.
 *
 * This function corresponds to the standard XHR read-only attribute,
 * <tt>statusText</tt>.
 *
 * @return Null-terminated string containing the status text. The contents
 * depend on the <tt>getStatus</tt> return code, as follows:<table>
 *     <tr><th><tt>getStatus</tt> return code</th><th>Message contents</th>
 *     </tr><tr><td>
 *         0 (zero)\n
 *         Connection failure.</td>
 *     <td>
 *         Description of the error condition that caused the failure.
 *         For example:\n
 *         SSL/TLS negotiation failed</td>
 *     </tr><tr><td>
 *         Other values\n
 *         Request sent OK.</td>
 *     <td>
 *         Success or failure message provided by the HTTP server.\n
 *         In the failure case, this includes the standard HTTP errors, such as:
 *         404&nbsp;'Not&nbsp;found' or 403&nbsp;'Forbidden'.</td>
 *     </tr>
 * </table>
 *
 * @see \ref getStatus
 */
- (GD_NSNULLABLE const char*) getStatusText DEPRECATE_GDHTTPREQUEST;

/** Get HTTP response data (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to obtain the response data, i.e. the body of the HTTP
 * response. Response data will be sent by the server as part of its response to
 * the HTTP request. Response data is sent after response headers.
 * (Compare \ref getResponseHeader: and \ref getAllResponseHeaders.)
 *
 * If asynchronous fetching is in use, this function can be used after the
 * <tt>send</tt> call has been made and the request's ready state has progressed
 * to <tt>GDHttpRequest_LOADING</tt>.\n
 * If synchronous fetching is in use, this function can be used after
 * <tt>send</tt> has returned. (By that time, the ready state will already have
 * progressed to <tt>GDHttpRequest_DONE</tt>.)
 *
 * The first time this function is called, all data received so far is returned.
 * Subsequent calls return only the data received since the previous call. (If
 * synchronous fetching is in use, then this function would only be called once
 * per request, and would return all the response data.)
 *
 * This function corresponds to the standard XHR read-only attributes,
 * <tt>responseText</tt> and <tt>responseXML</tt>.
 *
 * @return Pointer to a GDDirectByteBuffer object that contains the response
 *         data received since last called.\n
 *         The application must read the data prior to releasing or re-using the
 *         request object.
 *
 * This function doesn't affect the ready state.
 */
- (GD_NSNULLABLE GDDirectByteBuffer*) getReceiveBuffer DEPRECATE_GDHTTPREQUEST;

/** Close connection and reset options (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to force closure of all connections that were used by the
 * HTTP request, after the request has completed. If any default options were
 * switched off then these will be switched back on when the first connection is
 * re-opened. (See also the <tt>disableHostVerification</tt>,
 * <tt>disablePeerVerification</tt> and other disable functions, above.)
 *
 * This function should only be called if the following all apply.
 * - Some options were switched off when the request was sent.
 * - A new request will be sent to the same endpoint.
 * - For the new request, one or more of the options that were switched off are
 *   to be switched on.
 * .
 *
 * Connections may remain open after the HTTP transaction completes. Even if
 * another instance of GDHttpRequest is constructed, connections may still be
 * reused. Calling this function immediately after the transaction completes
 * will ensure that connections are closed, and any SSL/TLS verifications that
 * were switched off are switched back on.
 *
 * After calling this function, it may take longer to make another HTTP request
 * to the same host.
 *
 * This function can be called when the ready state is
 * <tt>GDHttpRequest_DONE</tt>. Compare \ref abort.
 *
 * @return <tt>YES</tt> if the request was closed and the options reset.
 * @return <tt>NO</tt> if the request couldn't be closed.
 *
 * This function doesn't affect the ready state.
 */
- (BOOL) close DEPRECATE_GDHTTPREQUEST;

/** Cancel the request (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Call this function to cancel the HTTP request. Any response data that had
 * been received will be discarded. Any HTTP request headers that were set will
 * be cleared. Further changes also take place, depending on the request's ready
 * state.
 *
 * If the ready state is <tt>GDHttpRequest_SENT</tt> or
 * <tt>GDHttpRequest_HEADERS_RECEIVED</tt> or
 * <tt>GDHttpRequest_LOADING</tt>, then the ready state is set to
 * <tt>GDHttpRequest_DONE</tt> and the delegated event handler is invoked. See
 * GDHttpRequestDelegate. The final status will be set to zero, i.e.
 * <tt>getStatus</tt> will return 0.
 *
 * If the ready state is <tt>GDHttpRequest_DONE</tt>,
 * <tt>GDHttpRequest_OPENED</tt>, or <tt>GDHttpRequest_UNSENT</tt>, then this
 * function does nothing and returns <tt>NO</tt>.
 *
 * @return <tt>YES</tt> if the request was aborted.
 * @return <tt>NO</tt> if the request couldn't be aborted, hadn't been sent, or
 *         had already completed.
 */
- (BOOL) abort DEPRECATE_GDHTTPREQUEST;

/** Enable and disable HTTP pipelining of the request (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * Set this property to enable and disable HTTP pipelining of the associated
 * request.
 *
 * If pipelining is enabled, then the BlackBerry Dynamics runtime may pipeline
 * the request with other requests to the same server. If pipelining is disabled
 * then the runtime won't pipeline the request.
 *
 * This property should be set before <tt>send</tt> has been called, when the
 * request's ready state is <tt>GDHttpRequest_UNSENT</tt> or
 * <tt>GDHttpRequest_OPENED</tt>.
 *
 * Setting this property doesn't affect the ready state.
 *
 * Set this property to <tt>YES</tt> to enable HTTP pipelining, or to <tt>NO</tt> to
 * disable HTTP pipelining. By default, HTTP pipelining is enabled.
 */
@property (nonatomic, assign) BOOL enablePipelining DEPRECATE_GDHTTPREQUEST;

/** Handler for state changes and received data (deprecated).
 * 
 * @deprecated
 * This function is deprecated and will be removed in a future release.
 * It is in a deprecated class, \ref GDHttpRequest.
 *
 * The GDHttpRequest interface can operate asynchronously, at the option of the
 * application. See <tt>isAsync</tt> under the <tt>open</tt> function, above.
 *
 * If operating asynchronously, when the ready state changes or response data is
 * received, a callback function in the application code is invoked by the
 * BlackBerry Dynamics runtime.
 *
 * Set this property to an instance of a class that contains the code for the
 * required callback function, i.e. a class that implements the
 * GDHttpRequestDelegate protocol.
 */
@property (GD_NSNULLABLE_PROP nonatomic, weak) id<GDHttpRequestDelegate> delegate DEPRECATE_GDHTTPREQUEST;

@end

GD_NS_ASSUME_NONNULL_END
