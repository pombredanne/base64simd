// this file must be included in the function's body

auto sse_naive = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode(base64::sse::lookup_naive, input, bytes, output);
};

auto sse_optimized1 = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode(base64::sse::lookup_version1, input, bytes, output);
};

auto sse_optimized2 = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode(base64::sse::lookup_version2, input, bytes, output);
};

auto sse_pshufb = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode(base64::sse::lookup_pshufb, input, bytes, output);
};

auto sse_optimized1_unrolled = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode_unrolled(base64::sse::lookup_version1, input, bytes, output);
};

auto sse_optimized2_unrolled = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode_unrolled(base64::sse::lookup_version2, input, bytes, output);
};

auto sse_pshufb_unrolled = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode_unrolled(base64::sse::lookup_pshufb, input, bytes, output);
};

auto sse_pshufb_improved = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode(base64::sse::lookup_pshufb_improved, input, bytes, output);
};

auto sse_pshufb_improved_unrolled = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode_unrolled(base64::sse::lookup_pshufb_improved, input, bytes, output);
};

#if defined(HAVE_BMI2_INSTRUCTIONS)
auto sse_bmi2_naive = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode_bmi2(base64::sse::lookup_naive, input, bytes, output);
};

auto sse_bmi2_optimized = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode_bmi2(base64::sse::lookup_version1, input, bytes, output);
};

auto sse_bmi2_pshufb_improved = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::sse::encode_bmi2(base64::sse::lookup_pshufb_improved, input, bytes, output);
};
#endif

#if defined(HAVE_XOP_INSTRUCTIONS)
auto xop_vperm = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::xop::encode(base64::xop::lookup, input, bytes, output);
};

auto xop_pshufb = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::xop::encode(base64::sse::lookup_pshufb_improved, input, bytes, output);
};
#endif

#if defined(HAVE_AVX2_INSTRUCTIONS)
auto avx2_optimized2 = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx2::encode(base64::avx2::lookup_version2, input, bytes, output);
};

auto avx2_optimized2_unrolled = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx2::encode_unrolled(base64::avx2::lookup_version2, input, bytes, output);
};

auto avx2_pshufb = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx2::encode(base64::avx2::lookup_pshufb, input, bytes, output);
};

auto avx2_pshufb_unrolled = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx2::encode_unrolled(base64::avx2::lookup_pshufb, input, bytes, output);
};

auto avx2_pshufb_improved = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx2::encode(base64::avx2::lookup_pshufb_improved, input, bytes, output);
};

auto avx2_pshufb_improved_unrolled = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx2::encode_unrolled(base64::avx2::lookup_pshufb_improved, input, bytes, output);
};

    #if defined(HAVE_AVX2_INSTRUCTIONS)
        auto avx2_bmi2_pshufb_improved = [](uint8_t* input, size_t bytes, uint8_t* output) {
            base64::avx2::encode_bmi2(base64::avx2::lookup_pshufb_improved, input, bytes, output);
        };
    #endif
#endif

#if defined(HAVE_AVX512BW_INSTRUCTIONS)
auto avx512bw = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx512bw::encode_improved_splitting(input, bytes, output);
};

auto avx512bw_faster = [](uint8_t* input, size_t bytes, uint8_t* output) {
    base64::avx512bw::encode_faster_spliting(input, bytes, output);
};
#endif

#if defined(HAVE_AVX512_INSTRUCTIONS)
auto avx512_gathers = [](uint8_t* input, size_t bytes, uint8_t* output) {
    using namespace base64::avx512;
    encode(lookup_gather, unpack_identity, input, bytes, output);
};

auto avx512_swar_logic = [](uint8_t* input, size_t bytes, uint8_t* output) {
    using namespace base64::avx512;
    encode(lookup_incremental_logic, unpack_improved, input, bytes, output);
};

auto avx512_swar_logic_improved = [](uint8_t* input, size_t bytes, uint8_t* output) {
    using namespace base64::avx512;
    encode(lookup_incremental_logic_improved, unpack_improved, input, bytes, output);
};

auto avx512_swar_logic_improved_load_gather = [](uint8_t* input, size_t bytes, uint8_t* output) {
    using namespace base64::avx512;
    encode_load_gather(lookup_incremental_logic_improved, unpack_improved, input, bytes, output);
};

auto avx512_bin_search = [](uint8_t* input, size_t bytes, uint8_t* output) {
    using namespace base64::avx512;
    encode(lookup_binary_search, unpack_improved, input, bytes, output);
};
#endif
