/*
 * (c) 2019 BlackBerry Limited. All rights reserved.
 */

#pragma once

/** \addtogroup cryptolist
 * @{
 */

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

#include "GDCryptoKeyStore.h"
#include "GDCryptoAlgorithms.h"
#include "GDCryptoError.h"

/** PKCS #1 padding.
 *  @deprecated Use GDCryptoKeyStore.h which is a generic API that can handle RSA, DSA, and EC keys.
 *
 *  Returns a constant representing a PKCS #1 v1.5 padding scheme.
 */
GD_C_API extern const int GDRSA_PKCS1_PADDING;

/** PKCS #1 OAEP padding.
 *  @deprecated Use GDCryptoKeyStore.h which is a generic API that can handle RSA, DSA, and EC keys.
 *
 *  Returns a constant representing a EME-OAEP padding scheme, as defined in PKCS #1 v2.0.
 */
GD_C_API extern const int GDRSA_PKCS1_OAEP_PADDING;

/** No padding.
 *  @deprecated Use GDCryptoKeyStore.h which is a generic API that can handle RSA, DSA, and EC keys.
 *
 *  Returns a constant representing no padding scheme.
 */
GD_C_API extern const int GDRSA_NO_PADDING;

/** RSA key structure.
 * @deprecated Use GDCryptoKeyStore.h which is a generic API that can handle RSA, DSA, and EC keys.
 *
 * A structure representing a public or private RSA key.
 * Private key data is not meaningful and will yield garbage if used to sign or decrypt data using other APIs.
 * It is only useful when supplying this to Dynamics Crypto APIs requiring a GDRSA argument.
 */
struct GD_C_API GDRSA;

/** Retrieve the RSA public key.
 * @deprecated Use GDKey_public().
 *
 * Return the RSA public key from within the supplied certificate.
 *
 * @param cert <tt>GDX509*</tt> the certificate for which the key is required.
 * @return <tt>GDRSA*</tt> the public key of the certificate. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDRSA* GDRSA_public_key(const struct GDX509* cert);

/** Retrieve a pseudo private RSA key corresponding to a user certificate.
 * @deprecated Use GDKey_private().
 *
 * Return the private RSA key corresponding to a user certificate.  The key data is not meaningful and will yield garbage if used to
 * sign or decrypt data using non-Dynamics APIs.
 *
 * @param cert <tt>GDX509*</tt> the certificate for which the private key is required.
 * @return <tt>GDRSA*</tt> the RSA private key of the certificate. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDRSA* GDRSA_private_key(const struct GDX509* cert);

/** RSA modulus size.
 * @deprecated Use GDKey_size().
 *
 * Returns the RSA modulus size in bytes. It can be used to determine how much memory must be allocated for an RSA encrypted value.
 *
 * @param rsa <tt>GDRSA*</tt> the RSA key for which the modulus size is required.
 * @return <tt>int</tt> the RSA modulus size in bytes.
 */
GD_C_API int GDRSA_size(const struct GDRSA* rsa);

/** Free a RSA key structure.
 * @deprecated Use GDKey_free().
 *
 * Deallocates resources held by the RSA key structure.
 *
 * @param rsa <tt>GDRSA*</tt> the RSA key structure to free.
 */
GD_C_API void GDRSA_free(struct GDRSA* rsa);

/** RSA sign a message digest.
 * @deprecated Use GDDigest_sign_final().
 *
 * RSA sign a message digest specified in PKCS #1 v2.0.
 *
 * @param digest_algorithm <tt>const GDDigest*</tt> the digest algorthim of the message.
 * @param digest <tt>const void*</tt> a pointer to a buffer containing the message digest.
 * @param digest_size <tt>unsigned int</tt> the digest size.
 * @param signature <tt>void*</tt> a pointer to memory large enough to contain the signature. The caller must allocate and eventually
 *                              free this buffer.  The size of the buffer required can be determined by calling GDRSA_size().
 * @param signature_size <tt>unsigned int</tt> the number of signature bytes copied to signature.
 * @param rsa <tt>GDRSA*</tt> the signer's RSA private key.
 * @return <tt>int</tt> 1 on success, 0 for failure.
 */
GD_C_API int GDRSA_sign(const struct GDDigest* digest_algorithm, const void* digest, unsigned int digest_size, void* signature, unsigned int* signature_size, struct GDRSA* rsa);

