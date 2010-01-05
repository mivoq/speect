macro(test nm)
    add_our_executable(${nm} ${SPCT_LINK_WITH})
endmacro(test nm)

test(init_speect)
