# Android ARM64 builds

The manually triggered GitHub Actions workflow builds two Android `arm64-v8a` UCI binaries:

- `bread-neon-arm64`: Armv8-A baseline with Advanced SIMD / NEON.
- `bread-dotprod-arm64`: Armv8.2-A with the DotProd extension enabled.

Bread's NNUE implementation uses x86 AVX2 intrinsic APIs. On AArch64, SIMDe maps those aliases to ARM Advanced SIMD instructions. The Android build installs the Ubuntu `libsimde-dev` package; it does not clone SIMDe from GitHub. This avoids depending on a Git clone that may be rejected with HTTP 403.

The NEON baseline uses `-march=armv8-a`. For AArch64, Armv8-A includes Advanced SIMD / NEON by default, so an explicit `+simd` suffix is unnecessary. The DotProd build uses `-march=armv8.2-a+dotprod`.

The NNUE model is embedded by the existing `INCBIN` implementation. The uploaded source snapshot contains five model components; if the feature-transformer weights are absent, the workflow downloads the matching file only after checking that upstream `version.txt` still matches Bread 4.0.0, then verifies its expected 18,874,368-byte size.

The workflow accepts a manual `uci_date` input and appends it to the UCI engine identity.

The DotProd target flag does not guarantee that the current NNUE kernels emit `SDOT`/`UDOT`. The workflow records the actual instruction count instead of claiming that DotProd instructions were generated.

Build target API: Android 24. NDK: 27.2.12479018 (r27c). C++ runtime: static.

Runtime UCI testing must be performed on an Android ARM64 device; the Ubuntu runner does not execute the Android binary.
