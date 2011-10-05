.. _cmake_macros:

.. index:: 
   single: CMake API; Macros and Functions

==========================
CMake Macros and Functions
==========================


Summary
=======


+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_func:`speect_example`                | Add an example that will be compiled as an executable.		   	  |
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_func:`speect_file_copy` 	      | Copy the files in src to dst. 	       	  				  | 
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_macro:`speect_plugin_definition`     | Define a plug-in.							  |
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_macro:`speect_plugin_sources` 	      | Append the given list of sources to the plug-in source list variable.	  |
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_macro:`speect_plugin_headers` 	      | Append the given list of headers to the plug-in header list variable.	  |
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_macro:`speect_plugin_include_dirs`   | Append the given list of directories to the plug-in include directories.  |
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_func:`speect_plugin_create`  	      | Create the plug-in shared object.       	   	   	   	  |
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_macro:`speect_plugin_configure_info` | Configure the plug-in information header.				  |
+---------------------------------------------+---------------------------------------------------------------------------+
| :cmake_macro:`speect_include_plugin` 	      | Include another plug-in in the build of this one. 			  |
+---------------------------------------------+---------------------------------------------------------------------------+


API
===


.. cmake_func:: speect_example

   Add an example that will be compiled as an executable.  Defined in
   ``speect/cmake/spctFunctions.cmake``.


   :signature: **speect_example(name [link_libs = NULL])**
   :param name: The name of the c file (excluding ".c"), will also be the name of the executable.
   :type name: string
   :param link_libs: Libraries that the executable must link with (excluding Speect).
   :type link_libs: string


.. cmake_func:: speect_file_copy

   Copy the files in src to dst. For compatibility with older versions
   of CMake.  Defined in ``speect/cmake/cmakeCompat.cmake``.


   :signature: **speect_file_copy(src dst)**
   :param src: A list of files to copy to dst (can be single file), must be absolute paths.
   :type src: list or single file
   :param dst: A destination path (must be absolute), or a full file name if the file in src must be renamed.


.. cmake_macro:: speect_plugin_definition

   Define a plug-in. The plug-in name is lowercased and the project
   name is set to "speect_lowercase_name_plugin" with the C
   language. The plug-in library's name will be "lowercase_name.spi"
   with links to:
   	     
	     * "lowercase_name.spi.version_major"
             *
               "lowercase_name.spi.version_major.version_minor.version_patch"

   The class name is used in the documentation strings (for example
   "SArrayFloat plug-in initialization").  The plug-in's "cmake"
   directory will be added to ``CMAKE_MODULE_PATH``.
   Defined in ``speect/plugins/cmake/pluginFunctions.cmake``.


   :signature: **speect_plugin_definition(name classname version_major version_minor version_patch)**
   :param name: The name of the plug-in.
   :type name: string
   :param name: The name of the class that the plug-in implements.
   :type name: string
   :param version_major: The major version number of the plug-in.
   :type version_major: int
   :param version_minor: The minor version number of the plug-in.
   :type version_minor: int
   :param version_patch: The patch number of the plug-in.
   :type version_patch: int
   
   For example: speect_plugin_definition(Viterbi "SViterbi" 0 9 5)
   will create a plug-in viterbi.spi that points to viterbi.spi.0 which in turn
   points to viterbi.spi.0.9.5


.. cmake_macro:: speect_plugin_sources

   Appends the given list of sources to the plug-in source list variable, lowercase_name_SRC_FILES.
   Defined in ``speect/plugins/cmake/pluginFunctions.cmake``.

   :signature: **speect_plugin_sources(source_list)**
   :param source_list: A list of source files to append to lowercase_name_SRC_FILES.
   :type source_list: CMake list


.. cmake_macro:: speect_plugin_headers

   Appends the given list of headers to the plug-in header list variable, lowercase_name_INCLUDE_FILES
   These headers will be installed to ``CMAKE_INSTALL_PREFIX/include/speect/plugins/<plugin_dir>``.
   Defined in ``speect/plugins/cmake/pluginFunctions.cmake``.


   :signature: **speect_plugin_headers(headers_list)**
   :param headers_list: A list of header files to append to lowercase_name_INCLUDE_FILES
   :type headers_list: CMake list


.. cmake_macro:: speect_plugin_include_dirs

   Appends the given list of directories to the plug-in include directories variable, 
   lowercase_name_INCLUDE_DIRS.
   Defined in ``speect/plugins/cmake/pluginFunctions.cmake``.


   :signature: **speect_plugin_include_dirs(directories)**
   :param directories: A list of directories to append to lowercase_name_INCLUDE_DIRS
   :type headers_list: CMake list


.. cmake_func:: speect_plugin_create

   Create the plug-in shared object. The shared object will be linked against the 
   Speect Engine library. Build options will be as set at top level Speect CMakeLists.txt
   The Speect Engine directories will be included, as well as all headers in the plug-in's
   source (src) directory. Optional parameters, which are libraries to link with,
   can be given in a list form. Installation paths for UNIX system types will be set and
   the binary path output will be ``CMAKE_BINARY_DIR/plugins/lib``.

   A plug-in CMake configuration file will also be created. This configuration file can be
   included in other plug-in CMakeLists.txt to include their header files.
   Defined in ``speect/plugins/cmake/pluginFunctions.cmake``.


   :signature: **speect_plugin_create([link_lib_list = NULL])**
   :param link_lib_list: Extra libraries that the plug-in must be linked with.
   :type link_lib_list: CMake list


.. cmake_macro:: speect_plugin_configure_info

   Configure the plug-in information header, ``plugin_info.h``, with the provided information.
   Defined in ``speect/plugins/cmake/pluginFunctions.cmake``.


   :signature: **speect_plugin_configure_info(description major_min minor_min)**
   :param description: A short description of the plug-in.
   :type description: string
   :param major_min: The minimum major version of the Speect Engine with which this plug-in will work.
   :type major_min: int
   :param minor_min: The minimum minor version of the Speect Engine with which this plug-in will work.
   :type minor_min: int


.. cmake_macro:: speect_include_plugin

   Include another plug-in in the build of this one. This macro will load the plug-in's
   CMake configuration, which contains the header files to include, as well as version info.
   A fatal error will be thrown if the given major and minor version numbers are greater 
   than the requested plug-in's versions.
   Defined in ``speect/plugins/cmake/pluginFunctions.cmake``.


   :signature: **speect_include_plugin(name major_min minor_min)**
   :param name: The name of the requested plug-in to include.
   :type description: string
   :param major_min: The minimum major version of the requested plug-in with which this plug-in will work.
   :type major_min: int
   :param minor_min: The minimum minor version of the requested plug-in with which this plug-in will work.
   :type minor_min: int

