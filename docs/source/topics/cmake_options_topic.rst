.. _cmake_options_topic/main:

.. index:: 
   single: Topic Guides (C API); CMake configuration options

===========================
CMake configuration options
===========================

Several configuration options are available to customize the Speect
build system.  These options are configured through an interactive
CMake interface as described in :doc:`../installation`. 


.. cmake:: CMAKE_BUILD_TYPE

   The build type influences the C  optimization and debug compilation flags.    

   :options: Debug, Release, RelWithDebInfo, Minsize, Profile
   :default: Debug
   

.. cmake:: CMAKE_INSTALL_PREFIX

   The installation path for the ``make install`` command.
   
   :default: ``/usr/local``


.. cmake:: ERROR_ABORT_FATAL

   Call the ISO C function ``abort()`` or gracefully exit 
   with error log.

   :options: ON or OFF
   :default: OFF


.. cmake:: ERROR_HANDLING

   Use Speect error handling mechanism.

   :options: ON or OFF
   :default: ON


.. cmake:: LIB_SUFFIX

   Suffix for library directories, e.g. "64"

   
.. cmake:: SAFE_CAST

   If ON then all Speect object casting will be checked.

   :options: ON or OFF
   :default: ON


.. cmake:: STRICT_WARN  

   Enable stricter compile flags.
   
   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_EXAMPLES

   Include examples in build process.

   
   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_TESTS

   Inlcuded tests in build process.

   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_THREADS

   Enable multi threaded support.

   :options: ON or OFF
   :default: OFF


.. cmake:: WARN_DECL_AFTER_STMT

   Warn about declarations after statements (GCC)

   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_PYTHON_WRAPPER

   Include Python bindings in build process.

   :options: ON or OFF
   :default: ON


.. cmake:: HTS_ENGINE_INCLUDE

   Path to HTS Engine include directory.


.. cmake:: HTS_ENGINE_LIB

   Full path and name of HTS Engine library.
