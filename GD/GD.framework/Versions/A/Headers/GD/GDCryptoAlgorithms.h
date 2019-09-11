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
#include "GDCryptoError.h"
#include <stddef.h>

/** Maximum block length for all ciphers.
 * Returns the maximum block length for all ciphers.
 */
GD_C_API extern const int GDCIPHER_MAX_BLOCK_LENGTH;

/** Maximum key length for all ciphers.
 * Returns the maximum key length for all ciphers.
 */
GD_C_API extern const int GDCIPHER_MAX_KEY_LENGTH;

/** Stream cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_STREAM_CIPHER;

/** ECB cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_ECB_MODE;

/** CBC cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_CBC_MODE;

/** CFB cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_CFB_MODE;

/** OFB cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_OFB_MODE;

/** CTR cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_CTR_MODE;

/** GCM cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_GCM_MODE;

/** CCM cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_CCM_MODE;

/** XTS cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_XTS_MODE;

/** Wrap cipher mode.
 */
GD_C_API extern const unsigned long GDCIPHER_WRAP_MODE;

/** Cipher suite structure.
 * A structure representing a cryptographic suite of cipher algorithms.
 */
struct GD_C_API GDCipher;

/** Cipher context structure.
 * A structure representing a symmetric cipher context.  A context is used for encrypting and decrypting a message.
 */
struct GD_C_API GDCipherContext;

/** Return all cipher suites by name.
 * Returns an array of supported cipher suite names.
 *
 * @return <tt>char**</tt> an array of c-strings containing the cipher names.
 */
GD_C_API const char** GDCipher_list();

/** Return a cipher suite by name.
 * Returns a cipher structure by suite name.  The name must be the long name format as specified in
 * obj_mac.h of OpenSSL.
 *
 * @param name <tt>char*</tt> the name of the cipher suite required. For example, "des-ede3-cbc".
 * @return <tt>GDCipher*</tt> the cipher suite or NULL. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API const struct GDCipher* GDCipher_byname(const char* name);

/** Return a cipher suite's name.
 * Returns the name of the given cipher suite.  The name will be in long name format as specified in
 * obj_mac.h of OpenSSL.
 *
 * @param ctx <tt>GDCipherContext*</tt> the cipher context for which the name is required.
 * @return <tt>char*</tt> the cipher suite name or NULL.
 */
GD_C_API const char* GDCipher_name(const struct GDCipherContext* ctx);

/** Return a cipher by context.
 * Returns a cipher structure for the given cipher context.
 *
 * @param ctx <tt>GDCipherContext*</tt> the context of the cipher required.
 * @return <tt>GDCipher*</tt> the cipher suite or NULL.
 */
GD_C_API const struct GDCipher* GDCipher_bycontext(const struct GDCipherContext* ctx);

/** Return the cipher block size.
 * Returns the block size of a given cipher.
 *
 * @param cipher <tt>GDCipher*</tt> the cipher for which the block size is required.
 * @return <tt>int</tt> the block size of the cipher in bytes.
 */
GD_C_API int GDCipher_block_size(const struct GDCipher* cipher);

/** Return the cipher key length.
 * Returns the key length of a given cipher.
 *
 * @param cipher <tt>GDCipher*</tt> the cipher for which the key length is required.
 * @return <tt>int</tt> the key length of the cipher.
 */
GD_C_API int GDCipher_key_length(const struct GDCipher* cipher);

/** Return the IV length.
 * Returns the IV length of a given cipher.
 *
 * @param cipher <tt>GDCipher*</tt> the cipher for which the IV length is required.
 * @return <tt>int</tt> the IV length of the cipher.
 */
GD_C_API int GDCipher_iv_length(const struct GDCipher* cipher);

/** Return the cipher mode.
 * Returns the mode of a given cipher.
 *
 * @param cipher <tt>GDCipher*</tt> the cipher for which the mode is required.
 * @return <tt>unsigned long</tt> the mode of the cipher.
 */
GD_C_API unsigned long GDCipher_mode(const struct GDCipher* cipher);

/** Enable or disable padding.
 * Enable or disable padding for the given cipher context.
 *
 * @param ctx <tt>GDCipherContext*</tt> the cipher context for which padding is to be enabled or disabled.
 * @param padding <tt>int</tt> 0 for no padding, any other value to enable padding.
 */
