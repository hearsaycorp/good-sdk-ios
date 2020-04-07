/*
 * (c) 2018 BlackBerry Limited. All rights reserved.
 *
 */

#pragma once

#import "GDAppResultCode.h"

/** \defgroup gdstateconstants GDState constants
 *
 * Use these constants with the \ref GDState programming interface.
 *
 * These constants are intended to facilitate:
 * - Key-value observing (KVO), with which \ref GDState complies.
 * - Implementing NSNotification observers of GDState.
 * .
 *
 * See the \link GDState \endlink class reference for details.
 * 
 * \{
 */

/** NSNotification name for changes to application configuration and other
 *  settings from the enterprise.
 *  
 *  An NSNotification with this name is posted from \link GDState \endlink
 *  whenever there is a change in any value that is returned by
 *  \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink.
 */
extern NSString* const GDRemoteSettingsUpdateNotification;

/** NSNotification name for changes to services-related configuration.
 *
 * An NSNotification with this name is posted from \link GDState \endlink
 * whenever a change to the services-related configuration of one or more
 * applications has been received. See under \reflink GDiOS::getServiceProvidersFor:andVersion:andServiceType: getServiceProvidersFor: (GDiOS) \endlink.
 */
extern NSString* const GDServicesUpdateNotification;

/** NSNotification name for changes to application-specific policy settings.
 *
 * An NSNotification with this name is posted from \link GDState \endlink
 * whenever one or more changes to application-specific policy settings has been
 * received. See under \reflink GDiOS::getApplicationPolicy getApplicationPolicy (GDiOS) \endlink.
 */
extern NSString* const GDPolicyUpdateNotification;

/** NSNotification name for changes to entitlement.
 *
 * An NSNotification with this name is posted from \link GDState \endlink
 * whenever a change to the entitlement data of the end user has been received.
 * If the entitlements of the end user had previously been checked, by calling
 * the \reflink GDiOS::getEntitlementVersionsFor:callbackBlock: getEntitlementVersionsFor:callbackBlock: (GDiOS) \endlink function,
 * then the entitlements should be checked again now.
 */
extern NSString* const GDEntitlementsUpdateNotification;

/** NSNotification name for pending migration of the end user.
 *
 * An NSNotification with this name is posted from \link GDState \endlink when
 * the end user has been selected for migration between management console
 * instances.
 *
 * Migration between management console instances could occur when end users are
 * moved from a legacy Good Control server to an integrated BlackBerry Unified
 * Endpoint Manager (UEM) server, for example.
 *
 * Migration has two stages:
 * -# The end user is selected, at which point migration is <em>pending</em> and
 *    this notification is posted.
 * -# Migration takes place, after which migration is <em>complete</em> and the
 *    \ref GDContainerMigrationCompletedNotification notification is posted.
 * .
 *
 * A pending migration will be finalized the next time the application starts,
 * as part of BlackBerry Dynamics authorization processing. Migration
 * finalization involves processing that is similar to initial activation.
 * Finalization can't be processed when the application is authorized and
 * running. Its connection to the BlackBerry Dynamics infrastructure would be
 * disrupted, so migration waits for the next application start.
 *
 * This notification enables the application to warn the end user that they will
 * be migrated the next time the application starts.
 */
extern NSString* const GDContainerMigrationPendingNotification;

/** NSNotification name for completion of end user migration.
 *
 * An NSNotification with this name is posted from \link GDState \endlink when
 * the end user has been migrated between management console instances, see
 * \ref GDContainerMigrationPendingNotification.
 *
 * This notification enables the application to clear any warnings it may have
 * shown about pending migration.
 */
extern NSString* const GDContainerMigrationCompletedNotification;

/** NSNotification name for <tt>GDState</tt> change notifications.
 *
 * An NSNotification with this name is posted from \link GDState \endlink
 * whenever there is a change to a \link GDState \endlink property value.
 */
extern NSString* const GDStateChangeNotification;

/** Key in the NSNotification <tt>userInfo</tt> dictionary for the name of the
 *  property that changed.
 */
extern NSString* const GDStateChangeKeyProperty;

/** Key in the NSNotification <tt>userInfo</tt> dictionary for the as-after copy
 *  of the GDState object.
 */
extern NSString* const GDStateChangeKeyCopy;

/** NSNotification name for <tt>GDState</tt> activation change notifications.
 *
 * An NSNotification with this name is posted from \link GDState \endlink
 * whenever an activation event happened.
 */
extern NSString* const GDActivationChangeNotification;

/** Key in the NSNotification <tt>userInfo</tt> dictionary for the
 *  current BBDActivationState value.
 */
extern NSString* const BBDActivationStateKey;

/** Key in the NSNotification <tt>userInfo</tt> dictionary for a
 * localised description for each activation processing event.
 */
extern NSString* const BBDActivationDescriptionKey;

/** Key in the NSNotification <tt>userInfo</tt> dictionary for an
 * index used to access a localised description for each activation processing event
 * from the activationProcessingDescriptions array.
 */
