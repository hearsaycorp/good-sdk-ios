/*
 * (c) 2018 BlackBerry Limited. All rights reserved.
 *
 */

#ifndef __GD_IOS_H__
#define __GD_IOS_H__

#import <UIKit/UIKit.h>
#import "GDAppResultCode.h"
#import "GDPortability.h"
#import "GDAppConfig.h"
#import "GDVersion.h"

/* \cond DOXYGEN_IGNORE */
// See: http://clang.llvm.org/docs/LanguageExtensions.html
#ifndef __has_extension
#define __has_extension(x) 0  // Compatibility with non-clang compilers.
#endif
/* \endcond */

@class GDServiceProvider, GDVersion, GDState;


/** Constants for BlackBerry Dynamics service type.
 *
 * This enumeration represents the type of service for which a service discovery
 * query is being issued. The <tt>serviceType</tt> parameter of the
 * \reflink GDiOS::getServiceProvidersFor:andVersion:andServiceType: getServiceProvidersFor: (GDiOS) \endlink function always takes one of these values.
 */
typedef NS_ENUM(NSInteger, GDServiceType)
{
    /** Application-based service. */
    GDServiceTypeApplication=0,
    /** Server-based service. */
    GDServiceTypeServer,
};


/** Deprecated constants for BlackBerry Dynamics service provider type.
 *
 * \deprecated This enumeration is deprecated and will be removed in a future
 * release. Use \ref GDServiceType instead.
 *
 * This enumeration represents the type of service for which a service discovery
 * query is being issued. The <tt>serviceProviderType</tt> parameter of the
 * \reflink GDiOS::getServiceProvidersFor:andVersion:andServiceType: getServiceProvidersFor: (GDiOS) \endlink function always takes one of these values.
 */
typedef NS_ENUM(NSInteger, GDServiceProviderType)
{
    /** Application-based service.
     *
     * \deprecated The enumeration is deprecated and will be removed in a future
     * release. Use \ref GDServiceTypeApplication instead.
     */
    GDServiceProviderApplication __deprecated_enum_msg("Use GDServiceTypeApplication instead.") = 0,
    
    /** Server-based service.
     *
     * \deprecated The enumeration is deprecated and will be removed in a future
     * release. Use \ref GDServiceTypeServer instead.
     */
    GDServiceProviderServer __deprecated_enum_msg("Use GDServiceTypeServer instead"),
};


GD_NS_ASSUME_NONNULL_BEGIN

/** Event dispatched from the BlackBerry Dynamics runtime.
 * 
 * This class is used to deliver events to the \reflink GDiOS \endlink event handler
 * in the application. See \ref GDiOSDelegate.
 */
@interface GDAppEvent : NSObject
{
   
    NSString* message;

    GDAppResultCode code;

    GDAppEventType type;
}

/** Textual description of the event.
 * 
 * This property contains a textual description of the event. The text is
 * suitable for display to the end user, at least for reporting diagnostic
 * information for support purposes.
 */
@property (nonatomic, copy) NSString* message;

/** Numeric result code.
 * 
 * This property contains a numeric result code.
 * For success events, the <tt>GDErrorNone</tt> code is used. Other values in
 * the \ref GDAppEvent.code enumeration are used for errors.
 */
@property (nonatomic, assign) GDAppResultCode code;
/* error code 0, means success */

/** Numeric event type.
 * 
 * This property contains a numeric type code. To determine the type of event
 * being notified, compare this property's value to the constants in the \ref
 * GDAppEvent.type enumeration.
 */
@property (nonatomic, assign) GDAppEventType type;

@end


/** Handler for events dispatched from the BlackBerry Dynamics Runtime,
 *  including authorization events.
 * 
 * Errors and state changes that occur when using \reflink GDiOS \endlink can be
 * handled by creating a class that implements this protocol.
 *
 * @see \ref GDState for an alternative interface.
 */
@protocol GDiOSDelegate
@optional

/** Callback for all events.
 * 
 * The callback is invoked whenever a BlackBerry Dynamics runtime object event
 * occurs. Runtime object events include authorization results, see
 * \reflink GDiOS::authorize: authorize (GDiOS) \endlink for details.
 *
 * @param anEvent GDAppEvent populated with details of the event.
 */
- (void)handleEvent:(GDAppEvent*)anEvent;

@end

