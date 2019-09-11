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

/** A data stream structure.
 * A structure representing a stream of data.
 */
struct GD_C_API GDStream;

/** A stream storage method.
 * A structure representing a storage method to be used by a steam.  For example, a memory stream.
 */
struct GD_C_API GDStreamStorageMethod;

/** Retrieve a memory stream storage method.
 * Retrieve a memory stream storage method required to back a stream object.
 *
 * @return <tt>GDStream_Method*</tt> The stream storage method. If NULL, the storage method is not implemented.
 */
GD_C_API const struct GDStreamStorageMethod* GDStream_mem_storage_method(void);

/** Create a stream from a region of memory.
 * Creates a read-only stream from a small region of memory.
 *
 * @param buf <tt>const void*</tt> a pointer to the beginning of the data.
 * @param len <tt>int</tt> the length of the data, or -1 if the data is null-terminated.
 * @return <tt>GDStream*</tt> A read-only memory stream. If NULL, the reason can be obtained from GDCryptoError_get.
 */
GD_C_API struct GDStream* GDStream_new_mem_buf(const void *buf, int len);

/** Create a stream.
 * Creates a stream bakced by the specified storage method. For example, a memory stream.
 *
 * @param method <tt>GDStreamStorageMethod*</tt> the storage method backing the stream. For example, a memory stream  from GDStream_mem_storage_method().
 * @return <tt>GDStream*</tt> A stream of the required storage method. If NULL, the reason can be obtained from GDCryptoError_get.
 */
GD_C_API struct GDStream* GDStream_new(const struct GDStreamStorageMethod* method);

/** Read from a stream.
 * Read data from a stream and place it into the supplied a buffer.
 *
 * @param stream <tt>GDStream*</tt> the stream to read from.
 * @param data <tt>void*</tt> a pointer to the buffer to place the data into.
 * @param len <tt>len</tt> length of the buffer.
 * @return <tt>int</tt> The number of bytes read. If -1, the reason can be obtained from GDCryptoError_get.
 */
GD_C_API int GDStream_read(struct GDStream* stream, void* data, int len);

/** Write to a stream.
 * Write data from the supplied buffer, into the stream.
 *
 * @param stream <tt>GDStream*</tt> the stream to write to.
 * @param data <tt>void*</tt> a pointer to the buffer to read the data from.
 * @param len <tt>len</tt> length of the buffer.
 * @return <tt>int</tt> The number of bytes written. If -1, the reason can be obtained from GDCryptoError_get.
 */
GD_C_API int GDStream_write(struct GDStream* stream, const void* data, int len);

/** Reset a stream.
 * Resets the stream to its initial state by rewinding the  pointer to the start of the stream.
 *
 * @param stream <tt>GDStream*</tt> the stream to reset.
 * @return <tt>int</tt> 1 for success.  If not 1, the error can be obtained from GDCryptoError_get.
 */
GD_C_API int GDStream_reset(struct GDStream* stream);

/** Tells if stream has reached the end.
 * Tells if the stream has been completely read or not.
 *
 * @param stream <tt>GDStream*</tt> the stream to test.
 * @return <tt>int</tt> 1 if EOF has been reached 0 otherwise.
 */
GD_C_API int GDStream_eof(struct GDStream* stream);

/** Free a stream structure.
 * Deallocates resources held by the stream structure.
 *
 * @param stream <tt>GDStream*</tt> the stream structure to free.
 */
GD_C_API void GDStream_free(struct GDStream* stream);

#ifdef __cplusplus
}
#endif

/** @}
 */
