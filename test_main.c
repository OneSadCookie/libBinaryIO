#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <binio.h>

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

typedef struct
{
    float64_t d;
    uint64_t  l;
    uint32_t  i;
    float32_t f;
    uint16_t  s;
    uint8_t   b;
}
test_data;

uint8_t big_endian_bytes[28] =
{
    0x40, 0x09, 0x21, 0xFB, 0x54, 0x52, 0x45, 0x50,
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x89, 0xAB, 0xCD, 0xEF,
    0x40, 0x49, 0x0F, 0xDB,
    0xAB, 0xCD,
    0x00,
    0xAB
};

uint8_t little_endian_bytes[28] =
{
    0x50, 0x45, 0x52, 0x54, 0xFB, 0x21, 0x09, 0x40,
    0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
    0xEF, 0xCD, 0xAB, 0x89,
    0xDB, 0x0F, 0x49, 0x40,
    0xCD, 0xAB,
    0x00,
    0xAB
};

uint8_t buffer[28];

#define assert_floats_close(f, g)                                \
{                                                                \
    if (fabs(f - g) > 0.0000001)                                 \
    {                                                            \
        fprintf(stderr, "failed assertion `%f ~== %f'\n", f, g); \
        abort();                                                 \
    }                                                            \
}

#define check_data_ok(data)                 \
{                                           \
    assert_floats_close(data.d, 3.1415927); \
    assert(data.l == 0xfedcba9876543210ll); \
    assert(data.i == 0x89abcdef);           \
    assert_floats_close(data.f, 3.1415927); \
    assert(data.s == 0xabcd);               \
    assert(data.b == 0xab);                 \
}

int main(int argc, const char *argv[])
{
    test_data data;
    
    memset(&data, 0, sizeof(data));
    sunpackf(big_endian_bytes, "Bd Bl Bi Bf Bs x Bb", &(data.d), &(data.l), &(data.i), &(data.f), &(data.s), &(data.b));
    check_data_ok(data);
    
    spackf(buffer, "dlifsxb", data.d, data.l, data.i, data.f, data.s, data.b);
    sunpackf(buffer, "dlifsxb", &(data.d), &(data.l), &(data.i), &(data.f), &(data.s), &(data.b));
    check_data_ok(data);
    
    memset(&data, 0, sizeof(data));
    sunpackf(little_endian_bytes, "Ld Ll Li Lf Ls x Lb", &(data.d), &(data.l), &(data.i), &(data.f), &(data.s), &(data.b));
    check_data_ok(data);
    
    FILE *file = fopen("testdata.hex", "w+");
    fpackf(file, "Nd Nl Ni Nf Ns x Nb", data.d, data.l, data.i, data.f, data.s, data.b);
    memset(&data, 0, sizeof(data));
    fseek(file, 0, SEEK_SET);
    funpackf(file, "Bd Bl Bi Bf Bs x Bb", &(data.d), &(data.l), &(data.i), &(data.f), &(data.s), &(data.b));
    check_data_ok(data);
    fclose(file);
    
    memset(&data, 0, sizeof(data));
    sunpackf(little_endian_bytes, "27x b", &(data.b));
    assert(data.b == 0xab);
    
    return EXIT_SUCCESS;
}