/** BlackBerry Dynamics Runtime object interface, including authorization.
 * 
 * This class represents the application's connection to its embedded
 * BlackBerry Dynamics runtime instance, and hence to the wider BlackBerry
 * Dynamics platform infrastructure.
 *
 * The programming interface includes a number of functions that the
 * application must call at particular points in the application execution
 * cycle. The application must also implement a handler for events dispatched
 * from this interface. Calling the functions, and handling the events
 * correctly, ensures compliance with enterprise security policies. This
 * programming interface also includes a number of miscellaneous functions
 * for various purposes, such as setting a custom logo for display in the
 * BlackBerry Dynamics user interface.
 *
 * The application must initialize the runtime object, using this programming
 * interface, prior to using any other BlackBerry Dynamics interface.
 * Initialization will only succeed if the end user has been set up in the
 * enterprise BlackBerry Dynamics management console, and is entitled to use the
 * application.
 * 
 * The user's entitlement to the application may later be revoked or
 * temporarily withdrawn. For example, temporary withdrawal may take place if
 * the user is inactive for a period of time. In either case, the application
 * will be notified with an event or callback from this interface.
 *
 * Successful initialization of the BlackBerry Dynamics interface object also
 * makes the BlackBerry Dynamics proxy infrastructure accessible from within the
 * URL Loading System, which includes the native URL session programming
 * interface.
 *
 * @see \ref GC
 * @see <a  HREF="https://docs.blackberry.com/en/endpoint-management/blackberry-uem/"  target="_blank" >Manuals page for the BlackBerry Dynamics enterprise servers</a > for the Platform Overview.
 * @see \ref threads
 * @see \ref background_execution
 * @see \ref GDURLLoadingSystem for proxy infrastructure usage within the URL
 *      Loading System.
 * @see \ref nsurlsession_support for proxy infrastructure usage through the
 *      native URL session programming interface.
 *
 * <h3>BlackBerry Dynamics Platform Connection</h3>
 * Establishing a connection to the BlackBerry Dynamics platform requires
 * authorization of the end user, and of the application. 
 * Both authorizations are initiated by a single call to the <tt>authorize</tt>
 * function.
 *
 * The <tt>authorize</tt> function call is typically made when the
 * application starts, in the
 * <tt>application:didFinishLaunchingWithOptions:</tt> handler.
 *
 * Authorization generally includes establishing a data connection to the
 * BlackBerry Dynamics proxy infrastructure, and hence to the enterprise that
 * set up the end user entitlement. In addition, authorization will also include
 * any necessary registration of the device, at the BlackBerry Dynamics Network
 * Operation Center (infrastructure activation), and at the enterprise
 * (enterprise activation). See under Activation, below, for more details.
 *
 * Authorization may include user interaction, see the documentation of the
 * authorize function, below, for details. All user interaction that is part
 * of authorization takes place in a user interface that is built into the
 * BlackBerry Dynamics runtime library, not part of the application.
 *
 * The authorization programming interface is state-based and asynchronous. The
 * initiation function generally returns immediately. Success or failure of
 * authorization is then notified to the application code later, as a transition
 * of the <em>authorization state</em>. The application should generally wait to
 * be notified of transition to the "authorized" state before attempting to
 * utilize any other BlackBerry Dynamics interfaces.
 *
 * Further changes to the authorization state can take place, and are notified
 * to the application in the same way. See under Authorization State, below.
 *
 * @see \ref enterprisesimulation for instructions on building an application to
 *      run in a special limited mode in which authorization with the enterprise
 *      is only simulated.
 * @see <a 
    href="https://developers.blackberry.com/content/dam/developer-blackberry-com/resource-assets/pdf/NOC%20server%20addresses.pdf"
    target="_blank"
    >Network Operation Center server addresses</a> on the application developer
 * portal, for IP address and port number details of the BlackBerry Dynamics
 * Network Operation Center services.
 *
 * <h3>Authorization State</h3>
 * The BlackBerry Dynamics runtime maintains the authorization state of the
 * application. The programming interfaces that can be utilized by the
 * application depend on its current authorization state.
 *
 * The initial state of the application when it starts is <em>not
 * authorized</em>. In this state the application can utilize the authorization
 * processing initiation interface but cannot utilize any principal interfaces,
 * such as secure store access and secure communication.
 *
 * After authorization has been initiated and has succeeded, the application
 * enters the <em>authorized </em>state. The principal interfaces can then be
 * utilized.
 *
 * Authorization of the end user may be temporarily withdrawn, in which case the
 * application enters the <em>locked </em>state. This would happen when, for
 * example, the user doesn't interact with the application for an extended
 * period and the enterprise inactivity time out expires. Note that the
 * authorization of the application itself has not been withdrawn in this state,
 * only the authorization of the end user to access the application's data.\n
 * In the locked state, the BlackBerry Dynamics runtime superimposes an unlock
 * screen on the application user interface to prevent the user from interacting
 * with it or viewing its data. Note that the runtime doesn't block the whole
 * device user interface, which means that native notification features and
 * other ancillary mechanisms
 * could still be utilized by the application. The application mustn't cause
 * sensitive enterprise data to be displayed through these features and
 * mechanisms when in the locked state.\n
 * The application can continue to utilize the principal BlackBerry Dynamics
 * interfaces, in the background.
 *
 * After a temporary withdrawal ends, the application returns to the authorized
 * state. This would happen when, for example, the user enters their security
 * password in the unlock screen.
 *
 * Authorization may also be permanently withdrawn, in which case the
 * application enters the <em>wiped </em>state. This would happen when, for
 * example, the end user's entitlement to the application is removed by the
 * enterprise administrator. In the wiped state, the application cannot utilize
 * the principal BlackBerry Dynamics interfaces.
 * 
 * Transitions of the authorization state are notified as follows.
 * - A <tt>GDAppEvent</tt> is dispatched to the
 *   <tt>GDiOSDelegate</tt> instance in the
 *   application. The event will have a number of attributes, including a type
 *   value that indicates whether the user is now authorized.
 * - A <tt>GDStateChangeNotification</tt> is posted to any registered
 *   NSNotification observers. See the \link GDState \endlink class reference for
 *   how to register.
 * - The <tt>isAuthorized</tt> property of the \reflink GDiOS::state state (GDiOS) \endlink object
 *   changes value, which can be notified by key-value observing (KVO). See
 *   again the \link GDState \endlink class reference for details.
 * .
 * 
 * The authorization states and their corresponding event type values are listed
 * in the following table.
  <table>
      <tr
          ><th>State</th
          ><th>Description</th
          ><th
          ><tt>GDApp</tt><tt>Event</tt><br>type value</th
 
      ></tr><tr><td
          >Not authorized</td
      ><td
          >Initial state.\n
          The application can initiate authorization, but cannot utilize the
          principal interfaces.</td
      ><td
          ></td
 
      ></tr><tr><td
          >Authorized</td
      ><td
          >Either the user has just been authorized to access the application,
          following authorization processing, or a condition that caused
          authorization to be withdrawn has been cleared.\n
          The application can utilize the principal interfaces.</td
      ><td
          ><tt>GDAppEventAuthorized</tt></td
 
      ></tr><tr><td
          >Locked</td
      ><td
          >Authorization of the user has been temporarily withdrawn, for
          example due to inactivity.\n
          User interaction is blocked. The application can still utilize the
          principal interfaces.</td
      ><td
          ><tt>GDAppEventNotAuthorized</tt></td
 
      ></tr><tr><td
          >Wiped</td
      ><td
          >Authorization of the user has been permanently withdrawn, for
          example due to violation of an enterprise policy for which the
          enforcement action is to wipe the secure container.\n
          The application cannot use any interfaces.</td
      ><td
          ><tt>GDAppEventNotAuthorized</tt>\n
          This is the same event type as the Locked state transition event.</td
 
      ></tr
  ></table
  >The transitions in the above table are also shown in the
 * \ref st04gdauthorisation.
 *
 * The BlackBerry Dynamics runtime user interface includes all the necessary
 * screens and messages to inform the user of the authorization state. The
 * application code needs only to ensure:
 * - That it doesn't bypass the runtime user interface.
 * - That it doesn't attempt to access the principal interfaces prior to
 *   authorization.
 * - That it doesn't attempt to access the principal interfaces after the
 *   authorization state has changed to wiped.
 * .
 * An authorized application may change an authorization state to 'locked'
 * and prompt the user to reauthenticate by using \reflink GDAuthenticationManager GDAuthenticationManager \endlink.
 *
 * 
 * <h3>Programming interface restrictions</h3>
 * The application cannot use any of the principal BlackBerry Dynamics
 * interfaces before authorization has succeeded. If the application attempts to
 * do so, the runtime generates an assertion,
 * which results in the application being terminated.
 * The runtime uses the Foundation <tt>NSAssert</tt> macro to generate these
 * assertions.
 *
 * The runtime doesn't generate assertions for
 * transient conditions, but only for apparent programming errors in the
 * application. Consequently, these assertions are
 * only expected when the application is in development, and not when the
 * application is in production. The failure message of the
 * assertion will describe the programming
 * error.
 *
 * The recommended approach is that the application should be allowed to
 * terminate, so that the failure message can be read
 * on the console.
 * The failure message will describe the programming error, which can then be
 * corrected.
 * For example, a message like the following could be seen in the logs:\n
 * <tt>My application [7506:40b] *** Terminating app due to uncaught exception
 * 'NSInternalInconsistencyException', reason:
 * 'Not authorized. Call [GDi</tt><tt>OS autho</tt><tt>rize] first.'</tt>
 *
 * <h4>First usage of BlackBerry Dynamics in the execution cycle</h4>
 * The typical first usage of the BlackBerry Dynamics programming interface in
 * the execution cycle is to initiate authorization, in the
 * <tt>application:didFinishLaunchingWithOptions:</tt> handler. The first point
 * in the execution cycle at which it is possible to use the BlackBerry Dynamics
 * interface is the invocation of the
 * <tt>application:willFinishLaunchingWithOptions:</tt> handler. The BlackBerry
 * Dynamics programming interface cannot be used prior to
 * <tt>application:willFinishLaunchingWithOptions:</tt> invocation. For example,
 * if the application uses a subclass of <tt>UIApplication</tt>, the BlackBerry
 * Dynamics programming interface cannot be used in its <tt>init</tt> method.
 *
 * <h3>Activation</h3>
 * In BlackBerry Dynamics, activation refers to a number of registration
 * procedures that must be completed in order to access all platform
 * capabilities. Once a particular activation has been completed, registration
 * credentials are stored on the device. This means that each activation need
 * only be processed once.
 *
 * Activations are generally transparent to the application. The application
 * will call a BlackBerry Dynamics authorization method, and the runtime will
 * process whichever activations are required.
 *
 * There are two activations in BlackBerry Dynamics.<dl
  ><dt
      >Infrastructure activation</dt><dd
          >Recognition of the application instance as a terminal by the
          BlackBerry Dynamics central server.</dd
  ><dt
      >Enterprise activation</dt><dd
          >Association of the terminal with a provisioned end user at the
          enterprise. This requires activation credentials, which may be
          obtained by any of the following means.
          - Entered manually into the BlackBerry Dynamics user interface.
          - Retrieved progammatically via another BlackBerry Dynamics
            application, known as Easy Activation.
          .</dd
  ></dl>
 * @see \ref GC.
 * @see \ref enterprisesimulation for instructions on building an application to
 *      run in a special limited mode in which there is no enterprise
 *      activation.
 *
 * <h3>Identification</h3>
 * Every BlackBerry Dynamics application has a unique identifier and version
 * for the purposes of entitlement management, publishing, and service provider
 * registration. The identifier was previously referred to as the Good Dynamics
 * application identifier, or in the abbreviated form GD App ID, but is now
 * referred to as the <em>entitlement identifier</em>.
 *
 * Entitlement identifiers are used:
 * - In the application.
 * - In the BlackBerry Dynamics management console at the enterprise.
 * - In some administrative user interfaces accessible via the application
 *   developer portal.
 * .
 *
 * A BlackBerry Dynamics entitlement identifier is generally accompanied by a
 * separate entitlement version.
 *
 * In the mobile application, the entitlement identifier and version values are
 * set by the authorization call, as documented in the <tt>authorize</tt>
 * function reference, below. In the management console, the entitlement
 * identifier and version values are entered as part of application management.
 *
 * @see \ref GC.
 *
 * <h4>Entitlement Identifier Format</h4>
 * Entitlement identifiers are textual and follow a typical naming convention.
 * The reversed Internet domain of the application developer forms the initial
 * part of the identifier. For example, applications developed by BlackBerry can
 * have identifiers that begin "com.blackberry." because BlackBerry owns the
 * blackberry.com Internet domain.
 *
 * The rest of the identifier is made up of the application's name, possibly
 * preceded by a number of categories and sub-categories. Categories and
 * sub-categories are separated by full stops. For example, the identifier of an
 * example remote database application, made by BlackBerry, could be:
 * "com.blackberry.gd.examples.remotedb". Note that BlackBerry also owns the
 * good.com domain, so could also use "com.good.gd.examples.remotedb".
 * 
 * Formally, the syntax of an entitlement identifer is a string that:
 * - Contains only the following ASCII characters: hyphen (dash), full stop
 *   (period), numeric digit, lower-case letter.
 * - Conforms to the &lt;subdomain&gt; format initially defined in section 2.3.1
 *   of <a href="http://www.ietf.org/rfc/rfc1035.txt" 
            target="_blank" >RFC1035</a>
     and subsequently modified in section 2.1 of
     <a href="http://www.ietf.org/rfc/rfc1123.txt" target="_blank" >RFC1123</a>.
 * .
 *
 * <h4>Entitlement Version Format</h4>
 * A BlackBerry Dynamics entitlement version is a string made up of a sequence
 * of numbers separated by full stops (periods). The following represents best
 * practice.
 * - The first release of the application should have "1.0.0.0" as its
 *   entitlement version.
 * - The version string should change in subsequent releases in which one of the
 *   following software changes is made:
 *   - The application starts to provide a new shared service or shared service
 *     version.
 *   - The application stops providing a shared service or shared service
 *     version.
 *   .
 *   Otherwise, version should not change in the release.
 * .
 *
 * See the \reflink GDService GDService class reference\endlink for details of shared services.
 *
 * The syntax rules of entitlement version values are as follows.
 * - A version string consists of one to four version numbers separated by full
 *   stop (period) characters.
 * - A version number consists of one of the following:
 *   - A single zero.
 *   - A sequence of up to three digits with no leading zero.
 *   .
 * .
 * The syntax can be formally expressed as the following regular expression:
 * <tt>(0|[1-9][0-9]{0,2})(.(0|[1-9][0-9]{0,2})){0,3}</tt>
 * 
 * Don't use a different entitlement version for an early access or beta
 * software release. Instead, add a suffix to the BlackBerry Dynamics
 * entitlement identifier and native application identifier used for general
 * access. For example, the entitlement identifier "com.example.gdapp.beta"
 * could be used to identify a "com.example.gdapp" beta release.\n
 * Using a different native identifier makes it possible for general access and
 * early access software to be installed on the same mobile device, and
 * facilitates use of different signing certificates.
 *
 * <h3>Application user interface restrictions</h3>
 * See the \ref AppUserInterfaceRestrictions page.
 *
 * <h3>Build-Time Configuration</h3>
 * See the \ref BuildTimeConfiguration page.
 *
 * <h3>Enterprise Configuration Information</h3>
 * There are a number of functions in this class for obtaining enterprise
 * configuration information, including settings that apply to the current end
 * user. The \reflink GDiOS::getApplicationPolicy getApplicationPolicy (GDiOS) \endlink and
 * \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink functions are examples of this
 * type of function.
 *
 * All the functions of this type:
 * - Return their results in a collection of objects.
 * - Have a corresponding \reflink GDAppEvent GDAppEvent \endlink event type that is
 *   dispatched to the application's 
 *   \reflink GDiOSDelegate GDiOSDelegate \endlink 
 *   instance when the result would change.
 * - Have a corresponding <tt>NSNotification</tt> that is posted to any
 *   registered observers, when the result would change. See the
 *   \link GDState \endlink class reference for how to register.
 *
 *
 * For example, the \reflink GDiOS::getApplicationPolicy getApplicationPolicy (GDiOS) \endlink function returns 
 * an <tt>NSDictionary</tt>
 * collection. When there is a change,
 * a <tt>GDPolicyUpdateNotification</tt> is posted, and
 * a <tt>GDAppEventPolicyUpdate</tt> event is dispatched.
 *
 * Use these functions as follows.
 * -# Make a first call to get an initial collection.
 * -# Retain the collection, and refer to it in any code that utilizes its
 *    values.
 * -# When the update event is received, discard the retained collection and
 *    call the function again to get a new collection.
 *
 *
 * Don't make a subsequent call to the same function until an update event
 * has been received. The BlackBerry Dynamics runtime generates a new collection
 * for each call to one of these functions. If the application code makes
 * multiple calls and retains all the returned collections, then they will all
 * consume memory or other application resources.
 * 
 * <h2>Code Snippets</h2> The following code snippets illustrate some common tasks.
 * <h3>Authorization</h3>
 * The following snippet shows initiation of BlackBerry Dynamics authorization.
 * \code
 * [GDiOS sharedInstance].delegate = self;
 * [[GDiOS sharedInstance] authorize];
 * \endcode
 * After executing the above code, the application would wait for its delegate
 * callback to be invoked. The invocation would have an event type of
 * <tt>GDAppEventAuthorized</tt> if the user was authorized. After that, the
 * application could make full use of all BlackBerry Dynamics capabilities.
 *
 * The above code relies on the identification parameters being in the
 * Info.plist file, as shown in the following snippet.
 * \code
 * <key>GDApplicationID</key>
 * <string>com.example.browser</string>
 * <key>GDApplicationVersion</key>
 * <string>1.0.0.0</string>
 * \endcode
 * The above is an extract from the XML of the application's Info.plist file.
 * The extract sets "com.example.browser" as the BlackBerry Dynamics entitlement
 * identifer, and "1.0.0.0" as the BlackBerry Dynamics entitlement version.
 *
 * <h3>User interface pre-initialization</h3>
 * The following snippet shows some necessary steps that precede initialization
 * of the application's user interface. The recommended place in the code for
 * these steps is as shown in the snippet.
 * \code
 *
 * - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
 * { 
 *     GDiOS *gdiOS = [GDiOS sharedInstance];
 *     gdiOS.delegate = self;
 *     started = NO;
 *     // Following line will cause the BlackBerry Dynamics user interface to open.
 *     [gdiOS authorize];
 *     return YES;
 * }
 * \endcode
 *
 * The above snippet shows the following sequence of actions.
 * -# A reference to the GDiOS singleton object is obtained.
 * -# The current class is set as the delegated event handler.
 * -# The <tt>started</tt> flag is set, to control initialization of the
 *    application's user interface, which is covered in the following snippet.
 *    The flag's declaration (not shown) would be <tt>BOOL</tt>.
 * -# The application initiates BlackBerry Dynamics authorization.
 * .
 * Authorization processing is asynchronous. On completion, an event will be
 * dispatched to the application's handler. The application completes user
 * interface initialization within its event handler, as shown in the following
 * code snippet.
 *
 * <h3>User interface initialization</h3>
 * The following snippet shows the recommended place in the code to initialize
 * the application's user interface.
 * \code
 *
 * -(void)handleEvent:(GDAppEvent*)anEvent
 * {
 *     switch (anEvent.type) {
 *         case GDAppEventAuthorized: {
 *             if (!started) {
 *                 [self initializeUI];
 *                 started = YES;
 *             }
 *             break;
 *         }
 *
 *         case GDAppEventNotAuthorized: {
 *             [self onNotAuthorized:anEvent];
 *             break;
 *         }
 *
 *         case GDAppEventRemoteSettingsUpdate:
 *         case GDAppEventServicesUpdate:
 *         case GDAppEventPolicyUpdate:
 *         default: {
 *             // This application ignores other types of event.
 *             break;
 *         }
 *     }
 * }
 * \endcode
 * The above code shows a simple BlackBerry Dynamics runtime event handler.
 *
 * The handler calls the application's own <tt>initializeUI</tt> function when a
 * <tt>GDAppEventAuthorized</tt> event is received for the first time. The
 * application's <tt>started</tt> flag is used to control first-time execution.
 * Initialization of this flag is shown in the previous snippet.\n
 * The <tt>initializeUI</tt> function would complete user interface
 * initialization, utilizing a reference to the BlackBerry Dynamics runtime
 * <tt>UIWindow</tt>. This reference can be obtained using the <tt>window</tt>
 * property from the main app delegate.
 *
 * The handler calls the application's own <tt>onNotAuthorized</tt> function
 * whenever a <tt>GDAppEventNotAuthorized</tt> event is received.
 */
