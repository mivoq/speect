.. _cmake_options:

.. index:: 
   single: CMake API; Configuration Options

===========================
CMake Configuration Options
===========================

Several configuration options are available to customize the Speect
build system.  These options can be configured through an interactive
CMake interface as described in :doc:`../../installation`, or through the
command line.


Summary
=======


+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`CMAKE_BUILD_TYPE`	| The build type influences the C optimization and debug compilation flags. |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`CMAKE_INSTALL_PREFIX`	| The installation path for the ``make install`` command.                   |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`ERROR_ABORT_FATAL`	| Call the ISO C function ``abort()`` or gracefully exit with error log.    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`ERROR_HANDLING`	| Use Speect error handling mechanism.              	      	    	    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`LIB_SUFFIX`		| Suffix for library directories, e.g. "64".                   		    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`SAFE_CAST`		| If ON then all Speect object casting will be checked.                     |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`STRICT_WARN`		| Enable stricter compile flags.                   			    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`WANT_EXAMPLES`	| Include examples in build process.                   			    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`WANT_PYTHON_3`	| Build Python wrappers for Python version 3.x.                   	    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`WANT_PYTHON_WRAPPER`	| Include Python wrappers in build process.                   		    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`WANT_TESTS`		| Include tests in build process.                   			    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`WANT_THREADS`		| Enable multi threaded support.                   			    |
+-------------------------------+---------------------------------------------------------------------------+
| :cmake:`WARN_DECL_AFTER_STMT`	| Warn about declarations after statements (GCC).                   	    |
+-------------------------------+---------------------------------------------------------------------------+


Option Descriptions
===================

.. cmake:: CMAKE_BUILD_TYPE

   The build type influences the C optimization and debug compilation
   flags. Possible values are empty, ``Debug``, ``Release``,
   ``RelWithDebInfo``, ``MinSizeRel`` and ``Profile``. This variable
   is only supported for make based generators. If this variable is
   supported, then CMake will also provide initial values for the
   variables with the name
   ``CMAKE_C_FLAGS_[Debug|Release|RelWithDebInfo|MinSizeRel|Profile]``. For
   example, if ``CMAKE_BUILD_TYPE`` is ``Debug``, then
   ``CMAKE_C_FLAGS_DEBUG`` will be added to the ``CMAKE_C_FLAGS``.

   :options: None, Debug, Release, RelWithDebInfo, MinSizeRel, Profile
   :default: Debug
   

.. cmake:: CMAKE_INSTALL_PREFIX

   The installation path for the ``make install`` command.  If ``make
   install`` is invoked or the ``INSTALL`` target is built, then this
   directory is pre-pended onto all install directories. This variable
   defaults to ``/usr/local`` on UNIX and ``C:/Program Files`` on
   Windows.


   :default: ``/usr/local``


.. cmake:: ERROR_ABORT_FATAL

   Call the ISO C function ``abort()`` or gracefully exit with error
   log. If this option is on then :c:macro:`S_FTL_ERR` will call
   ``abort()``, otherwise Speect will try to gracefully exit with an
   error log.

   :options: ON or OFF
   :default: OFF


.. cmake:: ERROR_HANDLING

   This option specifies if the Speect error handling mechanism is used. If off then
   no error checking or logging will occur.

   :options: ON or OFF
   :default: ON


.. cmake:: LIB_SUFFIX

   Suffix for library directories, e.g. "64"

   
.. cmake:: SAFE_CAST

   Specifies whether the macros :c:macro:`S_CAST` and
   :c:macro:`S_ITERATOR_GET` will do cast checking on the given
   objects.

   :options: ON or OFF
   :default: ON


.. cmake:: STRICT_WARN  

   Enable stricter compiler flags on warnings, turning them into errors.
   
   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_EXAMPLES

   Include examples in build process.

   
   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_TESTS

   Include tests in build process.

   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_THREADS

   Enable multi-threaded support.

   :options: ON or OFF
   :default: OFF


.. cmake:: WARN_DECL_AFTER_STMT

   Warn about declarations after statements, only available for GCC.

   :options: ON or OFF
   :default: OFF


.. cmake:: WANT_PYTHON_WRAPPER

   Include Python bindings in build process.

   :options: ON or OFF
   :default: ON


.. cmake:: WANT_PYTHON_3

   Build Python wrappers for Python version 3.x

   :options: ON or OFF
   :default: OFF
   :note: Not compatible with Python version 2.x, as
          code is linked with Python 3.x libraries.


Extra Options
=============

HTS Engine
----------

The `HTS Engine <http://hts-engine.sourceforge.net/>`_ compile time options for HTS Engine plug-ins are
in the form ``HTS_ENGINE_INCLUDE_*`` and ``HTS_ENGINE_LIB_*``, where ``*`` denotes the HTS Engine version. These
options are added as plug-ins for new HTS Engine versions are added, and versions 1.03, 1.04 and 1.05 are
currently supported. For example:

.. cmake:: HTS_ENGINE_INCLUDE_103

   Path to HTS Engine version 1.03 include directory.


.. cmake:: HTS_ENGINE_LIB_103

   Full path and name to the HTS Engine version 1.03 library.

