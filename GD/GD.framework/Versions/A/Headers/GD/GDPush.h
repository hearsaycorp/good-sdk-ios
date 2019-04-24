/*
 * (c) 2018 BlackBerry Limited. All rights reserved.
 *
 */

#ifndef __GD_PUSH_H__
#define __GD_PUSH_H__

#import <Foundation/Foundation.h>
#import "GDPortability.h"

// See: http://clang.llvm.org/docs/LanguageExtensions.html
#ifndef __has_extension
#define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_PUSH_CONNECT_DISCONNECT __attribute__((deprecated("No longer required")))
#   define DEPRECATE_GDPUSHCONNECTION __attribute__((deprecated("GDPushConnection interface has been deprecated")))
#   define DEPRECATE_GDPUSHCHANNELDELEGATE __attribute__((deprecated("GDPushChannelDelegate protocol has been deprecated, use notifications instead.")))
#else
#   define DEPRECATE_PUSH_CONNECT_DISCONNECT __attribute__((deprecated))
#   define DEPRECATE_GDPUSHCONNECTION __attribute__((deprecated))
#   define DEPRECATE_GDPUSHCHANNELDELEGATE __attribute__((deprecated))
#endif

GD_NS_ASSUME_NONNULL_BEGIN

/**
 * \ingroup pushchannelconstants
 * 
 * \{
 */

/** Push Channel state.
 * 
 * Use these enumerated constants to check the state of a Push Channel. The
 * \reflink GDPushChannel::state state\endlink property always takes one of these values.
 */
typedef NS_ENUM(NSInteger, GDPushChannelState)
{
    /** \copydetails ssPushChannelStateNone
     */
    GDPushChannelStateNone = 0,
    
    /** \copydetails ssPushChannelStateOpen
     */
    GDPushChannelStateOpen,
    
    /** \copydetails ssPushChannelStateClosed
     */
    GDPushChannelStateClosed,
    
    /** \copydetails ssPushChannelStateError
     */
    GDPushChannelStateError
};

/** Push Channel opened notification name.
 *
 * After a call to \reflink GDPushChannel::connect connect\endlink by the application,
 * an <tt>NSNotification</tt> with this name is posted when the Push Channel
 * opens. The notification object will be the corresponding \reflink GDPushChannel GDPushChannel\endlink
 * instance. The notification will have a user information dictionary that
 * contains the Push Channel token. Use \ref GDPushChannelTokenKey as the key to
 * access the token value.
 * 
 * The application code that handles the notification
 * must initiate sending of the Push Channel token to the application server,
 * out of band. The application server will then be able to use the token to
 * address Push Channel messages back to the application, via the BlackBerry
 * Dynamics proxy infrastructure. See the \ref GNP.
 *
 * After receiving this notification, the application
 * could receive any of the following for the same channel.
 * - \reflink GDPushChannel::GDPushChannelMessageNotification GDPushChannelMessageNotification\endlink.
 * - \reflink GDPushChannel::GDPushChannelClosedNotification GDPushChannelClosedNotification\endlink.
 * - \reflink GDPushChannel::GDPushChannelErrorNotification GDPushChannelErrorNotification\endlink.
 * - \reflink GDPushChannel::GDPushChannelPingFailedNotification GDPushChannelPingFailedNotification\endlink.
 * .
 *
 * The value of this constant is suitable to use as the name parameter to an
 * NSNotificationCenter addObserver call.
 */
extern NSString* const GDPushChannelOpenedNotification;

/** Push Channel closed notification name.
 *
 * 
 * An <tt>NSNotification</tt> with this name is posted when a Push Channel
 * closes. The notification object will be the corresponding
 * \reflink GDPushChannel GDPushChannel\endlink instance. The notification will have a user information
 * dictionary that contains the Push Channel token. Use \ref
 * GDPushChannelTokenKey as the key to access the token value.
 *
 * The Push Channel could have been closed by the remote end, or the application
 * could have called \reflink GDPushChannel::disconnect disconnect\endlink.
 * 
 * After receiving this notification:
 * - The associated Push Channel token cannot be used any more.
 * - No more messages will be received on this Push Channel.
 * .
 *
 * If the notification wasn't expected, the
 * application code that handles it could alert the user that Push Channel 
 * notifications will not be received, or cause this to be displayed as an 
 * ongoing state. The code could also initiate release of the Push Channel
 * object. Alternatively, reconnection could be initiated, see
 * \reflink GDPushChannel::connect connect\endlink.
 *
 * Note that this notification is only sent for
 * permanent Push Channel closure; not for transient losses of channel
 * communication. For example, this notification
 * wouldn't be sent in the case of an application on a mobile device that loses
 * packet data coverage or otherwise cannot connect to the BlackBerry Dynamics
 * proxy infrastructure. The status of the connection to the infrastructure can
 * be monitored by using the
 * \link GDReachability\endlink
 * programming interface.
 * 
 * The value of this constant is suitable to use as the name parameter to an
 * NSNotificationCenter addObserver call.
 */
extern NSString* const GDPushChannelClosedNotification;

/** Push Channel error notification name.
 *
 * 
 * An <tt>NSNotification</tt> with this name is posted when a permanent error
 * occurs on a Push Channel. The notification object will be the
 * \reflink GDPushChannel GDPushChannel\endlink instance corresponding to the channel on which the error
 * occurred. The notification will have a user information dictionary that
 * contains an error code. Use \ref GDPushChannelErrorKey as the key to access
 * the error.
 *
 * <table>
 *     <tr><th>Error code</th><th>Channel Error reason</th>
 *     </tr>
 *     <tr><td>
 *         0</td>
 *     <td>
 *         Push is not currently connected.</td>
 *    </tr>
 *    <tr><td>
 *         200-499</td>
 *     <td>
 *         Internal error.</td>
 *     </tr>
 *     <tr><td>
 *         500-599</td>
 *     <td>
 *         Internal server error.</td>
 *     </tr>
 * </table>
 * 
 * Receiving this notification warns the application
 * that the associated Push Channel token cannot be used any more, or that the
 * channel could not be connected in the first place. Furthermore, no (more)
 * Push Channel notifications will be received on this channel.
 *
 * The application code that handles the notification
 * could alert the user that Push Channel messages will not be received, or
 * cause this to be displayed as an ongoing state. The code should also initiate
 * reconnection, see \reflink GDPushChannel::connect connect\endlink, after checking that the Push
 * Channel service is available. Service availability can be checked by using
 * the
 * \link GDReachability\endlink
 * programming interface.
 *
 * The value of this constant is suitable to use as the name parameter to an
 * NSNotificationCenter addObserver call.
 */
extern NSString* const GDPushChannelErrorNotification;

/** Push Channel received message notification name.
 *
 * 
 * An <tt>NSNotification</tt> with this name is posted when a Push Channel
 * receives a message. The notification object will be the corresponding
 * \reflink GDPushChannel GDPushChannel\endlink instance. The notification will have a user information
 * dictionary that contains the message "payload". Use
 * \ref GDPushChannelMessageKey as the key to access the message.
 * 
 * The Push Channel message will have been sent by an application server, using
 * the Push Channel notify service. See the \ref GNP.
 *
 * The service supports a "payload" of data to be included in the message. The
 * data could be in any format chosen by the application developer. The payload
 * could also be empty.
 *
 * Note that a Push Channel message can be received at any time when the channel
 * is open. This includes the interval between the request for disconnection
 * (\reflink GDPushChannel::disconnect disconnect\endlink called) and channel disconnection being
 * finalized
 * (\reflink GDPushChannel::GDPushChannelClosedNotification GDPushChannelClosedNotification\endlink received).
 *
 * The application code that handles the notification
 * could initiate the following actions, for example.
 * - Alert the user that new data is available.
 * - Connect to the application server to retrieve the data.
 * .
 *
 * \note
 * Because of this notification, the application code does not need to maintain a
 * constant connection with the server. This is an important benefit of using
 * the BlackBerry Dynamics Push Channel framework.
 * 
 * The value of this constant is suitable to use as the name parameter to an
 * NSNotificationCenter addObserver call.
 */
