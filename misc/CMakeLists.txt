add_definitions(-DFLOATING_POINT -DVAR_ARRAYS)
include_directories(../src)

add_executable(mksine mksine.c)
if(UNIX)
target_link_libraries(mksine m)
endif()

add_executable(t16_8 t16_8.c ../src/fdmdv.c ../src/kiss_fft.c)
target_link_libraries(t16_8 codec2)

add_executable(t16_8_short t16_8_short.c ../src/fdmdv.c ../src/kiss_fft.c)
target_link_libraries(t16_8_short codec2)

add_executable(16_8_short 16_8_short.c ../src/fdmdv.c ../src/kiss_fft.c)
target_link_libraries(16_8_short codec2)

add_executable(extract extract.c)
if (TARGET getopt)
target_link_libraries(extract getopt)
endif()

add_executable(vqtrain vqtrain.c)
if (UNIX)
target_link_libraries(vqtrain m)
endif()
if (TARGET getopt)
target_link_libraries(vqtrain getopt)
endif()

add_executable(raw2h raw2h.c)
target_link_libraries(raw2h codec2)

add_executable(tnlp tnlp.c)
target_link_libraries(tnlp codec2)

add_executable(tlininterp tlininterp.c)

if (TARGET getopt)
target_link_libraries(tlininterp getopt)
endif()

add_executable(tdec tdec.c)
if (TARGET getopt)
target_link_libraries(tdec getopt)
endif()

