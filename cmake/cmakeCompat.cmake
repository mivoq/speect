######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : July 2010                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMake Compatibilty functions                                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#      speect_file_copy,  file(COPY ...  is new from CMake 2.8                       #
#------------------------------------------------------------------------------------#
#
# speect_file_copy(src dst)
#
# Copy the files in src to dst
#
# :param src: A list of files to copy to dst (can be single file), must be absolute paths
# :type src: list or single file
# :param dst: A destination path (must be absolute), or a full file name if the file in src
#             must be renamed

function(speect_file_copy src dst)
  # last argument must be dst
  list(GET ARGV -1 dst)
  list(REMOVE_AT ARGV -1) # and remove it
  if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 3.0)
    # get the destination directory
    list(LENGTH ARGV SLEN)
    if(SLEN EQUAL 1)  # single file to copy
      if(IS_DIRECTORY ${dst}) # just a copy
	get_filename_component(SOURCE_FILE ${ARGV} NAME)
	set(OUTPUT_FILE "${dst}/${SOURCE_FILE}")
	configure_file(${ARGV} ${OUTPUT_FILE} COPYONLY)
      elseif(IS_DIRECTORY ${dst}) # it's a rename copy
	configure_file(${ARGV} ${dst} COPYONLY)	 
      endif(IS_DIRECTORY ${dst})
    elseif(SLEN EQUAL 1)  # list of files
      get_filename_component(DEST_DIR ${dst} PATH)
      # src can be a list
      foreach(arg ${ARGV})
	if(NOT IS_ABSOLUTE ${arg})
	  message(FATAL_ERROR "Source file names must be absolute")
	endif(NOT IS_ABSOLUTE ${arg})
	# get the source file
	get_filename_component(SOURCE_FILE ${arg} NAME)
	set(OUTPUT_FILE "${DEST_DIR}/${SOURCE_FILE}")
	configure_file(${arg} ${OUTPUT_FILE} COPYONLY)
      endforeach(arg ${ARGV})
    endif(SLEN EQUAL 1)    
  else(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 3.0)
    # Ok, use normal copy
    file(COPY ${ARGV} DESTINATION ${dst})
  endif(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 3.0)
endfunction(speect_file_copy)