extern NSString* const GDPushChannelMessageNotification;

/** Push Channel ping failed on server notification name.
 *
 * 
 * An <tt>NSNotification</tt> with this name is posted when a Push Channel Ping
 * Failure occurs. The notification object will be the \reflink GDPushChannel GDPushChannel\endlink
 * instance corresponding to the channel on which the ping failure occurred. The
 * notification will have a user information dictionary that contains a ping
 * failure reason code. Use \ref GDPushChannelErrorKey as the key to access the
 * code.
 *
 * <table><tr><th>Reason code</th><th>Failure</th>
 *     </tr>
 *     <tr><td>
 *         600</td>
 *     <td>
 *         Application server address could not be resolved by the domain name
 *         service (DNS).</td>
 *     </tr>
 *     <tr><td>
 *         601</td>
 *     <td>
 *         Could not connect to application server address.</td>
 *     </tr>
 *    <tr><td>
 *         602</td>
 *     <td>
 *         Application server electronic certificate for Secure Socket Layer or
 *         Transport Layer Security (SSL/TLS) connection is invalid.</td>
 *     </tr>
 *    <tr><td>
 *         603</td>
 *     <td>
 *         Timed out waiting for application server HTTP response.</td>
 *     </tr>
 *     <tr><td>
 *         604</td>
 *     <td>
 *         Application server returned an invalid response.</td>
 *     </tr>
 *     <tr><td>
 *         605</td>
 *     <td>
 *         Application server indicated that the token has been lost.</td>
 *     </tr>
 * </table>
 * 
 * The application code that handles the notification
 * should initiate resending of the Push Channel token to the application
 * server, if the token has been lost. Loss of token is indicated by reason code
 * 605, see above. This is similar to the processing when the channel is
 * initially opened, see
 * \reflink GDPushChannel::GDPushChannelOpenedNotification GDPushChannelOpenedNotification\endlink.
 * If the application server is able to accept the token, then Push Channel
 * notification can resume.
 *
 * Note that ping failure can occur at any time when the channel is open. This
 * includes the interval between the request for disconnection
 * (\reflink GDPushChannel::disconnect disconnect\endlink called) and channel disconnection being
 * finalized
 * (\reflink GDPushChannel::GDPushChannelClosedNotification GDPushChannelClosedNotification\endlink received).
 *
 * <h2>Ping Failure</h2>
 * Ping Failure is an optional feature of the Push Channel framework. The
 * application server can register a ping address after receiving the Push
 * Channel token from the mobile application.
 *
 * If the application server registers a ping address, then it will be
 * periodically checked ("pinged") by the BlackBerry Dynamics Network Operation
 * Center (NOC). If the server does not respond to a ping, then the NOC notifies
 * the application that requested the corresponding Push Channel.
 * 
 * The purpose of this feature is to support servers that lose the Push Channel
 * token when they are restarted.
 *
 * See the \ref GNP for details of Ping Failure registration.
 */
extern NSString* const GDPushChannelPingFailedNotification;

/** Key for the Push Channel token, in a notification user information
 *  dictionary.
 *
 * Use this constant to access the Push Channel token in a notification. The
 * token will be in the posted <tt>NSNotification</tt> instance, in the user
 * information dictionary, with this value as its key. It will be an
 * <tt>NSString</tt>.
 *
 * Example of usage:
 * \code
 * NSString *token = nsNotification.userInfo[GDPushChannelTokenKey];
 * \endcode
 *
 * The key will be present if the <tt>NSNotification</tt> has one of the
 * following values as its <tt>name</tt>.
 * - \reflink GDPushChannel::GDPushChannelOpenedNotification GDPushChannelOpenedNotification\endlink.
 * - \reflink GDPushChannel::GDPushChannelClosedNotification GDPushChannelClosedNotification\endlink.
 * .
 */
extern NSString* const GDPushChannelTokenKey;

/** Key for the message data, in a Push Channel notification user information
 *  dictionary.
 *
 * Use this constant to access the message data in a Push Channel notification.
 * The message will be in the posted <tt>NSNotification</tt> instance, in the
 * user information dictionary, with this value as its key. It will be an
 * <tt>NSString</tt>.
 *
 * Example of usage:
 * \code
 * NSString *message = nsNotification.userInfo[GDPushChannelMessageKey];
 * \endcode
 * 
 * The key will be present if the <tt>NSNotification</tt> has the <tt>name</tt>:
 * \reflink GDPushChannel::GDPushChannelMessageNotification GDPushChannelMessageNotification\endlink.
 */
extern NSString* const GDPushChannelMessageKey;

/** Key for the error code, in a Push Channel notification user information
 *  dictionary.
 *
 * Use this constant to access the error code in a Push Channel notification. The
 * code will be in the posted <tt>NSNotification</tt> instance, in the user
 * information dictionary, with this value as its key. It will be an
 * <tt>NSInteger</tt> value.
 *
 * Example of usage:
 * \code
 * NSInteger code = [nsNotification.userInfo[GDPushChannelErrorKey] integerValue];
 * \endcode
 *
 * The key will be present if the <tt>NSNotification</tt> has one of the
 * following values as its <tt>name</tt>.
 * - \reflink GDPushChannel::GDPushChannelErrorNotification GDPushChannelErrorNotification\endlink.
 * - \reflink GDPushChannel::GDPushChannelPingFailedNotification GDPushChannelPingFailedNotification\endlink.
 * .
 */
extern NSString* const GDPushChannelErrorKey;

/** \}
 */

/** Delegate for handling GDPushConnection state transition (deprecated).
 *
 * 
 * \deprecated This class is deprecated and will be removed in a future
 * release.
 * Instead of setting a PushConnection delegate, register for notifications as
 * described in the \link GDReachability\endlink class reference.
 * 
 *
 * Errors and state changes that occur when using \reflink GDPushConnection GDPushConnection\endlink are
 * handled by creating a class that implements this protocol.
 *
 *  <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Print State-Change</h3>
 * \code
 * @interface BasicHandler : NSObject <GDPushConnectionDelegate> {}
 * @end
 *
 * @implementation BasicHandler
 * -(void)onStatus:(int)status
 * {
 *     if ( [[GDPushConnection sharedConnection] isConnected] ) {
 *         NSLog( @"Connected OK. Push Channel service available.");
 *     }
 *     else {
 *         NSLog( @"Push Channel service not available.");
 *     }
 * }
 * @end
 * \endcode
 * The above snippet shows a very simple handler for \reflink GDPushConnection GDPushConnection\endlink
 * state-change. The availability of the Push Channel service is written to the
 * system log.
 *
 * <h3>Set Event Handler</h3>
 * \code
 * basicDelegate = [[BasicHandler alloc] init];
 * connection.delegate = basicDelegate;
 * \endcode
 * The above snippet shows how the above handler could be associated with the
 * Push Connection.
 */

DEPRECATE_GDPUSHCONNECTION
@protocol GDPushConnectionDelegate

