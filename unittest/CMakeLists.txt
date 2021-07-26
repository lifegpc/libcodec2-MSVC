add_definitions(-DFLOATING_POINT -DVAR_ARRAYS)
include_directories(../src)

if (UNIX)
set(MATH_LIB m)
endif()

add_executable(tfdmdv tfdmdv.c ../src/fdmdv.c ../src/kiss_fft.c ../src/octave.c)
target_link_libraries(tfdmdv codec2)

add_executable(tcohpsk tcohpsk.c ../src/cohpsk.c ../src/octave.c)
target_link_libraries(tcohpsk codec2)

add_executable(tfsk tfsk.c ../src/kiss_fft.c ../src/kiss_fftr.c ../src/octave.c ../src/modem_probe.c)
target_link_libraries(tfsk ${MATH_LIB})

add_executable(tfmfsk tfmfsk.c ../src/octave.c ../src/modem_probe.c)
target_link_libraries(tfmfsk ${MATH_LIB})

add_executable(tdeframer tdeframer.c)
target_link_libraries(tdeframer ${MATH_LIB} codec2)

add_definitions(-DMODEMPROBE_ENABLE -DXXXXX)

if (MSVC)
set(FDM_SRCS_MORE ../src/complex_win32.c)
endif()

add_executable(tofdm tofdm.c ../src/ofdm.c ../src/octave.c ../src/kiss_fft.c ../src/modem_probe.c ../src/mpdecode_core.c ../src/phi0.c ../src/filter.c ../src/HRA_112_112.c "${FDM_SRCS_MORE}")
target_link_libraries(tofdm ${MATH_LIB})
if (TARGET getopt)
target_link_libraries(tofdm getopt)
endif()

if (NOT "${CMAKE_USE_PTHREADS_INIT}" STREQUAL "")
add_executable(tfifo tfifo.c ../src/codec2_fifo.c)
target_link_libraries(tfifo codec2 ${CMAKE_THREAD_LIBS_INIT})
endif()

add_executable(fdmdv_mem fdmdv_mem.c)

if (MSVC)
set(OFDM_MEM_SRCS ../src/complex_win32.c)
endif()

add_executable(ofdm_mem ofdm_mem.c ../src/ofdm.c ../src/octave.c ../src/kiss_fft.c ../src/modem_probe.c ../src/mpdecode_core.c ../src/phi0.c ../src/filter.c ${OFDM_MEM_SRCS})
target_link_libraries(ofdm_mem ${MATH_LIB})

add_library(function_trace STATIC ../unittest/function_trace.c)

add_executable(ofdm_stack ofdm_stack.c ../src/ofdm.c ../src/octave.c ../src/kiss_fft.c ../src/modem_probe.c ../src/mpdecode_core.c ../src/phi0.c ../src/filter.c ${OFDM_MEM_SRCS})
if (CMAKE_C_COMPILER MATCHES "gcc$")
    target_link_libraries(ofdm_stack function_trace m -no-pie "-Wl,-Map=ofdm_stack.map")
    target_compile_options(ofdm_stack PUBLIC -fstack-usage -finstrument-functions -no-pie)
elseif(NOT MSVC)
    target_link_libraries(ofdm_stack function_trace m -no-pie)
    target_compile_options(ofdm_stack PUBLIC -finstrument-functions -no-pie)
endif()
add_definitions(-D__UNITTEST__)

if (TARGET getopt)
target_link_libraries(ofdm_stack getopt)
endif()

if (NOT WIN32 OR NOT BUILD_SHARED_LIBS)
add_executable(tnewamp1 tnewamp1.c ../src/quantise.c ../src/newamp1.c ../src/mbest.c ../src/kiss_fft.c ../src/sine.c ../src/nlp.c ../src/dump.c ../src/octave.c ${CODEBOOKS})
target_link_libraries(tnewamp1 codec2)
endif()

add_executable(compare_ints compare_ints.c)
if (TARGET getopt)
target_link_libraries(compare_ints getopt)
endif()

add_executable(compare_floats compare_floats.c)
if (TARGET getopt)
target_link_libraries(compare_floats getopt)
endif()

add_executable(test_phi0 test_phi0.c ../src/phi0.c)
target_link_libraries(test_phi0 ${MATH_LIB})

add_executable(tst_codec2_fft_init tst_codec2_fft_init.c)
target_link_libraries(tst_codec2_fft_init ${MATH_LIB} codec2)
