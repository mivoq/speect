# From: https://github.com/magthe/cmake-ocaml

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:

# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Find OCaml binaries.

include( FindPackageHandleStandardArgs )

# find base ocaml binary
find_program( CMAKE_OCAML_OCAML
    NAMES ocaml
    )
# get the directory, used for hints for the rest of the binaries
if( CMAKE_OCAML_OCAML )
    get_filename_component( OCAML_ROOT_DIR ${CMAKE_OCAML_OCAML} PATH )
endif()

# ocamlfind
find_program( CMAKE_OCAML_OCAMLFIND
    NAMES ocamlfind ocamlfind_opt
    HINTS ${OCAML_ROOT_DIR}
    )

if( CMAKE_OCAML_OCAMLFIND )
    execute_process(
        COMMAND ${CMAKE_OCAML_OCAMLFIND} ocamlc -version
        OUTPUT_VARIABLE CMAKE_OCAML_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    execute_process(
        COMMAND ${CMAKE_OCAML_OCAMLFIND} ocamlc -where
        OUTPUT_VARIABLE CMAKE_OCAML_STDLIB_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
endif()

find_package_handle_standard_args( OCAML "Could NOT find OCaml."
    CMAKE_OCAML_VERSION
    CMAKE_OCAML_STDLIB_PATH
    CMAKE_OCAML_OCAML
    CMAKE_OCAML_OCAMLFIND
    )
        
mark_as_advanced(
    CMAKE_OCAML_OCAML
    CMAKE_OCAML_OCAMLFIND
    )