/** Callback for all status changes (deprecated).
 * The callback is invoked when the Push Connection changes state, or
 * when an error occurs.
 *
 * The function that is invoked could call
 *  \reflink GDPushConnection::isConnected isConnected\endlink to determine the availability of the
 * following features:
 * - Push Channel service
 * - Communication through the BlackBerry Dynamics proxy infrastructure
 * .
 * If the features are available (i.e. <tt>isConnected</tt> returns <tt>YES</tt>),
 * then any of the following actions that were waiting could now proceed:
 * - Establishing a Push Channel, see \reflink GDPushChannel::connect connect\endlink
 * - Opening a socket connection to an enterprise server, see
 * \ref GDSocket::connect
 * - Sending an HTTP request to an enterprise server, see
 *   \ref nsurlsession_support.
 * .
 * If the features are not available (i.e. <tt>isConnected</tt> returns
 * <tt>NO</tt>) then the function that is invoked could alert the user, or
 * display this as an ongoing state.
 * In this state, Push Channel notifications would not be received.
 *
 * @param status Internal numeric code for the new status.
 * Specific values are not documented, and should not be relied upon.
 *
 * During establishment or termination of the Push Connection with the BlackBerry
 * Dynamics proxy infrastructure, see <tt>GDPush</tt><tt>Connection</tt>
 *  \reflink GDPushConnection::connect connect\endlink and  \reflink GDPushConnection::disconnect disconnect\endlink,
 * the callback will be invoked a number of times, as the action progresses.
 *
 * Events that have an impact on the state of the Push Connection also
 * result in the callback being invoked. This would include loss of
 * network coverage and other losses of data connection, as well as the
 * subsequent automatic re-establishment of the connection.
 */
- (void)onStatus:(int)status;
@end


/** Manage Push Channel connection (deprecated).
 * 
 * \deprecated This class is deprecated and will be removed in a future
 * release.
 * - Instead of <tt>isConnected</tt>, use \link
 *   GDReachability.isPushChannelAvailable\endlink.
 * - Instead of setting a PushConnection delegate, register for notifications as
 *   described in the \link GDReachability\endlink class reference.
 * - The <tt>connect</tt> and <tt>disconnect</tt> functions are no longer
 *   required. They were added to enable management of power consumption by the
 *   application code. This is no longer required because of advances in the
 *   built-in power management of mobile operating systems and devices.
 * .
 * 
 *
 * This API is part of the BlackBerry Dynamics Push Channel feature.
 * For an overall description of how to use the feature, see under
 * \reflink GDPushChannel GDPushChannel\endlink.
 *
 * The Push Connection is the container and conduit for the device's Push
 * Channels. An application may open multiple Push Channels; all will be
 * managed within a single Push Connection.
 *
 * The Push Connection is automatically established during BlackBerry Dynamics
 * authorization processing, and then maintained by the BlackBerry Dynamics
 * Runtime under application control.
 * The application can instruct the runtime to switch the Push Connection
 * off and on.
 *
 * When instructed to switch off, the BlackBerry Dynamics Runtime will terminate the Push
 * Connection, and suspend its maintenance. When instructed to switch
 * back on, the BlackBerry Dynamics Runtime will re-establish the Push Connection, and
 * resume maintenance.
 *
 * Switching off the Push Connection might be an option that the application
 * offers to the end user, for example, allowing them to
 * reduce power consumption on the device.
 *
 * Push Connection functions cannot be called until BlackBerry Dynamics
 * authorization processing is complete.
 * @see \reflink GDPushChannel GDPushChannel\endlink
 * @see \reflink GDiOS\endlink, for BlackBerry Dynamics authorization
 * @see \ref threads
 * @see \ref background_execution
 *
 * <h3>Push Channel Network</h3>
 * The Push Connection is a link between the mobile application and the BlackBerry
 * Dynamics proxy infrastructure Network Operation Center (NOC).
 * The Push Channel is a link between the mobile application and its application
 * server (App Server). There can be more than one Push Channel; the mobile
 * application can receive push communications from more than one App Server.
 * Push Channels are mediated by the NOC, and sometimes other proxy
 * infrastructure elements.
 *
 * This is shown in the following diagram.
 *  \image html "Push Channel network.png"
 \image rtf "Push Channel network.png"

 *
 * <h3>API Overview</h3>
 * The BlackBerry Dynamics Push Connection API consists of a small number of functions that must
 * be used in a particular order. Whilst some other APIs are general-purpose
 * toolkits, the Push Connection API has only a single purpose: to enable
 * the Push Channel API.
 *
 * The normal sequence of operations is as follows.
 * -# Application started.
 * -# BlackBerry Dynamics initialization and authorization, see under \reflink GDiOS\endlink.
 * -# When the application needs a Push Channel...
 * -# Call
 *  \reflink GDPushConnection::sharedConnection sharedConnection\endlink 
 * to access the Push Connection object,
 * -# Call  \reflink GDPushConnection::isConnected isConnected\endlink to check the connection state,
 * -# If the state is not connected:
 *   -# Set a  \reflink GDPushConnectionDelegate GDPushConnectionDelegate\endlink to handle connection state changes
 *   -# Call  \reflink GDPushConnection::connect connect\endlink to ensure that connection is
 *      being attempted,
 *   -# When the  \reflink GDPushConnectionDelegate::onStatus: onStatus\endlink callback in the
 *      handler is invoked, go back and check the connection state again.
 * -# If the state is connected, proceed to setting up a Push Channel.
 * .
 * For details of Push Channel set-up, see under \reflink GDPushChannel GDPushChannel\endlink.
 *
 *  <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Terminate Push Connection</h3>
 * \code
 * [[GDPushConnection sharedConnection] disconnect];
 * \endcode
 * After the disconnect, the connection can be re-opened later.
 *
 * <h3>Re-open Push Connection</h3>
 * \code
 * if ( ! [[GDPushConnection sharedConnection] isConnected] ) {
 *     myHandler = [[AppHandler alloc] init];
 *     myConnection.delegate = myHandler;
 *     [myConnection connect];
 * }
 * \endcode
 * The above snippet shows a check for whether the Push Channel service is
 * already available. If it is not, then a connection is initiated.
 * The connection attempt is asynchronous. The <tt>onStatus</tt>
 * callback would be invoked, with <tt>isConnected</tt> returning <tt>YES</tt>,
 * when the attempt succeeds (not shown). See  \reflink GDPushConnectionDelegate GDPushConnectionDelegate\endlink.
 */
DEPRECATE_GDPUSHCONNECTION
@interface GDPushConnection : NSObject {
}

/** Get a reference to the Push Connection object (deprecated).
 * @deprecated This function is in a deprecated class and will be
 * removed in a future release.
 *
 * This function returns a reference to the Push Connection object.
 *
 * The Push Connection object is a "singleton class".
 *
 * @return Reference that can be used to call, for example, the
 *         <tt>isConnected</tt> function.
 */
+ (instancetype)sharedConnection;

/** Get state of the connection to the Push Channel service (deprecated).
 * @deprecated This function is in a deprecated class and will be
 * removed in a future release.
 *
 * This function returns the current status of the Push Channel connection.
 * 
 * @return <tt>YES</tt> if the Push Channel connection is open and operating, and
 *         the Push Channel service is available.
 * @return <tt>NO</tt> otherwise.
 * 
 * @throws_GDauth
 */
- (BOOL)isConnected;

