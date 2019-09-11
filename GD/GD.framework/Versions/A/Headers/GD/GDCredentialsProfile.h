/*
 * (c) 2018 BlackBerry Limited. All rights reserved.
 */

#ifndef GD_CREDENTIALS_PROFILE_H
#define GD_CREDENTIALS_PROFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GD_C_API
#   if !defined(_WIN32)
#       define GD_C_API __attribute__((visibility("default")))
#   else
#       define GD_C_API
#   endif
#endif
    
#include "GDCommon.h"

struct GDError;

/** User Credential Profile states.
 *
 * Values in this enumeration represent the possible states of a User
 * Credentials Profile (UCP). The <tt>state</tt> element in a
 * \ref GDCredentialsProfile structure always takes one of these values,
 * including the structure in a \ref GDCredentialsProfileEvent dispatched to the
 * application.
 * 
 * \ingroup clientcertificateimport
 */
typedef enum {
    /** Import of credentials is required.
     * 
     * This state can arise, for example, because:
     * - The profile has recently been assigned to the user's account and
     *   credentials haven't yet been imported for it.
     * - The credentials that were imported for the profile have expired.
     * .
     * The application should initiate import of credentials for the profile.
     *
     * @see GDCredential_import
     */
    GDCredentialsProfileStateImportDue,
    
    /** Another application has requested credentials.
     *
     * This state arises when another application is requesting credentials, but
     * they aren't yet available in the BlackBerry Dynamics secure store of this
     * application. This could be for one of the following reasons, for example.
     * - The user didn't complete the import of credentials at activation time.
     * - The profile has recently been assigned to the user's account.
     * .
     * 
     * The user interface will have flipped to this application, which should
     * initiate import of credentials for the profile straight away.
     * 
     * @see GDCredential_import
     */
    GDCredentialsProfileStateImportNow,
    
    /** All credentials have been imported. The application needn't take any
     * action in this state.
     */
    GDCredentialsProfileStateImported,
    
    /** The profile has been modified since credentials were imported. For
     *  example, the display name of the profile has been changed. If the
     *  application displays the details of the profile to the user, then the
     *  display should be updated.
     *
     * @see GDCredentialsProfile
     */
    GDCredentialsProfileStateModified,
              
    /** Credentials will soon expire. The application should initiate renewal of
     *  credentials for the profile when it is convenient to do so.
     *
     * If renewal isn't initiated, the credentials will expire and the profile
     * will become due for import. See the GDCredentialsProfileStateImportDue
     * state, above.
     */
    GDCredentialsProfileStateRenewalDue,
    
    /** Profile removed.
     *
     * This state arises if the profile has been deleted or detached from the
     * end user in the management console. If the application displays the
     * details of the profile to the user, then the display should be updated.
     *
     * @see GDCredentialsProfile
     */
    GDCredentialsProfileStateDeleted
} GD_C_API GDCredentialsProfileState;

/** User Credentials Profile.
 *
 * This structure is used to represent a User Credentials Profile (UCP) from the
 * enterprise BlackBerry Dynamics management console.
 *
 * @see clientcertificateimport
 * 
 * \ingroup clientcertificateimport
 */
struct GD_C_API GDCredentialsProfile {
    /** Identifier.
     *
     * Null-terminated string containing the identifier of the profile.
     *
     * The value of this element:
     * - Can be passed as the <tt>profileId</tt> parameter in a call to the
     *   GDCredential_list() function.
     * - Can be compared with the <tt>profileId</tt> value written by the
     *   GDCredential_import() function.
     * .
     */
    char* id;

    /** Current or latest state.
     * 
     * The current or latest state of the UCP, see
     * \ref GDCredentialsProfileState for descriptions.
     */
    GDCredentialsProfileState state;
    
    /** Profile type (reserved for future use).
     * 
     * NULL-terminated string containing an identifier of the profile type.
     *
     * If "localCertProvider", then the system will decide which profile will
     * manage the credential based on certificate properties configured in the
     * device-based UCP.
     *
     * If "localCertProvider2", then the app must specify the profile identifier
     * when importing the credential.
     */
    char* type;
    
    /** Display name.
     *
     * Null-terminated string containing a display name of the profile. The
     * display name is suitable for use in the application user interface.
     */
    char* name;
    
    /** Required flag (reserved for future use).
     *
     * In this version of BlackBerry Dynamics, this element always has the
     * value: <tt>true</tt>.
     */
    bool required;
    
    /** Additional settings from the credential provider.
     *
     * Additional settings can be attached to a UCP by the credential provider.
     * Attachment takes place at the BlackBerry Dynamics enterprise management
     * console. The settings are conveyed by BlackBerry Dynamics, but are
     * otherwise opaque to the infrastructure and can be in any format. For
     * example, the settings could be a JavaScript Object Notation (JSON)
     * string.
     * 
     * If no additional data has been attached to the UCP, this element will be
     * <tt>null</tt>.
     */
    char* providerSettings;
};

/** User Credentials Profile event.
 *
 * An instance of this structure is dispatched to the application profile
 * event callback when a User Credentials Profile (UCP) changes state.
 * 
 * \ingroup clientcertificateimport
 */
struct GD_C_API GDCredentialsProfileEvent {
    /** Profile that has changed state.
     * 
     * Reference to a \ref GDCredentialsProfile structure for the UCP whose
     * state changed. The latest state can be read from the <tt>state</tt>
     * element.
     */
    struct GDCredentialsProfile* profile;
};

/** \ingroup clientcertificateimport
 * 
 * \{
 */

