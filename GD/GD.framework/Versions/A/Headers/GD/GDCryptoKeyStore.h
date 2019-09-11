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

#include "GDCryptoAlgorithms.h"
#include <stddef.h>

/** Retrieve the current FIPS 140-2 mode of operation.
 * This function returns the FIPS mode of operation.  When in FIPS mode, calls to the Dynamics
 * Crypto API will fail if an algorithm not compliant with FIPS 140-2 is used.
 *
 * @return <tt>int</tt> A return code of non-zero indicates FIPS mode, 0 indicates non-FIPS mode.
 */
GD_C_API int GDFIPS_mode(void);

/** A X509 certificate structure.
 * A X509 structure representing a certificate.
 */
struct GD_C_API GDX509;

/** Copy a X509 structure.
 * Copy resources held by the X509 structure.
 *
 * @param cert <tt>GDX509*</tt> the X509 structure to copy.
 */
GD_C_API struct GDX509* GDX509_copy(const struct GDX509* cert);

/** Free a X509 structure.
 * Deallocates resources held by the X509 structure.
 *
 * @param cert <tt>GDX509*</tt> the X509 structure to free.
 */
GD_C_API void GDX509_free(struct GDX509* cert);

/** A list of X509 certificates.
 * A list of X509 certificate structures.
 */
struct GD_C_API GDX509List;

/** Retrieve a list of X509 certificates within a PEM container.
 * Decodes a PEM container and returns a list of certificates within.
 *
 * @param pem <tt>const char*</tt> a pointer to the PEM data.
 * @param pem_length <tt>int</tt> length of the PEM data.
 * @return <tt>GDX509List*</tt> the list of certificates within the PEM data. If NULL, the reason can be obtained from GDCryptoError_get().
 */
struct GD_C_API GDX509List* GDX509List_read(const char* pem, int pem_length);

/** Copy a list of X509 certificates.
 * Copies and returns a list of certificates.
 *
 * @param certs <tt>GDX509List*</tt> the list to copy.
 * @return <tt>GDX509List*</tt> the copied list of certificates. If NULL, the reason can be obtained from GDCryptoError_get().
 */
struct GD_C_API GDX509List* GDX509List_copy(const struct GDX509List* certs);

/** Count the number of X509 certificates.
 * Returns the number of X509 certificates within a list.
 *
 * @param certs <tt>GDX509List*</tt> the list of certificates to count.
 * @return <tt>int</tt> the number of certificates within the list.
 */
GD_C_API int GDX509List_num(const struct GDX509List* certs);

/** Retrieve an X509 certificate from a list.
 * Returns an X509 certificate at a zero-based index within the list.
 *
 * @param certs <tt>GDX509List*</tt> the list of certificates to access.
 * @param index <tt>int</tt> the index of list to access.
 * @return <tt>GDX509*</tt> the certificate at the specified index.
 */
GD_C_API const struct GDX509* GDX509List_value(const struct GDX509List* certs, int index);

/** Insert a certificate into the list.
 * Copies and inserts a X509 certificate into a list at a given index.
 *
 * @param certs <tt>GDX509List*</tt> the list of certificates to insert into.
 * @param index <tt>int</tt> the index within the list to insert the certificate.
 * @param cert <tt>GDX509*</tt> the certificate to copy and insert.
 * @return <tt>int</tt> the number of certificates within the list.
 */
GD_C_API void GDX509List_insert(const struct GDX509List* certs, int index, const struct GDX509* cert);

/** Retrieve all user certificates managed by Dynamics.
 * Return a list of user certificates known to Dynamics.
 *
 * @return <tt>GDX509List*</tt> a list of all user certificates known to Dynamics, NULL if none.
 */
GD_C_API struct GDX509List* GDX509List_all_user_certs();

/** Retrieve all valid user certificates managed by Dynamics.
 * Return a list of user certificates known to Dynamics if the device's time falls within the certificate's validity date range.
 *
 * @return <tt>GDX509List*</tt> a list of all valid user certificates known to Dynamics, NULL if none.
 */
GD_C_API struct GDX509List* GDX509List_valid_user_signing_certs();

