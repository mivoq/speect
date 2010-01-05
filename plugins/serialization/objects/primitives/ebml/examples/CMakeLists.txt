macro(example nm)
    add_our_executable(${nm} SPCT_LIBRARY_TARGET)
endmacro(example nm)

example(prim_example_write)
example(prim_example_read)
