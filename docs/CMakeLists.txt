######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 11 June 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for Speect Docs                                                        ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"Speect Docs\"")
project(speect_plugins C)


#------------------------------------------------------------------------------------#
#                             Find sphinx-build                                      #
#------------------------------------------------------------------------------------#

if(NOT SPHINXBUILD_EXECUTABLE)
  find_program(SPHINXBUILD_EXECUTABLE 
    NAMES sphinx-build)
endif(NOT SPHINXBUILD_EXECUTABLE)
 
if(SPHINXBUILD_EXECUTABLE-NOTFOUND)
  message(FATAL_ERRROR "Failed to find the 'sphinx-build' executable, please specify it with -DSPHINXBUILD_EXECUTABLE")
endif(SPHINXBUILD_EXECUTABLE-NOTFOUND)


#------------------------------------------------------------------------------------#
#                          Set some Sphinx variables                                 #
#------------------------------------------------------------------------------------#

set(PAPEROPT_a4 "-D latex_paper_size=a4")
set(PAPEROPT_letter "-D latex_paper_size=letter")
set(SPHINXOPTS)
set(PAPER)
set(ALLSPHINXOPTS -d ${CMAKE_BINARY_DIR}/docs/doctrees ${PAPEROPT_${PAPER}} ${SPHINXOPTS} ${CMAKE_SOURCE_DIR}/docs)


#------------------------------------------------------------------------------------#
#                                 Add targets                                        #
#------------------------------------------------------------------------------------#

# html
add_custom_command(
  OUTPUT  ${CMAKE_BINARY_DIR}/docs/html/index.html 
  COMMAND ${SPHINXBUILD_EXECUTABLE} -b html ${ALLSPHINXOPTS} ${CMAKE_BINARY_DIR}/docs/html
  COMMENT "Building Sphinx HTML pages in ${CMAKE_BINARY_DIR}/docs/html"
  VERBATIM
  )

if(WANT_DOCS_HTML)
  add_custom_target(html ALL DEPENDS ${CMAKE_BINARY_DIR}/docs/html/index.html)
endif(WANT_DOCS_HTML)


# latex
add_custom_command(
  OUTPUT  ${CMAKE_BINARY_DIR}/docs/latex/Speect.tex
  COMMAND ${SPHINXBUILD_EXECUTABLE} -b latex ${ALLSPHINXOPTS} ${CMAKE_BINARY_DIR}/docs/latex
  COMMENT "Building Sphinx LaTeX files in ${CMAKE_BINARY_DIR}/docs/latex. Run `make all-pdf' or `make all-ps' in that directory to run these through (pdf)latex."
  VERBATIM
  )

if(WANT_DOCS_PDF)
  add_custom_target(latex DEPENDS ${CMAKE_BINARY_DIR}/docs/latex/Speect.tex)
endif(WANT_DOCS_PDF)