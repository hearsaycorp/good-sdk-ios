/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 * 
 */

#pragma once

#import <Foundation/Foundation.h>

/** Constants for GDAppEvent type.
 * 
 * This enumeration represents the type of a GDAppEvent that is being notified.
 * The \ref GDAppEvent.type property will always take one of these values.
 */
typedef NS_ENUM(NSInteger, GDAppEventType)
{
    /** Either the user has been authorized to access the application and its
     *  data, following authorization processing, or a condition that caused
     *  authorization to be withdrawn has been cleared.
     * 
     * The user can be given access to the application data, and the application
     * can make full use of the BlackBerry Dynamics programming interface.
     *
     * The event result code will be <TT>GDErrorNone</TT>.
     *
     * See \reflink GDiOS::authorize: authorize (GDiOS) \endlink for authorization processing initiation.
     */
    GDAppEventAuthorized = 0,
    
    /** Either the user hasn't been authorized to access the application and its
     *  data, following authorization processing, or a condition has arisen that
     *  caused authorization to be withdrawn.
     * 
     * The application must deny the user access to any application data. This
     * includes not displaying any data in the application user interface.
     *
     * In the case that the user is found not to be authorized following
     * authorization processing, the application cannot make use of the
     * BlackBerry Dynamics programming interface, except to initiate
     * authorization processing again. Otherwise, if authorization has only been
     * withdrawn, the application can make use of the BlackBerry Dynamics
     * programming interface.
     *
     * The event result code will indicate the condition that has arisen.
     * See \ref GDAppResultCode.
     *
     * See \reflink GDiOS::authorize: authorize (GDiOS) \endlink for authorization processing initiation.
     */
    GDAppEventNotAuthorized = 1,
    
    /** A change to application configuration or other settings from the
     *  enterprise has been received.\ An event of this type is dispatched
     *  whenever there is a change in any value that is returned by
     *  \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink.
     */
    GDAppEventRemoteSettingsUpdate = 2,
    
    /** A change to services-related configuration of one or more applications
     *  has been received.\ See under \reflink GDiOS::getServiceProvidersFor:andVersion:andServiceType: getServiceProvidersFor: (GDiOS) \endlink.
     */
    GDAppEventServicesUpdate = 3,
    
    /** A change to one or more application-specific policy settings has been
     *  received.\ See under \reflink GDiOS::getApplicationPolicy getApplicationPolicy (GDiOS) \endlink.
     */
    GDAppEventPolicyUpdate = 4,
    
    /** A change to the entitlements data of the end user has been received.\ If
     *  the entitlements of the end user had previously been checked, by calling
     *  the \reflink GDiOS::getEntitlementVersionsFor:callbackBlock: getEntitlementVersionsFor:callbackBlock: (GDiOS) \endlink
     *  function, then the entitlements should be checked again now.
     */
    GDAppEventEntitlementsUpdate = 6,
    
    /** The application has been authorized to access its
    *   data as a result of the system starting the application process from cold in the background,
    *   following background authorization processing.
    *
    *   The application can make full use of the BlackBerry Dynamics programming interface.
    *
    *   The event result code will be <TT>GDErrorNone</TT>.
    *
    *   See \reflink GDiOS::authorize: authorize (GDiOS) \endlink for authorization processing initiation.
    */
    GDAppEventBackgroundAuthorized = 7,
    
    /** The application hasn't been authorized to access its
     *  data, following background authorization processing, or a condition has arisen that
     *  caused background authorization to be withdrawn.
     *
     * The application must deny the user access to any application data.
     *
     * The event result code will indicate the condition that has arisen.
     * See \ref GDAppResultCode.
     *
     * See \reflink GDiOS::authorize: authorize (GDiOS) \endlink for authorization processing initiation.
     */
    GDAppEventBackgroundNotAuthorized = 8,
};