GD_C_API void GDCipher_set_padding(struct GDCipherContext* ctx, int padding);

/** Return a new cipher context.
 *  A cipher context is used to symmetrically encrypt or decrypt a message.
 *  The context should be initialized with a specific cipher algorithm by calling GDCipher_init().
 *
 * @return <tt>GDCipherContext*</tt> a cipher context.
 */
GD_C_API struct GDCipherContext* GDCipher_new(void);

/** Initialize a cipher context.
 *  Initialize the cipher context with a specific cipher suite.
 *
 * @param ctx <tt>GDCipherContext*</tt> a pointer to the cipher context.
 * @param cipher <tt>const GDCipher*</tt> the cipher suite, for example AES-256-CBC.
 * @param key <tt>const void*</tt> NULL or the symmetric key to use, depending on the cipher.
 * @param iv <tt>const void*</tt> NULL or the IV to use, depending on the cipher.
 * @param mode <tt>int</tt> 1 for encryption, 0 for decryption
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDCipher_init(struct GDCipherContext* ctx, const struct GDCipher* cipher, const void* key, const void* iv, int mode);

/** Copy a cipher context.
 *  Copies the cipher context. The destination cipher is cleaned up before being updated to match the
 *  source cipher.
 *
 * @param source <tt>GDCipherContext*</tt> a pointer to a cipher context.
 * @param destination <tt>const GDCipherContext*</tt> the cipher context to copy.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDCipher_copy(struct GDCipherContext* destination, const struct GDCipherContext* source);

/** Encipher or decipher.
 * Perform encryption or decryption of a message. This function can be called multiple times to encrypt or
 * decrypt successive blocks of data.
 *
 * @param ctx <tt>GDCipherContext*</tt> a pointer to the cipher context.
 * @param out_data <tt>void*</tt> a buffer large enough to contain the encrypted or decrypted data. The amount of data written
 *                                may be anything from zero bytes to (in_sz + cipher_block_size - 1).
 * @param out_data_sz <tt>size_t</tt> the number of bytes written to out_data.
 * @param in_data <tt>const void*</tt> a pointer to the data to be encrypted or decrypted.
 * @param in_data_sz <tt>int</tt> the number of bytes of the data to be encrypted.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDCipher_update(struct GDCipherContext* ctx, void* out_data, int* out_data_sz, const void* in_data, int in_data_sz);

/** Encipher or decipher last block.
 * Finish encryption or decryption of the last block of data.
 *
 * @param ctx <tt>GDCipherContext*</tt> a pointer to the cipher context.
 * @param out_data <tt>size_t</tt> a buffer large enough to contain the encrypted or decrypted data. The amount of data written
 *                                 may be anything from zero bytes to (remaining_sz + cipher_block_size - 1).
 * @param out_sz <tt>size_t</tt> the number of bytes written to out_data.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDCipher_final(struct GDCipherContext* ctx, void* out_data, int* out_sz);

/** Encipher or decipher.
 * Perform encryption or decryption of a message.
 *
 * @param ctx <tt>GDCipherContext*</tt> a pointer to the cipher context.
 * @param out_data <tt>void*</tt> a buffer large enough to contain the encrypted or decrypted data. The amount of data written
 *                                may be anything from zero bytes to (in_sz + cipher_block_size - 1).
 * @param in_data <tt>const void*</tt> a pointer to the data to be encrypted or decrypted.
 * @param in_data_sz <tt>unsigned int</tt> the number of bytes of the data to be encrypted.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDCipher(struct GDCipherContext* ctx, void* out_data, const void* in_data, unsigned int in_data_sz);

/** Clears all information from a cipher context.
 *  Clears all information from a cipher context and frees up any allocated memory associate with it.
 *  It should be called after all operations using a cipher are complete so sensitive information does not remain
 *  in memory.
 *
 * @param ctx <tt>GDCipherContext*</tt> a pointer to the cipher context.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDCipher_cleanup(struct GDCipherContext* ctx);

/** Free the cipher context.
 * Deallocate cipher context resources.  It does not free the buffer holding the encrypted or decrypted data that the
 * caller supplied to GDCipher_final().
 *
 * @param ctx <tt>GDCipherContext*</tt> a pointer to the cipher context to free.
 */
