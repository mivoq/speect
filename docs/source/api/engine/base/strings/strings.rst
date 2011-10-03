.. index:: 
   single: Strings (C API); String Functions

.. _strings:

================
String Functions
================

UTF-8 aware string utilities and functions.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   s_strsize
   s_strnsize
   s_strzsize
   s_strlen
   s_strdup
   s_strzcpy
   macro s_strcpy
   macro s_strncpy
   s_strzncpy
   s_strzcat
   macro s_strcat
   macro s_strncat
   s_strzncat
   s_strcmp
   s_strncmp
   s_stricmp
   s_strnicmp
   s_strlwr
   s_strupr
   s_strchr
   s_strrchr
   s_strstr
   s_strpbrk
   s_strtok
   s_strtok_r
   s_atof
   s_strtol
   s_strtod
   s_strerror
   s_isvalid
   s_smatches
   s_safter
   s_sbefore
   s_sappend


Size/Length
===========

.. doxybridge:: s_strsize

.. doxybridge:: s_strnsize

.. doxybridge:: s_strzsize

.. doxybridge:: s_strlen


Duplicate
=========

.. doxybridge:: s_strdup


Copy
====

.. doxybridge:: s_strzcpy

.. doxybridge:: s_strcpy
   :type: macro

.. doxybridge:: s_strncpy
   :type: macro

.. doxybridge:: s_strzncpy


Concatenate
===========

.. doxybridge:: s_strzcat

.. doxybridge:: s_strcat
   :type: macro

.. doxybridge:: s_strncat
   :type: macro

.. doxybridge:: s_strzncat


Compare
=======

.. doxybridge:: s_strcmp

.. doxybridge:: s_strncmp

.. doxybridge:: s_stricmp

.. doxybridge:: s_strnicmp


Case conversion
===============

.. doxybridge:: s_strlwr

.. doxybridge:: s_strupr


Character search
================

.. doxybridge:: s_strchr

.. doxybridge:: s_strrchr


String search
=============

.. doxybridge:: s_strstr

.. doxybridge:: s_strpbrk


Token search
============

.. doxybridge:: s_strtok

.. doxybridge:: s_strtok_r


Number conversions
==================

.. doxybridge:: s_atof

.. doxybridge:: s_strtol

.. doxybridge:: s_strtod


Error to string conversion
==========================

.. doxybridge:: s_strerror


Auxiliary functions
===================

.. doxybridge:: s_isvalid

.. doxybridge:: s_smatches

.. doxybridge:: s_safter

.. doxybridge:: s_sbefore

.. doxybridge:: s_sappend
