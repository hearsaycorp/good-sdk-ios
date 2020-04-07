/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#import <Foundation/Foundation.h>

/** BlackBerry Dynamics log upload state.
 *
 * Use the constants in this enumeration to check the current state of
 * BlackBerry Dynamics activity log upload.
 * The GDLogManager.uploadState property always takes one of these values.
 * 
 * It is only necessary to check the log upload state if the application
 * implements its own log upload monitoring user interface instead of using the
 * built-in user interface. See the \ref GDLogManager.openLogUploadUI
 * function documentation.
 */
typedef NS_ENUM(NSInteger, GDLogUploadState)
{
    /** Log upload hasn't been started.
     *
     * This is the initial state.
     */
    GDLogUploadStateIdle       = 0,

    /** Log upload has been started and is now underway.
     */
    GDLogUploadStateInProgress = 100,

    /** Log upload has finished.
     */
    GDLogUploadStateCompleted  = 200,

    /** The last log upload operation was cancelled.
     * 
     * A new upload hasn't been started.
     */
    GDLogUploadStateCancelled  = 300,

    /** Log upload has been started but is currently suspended.
     */
    GDLogUploadStateSuspended  = 400,

    /** Log upload was suspended but has now been resumed.
     */
    GDLogUploadStateResumed    = 500,
};

/** Notification name for BlackBerry Dynamics activity log upload progress.
 * 
 * Use this value to add an <tt>NSNotificationCenter</tt> observer of BlackBerry
 * Dynamics activity log upload progress. See the \ref GDLogManager class
 * reference for details.
 */
extern NSString * const GDLogUploadNotification;

/** Maximum duration of user detailed logging.
 *
 * The maximum duration for which detailed logging can be switched on by
 * the \ref GDLogManager.detailedLoggingFor: function.
 * 
 * \internal Proposed by Jim, 15 min is considered a reasonable max duration that discourages users from leaving detailed logging on for too long.
 */
extern const NSTimeInterval GDLogManagerDetailedMaximumDuration;

/** BlackBerry Dynamics log manager.
 *
 * This class contains a number of programming interfaces for managing
 * BlackBerry Dynamics activity logs.
 *
 * The BlackBerry Dynamics runtime logs its activity to files in the secure
 * store on the device. The application can use the programming interface in
 * this class to:
 * - Initiate upload of activity log files to a central server.
 * - Show a BlackBerry Dynamics user interface for monitoring the upload
 *   process.
 * - Implement a custom user interface for monitoring the upload process.
 * - Switch on detailed logging, if allowed by enterprise policies.
 * .
 *
 * <h2>Notifications for user interface update</h2>
 * In the case that the application implements a custom user interface, it can
 * be notified of progress by the BlackBerry Dynamics runtime. A notification
 * will be dispatched by the runtime when there is any change to the upload
 * state, or to the amount of data uploaded. For example, a notification would
 * be dispatched when:
 * - Log upload has finished.
 * - The number of bytes sent so far has increased.
 * .
 *
 * When a notification is received, the application code could determine the new
 * status and then update the user interface accordingly, for example:
 * - Update a progress bar to reflect the new number of bytes sent.
 * - Show or hide an option for the end user to cancel upload, based on
 *   whether there is an upload in progress.
 * .
 *
 * The following mechanisms are supported for notifications.
 * - Key-value observing (KVO).
 * - The <tt>NSNotificationCenter</tt> programming interface.
 * .
 * To use KVO, observe the properties of the GDLogManager interface object,
 * which is a singleton. The properties to observe are:
 * - GDLogManager.uploadState
 * - GDLogManager.uploadBytesSent
 * - GDLogManager.uploadBytesTotal
 * .
 * To use <tt>NSNotificationCenter</tt>:
 * - Set the notification name to the \ref GDLogUploadNotification value.
 * - Set the notification object to the \ref GDLogManager object, obtained by
 *     <tt>[G<tt></tt>DLogManager sharedInstance]</tt>.
 * - The posted notifications won't have a <tt>userInfo</tt> dictionary.
 * .
 *
 * @see <a
      HREF="https://developer.apple.com/reference/foundation/nsnotificationcenter"
      target="_blank">NSNotificationCenter class reference</a> in the Apple API
      Reference on the apple.com developer website.
 * @see <a
      HREF="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/KeyValueObserving/KeyValueObserving.html"
      target="_blank">Key-Value Observing Programming Guide</a> on the apple.com
      developer website.
 *
 * Implementation of an observer of log upload notifications is only required if
 * the application has its own monitoring user interface. It should be easier to
 * display the built-in monitoring user interface, by calling the
 * GDLogManager.openLogUploadUI: function.
 * 
 * @see \ref logging.
 */
@interface GDLogManager : NSObject

/** Current activity log upload state.
 * 
 *  @return GDLogUploadState value representing the current upload state.
 */
@property (nonatomic, readonly) GDLogUploadState uploadState;

/** Amount of activity log data uploaded so far.
 *
 * @return <tt>NSInteger</tt> representing the amount of data sent so far, in
 *         bytes.
 */
@property (nonatomic, readonly) NSInteger          uploadBytesSent;

/** Total amount of activity log data to upload.
 *
 * @return <tt>NSInteger</tt> representing the total data to be sent, in bytes.
 */
@property (nonatomic, readonly) NSInteger          uploadBytesTotal;

/** Get a reference to the BlackBerry Dynamics activity log manager object.
 * 
 * This function returns a reference to the BlackBerry Dynamics activity log
 * manager object. The activity log manager is a "singleton class".
 *
 * @return Reference that can be used to call, for example, the
 *         \ref startUpload function.
 */
+ (instancetype)sharedInstance;

