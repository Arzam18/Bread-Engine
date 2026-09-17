#pragma once

#if defined(__AVX512F__)
    #define USE_AVX512
    #define HAS_VNNI512 __AVX512VNNI__
    #include <immintrin.h>
#elif defined(__AVX2__)
    #define USE_AVX2
    #include <immintrin.h>
#elif defined(__ARM_NEON) || defined(__aarch64__) || defined(_M_ARM64)
    #define USE_SSE2NEON
    #include "extern/sse2neon.h"
#elif defined(__SSE2__)
    #define USE_SSE
    #include <immintrin.h>
#else
    #error "bread requires AVX512, AVX2, or ARM NEON (via sse2neon.h) to run."
#endif

#if defined(USE_SSE2NEON) || defined(USE_SSE)
    using vec_int8 = __m128i;
    using vec_uint8 = __m128i;
    using vec_int16 = __m128i;
    using vec_uint16 = __m128i;
    using vec_int32 = __m128i;
    using vec_uint32 = __m128i;

    inline vec_int8 setzero_epi8() { return _mm_setzero_si128(); }
    inline vec_int16 setzero_epi16() { return _mm_setzero_si128(); }
    inline vec_int32 setzero_epi32() { return _mm_setzero_si128(); }

    inline vec_int16 set1_epi16(int i) { return _mm_set1_epi16(i); }
    inline vec_int32 set1_epi32(int i) { return _mm_set1_epi32(i); }

    inline vec_int8 load_epi8(int8_t* ptr) { return _mm_loadu_si128((const __m128i*)ptr); }
    inline vec_int8 load_epi8(uint8_t* ptr) { return _mm_loadu_si128((const __m128i*)ptr); }
    inline vec_int16 load_epi16(int16_t* ptr) { return _mm_loadu_si128((const __m128i*)ptr); }
    inline vec_int16 load_epi16(uint16_t* ptr) { return _mm_loadu_si128((const __m128i*)ptr); }
    inline vec_int32 load_epi32(int32_t* ptr) { return _mm_loadu_si128((const __m128i*)ptr); }

    inline void store_epi8(int8_t* ptr, vec_int8 v) { _mm_storeu_si128((__m128i*)ptr, v); }
    inline void store_epi8(uint8_t* ptr, vec_int8 v) { _mm_storeu_si128((__m128i*)ptr, v); }
    inline void store_epi16(int16_t* ptr, vec_int16 v) { _mm_storeu_si128((__m128i*)ptr, v); }
    inline void store_epi32(int32_t* ptr, vec_int32 v) { _mm_storeu_si128((__m128i*)ptr, v); }

    inline vec_int8 packs_epi16(vec_int16 v1, vec_int16 v2) { return _mm_packs_epi16(v1, v2); }
    inline vec_int16 packs_epi32(vec_int32 v1, vec_int32 v2) { return _mm_packs_epi32(v1, v2); }
    inline vec_int8 packus_epi16(vec_int16 v1, vec_int16 v2) { return _mm_packus_epi16(v1, v2); }
    inline vec_int16 packus_epi32(vec_int32 v1, vec_int32 v2) { return _mm_packus_epi32(v1, v2); }

    inline vec_int8 max_epi8(vec_int8 v1, vec_int8 v2) { return _mm_max_epi8(v1, v2); }
    inline vec_int8 min_epi8(vec_int8 v1, vec_int8 v2) { return _mm_min_epi8(v1, v2); }
    inline vec_int16 max_epi16(vec_int16 v1, vec_int16 v2) { return _mm_max_epi16(v1, v2); }
    inline vec_int16 min_epi16(vec_int16 v1, vec_int16 v2) { return _mm_min_epi16(v1, v2); }

    inline vec_int16 add_epi16(vec_int16 v1, vec_int16 v2) { return _mm_add_epi16(v1, v2); }
    inline vec_int32 add_epi32(vec_int32 v1, vec_int32 v2) { return _mm_add_epi32(v1, v2); }
    inline vec_int16 sub_epi16(vec_int16 v1, vec_int16 v2) { return _mm_sub_epi16(v1, v2); }

    inline vec_int32 madd_epi16(vec_int16 v1, vec_int16 v2) { return _mm_madd_epi16(v1, v2); }
    inline vec_int16 mullo_epi16(vec_int16 v1, vec_int16 v2) { return _mm_mullo_epi16(v1, v2); }
    inline vec_int16 mulhi_epi16(vec_int16 v1, vec_int16 v2) { return _mm_mulhi_epi16(v1, v2); }
    inline vec_int16 maddubs_epi16(vec_int8 v1, vec_int8 v2) { return _mm_maddubs_epi16(v1, v2); }

    inline vec_int32 slli_epi16(vec_int32 v, int i) { return _mm_slli_epi16(v, i); }
    inline vec_int32 srai_epi32(vec_int32 v, int i) { return _mm_srai_epi32(v, i); }

    inline vec_int32 dpbusd_epi32(vec_int32 sum, vec_int8 v1, vec_int8 v2) {
        const vec_int16 prod = maddubs_epi16(v1, v2);
        return add_epi32(sum, madd_epi16(prod, set1_epi16(1)));
    }

    inline uint8_t nonzero_mask_epi32(vec_int32 v) {
        uint8_t z_bitmask = _mm_movemask_ps((__m128)_mm_cmpeq_epi32(v, _mm_setzero_si128()));
        return (~z_bitmask) & 0x0F;
    }

    constexpr int NUM_AVX_REGISTERS = 8;
    constexpr int INT32_PER_REG = sizeof(vec_int32) / sizeof(int32_t);
    constexpr int INT16_PER_REG = sizeof(vec_int16) / sizeof(int16_t);
    constexpr int INT8_PER_REG = sizeof(vec_int8) / sizeof(int8_t);