GD_C_API void GDCipher_free(struct GDCipherContext* ctx);

/** Digest algorithm structure.
 * A structure representing a cryptographic digest algorithm.
 */
struct GD_C_API GDDigest;

/** Digest context structure.
 * A structure representing a cryptographic digest algorithm context.  A context is used for calculating the message digest (hash)
 * of a message.
 */
struct GD_C_API GDDigestContext;

/** Return a digest algorithm by name.
 * Returns a digest algorithm structure by name.  The name must be the long name format as specified in
 * obj_mac.h of OpenSSL.
 *
 * @param name <tt>char*</tt> the name of the digest required. For example, "sha256".
 * @return <tt>int</tt> the digest or NULL. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API const struct GDDigest* GDDigest_byname(const char *name);

/** Return a digest's name.
 * Returns the name of the given digest structure.  The name will be in long name format as specified in
 * obj_mac.h of OpenSSL.
 *
 * @param ctx <tt>GDDigestContext*</tt> the digest context for which the name is required.
 * @return <tt>char*</tt> the digest name or NULL.
 */
GD_C_API const char* GDDigest_name(const struct GDDigestContext* ctx);

/** Return a digest by context.
 * Returns a digest structure for the given digest context.
 *
 * @param ctx <tt>GDDigestContext*</tt> the context of the digest required.
 * @return <tt>GDDigest*</tt> the digest or NULL. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API const struct GDDigest* GDDigest_bycontext(const struct GDDigestContext* ctx);

/** Maximum supported digest size in bytes.
 *  Returns the maxmium supported digest size in bytes. As of early 2019, this value is 64, which is the
 *  size of a SHA-512 digest.
 */
GD_C_API extern const int GDDIGEST_MAX_SIZE;

/** Size of a digest in bytes.
 *  Returns the specified digest size in bytes.
 *
 * @param digest <tt>const GDDigest* digest</tt> the digest. For example, "sha256".
 * @return <tt>int</tt> the digest size.
 */
GD_C_API int GDDigest_size(const struct GDDigest* digest);

/** Return the digest block size.
 * Returns the block size of a given digest.
 *
 * @param digest <tt>GDCipher*</tt> the digest for which the block size is required.
 * @return <tt>int</tt> the block size of the digest in bytes.
 */
GD_C_API int GDDigest_block_size(const struct GDDigest* digest);

/** Return a new digest context.
 *  A digest context is used to calculate the digest (hash) of a message.
 *  The context should be initialized with a specific digest algorithm by calling GDDigest_init().
 *
 * @return <tt>GDDigestContext*</tt> a digest context.
 */
GD_C_API struct GDDigestContext* GDDigest_new(void);

/** Initialize a digest context.
 *  Initialize the digest context with a specific digest algorithm.
 *
 * @param ctx <tt>GDDigestContext*</tt> a pointer to the digest context.
 * @param algorithm <tt>const GDDigest*</tt> the digest algorithm, for example SHA-256.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDDigest_init(struct GDDigestContext* ctx, const struct GDDigest* algorithm);

/** Copy a digest context.
 *  Copies the digest context. The destination digest is cleaned up before being updated to match the
 *  source digest.
 *
 * @param source <tt>GDDigestContext*</tt> a pointer to a digest context.
 * @param destination <tt>const GDDigestContext*</tt> the digest context to copy.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDDigest_copy(struct GDDigestContext* destination, const struct GDDigestContext* source);

/** Calculate a digest.
 * Calculate the digest of a message. This function can be called multiple times to update
 * the digest with additional message data.
 *
 * @param ctx <tt>GDDigestContext*</tt> a pointer to the digest context.
 * @param message <tt>const void*</tt> a pointer to the message chunk.
 * @param message_sz <tt>size_t</tt> the number of bytes of the message chunk.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDDigest_update(struct GDDigestContext* ctx, const void* message, size_t message_sz);

/** Finalize and output the digest.
 * Return the final digest of the complete message.
 *
 * @param ctx <tt>GDDigestContext*</tt> a pointer to the digest context.
 * @param digest <tt>void*</tt> a pointer to memory large enough to contain the digest. The caller must allocate and eventually
 *                              free this buffer.  The size of the buffer required can be determined by calling GDDigest_size(),
 *                              or using GDDIGEST_MAX_SIZE.
 * @param digest_sz <tt>size_t</tt> the number of digest bytes copied to digest.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDDigest_final(struct GDDigestContext* ctx, void* digest, unsigned int* digest_sz);

/** Calculate a digest.
 * Calculate the digest of a message.
 *
 * @param data <tt>const void*</tt> a buffer containing the data to hash.
 * @param data_sz <tt>size_t</tt> the size of the buffer containing the data to hash.
 * @param md <tt>void*</tt> a buffer large enough to contain the digest.
 * @param md_sz <tt>size_t</tt> upon return, the number of bytes of the calculated digest.
 * @param type <tt>GDDigest*</tt> the digest required.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDDigest(const void* data, size_t data_sz, void* md, unsigned int* md_sz, const struct GDDigest* type);

/** Initialize a digest context for signing.
 * Initialize a digest context for a signing operation.
 *
 * @param ctx <tt>GDDigestContext*</tt> the digest context to initialize.
 * @param key_context <tt>GDKeyContext**</tt> if not NULL, upon return, a key context that can be used to set additional signing parameters. The key context will be freed automatically when the digest context is freed.
 * @param digest <tt>GDDigest*</tt> the digest algorithm to use when signing.
 * @param key <tt>GDKey*</tt> the signer's private key.
 * @return <tt>int</tt> 1 for success, 0 or a negative value for failure.
 */