/** Initiate connection to the overall Push Channel service (deprecated).
 *
 * @deprecated This function is deprecated and will be removed in a
 * future release.\n
 * This function enabled management of power consumption by the application
 * code. This is no longer required because of advances in the built-in power
 * management of mobile operating systems and devices.
 *
 * Call this function to establish, or re-establish, the Push Channel
 * connection with the BlackBerry Dynamics proxy infrastructure Network Operation
 * Center (NOC).
 *
 * Establishing the connection involves a number of messages being
 * exchanged with the NOC. The <tt>onStatus</tt> callback in the
 * delegate will be invoked as this progresses.
 *
 * If mobile data coverage is lost after this function has been called,
 * the Push Channel connection will stop operating.
 * The BlackBerry Dynamics Runtime will automatically attempt to re-establish
 * the Push Channel connection when coverage is regained.
 * The BlackBerry Dynamics Runtime uses the native
 * System Configuration feature to
 * be notified of coverage status.
 * @throws_GDauth
 */
- (void)connect DEPRECATE_PUSH_CONNECT_DISCONNECT;

/** Terminate the connection to the Push Channel service (deprecated).
 *
 * @deprecated This function is deprecated and will be removed in a
 * future release.\n
 * This function enabled management of power consumption by the application
 * code. This is no longer required because of advances in the built-in power
 * management of mobile operating systems and devices.
 * 
 * Call this function to terminate the Push Channel connection with the BlackBerry
 * Dynamics proxy infrastructure Network Operation Center (NOC).
 *
 * If the connection was open and operating, termination will result in the
 * <tt>onStatus</tt> callback in the delegate being
 * invoked.
 * @throws_GDauth
 */
- (void)disconnect DEPRECATE_PUSH_CONNECT_DISCONNECT;

/** Delegated event-handling (deprecated).
 *
 * @deprecated This property is deprecated and will be removed in a future
 *             release. Use the <tt>NSNotification</tt> mechanism to receive
 *             notifications instead. See under Notifications in the Programming
 *             Interface, above.
 * 
 * The Push Connection object works asynchronously. When its state changes, an
 * event is generated by the BlackBerry Dynamics Runtime, and passed to a
 * callback function in the application.
 *
 * Set this property to an instance of a class that contains the code for the
 * required callback function, i.e. a class that implements the
 *  \reflink GDPushConnectionDelegate GDPushConnectionDelegate\endlink protocol.
 */
@property (GD_NSNULLABLE_PROP nonatomic, weak) id<GDPushConnectionDelegate> delegate;

@end


/** Delegate for handling GDPushChannel state transitions and received Push
 *  Channel notifications (deprecated).
 *
 * 
 * \deprecated This class is deprecated and will be removed in a future
 * release. Instead of setting a PushChannel
 * delegate, register for notifications as
 * described in the \reflink GDPushChannel GDPushChannel\endlink class reference.
 *
 * State changes that occur when using \reflink GDPushChannel GDPushChannel\endlink
 * are handled by creating a class that implements this protocol.
 * The callback for handling received Push Channel notifications is also part
 * of this protocol.
 * @see \ref st01pushchannel
 *
 *  <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Receive Push Channel Token</h3>
 * \code
 * -(void)onChannelOpen:(NSString*)token
 * {
 *     NSLog(@"onChannelOpen token: %@", token);
 *     myApp.pushIsOpen = YES;
 *     myApp.pushToken = token;
 *     [myApp sendPushToken];
 * }
 * \endcode
 * The above snippet shows a simple <tt>onChannelOpen</tt> handler. The
 * following takes place when the Push Channel is opened:
 * - The token is logged to the system monitor
 * - The application's channel state is flagged as connected
 * - The token is stored in the application
 * - The application's <tt>sendPushToken</tt> function is called
 *
 * The <tt>sendPushToken</tt> function, which would be written by the
 * application developer, would send the token to the
 * application server. This could use a socket, an HTTP
 * request, or another means of communication. From the Push Channel
 * point of view, this is an out-of-band communication.
 *
 * The server will use the token to address Push Channel notification messages
 * back to the application. These would be received by the application's
 * onChannelMessage handler.
 *
 * <h3>Receive Push Channel notification</h3>
 * \code
 * -(void)onChannelMessage:(NSString*)data
 * {
 *     NSLog(@"onChannelMessage: %@", data);
 *     [myApp processPush:data];
 * }
 * \endcode
 * The above snippet shows a simple <tt>onChannelMessage</tt> handler.
 *
 * The handler logs the received data to the system monitor, then calls the
 * application <tt>processPush</tt> function. The "payload" of the notification
 * is passed as a parameter to the <tt>processPush</tt> function.
 *
 * The <tt>processPush</tt> function, which would be written by the
 * application developer, could initiate any of the following actions:
 * - Alert the user that new data is available.
 * - Connect to the application server to retrieve the data. (Connection
 * could use a socket, an HTTP
 * request, or another means of communication. From the Push Channel
 * point of view, this is an out-of-band communication.)
 *
 * <h3>Handle Channel Closure</h3>
 * \code
 * -(void)onChannelClose:(NSString*)data
 * {
 *     NSLog(@"onChannelClose: %@", data);
 *     myApp.pushIsOpen = NO;
 *     [myApp discardPushToken:data];
 * }
 * \endcode
 * The above snippet shows a simple <tt>onChannelClose</tt> handler. The
 * following takes place when the Push Channel is closed:
 * - The token is logged to the system monitor
 * - The application's channel state is flagged as not connected
 * - The application <tt>discardPushToken</tt> function is called. The
 * token of the closed channel is passed as a parameter.
 *
 * The <tt>discardPushToken</tt> function would delete the application's
 * copy of the token, possibly after checking that it matches the
 * <tt>whichWas</tt> parameter.
 * The function could also initiate connection of a new Push Channel, which
 * would have a new token. See \reflink GDPushChannel::connect connect\endlink.
 *
 * <h3>Handle Channel Error</h3>
 * \code
 * -(void)onChannelError:(int)error
 * {
 *     NSLog(@"onChannelError: %d", error);
 *     myApp.pushIsOpen = NO;
 *     myApp.pushErr = error;
 *     [myApp discardPushToken];
 * }
 * \endcode
 * The above snippet shows a simple <tt>onChannelError</tt> handler.
 *
 * The handler logs the error code to the system monitor, flags
 * the channel's state as not connected, records the error code in
 * the application, then calls the application <tt>discardPushToken</tt>
 * function.
 *
 * The <tt>discardPushToken</tt> function could do any of the following:
 * - Delete the application's copy of the token.
 * - Set the error state in an ongoing status display.
 * - Depending on the error code, initiate connection of a new
 * Push Channel, which would have a new token. See \reflink GDPushChannel::connect connect\endlink.
 *
 * <h3>Handle Ping Failure</h3>
 * See under  \reflink GDPushChannelDelegate::onChannelPingFail: onChannelPingFail\endlink
 * for an explanation of the Ping Failure feature.

 * \code
 * -(void)onChannelPingFail:(int)error
 * {
 *     NSLog(@"onChannelPingFail %d", error);
 *     if ( error == 605 ) {
 *         [myApp resendPushToken];
 *     }
 * }
 * \endcode
 * The above snippet shows a simple <tt>onChannelPingFail</tt> handler.
 *
 * The handler logs the error code to the system monitor,
 * then calls the application <tt>resendPushToken</tt> function if the
 * token was lost.
 *
 * The <tt>resendPushToken</tt> function, which would be written by the
 * application developer, would send the application's stored token to the
 * application server. This could use a socket, an HTTP
 * request, or another means of communication. From the Push Channel
 * point of view, this is an out-of-band communication.
 *
 * The <tt>resendPushToken</tt> function should expect that the server is
 * not immediately available, perhaps employing a retry policy.
 */
DEPRECATE_GDPUSHCHANNELDELEGATE
@protocol GDPushChannelDelegate

