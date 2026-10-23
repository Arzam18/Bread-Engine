# Android ARM64 builds

The GitHub Actions workflow builds two Android `arm64-v8a` UCI binaries:

- `bread-neon-arm64`: ARMv8-A + NEON baseline.
- `bread-dotprod-arm64`: ARMv8.2-A + dot-product target for compatible CPUs.

Bread's NNUE implementation uses x86 intrinsic APIs. On AArch64, the relevant AVX2/SSE intrinsic compatibility API is supplied by SIMDe, downloaded during the workflow. The dotprod build is compiled with `-march=armv8.2-a+dotprod`; that target flag permits the compiler to use dot-product instructions where the translated operations allow it, but does not prove that a particular hot loop emits `SDOT`.

The workflow configures Java 17 before Android SDK setup to avoid `sdkmanager` failures with unsupported Java runtimes, and asks `setup-android` to install NDK 27.2.12479018 rather than calling `sdkmanager` a second time manually.

**Compatibility:** use the NEON binary on general 64-bit ARM Android devices. Use the dotprod binary only on CPUs advertising the dot-product feature (Linux feature name `asimddp`). The dotprod binary may not run on older NEON-only devices.

Build target API: Android 24. The existing CMake configuration embeds the NNUE model using its existing `INCBIN` mechanism.

**Validation status:** the workflow and source changes have not yet been confirmed by a successful remote GitHub Actions build. Treat the first run's compiler output as the final check for any remaining intrinsic compatibility issues.


### CMake source-list parsing fix

The `bread_configure()` helper explicitly strips the literal `SOURCES` keyword from its argument list and fails with a clear diagnostic if the list is empty. This addresses the reported `No SOURCES given to target: bread_engine` CMake generation error.


### Clang NEON variable-shift fix

Bread's NNUE SIMD wrappers passed runtime `int` shift counts to AVX2 immediate-shift intrinsics (`_mm256_slli_epi16` and `_mm256_srai_epi32`). Clang's AArch64 NEON intrinsics require constant immediates, so SIMDe could not compile those calls. The wrappers now use the AVX2 variable-count forms (`_mm256_sll_epi16` and `_mm256_sra_epi32`) with the count placed in an XMM vector. This retains per-lane shift behavior while allowing runtime counts.