extern NSString* const BBDActivationProcessingIndexKey;

/** Key in the NSNotification <tt>userInfo</tt> dictionary for the
 * activation error message should activation fail.
 */
extern NSString* const BBDActivationErrorKey;

/** Path for KVO of the GDState <tt>activationState</tt> property
 *
 */
extern NSString* const BBDKeyActivationState;

/** Path for KVO of the GDState <tt>isAuthorized</tt> property.
 */
extern NSString* const GDKeyIsAuthorized;

/** Path for KVO of the GDState <tt>reasonNotAuthorized</tt> property.
 */
extern NSString* const GDKeyReasonNotAuthorized;

/** Path for KVO of the GDState <tt>userInterfaceState</tt> property.
 */
extern NSString* const GDKeyUserInterfaceState;

/** Path for KVO of the GDState <tt>currentScreen</tt> property.
 */
extern NSString* const GDKeyCurrentScreen;

/** User interface states.
 *
 * This enumeration represents the possible states of the user interface of a
 * BlackBerry Dynamics application.
 *
 * The \ref GDState <tt>userInterfaceState</tt> property takes one of these
 * values.
 */
typedef NS_ENUM(NSInteger, GDUserInterfaceState)
{
    /** Initial state.
     */
    GDUIStateNone = 0,
    
    /** A screen from the application code is in front.
     */
    GDUIStateApplicationInFront,
    
    /** A built-in screen of the GD runtime library is in front.
     */
    GDUIStateGDLibraryInFront,
    
    /** A designated bypass screen from the application is in front. This state
     *  is only possible if the application uses the Bypass Unlock feature.
     *
     * @see <a 
           href="https://developers.blackberry.com/content/dam/developer-blackberry-com/resource-assets/pdf/BypassUnlock.pdf"
           target="_blank"
       >Bypass Unlock: Application Developer Guide</a> on the application
       developer portal for a description of Bypass Unlock.
     */
    GDUIStateBypassUnlockInFront
};

/** BlackBerry Dynamics user interface screen indication.
 *
 * This enumeration is used to indicate which of the built-in BlackBerry
 * Dynamics screens is in front of the user interface.
 *  
 * The \ref GDState <tt>currentScreen</tt> property takes one of these values,
 * when the <tt>userInterfaceState</tt> property has the value
 * <tt>GDUIStateGDLibraryInFront</tt>.
 */
typedef NS_ENUM(NSInteger, GDLibraryScreen)
{
    /** Indicates that no screen has been shown yet.
     */
    GDLibraryScreenNone = 0,
    
    /** Indicates that the certificate enrollment screen is in front. The
     * application code shouldn't attempt to use secure communication when this
     * screen is in front. The user might be enrolling a certificate that is
     * required for access to the enterprise infrastructure, or for
     * authentication by an application server.
     */
    GDLibraryScreenCertificateImport,
    
    /** Indicates that another screen is in front.
     */
    GDLibraryScreenOther = 1000
};

/** BlackBerry Dynamics activation state
 *
 * This enumeration is used to indicate the current state of the
 * BlackBerry Dynamics activation process.
 */
typedef NS_ENUM(NSInteger, BBDActivationState)
{
    BBDActivationStateNotActivated = 0,
    BBDActivationStateInProgress,
    BBDActivationStateActivated
};

/**
 * \}
 */