/** Channel opened callback (deprecated).
 * 
 * @deprecated This callback has been replaced by observation of
 *             \ref GDPushChannelOpenedNotification notifications.
 *
 * This callback will be invoked when the associated Push Channel is
 * opened in the BlackBerry Dynamics proxy infrastructure. See 
 * \reflink GDPushChannel::connect connect\endlink. At this point, a Push
 * Channel token will have been issued by the BlackBerry Dynamics proxy
 * infrastructure Network Operation Center (NOC).
 *
 * The function that is invoked must initiate sending of the token to
 * the application server, out of band.
 * The application server will then be able to use the token to address
 * Push Channel notifications back to the application on the device,
 * via the NOC.
 *
 * @see \ref GNP
 *
 * Invocation of this callback also notifies the application on the device
 * that any of the following callbacks could now be invoked:
 * <tt>onChannelMessage</tt>, <tt>onChannelPingFail</tt>, <tt>onChannelClose</tt>.
 *
 * @param token <tt>NSString</tt> containing the
 * Push Channel token issued by the NOC.
 */
- (void)onChannelOpen:(NSString*)token;

/** Push Channel notification callback (deprecated).
 *
 * @deprecated This callback has been replaced by observation of
 *             \ref GDPushChannelMessageNotification notifications. 
 *
 * This callback will be invoked when a Push Channel notification is
 * received on the associated Push Channel. The message will have been sent by the
 * application server, using the Push Channel notify service, which is hosted
 * by the BlackBerry Dynamics Network Operation Center (NOC).
 *
 * The service supports a "payload" of data to be included in the notification.
 * The data could be in any format chosen by the application developer.
 * The payload could also be empty.
 *
 * Note that Push Channel notifications can be received at any time when the
 * channel is open. This includes the interval between the request for
 * disconnection (<tt>disconnect</tt> called) and channel disconnection being
 * finalized (<tt>onChannelClose</tt> received).
 *
 * The function that is invoked could initiate the following actions:
 * - Alert the user that new data is available.
 * - Connect to the application server to retrieve the data.
 * .
 *
 * @param data <tt>NSString</tt> containing the data
 * payload included by the application server, encoded in UTF-8.
 *
 * \note
 * Because of this callback, the application code does not need to maintain a
 * constant connection with the server. This is an important benefit of using
 * the BlackBerry Dynamics Push Channel framework.
 * 
 * @see \ref GNP
 */
- (void)onChannelMessage:(NSString*)data;

/** Channel closed callback (deprecated).
 *
 * @deprecated This callback has been replaced by observation of
 *             \ref GDPushChannelClosedNotification notifications. 
 *
 * This callback will be invoked when the associated Push Channel is
 * closed.
 * This means closed by the remote end, or by the application having
 * called \reflink GDPushChannel::disconnect disconnect\endlink.
 *
 * Invocation of this callback notifies the application on the device
 * that:
 * - The associated Push Channel token cannot be used any more
 * - No more Push Channel notifications will be received on this channel
 *
 * If the <tt>onChannelClose</tt> was not expected, the function that is
 * invoked could alert the user that Push Channel notifications will not be
 * received, or cause this to be displayed as an ongoing state.
 * The function could also initiate release of the Push Channel object.
 * Alternatively, reconnection could be initiated, see
 * \reflink GDPushChannel::connect connect\endlink.
 *
 * @param data Token for the Push Channel that was
 *                                      closed.
 *
 * Note that this callback is only invoked for permanent Push Channel closure.
 * This callback is not invoked for transient losses of channel communication.
 * For example, this callback is not invoked when the mobile device loses
 * packet data coverage or otherwise cannot connect to the BlackBerry Dynamics proxy
 * infrastructure.
 * The status of the connection to the infrastructure can be monitored by using
 * the
 * \link GDReachability\endlink
 * programming interface.
 */
- (void)onChannelClose:(NSString*)data;

/** Generic channel error callback (deprecated).
 *
 * @deprecated This callback has been replaced by observation of
 *             \ref GDPushChannelErrorNotification notifications.
 *
 * This callback is invoked when a permanent error condition is encountered on
 * the associated Push Channel.
 *
 * Invocation of this callback notifies the application that the Push
 * Channel token cannot be used any more, or that the channel could not
 * be connected in the first place. Furthermore, no (more) Push Channel
 * notifications will be received on this channel.
 *
 * The function that is invoked could alert the user that Push Channel
 * notifications will not be received, or cause this to be displayed as an
 * ongoing state.
 * The function that is invoked should initiate reconnection, see
 * \reflink GDPushChannel::connect connect\endlink, after checking that the Push Channel service
 * is available,
 * see \link GDReachability.isPushChannelAvailable\endlink.
 *
 * @param error Reason code for the condition encountered,
 * as follows.
 *
 * <table>
 *     <tr><th>error</th><th>Channel Error reason</th>
 *     </tr>
 *     <tr><td>
 *         0</td>
 *     <td>
 *         Push is not currently connected.</td>
 *    </tr>
 *    <tr><td>
 *         200-499</td>
 *     <td>
 *         Internal error.</td>
 *     </tr>
 *     <tr><td>
 *         500-599</td>
 *     <td>
 *         Internal server error.</td>
 *     </tr>
 *  </table>
 *
 */
- (void)onChannelError:(int)error;

/** Specific Ping Failure callback (deprecated).
 *
 * @deprecated This callback has been replaced by observation of
 *             \ref GDPushChannelPingFailedNotification notifications.
 *
 * This callback is invoked when Ping Failure is encountered on
 * the associated Push Channel.
 *
 * The function that is invoked should initiate resending of the Push
 * Channel token to the application server, if the token has been lost. This is
 * similar to the processing when the channel is initially opened, see
 *  \reflink GDPushChannelDelegate::onChannelOpen: onChannelOpen\endlink. If the application server is
 * able to accept the token, then Push Channel notification can resume.
 *
 * @see \ref GNP
 *
 * Note that Ping Fail notifications can be received at any time when
 * the channel is open.
 * This includes the interval between the request for disconnection
 * (<tt>disconnect</tt> called) and channel disconnection being finalized
 * (<tt>onChannelClose</tt> received).
 *
 * @param error Reason code for the condition encountered,
 * as follows.
 *
 * <table><tr><th>error</th><th>Ping Failure reason</th>
 *     </tr>
 *     <tr><td>
 *         600</td>
 *     <td>
 *         Application server address could not be resolved via DNS</td>
 *     </tr>
 *     <tr><td>
 *         601</td>
 *     <td>
 *         Could not connect to application server address</td>
 *     </tr>
 *    <tr><td>
 *         602</td>
 *     <td>
 *         Application server TLS/SSL certificate invalid</td>
 *     </tr>
 *    <tr><td>
 *         603</td>
 *     <td>
 *         Timed out waiting for application server HTTP response</td>
 *     </tr>
 *     <tr><td>
 *         604</td>
 *     <td>
 *         Application server returned an invalid response</td>
 *     </tr>
 *     <tr><td>
 *         605</td>
 *     <td>
 *         Application server indicated that token is unknown</td>
 *     </tr>
 * </table>
 * Note that only error 605 means that the token has been lost and must be
 * resent.
 */
- (void)onChannelPingFail:(int)error;

@end