/** Type for User Credentials Profile callback implementation.
 *
 * Pass a pointer to a function of this type as the <tt>cb</tt> parameter to the
 * \ref GDCredentialsProfile_register function.
 *
 * The callback will be invoked when a User Credentials Profile (UCP) changes
 * state. See below for the invocation parameters.
 *
 * If multiple UCP state changes occur, the callback is invoked sequentially for
 * each. Invocation can take place on any thread except the user interface
 * thread. The callback should return immediately. If handling of a state change
 * requires user interaction or lengthy processing, that should be scheduled. It
 * shouldn't be executed synchronously in the callback itself.
 * 
 * @param event GDCredentialsProfileEvent structure representing the UCP and the
 *              state change.\n
 *              The data in the structure is only valid within the scope of this
 *              callback invocation. The implementation must copy any data that
 *              will be used outside the scope of the callback.
 * @param appData <tt>void *</tt> pointer to the <tt>appData</tt> that was
 *                passed in the original \ref GDCredentialsProfile_register
 *                call.
 */
typedef void (*GDCredentialsProfileEventCb)(const struct GDCredentialsProfileEvent event, void* appData);
    
/** Register for notification of User Credentials Profile state changes.
 *
 * Call this function to register a callback for notification of User
 * Credentials Profile (UCP) state changes. If the application makes use of the
 * credential importing feature, then a callback should be registered
 * immediately after BlackBerry Dynamics authorization processing has completed.
 *
 * The application will be notified when any UCP changes state. See the
 * \ref GDCredentialsProfileState reference documentation for descriptions of
 * the different UCP states. Each UCP state change is notified exactly once.
 *
 * UCP state changes can occur at any time that the application is running,
 * including prior to registration of the callback. In the case that a backlog
 * of state changes has accumulated prior to registration of a callback, the
 * last state change for each UCP will be notified at the point of registration.
 *
 * Additional application data can be passed to this function. This data will
 * then be conveyed to the callback invocation, but is otherwise opaque to the
 * BlackBerry Dynamics runtime.
 * 
 * @param cb GDCredentialsProfileEventCb implementation for the callback.
 * @param appData <tt>void*</tt> pointer to the application data for the callback.
 */
GD_C_API void GDCredentialsProfile_register(GDCredentialsProfileEventCb cb, void* appData);

/** List User Credentials Profiles.
 *
 * This function returns a list of User Credentials Profiles that are assigned
 * to the current end user. Assignment will have been made in the enterprise
 * BlackBerry Dynamics management console.  The list can be empty, if there is
 * no User Credentials Profile (UCP) assigned to the end user.
 * 
 * This function returns the list, or an error code, by overwriting a number of
 * memory locations supplied as parameters, see below.
 * 
 * Note the following about the returned list.
 * - The list will be returned in a buffer containing a sequence of
 *   \link GDCredentialsProfile\endlink structures, each corresponding to a UCP,
 *   if any profiles are available.
 * - The buffer containing the list must be released by the application code, by
 *   calling the \ref GDCredentialsProfile_free function, below.
 * - If no profiles are available, <tt>NULL</tt> will be returned instead of
 *   a buffer.
 * .
 * 
 * This function can fail, in which case it returns <tt>false</tt> and writes a
 * \link GDError \endlink structure into the location supplied in the
 * <tt>error</tt> parameter, see below. The <tt>code</tt> value of the structure
 * will be set according to the error condition, as follows.
 * - \ref GDErrorInvalidArgument if any input parameter has a wrong value, for
 *   example if <tt>profiles</tt> is <tt>NULL</tt>.
 * - \ref GDErrorOutOfMemory if it wasn't possible to allocate memory for the
 *   returned list buffer, see above.
 * - \ref GDErrorGeneral if it isn't possible to provide a list of profiles at
 *   the time this function was called.
 * .
 *
 * 
 * @param profileCount Location of a <tt>size_t</tt> that will be overwritten
 *                     with the number of profiles, which could be zero.
 *                     
 * @param profiles Location of a pointer to <tt>GDCredentialsProfile</tt>,
 *                 i.e. a pointer to a pointer. The pointer must be initialized
 *                 to <tt>NULL</tt> by the caller. The location will be
 *                 overwritten with a pointer to a buffer containing the
 *                 returned list of profiles, if any, see above.
 *                 
 * @param error Location of a \ref GDError instance to be overwritten if the 
 *              function fails. See the error descriptions, above.
 *              
 * @return <tt>true</tt> if a list was returned, or if there are no profiles
 *         assigned to the current end user.
 * @return <tt>false</tt> if an error was encountered.
 */
GD_C_API bool GDCredentialsProfile_list(size_t* profileCount,
                                        struct GDCredentialsProfile** profiles,
                                        struct GDError* error);

/** Free a returned profiles buffer.
 * 
 * Call this function to release a profiles list buffer returned by the
 * \ref GDCredentialsProfile_list function.
 * 
 * @param profiles Buffer to release.
 * @param profileCount <tt>size_t</tt> representation of the number of
 *                     structures in the buffer.
 */
GD_C_API void GDCredentialsProfile_free(struct GDCredentialsProfile* profiles,
                                        size_t profileCount);

/** Unregister from notification of User Credentials Profile state changes.
 *
 * Call this function to cancel notification of User Credentials Profile (UCP)
 * state changes. State changes that occur after cancellation may not be
 * notified ever.
 *
 * @see \ref GDCredentialsProfile_register
 */
GD_C_API void GDCredentialsProfile_unregister();

/**
 * \}
 */


#ifdef __cplusplus
}
#endif

#endif