/** BlackBerry Dynamics run-time state.
 *
 * This class represents a number of aspects of the BlackBerry Dynamics run-time
 * state. The representation is compliant with key-value observing (KVO). An
 * instance of this class is also used as the object from which notifications of
 * configuration changes are posted.
 * 
 * The following aspects of run-time state are represented here.
 * - Authorization state.
 * - Whether the BlackBerry Dynamics user interface is currently in front of the
 *   application user interface.
 * .
 *
 * Changes to the following types of configuration are posted from here.
 * - Services-related, see under \reflink GDiOS::getServiceProvidersFor:andVersion:andServiceType: getServiceProvidersFor: (GDiOS) \endlink.
 * - Application-specific policy settings, see
 *   \reflink GDiOS::getApplicationPolicy getApplicationPolicy (GDiOS) \endlink.
 * - Entitlement, see
 *   \reflink GDiOS::getEntitlementVersionsFor:callbackBlock: getEntitlementVersionsFor:callbackBlock: (GDiOS) \endlink.
 * - Other settings from the enterprise, see
 *   \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink.
 * .
 *
 * <h2>Synchronous access and notifications</h2>
 * To get a current state synchronously, read the instance's properties. To
 * receive a notification whenever an aspect of the current state changes, use
 * KVO or NSNotificationCenter programming interfaces, with the following notes.
 *
 * Notes on KVO usage:
 * - To get an object whose properties are suitable for observing, access the
 *   \reflink GDiOS::state state (GDiOS) \endlink property.
 * - Use the \ref gdstateconstants to specify the paths of properties of the
 *   observable object.
 * .
 *
 * Notes on <tt>NSNotificationCenter</tt> usage:
 * - Set the notification name to one of the \ref gdstateconstants.
 * - Set the notification object to the GDState instance that is accessible
 *   as the \reflink GDiOS::state state (GDiOS) \endlink property.
 * - In addition to the base NSNotification, the notifications for state changes
 *   will include a <tt>userInfo</tt> dictionary with the following items.
 *   - Name of the property whose change triggered the notification, in the
 *     key named by the <tt>GDStateChangeKeyProperty</tt> constant.
 *   - Copy of the <tt>GDSt</tt><tt>ate</tt> instance as it was after the
 *     change, in the key named by the <tt>GDStateChangeKeyCopy</tt> constant.
 *   .
 * .
 * @see <a
      href="https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/KeyValueObserving/KeyValueObserving.html"
      target="_blank"
 * >Key-Value Observing Programming Guide</a>
 *  on the apple.com developer website.
 * @see <a
      href="https://developer.apple.com/reference/foundation/nsnotificationcenter"
      target="_blank"
 * >NSNotificationCenter class reference</a>
 *  on the apple.com developer website.
 *
 * <h2>Alternatives</h2>
 * Using this class is an alternative to using some parts of using the \link
 * GDiOSDelegate \endlink class.
 *
 * If the application uses this class to monitor authorization, and doesn't
 * implement <tt>GDiOSD</tt><tt>elegate</tt>, then the BlackBerry Dynamics event
 * receiver check must be switched off. Instructions for switching off are on
 * the \ref BuildTimeConfiguration page, under the Event Receiver Check heading.
 * @see Sample code in the GreetingsServer application that comes with the
 *      BlackBerry Dynamics software development kit for iOS.
 * 
 * 
 */
@interface GDState : NSObject

/** BlackBerry Dynamics authorization state.
 *
 * The BlackBerry Dynamics runtime sets this property to <tt>NO</tt> when the
 * application starts.
 * 
 * The BlackBerry Dynamics runtime sets this property to <tt>YES</tt> when
 * authorization processing completes with success. The property value remains
 * the same when the inactivity time expires and the user interface is locked.
 *
 * The BlackBerry Dynamics runtime sets this property to <tt>NO</tt> if the end
 * user's authorization to use the application is permanently withdrawn. For
 * example, when any of the following happen.
 * - A remote lock container management command is received from the enterprise
 *   management console.
 * - An enterprise policy violation is detected.
 * .
 *
 * If this property has the value <tt>YES</tt>, then the prinicipal programming
 * interfaces of BlackBerry Dynamics can be used by the application code. This
 * includes access to the secure store and secure communication, for example.
 *
 * @see \reflink GDiOS \endlink for more details on authorization processing and
 * states.
 */
@property (nonatomic, readonly) BOOL isAuthorized;

/** Reason the user isn't authorized, if they aren't.
 *
 * The BlackBerry Dynamics runtime sets this property when the user isn't
 * authorized, i.e. when the \ref isAuthorized property is <tt>NO</tt>. The value
 * set represents the reason that the user isn't authorized. It will be from the
 * \ref GDAppResultCode enumeration.
 */
@property (nonatomic, readonly) GDAppResultCode reasonNotAuthorized;

/** Which user interface is in front.
 *
 * The BlackBerry Dynamics runtime sets this property to
 * <tt>GDUIStateGDLibraryInFront</tt> when any of its built-in screens is in
 * front of the application user interface.
 *
 * The BlackBerry Dynamics runtime sets this property to a different value
 * otherwise. See the \ref GDUserInterfaceState enumeration.
 */
@property (nonatomic, readonly) GDUserInterfaceState userInterfaceState;

/** Limited indication of which screen from the BlackBerry Dynamics user
 *  interface is in front, if any.
 *
 * The BlackBerry Dynamics runtime sets this property when its user interface is
 * in front, i.e. when it has set <tt>userInterfaceState</tt> to
 * <tt>GDUIStateGDLibraryInFront</tt>. The value set gives a limited indication
 * of which screen it is showing. See the \ref GDLibraryScreen enumeration.
 */
@property (nonatomic, readonly) GDLibraryScreen currentScreen;

/** BlackBerry Dynamics activation state
 *
 * This property is used to indicate the current state of the
 * BlackBerry Dynamics activation process.
 */
@property (nonatomic, readonly) BBDActivationState activationState;

/** An Array containing static descriptions of Blackberry Dynamics activation processing states.
 *
 * NSArray of NSString descriptions of Blackberry Dynamics activation processing states.
 * To retrieve the current activation processing state use \ref activationProcessingIndex
 */
@property (nonatomic, readonly) NSArray<NSString *> *activationProcessingDescriptions;

/** An integer representing the current activation processing stage.
 *
 * Use this index to access the descriptions contained within \ref activationProcessingDescriptions
 */
@property (nonatomic, readonly) NSUInteger activationProcessingIndex;
@end