/** Manage Push Channel tokens and notifications.
 * 
 * The Push Channel framework is a BlackBerry Dynamics feature used to
 * receive notifications from an application server.
 *
 * Note that the BlackBerry Dynamics Push Channel feature is not part of the
 * native iOS notification feature set.
 *
 * Use of the Push Channel feature in the application is dependent on:
 * - Completion of BlackBerry Dynamics authorization processing.
 * - Connection to the BlackBerry Dynamics infrastructure.
 * .
 *
 * Push Channel data communication doesn't go via the proxy specified in the
 * native settings of the device or computer on which the application is
 * running, if any.
 *
 * @see <a  HREF="http://help.blackberry.com/en/good-control-good-proxy/current/"  target="_blank" >Manuals page for the BlackBerry Dynamics enterprise servers</a > for the Platform Overview.
 * @see \ref threads
 * @see \ref background_execution
 * @see <a
 *     HREF="https://developer.apple.com/library/content/documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/"
 *     target="_blank"
 * >Local and Remote Notification Programming Guide</a> on the apple.com
 * developer website.
 *
 * <h2>Push Channel Usage</h2>
 * Push Channels are established by the BlackBerry Dynamics application, then
 * used by the application server when needed. The sequence of events is as
 * follows.
 * -# The application sets an event handler for Push Channel notifications.
 * -# The application requests a Push Channel token from the BlackBerry Dynamics
 *    proxy infrastructure.
 * -# The application sends the token to its server using, for example, a socket
 *    connection or HTTP request.
 * -# The application can now wait for a Push Channel notification.\n\n
 * Later, when the server has data for the user, the following steps take
 * place:\n\n
 * -# The server sends a Push Channel notification message through the
 *    BlackBerry Dynamics proxy infrastructure. The message is addressed using
 *    the token.
 * -# The message is sent on, and the waiting application's event handler is
 *    invoked.\n\n
 * Later, when the server has more data for the user, the following steps
 * take place:\n\n
 * -# The server sends another Push Channel notification message through the
 *    proxy infrastructure. The message is addressed using the same token.
 * -# The message is sent on, and the waiting application's event handler is
 *    invoked again.
 * .
 * (The above is also shown in the \ref sq01pushchannel.)
 *
 * The BlackBerry Dynamics platform keeps data communications between the
 * application and server alive while the application is waiting for a Push
 * Channel notification. This is achieved by sending "heartbeat" messages at an
 * interval that is dynamically optimized for battery and network performance.
 *
 * <h2>Ping Failure</h2>
 * Ping Failure is an optional feature of the Push Channel framework. The
 * application server can register a ping address after receiving the Push
 * Channel token from the mobile application.
 *
 * If the application server registers a ping address, then it will be
 * periodically checked ("pinged") by the BlackBerry Dynamics Network Operation
 * Center (NOC). If the server does not respond to a ping, then the NOC notifies
 * the application that requested the corresponding Push Channel.
 * 
 * The purpose of this feature is to support servers that lose the Push Channel
 * token when they are restarted.
 *
 * See the \ref GNP for details of Ping Failure registration.
 * 
 * <h2>Programming Interface</h2>
 * The Push Channel programming interface is asynchronous and state-based. The
 * application code creates a Push Channel object for each channel it will use,
 * typically one per application server. When a channel changes state, or a
 * channel event occurs, the BlackBerry Dynamics runtime notifies the
 * application.
 * 
 * <h3>Notifications</h3>
 * To notify the application, the runtime posts an <tt>NSNotification</tt>.
 *
 * For all <tt>NSNotification</tt> instances that are Push Channel
 * notifications:
 * - The object will be the Push Channel object for the channel to which the
 *   notification relates.
 * - The name depends on the type of event or change that has occurred. It will
 *   be one of the following values.
 *   - \reflink GDPushChannel::GDPushChannelOpenedNotification GDPushChannelOpenedNotification\endlink.
 *   - \reflink GDPushChannel::GDPushChannelMessageNotification GDPushChannelMessageNotification\endlink.
 *   - \reflink GDPushChannel::GDPushChannelClosedNotification GDPushChannelClosedNotification\endlink.
 *   - \reflink GDPushChannel::GDPushChannelErrorNotification GDPushChannelErrorNotification\endlink.
 *   - \reflink GDPushChannel::GDPushChannelPingFailedNotification GDPushChannelPingFailedNotification\endlink.
 *   .
 * - The contents of the user information dictionary, <tt>userInfo</tt>, depends
 *   on the event type. The following values are used as keys.
 *   - \reflink GDPushChannel::GDPushChannelTokenKey GDPushChannelTokenKey\endlink.
 *   - \reflink GDPushChannel::GDPushChannelMessageKey GDPushChannelMessageKey\endlink.
 *   - \reflink GDPushChannel::GDPushChannelErrorKey GDPushChannelErrorKey\endlink.
 *   .
 * .
 * Push Channel state changes can also be detected by key-value observing (KVO)
 * of the \reflink GDPushChannel::state state\endlink property.
 * 
 * @see <a
      href="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/KeyValueObserving/KeyValueObserving.html"
      target="_blank"
 * >Key-Value Observing Programming Guide</a
 * > on the apple.com developer website.
 * @see <a
      href="https://developer.apple.com/reference/foundation/nsnotificationcenter"
      target="_blank"
 * >NSNotificationCenter class reference</a
 * > on the apple.com developer website.
 *
 * <h3>State cycle</h3>
 * The availability of functions in the Push Channel programming interface,
 * and what actions take place, are detailed below, and summarized in the
 * following table. The table also summarizes which notifications are expected
 * in each state.
 * <table>
 *     <tr><th>
 *          State</th>
 *     <th>
 *          Functions and actions</th>
 *     <th>
 *          Expected notifications\n See \ref pushchannelconstants</th>
 *
 *  </tr><tr><td>
 *         Prepared</td>
 *     <td>
 *         Application can call <tt>connect</tt>: state becomes Connecting</td>
 *     <td>
 *         None</td>
 *
 *     </tr><tr><td>
 *         Connecting</td>
 *     <td>
 *         BlackBerry Dynamics runtime requests a new channel from the proxy
 *         infrastructure</td>
 *     <td>
 *         <tt>GDPushChannelErrorNotification</tt>: new
 *         state is Failed\n
 *         <tt>GDPushChannelOpenedNotification</tt>: new
 *         state is Open</td>
 *
 *     </tr><tr><td>
 *         Open</td>
 *     <td>
 *         Application can call <tt>disconnect</tt>: state becomes
 *         Disconnecting</td>
 *     <td>
 *         <tt>GDPushChannelMessageNotification</tt>:
 *         no state change\n
 *     <tt>GDPushChannelPingFailedNotification</tt>:
 *         no state change\n
 *         <tt>GDPushChannelClosedNotification</tt>:
 *         new state is Disconnected</td>
 *
 *     </tr><tr><td>
 *         Disconnecting</td>
 *     <td>
 *         BlackBerry Dynamics Runtime requests the proxy infrastructure to
 *         close the channel</td>
 *     <td>
 *         <tt>GDPushChannelMessageNotification</tt>:
 *         no state change\n
 *      <tt>GDPushChannelPingFailedNotification</tt>:
 *         no state change\n
 * <tt>GDPushChannelClosedNotification</tt>:&nbsp;new&nbsp;state&nbsp;is&nbsp;Disconnected</td>
 *
 *     </tr><tr><td>
 *         Disconnected</td>
 *     <td>
 *         Application can call <tt>connect</tt>: state becomes Connecting</td>
 *     <td>
 *         None</td>
 *
 *     </tr><tr><td>
 *         Failed</td>
 *     <td>
 *         Application can call <tt>connect</tt>: state becomes Connecting</td>
 *     <td>
 *         None</td>
 *     </tr>
 *  </table>
 * The transitions in the above table are also shown in the
 * \ref st01pushchannel. Note that an individual Push Channel might or might not
 * be closed when the overall Push Channel service becomes unavailable.
 *
 * @see \ref GNP
 * 
 * <h3>Notification feature differences</h3>
 * The capabilities of the BlackBerry Dynamics Push Channel are different to the
 * capabilities of the native Apple Push Notification Service (APNS) in the
 * following ways.
 *
 * Only native notifications can be received when the application is in
 * background. This might change in a future release of iOS.
 *
 * In principle, native notifications alert the user, not the application.
 * Having been alerted, the user may choose to open the application. BlackBerry
 * Dynamics Push Channel messages alert the application, which in turn may alert
 * the user.
 *
 * BlackBerry Dynamics Push Channel messages can include a "payload" of
 * application data from the server. The application data is conveyed by the
 * proxy infrastructure from the server to the application.
 * 
 * Native notifications may be received whenever the device has a connection to
 * APNS. BlackBerry Dynamics Push Channel messages may be received whenever the
 * application has a connection to the BlackBerry Dynamics infrastructure.
 *
 * <h2>Push Channel Identifiers</h2>
 * Every Push Channel must have an identifier. Identifiers must be unique within
 * an application. A Push Channel identifier is a text string set by the
 * application code. Note that Push Channel identifiers aren't the same as Push
 * Channel tokens. Token values are set by the BlackBerry Dynamics
 * infrastructure and runtime.
 *
 * The following convention represents best practice for Push Channel
 * identifiers.
 *
 *  \htmlonly <div class="bulletlists"> \endhtmlonly
 * A Push Channel identifier should be composed of a <em>domain</em> followed by
 * a module <em>name</em> and an optional <em>purpose</em>. The parts are
 * separated by full stops (periods). For following examples illustrate the
 * convention.
 *
 * Example: <tt>com.example.mobile-life.email</tt>
 * - Domain is "com.example"
 * - Name is "mobile-life"
 * - Purpose is "email"
 * .
 *
 * Example: <tt>com.example.dashboard</tt>
 * - Domain is "com.example"
 * - Name is "dashboard"
 * - Purpose is omitted.
 * .
 *
 * The rules for identifier part values are as follows.
 * - Domain must be the reversal of an Internet domain that is owned by the
 *   developer organisation.
 * - Name must be unique within all the organisation's applications, libraries,
 *   and other code modules that might create a Push Channel. Ensuring
 *   uniqueness of name values is the responsibility of the developer
 *   organisation.
 * - Purpose need only be used in the case that a single application, library,
 *   or other code module uses more than one Push Channel. Ensuring uniqueness
 *   of purpose values is the responsibility of the code module's developer.
 * .
 *  \htmlonly </div> \endhtmlonly
 *
 *  <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Open Push Channel</h3>
 * The following snippet shows a Push Channel being created and opened after
 * checking that the service is available.
 * \code
 * if ([GDReachability sharedInstance].isPushChannelAvailable) {
 *     NSLog( @"Push Channel service available");
 *     myChannel = [[GDPushChannel alloc] init];
 *     myHandler = [[AppChannelHandler alloc] init]
 *     NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
 *     [notificationCenter addObserver:myHandler
 *                            selector:@selector(channelOpenedNotification:)
 *                                name:GDPushChannelOpenedNotification
 *                              object:myChannel];
 *
 *     [notificationCenter addObserver:myHandler
 *                            selector:@selector(channelClosedNotification:)
 *                                name:GDPushChannelClosedNotification
 *                              object:myChannel];
 *
 *     [notificationCenter addObserver:myHandler
 *                            selector:@selector(channelErrorNotification:)
 *                                name:GDPushChannelErrorNotification
 *                              object:myChannel];
 *
 *     [notificationCenter addObserver:myHandler
 *                            selector:@selector(channelPingFailedNotification:)
 *                                name:GDPushChannelPingFailedNotification
 *                              object:myChannel];
 *
 *     [notificationCenter addObserver:myHandler
 *                            selector:@selector(channelMessageReceivedNotification:)
 *                                name:GDPushChannelMessageNotification
 *                              object:myChannel];
 *     [myChannel connect];
 * }
 * \endcode
 * The above snippet shows the following taking place:
 * - Availability logged to the system monitor.
 * - Allocation of a Push Channel object.
 * - Allocation of an application Push Channel event handler.
 * - Registration of the new handler for all notifications from the channel.
 *   All notifications are sent to the same handler instance, but to different
 *   functions, specified as selectors.
 * - Attempt to connect the Push Channel.
 * .
 * 
 * The attempt is asynchronous. The associated
 * <tt>GDPushChannelOpenedNotification</tt>
 * notification would be received if and when the attempt is succesful (not
 * shown).
 *
 * <h3>Close Push Channel</h3>
 * \code
 * [myChannel disconnect];
 * \endcode
 * The above snippet shows: Request to disconnect the Push Channel.
 * 
 * The request is asynchronous. The associated
 * <tt>GDPushChannelClosedNotification</tt>
 * notification would be received when the closure is finalized (not shown).
 *
 *  <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Handle Push Channel Opening</h3>
 * The following snippet shows a simple handler for when a Push Channel opens.
 * \code
 * -(void)channelOpenedNotification:(NSNotification *)notification
 * {
 *     NSString *token = notification.userInfo[GDPushChannelTokenKey];
 *     NSLog(@"GDPushChannelOpenedNotification token: %@", token);
 *     myApp.pushIsOpen = YES;
 *     myApp.pushToken = token;
 *     [myApp sendPushToken];
 * }
 * \endcode
 * The above snippet shows the following taking place:
 * - The Push Channel token is extracted from the user information dictionary in
 *   the notification.
 * - The token is logged to the system monitor.
 * - The application flags internally that its channel is open, and stores the
 *   token value.
 * - The <tt>sendPushToken</tt> function in the application is called.
 * .
 *
 * The <tt>sendPushToken</tt> function, which would be written by the
 * application developer, would send the token to the application server. This
 * could use a socket, an HTTP request, or another means of communication. From
 * the Push Channel point of view, this is an out-of-band communication.
 *
 * The server will use the token to address Push Channel notification messages
 * back to the application. These would be received by the application's
 * Push Channel event handler code.
 *
 * <h3>Receive Push Channel Message</h3>
 * \code
 * - (void)channelMessageReceivedNotification:(NSNotification *)notification
 * {
 *     NSString *data = notification.userInfo[GDPushChannelMessageKey];
 *     NSLog(@"channelMessageReceivedNotification message: %@", data);
 *     [myApp processPush:data];
 * }
 * \endcode
 * The above snippet shows the following taking place when a Push Channel
 * message is received:
 * - The Push Channel message is extracted from the user information dictionary
 *   in the notification.
 * - The message is logged to the system monitor.
 * - The <tt>processPush</tt> function in the application is called and
 *   passed the content of the message.
 *
 * The <tt>processPush</tt> function, which would be written by the
 * application developer, could initiate any of the following actions:
 * - Alert the user that new data is available.
 * - Connect to the application server to retrieve the data. (Connection
 *   could use a socket, an HTTP request, or another means of communication.
 *   From the Push Channel point of view, this is an out-of-band communication.)
 * .
 *
 * <h3>Handle Channel Closure</h3>
 * \code
 * -(void)channelClosedNotification:(NSNotification *)notification
 * {
 *     NSString *data = notification.userInfo[GDPushChannelTokenKey];
 *     NSLog(@"GDPushChannelClosedNotification: %@", data);
 *     myApp.pushIsOpen = NO;
 *     [myApp discardPushToken:data];
 * }
 * \endcode
 * The above snippet shows a simple channel closure handler. The following takes
 * place when the Push Channel is closed:
 * - The token is logged to the system monitor.
 * - The application flags internally that its channel isn't open.
 * - The application <tt>discardPushToken</tt> function is called. The token
 *   of the closed channel is passed as a parameter.
 * .
 *
 * The <tt>discardPushToken</tt> function would delete the application's
 * copy of the token, possibly after checking that it matches the previous
 * stored token.
 * 
 * The function could also initiate connection of a new Push Channel, which
 * would have a new token. See \reflink GDPushChannel::connect connect\endlink.
 *
 * <h3>Handle Channel Error</h3>
 * \code
 * - (void)channelErrorNotification:(NSNotification *)notification
 * {
 *     NSInteger errorCode = [notification[GDPushChannelErrorKey] integerValue];
 *     NSLog(@"GDPushChannelErrorNotification: %zd", error);
 *     myApp.pushIsOpen = NO;
 *     myApp.pushErr = error;
 *     [myApp discardPushToken];
 * }
 * \endcode
 * The above snippet shows a simple <tt>Error</tt> handler.
 *
 * The handler logs the error code to the system monitor, flags
 * the channel's state as not connected, records the error code in
 * the application, then calls the application <tt>discardPushToken</tt>
 * function.
 *
 * The <tt>discardPushToken</tt> function could do any of the following:
 * - Delete the application's copy of the token.
 * - Set the error state in an ongoing status display.
 * - Depending on the error code, initiate connection of a new Push Channel,
 *   which would have a new token. See \reflink GDPushChannel::connect connect\endlink.
 *
 * <h3>Handle Ping Failure</h3>
 * See under Ping Failure in the \reflink GDPushChannel GDPushChannel\endlink class reference for an
 * explanation of the Ping Failure feature.
 * \code
 * - (void)channelErrorNotification:(NSNotification *)notification
 * {
 *     NSInteger errorCode = [notification[GDPushChannelErrorKey] integerValue];
 *     NSLog(@"GDPushChannelPingFailedNotification %zd", error);
 *     if ( error == 605 ) {
 *         [myApp resendPushToken];
 *     }
 * }
 * \endcode
 * The above snippet shows a simple Ping Failure handler.
 *
 * The handler logs the error code to the system monitor, then calls the
 * application <tt>resendPushToken</tt> function if the token was lost.
 *
 * The <tt>resendPushToken</tt> function, which would be written by the
 * application developer, would send the application's stored token to the
 * application server. This could use a socket, an HTTP request, or another
 * means of communication. From the Push Channel point of view, this is an
 * out-of-band communication.
 *
 * The <tt>resendPushToken</tt> function should expect that the server is
 * not immediately available, perhaps employing a retry policy.
 */
