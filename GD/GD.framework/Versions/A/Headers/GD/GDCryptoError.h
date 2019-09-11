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

/** Retrieve the earliest error code.
 * Returns the earliest error code from the thread's error stack and removes the entry.
 * This function can be called repeatedly until there are no more error codes to return.
 *
 * @return <tt>unsigned long</tt> an error code, or 0 if there is no error in the queue.
 */
GD_C_API unsigned long GDCryptoError_get(void);

/** Read the earliest error code.
 * Returns the earliest error code without modifying the error stack for this thread.
 *
 * @return <tt>unsigned long</tt> The error code, or 0 if there is no error in the queue.
 */
GD_C_API unsigned long GDCryptoError_peek(void);

/** Read the latest error code.
 * Returns the latest error code without modifying the error stack for this thread.
 *
 * @return <tt>unsigned long</tt> The error code, or 0 if there is no error in the queue.
 */
GD_C_API unsigned long GDCryptoError_peek_last(void);

/** Empties the error stack.
 * Clear the thread's error stack.
 */
GD_C_API void GDCryptoError_clear(void);

/** Return a human-readable string representing the error code.
 * Generates a human-readable string representing the error code.
 *
 * @param e <tt>unsigned long</tt> the error code.
 * @return <tt>char*</tt> a pointer to a static buffer containing the string.
 */
GD_C_API const char* GDCryptoError_string(unsigned long e);

#ifdef __cplusplus
}
#endif

/** @}
 */
