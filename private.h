#ifndef private_h
#define private_h

#include <stdarg.h>
#include <stddef.h>

#define BinIO_TYPE_IGNORE_BYTE         'x'
#define BinIO_TYPE_BYTE                'b'
#define BinIO_TYPE_INT16               's'
#define BinIO_TYPE_INT32               'i'
#define BinIO_TYPE_INT64               'l'
#define BinIO_TYPE_FLOAT32             'f'
#define BinIO_TYPE_FLOAT64             'd'

#define BinIO_LITTLE_ENDIAN_BYTE_ORDER 'L'
#define BinIO_BIG_ENDIAN_BYTE_ORDER    'B'
#define BinIO_HOST_BYTE_ORDER          'H'
#define BinIO_NETWORK_BYTE_ORDER       'N'

#if defined(BinIO_STDINT_HEADER)
    #include BinIO_STDINT_HEADER
#else
    #warning no stdint header specified; making assumptions
    typedef unsigned char      uint8_t;
    typedef unsigned short     uint16_t;
    typedef unsigned int       uint32_t;
    typedef unsigned long long uint64_t;
#endif

typedef float              float32_t;
typedef double             float64_t;

#if defined(__cplusplus)
    #define BinIO_INLINE inline
#elif defined(__STDC_VERSION__)
    // is this OK?  Do we need to require C94?  C99?
    #define BinIO_INLINE static inline
#elif defined(__GNUC__)
    #define BinIO_INLINE static inline
#else
    #warning unknown compiler; not inlining functions
    #define BinIO_INLINE static
#endif

#ifndef BinIO_BYTE_ORDER
    #if defined(__ppc__) || defined(__POWERPC__)
        #define BinIO_BYTE_ORDER BinIO_BIG_ENDIAN_BYTE_ORDER
    #elif defined(__i386__)
        #define BinIO_BYTE_ORDER BinIO_LITTLE_ENDIAN_BYTE_ORDER
    #else
        #warning unknown architecture; making assumptions
        #define BinIO_BYTE_ORDER BinIO_LITTLE_ENDIAN_BYTE_ORDER
    #endif
#endif

BinIO_INLINE void BinIOSwap1(const uint8_t *src, uint8_t *dst)
{
    *dst = *src;
}

BinIO_INLINE void BinIOSwap2(const uint8_t *src, uint8_t *dst)
{
    *(dst + 1) = *(src + 0);
    *(dst + 0) = *(src + 1);
}

BinIO_INLINE void BinIOSwap4(const uint8_t *src, uint8_t *dst)
{
    *(dst + 3) = *(src + 0);
    *(dst + 2) = *(src + 1);
    *(dst + 1) = *(src + 2);
    *(dst + 0) = *(src + 3);
}

BinIO_INLINE void BinIOSwap8(const uint8_t *src, uint8_t *dst)
{
    *(dst + 7) = *(src + 0);
    *(dst + 6) = *(src + 1);
    *(dst + 5) = *(src + 2);
    *(dst + 4) = *(src + 3);
    *(dst + 3) = *(src + 4);
    *(dst + 2) = *(src + 5);
    *(dst + 1) = *(src + 6);
    *(dst + 0) = *(src + 7);
}

BinIO_INLINE int BinIONormalizeByteOrder(int byte_order)
{
    if (byte_order == BinIO_HOST_BYTE_ORDER)
    {
        byte_order = BinIO_BYTE_ORDER;
    }
    else if (byte_order == BinIO_NETWORK_BYTE_ORDER)
    {
        byte_order = BinIO_BIG_ENDIAN_BYTE_ORDER;
    }
    
    return byte_order;
}

extern void BinIOConvert1(int from_byte_order, int to_byte_order,
                          const uint8_t *src, uint8_t *dst,
                          unsigned int count);
extern void BinIOConvert2(int from_byte_order, int to_byte_order,
                          const uint8_t *src, uint8_t *dst,
                          unsigned int count);
extern void BinIOConvert4(int from_byte_order, int to_byte_order,
                          const uint8_t *src, uint8_t *dst,
                          unsigned int count);
extern void BinIOConvert8(int from_byte_order, int to_byte_order,
                          const uint8_t *src, uint8_t *dst,
                          unsigned int count);

struct BinIOFormatCursor
{
    const char *cursor;
    int         byte_order;
    int         count;
};

typedef void (*BinIOProcessFunction)(void *context,
                                     int   type,
                                     int   byte_order,
                                     int   count);

extern void BinIOInitFormatCursor(struct BinIOFormatCursor *cursor,
                                  const char               *format);

extern int BinIONextChar(void                     *context,
                         struct BinIOFormatCursor *cursor,
                         BinIOProcessFunction      func);

extern void BinIOCountBytes(void *context, int type, int byte_order, int count);

extern size_t BinIOFormatByteCount(const char *format);

#endif
