#include <stdlib.h>

#include "binio.h"
#include "private.h"
#include "unpack_private.h"

void unpackf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfunpackf(stdin, format, args);
    va_end(args);
}

void sunpackf(const void *buffer, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsunpackf(buffer, format, args);
    va_end(args);
}

void funpackf(FILE *file, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfunpackf(file, format, args);
    va_end(args);
}

void vsunpackf(const void *buffer, const char *format, va_list args)
{
    struct BinIOFormatCursor cursor;
    BinIOInitFormatCursor(&cursor, format);
    
    struct BinIOUnpackContext context;
    context.data = buffer;
    context.args = args;
    
    while (BinIONextChar(&context, &cursor, BinIOUnpack)) {}
}

void vfunpackf(FILE *file, const char *format, va_list args)
{
    size_t n_bytes = BinIOFormatByteCount(format);
    void* buffer = malloc(n_bytes);
    fread(buffer, n_bytes, 1, file);
    
    vsunpackf(buffer, format, args);
    
    free(buffer);
}
