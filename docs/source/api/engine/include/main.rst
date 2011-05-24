.. _include/main:

==============
System Defines
==============

..todo:: build time stuff

Standard defines for the Speect Engine.

Defines
=======

.. doxybridge:: S_BEGIN_C_DECLS
   :type: define hidden	

.. doxybridge:: S_END_C_DECLS
   :type: define hidden	

.. doxybridge:: NULL
   :type: define hidden	

.. doxybridge:: S_MAX_PATH_LENGTH
   :type: define 

.. doxybridge:: S_PATH_SEP
   :type: define hidden

.. doxybridge:: TRUE
   :type: define hidden

.. doxybridge:: FALSE
   :type: define hidden


Macros
======

.. doxybridge :: S_CONCAT
   :type: macro

.. doxybridge :: S_TOSTRING
   :type: macro

.. doxybridge :: S_VOIDPTR
   :type: macro


Visibility
==========

Macros for controlling library symbol visibility and linkage. These
macros have platform specific values. :c:type:`S_API` and :c:type:`S_LOCAL`
should be used by plug-in implementations for better portability.

.. doxybridge :: S_API
   :type: macro

.. doxybridge :: S_LOCAL
   :type: macro
