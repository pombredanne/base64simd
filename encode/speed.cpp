#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <memory>

#include "config.h"
#include "../gettime.cpp"
#include "../cmdline.cpp"
#include "../fnv32.cpp"

#include "encode.scalar.cpp"
#include "lookup.swar.cpp"
#include "encode.swar.cpp"
#if defined(HAVE_SSE_INSTRUCTIONS)
#   include "lookup.sse.cpp"
#   include "encode.sse.cpp"
#endif
#if defined(HAVE_XOP_INSTRUCTIONS)
#   include "encode.xop.cpp"
#   include "lookup.xop.cpp"
#endif
#if defined(HAVE_AVX2_INSTRUCTIONS)
#   include "lookup.avx2.cpp"
#   include "encode.avx2.cpp"
#endif
#if defined(HAVE_AVX512_INSTRUCTIONS)
#   include "../avx512_swar.cpp"
#   include "unpack.avx512.cpp"
#   include "lookup.avx512.cpp"
#   include "encode.avx512.cpp"
#endif
#if defined(HAVE_AVX512BW_INSTRUCTIONS)
#   include "lookup.avx512bw.cpp"
#   include "encode.avx512bw.cpp"
#endif
#if defined(HAVE_NEON_INSTRUCTIONS)
#   include "lookup.neon.cpp"
#   include "encode.neon.cpp"
#endif

#include "application.cpp"

class Application final: public ApplicationBase {

    double reference_time;

public:
    Application(const CommandLine& c)
#if defined(BUFFER_SIZE)
        : ApplicationBase(c, BUFFER_SIZE) {}
#else
        : ApplicationBase(c) {}
#endif

    int run() {
        
        reference_time = 0.0;

        #include "functions.cpp"

        if (cmd.empty() || cmd.has("scalar32")) {
            measure("scalar (32 bit)", base64::scalar::encode32);
        }

#if !defined(HAVE_NEON_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("scalar64")) {
            measure("scalar (64 bit)", base64::scalar::encode64);
        }

        if (cmd.empty() || cmd.has("swar")) {
            measure("SWAR (64 bit)", base64::swar::encode);
        }
#endif

#if defined(HAVE_SSE_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("sse")) {
            measure("SSE (lookup: naive)", sse_naive);
        }

        if (cmd.empty() || cmd.has("sse1")) {
            measure("SSE (lookup: other improved)", sse_optimized1);
        }

        if (cmd.empty() || cmd.has("sse2")) {
            measure("SSE (lookup: improved)", sse_optimized2);
        }

        if (cmd.empty() || cmd.has("sse3")) {
            measure("SSE (lookup: pshufb-based)", sse_pshufb);
        }

        if (cmd.empty() || cmd.has("sse3/improved")) {
            measure("SSE (lookup: pshufb improved)", sse_pshufb_improved);
        }

        if (cmd.empty() || cmd.has("sse1/unrolled")) {
            measure("SSE (lookup: other improved, unrolled)", sse_optimized1_unrolled);
        }

        if (cmd.empty() || cmd.has("sse2/unrolled")) {
            measure("SSE (lookup: improved, unrolled)", sse_optimized2_unrolled);
        }

        if (cmd.empty() || cmd.has("sse2/unrolled2")) {
            measure("SSE (lookup: pshufb-based, unrolled)", sse_pshufb_unrolled);
        }

        if (cmd.empty() || cmd.has("sse3/improved/unrolled")) {
            measure("SSE (lookup: pshufb improved unrolled)", sse_pshufb_improved_unrolled);
        }

        if (cmd.empty() || cmd.has("sse2/fully_unrolled")) {
            measure("SSE (fully unrolled improved lookup)", base64::sse::encode_full_unrolled);
        }
#endif

#if defined(HAVE_BMI2_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("bmi1")) {
            measure("SSE & BMI2 (lookup: naive)", sse_bmi2_naive);
        }

        if (cmd.empty() || cmd.has("bmi2")) {
            measure("SSE & BMI2 (lookup: improved)", sse_bmi2_optimized);
        }

        if (cmd.empty() || cmd.has("bmi3")) {
            measure("SSE & BMI2 (lookup: pshufb improved)", sse_bmi2_pshufb_improved);
        }