@interface GDiOS : NSObject

/** Get a reference to the runtime interface object.
 * 
 * This function returns a reference to the BlackBerry Dynamics
 * runtime object.
 * This function can be called in the
 * <tt>application:didFinishLaunchingWithOptions:</tt>  
 * handler of the application.
 *
 * The BlackBerry Dynamics runtime object is a "singleton class".
 *
 * @return Reference that can be used to call, for example,\reflink GDiOS::authorize: authorize (GDiOS) \endlink.
 */
+ (instancetype)sharedInstance;

/** Connect to the BlackBerry Dynamics infrastructure.
 * 
 * Call this function to initiate BlackBerry Dynamics authorization
 * processing.
 *
 * 
 * Authorization involves a number of exchanges with various servers in the
 * BlackBerry Dynamics proxy infrastructure, and may involve user interaction.
 * The following processing items are the most significant.<dl>
 *
 * <dt>Infrastructure activation</dt><dd>
 * The BlackBerry Dynamics runtime registers the application instance as a
 * terminal with the BlackBerry Dynamics Network Operation Center (NOC). The NOC
 * issues registration credentials, which are encrypted and stored locally by
 * the runtime. Infrastructure activation is not processed if registration
 * credentials are already present.</dd>
 *
 * <dt>Enterprise activation</dt><dd>
 * Enterprise activation is the completion of the enterprise provisioning
 * process. An access key will have been created by the enterprise management
 * console, and made available to the runtime instance embedded in the
 * application. See under Activation, above, for the methods by which the key
 * can be made available.
 *
 * During enterprise activation, the runtime sends the key to the NOC. If the
 * key is recognized, the application instance is associated with the enterprise
 * that created the key. Enterprise registration credentials are then issued,
 * and encrypted and stored locally by the runtime.\n
 * Enterprise activation is not processed if enterprise registration
 * credentials are already present.\n
 * Note that successful enterprise activation has the effect of consuming the
 * access key. This will be the case even if a later stage of authorization
 * processing fails, or if it is found that the user isn't entitled to this
 * application.</dd>
 *
 * <dt>Entitlement checking</dt><dd>
 * The enterprise that provisioned the end user can later withdraw the
 * user's entitlement to the application. Withdrawal is flagged in the
 * enterprise's management console. The runtime checks the end user's ongoing
 * entitlement to the application every time authorization is processed. (In
 * addition, withdrawal may be notified to the runtime by the NOC.)\n
 * In the management console, entitlement is given to particular versions of
 * particular applications. The entitlement version and identifier values built
 * into the application must match exactly with a version and identifier to
 * which the user has been given entitlement in the management console. If there
 * isn't an exact match, then user entitlement is assumed to have been
 * withdrawn.\n
 * If the user is found not to be entitled, then the runtime deletes all locally
 * stored application data and authentication credentials.</dd>
 *
 * <dt>Policy update</dt><dd>
 * Policies govern many aspects of the BlackBerry Dynamics user experience. For
 * example, the minimum characteristics of the end user's security password with
 * respect to length and types of character are governed by a policy. So is the
 * need for the end user to set a security password. The runtime checks for and
 * retrieves updated policies every time authorization is processed.</dd>
 *
 * <dt>Policy checking</dt><dd>
 * The runtime checks for compliance with the most up-to-date policies every
 * time authorization is processed.</dd>
 *
 * <dt>Configuration retrieval</dt><dd>
 * The runtime retrieves a set of application configuration settings from the
 * enterprise management console during authorization processing. These will
 * have been stored in the management console. See under
 * \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink for a list of settings.</dd>
 *
 * <dt>Unlock secure store</dt><dd>
 * The BlackBerry Dynamics secure store is unlocked during authorization. If the
 * store is protected by an end-user password, then the user must enter their
 * password for authorization processing to complete. Password entry takes place
 * in a BlackBerry Dynamics user interface, not in the application user
 * interface.</dd>
 * </dl>
 * Note that BlackBerry Dynamics user interfaces mentioned in the above, such as
 * Enterprise activation and Password entry, are opened and closed as needed,
 * without reference to the application.
 *
 * This function can be called in the
 * <tt>application:didFinishLaunchingWithOptions:</tt> 
 * handler of the application.
 *
 * This function must be called before any of the principal BlackBerry
 * Dynamics functions.
 *
 * Data communication during authorization processing doesn't go via the proxy
 * specified in the device's native settings, if any.
 *
 * @param delegate Reference to the delegated event handler, typically
 *                 <tt>self</tt>.
 *
 * Application identification parameters will be read from the following properties
 * in the application Info.plist fi<tt></tt>le:<table>
 *     <tr><th>Key</th><th>Type</th><th>Value</th>
 *     </tr><tr><td>
 *         <tt>GDApplicationID</tt></td>
 *         <td><tt>String</tt></td>
 *         <td>BlackBerry Dynamics entitlement identifier.\n
 *         (This identifier was previously referred to as the Good Dynamics
 *         application identifier, or in the abbreviated form GD App ID.)\n
 *         This identifier is used to control access to the application by end
 *         users, and to configure its application server connections. The value
 *         passed must be the same as the value present in the enterprise
 *         management console.\n
 *         (Note that the value needn't be the same as the native application
 *         identifier.)\n
 *         For syntax details see under Entitlement Identifier Format,
 *         above.</td>
 *     </tr><tr><td>
 *         <tt>GDApplicationVersion</tt></td>
 *         <td><tt>String</tt></td>
 *         <td>BlackBerry Dynamics entitlement version number\n The version
 *         number can be combined with the entitlement identifier, above, to
 *         control access to particular versions of the application.</td>
 *     </tr>
 * </table>
 * (In case there are multiple Info.plist files, check that the correct one has
 * been edited by opening the Info tab of the application target being built.
 * The settings should appear there.)
 * 
 * Authorization is asynchronous. When authorization processing completes:
 * - The delegate \link GDiOSDelegate::handleEvent: handleEvent\endlink callback
 *   is invoked.
 * - The \ref GDState singleton object is updated, and any observers of that
 *   object are sent notifications.
 * .
 *
 * @see \ref GC
 * @see \ref enterprisesimulation.
 */
- (void)authorize:(id<GDiOSDelegate> _Nonnull)delegate;

/** Connect to the BlackBerry Dynamics infrastructure, without specifying a
 *  delegate.
 * 
 * Call this function to initiate BlackBerry Dynamics authorization
 * processing without specifying a delegated event handler. This style of call
 * can be used in the following cases.
 * - The <tt>delegate</tt> property has already been set directly, to a
 *   GDiOSDelegate implementation.
 * - The application uses the \ref GDState interface to monitor authorization
 *   state, doesn't implement <tt>GDiOSD</tt><tt>elegate</tt>, and has switched
 *   off the event receiver check. (Instructions for switching off are on the
 *   \ref BuildTimeConfiguration page, under the Event Receiver Check heading.)
 * .
 * 
 * Calling this function is otherwise equivalent to calling the
 * <tt>authorize:</tt> function, above. See \reflink GDiOS::authorize: authorize (GDiOS) \endlink for details.
 */
- (void)authorize;