/** Constants for GDAppEvent result code.
 *
 * This enumeration represents the possible reasons that the end user isn't
 * authorized. Its values are used in the following programming interfaces.
 * - In a \ref GDAppEvent that has been dispatched to the application, the
 *   <tt>code</tt> property takes one of these values. The code can be
 *   interpreted in conjunction with the event type, see \ref GDAppEventType.
 * - The \ref GDState <tt>reasonNotAuthorized</tt> property takes one of these
 *   values if the <tt>isAuthorized</tt> property has the value <tt>NO</tt>.
 * .
 *
 * \ingroup gdappevent
 */
typedef NS_ENUM(NSInteger, GDAppResultCode)
{
    /** Used for all non-failure events. */
    GDErrorNone = 0,
    
    /** Device activation failed.\ Device activation is part of authorization
     *  processing.\ This code notifies the application that processing didn't
     *  succeed this time, but might succeed if another attempt is made.\ See
     *  under \reflink GDiOS::authorize: authorize (GDiOS) \endlink.
     */
    GDErrorActivationFailed = -101,
    
    /** Enterprise activation failed.\ Enterprise activation is part of
     *  authorization processing.\ This code notifies the application that
     *  processing didn't succeed this time, but might succeed if another
     *  attempt is made.\ See under \reflink GDiOS::authorize: authorize (GDiOS) \endlink.
     */
    GDErrorProvisioningFailed = -102,
    
    /** Connection to the Push Channel service failed but is required to
     *  complete authorization.\ This code notifies the application that
     *  authorization processing didn't succeed this time, but might succeed if
     *  another attempt is made. See \reflink GDiOS::authorize: authorize (GDiOS) \endlink.
     */
    GDErrorPushConnectionTimeout = -103,
    
    /** User not entitled.\ Authorization processing has completed, but the user
     *  isn't entitled to use this application.\ This code notifies the
     *  application that the BlackBerry Dynamics container has been wiped of all
     *  application data and authentication credentials.
     *
     * If entitlement was withdrawn in error then, after reinstating
     * entitlement, the following steps must be taken.
     * -# The user must terminate the application on the device, for example,
     *    using the native task manager, and then restart the application.
     * -# The application will then open as if being started for the first time.
     * -# The user will then have to activate the application.
     * .
     * 
     * This code is utilized when the end user is not entitled to any version of
     * the application. Compare the <TT>GDErrorAppVersionNotEntitled</TT> code,
     * below.
     *
     * @see \ref GC.
     */
    GDErrorAppDenied = -104,
    
    /** User not entitled to this version.\ Authorization processing has
     *  completed, but the user isn't entitled to this version of this
     *  application.\ This code notifies the application that the BlackBerry
     *  Dynamics container has been locked and isn't accessible.
     *
     * If entitlement to the version is later granted, or reinstated, then the
     * lock is removed. The device must be on-line and able to connect to the
     * BlackBerry Dynamics infrastructure, and the user will have to
     * re-authenticate, in order to complete the removal of the lock.
     *
     * This code is utilized when the end user is entitled to at least one other
     * version of the application, but not to the current version. Compare the
     * <TT>GDErrorAppDenied</TT> code, above.
     *
     * @see \ref GC.
     */
    GDErrorAppVersionNotEntitled = -105,
    
    /** User inactive.\ The enterprise's security policies specify a time after
     *  which the application is to be locked, and the user has now been
     *  inactive for a period that exceeds this time.\ In effect, the user's
     *  authorization to access the application data has been withdrawn.\ This
     *  code notifies the application that the BlackBerry Dynamics unlock screen
     *  is active and therefore the application's own user interface mustn't be
     *  made active.
     *
     * The locked condition will be cleared when the user enters their password,
     * at which point the application will be notified with a new event.
     */
    GDErrorIdleLockout = -300,
    
    /** Policy violation block.\ The enterprise's security policies specify a
     *  condition under which access is to be blocked, and that condition has
     *  occurred.\ In effect, the user's authorization to access the application
     *  data has been withdrawn.\ This code notifies the application that its
     *  user interface must not be made active. (Compare <TT>GDErrorWiped</TT>,
     *  below.)
     *
     * This code may be set when, for example, connection to the BlackBerry
     * Dynamics infrastructure hasn't been made for a specified interval. If the
     * condition is cleared, the application will be notified with a new event.
     */
    GDErrorBlocked = -301,
    
    /** Policy violation wipe.\ The enterprise's security policies specify a
     *  condition under which the secure container is to be wiped, and that
     *  condition has occurred.\ This code notifies the application that the
     *  container has been wiped of all application data and authentication
     *  credentials. (Compare <TT>GDErrorBlocked</TT>, above, which also gives
     *  an example of a policy condition.)
     *
     * After a device wipe, the application cannot be run until the following
     * steps have been taken.
     * -# The user must terminate the application on the device, for example,
     *    using the native task manager, and then restart the application.
     * -# The application will then open as if being started for the first time.
     * -# The user will then have to activate the application.
     * .
     */
    GDErrorWiped = -302,
    
    /** Remote lock-out.\ Either a remote lock command has been received from
     *  the enterprise management console,\n
     *  or the security password on the device has been retried too often.\ In
     *  effect, the user's authorization to access the application data has been
     *  withdrawn.\ This code notifies the application that its user interface
     *  mustn't be made active.
     *
     * The user's authorization will remain withdrawn until the lock is removed
     * at the enterprise management console, and the end user has entered a
     * special unlock code at the device.
     */
    GDErrorRemoteLockout = -303,
    
    /** Password change required.\ The user's security password has expired, or
     *  no longer complies with enterprise security policy.\ In effect, the
     *  user's authorization to access the application data has been
     *  withdrawn.\ This code notifies the application that the BlackBerry
     *  Dynamics password change screen is active and therefore the
     *  application's own user interface mustn't be made active.
     */
    GDErrorPasswordChangeRequired = -304,
    
    /** Internal error: Secure store couldn't be unlocked.
     */
    GDErrorSecurityError = -100,
    
    /** Programmatic activation connection failed.\ It wasn't possible to
     *  establish a data connection for programmatic activation.\ This code
     *  notifies the application that programmatic activation didn't succeed
     *  this time, but might succeed if another attempt is made.\ See under
     *  \reflink GDiOS::authorize: authorize (GDiOS) \endlink.
     */
    GDErrorProgrammaticActivationNoNetwork = -601,
    
    /** Programmatic activation credentials failed.\ The credential values
     *  supplied for programmatic activation were rejected during some stage of
     *  activation processing.\ This code notifies the application that
     *  programmatic activation didn't succeed this time, but could succeed if
     *  another attempt is made with different credential values.\ See under
     *  \reflink GDiOS::authorize: authorize (GDiOS) \endlink.
     */
    GDErrorProgrammaticActivationCredentialsFailed = -602,
    
    /** Programmatic activation server communication failed.\ A data connection
     *  was established but communication with a required server resource
     *  subsequently failed.\ This code notifies the application that
     *  programmatic activation didn't succeed this time, but could succeed if
     *  another attempt is made later.\ See under \reflink GDiOS::authorize: authorize (GDiOS) \endlink.
     *
     * It is recommended not to make repeated attempts at programmatic
     * activation with no delay between attempts. Instead, an exponential
     * back-off algorithm should be used to calculate a delay.
     */
    GDErrorProgrammaticActivationServerCommsFailed = -603,
    
    /** Programmatic activation failed.\ A general failure occurred during
     *  programmatic activation processing.\ This code notifies the application
     *  that programmatic activation didn't succeed this time, but could succeed
     *  if another attempt is made.\ See under \reflink GDiOS::authorize: authorize (GDiOS) \endlink.
     */
    GDErrorProgrammaticActivationUnknown = -600
};