#endif

#if defined(HAVE_XOP_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("xop/vpermb")) {
            measure("XOP (vpermb)", xop_vperm);
        }

        if (cmd.empty() || cmd.has("xop/pshufb")) {
            measure("XOP (pshufb improved)", xop_pshufb);
        }
#endif

#if defined(HAVE_AVX2_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("avx2")) {
            measure("AVX2 (lookup: improved)", avx2_optimized2);
        }

        if (cmd.empty() || cmd.has("avx2/unrolled")) {
            measure("AVX2 (lookup: improved, unrolled)", avx2_optimized2_unrolled);
        }

        if (cmd.empty() || cmd.has("avx2/pshufb")) {
            measure("AVX2 (lookup: pshufb-based)", avx2_pshufb);
        }

        if (cmd.empty() || cmd.has("avx2/pshufb/unrolled")) {
            measure("AVX2 (lookup: pshufb-based, unrolled)", avx2_pshufb_unrolled);
        }

        if (cmd.empty() || cmd.has("avx2/pshufb/improved")) {
            measure("AVX2 (lookup: pshufb improved)", avx2_pshufb_improved);
        }

        if (cmd.empty() || cmd.has("avx2/pshufb/improved/unrolled")) {
            measure("AVX2 (lookup: pshufb unrolled improved)", avx2_pshufb_improved_unrolled);
        }

    #if defined(HAVE_BMI2_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("avx2/pshufb/improved/unrolled")) {
            measure("AVX2 & BMI (lookup: pshufb improved)", avx2_bmi2_pshufb_improved);
        }
    #endif
#endif

#if defined(HAVE_AVX512_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("avx512/logic")) {
            measure("AVX512 (incremental logic)", avx512_swar_logic);
        }

        if (cmd.empty() || cmd.has("avx512/logic/improved")) {
            measure("AVX512 (incremental logic improved)", avx512_swar_logic_improved);
        }

        if (cmd.empty() || cmd.has("avx512/logic/improved/gather")) {
            measure("AVX512 (incremental logic improved with gather load)", avx512_swar_logic_improved_load_gather);
        }

        if (cmd.empty() || cmd.has("avx512/binsearch")) {
            measure("AVX512 (binary search)", avx512_bin_search);
        }

        if (cmd.empty() || cmd.has("avx512/gather")) {
            measure("AVX512 (gather)", avx512_gathers);
        }
#endif

#if defined(HAVE_NEON_INSTRUCTIONS)
        if (cmd.empty() || cmd.has("neon/1")) {
            measure("ARM NEON (naive lookup)", neon_naive);
        }

        if (cmd.empty() || cmd.has("neon/2")) {
            measure("ARM NEON (optimized lookup)", neon_optimized);
        }

        if (cmd.empty() || cmd.has("neon/3")) {
            measure("ARM NEON (pshufb improved lookup)", neon_pshufb_improved);
        }
#endif

        return 0;
    }

    template<typename T>
    double measure(const char* name, T callback) {

        initialize();

        printf("%-40s... ", name);
        fflush(stdout);

        unsigned n = iterations;
        double time = -1;
        while (n-- > 0) {

            const auto t1 = get_time();
            callback(input.get(), get_input_size(), output.get());
            const auto t2 = get_time();

            const double t = (t2 - t1)/1000000.0;
            if (time < 0) {
                time = t;
            } else {
                time = std::min(time, t);
            }
        }

        printf("%0.5f", time);

        if (reference_time == 0.0) {
            reference_time = time;
        } else {
            printf(" (speedup %0.2f)", reference_time/time);
        }

        putchar('\n');

        return time;
    }
};


int main(int argc, char* argv[]) {

    CommandLine cmd(argc, argv);
    Application app(cmd);

    return app.run();
}