@interface GDPushChannel : NSObject {
    @private
    void* m_pushChannelInternal;
}

/** Delegated event-handling (deprecated).
 *
 * @deprecated This property is deprecated and will be removed in a future
 *             release. Use the <tt>NSNotification</tt> mechanism to receive
 *             notifications instead. See under Notifications in the Programming
 *             Interface, above.
 * 
 * The Push Channel object works asynchronously. When its state changes, or a
 * Push Channel notification is received, an event is generated by the
 * BlackBerry Dynamics Runtime, and passed to a callback function in the
 * application code.
 *
 * Set this property to an instance of a class that contains the code for the
 * required callback functions, i.e. a class that implements
 * the  \reflink GDPushChannelDelegate GDPushChannelDelegate\endlink protocol.
 */
@property (GD_NSNULLABLE_PROP nonatomic, weak) id<GDPushChannelDelegate> delegate DEPRECATE_GDPUSHCHANNELDELEGATE;

/** Push Channel state.
 *
 * The BlackBerry Dynamics runtime sets this property to one of the
 * GDPushChannelState values to represent the state of the channel.
 * 
 * This property is compatible with key-value observing (KVO).
 */
@property (nonatomic, readonly) GDPushChannelState state;


/** Constructor that prepares a new Push Channel.
 *
 * Call this function to construct a new Push Channel object. This
 * function does not initiate data communication.
 * See \reflink GDPushChannel::connect connect\endlink.
 *
 * @param pushChannelIdentifier <tt>NSString</tt> containing the identifier for this
 *                              Push Channel, see under Push Channel
 *                              Identifiers, above.
 * @throws_GDauth
 */