/** Connect to the BlackBerry Dynamics infrastructure in background.
 *
 * Call this function to initiate BlackBerry Dynamics authorization
 * processing in background.
 *
 * This function should be called when the application is launched in
 * background. This could happen if any of the following are in use.
 * - Apple Push Notification Service (APNS).
 * - Background fetch.
 * .
 *
 * Authorization processing can complete in background, if all the following
 * conditions are met:
 * - Activation has completed. The application must have run in foreground and
 *   completed BlackBerry Dynamics authorization processing at least once for
 *   this to be the case.
 * - Authorization can complete autonomously, without user interaction, i.e. a
 *   <em>no-password</em> enterprise policy is in effect for the current end
 *   user.
 * - The application is currently in background.
 * .
 * 
 * Call this function from any callback that handles background launch, for
 * example the application <tt>didReceiveRemoteNotification:</tt> or
 * <tt>performFetchWithCompletionHandler:</tt> function. Access the
 * \reflink GDiOS::canAuthorizeAutonomously canAuthorizeAutonomously (GDiOS) \endlink property first, to check that a
 * no-password policy applies to the current end user and that autonomous
 * authorization is possible.
 * 
 * If authorization can complete autonomously, calling this function
 * results in the same processing and notification as if \reflink GDiOS::authorize: authorize (GDiOS) \endlink had
 * been called. Application identification parameters will be read from the
 * Info.plist file.
 *
 * @param delegate Reference to the delegated event handler, typically
 *                 <tt>self</tt>.
 *        
 * @return <tt>YES</tt> if authorization can complete autonomously. The application
 *         will be notified when authorization completes, or if it fails for
 *         some reason.
 * @return <tt>NO</tt> if authorization cannot complete autonomously. No further
 *         notification will be dispatched.
 */
- (BOOL)authorizeAutonomously:(id<GDiOSDelegate> _Nonnull)delegate;

/** Connect to the BlackBerry Dynamics infrastructure in background, without
 *  specifying a delegate.
 * 
 * Call this function to initiate autonomous BlackBerry Dynamics
 * authorization processing without specifying a delegated event handler. This
 * style of call can be used in the same cases as the
 * \reflink GDiOS::authorize authorize (GDiOS) \endlink function that takes no parameters, see above.
 * 
 * Calling this function is otherwise equivalent to calling the
 * <tt>authorizeAutonomously:</tt> function, above.
 * 
 * See \reflink GDiOS::authorizeAutonomously: authorizeAutonomously: (GDiOS) \endlink for details.
 */
- (BOOL)authorizeAutonomously;

/** String constants used as keys for programmatic activation parameters.
 *Keys used to set the values of the activation parameters required during programmatic activation.
 *
 * <table>
 *     <tr>
 *         <th>Key Constant</th>
 *         <th>Setting</th>
 *         <th>Type</th>
 *     </tr>
 *     <tr>
 *         <td>
 *            <tt>ActivationParameterUserIdentifier</tt>
 *         </td>
 *         <td>
 *             <tt>NSString</tt> containing the first enterprise activation
 *               credential: the user identifier. This is typically the
 *               enterprise email address of the end user.
 *         </td>
 *         <td>
 *             <tt>NSString</tt>
 *         </td>
 *     </tr>
 *     <tr>
 *         <td>
 *             <tt>ActivationParameterAccessKey</tt>
 *         </td>
 *         <td>
 *             <tt>NSString</tt> containing the second enterprise activation credential: the access key.
 *         </td>
 *         <td><tt>NSString</tt></td>
 *     </tr>
 *     <tr>
 *         <td>
 *             <tt>ActivationParameterNOCAddress</tt>
 *         </td>
 *         <td>
 *             <tt>NSURL</tt> containing
 *                   a NOC server address, or an alias. The URL must always
 *                   include a scheme, such as <tt>https</tt>.
 *         </td>
 *         <td><tt>NSURL</tt></td>
 *     </tr>
 *     <tr>
 *         <td>
 *             <tt>ActivationParameterShowUserInterface</tt>
 *         </td>
 *         <td>
 *             <tt>Boolean</tt> value which if set to <tt>false</tt>, will prevent the BlackBerry Dynamics runtime activation UI from being shown during activation.</td>
 *         </td>
 *         <td><tt>NSNumber</tt></td>
 *     </tr>
 * </table>
 * \link GDiOS::programmaticAuthorize: \endlink
 */
typedef NSString *ActivationParameter;
extern ActivationParameter const ActivationParameterUserIdentifier;
extern ActivationParameter const ActivationParameterAccessKey;
extern ActivationParameter const ActivationParameterNOCAddress;
extern ActivationParameter const ActivationParameterShowUserInterface;

/** Initiate programmatic activation.
 *
 * Call this function to 
 * initiate programmatic activation.
 * Programmatic activation can only be utilized by applications that can obtain
 * credentials for enterprise activation on behalf of the end user. The
 * credentials are passed as parameters to this function. The credentials
 * must already have been generated, by an enterprise BlackBerry Dynamics
 * management console, prior to this function being called.
 *
 * Activation requires processing on the BlackBerry Dynamics Network Operation
 * Center (NOC). From an architectural point-of-view, there is a single NOC for
 * all BlackBerry Dynamics enterprises and users. For practical reasons,
 * however, there may actually be a number of NOC deployments. For example,
 * there may be separate production and development deployments. The application
 * can specify which NOC is to process programmatic activation, in the
 * <tt>nocAddress</tt> parameter to this function.
 *
 * Calling this function also initiates authorization processing, as if the
 * \reflink GDiOS::authorize: authorize (GDiOS) \endlink function had
 * been called.
 *
 * Only call this function after checking that the application is not
 * already activated, by
 * accessing the \reflink GDiOS::activationComplete activationComplete (GDiOS) \endlink property.
 *
 * @param activationParameters <tt>NSDictionary</tt>
 *         object containing configuration values. Use the
 *         <tt>ActivationParameter</tt> string constants as keys.
 */
- (void)programmaticAuthorize:(NSDictionary<ActivationParameter, id> *)activationParameters;

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_PROGRAMMATICAUTHORIZE __attribute((deprecated("Use -programmaticAuthorize:activationParameters instead.")))
#else
#   define DEPRECATE_PROGRAMMATICAUTHORIZE __attribute((deprecated))
#endif
/* \endcond */

/** Initiate programmatic activation (deprecated).
 *
 * \deprecated This function is deprecated and will be removed in a future
 * release. Use
 * \link GDiOS::programmaticAuthorize: \endlink
 * instead.
 *
 * Calling this function is equivalent to calling the
 * form with an additional URL parameter, see below, and specifying the URL of
 * the default production Network Operation Center.
 */
- (void)programmaticAuthorize:(NSString *)userID  withAccessKey:(NSString *)accessKey DEPRECATE_PROGRAMMATICAUTHORIZE;

/** Initiate programmatic activation by a specific Network Operation Center (deprecated).
 *
 * \deprecated This function is deprecated and will be removed in a future
 * release. Use
 * \link GDiOS::programmaticAuthorize: \endlink
 * instead.
 *
 * Call this function to 
 * initiate programmatic activation.
 * Programmatic activation can only be utilized by applications that can obtain
 * credentials for enterprise activation on behalf of the end user. The
 * credentials are passed as parameters to this function. The credentials
 * must already have been generated, by an enterprise BlackBerry Dynamics
 * management console, prior to this function being called.
 *
 * Activation requires processing on the BlackBerry Dynamics Network Operation
 * Center (NOC). From an architectural point-of-view, there is a single NOC for
 * all BlackBerry Dynamics enterprises and users. For practical reasons,
 * however, there may actually be a number of NOC deployments. For example,
 * there may be separate production and development deployments. The application
 * can specify which NOC is to process programmatic activation, in the
 * <tt>nocAddress</tt> parameter to this function.
 *
 * Calling this function also initiates authorization processing, as if the
 * \reflink GDiOS::authorize: authorize (GDiOS) \endlink function had
 * been called.
 *
 * Only call this function after checking that the application is not
 * already activated, by
 * accessing the \reflink GDiOS::activationComplete activationComplete (GDiOS) \endlink property.
 *
 * @param userID <tt>NSString</tt> containing the first enterprise activation
 *               credential: the user identifier. This is typically the
 *               enterprise email address of the end user.
 *               
 * @param accessKey <tt>NSString</tt> containing the second enterprise activation
 *                  credential: the access key.
 *                  
 * @param nocAddress <tt>NSURL</tt> containing
 *                   a NOC server address, or an alias. The URL must always
 *                   include a scheme, such as <tt>https</tt>.
 */
- (void)programmaticAuthorize:(NSString *)userID
                withAccessKey:(NSString *)accessKey
       networkOperationCenter:(NSURL *)nocAddress DEPRECATE_PROGRAMMATICAUTHORIZE;

#undef DEPRECATE_PROGRAMMATICAUTHORIZE

/** Application activation status.
 *
 * Read the value of this property, using the <tt>isActivated</tt> accessor, to
 * check whether the application has already been activated. It is necessary to
 * check whether the application has been activated before initiating
 * programmatic activation.
 *
 * This property has the value:
 * - <tt>YES</tt> if the application has been activated.
 * - <tt>NO</tt> otherwise.
 * .
 */
@property (nonatomic, assign, readonly, getter=isActivated) BOOL activationComplete;

