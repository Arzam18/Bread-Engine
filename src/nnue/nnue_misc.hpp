#pragma once
#include <cstdint>
#include <cassert>
#include "simd.hpp"

#if defined(USE_SSE2NEON) || defined(USE_SSE)
    [[maybe_unused]]
    inline void crelu32_to_16(int32_t *input, int16_t *output, int size){
        assert(size % INT16_PER_REG == 0);
        const int num_regs = size / INT16_PER_REG;
        const vec_int16 zero = setzero_epi16();
        const vec_int16 qscale = set1_epi16(255);

        for (int i = 0; i < num_regs; i++){
            vec_int32 in_1 = load_epi32(&input[(2*i)*INT32_PER_REG]);
            vec_int32 in_2 = load_epi32(&input[(2*i+1)*INT32_PER_REG]);
            vec_int16 out = packs_epi32(in_1, in_2);
            out = min_epi16(qscale, max_epi16(out, zero));
            store_epi16(&output[i*INT16_PER_REG], out);
        }
    }

    [[maybe_unused]]
    inline void crelu16_to_8(int16_t *input, uint8_t *output, int size){
        assert(size % INT8_PER_REG == 0);
        const int num_regs = size / INT8_PER_REG;

        for (int i = 0; i < num_regs; i++){
            vec_int16 in_1 = load_epi16(&input[(2*i)*INT16_PER_REG]);
            vec_int16 in_2 = load_epi16(&input[(2*i+1)*INT16_PER_REG]);
            vec_int8 out = packus_epi16(in_1, in_2);
            store_epi8(&output[i*INT8_PER_REG], out);
        }
    }

    [[maybe_unused]]
    inline void pairwise_screlu16_to_8(int16_t *in, int16_t *in_pair, uint8_t *output, int size){
        assert(size % (2*INT16_PER_REG) == 0);
        const vec_int16 zero = setzero_epi16();
        const vec_int16 qscale = set1_epi16(255);

        for (int i = 0; i < size; i += 2*INT16_PER_REG){
            vec_int16 in_1 = load_epi16(&in[i]);
            vec_int16 in_1_pair = load_epi16(&in_pair[i]);
            vec_int16 in_2 = load_epi16(&in[i + INT16_PER_REG]);
            vec_int16 in_2_pair = load_epi16(&in_pair[i + INT16_PER_REG]);

            in_1 = min_epi16(qscale, max_epi16(in_1, zero));
            in_1_pair = min_epi16(qscale, in_1_pair);
            in_2 = min_epi16(qscale, max_epi16(in_2, zero));
            in_2_pair = min_epi16(qscale, in_2_pair);

            vec_int8 out = packus_epi16(
                mulhi_epi16(slli_epi16(in_1, 16 - 9), in_1_pair),
                mulhi_epi16(slli_epi16(in_2, 16 - 9), in_2_pair)
            );
            store_epi8(&output[i], out);
        }
    }

    inline int32_t reduce1_epi32(vec_int32 input){
        alignas(16) int32_t out_ptr[4];
        store_epi32(out_ptr, input);
        return out_ptr[0] + out_ptr[1] + out_ptr[2] + out_ptr[3];
    }
#endif