- (instancetype)initWithIdentifier:(NSString*)pushChannelIdentifier;

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_PUSH_CHANNEL_INIT __attribute((deprecated("Use initWithIdentifier instead")))
#else
#   define DEPRECATE_PUSH_CHANNEL_INIT __attribute((deprecated))
#endif

/** Constructor that prepares a new Push Channel without an identifier
 *  (deprecated).
 *
 * \deprecated This function is deprecated and will be removed in a future
 * release. Use
 * \link GDPushChannel::initWithIdentifier:\endlink
 * instead.
 *
 * Call this function to construct a new Push Channel object. This
 * function doesn't initiate data communication.
 * See \reflink GDPushChannel::connect connect\endlink.
 */
- (instancetype)init DEPRECATE_PUSH_CHANNEL_INIT;


/** Connect Push Channel.
 * 
 * Call this function to open the Push Channel connection. This
 * function can only be called when the channel isn't open.
 *
 * This function causes a request for a Push Channel to be sent to the
 * BlackBerry Dynamics Network Operation Center (NOC). The NOC will create the
 * channel, and issue a Push Channel token, which can then be used to identify
 * the channel.
 *
 * The connection attempt is asynchronous.
 * An <tt>NSNotification</tt> with a user information dictionary,
 * <tt>userInfo</tt>, will be posted to notify the application of the result. If
 * the attempt succeeds, the notification name will be
 * <tt>GDPushChannelOpenedNotification</tt>, and the <tt>userInfo</tt> will
 * contain a <tt>GDPushChannelTokenKey</tt> entry of <tt>NSString</tt> type with
 * the token as its value. If the attempt fails, the notification name will be
 * <tt>GDPushChannelErrorNotification</tt>, and the <tt>userInfo</tt> will
 * contain a <tt>GDPushChannelErrorKey</tt> entry holding an <tt>NSInteger</tt>
 * value.
 *
 * @throws_GDauth
 */
- (void)connect;

/** Disconnect Push Channel.
 * 
 * Call this function to initiate permanent disconnection of the
 * Push Channel. This function can only be called when the channel is open.
 *
 * This function causes a request for Push Channel termination to be sent to
 * the BlackBerry Dynamics Network Operation Center (NOC). The NOC will delete
 * the channel, and invalidate the Push Channel token that was issued when the
 * channel was initially opened, see \reflink GDPushChannel::connect connect\endlink.
 *
 * Disconnection is asynchronous. When disconnection is complete,
 * an <tt>NSNotification</tt> is posted to notify the application. The
 * notification name will be <tt>GDPushChannelClosedNotification</tt>.
 * 
 * Note. This function is for permanent closure of the channel. Transient
 * suspension of Push Channel notifications may be more easily accomplished
 * out-of-band, by direct communication with the application server.
 *
 * If the connection with the NOC is open and operating, and the application
 * server that was sent the token registered for <tt>isDisconnected</tt>, then a
 * disconnect notification will be sent to the application server, by the NOC.
 * See the \ref GNP.
 * 
 * @throws_GDauth
 */
- (void)disconnect;

@end

GD_NS_ASSUME_NONNULL_END

#endif /* __GD_PUSH_H__ */
