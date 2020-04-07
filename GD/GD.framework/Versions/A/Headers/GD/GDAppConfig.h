/*
 * (c) 2018 BlackBerry Limited. All rights reserved.
 *
 */

#pragma once

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

/* \cond DOXYGEN_IGNORE */

// See: http://clang.llvm.org/docs/LanguageExtensions.html
#   ifndef __has_extension
#       define __has_extension(x) 0  // Compatibility with non-clang compilers.
#   endif

#   if __has_extension(attribute_deprecated_with_message)
#       define DEPRECATE_APP_COPYPASTEON __attribute__((deprecated("Use GDAppConfigKeyPreventDataLeakageOut")))
#   else
#       define DEPRECATE_APP_COPYPASTEON __attribute__((deprecated))
#   endif

/* \endcond */

#endif

/** Constant key value for application server configuration.\ See under
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyServers;

/** Constant key value for application-specific configuration data.\ See under
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyConfig;

/** Constant key value for the Data Leakage security policy indicator.\ See
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyPreventDataLeakageOut;

/** Constant key value for the Data Leakage security policy indicator.\ See
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyPreventDataLeakageIn;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/**
 * Deprecated constant key value for the Data Leakage security policy
 * indicator.\ See under \ref GDiOS::getApplicationConfig.
 *
 * @deprecated This key is deprecated and will be removed in a future release.
 *             Use the <tt>GDAppConfigKeyPreventDataLeakageOut</tt> value
 *             instead.
 */
extern const NSString* const GDAppConfigKeyCopyPasteOn DEPRECATE_APP_COPYPASTEON;
#endif

/** Constant key value for enabling and disabling detailed diagnostic
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyDetailedLogsOn;

/** Constant key value for allowing and disallowing users from enabling detailed
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyPreventUserDetailedLogs;

/** Constant key value for the user's enterprise email address.\ See under
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyUserId;

/** Constant key value for the user's User Principal Name (UPN).\ See under
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyUserPrincipalName;

/** Constant key value for application instance identifier.\ See under
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyInstanceIdentifier;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/**
 * Constant key value for whether dictation input is allowed.\ See under
 * \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyPreventDictation;

/**
 * Constant key value for whether custom keyboards are allowed.\ See under
 * \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyPreventCustomKeyboards;

/**
 * Constant key value for whether screen recording and sharing is allowed.\ See under
 * \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyPreventScreenRecording;

#endif

/** Constant key value for whether the application is protected by a BlackBerry
 *  Dynamics password.\ See under
 *  \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyProtectedByPassword;

/** Constant key value for whether BlackBerry Enterprise Identity is setup.
 */
extern const NSString* const GDAppEnterpriseIdActivated;

/** Constant key value containing a JSON array of available features.
 */
extern const NSString* const GDAppConfigKeyEnterpriseIdFeatures;

/** Constant key value for extra information from the management
 *  console.\ See under \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyExtraInfo;

/** \defgroup GDProtocols Secure Communication Protocols
 * These constants represent secure communication protocols. They are
 * used in the programming interface for the list of protocols that are allowed
 * and disallowed by the enterprise. Under
 * \reflink GDiOS::getApplicationConfig getApplicationConfig (GDiOS) \endlink, see the
 * <tt>GDAppConfigKeyCommunicationProtocols</tt> item.
 *
 * The following common abbreviations are used:\n
 * SSL for Secure Socket Layer protocols.\n
 * TLS for Transport Layer Security protocols.
 *
 * \{
 */

/** Constant key value for the list of communications protocols allowed by the
 *  enterprise.\ See under \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDAppConfigKeyCommunicationProtocols;

/** Constant value for a secure communication protocol: TLS version 1.0.\ See
 *  under \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDProtocolsKeyTLSv1_0;

/** Constant value for a secure communication protocol: TLS version 1.1.\ See
 *  under \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDProtocolsKeyTLSv1_1;

/** Constant value for a secure communication protocol: TLS version 1.2.\ See
 *  under \ref GDiOS::getApplicationConfig.
 */
extern const NSString* const GDProtocolsKeyTLSv1_2;

/** \}
 */