/** Retrieve all valid user encryption certificates managed by Dynamics.
 * Return a list of user certificates known to Dynamics if the device's time falls within the certificate's validity date range, and usage properties
 * permit it to be used for encryption purposes.
 *
 * @return <tt>GDX509List*</tt> a list of all valid user encryption certificates known to Dynamics, NULL if none.
 */
GD_C_API struct GDX509List* GDX509List_valid_user_encryption_certs();

/** Retrieve all  user encryption certificates managed by Dynamics.
 * Return a list of user certificates known to Dynamics if usage properties permit it to be used for encryption purposes.
 * The list may include expired certificates.
 *
 * @return <tt>GDX509List*</tt> a list of all user encryption certificates known to Dynamics, NULL if none.
 */
GD_C_API struct GDX509List* GDX509List_all_user_encryption_certs();

/** Retrieve a certificate chain for a user certificate.
 * Return a certificate chain for the user certificate, containing intermediate certificates, if any.
 *
 * @param cert <tt>GDX509*</tt> the user certificates for which the chain is required.
 * @return <tt>GDX509List*</tt> a list of intermediate certificates or NULL if none.
 */
GD_C_API struct GDX509List* GDX509List_aux_certs(const struct GDX509* cert);

/** Free a X509 list structure.
 * Deallocates resources held by the X509 list structure.
 *
 * @param certs <tt>GDX509List*</tt> the X509 list structure to free.
 */
GD_C_API void GDX509List_free(struct GDX509List* certs);

/** Key structure.
 * A structure representing a generic public or private key. The key may be an RSA, DSA (Digtal Signature Algorithm),
 * or EC (Elliptic Curve) type.
 * Private keys originating from the Dynamics Key Store are are opaque and cannot be used to encrypt or decrypt data using
 * external APIs. Privte keys are only useful when supplying to Dynamics Crypto APIs requiring a GDKey argument.
 */
struct GD_C_API GDKey;

/** Key context structure.
 * A structure representing a public or private key context.  A context is used for encrypting or decrypting data.
 */
struct GD_C_API GDKeyContext;

/** RSA key type.
 * Represents a RSA (Rivest–Shamir–Adleman) key type. It may be returned when calling GDKey_type().
 * It may also be required for some function calls requiring the key type to be specified.
 */
GD_C_API extern const int GDKEY_TYPE_RSA;

/** DSA key type.
 * Represents a DSA (Digital Signature Algorithm) key type. It may be returned when calling GDKey_type().
 * It may also be required for some function calls requiring the key type to be specified.
 */
GD_C_API extern const int GDKEY_TYPE_DSA;

/** EC key type.
 * Represents a EC (Elliptic Curve) key type. It may be returned when calling GDKey_type().
 * It may also be required for some function calls requiring the key type to be specified.
 */
GD_C_API extern const int GDKEY_TYPE_EC;

/** Retrieve the public key.
 * Return the public key from within the supplied certificate.
 *
 * @param cert <tt>GDX509*</tt> the certificate for which the key is required.
 * @return <tt>GDKey*</tt> the public key of the certificate. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDKey* GDKey_public(const struct GDX509* cert);

/** Retrieve an opaque private key corresponding to a user certificate.
 * Return the private key corresponding to a user certificate stored within the Dynamics Key Store.
 * The key data is not meaningful and will yield garbage if used to sign or decrypt data using external (non-Dynamics) APIs.
 *
 * @param cert <tt>GDX509*</tt> the certificate for which the private key is required.
 * @return <tt>GDKey*</tt> the private key of the certificate. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDKey* GDKey_private(const struct GDX509* cert);

/** Free a key structure.
 * Deallocates resources held by the key structure.
 *
 * @param key <tt>GDKey*</tt> the key structure to free.
 */
GD_C_API void GDKey_free(struct GDKey* key);

/** Compare two keys.
 * Compare two keys.
 *
 * @param a <tt>GDKey*</tt> key a.
 * @param b <tt>GDKey*</tt> key b.
 * @return <tt>int</tt> 1 if the keys match, 0 if they don't match, -1 if the key types are different and -2 if the operation is not supported.
 */
