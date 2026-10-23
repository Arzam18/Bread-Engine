# Android ARM64 builds

GitHub Actions (`.github/workflows/android.yml`) builds two Android `arm64-v8a` UCI binaries:

- `bread-neon-arm64`: ARMv8-A + NEON baseline.
- `bread-dotprod-arm64`: ARMv8.2-A + dot-product target for compatible CPUs.

The original NNUE SIMD implementation is written using AVX2 intrinsics and did not include ARM NEON or ARM dotprod paths. `src/nnue/simd.hpp` now uses SIMDe's AVX2 compatibility API on AArch64. SIMDe is fetched by the workflow and maps supported operations to the target's native SIMD implementation. The dotprod artifact may use dot-product instructions when the compiler/SIMDe implementation can map the relevant operations; it is not a separate hand-written NNUE dotprod kernel.

**Compatibility:** run the NEON artifact on general 64-bit ARM Android devices. Run the dotprod artifact only on CPUs advertising `asimddp` / `dotprod` (for example, ARMv8.2-A CPUs with that extension). The dotprod binary is not expected to run on older NEON-only devices.

Builds target Android API 24 and use the bundled NNUE model via the existing CMake `INCBIN` mechanism.
