/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 * 
 */

#pragma once

#import <Foundation/Foundation.h>
#import "GDPortability.h"
#import "GDServiceProvider.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import <GD/GDiOS.h>
#else
#import "GDMac.h"
#endif

GD_NS_ASSUME_NONNULL_BEGIN



/** Details of a provided service.
 * 
 * This class is used to return information about a provided service. The
 * <tt>services</tt> property of a \reflink GDServiceProvider GDServiceProvider\endlink object is a
 * collection of instances of this class.
 */
@interface GDServiceDetail : NSObject

/** Initializer.
 *
 * The initializer is provided for completeness. In general, there is no need
 * for the application code to create instances of this class.
 */
- (instancetype)initWithService:(NSString*)identifier
                     andVersion:(NSString*)version
                 andServiceType:(GDServiceType)type;


#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_SERVICEDETAIL_INITIALIZER __attribute__((deprecated("Use initWithService:andVersion:andServiceType instead.")))
#else
#   define DEPRECATE_SERVICEDETAIL_INITIALIZER __attribute__((deprecated))
#endif

/** Initializer (deprecated).
 * 
 * \deprecated This public initializer is deprecated and will be removed in a
 * future release. Use
 * \link GDServiceDetail::initWithService:andVersion:andServiceType:\endlink
 * instead.
 */
- (instancetype)initWithService:(NSString*)identifier andVersion:(NSString*)version andType:(GDServiceProviderType)type DEPRECATE_SERVICEDETAIL_INITIALIZER;

/** BlackBerry Dynamics service identifier.
 */
@property (nonatomic, strong, readonly) NSString* identifier;

/** BlackBerry Dynamics service version.
 */
@property (nonatomic, strong, readonly) NSString* version;

/** Indicator of the type of the provided service, either application-based or
 *  server-based.
 */
@property (nonatomic, assign, readonly) GDServiceType serviceType;

#if __has_extension(attribute_deprecated_with_message)
#   define DEPRECATE_SERVICEPROVIDERTYPE __attribute__((deprecated("Use GDServiceType instead.")))
#else
#   define DEPRECATE_SERVICEPROVIDERTYPE __attribute__((deprecated))
#endif

/** Indicator of the type of the provided service, either application-based or
 *  server-based (deprecated).
 *
 * \deprecated This property is deprecated and will be removed in a future
 *  release. Use the <tt>serviceType</tt> property instead.
 */
@property (nonatomic, assign, readonly) GDServiceProviderType type DEPRECATE_SERVICEPROVIDERTYPE;

#undef DEPRECATE_SERVICEDETAIL_INITIALIZER
#undef DEPRECATE_SERVICEPROVIDERTYPE

@end
GD_NS_ASSUME_NONNULL_END
