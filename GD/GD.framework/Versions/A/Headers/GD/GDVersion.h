/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 *
 */

#pragma once

#import <Foundation/Foundation.h>

#import "GDPortability.h"

GD_NS_ASSUME_NONNULL_BEGIN

/** BlackBerry Dynamics entitlement version.
 * 
 * Objects of this class are used to represent BlackBerry Dynamics
 * entitlement versions.
 *
 * BlackBerry Dynamics entitlement versions are sequences of numbers. The first
 * number is the major version number and is the most significant. Numbers later
 * in the sequence are of decreasing significance. By convention, there are four
 * numbers in an entitlement version.
 *
 * In the BlackBerry Dynamics management console, and in other administrative
 * user interfaces, entitlement versions are represented by "dotted string"
 * values, in which the numbers are separated by full stops (periods).
 *
 * Objects of this class are used in the
 * \reflink GDiOS::getEntitlementVersionsFor:callbackBlock: getEntitlementVersionsFor: (GDiOS)\endlink
 * results list.
 *
 * <h3>Interface Usage</h3>
 *
 * \code
 * 
 * #import <GD/GDVersion.h>
 *
 * // Initialize from dotted string representation.
 * GDVersion *gdVersionA = [[GDVersion alloc] initWithString:@"1.2.0.3"];
 * 
 * NSUInteger length = [gdVersionA numberOfVersionParts];
 *  // length == 4
 *
 * NSUInteger majorVersion = [gdVersionA versionPartAt:0];
 * // majorVersion == 1
 *
 * // Initialize from array of numbers.
 * GDVersion *gdVersionB = [[GDVersion alloc] initWithArray:@[1, 3] ];
 * 
 * length = [gdVersionB numberOfVersionParts];
 * // length == 2
 *
 * NSInteger comparison = [gdVersionA compare:gdVersionB];
 * // comparison == NSOrderedAscending
 *
 * comparison = [gdVersionB compare:gdVersionA];
 * // comparison == NSOrderedDescending
 *
 * GDVersion *gdVersionC = [[GDVersion alloc] initWithString:@"1.3"];
 *
 * comparison = [gdVersionB compare:gdVersionC];
 * // comparison == NSOrderedSame
 *
 * BOOL isOrder = [gdVersionA isEqualToVersion:gdVersionB];
 * // isOrder == NO
 *
 * isOrder = [gdVersionA isGreaterThanVersion:gdVersionB];
 * // isOrder == NO
 *
 * isOrder = [gdVersionA isLessThanVersion:gdVersionB];
 * // isOrder == YES
 *
 * NSString *dottedString = [gdVersionB stringValue];
 * // [dottedString isEqualToString:@"1.3"] == YES
 *
 * \endcode
 *
 * The code snippet above illustrates the programming interface.
 */
@interface GDVersion : NSObject

/** Initialize from a dotted string representation.
 * 
 * @return <tt>GDVersion</tt> object with constituent version numbers read from
 *         a dotted string representation.
 */
- (GD_NSNULLABLE instancetype)initWithString:(NSString*)versionString;

/** Initialize from an array of <tt>NSNumber</tt> objects.
 * 
 * @return <tt>GDVersion</tt> object with constituent version numbers
 *         initialized from an array of <tt>NSNumber</tt> objects.
 */
- (GD_NSNULLABLE instancetype)initWithArray:(NSArray*)array;

/** Count of how many constituent numbers are in the version.
 * 
 * @return <tt>NSUInteger</tt> representation of the count of how many
 *         constituent numbers there are in the version.
 */
- (NSUInteger)numberOfVersionParts;

/** Get one constituent version number.
 * 
 * Call this function to get one of the constituent numbers in the version,
 * specified by a numeric position. Position zero is the major version number,
 * which is the most significant.
 *
 * @return The constituent version number at the specified position.
 */
- (NSUInteger)versionPartAt:(NSUInteger)position;

/** Compare this version with another version.
 * 
 * @return <tt>NSComparisonResult</tt> representing the relative value of this
 *         version compared to the other version.
 */
- (NSComparisonResult)compare:(GDVersion*)anotherVersion;

/** Check for equality with another version.
 * 
 * @return <tt>YES</tt> if the two versions are the same.
 * @return <tt>NO</tt> Otherwise.
 */
- (BOOL)isEqualToVersion:(GD_NSNULLABLE GDVersion*)anotherVersion;

/** Check whether this version is more than another version.
 * 
 * @return <tt>YES</tt> if this version is more than the specified version.
 * @return <tt>NO</tt> Otherwise.
 */
- (BOOL)isGreaterThanVersion:(GD_NSNULLABLE GDVersion*)anotherVersion;

/** Check whether this version is less than another version.
 * 
 * @return <tt>YES</tt> if this version is less than the specified version.
 * @return <tt>NO</tt> Otherwise.
 */
- (BOOL)isLessThanVersion:(GD_NSNULLABLE GDVersion*)anotherVersion;

/** Dotted string representation.
 * 
 * @return Dotted string representation of this version.
 */
@property(nonatomic, readonly) NSString *stringValue;
@end

GD_NS_ASSUME_NONNULL_END
