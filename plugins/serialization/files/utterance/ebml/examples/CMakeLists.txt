macro(example nm)
    add_our_executable(${nm} SPCT_LIBRARY_TARGET)
endmacro(example nm)

example(save_utt)
example(load_utt)

