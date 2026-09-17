#include <cstdint>
#include <bit>
#include "simd.hpp"
#include "nnue.misc.hpp"

constexpr int L1_INPUT_SIZE = 1536;
extern uint16_t nnz_lookup[256][8];

void run_L1_sparse(uint8_t* input, int32_t* output, int bucket) {
    constexpr int MAX_NNZ_INPUTS = L1_INPUT_SIZE / 4;
    int16_t nnz_indices[MAX_NNZ_INPUTS];
    int num_nnz_inputs = 0;

    const int BLOCKS_PER_REG = INT8_PER_REG / 4;
    __m128i offset = _mm_set1_epi16(0);
    __m128i stride = _mm_set1_epi16(BLOCKS_PER_REG);

    // Get non-zero indices
    for (int i = 0; i < L1_INPUT_SIZE; i += INT8_PER_REG) {
        vec_int32 input_chunk = load_epi32(reinterpret_cast<int32_t*>(&input[i]));
        auto nnz_bitmask = nonzero_mask_epi32(input_chunk);

        for (size_t group_idx = 0; group_idx < 8 * sizeof(nnz_bitmask); group_idx += 8) {
            uint8_t group = (nnz_bitmask >> group_idx) & 0xFF;

            __m128i indexes = _mm_loadu_si128((__m128i*)nnz_lookup[group]);
            _mm_storeu_si128((__m128i*)(&nnz_indices[num_nnz_inputs]), _mm_add_epi16(offset, indexes));
            num_nnz_inputs += std::popcount(group);
            offset = _mm_add_epi16(offset, stride);
        }
    }
}
