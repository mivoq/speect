macro(example nm)
    add_our_executable(${nm} SPCT_LIBRARY_TARGET)
endmacro(example nm)

example(file_tokenizer)
example(string_tokenizer)
