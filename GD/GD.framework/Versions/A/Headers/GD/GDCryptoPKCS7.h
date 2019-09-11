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

#include "GDStream.h"
#include "GDCryptoKeyStore.h"
#include "GDCryptoAlgorithms.h"
#include "GDCryptoError.h"

/** PKCS7 structure.
 * A structure representing PKCS7 data.
 */
struct GD_C_API GDPKCS7;

/** PKCS7 flags.
 * PKCS7 flags to control operations on a PKCS7 structure.
 * These map directly to OpenSSL flags defined in pkcs7.h. Refer to OpenSSL documentation for
 * PKCS7 functions requiring a flag argument for more information.
 */
GD_C_API extern const int GDPKCS7_TEXT;
GD_C_API extern const int GDPKCS7_NOCERTS;
GD_C_API extern const int GDPKCS7_NOSIGS;
GD_C_API extern const int GDPKCS7_NOCHAIN;
GD_C_API extern const int GDPKCS7_NOINTERN;
GD_C_API extern const int GDPKCS7_NOVERIFY;
GD_C_API extern const int GDPKCS7_DETACHED;
GD_C_API extern const int GDPKCS7_BINARY;
GD_C_API extern const int GDPKCS7_NOATTR;
GD_C_API extern const int GDPKCS7_NOSMIMECAP;
GD_C_API extern const int GDPKCS7_NOOLDMIMETYPE;
GD_C_API extern const int GDPKCS7_CRLFEOL;
GD_C_API extern const int GDPKCS7_STREAM;
GD_C_API extern const int GDPKCS7_NOCRL;
GD_C_API extern const int GDPKCS7_PARTIAL;
GD_C_API extern const int GDPKCS7_REUSE_DIGEST;
GD_C_API extern const int GDPKCS7_USE_KEYID;
GD_C_API extern const int GDPKCS7_KEY_PARAM;

/** PKCS7 content-type:pkcs7-signedData.
 * If returned from GDPKCS7_type(), PKCS7 structure is signed data.
 */
GD_C_API extern const int GDPKCS7_SIGNED;

/** PKCS7 content-type:pkcs7-encryptedData.
 * If returned from GDPKCS7_type(), PKCS7 structure is encrypted data.
 */
GD_C_API extern const int GDPKCS7_ENCRYPTED;

/** PKCS7 content-type:pkcs7-envelopedData.
 * If returned from GDPKCS7_type(), PKCS7 structure is enveloped data.
 */
GD_C_API extern const int GDPKCS7_ENVELOPED;

/** PKCS7 content-type:pkcs7-signedAndEnvelopedData.
 * If returned from GDPKCS7_type(), PKCS7 structure is signed and enveloped data.
 */
GD_C_API extern const int GDPKCS7_SIGNED_AND_ENVELOPED;

/** PKCS7 content-type:pkcs7-data.
 * If returned from GDPKCS7_type(), PKCS7 structure is data type.
 */
GD_C_API extern const int GDPKCS7_DATA;

/** PKCS7 content-type:pkcs7-digestData.
 * If returned from GDPKCS7_type(), PKCS7 structure is digest data.
 */
GD_C_API extern const int GDPKCS7_DIGEST;

/** Deserialize SMIME to a PKCS7 structure.
 * Deserialize SMIME content received over a network, read from a file, and so on.
 *
 * @param stream <tt>GDStream*</tt> the stream to read the SMIME message from.
 * @param clearText <tt>GDStream**</tt> if cleartext signing is used then the content is returned in a memory stream,
 *                                      otherwise *clearText is set to NULL. The caller is responsible for freeing the stream.
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>GDPKCS7*</tt> a PKCS7 structure or NULL if an error occurred. The error can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDPKCS7* GDPKCS7_SMIME_read(struct GDStream* stream, struct GDStream** clearText, int flags);

/** Deserialize ASN.1/DER encoding to a PKCS7 structure.
 * Deserialize ASN.1/DER encoded PKCS7 content received over a network, read from a file, and so on.
 *
 * @param stream <tt>GDStream*</tt> the stream to read the ASN.1/DER encoded data from.
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>GDPKCS7*</tt> a PKCS7 structure or NULL if an error occurred. The error can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDPKCS7* GDPKCS7_read(struct GDStream* stream, int flags);

/** Retrieve the type of PKCS7 structure.
 * Retrieve the type of the PKCS7 structure.
 *
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 structure to verify. The PKCS7 structure may be obtained from an initial call to GDPKCS7_read(),
 *                              GDPKCS7_encrypt(), or GDPKCS7_add_signer().
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>int</tt> the type of the PKCS7 structure.
 */
GD_C_API int GDPKCS7_type(struct GDPKCS7 *p7, int flags);

/** Verify a PKCS7 signedData structure.
 * Verifies a PKCS7 signedData structure.
 *
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 structure to verify. The PKCS7 structure may be obtained from an initial call to GDPKCS7_read().
 * @param certs <tt>GDX509List*</tt> a set of certificates in which to search for the signer's certificate.
 * @param not_used <tt>void*</tt> optional additional set of certificates to include in the PKCS7 structure, for example, intermediate CAs.
 * @param indata <tt>GDStream*</tt> the signed data if the content is detached.
 * @param out <tt>GDStream*</tt> the content is written to out if it is not NULL.
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>int</tt> 1 for a successful verification and 0 if an error occurs. The error can be obtained from GDCryptoError_get().
 */
