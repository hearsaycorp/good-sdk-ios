/*
 * (c) 2017 BlackBerry Limited. All rights reserved.
 */

#ifndef GD_COMMON_H
#define GD_COMMON_H

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
  
#ifndef GD_C_API_EXT
#   define GD_C_API_EXT
#endif

#include <stdint.h>
#include <stddef.h>

/** Success.
 *
 * No error occurred.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDSuccess;

/** Out of memory.
 *
 * Memory allocation for results failed.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorOutOfMemory;

/** Not authorized.
 *
 * BlackBerry Dynamics authorization processing isn't complete.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorNotAuthorized;

/** Not found.
 *
 * Profile not found.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorNotFound;

/** Not mapped to a User Credential Profile.
 *
 * Credential doesn't match the mapping criteria specified in any User
 * Credential Profile that is assigned to the end user in the management
 * console.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorNotMapped;

/** Wrong password.
 *
 * Encrypted file couldn't be decrypted with the specified password.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorWrongPassword;

/** General error.
 *
 * Error condition not covered by specific codes.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorGeneral;

/** Invalid argument.
 *
 * Input parameter has a wrong value.
 * 
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorInvalidArgument;
    
/** Not allowed.
 *
 * Certificate couldn't be imported because it doesn't comply with
 * the enterprise policy settings that apply to the end user.
 * For example, the certificate file's encryption doesn't comply with
 * the Federal Information Processing Standard (FIPS), if this is required
 * by policy.
 *
 * \ingroup clientcertificateimport
 */
GD_C_API extern const int32_t GDErrorNotAllowed;

/** Error structure.
 *
 * This structure is used to represent errors in some parts of the BlackBerry
 * Dynamics programming interface.
 *
 * For example, it is used by the \ref clientcertificateimport functions.
 */
struct GD_C_API GDError {
    
    /** Numeric code for the error, always one of the GDError constants defined
     *  in this header file.
     */
    int32_t code;
    
    /** Error message that is suitable to display to the end user, at least for
     *  diagnostic purposes.
     */
    const char* description;
};

/** Data encapsulation structure.
 *
 * This structure is used to encapsulate some types of binary data in the
 * BlackBerry Dynamics programming interface.
 *
 * For example, this structure is used to encapsulate the binary DER encoded
 * representation of X.509 ASN1 certificate data in a
 * \link GDX509Certificate \endlink instance.
 * 
 * @see clientcertificateimport
 */
struct GD_C_API GDData {
    /** Binary data. */
    void* data;
    
    /** Size of the data. */
    size_t size;
    
    /** Indication of the type of data in this encapsulation. */
    char* type;
};

#ifdef __cplusplus
}
#endif

#endif