GD_C_API int GDKey_compare(const struct GDKey* a, const struct GDKey* b);

/** Signature size.
 * Returns the maximum size of an ASN.1 encoded signature in bytes.
 * It can be used to determine how much memory must be allocated for a signature.
 *
 * @param key <tt>GDKey*</tt> the key for which the signature size is required.
 * @return <tt>int</tt> the maximum signature size in bytes.
 */
GD_C_API int GDKey_size(const struct GDKey* key);

/** Key size.
 * Returns the key size in bits.
 *
 * @param key <tt>GDKey*</tt> the key for which the size is required.
 * @return <tt>int</tt> the key size in bits.
 */
GD_C_API int GDKey_bits(const struct GDKey* key);

/** Key type.
 * Returns the type of the underlying key.
 *
 * @param key <tt>GDKey*</tt> the key for which the type is required.
 * @return <tt>int</tt> the key type, which will be either GDKEY_TYPE_RSA, GDKEY_TYPE_DSA, or GDKEY_TYPE_EC.
 */
GD_C_API int GDKey_type(const struct GDKey* key);

/** Return the key algorithm name.
 * Return the key algorithm name.
 *
 * @param key <tt>GDKey*</tt> the key for which the algorithm name is required.
 * @return <tt>char*</tt> the key algorithm name.
 */
GD_C_API const char* GDKey_name(const struct GDKey* key);

/** Create a new key context.
 * Create a new key context object. Contexts are opaque structures used by other APIs and must not be used simultaneously by multiple threads.
 *
 * @param key <tt>GDKey*</tt> the key for which the context is required.
 * @return <tt>GDKeyContext*</tt> the key context or NULL if an error occurred.
 */
GD_C_API struct GDKeyContext* GDKeyContext_new(struct GDKey* key);

/** Copy a key context.
 * Copy a key context object.
 *
 * @param ctx <tt>GDKey*</tt> the source key context to copy.
 * @return <tt>GDKeyContext*</tt> a copy of the key context.
 */
GD_C_API struct GDKeyContext* GDKeyContext_copy(struct GDKeyContext* ctx);

/** Free a key context.
 * Deallocates resources held by the key context.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to free.
 */
GD_C_API void GDKeyContext_free(struct GDKeyContext* ctx);

