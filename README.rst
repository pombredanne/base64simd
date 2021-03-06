================================================================================
                        base64 using SIMD instructions
================================================================================

Overview
--------------------------------------------------

Repository contains code for encoding and decoding base64 using SIMD instructions.
Depending on CPU's architecture, vectorized encoding is faster than scalar
versions by factor from **2 to 4**; decoding is faster **2 .. 2.7** times.

There are several versions of procedures utilizing following instructions sets:

* SSE,
* AVX2,
* AVX512F,
* AVX512BW,
* AVX512VL,
* BMI2, and
* ARM Neon.

Vectorization approaches were described in a series of articles:

* `Base64 encoding with SIMD instructions`__,
* `Base64 decoding with SIMD instructions`__,
* `Base64 encoding & decoding using AVX512BW instructions`__,
* `AVX512F base64 coding and decoding`__.

__ http://0x80.pl/notesen/2016-01-12-sse-base64-encoding.html
__ http://0x80.pl/notesen/2016-01-17-sse-base64-decoding.html
__ http://0x80.pl/notesen/2016-04-03-avx512-base64.html
__ http://0x80.pl/articles/avx512-foundation-base64.html

Performance results from various machines are located
in subdirectories ``results``.


Project organization
--------------------------------------------------

There are separate subdirectories for both algorithms, however both have
the same structure. Each project contains three programs:

* ``verify`` --- does simple validation of particular parts of algorithms,
* ``check`` --- validates whole procedures,
* ``speed`` --- compares speed of different variants of procedures.


Building
--------------------------------------------------

Type ``make`` to build ``verify``, ``check`` and ``speed`` program.  The
programs support only scalar, SSE and BMI procedures.

Type ``make avx2`` to build ``verify_avx2``, ``check_avx2`` and ``speed_avx2``.
The programs additionally support AVX2 procedures.

Type ``make avx512`` to build ``verify_avx512``, ``check_avx512`` and
``speed_avx512``.  The programs support also AVX512F procedures.

Type ``make avx512BW`` to build ``verify_avx512BW``, ``check_avx512BW`` and
``speed_avx512BW``.  The programs support also AVX512BW procedures.

Type ``make xop`` to build ``verify_xop``, ``check_xop`` and ``speed_xop``.
The programs support scalar, SSE and AMD XOP procedures.

Type ``make arm`` to build ``verify_arm``, ``check_arm`` and ``speed_arm``.
The programs support scalar, ARM Neon procedures.

Type ``make run``, ``make run_avx2``, ``make run_avx512`` or ``make run_avx512bw``
to run all programs.

BMI2 presence is determined based on ``/proc/cpuinfo`` or counterpart.
When an AVX2 or AVX512 target is used then BMI2 is enabled by default.


AVX512
--------------------------------------------------

To compile AVX512 versions of the programs at least GCC 5.3 is required.
GCC 4.9.2 hasn't got AVX512 support.

Please download `Intel Software Development Emulator`__ in order to run AVX512
variants via ``make run_avx512`` or ``run_avx512bw``.  The emulator path should
be added to the ``PATH``.

__ https://software.intel.com/en-us/articles/intel-software-development-emulator


Known problems
--------------------------------------------------

Both encoding and decoding don't match the base64 specification,
there is no processing of data tail, i.e. encoder never produces
'=' chars at the end, and decoder doesn't handle them at all.

All these shortcoming are not present in a brilliant library
by Alfred Klomp: https://github.com/aklomp/base64.


See also
--------------------------------------------------

* Daniel's benchmarks and comparison with state of the art solutions
  https://github.com/lemire/fastbase64
