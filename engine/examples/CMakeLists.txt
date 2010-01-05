macro(example nm)
    add_our_executable(${nm} ${SPCT_LINK_WITH})
endmacro(example nm)

add_subdirectory(base)
add_subdirectory(containers)
add_subdirectory(datasources)
add_subdirectory(hrg)

