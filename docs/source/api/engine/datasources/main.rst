.. _datasources/main:

.. index:: ! Data Sources (C API)

============
Data Sources
============

Provides a generic interface to different data sources, where a data source can
be anything that can be read from and/or written to.


Implementation
==============

.. index::
   single: Data Sources (C API); SFileSource

SFileSource
-----------

.. doxybridge:: SFilesource
   :type: speect object
   :members: none
   :inheritance: SObject.SDatasource.SFilesource
   

.. doxybridge:: SFilesourceClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SDatasource.SFilesourceClass
 

Functions
~~~~~~~~~

.. doxybridge:: SFilesourceOpenHandle
   
.. doxybridge:: SFilesourceOpenFile
   

.. index::
   single: Data Sources (C API); SMMapFileSource

SMMapFilesource
---------------

.. doxybridge:: SMMapFilesource
   :type: speect object
   :members: none
   :inheritance: SObject.SDatasource.SMMapFilesource

.. doxybridge:: SMMapFilesourceClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SDatasource.SMMapFilesourceClass
 

Functions
~~~~~~~~~

.. doxybridge:: SMMapFilesourceOpenFile
   

Abstract
========
	
.. toctree::
   :maxdepth: 2

   datasource
   datawriter
   datareader
 