/** Verify an RSA signed message.
 * @deprecated GDDigest_verify_final().
 *
 * Verify an RSA signed message.
 *
 * @param digest_algorithm <tt>const GDDigest*</tt> denotes the message digest algorithm that was used to generate the signature.
 * @param digest <tt>const void*</tt> a pointer to a buffer containing the digest of the message to verify.
 * @param digest_size <tt>unsigned int</tt> the message digest size in bytes.
 * @param signature <tt>const void*</tt> a pointer to the RSA signature.
 * @param signature_size <tt>unsigned int</tt> the size of the RSA signature.
 * @param rsa <tt>GDRSA*</tt> the signer's RSA public key.
 * @return <tt>int</tt> 1 on successful verification, 0 for failure. The reason for failure can be determined by calling GDCryptoError_get().
 */
GD_C_API int GDRSA_verify(const struct GDDigest* digest_algorithm, const void* digest, unsigned int digest_size, const void* signature, unsigned int signature_size, struct GDRSA* rsa);

/** RSA public encrypt.
 * @deprecated Use GDKey_encrypt().
 *
 * RSA encrypt a message with the recipient's public key.
 *
 * @param flen <tt>int</tt> the size of the data to be encrypted.
 * @param from <tt>const void*</tt> a pointer to the data (usually a symmetric key) to be encrypted.
 * @param to <tt>void*</tt> a pointer to memory large enough to contain the encrypted data. The caller must allocate and eventually
 *                          free this buffer.  The size of the buffer required can be determined by calling GDRSA_size().
 * @param rsa <tt>GDRSA*</tt> the recipient's public key.
 * @param padding <tt>int</tt> the padding scheme to use.
 * @return <tt>int</tt> if successful, the size in bytes of the encrypted data, or -1 if not. The reason for failure can be determined by calling GDCryptoError_get().
 */
GD_C_API int GDRSA_public_encrypt(int flen, const void* from, void* to, struct GDRSA* rsa, int padding);

/** RSA private decrypt.
 * @deprecated Use GDKey_decrypt().
 *
 * RSA decrypt a message with the recipient's private key.
 *
 * @param flen <tt>int</tt> the size of the data to be decrypted.
 * @param from <tt>const void*</tt> a pointer to the data (usually a symmetric key) to be decrypted.
 * @param to <tt>void*</tt> a pointer to memory large enough to contain the decrypted data. The caller must allocate and eventually
 *                          free this buffer.  The size of the buffer required can be determined by calling GDRSA_size().
 * @param rsa <tt>GDRSA*</tt> the recipient's private key.
 * @param padding <tt>int</tt> the padding scheme to use.
 * @return <tt>int</tt> if successful, the size in bytes of the decrypted data, or -1 if not. The reason for failure can be determined by calling GDCryptoError_get().
 */
GD_C_API int GDRSA_private_decrypt(int flen, const void* from, void* to, struct GDRSA* rsa, int padding);

/** RSA private encrypt.
 * @deprecated Use GDKey_encrypt().
 *
 * RSA encrypt a message with the recipient's private key. This is a low-level signing operation. To generate an RSA signature, call GDRSA_sign().
 *
 * @param flen <tt>int</tt> the size of the data to be encrypted.
 * @param from <tt>const void*</tt> a pointer to the data (usually a message digest) to be encrypted.
 * @param to <tt>void*</tt> a pointer to memory large enough to contain the encrypted data. The caller must allocate and eventually
 *                          free this buffer.  The size of the buffer required can be determined by calling GDRSA_size().
 * @param rsa <tt>GDRSA*</tt> the signer's private key.
 * @param padding <tt>int</tt> the padding scheme to use.
 * @return <tt>int</tt> if successful, the size in bytes of the encrypted data, or -1 if not. The reason for failure can be determined by calling GDCryptoError_get().
 */
GD_C_API int GDRSA_private_encrypt(int flen, const void* from, void* to, struct GDRSA* rsa, int padding);

/** RSA public decrypt.
 * @deprecated Use GDKey_decrypt().
 *
 * RSA decrypt a message with the sender's public key. This is a low-level verification operation. To verify an RSA signature, call GDRSA_verify().
 *
 * @param flen <tt>int</tt> the size of the data to be decrypted.
 * @param from <tt>const void*</tt> a pointer to the data (usually a message digest) to be decrypted.
 * @param to <tt>void*</tt> a pointer to memory large enough to contain the decrypted data. The caller must allocate and eventually
 *                          free this buffer.  The size of the buffer required can be determined by calling GDRSA_size().
 * @param rsa <tt>GDRSA*</tt> the sender's public key.
 * @param padding <tt>int</tt> the padding scheme to use.
 * @return <tt>int</tt> if successful, the size in bytes of the decrypted data, or -1 if not. The reason for failure can be determined by calling GDCryptoError_get().
 */
GD_C_API int GDRSA_public_decrypt(int flen, const void* from, void* to, struct GDRSA* rsa, int padding);

#ifdef __cplusplus
}
#endif

/** @}
 */