#elif defined(USE_AVX2)
    using vec_int8 = __m256i;
    using vec_uint8 = __m256i;
    using vec_int16 = __m256i;
    using vec_uint16 = __m256i;
    using vec_int32 = __m256i;
    using vec_uint32 = __m256i;

    inline vec_int8 setzero_epi8() { return _mm256_setzero_si256(); }
    inline vec_int16 setzero_epi16() { return _mm256_setzero_si256(); }
    inline vec_int32 setzero_epi32() { return _mm256_setzero_si256(); }

    inline vec_int16 set1_epi16(int i) { return _mm256_set1_epi16(i); }
    inline vec_int32 set1_epi32(int i) { return _mm256_set1_epi32(i); }

    inline vec_int8 load_epi8(int8_t* ptr) { return _mm256_loadu_si256((const __m256i*)ptr); }
    inline vec_int8 load_epi8(uint8_t* ptr) { return _mm256_loadu_si256((const __m256i*)ptr); }
    inline vec_int16 load_epi16(int16_t* ptr) { return _mm256_loadu_si256((const __m256i*)ptr); }
    inline vec_int16 load_epi16(uint16_t* ptr) { return _mm256_loadu_si256((const __m256i*)ptr); }
    inline vec_int32 load_epi32(int32_t* ptr) { return _mm256_loadu_si256((const __m256i*)ptr); }

    inline void store_epi8(int8_t* ptr, vec_int8 v) { _mm256_storeu_si256((__m256i*)ptr, v); }
    inline void store_epi8(uint8_t* ptr, vec_int8 v) { _mm256_storeu_si256((__m256i*)ptr, v); }
    inline void store_epi16(int16_t* ptr, vec_int16 v) { _mm256_storeu_si256((__m256i*)ptr, v); }
    inline void store_epi32(int32_t* ptr, vec_int32 v) { _mm256_storeu_si256((__m256i*)ptr, v); }

    constexpr int NUM_AVX_REGISTERS = 16;
    constexpr int INT32_PER_REG = sizeof(vec_int32) / sizeof(int32_t);
    constexpr int INT16_PER_REG = sizeof(vec_int16) / sizeof(int16_t);
    constexpr int INT8_PER_REG = sizeof(vec_int8) / sizeof(int8_t);
#endif
