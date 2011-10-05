.. _include/build:

=============
Build Defines
=============

These defines are gathered by the build system, and may or may not
be defined. To use them one would have to test for their definitions with
the ``#ifdef`` pre-processor command.


Summary
=======

+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_MSVC`              | Defined if the compiler is some version Microsoft Visual C.  	        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_MINGW32` 	    | Defined if the compiler is a MinGW compiler in Windows.		        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_BCC32` 	    | Defined if the compiler is some version of a Borland compiler.	        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_GCC` 		    | Defined if the compiler is GNU compiler collection.		        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_WIN32` 	    | Defined if the platform is Windows (including 64-bit). 		        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_UNIX` 	  	    | Defined if the platform is UNIX or UNIX like (i.e. Apple and Cygwin).     |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_MACOSX` 	    | Defined if the platform is Apple Mac OSX.    	       	   	        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_HAVE_SYS_TYPES_H`  | Defined if the ``sys/types.h`` header is available. 		        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_HAVE_LIMITS_H`     | Defined if the ``limits.h`` header is available. 			        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_HAVE_SYS_STAT_H`   | Defined if the ``sys/stat.h`` header is available. 		        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_HAVE_UNISTD_H`     | Defined if the ``unistd.h`` header is available. 			        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_HAVE_FCNTL_H` 	    | Defined if the ``fcntl.h`` header is available. 			        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_HAVE_SYS_MMAN_H`   | Defined if the ``sys/mman.h`` header is available. 		        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_LITTLE_ENDIAN`     | Defined if the machine is little endian type. 			        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_BIG_ENDIAN` 	    | Defined if the machine is big endian type. 			        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_USE_THREADS` 	    | Defined if the CMake build option :cmake:`WANT_THREADS` is on. 	        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_ERROR_ABORT_FATAL` | Defined if the CMake build option :cmake:`ERROR_ABORT_FATAL` is on. 	|
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_ERROR_HANDLING`    | Defined if the CMake build option :cmake:`ERROR_HANDLING` is on. 	        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_DO_SAFE_CAST` 	    | Defined if the CMake build option :cmake:`SAFE_CAST` is on.  	        |
+-----------------------------------+---------------------------------------------------------------------------+
| :c:macro:`SPCT_DEBUGMODE` 	    | Defined if the CMake build option :cmake:`CMAKE_BUILD_TYPE` is ``Debug``. |
+-----------------------------------+---------------------------------------------------------------------------+


Defines
=======

Compilers
---------

.. c:macro:: SPCT_MSVC
   
   Defined if the compiler is some version Microsoft Visual C.


.. c:macro:: SPCT_MINGW32
   
   Defined if the compiler is a MinGW compiler in Windows.


.. c:macro:: SPCT_BCC32
   
   Defined if the compiler is some version of a Borland compiler.


.. c:macro:: SPCT_GCC
   
   Defined if the compiler is GNU compiler collection.


Platform
--------

.. c:macro:: SPCT_WIN32
   
   Defined if the platform is Windows (including 64-bit).

.. c:macro:: SPCT_UNIX
   
   Defined if the platform is UNIX or UNIX like (i.e. Apple and Cygwin).

.. c:macro:: SPCT_MACOSX
   
   Defined if the platform is Apple Mac OSX.


System Includes
---------------

.. c:macro:: SPCT_HAVE_SYS_TYPES_H

   Defined if the ``sys/types.h`` header is available.

.. c:macro:: SPCT_HAVE_LIMITS_H

   Defined if the ``limits.h`` header is available.

.. c:macro:: SPCT_HAVE_SYS_STAT_H

   Defined if the ``sys/stat.h`` header is available.

.. c:macro:: SPCT_HAVE_UNISTD_H

   Defined if the ``unistd.h`` header is available.

.. c:macro:: SPCT_HAVE_FCNTL_H

   Defined if the ``fcntl.h`` header is available.

.. c:macro:: SPCT_HAVE_SYS_MMAN_H

   Defined if the ``sys/mman.h`` header is available.


Machine
-------

.. c:macro:: SPCT_LITTLE_ENDIAN

   Defined if the machine is little endian type.

.. c:macro:: SPCT_BIG_ENDIAN

   Defined if the machine is big endian type.


Threads
-------

.. c:macro:: SPCT_USE_THREADS

   Defined if the CMake build option :cmake:`WANT_THREADS` is on.


Errors
------

.. c:macro:: SPCT_ERROR_ABORT_FATAL

   Defined if the CMake build option :cmake:`ERROR_ABORT_FATAL` is on.

.. c:macro:: SPCT_ERROR_HANDLING

   Defined if the CMake build option :cmake:`ERROR_HANDLING` is on.


Debugging
---------

.. c:macro:: SPCT_DEBUGMODE

   Defined if the CMake build option :cmake:`CMAKE_BUILD_TYPE` is ``Debug``


Casting
-------

.. c:macro:: SPCT_DO_SAFE_CAST

   Defined if the CMake build option :cmake:`SAFE_CAST` is on.
