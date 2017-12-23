speect_example(synth_test)
speect_example(speect_test)
speect_example(syll_test)

if (NOT CMAKE_VERSION VERSION_LESS 2.8.12)
  target_include_directories(syll_test PRIVATE $<BUILD_INTERFACE:${CMAKE_SPEECT_SOURCE_DIR}/plugins/linguistic/syllabification/src>)
else()
  include_directories(../../plugins/linguistic/syllabification/src)
endif()

if(SPCT_UNIX OR SPCT_MACOSX)
  if(WANT_THREADS)
    speect_example(synth_threads_test pthread)
  endif(WANT_THREADS)
endif(SPCT_UNIX OR SPCT_MACOSX)

add_executable(path base/utils/path.c)
target_link_libraries(path ${SPCT_LIBRARIES_TARGET})


add_executable(posix_path base/utils/platform/posix/posix_path.c)
target_link_libraries(posix_path ${SPCT_LIBRARIES_TARGET})

add_executable(win32_path base/utils/platform/win32/win32_path.c)
target_link_libraries(win32_path ${SPCT_LIBRARIES_TARGET})
