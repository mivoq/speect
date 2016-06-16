speect_example(synth_test)
speect_example(speect_test)
speect_example(syll_test)

if(SPCT_UNIX OR SPCT_MACOSX)
  if(WANT_THREADS)
    speect_example(synth_threads_test pthread)
  endif(WANT_THREADS)
endif(SPCT_UNIX OR SPCT_MACOSX)
