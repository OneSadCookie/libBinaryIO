#ifndef binio_h
#define binio_h

#include <stdarg.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
   Notes on format of format strings:
     * whitespace is ignored
     * each "group" consists of an optional count (defaults to 1),
                                an optional byte-order marker (defaults to H, "host-native"),
                            and a  data-type specifier.
     * when unpacking, each variable argument is a pointer to the
       appropriate number of objects of the appropriate type.
     * when packing, each variable argument is an object of the
       appropriate type if the count is omitted, or a pointer to the
       appropriate number of objects of the appropriate type if the
       count is specified.
     * the buffer supplied to pack/unpack must be of sufficient length
       to hold all the data, or the behavior is unspecified.
     * the file provided to the "f" versions of the functions must be
       open in the appropriate mode, or the behavior is unspecified.
     * the file supplied to funpackf must be of sufficient length to
       hold all the data, or the behavior is unspecified.
     * the behavior of all functions is unspecified if the format string
       is incorrectly-formed.
   
   Data-type specifiers:
     x skipped byte; no corresponding argument
     b byte
     s two-byte two's-complement integer
     i four-byte two's-complement integer
     l eight-byte two's-complement integer
     f four-byte IEEE754 float
     d eight-byte IEEE754 double
   
   Byte-order specifiers:
     L little-endian
     B big-endian
     H host's native byte order
     N network byte order
 */

extern void packf    (                    const char *format, ...);
extern void spackf   (void *buffer,       const char *format, ...);
extern void fpackf   (FILE *file,         const char *format, ...);
extern void vspackf  (void *buffer,       const char *format, va_list args);
extern void vfpackf  (FILE *file,         const char *format, va_list args);

extern void unpackf  (                    const char *format, ...);
extern void sunpackf (const void *buffer, const char *format, ...);
extern void funpackf (FILE       *file,   const char *format, ...);
extern void vsunpackf(const void *buffer, const char *format, va_list args);
extern void vfunpackf(FILE       *file,   const char *format, va_list args);

#if defined(__cplusplus)
}
#endif

#endif