/** Initialize a key context for signing.
 * Initializes a key context object for a signing operation.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to initialize.
 * @return 1 for success and 0 or a negative value for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_sign_init(struct GDKeyContext* ctx);

/** Sign a message.
 * Signs a message, usually a message digest and returns the ASN.1 DER encoded signature. This function does not calculate the
 * digest of the data to be signed. The required size for the signature buffer can be computed by calling this function with a
 * NULL signature buffer.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to sign with.
 * @param signature <tt>void*</tt> NULL, or a buffer large enough to hold the signature.
 * @param signature_length <tt>size_t*</tt> must be set to the length of the signature buffer, and upon return will be set to the amount of data written to the signature buffer. If signature is NULL, it will be set to the maximum size of the signature.
 * @param message <tt>void*</tt> the message to sign, usually a digest.
 * @param message_length <tt>size_t*</tt> the length of the message to sign.
 * @return 1 for success and 0 or a negative value for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_sign(struct GDKeyContext* ctx, void* signature, size_t* signature_length, const void* message, size_t message_length);

/** Initialize a key context for verification.
 * Initializes a key context object for a verification operation.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to initialize.
 * @return 1 for success and 0 or a negative value for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_verify_init(struct GDKeyContext* ctx);

/** Verify a signed message.
 * Verifys an ASN.1 DER encoded signature.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to verify with.
 * @param signature <tt>void*</tt> a buffer holding the signature.
 * @param signature_length <tt>size_t*</tt> the length of the signature.
 * @param message <tt>void*</tt> a buffer holding the message or digest to be verified.
 * @param message_length <tt>size_t*</tt> the length of the message or digest to verify.
 * @return 1 for successful verification, 0 for verification failure (not an error - message or signature is not to be trusted), or a
 *         negative value for a more serious error. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_verify(struct GDKeyContext* ctx, const void* signature, size_t signature_length, const void* message, size_t message_length);

/** Initialize a key context for encryption.
 * Initializes a key context object for an encryption operation.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to initialize.
 * @return 1 for success and 0 or a negative value for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_encrypt_init(struct GDKeyContext* ctx);

/** Encrypt data.
 * Encrypts data using the key associated with the supplied key context.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to encrypt with.
 * @param encrypted <tt>void*</tt> a buffer large enough to hold the encrypted data. If NULL, the length required is returned in encrypted_length.
 * @param encrypted_length <tt>size_t*</tt> the length of the buffer supplied to hold the encrypted data, and upon successful return the actual length of the encrypted data.
 * @param clear <tt>void*</tt> the data to encrypt.
 * @param clear_length <tt>size_t*</tt> the length of the data to encrypt.
 * @return 1 for success, 0 or a negative value for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_encrypt(struct GDKeyContext* ctx, void* encrypted, size_t* encrypted_length, const void* clear, size_t clear_length);

/** Initialize a key context for decryption.
 * Initializes a key context object for a decryption operation.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to initialize.
 * @return 1 for success and 0 or a negative value for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_decrypt_init(struct GDKeyContext* ctx);

/** Decrypt data.
 * Decrypts data using the key associated with the supplied key context.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to decrypt with.
 * @param clear <tt>void*</tt> a buffer large enough to hold the decrypted data. If NULL, the length required is returned in clear_length.
 * @param clear_length <tt>size_t*</tt> the length of the buffer supplied to hold the decrypted data, and upon successful return the actual length of the decrypted data.
 * @param encrypted <tt>void*</tt> the data to decrypt.
 * @param encrypted_length <tt>size_t*</tt> the length of the data to decrypt.
 * @return 1 for success, 0 or a negative value for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_decrypt(struct GDKeyContext* ctx, void* clear, size_t* clear_length, const void* encrypted, size_t encrypted_length);

/** Sign a digest.
 * Signs a message digest. The output signature buffer size can be computed by calling GDKey_size().
 *
 * @param digest <tt>GDDigestContext*</tt> the initialized digest context.
 * @param signature <tt>void*</tt> a buffer large enough to hold the signature at least GDKey_size(key) bytes.
 * @param signature_length <tt>unsigned int*</tt> upon successful return will be set to the amount of data written to the signature buffer.
 * @param key <tt>GDKey*</tt> the private signing key.
 * @return 1 for success and 0 for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_sign_final(struct GDDigestContext* digest, void* signature, unsigned int* signature_length, struct GDKey* key);

/** Verify a digest.
 * Verifies a message digest.
 *
 * @param digest <tt>GDDigestContext*</tt> the initialized digest context to verify.
 * @param signature <tt>void*</tt> a buffer containing the DER encoded signature.
 * @param signature_length <tt>unsigned int</tt> the length of the signature buffer in bytes.
 * @param key <tt>GDKey*</tt> the signer's public key.
 * @return 1 for successful verification, 0 for verification failure (not an error - message or signature is not to be trusted), or a
 *         negative value for a more serious error. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_verify_final(struct GDDigestContext* digest, const void* signature, unsigned int signature_length, struct GDKey* key);

/** Sets the message digest type used in a signature.
 * Sets the digest algorithm to be used when calculating a signature.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to setup.
 * @param digest <tt>GDDigest*</tt> the digest algorithm to use.
 * @return 1 for success, 0 for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_set_signature_md(struct GDKeyContext* ctx, const struct GDDigest* digest);

/** Returns the message digest type.
 * Returns the digest algorithm for a given key context.
 *
 * @param ctx <tt>GDKeyContext*</tt> the key context to query.
 * @param digest <tt>GDDigest**</tt> the digest algorithm returned.
 * @return 1 for success, 0 for failure. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDKey_get_signature_md(struct GDKeyContext* ctx, const struct GDDigest** digest);

#ifdef __cplusplus
}
#endif

/** @}
 */
