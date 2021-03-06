.SUFFIXES:
.PHONY: all clean run

SDE=sde -cnl
FLAGS_BASE=-Wall -Wextra -pedantic -O3 -std=gnu++11

HAVE_BMI2=$(shell python script/cpuflags.py bmi2)
ifeq ($(HAVE_BMI2),present)
    FLAGS_BASE+=-DHAVE_BMI2_INSTRUCTIONS
endif

FLAGS=$(FLAGS_BASE) -msse4 -DHAVE_SSE_INSTRUCTIONS
FLAGS_AVX2=$(FLAGS_BASE) -mavx2 -mbmi2 -DHAVE_AVX2_INSTRUCTIONS -DHAVE_BMI2_INSTRUCTIONS
FLAGS_AVX512=$(FLAGS_BASE) -mavx512f -mbmi2 -DHAVE_AVX512_INSTRUCTIONS -DHAVE_BMI2_INSTRUCTIONS
FLAGS_AVX512BW=$(FLAGS_BASE) -mavx512vbmi -mbmi2 -DHAVE_AVX512BW_INSTRUCTIONS -DHAVE_BMI2_INSTRUCTIONS 
FLAGS_XOP=$(FLAGS_BASE) -mxop -DHAVE_XOP_INSTRUCTIONS
FLAGS_ARM=$(FLAGS_BASE) -mfpu=neon -DHAVE_NEON_INSTRUCTIONS

DEPS=decode.*.cpp decoders.*.cpp lookup.*.cpp pack.*.cpp run_all.cpp function_registry.cpp ../avx512_swar.cpp config.h

ALL=verify speed check
ALL_XOP=verify_xop speed_xop check_xop
ALL_ARM=verify_arm speed_arm check_arm
ALL_AVX2=verify_avx2 speed_avx2 check_avx2
ALL_AVX512=verify_avx512 check_avx512 speed_avx512
ALL_AVX512BW=verify_avx512bw check_avx512bw
ALL_INTEL_TARGETS=$(ALL) $(ALL_XOP) $(ALL_AVX2) $(ALL_AVX512) $(ALL_AVX512BW)
ALL_TARGETS=$(ALL_INTEL_TARGETS) $(ALL_ARM)

all: $(ALL)

sse: $(ALL)

xop: $(ALL_XOP)

arm: $(ALL_ARM)

avx2: $(ALL_AVX2)

avx512: $(ALL_AVX512)

avx512bw: $(ALL_AVX512BW)

run: $(ALL)
	./verify
	./check
	./speed

run_xop: $(ALL_XOP)
	./verify_xop
	./check_xop
	./speed_xop

run_avx2: $(ALL_AVX2)
	./verify_avx2
	./check_avx2
	./speed_avx2

run_avx512: $(ALL_AVX512)
	$(SDE) -- ./verify_avx512
	$(SDE) -- ./check_avx512
	$(SDE) -- ./speed_avx512

run_avx512bw: $(ALL_AVX512BW)
	$(SDE) -- ./verify_avx512bw
	$(SDE) -- ./check_avx512bw

VERIFY_DEPS=verify.cpp $(DEPS)
SPEED_DEPS=speed.cpp $(DEPS) application.cpp ../cmdline.cpp ../gettime.cpp
CHECK_DEPS=check.cpp $(DEPS) application.cpp ../cmdline.cpp ../fnv32.cpp

verify: $(VERIFY_DEPS)
	$(CXX) $(FLAGS) verify.cpp -o $@

speed: $(SPEED_DEPS)
	$(CXX) $(FLAGS) speed.cpp -o $@

check: $(CHECK_DEPS)
	$(CXX) $(FLAGS) check.cpp -o $@

verify_xop: $(VERIFY_DEPS)
	$(CXX) $(FLAGS_XOP) verify.cpp -o $@

speed_xop: $(SPEED_DEPS)
	$(CXX) $(FLAGS_XOP) speed.cpp -o $@

check_xop: $(CHECK_DEPS)
	$(CXX) $(FLAGS_XOP) check.cpp -o $@

verify_avx2: $(VERIFY_DEPS)
	$(CXX) $(FLAGS_AVX2) verify.cpp -o $@

verify_avx512: $(VERIFY_DEPS)
	$(CXX) $(FLAGS_AVX512) verify.cpp -o $@

verify_avx512bw: $(VERIFY_DEPS)
	$(CXX) $(FLAGS_AVX512BW) verify.cpp -o $@

speed_avx2: $(SPEED_DEPS)
	$(CXX) $(FLAGS_AVX2) speed.cpp -o $@

speed_avx512: $(SPEED_DEPS)
	$(CXX) $(FLAGS_AVX512) speed.cpp -o $@

check_avx2: $(CHECK_DEPS)
	$(CXX) $(FLAGS_AVX2) check.cpp -o $@

check_avx512: $(CHECK_DEPS)
	$(CXX) $(FLAGS_AVX512) check.cpp -o $@

check_avx512bw: $(CHECK_DEPS)
	$(CXX) $(FLAGS_AVX512BW) check.cpp -o $@

verify_arm: $(VERIFY_DEPS)
	$(CXX) $(FLAGS_ARM) verify.cpp -o $@

speed_arm: $(SPEED_DEPS)
	$(CXX) $(FLAGS_ARM) speed.cpp -o $@

check_arm: $(CHECK_DEPS)
	$(CXX) $(FLAGS_ARM) check.cpp -o $@

build_all: $(ALL_INTEL_TARGETS)
verify_all: verify verify_avx2 verify_avx512 verify_avx512bw
	# only Intel targets
	sde -cnl -- ./verify
	sde -cnl -- ./verify_avx2
	sde -cnl -- ./verify_avx512
	sde -cnl -- ./verify_avx512bw

clean:
	rm -f $(ALL_TARGETS)