/** Get application configuration and other settings from the enterprise.
 * 
 * This function returns a collection of application configuration
 * and other settings. The settings will have been made in the enterprise
 * management console, and retrieved by the BlackBerry Dynamics runtime.
 *
 * Retrieval of configuration settings may happen during authorization
 * processing, or whenever settings are changed. When changed settings have been
 * retrieved by the runtime, a <tt>GDAppEventRemoteSettingsUpdate</tt> event
 * will be dispatched to the application. See the \reflink GDAppEvent GDAppEvent \endlink
 * class reference for details, and see the notes under the Enterprise
 * Configuration Information heading, above.
 *
 * Note that retrieval isn't as such triggered by calling this function.
 *
 * @return <tt>NSDictionary</tt>
 *         object containing configuration values. Use the
 *         <tt>GDAppConfigKey</tt> constant strings as keys.
 *         
 * Any of the following configuration settings may be present.
 * <table>
 *     <tr>
 *         <th>Key Constant</th>
 *         <th>Setting</th>
 *         <th>Type</th>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyServers</tt></td>
 *     <td>
 *         Application server configuration.\n
 *         A number of servers can be configured for an application, in the
 *         management console.\n
 *         See the \link GDAppServer
 *         GDAppServer class reference\endlink for documentation of the details
 *         that are returned for each configured server.
 * </td><td>
 *         <tt>NSArray</tt> of
 *         <tt>GDAppServer</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyConfig</tt></td>
 *     <td>
 *         Application-specific configuration data.\n
 *         As well as the application server details, above, a free
 *         text can also be entered in the management console. Whatever was
 *         entered is passed through by the runtime and made available to the
 *         application code here.</td>
 *     <td><tt>NSString</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyPreventDataLeakageOut</tt></td>
 *     <td>
 *         Outbound Data Leakage policy indicator.\n
 *         1 means that enterprise security
 *         policies require that the end user must be prevented from taking any
 *         action that is classified as data loss or data leakage in the
 *         BlackBerry Dynamics Security Compliance Requirements document.\n
 *         0 means that the above policy isn't
 *         in effect, so the user is permitted to take those actions.</td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyPreventDataLeakageIn</tt></td>
 *     <td>
 *         Inbound Data Leakage policy indicator.\n
 *         1 means that enterprise security
 *         policies require that the end user must be prevented from copying
 *         into the application data that originates from elsewhere than another
 *         BlackBerry Dynamics application activated by the same end user on the
 *         same device or computer.\n
 *         0 means that the above policy isn't
 *         in effect, so the user is permitted to copy in data from any other
 *         application.</td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyPreventDictation</tt></td>
 *     <td>
 *         Dictation Prevention policy indicator.\n
 *         1 means that enterprise security
 *         policies require that the end user must be prevented from using
 *         dictation as a form of input.\n
 *         0 means that the above policy isn't
 *         in effect, so the user is permitted to use dictation.\n
 *         This setting is included for completeness. The policy is generally
 *         enforced by the BlackBerry Dynamics runtime without reference to the
 *         application code.</td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *
 *     </tr>  *     </tr> <tr><td>
 *         <tt>GDAppConfigKeyPreventScreenRecording</tt></td>
 *     <td>
 *         Screen Recording policy indicator.\n
 *         1 means that enterprise security
 *         policies require that the end user must be prevented from recording or sharing their screen.\n
 *         0 means that the above policy isn't
 *         in effect, so the user is permitted to record or share their screen.\n
 *         This setting is included for completeness. The policy is generally
 *         enforced by the BlackBerry Dynamics runtime without reference to the
 *         application code.</td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *    
 *     </tr> <tr><td>
 *         <tt>GDAppConfigKeyPreventCustomKeyboards</tt></td>
 *     <td>
 *         Custom Keyboard Prevention policy indicator.\n
 *         1 means that enterprise security policies require that the end
 *         user must be prevented from using custom keyboards.\n
 *         0 means that the above policy isn't in effect, so the user is
 *         permitted to use custom keyboards.\n
 *         This setting is included for completeness. The policy is generally
 *         enforced by the BlackBerry Dynamics runtime without reference to the
 *         application code.</td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *         
 *     </tr> <tr><td>
 *         <tt>GDAppConfigKeyCopyPasteOn</tt> (deprecated)</td>
 *     <td>
 *         This key is deprecated and will be removed in a future release. Use
 *         the <tt>GDAppConfigKeyPreventDataLeakageOut</tt> key instead.\n</td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *
 *     </tr> <tr><td>
 *         <tt>GDAppConfigKeyDetailedLogsOn</tt></td>
 *     <td>
 *         Logging level.\n
 *         0 means that the logging level is
 *         low, and only minimal logs should be written.\n
 *         1 means that the logging level is
 *         high, and detailed logs should be written. Detailed logs facilitate
 *         debugging of issues encountered at run time.\n
 *         The BlackBerry Dynamics runtime will automatically adjust its logging
 *         according to the configured setting. The setting is present in the
 *         programming interface so that the application can adjust its logging
 *         consistently with the runtime.</td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *
 *     </tr><tr>
 *         <td><tt>GDAppConfigKeyPreventUserDetailedLogs</tt></td>
 *     <td>
 *         User detailed logging block indicator.\n
 *         1 if the end user is blocked from
 *         switching on detailed logging, 0
 *         otherwise.\n
 *         Use this configuration key to determine whether to offer the end user
 *         an option to switch on detailed logging. Only offer the end user an
 *         option to switch on detailed logging if it isn't blocked by policy.\n
 *         See GDLogManager for
 *         how to switch on detailed logging from the application code.
 *     <td>
 *         <tt>NSNumber</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyUserId</tt></td>
 *     <td>
 *         Enterprise user identifier, typically email address.\n
 *         An enterprise identifier for the end user is one of the credentials
 *         required for activation of BlackBerry Dynamics application. This
 *         could be the email address entered in the activation user interface
 *         when the application was run for the first time, for example.\n
 *         The value obtained for this setting will initially be the enterprise
 *         email address, or other identifier, used for activation. If the
 *         enterprise email address of the end user changes after activation
 *         then the value of this setting will also change, next time it is
 *         obtained.</td>
 *     <td><tt>NSString</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyUserPrincipalName</tt></td>
 *     <td>
 *         User Principal Name.\n
 *         The User Principal Name (UPN) will have been retrieved from the
 *         enterprise Active Directory (AD) service by the enterprise management
 *         console, and then sent to the runtime, initially at activation time.
 *         This value will only be present if the current end user corresponds
 *         to an AD entry. The value may or may not be the same as the
 *         <tt>GDAppConfigKeyUserId</tt> value, depending on enterprise domain
 *         configuration.\n
 *         The value will be in
 *         <tt><em>username</em>\@<em>fully.qualified.domain.name</em></tt>
 *         format.\n
 *         If the UPN of the end user changes after activation then the value of
 *         this setting will also change, next time it is obtained.</td>
 *     <td><tt>NSString</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyInstanceIdentifier</tt></td>
 *     <td>
 *         Application instance identifier.\n
 *         The BlackBerry Dynamics runtime generates a unique identifier for the
 *         installed application when it is activated. This identifier doesn't
 *         change during upgrade of the application. It can be matched with a
 *         value on the management console, for tracking and analysis.\n
 *         This identifier will be different:
 *         - For the same application installed on different devices, even if
 *           installed for the same end user in the same deployment.
 *         - For different applications installed on the same device.
 *         - If the application is uninstalled and reinstalled.
 *         .
 *         The identifier will change value if the end user is migrated from one
 *         management console instance to another. See also the
 *         \link GDState::GDContainerMigrationPendingNotification
 *         GDContainerMigrationPendingNotification\endlink migration pending
 *         NSNotification name
 *         reference documentation.
 *         </td>
 *     <td><tt>NSString</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyCommunicationProtocols</tt></td>
 *     <td>
 *         Communication protocols allowed by the enterprise.\n
 *         A list of allowed and disallowed communication protocols can be set
 *         in the enterprise management console. The runtime will have retrieved
 *         the list, initially at activation time.\n
 *         The list is represented as
 *         an <tt>NSDictionary</tt>
 *          collection with the following keys:
 *         - \reflink GDiOS::GDProtocolsKeyTLSv1_0 GDProtocolsKeyTLSv1_0 (GDiOS) \endlink
 *         - \reflink GDiOS::GDProtocolsKeyTLSv1_1 GDProtocolsKeyTLSv1_1 (GDiOS) \endlink
 *         - \reflink GDiOS::GDProtocolsKeyTLSv1_2 GDProtocolsKeyTLSv1_2 (GDiOS) \endlink
 *         .
 *         The value in the collection for a particular key will be <tt>YES</tt> if
 *         the corresponding protocol is allowed, and <tt>NO</tt> otherwise.\n
 *         Some earlier versions of the management console software don't
 *         support this feature, and don't provide a list of allowed and
 *         disallowed protocols to the runtime. In that case, this element will
 *         be <tt>nil</tt> instead of a collection.</td>
 *     <td>
 *       <tt>NSDictionary</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyProtectedByPassword</tt></td>
 *     <td>
 *         BlackBerry Dynamics password enforcement.\n
 *         1 if a password or other interactive
 *         authentication is enforced by BlackBerry Dynamics,
 *         0 otherwise.\n
 *         See \reflink GDiOS::canAuthorizeAutonomously canAuthorizeAutonomously (GDiOS) \endlink for discussion.
 * </td>
 *     <td>
 *         <tt>NSNumber</tt></td>
 *
 *     </tr><tr><td>
 *         <tt>GDAppConfigKeyExtraInfo</tt></td>
 *     <td>
 *         Extra information sent by the management console.\n
 *         The management console can send extra information that isn't required
 *         for the operation of the BlackBerry Dynamics runtime but which might
 *         be of use to the application. For example, one or more additional
 *         identifiers for the end user could be included as extra
 *         information. The values will be encapsulated in a single JavaScript
 *         Object Notation (JSON) string.\n
 *         Some earlier versions of the management console software don't
 *         provide extra information. In that case, this element will be absent
 *         from the collection.</td>
 *     <td><tt>NSString</tt></td>
 *     
 *     </tr>
 * </table>

 * \par Data Leakage Policy Enforcement
 * Security policies other than the Data Leakage policy (DLP) mentioned in the
 * above are enforced by the BlackBerry Dynamics runtime, without reference to
 * the application. DLP must generally be enforced by the application, with some
 * exceptions.\n
 * If DLP is switched on, the BlackBerry Dynamics runtime will:<ul>
 * <li>
 *     Secure general cut-copy-paste operations by the user.</li>
 * <li>
 *     Secure data written to the general pasteboard
 *     by content-rendering <tt>UIKit</tt> components.</li>
 *   </ul>
 *  Secure cut-copy-paste  
 *  operations allow the user to copy and move data via the clipboard
 *  only:
 * - Within one BlackBerry Dynamics application.
 * - Between BlackBerry Dynamics applications, on the same device, that were activated
 *   for the same end user from the same enterprise management console.
 * - Between BlackBerry Dynamics applications, on multiple iOS and macOS devices
 *   conforming to Apple Continuity system requirements, that were activated
 *   for the same end user from the same enterprise management console.
 * .
 * Other aspects of data leakage must be enforced by the application.\n 
 * Note that pasteboards other than the general pasteboard, i.e.
 * programmatically created <tt>UIPasteboard</tt> instances, are never secured
 * by the BlackBerry Dynamics runtime.\n
 * @see <a
 href="https://support.apple.com/en-ca/HT204681#clipboard"
 target="_blank"> Universal Clipboard</a> on the apple.com support website. \n
 * \n \par Application Server Selection
 * The <tt>GDAppConfigKeyServers</tt> value will contain a list of the servers
 * that have been configured for the application in the enterprise management
 * console. In the case that more than one server has been configured, the
 * recommended selection algorithm is as follows:
 * -# For each priority value in the list, starting with the highest:
 * -# Select a server that has that priority, at random.
 * -# Attempt to connect to the server.
 * -# If connection succeeds, use that server.
 * -# If connection fails, try another server at the same priority, at random.
 * -# If there are no more untried servers at that priority, try the servers at
 *    the next lower priority.
 * .
 * 
 * @see \ref GC
 * @see <a
    href="https://developer.apple.com/reference/foundation/nsdictionary?language=objc"
    target="_blank"
    >NSDictionary class reference</a> in the Apple API Reference on the
    apple.com developer website.
 * @see The GD-Secure compliance document.
 */