GD_C_API int GDPKCS7_verify(struct GDPKCS7 *p7, const struct GDX509List* certs, void* not_used, struct GDStream* indata, struct GDStream* out, int flags);

/** Retrieve the signer's certificate(s).
 * Retrieve the signer's certificate(s) from a PKCS7 signedData structure.
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 signedData. The PKCS7 structure may be obtained from an initial call to GDPKCS7_read().
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>GDX509List*</tt> a list of end-user certificates belonging to the signer(s), or NULL. If NULL, the reason can be obtained from GDCryptoError_get().
 */
GD_C_API const struct GDX509List* GDPKCS7_get_signers(struct GDPKCS7* p7, int flags);

/** Decrypt a PKCS7 envelopedData structure.
 * Decrypts content within a PKCS7 envelopedData structure.
 *
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 envelopedData structure to decrypt.
 * @param pkey <tt>GDKey*</tt> private key of the recipient.
 * @param cert <tt>GDX509*</tt> the recipients certificate.
 * @param data <tt>GDStream*</tt> the decrypted content is written to data.
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>int</tt> either 1 for success or 0 for failure. The error can be obtained from GDCryptoError_get().
 */
GD_C_API int GDPKCS7_decrypt(struct GDPKCS7* p7, struct GDKey* pkey, const struct GDX509* cert, struct GDStream* data, int flags);

/** Create a PKCS7 envelopedData structure.
 * Create and return an encrypted PKCS7 envelopedData structure.
 *
 * @param certs <tt>GDX509List*</tt> a list of recipient certificates.
 * @param in <tt>GDStream*</tt> the content to be encrypted.
 * @param cipher <tt>GDCipher*</tt> the symmetric cipher to use.
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>GDPKCS7*</tt> a PKCS7 structure or NULL if an error occurred. The error can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDPKCS7* GDPKCS7_encrypt(struct GDX509List* certs, struct GDStream* in, const struct GDCipher* cipher, int flags);

/** Add signer information to a PKCS7 structure.
 * Add signer information and return a PKCS7 signedData structure.
 *
 * @param signcert <tt>GDX509*</tt> the signing certificate.
 * @param pkey <tt>GDKey*</tt> the corresponsding private key of the signing certificate.
 * @param certs <tt>GDX509List*</tt> optional additional set of certificates to include in the PKCS7 structure, for example, intermediate CAs.
 * @param md <tt>GDDigest*</tt> the message digest to use.
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>GDPKCS7*</tt> a PKCS7 structure or NULL if an error occurred. The error can be obtained from GDCryptoError_get().
 */
GD_C_API struct GDPKCS7* GDPKCS7_add_signer(const struct GDX509* signcert, struct GDKey* pkey, struct GDX509List* certs, const struct GDDigest* md, int flags);

/** Finalize a PKCS7 structure.
 * Finalize a PKCS7 structure, readying it for serialization.  This may trigger computation of the actual PKCS7 data, such as content signing.
 *
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 structure to finalize.
 * @param data <tt>GDStream*</tt> the content to be finalized.
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>int</tt> either 1 for success or 0 for failure. The error can be obtained from GDCryptoError_get().
 */
GD_C_API int GDPKCS7_final(struct GDPKCS7* p7, struct GDStream* data, int flags);

/** Serialize a PKCS7 structure to ASN.1/DER encoding.
 *
 * Serialize a PKCS7 structure to ASN.1/DER encoding suitable for sending over the network, writing to a file, and so on.
 *
 * @param stream <tt>GDStream*</tt> the stream to write the ASN.1/DER encoded data to.
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 structure to serialize. The PKCS7 structure may be obtained from an initial call to GDPKCS7_add_signer() or GDPKCS7_encrypt().
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>int</tt> either 1 for success or 0 for failure. The error can be obtained from GDCryptoError_get().
 */
GD_C_API int GDPKCS7_write(struct GDStream* stream, struct GDPKCS7* p7, int flags);

/** Serialize a PKCS7 structure to SMIME encoding.
 *
 * Serialize a PKCS7 structure to SMIME encoding suitable for sending over the network, writing to a file, and so on.
 *
 * @param stream <tt>GDStream*</tt> the stream to write the SMIME message to.
 * @param data <tt>GDStream*</tt> the content to read from if streaming is enabled.
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 structure to serialize. The PKCS7 structure may be obtained from an initial call to GDPKCS7_add_signer() or GDPKCS7_encrypt().
 * @param flags <tt>int</tt> optional set of flags.
 * @return <tt>int</tt> either 1 for success or 0 for failure. The error can be obtained from GDCryptoError_get().
 */
GD_C_API int GDPKCS7_SMIME_write(struct GDStream* stream, struct GDPKCS7* p7, struct GDStream* data, int flags);

/** Free a PKCS7 structure.
 * Deallocates resources held by the PKCS7 structure.
 *
 * @param p7 <tt>GDPKCS7*</tt> the PKCS7 structure to free.
 * @param flags <tt>int</tt> optional set of flags.
 */
GD_C_API void GDPKCS7_free(struct GDPKCS7* p7, int flags);

#ifdef __cplusplus
}
#endif

/** @}
 */
