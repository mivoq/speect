macro(example nm)
    add_our_executable(${nm} SPCT_LIBRARY_TARGET)
endmacro(example nm)

example(example_write)
example(example_read)