- (GD_NSDictionary(NSString *, id) *)getApplicationConfig;

/** Get application-specific policy settings from the enterprise management
 *  console, as a collection.
 *  
 * 
 * This function returns a collection of application-specific policy
 * settings. The settings will have been made in the management console, and
 * retrieved by the BlackBerry Dynamics runtime.
 *
 * For more documentation of the feature and how application policies are
 * defined, see the \ref AppPolicies documentation.
 * 
 * Retrieval of policy settings may happen during authorization processing, or
 * whenever settings are changed. When changed settings have been retrieved by
 * the runtime, a <tt>GDAppEventPolicyUpdate</tt> event will be dispatched to
 * the application. See the \reflink GDAppEvent GDAppEvent \endlink class reference for
 * details, and see the notes under the Enterprise Configuration Information
 * heading, above.
 *
 * Note that retrieval of that latest policy from the management server is
 * not triggered by calling this function.
 * 
 * @return <tt>NSDictionary</tt>
 *         containing policy settings.
 *         
 * The keys will be the same as the settings identifiers in the policy
 * definition. The values will be the particular values that apply to the end
 * user. The value types are mapped from the definition as follows.
   <table
   ><tr
       ><th>Definition Type</th><th>Dictionary Value Type</th
 
   ></tr><tr><td>null</td><td><tt>nil</tt></td
 
   ></tr><tr><td
       >boolean</td><td><tt>NSNumber</tt></td
 
   ></tr><tr><td
       >double</td><td><tt>NSNumber</tt></td
 
   ></tr><tr><td
       >int</td><td><tt>NSNumber</tt></td
 
   ></tr><tr><td
       >object</td><td><tt
           >NSDictionary</tt></td
 
   ></tr><tr><td
       >array</td><td><tt>NSArray</tt></td
 
   ></tr><tr><td>string</td><td><tt>NSString</tt></td
 
  ></tr
  ></table>
 * @see \reflink GDiOS::getApplicationPolicyString getApplicationPolicyString (GDiOS) \endlink
 */
- (GD_NSDictionary(NSString *, id) *)getApplicationPolicy;

/** Get application-specific policy settings from the enterprise management
 *  console, as JSON.
 * 
 *  
 * This function returns application-specific policy settings in a
 * JavaScript Object Notation (JSON) string. The settings will have been made in
 * the management console, and retrieved by the BlackBerry Dynamics runtime.
 * 
 * For more documentation of the feature and how application policies are
 * defined, see the \ref AppPolicies documentation.
 *
 * Retrieval of policy settings may happen during authorization processing, or
 * whenever settings are changed. When changed settings have been retrieved by
 * the runtime, a <tt>GDAppEventPolicyUpdate</tt> event will be dispatched to
 * the application. See the \reflink GDAppEvent GDAppEvent \endlink class reference for
 * details, and see the notes under the Enterprise Configuration Information
 * heading, above.
 *
 * Note that retrieval isn't as such triggered by calling this function.
 * 
 * @return <tt>NSString</tt> containing policy settings in a JSON string.
 *         The string will evaluate to an object with an attribute for each
 *         application-specific setting. The names of the attributes will be the
 *         same as the settings identifiers in the policy definition. The
 *         attribute values will be the particular values that apply to the end
 *         user.
 * 
 * @see \reflink GDiOS::getApplicationPolicy getApplicationPolicy (GDiOS) \endlink
 */
- (NSString*)getApplicationPolicyString;

/** Get providers of shared services.
 * 
 * This function returns a list of all available providers of
 * shared services. The list contains both application-based and server-based
 * service providers.
 *
 * The returned list is based on the BlackBerry Dynamics entitlement
 * configuration in the enterprise management console and in the central
 * BlackBerry Dynamics Catalog. An entitlement can correspond to an application,
 * or can be a placeholder for something more abstract such as access to a
 * feature or resource. In any case, entitlements have unique identifiers and
 * versions.
 *
 * The list includes an entry for each entitlement version that meets all the
 * following conditions.
 * - The entitlement version is registered as the provider of one or more shared
 *   services, see notes on service provider registrations, below.
 * - The end user that activated the running application has been granted the
 *   entitlement version, i.e. they are entitled to the service provider.
 * - For entitlements that represent application-based service providers: the
 *   corresponding application has been installed and activated for the same end
 *   user, and on the same device or computer, as the current application.
 * .
 *
 * On devices running iOS version 9 or later, the list could include providers
 * of application-based services that were installed and activated as above, but
 * then uninstalled. An attempt to send a service request to an application that
 * is not currently installed results in a
 * <tt>GDServicesErrorApplicationNotFound</tt> error.
 *
 * <em>Service provider registrations</em> are stored within the BlackBerry
 * Dynamics infrastructure, on a server known as the Catalog. Registrations are
 * stored as relationships between entitlement versions and service identifiers.
 * The user interface for registration administration is either the enterprise
 * management console, or a website that can be reached via the application
 * developer portal. The same user interfaces are used for the creation and
 * administration of individual shared service definitions.
 * 
 * The runtime retrieves the information used to generate the list of service
 * providers from the Catalog, and from the enterprise management console.
 * Retrieval can take place when the application is authorized, or when another
 * application activates on the same device, or when the configuration is
 * changed in the enterprise management console. Note that retrieval isn't as
 * such triggered by calling this function.
 * 
 * For each entitlement version that satisfies the above conditions, this
 * function returns a \reflink GDServiceProvider GDServiceProvider \endlink object.
 *
 * The returned details for an application-based provider can be used to send a
 * service request to the service provider using BlackBerry Dynamics
 * AppKinetics. See the \reflink GDService GDService class reference\endlink for details of the programming
 * interface. See under Service Provider Requirements on that page for
 * information on service provider registration, which is a prerequisite for an
 * application to be on the list returned by this function.
 *
 * The returned details for a server-based provider can be used to establish
 * HTTP or TCP socket communications with a server instance.
 * 
 * @return <tt>NSArray</tt> of <tt>GDServiceProvider</tt
 *         > objects containing the list of available service providers. The
 *         order of objects in the list is unspecified.
 * 
 * A single service could have multiple providers. A single provider could
 * provide multiple services, and multiple versions of services.
 *
 * When changed details have been retrieved by the runtime, a
 * <tt>GDAppEventServicesUpdate</tt> event will be dispatched to the
 * application. See the \reflink GDAppEvent GDAppEvent \endlink class reference for details,
 * and see the notes under the Enterprise Configuration Information heading,
 * above.
 * 
 * <h4>Icon images</h4>
 * The objects returned by this function can include icon images for service
 * provider applications. Utilization of icon images is optional. If the
 * application does utilize icon images, then make a setting in the
 * application's Info.plist file* as follows.
 * - Key: <tt>GDFetchResources</tt>
 * - Type: <tt>Boolean</tt>
 * - Value: <tt>YES</tt>
 * .
 * (*In case there are multiple Info.plist files, check that the correct one has
 * been edited by opening the Info tab of the application target being built.
 * The setting just made should appear there.)
 *
 * If the setting is missing or the value is <tt>NO</tt>, this indicates that
 * the application doesn't utilize icon images.
 *
 * The data for icon images would be retrieved by the BlackBerry Dynamics
 * runtime, from a BlackBerry Dynamics Catalog service. BlackBerry may switch
 * off the icon image service at the Catalog from time to time, for operational
 * reasons. If the icon image service is switched off then all icon images in
 * the returned objects will be <tt>nil</tt>.
 */
- (GD_NSArray(GDServiceProvider *) *)getServiceProviders;

/** Discover providers of a specific shared service.
 *
 *  This function returns a list of the available providers of a
 * specified service.
 *
 * The returned list is based on the BlackBerry Dynamics entitlement
 * configuration in the enterprise management console and in the central
 * BlackBerry Dynamics Catalog. The list includes an entry for each entitlement
 * version that meets all the following conditions.
 * - The entitlement version would be on the list of service providers returned
 *   by the \reflink GDiOS::getServiceProviders getServiceProviders (GDiOS) \endlink function, see above.
 * - The entitlement version is registered as a provider of the specified
 *   service.
 * - The entitlement version is registered as a provider of the specified
 *   service type, either server-based or application-based.
 * - The entitlement version is registered as a provider of the service version,
 *   if specified.
 * .
 *
 * @param serviceId <tt>NSString</tt> specifying the identifier of the required
 *                  service.
 *
 * @param version <tt>NSString</tt> specifying the required version of the service, or
 *                <tt>nil</tt> to leave unspecified.
 *
 * @param serviceType <tt>GDServiceType</tt> value specifying the required type
 *                    of service.
 *
 * @return <tt>NSArray</tt> of <tt>GDServiceProvider</tt
 *         > objects containing the list of available service providers. The
 *         order of objects in the list is unspecified.
 *
 * See also the note on Icon images in the <tt>getServiceProviders</tt>
 * function documentation, above.
 */
- (GD_NSArray(GDServiceProvider *) *)getServiceProvidersFor:(NSString*)serviceId andVersion:(GD_NSNULLABLE NSString*)version andServiceType:(GDServiceType)serviceType;

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_GETSERVICEPROVIDERSFOR __attribute((deprecated("Use getServiceProvidersFor:andVersion:andServiceType:")))
#else
#   define DEPRECATE_GETSERVICEPROVIDERSFOR __attribute((deprecated))
#endif
/* \endcond */

/** Discover providers of a specific shared service (deprecated).
 * 
 * \deprecated This function is deprecated and will be removed in a future
 * release. Use
 * \link GDiOS::getServiceProvidersFor:andVersion:andServiceType: \endlink
 * instead.
 *
 * This function returns a list of the available providers of a
 * specified service.
 *
 * The returned list is based on the BlackBerry Dynamics entitlement
 * configuration in the enterprise management console and in the central
 * BlackBerry Dynamics Catalog. The list includes an entry for each entitlement
 * version that meets all the following conditions.
 * - The entitlement version would be on the list of service providers returned
 *   by the \reflink GDiOS::getServiceProviders getServiceProviders (GDiOS) \endlink function, see above.
 * - The entitlement version is registered as a provider of the specified
 *   service.
 * - The entitlement version is registered as a provider of the specified
 *   service type, either server-based or application-based.
 * - The entitlement version is registered as a provider of the service version,
 *   if specified.
 * .
 * 
 * @param serviceId <tt>NSString</tt> specifying the identifier of the required
 *                  service.
 *
 * @param version <tt>NSString</tt> specifying the required version of the service, or
 *                <tt>nil</tt> to leave unspecified.
 *
 * @param serviceProviderType <tt>GDServiceProviderType</tt> value specifying
 *                            the required type of service.
 * 
 * @return <tt>NSArray</tt> of <tt>GDServiceProvider</tt
 *         > objects containing the list of available service providers. The
 *         order of objects in the list is unspecified.
 *
 * See also the note on Icon images in the <tt>getServiceProviders</tt>
 * function documentation, above.
 */