GD_C_API int GDDigest_sign_init(struct GDDigestContext* ctx, struct GDKeyContext** key_context, const struct GDDigest* digest, struct GDKey* key);

/** Sign a digest.
 * Signs a message digest.
 *
 * @param ctx <tt>GDDigestContext*</tt> the initialized digest context to use when signing.
 * @param signature <tt>void*</tt> a buffer large enough to contain the signature. If NULL, the size required is returned in signature_sz.
 * @param signature_sz <tt>GDDigest*</tt> upon return, the size of the signature.
 * @return <tt>int</tt> 1 for success and 0 or a negative value for failure.
 */
GD_C_API int GDDigest_sign_final(struct GDDigestContext* ctx, void* signature, size_t* signature_sz);

/** Initialize a digest context for verification.
 * Initialize a digest context for a verification operation.
 *
 * @param ctx <tt>GDDigestContext*</tt> the digest context to initialize.
 * @param key_ctx <tt>GDKeyContext**</tt> if not NULL, upon return, a key context that can be used to set additional verification parameters. The key context will be freed automatically when the digest context is freed.
 * @param digest <tt>GDDigest*</tt> the digest algorithm to use when verifying.
 * @param key <tt>GDKey*</tt> the signer's public key.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDDigest_verify_init(struct GDDigestContext* ctx, struct GDKeyContext** key_ctx, const struct GDDigest* digest, struct GDKey* key);

/** Verify a digest.
 * Verifies a message digest.
 *
 * @param ctx <tt>GDDigestContext*</tt> the initialized digest context to use for verification.
 * @param signature <tt>void*</tt> a buffer containing a signature.
 * @param signature_sz <tt>GDDigest*</tt> the size of the signature buffer.
 * @return 1 for successful verification, 0 for verification failure (not an error - message or signature is not to be trusted), or a
 *         negative value for a more serious error. The reason for failure can be obtained from GDCryptoError_get().
 */
GD_C_API int GDDigest_verify_final(struct GDDigestContext* ctx, const void* signature, size_t signature_sz);

/** Clears all information from a digest context.
 *  Clears all information from a digest context and frees up any allocated memory associate with it.
 *  It should be called after all operations using a digest are complete so sensitive information does not remain
 *  in memory.
 *
 * @param ctx <tt>GDDigestContext*</tt> a pointer to the digest context.
 * @return <tt>int</tt> 1 for success, 0 for failure.
 */
GD_C_API int GDDigest_cleanup(struct GDDigestContext* ctx);

/** Free the digest context.
 * Deallocate digest context resources.  It does not free the digest buffer that the caller supplied to GDDigest_final().
 * @param ctx <tt>GDDigestContext*</tt> a pointer to the digest context to free.
 */
GD_C_API void GDDigest_free(struct GDDigestContext* ctx);

#ifdef __cplusplus
}
#endif

/** @}
 */