/** Upload current activity logs.
 *
 * Call this function to upload the current activity log files for
 * support purposes. The logs will be uploaded to a server in the BlackBerry
 * Dynamics Network Operation Center (NOC).
 *
 * BlackBerry Dynamics support staff have access to the server to which log
 * files are uploaded, and can use the data for support analysis.
 *
 * This function can be used to upload logs even if BlackBerry Dynamics
 * authorization has failed. The user identifier with which the application was
 * activiated will be needed by support staff, to identify uploaded files. The
 * user identifier is typically the end user's enterprise email address. If
 * authorization has failed or been cancelled without a user identifier being
 * successfully entered, then no logs will be uploaded.
 *
 * The upload takes place in background and will be retried as necessary by the
 * runtime. Progress can be monitored and managed by using the programming
 * interface of this class.
 * - The \reflink GDLogManager::openLogUploadUI GDLogManager::openLogUploadUI \endlink function opens a
 *   complete built-in user interface that displays progress and gives the user
 *   a number of options to manage the upload.
 * - The other functions in this class could be utilized instead, to
 *   implement a custom log upload user interface in the application code.
 * .
 *
 * The runtime makes a copy of its current activity log files when this
 * function is called. The copy is retained until upload completes or is
 * cancelled. If an earlier log upload is still in progress when this
 * function is called, the current activity log files are added to the
 * in-progress upload.
 *
 * @return <tt>YES</tt> if upload was started OK.
 * @return <tt>NO</tt> otherwise or if this application is in the Wiped state.
 */
- (BOOL)startUpload;

/** Cancel activity log upload.
 *
 * Call this function to cancel the upload of activity logs, which
 * would have been started by calling 
 * \reflink GDLogManager::startUpload GDLogManager::startUpload \endlink.
 * Upload cannot be resumed after cancellation.
 *
 * @return <tt>YES</tt> if upload was cancelled OK.
 * @return <tt>NO</tt> otherwise or if this application is in the Wiped state.
 */
- (BOOL)cancelUpload;

/** Suspend activity log upload.
 *
 * Call this function to suspend the upload of activity logs, which
 * would have been started by calling
 * \reflink GDLogManager::startUpload GDLogManager::startUpload \endlink.
 * Upload can be resumed after suspension.
 * 
 * @return <tt>YES</tt> if upload was suspended OK.
 * @return <tt>NO</tt> otherwise or if this application is in the Wiped state.
 */
- (BOOL)suspendUpload;

/** Resume activity log upload.
 *
 * Call this function to resume the upload of activity logs after
 * suspension. Upload would have been suspended by calling
 * \reflink GDLogManager::suspendUpload GDLogManager::suspendUpload \endlink.
 *
 * @return <tt>YES</tt> if upload was resumed OK.
 * @return <tt>NO</tt> otherwise or if this application is in the Wiped state.
 */
- (BOOL)resumeUpload;

/** Switch on detailed activity logging in the BlackBerry Dynamics Runtime.
 * 
 * Call this function to switch on detailed logging by the
 * BlackBerry Dynamics runtime.
 *
 * The runtime writes more messages to its activity log if detailed logging
 * is switched on. Detailed logging slows down the runtime, and increases the
 * amount of device storage occupied by the activity logs in the secure store.
 * For these reasons, detailed logging can only be switched on by the
 * application for a specified duration, and there is a maximum allowed
 * duration. The maximum duration is 15 minutes and is given by the
 * \reflink GDLogManager::GDLogManagerDetailedMaximumDuration GDLogManager::GDLogManagerDetailedMaximumDuration \endlink constant.
 *
 * Detailed activity logging can also be switched on by the BlackBerry Dynamics
 * management console, in which there are a number of options. Check the user
 * interface and documentation of the management console for details.
 *
 * The ability to switch on detailed logging from the application code can be
 * blocked by enterprise policy. See the
 * <tt>GDAppConfigKeyPreventUserDetailedLogs</tt> item in the collection
 * returned by the \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink function.
 *
 * Detailed activity logging can be switched off by the application code, if it
 * was switched on by the application code. If detailed logging is switched on
 * by the managment console then it cannot be switched off by the application
 * code. Switch off detailed logging by calling this function and passing
 * zero or a negative duration.
 *
 * @param duration <tt>NSTimeInterval</tt> representation of the duration
 *                 for which detailed logging is to be switched on, in
 *                 seconds.\n
 *                 Pass zero or a negative number to switch off detailed
 *                 logging.
 * 
 * @return <tt>YES</tt> if the parameter value is less than or equal to the
 *         maximum allowed duration.
 * @return <tt>NO</tt> otherwise.
 *
 * @see \ref logging for background. This function relates to the Container
 * Log, not the Console Log.
 * @see <a
      HREF="https://developer.apple.com/reference/foundation/nstimeinterval"
      target="_blank">NSTimeInterval reference</a> in the Apple API
      Reference on the apple.com developer website.
 */
- (BOOL)detailedLoggingFor:(NSTimeInterval) duration;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/** Open the built-in activity log upload user interface.
 *
 * This function opens a complete user interface that gives the
 * user an option to start uploading activity log files, then displays progress
 * and a number of management options.
 *
 * The upload user interface is built in to the BlackBerry Dynamics runtime.
 * This function makes it possible to include the upload user interface in
 * the application's own user interface. It is an alternative to implementing a
 * whole user interface in the application code, using the other functions
 * in this class.
 *
 * @return <tt>YES</tt> if the activity log upload user interface opened OK, or was
 *         already open.
 * @return <tt>NO</tt> if this application is in the Wiped state.
 *
 * 
 */
- (void) openLogUploadUI;
#endif
@end