- (GD_NSArray(GDServiceProvider *) *)getServiceProvidersFor:(NSString*)serviceId andVersion:(GD_NSNULLABLE NSString*)version andType:(GDServiceProviderType)serviceProviderType DEPRECATE_GETSERVICEPROVIDERSFOR;

#undef DEPRECATE_GETSERVICEPROVIDERSFOR

/** Type for getEntitlementsFor block parameter.
 * 
 * Pass a code block of this type as the <tt>block</tt> parameter to the
 * \link GDiOS::getEntitlementVersionsFor:callbackBlock:
 * getEntitlementVersionsFor: \endlink function.
 *
 * The block receives the following parameters.
 * @param entitlementVersions <tt>NSArray</tt> of \ref GDVersion objects
 *                            representing the versions of the entitlement to
 *                            which the end user is entitled if the original
 *                            call succeeded, or <tt>nil</tt> otherwise.
 * 
 * @param error <tt>NSError</tt> containing a description of the error condition
 *                               if the original call failed, or <tt>nil</tt>
 *                               otherwise.
 */
typedef void (^GDGetEntitlementVersionsForBlock) (GD_NSArray(GDVersion *)* GD_NSNULLABLE_POINTER entitlementVersions, NSError* error);

/** Check whether the end user has a particular entitlement.
 * 
 * Call this function to check whether the current end user has a
 * specific BlackBerry Dynamics entitlement. The return value is a list of
 * entitlement versions, which might be empty.
 *
 * This function can be used to check for entitlement to:
 * - A specific BlackBerry Dynamics mobile or desktop application, identified by
 *   a BlackBerry Dynamics entitlement identifier.
 * - A more abstract entitlement, such as a feature, also identified by an
 *   entitlement identifier.
 * .
 * Note that there is a single namespace and format for entitlement identifiers,
 * whether used for applications or for more abstract entitlements. All
 * entitlements have versions, as well as identifiers. The syntax for
 * entitlement identifiers and versions is detailed above, under the headings
 * Entitlement Identifier Format and Entitlement Version Format. The syntax
 * applies to all types of entitlement.
 *
 * Specify the entitlement to be checked by passing its identifier as a
 * parameter. The return value will be a list:
 * - If the end user doesn't have the specified entitlement, the list will have
 *   zero elements.
 * - Otherwise, the list will have one element for each version to which the end
 *   user is entitled. Each element will be a
 *   \link GDVersion \endlink
 *   object.
 * .
 *
 * This function is asynchronous.
 * 
 * The result list is returned by execution of a code block. Specify the code
 * block as a parameter to this function. The result list will be passed as
 * a block parameter to the execution. If an error occurs and the entitlement
 * cannot be checked, <tt>nil</tt> will be passed instead of the list, and an
 * <tt>NSError</tt> object will be passed as a second block parameter.
 *
 * Calling this function can result in data communication with the
 * BlackBerry Dynamics infrastructure.
 * 
 * @param identifier <tt>NSString</tt> containing the entitlement identifier.
 * 
 * @param block Block to execute when the lookup completes. The block receives
 *              two parameters:\n
 *              <tt>NSArray</tt> of \ref GDVersion objects representing the
 *              versions to which the end user is entitled if the original call
 *              succeeds, or <tt>nil</tt> otherwise.\n
 *              <tt>NSError</tt> object containing a numeric code for the error
 *              condition if the original call fails, or <tt>nil</tt> otherwise.\n
 *              The numeric code will be in one of the following ranges,
 *              depending on the type of error condition encountered:
 *              - 400 to 599: One or more servers involved in the check
 *                            encountered an error and the check could not be
 *                            completed.
 *              - -1 to -50: A general error occurred.
 *              .
 * 
 * \par
 *
 * The entitlements of the end user can change, for example if the user's group
 * membership is changed at the enterprise management console. The BlackBerry
 * Dynamics runtime is notified of these changes by the BlackBerry Dynamics
 * infrastructure, and dispatches a <tt>GDAppEventEntitlementsUpdate</tt> event
 * to the application. See the \reflink GDAppEvent GDAppEvent \endlink class reference for
 * details, and see the notes under the Enterprise Configuration Information
 * heading, above.
 */
- (void)getEntitlementVersionsFor:(NSString*)identifier
                    callbackBlock:(GDGetEntitlementVersionsForBlock)block;

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_GETWINDOW __attribute((deprecated("Use the window property from the main app delegate instance.")))
#else
#   define DEPRECATE_GETWINDOW __attribute((deprecated))
#endif
/* \endcond */

/** Get the <tt>UIWindow</tt> for the application (deprecated).
 * \deprecated This function is deprecated and will be removed in a future
 * release. Use the <tt>window</tt> property from the main app delegate instance.
 *
 * This function returns a reference to the <tt>UIWindow</tt> that contains
 * the core logic of the BlackBerry Dynamics runtime. Always use this function
 * instead of creating a new <tt>UIWindow</tt> in the application.
 *
 * The BlackBerry Dynamics runtime creates its own <tt>UIWindow</tt> in order to
 * show its user interface elements, and to track for user inactivity. The
 * runtime doesn't add any persistent subviews, so the application is free to
 * add and remove its own subviews on the BlackBerry Dynamics <tt>UIWindow</tt>.
 * For example, after authorization, the application could call
 * <tt>setRootViewController</tt> to add its own <tt>UIViewController</tt> or
 * <tt>UINavigationController</tt>.
 *
 * The runtime calls <tt>makeKeyAndVisible</tt> on its <tt>UIWindow</tt> during
 * authorization processing, so the application needn't do this. The application
 * mustn't make a different <tt>UIWindow</tt> the key window. The application
 * also mustn't release the runtime's <tt>UIWindow</tt> object.
 *
 * @return Reference to the BlackBerry Dynamics <tt>UIWindow</tt>, which must be
 *         used as the application's key window.
 *
 * @see <a
 *     HREF="https://developer.apple.com/reference/uikit/uiwindow?language=objc"
 *     target="_blank"
 * >UIWindow class reference</a> in the Apple API Reference on the apple.com
 * developer website.
 */
- (UIWindow*)getWindow DEPRECATE_GETWINDOW;

#undef DEPRECATE_GETWINDOW

/** Get the BlackBerry Dynamics runtime library version.
 * @return <tt>NSString</tt> containing the BlackBerry Dynamics runtime library
 *         version in
 *         <em>major</em><tt>.</tt><em>minor</em><tt>.</tt><em>build</em>
 *         format.
 */
- (NSString*)getVersion;

/** Open the BlackBerry Dynamics preferences user interface.
 * 
 * Call this function to show the BlackBerry Dynamics preferences user
 * interface. This is the interface in which the end user sets any options that
 * are applied by the runtime directly, without reference to the application.
 * This includes, for example, changing their unlock password.
 * 
 * This function enables a BlackBerry Dynamics user interface element to be
 * included in the application's own user interface.
 *
 * @param baseViewController Reference to the navigation controller within which
 *        the BlackBerry Dynamics user interface element is to open as a view
 *        controller.\n
 *        Pass a null pointer to open the the user interface as a modal view
 *        controller, for example when no navigation controller is available.
 *
 * @return <tt>YES</tt> if the user interface element opened OK.
 * @return <tt>NO</tt> if the user interface element was already open, or if 
 *                   authorization is delegated to another application.
 */
- (BOOL)showPreferenceUI:(GD_NSNULLABLE UIViewController*)baseViewController;

/** Set autorotation for the BlackBerry Dynamics user interface.
 * 
 * Call this function to set the supported presentation orientations for
 * screens in the BlackBerry Dynamics user interface. The same orientations
 * should be set here as for the application's own user interface, if these are
 * different to the BlackBerry Dynamics default, see below.
 *
 * The BlackBerry Dynamics user interface consists of a small number of screens,
 * including the Enterprise activation screen and the Password entry screen, see
 * under <tt>authorize</tt>, above. Some of these screens will generally appear
 * before the application's own user interface has been opened.
 *
 * The iOS operating system recognizes four device orientations. Each of these
 * orientations may be set as supported or unsupported for BlackBerry Dynamics.
 * The BlackBerry Dynamics user interface will auto-rotate so as to be visually
 * the "right way up" when the device is placed in a supported presentation
 * orientation. When the device is placed in an unsupported orientation, the
 * BlackBerry Dynamics user interface doesn't auto-rotate.
 *
 * The BlackBerry Dynamics default orientation support settings depend on the
 * type of device in use:
 * - For iPhone devices, <tt>UIInterfaceOrientationPortrait</tt> and
 *   <tt>UIInterfaceOrientationPortraitUpsideDown</tt> are supported and other
 *   orientations are unsupported.
 * - For iPad devices, all orientations are supported.
 *
 * @param portrait Sets support for <tt>UIInterfaceOrientationPortrait</tt>
 *                 presentation orientation. <tt>YES</tt> for supported,
 *                 <tt>NO</tt> for unsupported.
 * 
 * @param portraitUpsideDown Sets support for
 *                           <tt>UIInterfaceOrientationPortraitUpsideDown</tt>
 *                           presentation orientation.
 * 
 * @param landscapeRight Sets support for
 *                       <tt>UIInterfaceOrientationLandscapeRight</tt>
 *                       presentation orientation.
 * 
 * @param landscapeLeft Sets support for
 *                      <tt>UIInterfaceOrientationLandscapeLeft</tt>
 *                      presentation orientation.
 *
 * @return <tt>YES</tt> if supported presentation orientations were set as
 *                      specified.
 * @return <tt>NO</tt> if supported presentation orientations were not set. Note
 *                     that this will be returned if all parameters were
 *                     <tt>NO</tt>, which would specify no supported
 *                     presentation orientations.
 *
 * @see <a
 *     HREF="https://developer.apple.com/reference/uikit/uiinterfaceorientation?language=objc"
 *     target="_blank"
 * >UIInterfaceOrientation documentation</a> in the Apple API Reference on the apple.com
 * developer website, for definitions of iOS device orientations.
 */
- (BOOL)setUIAutoRotationForPortrait:(BOOL) portrait
               andPortraitUpsideDown:(BOOL) portraitUpsideDown
                   andLandscapeRight:(BOOL) landscapeRight
                    andLandscapeLeft:(BOOL)landscapeLeft;

/** Configure the visual appearance of the BlackBerry Dynamics user interface.
 * 
 * 
 * Call this function to configure the visual appearance of the built-in
 * BlackBerry Dynamics user interface. The following aspects can be configured:
 * - Logo image.
 * - Brand color, used for the key line and interactive elements.
 * .
 * This function can be called prior to <tt>authorize</tt>, in order to
 * configure the user interface as displayed during authorization processing.
 *
 * @param imagePath <tt>NSString</tt> containing the path of the image to show as the
 *                  logo. The image must be in PNG format.\n
 *                  The maximum supported image size is 528 by 140 pixels. If
 *                  the specified image is larger than the maximum, the image
 *                  will be adjusted using
 *                  <tt>UIViewContentModeScaleAspectFit</tt> mode.
 * @param bundle <tt>NSBundle</tt> for the resource bundle that contains the
 *               replacement logo image, or <tt>nil</tt> to specify
 *               <tt>mainBundle</tt>.
 * 
 * @param color <tt>UIColor</tt> for the brand color, or
 *              <tt>nil</tt> to select the default.
 *
 * @see \reflink GDiOS::authorize: authorize (GDiOS) \endlink for details of which BlackBerry Dynamics user
 * interface elements may be shown during authorization processing.
 * @see <a
      href="https://developer.apple.com/reference/uikit/uiviewcontentmode?language=objc"
      target="_blank"
  >UIViewContentMode documentation</a> in the Apple API Reference on the
  apple.com developer website, for definitions of image adjustment modes
 */
- (void)configureUIWithLogo:(NSString*)imagePath
                     bundle:(GD_NSNULLABLE NSBundle*)bundle
                      color:(GD_NSNULLABLE UIColor*)color;

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_CONFIG_UI_WITH_BLOCK_MESSAGE __attribute__((deprecated("No longer supported")))
#   define DEPRECATE_CONFIG_UI_WITH_WIPE_MESSAGE __attribute__((deprecated("No longer supported")))
#else
#   define DEPRECATE_CONFIG_UI_WITH_BLOCK_MESSAGE __attribute__((deprecated))
#   define DEPRECATE_CONFIG_UI_WITH_WIPE_MESSAGE __attribute__((deprecated))
#endif
/* \endcond */

/** Customize the BlackBerry Dynamics blocked screen (deprecated).
 * 
 * @deprecated This function is deprecated and will be
 * removed in a future release.
 *
 * Call this function to configure the BlackBerry Dynamics blocked screen.
 * The blocked screen can be configured to display a custom message instead of
 * the default message that is built in to the BlackBerry Dynamics runtime user
 * interface.
 *
 * The message will be shown in the following case:
 * - If a remote container management command to block the user interface has
 *   been received.
 * .
 * This function can be called prior to the completion of authorization
 * processing.
 *
 * @param message <tt>NSString</tt> containing the custom message text, or <tt>nil</tt> to
 *                select the default.
 */
- (void)configureUIWithBlockedMessage:(GD_NSNULLABLE NSString*)message DEPRECATE_CONFIG_UI_WITH_BLOCK_MESSAGE;

/** Customize the BlackBerry Dynamics wiped screen (deprecated).
 * 
 * @deprecated This function is deprecated and will be
 * removed in a future release.
 *
 * Call this function to configure the BlackBerry Dynamics wiped screen.
 * The wiped screen can be configured to display a custom message instead of the
 * default message that is built in to the BlackBerry Dynamics runtime user
 * interface.
 * 
 * The message will be shown in the following case:
 * - If a remote container management command to wipe the application has
 *   been received.
 * .
 * 
 * This function can be called prior to the completion of authorization
 * processing.
 *
 * @param message <tt>NSString</tt> containing the custom message text, or <tt>nil</tt> to
 *                select the default.
 */
- (void)configureUIWithWipedMessage:(GD_NSNULLABLE NSString*)message DEPRECATE_CONFIG_UI_WITH_WIPE_MESSAGE;

/** Delegated event-handling.
 * 
 * When authorization processing completes, or a BlackBerry Dynamics runtime
 * object event occurs, an event is generated by the runtime, and passed to a
 * callback function in the application code.
 *
 * Set this property to an instance of a class in the application that contains
 * the code for the required callback function, i.e. a class that implements
 * the GDiOSDelegate protocol.
 *
 * @see \ref GDState for an alternative mechanism.
 */
@property (GD_NSNULLABLE_PROP nonatomic, weak) id<GDiOSDelegate> delegate;

/* \cond DOXYGEN_IGNORE */
#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_LAUNCHOPTIONS __attribute__((deprecated("No longer required.")))
#else
#   define DEPRECATE_LAUNCHOPTIONS __attribute__((deprecated))
#endif
/* \endcond */

/** Application launch options (deprecated).
 *
 * \deprecated This property is deprecated and will be removed in a future release.
 *
 * Access this property to obtain the options with which the application was
 * launched. The property is a reference to an object with the same semantics as
 * the <tt>launchOptions</tt> parameter to the
 * <tt>application:didFinishLaunchingWithOptions:</tt> function in the
 * <tt>UIApplicationDelegate</tt> protocol.
 *
 * There is a known issue in the BlackBerry Dynamics SDK for iOS that this
 * property isn't always populated.
 * 
 * @see <a
 *     HREF="https://developer.apple.com/reference/uikit/uiapplicationdelegate/1622921-application?language=objc"
 *     target="_blank"
 * >application:didFinishLaunchingWithOptions: documentation</a> in the Apple
 * API Reference on the apple.com developer website, for details.
 */
@property (GD_NSNULLABLE_PROP nonatomic, strong) GD_NSDictionary(NSObject *, id)* launchOptions DEPRECATE_LAUNCHOPTIONS;

/** Whether it is possible to authorize autonomously.
 *
 * Access this property to check
 * whether autonomous authorization is possible. Autonomous authorization won't
 * be possible if the application is protected by a BlackBerry Dynamics
 * password. This means a password or other interactive authentication that is
 * enforced by the BlackBerry Dynamics runtime.
 *
 * Interactive authentication will be enforced by BlackBerry Dynamics unless a
 * <em>no-password</em> policy applies to the current end user. A no-password
 * policy would typically be put in place only in the case that interactive
 * authentication is enforced by some other means. For example, enterprise
 * mobile device management (MDM) can be used to force users to set a device
 * password or lock pattern. In that case, the enterprise might choose to set a
 * no-password policy for BlackBerry Dynamics, so that end users don't have to
 * enter two passwords.
 *
 * If interactive authentication isn't enforced by BlackBerry Dynamics, then it
 * is possible for authorization processing to complete autonomously, because
 * interaction with the user isn't required.
 *
 * The intended use of this property is to check
 * whether it is possible for authorization processing to complete autonomously,
 * before calling the
 * \reflink GDiOS::authorizeAutonomously: authorizeAutonomously: (GDiOS) \endlink function.
 * 
 * Note that it won't be possible to authorize autonomously if enterprise
 * policy specifies that the application delegates authentication to another
 * application. This would only happen if the current end user somehow had both
 * policies in place: authentication delegation, and no-password. This
 * combination isn't recommended. There would seem to be no point in delegating
 * authentication to another application if that other application is going to
 * authenticate the end user without interaction.
 *
 * This property can be accessed before
 * authorization processing is complete.
 * 
 * This property has the value:
 * - <tt>YES</tt> if autonomous authorization is possible.
 * - <tt>NO</tt> if a password is enforced by BlackBerry Dynamics, or if
 *             autonomous authorization isn't possible for some other reason.
 */
@property (nonatomic, readonly) BOOL canAuthorizeAutonomously;

/** Lock the application permanently.
 * 
 * Call this function to lock the application permanently, as
 * though a remote lock-out had been received from the enterprise management
 * console. The application data will become inaccessible but won't be erased.
 *
 * A remote lock-out is, in effect, a withdrawal of the end user's authorization
 * to utilise the application. The user cannot unlock the application in the
 * normal way, for example they cannot enter their BlackBerry Dynamics password,
 * if a remote lock-out is in place. Instead, a special unlock code must be
 * obtained from the management console, and entered in the remote unlock
 * BlackBerry Dynamics user interface.
 *
 * @see \reflink GDAppEvent GDAppEvent \endlink class reference, which mentions the
 * <tt>GDErrorRemoteLockout</tt> result code. An event with that result code
 * would be dispatched if a remote lock-out command was received from the
 * management console.
 */
- (BOOL)executeRemoteLock;

/** Blocks application locally with defined blockId.
 *
 * Call this function to block use of the application, as though a remote block
 * had been received from the enterprise management console. Include a title and description
 * to inform the user what threat or action has triggered the block. If several blocks have
 * been applied then it is the most recent block which will be displayed
 * to the user in the block screen.
 
 * While blocked, the application can utilize the principal Dynamics APIs like secure
 * storage and secure communications.
 
 * Blocks are persisted which means that after an application is restarted and the user
 * is authenticated the block screen will be displayed. In addition a \reflink GDAppEvent GDAppEvent \endlink
 * with the result code <tt>GDErrorBlocked</tt> would be dispatched. This event would be received
 * for blocks applied locally with this function and those applied from the management console.
 *
 * The application should persist in secure storage the blockId called in this function. When
 * a <tt>GDErrorBlocked</tt> event is received the application should check if any blocks are
 * applied locally and determine if the block can now be removed.
 *
 * @param blockId <tt>NSString</tt> specifying the block identifier. This identifier is required to
 * subsequently remove the block.
 * @param title <tt>NSString</tt> containing the title shown on the block screen.
 * @param message <tt>NSString</tt> containing the detailed message to display on the block screen.
 */
- (void)executeBlock:(NSString *)blockId withTitle:(NSString *)title withMessage:(NSString *)message;

/** Unblocks application locally with defined blockId.
 *
 * Call this function to remove a block screen with associated blockId.
 * Other local or remote blocks will still leave the application blocked. Blocks applied
 * by the management console cannot be unblocked using this function.
 *
 * @param blockId <tt>NSString</tt> specifying the block identifier.
 */
- (void)executeUnblock:(NSString *)blockId;


/** Executes migration of the application to a different management console.
 * @return <tt>YES</tt> if destinationTenantID is provided
 * @return <tt>NO</tt> if destinationTenantID is empty
 *
 * Call this function to execute a migration of the application to the specified 
 * management console. This should only be called by an application that has been informed by the 
 * management console that a migration request is pending. If a migration request is not pending 
 * calling this method may result in the container being wiped.
 *
 * @param destinationTenantID <tt>NSString</tt> specifying the tenant identifier of the UEM management console.
 */
-(BOOL)executePendingConsoleMigration:(NSString *)destinationTenantID;

/** Observable representation of authorization state and user interface state.
 *
 * Access this property to check or observe the state of the BlackBerry Dynamics
 * runtime.
 *
 * A number of aspects of run-time state are represented in this property,
 * including authorization state and user interface state. The representation is
 * compatible with key-value observing (KVO). See the \link GDState \endlink
 * class reference for details.
 * 
 * @return GDState instance representing the current state.
 */
@property (nonatomic, strong, readonly) GDState *state;

@end

GD_NS_ASSUME_NONNULL_END

#endif /* __GD_IOS_H__ */
